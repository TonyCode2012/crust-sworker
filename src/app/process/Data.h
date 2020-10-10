#ifndef _APP_DATA_H_
#define _APP_DATA_H_

#include <stdio.h>
#include <string>

#if defined(__cplusplus)
extern "C"
{
#endif

std::string get_g_sworker_identity();
void set_g_sworker_identity(std::string identity);
std::string get_g_order_report();
void set_g_order_report(std::string order_report);
std::string get_g_enclave_id_info();
void set_g_enclave_id_info(std::string id_info);
std::string get_g_enclave_workload();
void set_g_enclave_workload(std::string workload);
std::string get_g_enclave_workreport();
void set_g_enclave_workreport(std::string workreport);
std::string get_g_new_karst_url();
void set_g_new_karst_url(std::string karst_url);

#if defined(__cplusplus)
}
#endif

#endif /* !_APP_DATA_H_ */
