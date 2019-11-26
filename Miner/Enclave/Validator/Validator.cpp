#include "Validator.h"

void ecall_validate_empty_disk(const char *path)
{
    Workload* workload = get_workload();
    size_t current_capacity = 0;
    ocall_get_folders_number_under_path(path, &current_capacity);

    for (size_t i = 0; i < (workload->all_g_hashs.size() < current_capacity ? workload->all_g_hashs.size() : current_capacity); i++)
    {
        unsigned char rand_val;
        sgx_read_rand((unsigned char *)&rand_val, 1);

        if (rand_val < 256 * VALIDATE_RATE)
        {
            // Get m hashs
            unsigned char *m_hashs = NULL;
            std::string g_path = get_g_path_with_hash(path, i, workload->all_g_hashs[i]);
            ocall_get_file(&m_hashs, get_m_hashs_file_path(g_path.c_str()).c_str(), PLOT_RAND_DATA_NUM * PLOT_HASH_LENGTH);

            if (m_hashs == NULL)
            {
                eprintf("\n!!!!USER CHEAT: GET M HASHS FAILED!!!!\n");
                return;
            }

            // Compare m hashs
            sgx_sha256_hash_t m_hashs_hash256;
            sgx_sha256_msg(m_hashs, PLOT_RAND_DATA_NUM * PLOT_HASH_LENGTH, &m_hashs_hash256);

            for (size_t j = 0; j < PLOT_HASH_LENGTH; j++)
            {
                if (workload->all_g_hashs[i][j] != m_hashs_hash256[j])
                {
                    eprintf("\n!!!!USER CHEAT: WRONG M HASHS!!!!\n");
                    return;
                }
            }

            // Get leaf data
            unsigned int rand_val_m;
            sgx_read_rand((unsigned char *)&rand_val_m, 4);
            size_t select = rand_val_m % PLOT_RAND_DATA_NUM;
            std::string leaf_path = get_leaf_path(g_path.c_str(), select, m_hashs + select * 32);
            eprintf("Select path: %s\n", leaf_path.c_str());

            unsigned char *leaf_data = NULL;
            ocall_get_file(&leaf_data, leaf_path.c_str(), PLOT_RAND_DATA_LENGTH);

            if (leaf_data == NULL)
            {
                eprintf("\n!!!!USER CHEAT: GET LEAF DATA FAILED!!!!\n");
                return;
            }

            // Compare leaf data
            sgx_sha256_hash_t leaf_data_hash256;
            sgx_sha256_msg(leaf_data, PLOT_RAND_DATA_LENGTH, &leaf_data_hash256);

            for (size_t j = 0; j < PLOT_HASH_LENGTH; j++)
            {
                if (m_hashs[select * 32 + j] != leaf_data_hash256[j])
                {
                    eprintf("\n!!!!USER CHEAT: WRONG LEAF DATA HASHS!!!!\n");
                    return;
                }
            }
        }
    }

    for (size_t i = workload->all_g_hashs.size() - 1; i > current_capacity - 1; i--)
    {
        delete[] workload->all_g_hashs[i];
        workload->all_g_hashs.pop_back();
    }

    ecall_generate_root();
}
