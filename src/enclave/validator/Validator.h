#ifndef _CRUST_VALIDATOR_H_
#define _CRUST_VALIDATOR_H_

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include "sgx_thread.h"
#include "sgx_trts.h"
#include "MerkleTree.h"
#include "Workload.h"
#include "PathHelper.h"
#include "Persistence.h"
#include "EUtils.h"
#include "Parameter.h"
#include "SafeLock.h"

void validate_srd();
void validate_meaningful_file();

#endif /* !_CRUST_VALIDATOR_H_ */
