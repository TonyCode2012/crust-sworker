#include "Storage.h"

using namespace std;

// Old tree root hash to pair which consists of serialized old tree and leaf position and total size
map<vector<char>, tuple<string,size_t,size_t>> tree_meta_map;
// Map used to store sealed tree root to sealed tree root node
map<vector<char>, MerkleTree *> new_tree_map;

// Current node public and private key pair
extern ecc_key_pair id_key_pair;

common_status_t _validate_meaningful_data(MerkleTree *root, MerkleTree *cur_node, vector<uint32_t> path);

/**
 * @description: Validate merkle tree and storage tree related meta data
 * @param tree -> Pointer to Merkle tree root node
 * @return: Validate status
 * */
common_status_t storage_validate_merkle_tree(MerkleTree *tree)
{
    if (CRUST_SUCCESS != validate_merkle_tree_c(tree))
    {
        return CRUST_INVALID_MERKLETREE;
    }

    // Serialize Merkle tree
    string ser_tree = storage_ser_merkle_tree(tree);

    // Record position of first leaf node
    size_t spos = ser_tree.find(LEAF_SEPARATOR);

    // Get vector of root hash
    vector<char> hash_v(tree->hash, tree->hash + HASH_LENGTH);

    // Record merkle tree metadata
    tree_meta_map[hash_v] = make_tuple(ser_tree, spos, 0);

    return CRUST_SUCCESS;
}

/**
 * @description: Seal file block and generate new tree
 * @param root_hash -> file root hash
 * @param root_hash_len -> file root hash lenght
 * @param p_src -> pointer to file block data
 * @param src_len -> file block data size
 * @param p_sealed_data -> sealed file block data
 * @param sealed_data_size -> sealed file block data size
 * @return: Seal and generate result
 * */
common_status_t storage_seal_file_data(const char *root_hash, uint32_t root_hash_len,
        const uint8_t *p_src, size_t src_len, uint8_t *p_sealed_data, size_t sealed_data_size)
{
    /* Get merkle tree metadata */
    vector<char> root_hash_v(root_hash, root_hash + root_hash_len);

    if (tree_meta_map.find(root_hash_v) == tree_meta_map.end())
    {
        return CRUST_NOTFOUND_MERKLETREE;
    }
    auto entry = tree_meta_map[root_hash_v];

    /* Verify file block hash */
    sgx_sha256_hash_t cur_hash;
    sgx_sha256_msg(p_src, src_len, &cur_hash);

    string ser_tree = std::get<0>(entry);
    size_t spos = std::get<1>(entry) + strlen(LEAF_SEPARATOR);
    size_t file_size = std::get<2>(entry);
    // Compare hash value
    for (size_t i = 0; i < root_hash_len; i++)
    {
        if (ser_tree[spos + i] != cur_hash[i])
        {
            return CRUST_WRONG_FILE_BLOCK;
        }
    }
    
    /* Seal original file block */
    uint32_t pri_key_len = sizeof(id_key_pair.pri_key);
    uint32_t src_r_len = sizeof(uint32_t) * 2 + src_len + pri_key_len;
    uint8_t *p_src_r = (uint8_t*)malloc(src_r_len);
    uint8_t *pp = p_src_r;
    memset(p_src_r, 0, src_r_len);
    // Get source data
    memcpy(pp, &src_len, sizeof(uint32_t));
    pp += sizeof(uint32_t);
    memcpy(pp, &pri_key_len, sizeof(uint32_t));
    pp += sizeof(uint32_t);
    memcpy(pp, p_src, src_len);
    pp += src_len;
    memcpy(pp, &id_key_pair.pri_key, sizeof(id_key_pair.pri_key));
    // Get sealed data
    uint8_t *p_sealed_data_r = NULL;
    common_status_t common_status = seal_data_mrenclave(p_src_r, src_r_len, 
                (sgx_sealed_data_t**)&p_sealed_data_r, &sealed_data_size);
    if (CRUST_SUCCESS != common_status || p_sealed_data_r == NULL)
    {
        return CRUST_SEAL_DATA_FAILED;
    }
    memcpy(p_sealed_data, p_sealed_data_r, sealed_data_size);

    /* Set new metadata */
    sgx_sha256_hash_t cur_new_hash;
    sgx_sha256_msg(p_sealed_data_r, sealed_data_size, &cur_new_hash);
    // Set new node hash
    for (int i = 0; i < HASH_LENGTH; i++)
    {
        ser_tree[i + spos] = cur_new_hash[i];
    }
    // Set new start position
    spos = ser_tree.find(LEAF_SEPARATOR, spos);
    tree_meta_map[root_hash_v] = make_tuple(ser_tree, spos, file_size + sealed_data_size);


    free(p_sealed_data_r);

    return CRUST_SUCCESS;
}

/**
 * @description: Unseal and verify file block data
 * @param p_sealed_data -> sealed file block data
 * @param sealed_data_size -> sealed file block data size
 * @param p_unsealed_data -> unsealed file block data
 * @param unsealed_data_size -> unsealed file block data size
 * @return: Unseal status
 * */
common_status_t storage_unseal_file_data(const uint8_t *p_sealed_data, size_t sealed_data_size,
        uint8_t *p_unsealed_data, uint32_t unsealed_data_size)
{
    common_status_t common_status = CRUST_SUCCESS;
    sgx_status_t sgx_status = SGX_SUCCESS;
    uint8_t *p_unsealed_pri_key = NULL;
    uint32_t src_len = 0;
    uint32_t pri_key_len = 0;

    /* Unseal file block */
    // Create buffer in enclave
    sgx_sealed_data_t *p_sealed_data_r = (sgx_sealed_data_t *)malloc(sealed_data_size);
    memset(p_sealed_data_r, 0, sealed_data_size);
    memcpy(p_sealed_data_r, p_sealed_data, sealed_data_size);
    // Create buffer for decrypted data
    uint32_t decrypted_data_len = sgx_get_encrypt_txt_len(p_sealed_data_r);
    uint8_t *p_decrypted_data = (uint8_t *)malloc(decrypted_data_len);
    sgx_status = sgx_unseal_data(p_sealed_data_r, NULL, NULL,
            p_decrypted_data, &unsealed_data_size);
    if (SGX_SUCCESS != sgx_status)
    {
        common_status = CRUST_UNSEAL_DATA_FAILED;
        goto cleanup;
    }

    /* Deserialize file block related data */
    memcpy(&src_len, p_decrypted_data, sizeof(uint32_t));
    if (src_len != unsealed_data_size)
    {
        common_status = CRUST_INVALID_SEALED_DATA;
        goto cleanup;
    }
    p_decrypted_data += sizeof(uint32_t);
    memcpy(&pri_key_len, p_decrypted_data + sizeof(uint32_t), sizeof(uint32_t));
    if (pri_key_len != sizeof(id_key_pair.pri_key))
    {
        common_status = CRUST_INVALID_SEALED_DATA;
        goto cleanup;
    }
    p_decrypted_data += sizeof(uint32_t);
    // Verify unsealed data
    memcpy(p_unsealed_data, p_decrypted_data, src_len);
    p_decrypted_data += src_len;
    p_unsealed_pri_key = (uint8_t *)malloc(pri_key_len);
    memset(p_unsealed_pri_key, 0, pri_key_len);
    memcpy(p_unsealed_pri_key, p_decrypted_data, pri_key_len);
    if (memcmp(p_unsealed_pri_key, &id_key_pair.pri_key, pri_key_len) != 0)
    {
        common_status = CRUST_INVALID_SEALED_DATA;
        goto cleanup;
    }

cleanup:

    return common_status;
}

/**
 * @description: Validate meaningful data
 * @return: Validate result
 * */
common_status_t storage_validate_meaningful_data()
{
    unsigned char rand_val;
    for (auto it : new_tree_map)
    {
        sgx_read_rand((unsigned char *)&rand_val, 1);
        if (rand_val % 2 == 0)
        {
            MerkleTree *root = it.second;
            vector<uint32_t> path_v;
            if (CRUST_SUCCESS != _validate_meaningful_data(root, root, path_v))
            {
                return CRUST_VERIFY_MEANINGFUL_FAILED;
            }
        }
    }

    return CRUST_SUCCESS;
}

/**
 * @description: Real validate meaningful data function
 * @param root -> To be validated merkle tree root node
 * @param cur_node -> Validating merkle tree node
 * @param path -> Path from root node to leaf node
 * @return: Validate result
 * */
common_status_t _validate_meaningful_data(MerkleTree *root, MerkleTree *cur_node, vector<uint32_t> path)
{
    if (cur_node == NULL)
    {
        return CRUST_SUCCESS;
    }

    // Validate leaf node hash value
    if (cur_node->links_num == 0)
    {
        char *cur_hash = (char*)malloc(HASH_LENGTH);
        common_status_t common_status = CRUST_SUCCESS;
        ocall_get_file_block_by_path(&common_status, root->hash, cur_hash, HASH_LENGTH, path.data(), path.size());
        if (CRUST_SUCCESS != common_status)
        {
            return CRUST_GET_FILE_BLOCK_FAILED;
        }
        if (memcmp(cur_hash, cur_node->hash, HASH_LENGTH) != 0)
        {
            return CRUST_INVALID_SEALED_DATA;
        }
    }

    // Validate file block recursively
    unsigned char rand_val;
    set<uint32_t> validated_idx;
    for (int i = 0; i < 3; i++)
    {
        sgx_read_rand((unsigned char *)&rand_val, 1);
        uint32_t idx = rand_val % cur_node->links_num;
        if (validated_idx.find(idx) != validated_idx.end())
        {
            continue;
        }
        path.push_back(idx);
        if (CRUST_SUCCESS != _validate_meaningful_data(root, cur_node->links[idx], path))
        {
            return CRUST_INVALID_SEALED_DATA;
        }
        path.pop_back();
    }

    return CRUST_SUCCESS;
}

/**
 * @description: Transfer a Merkle tree to valid hash tree
 * @param tree -> Pointer to Merkle tree root node
 * */
void storage_gen_validated_merkle_tree(MerkleTree *tree)
{
    if (tree == NULL || tree->links_num == 0)
    {
        return;
    }

    uint8_t *g_hashs = (uint8_t*)malloc(tree->links_num * HASH_LENGTH);
    for (uint32_t i = 0; i < tree->links_num; i++)
    {
        storage_gen_validated_merkle_tree(tree->links[i]);
        memcpy(g_hashs + i * HASH_LENGTH, tree->links[i]->hash, HASH_LENGTH);
    }

    sgx_sha256_hash_t g_hashs_hash256;
    sgx_sha256_msg(g_hashs, tree->links_num * HASH_LENGTH, &g_hashs_hash256);

    tree->hash = (char*)&g_hashs_hash256;
}

/**
 * @description: Serialize Merkle tree
 * @param tree -> root of Merkle tree
 * @return: Serialized string
 * */
string storage_ser_merkle_tree(MerkleTree *tree)
{
    if (tree == NULL)
    {
        return "";
    }

    std::string ans;

    if (tree->links_num == 0)
    {
        ans.append(LEAF_SEPARATOR).append(tree->hash);
    }

    ans.append("{")
       .append(to_string(tree->size))
       .append(",")
       .append(to_string(tree->links_num))
       .append(",");

    if (tree->links_num != 0)
    {
        for (size_t i = 0; i < tree->links_num; i++)
        {
            ans.append(storage_ser_merkle_tree(tree->links[i]))
               .append(",");
        }
    }

    ans.append("}");

    return ans;
}

/**
 * @description: Deserialize Merkle tree
 * @param father -> root of Merkle tree
 * @param ser_tree -> serialized Merkle tree
 * @param spos -> start position of deserialized string
 * @return: Root value of Merkle tree
 * */
common_status_t storage_deser_merkle_tree(MerkleTree *father, string ser_tree, size_t &spos)
{
    if (spos >= ser_tree.size())
    {
        return CRUST_SUCCESS;
    }

    // Get hash
    size_t epos = ser_tree.find("{", spos);
    string hash = ser_tree.substr(spos, epos - spos);
    const char* p_hash = hash.c_str() + strlen(LEAF_SEPARATOR);
    father->hash = (char*)malloc(HASH_LENGTH);
    memcpy(father->hash, p_hash, HASH_LENGTH);

    // Get size
    spos = epos + 1;
    epos = ser_tree.find(",", spos);
    size_t size = atoi(ser_tree.substr(spos, epos - spos).c_str());
    father->size = size;

    // Get links_num
    spos = epos + 1;
    epos = ser_tree.find(",", spos);
    size_t links_num = atoi(ser_tree.substr(spos, epos - spos).c_str());
    father->links_num = links_num;

    /* Get children nodes */
    spos = epos + 1;
    if (father->links_num != 0)
    {
        father->links = (MerkleTree**)malloc(father->links_num * sizeof(MerkleTree*));
    }
    else
    {
        while (spos < ser_tree.size() && (ser_tree[spos] == '}' || ser_tree[spos] == ',')) spos++;
        return CRUST_SUCCESS;
    }
    // Deserialize merkle tree recursively
    for (size_t i = 0; i < father->links_num; i++)
    {
        MerkleTree *child = new MerkleTree();
        storage_deser_merkle_tree(child, ser_tree, spos);
        father->links[i] = child;
    }

    return CRUST_SUCCESS;
}
