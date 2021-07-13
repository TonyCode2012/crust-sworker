#ifndef _CRUST_VALIDATOR_H_
#define _CRUST_VALIDATOR_H_

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <algorithm>

#include "sgx_thread.h"
#include "sgx_trts.h"

#include "Identity.h"
#include "Srd.h"
#include "MerkleTree.h"
#include "Workload.h"
#include "PathHelper.h"
#include "Persistence.h"
#include "EUtils.h"
#include "Parameter.h"
#include "SafeLock.h"
#include "Storage.h"

#define VALIDATE_PROOF_MAX_NUM 2

class Validator
{
public:
    static Validator *validator;
    static Validator *get_instance();
    void validate_srd();
    void validate_srd_real();
    void validate_meaningful_file();
    void validate_meaningful_file_real();
    void report_add_validated_srd_proof();
    void report_add_validated_file_proof();
    void report_reset_validated_proof();
    bool report_has_validated_proof();

private:
    Validator() {}
    // Srd related variables
    std::vector<uint8_t *> del_srd_v;
    sgx_thread_mutex_t del_srd_v_mutex = SGX_THREAD_MUTEX_INITIALIZER;
    std::map<int, uint8_t *> validate_srd_m;
    std::map<int, uint8_t *>::const_iterator validate_srd_m_iter;
    sgx_thread_mutex_t validate_srd_m_iter_mutex = SGX_THREAD_MUTEX_INITIALIZER;
    uint32_t validated_srd_num = 0;
    sgx_thread_mutex_t validated_srd_num_mutex = SGX_THREAD_MUTEX_INITIALIZER;
    // File related method and variables
    std::vector<json::JSON *> changed_files_v;
    sgx_thread_mutex_t changed_files_v_mutex = SGX_THREAD_MUTEX_INITIALIZER;
    std::map<std::string, json::JSON> validate_files_m;
    std::map<std::string, json::JSON>::const_iterator validate_files_m_iter;
    sgx_thread_mutex_t validate_files_m_iter_mutex = SGX_THREAD_MUTEX_INITIALIZER;
    uint32_t validated_files_num = 0;
    sgx_thread_mutex_t validated_files_num_mutex = SGX_THREAD_MUTEX_INITIALIZER;
    uint32_t validate_random = 0;
    sgx_thread_mutex_t validate_random_mutex = SGX_THREAD_MUTEX_INITIALIZER;

    int validated_srd_proof = 0; // Generating workreport will decrease this value, while validating will increase it
    sgx_thread_mutex_t validated_srd_mutex = SGX_THREAD_MUTEX_INITIALIZER;
    int validated_file_proof = 0; // Generating workreport will decrease this value, while validating will increase it
    sgx_thread_mutex_t validated_file_mutex = SGX_THREAD_MUTEX_INITIALIZER;
};

#endif /* !_CRUST_VALIDATOR_H_ */
