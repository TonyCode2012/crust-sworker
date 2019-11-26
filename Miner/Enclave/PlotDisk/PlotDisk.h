#ifndef _CRUST_PLOT_DISK_H_
#define _CRUST_PLOT_DISK_H_

#include "../Enclave.h"
#include "../Utils/FormatHelper.h"
#include <vector>
#include <string>
#include "sgx_trts.h"
#include "sgx_thread.h"

#define PLOT_RAND_DATA_LENGTH 1048576
#define PLOT_RAND_DATA_NUM 1024
#define PLOT_HASH_LENGTH 32
#define PLOT_M_HASHS "m-hashs.bin"

#define VALIDATE_RATE 0.25

sgx_thread_mutex_t g_mutex = SGX_THREAD_MUTEX_INITIALIZER;
std::vector<unsigned char *> all_g_hashs;
sgx_sha256_hash_t root_hash;
size_t empty_disk_capacity = 0;

std::string get_g_path(const char *dir_path, const size_t now_index);
std::string get_leaf_path(const char *g_path, const size_t now_index, const unsigned char *hash);
std::string get_g_path_with_hash(const char *dir_path, const size_t now_index, const unsigned char *hash);
std::string get_m_hashs_file_path(const char *g_path);
void save_file(const char *g_path, size_t index, sgx_sha256_hash_t hash, const unsigned char *data, size_t data_size);
void save_m_hashs_file(const char *g_path, const unsigned char *data, size_t data_size);

#endif /* !_CRUST_PLOT_DISK_H_ */
