#ifndef _CRUST_APP_H_
#define _CRUST_APP_H_

#include <stdio.h>
#include <string>
#include <unistd.h>
#include "sgx_error.h"
#include "sgx_eid.h"
#include "sgx_urts.h"
#include "Enclave_u.h"
#include "Config/Config.h"
#include "Http/ApiHandler.h"
#include "Ipfs/Ipfs.h"
#include "OCalls/OCalls.h"
#include "ValidationStatus.h"

# define TOKEN_FILENAME   "enclave.token"
# define ENCLAVE_FILENAME "enclave.signed.so"

extern sgx_enclave_id_t global_eid;

#endif /* !_CRUST_APP_H_ */
