#ifndef _CRUST_ENCLAVE_STATUS_H_
#define _CRUST_ENCLAVE_STATUS_H_

#define COMMON_MK_ERROR(x) (0x00000000 | (x))

typedef enum _common_status_t
{
    CRUST_SUCCESS = COMMON_MK_ERROR(0),
    CRUST_MALLOC_FAILED = COMMON_MK_ERROR(0x0400),
    CRUST_SEAL_DATA_FAILED = COMMON_MK_ERROR(0x0401),
    CRUST_UNSEAL_DATA_FAILED = COMMON_MK_ERROR(0x0402),
    CRUST_STORE_DATA_TO_FILE_FAILED = COMMON_MK_ERROR(0x0403),
    CRUST_GET_DATA_FROM_FILE_FAILED = COMMON_MK_ERROR(0x0404),
    CRUST_BAD_SEAL_DATA = COMMON_MK_ERROR(0x0405),
    CRUST_SGX_FAILED = COMMON_MK_ERROR(0x0406),
    CRUST_DOUBLE_SET_VALUE = COMMON_MK_ERROR(0x0407),
    CRUST_NOT_EQUAL = COMMON_MK_ERROR(0x0408),
    CRUST_SGX_SIGN_FAILED = COMMON_MK_ERROR(0x0409),
    CRUST_SGX_VERIFY_SIG_FAILED = COMMON_MK_ERROR(0x0410),
    CRUST_UNEXPECTED_ERROR = COMMON_MK_ERROR(0x0411),
} common_status_t;

#endif /* !_CRUST_ENCLAVE_STATUS_H_ */
