#ifndef _CRUST_CRUST_STATUS_H_
#define _CRUST_CRUST_STATUS_H_

#define CRUST_SEPARATOR "$crust_separator$"

#define CRUST_MK_ERROR(x) (0x00000000 | (x))

typedef enum _crust_status_t
{
    // Successed
    CRUST_SUCCESS = CRUST_MK_ERROR(0),
    // Successed with other situation
    CRUST_BLOCK_HEIGHT_EXPIRED = CRUST_MK_ERROR(0x0200),
    CRUST_FIRST_WORK_REPORT_AFTER_REPORT = CRUST_MK_ERROR(0x0201),
    CRUST_WORK_REPORT_NOT_VALIDATED = CRUST_MK_ERROR(0x0202),
    // Failed
    CRUST_MALLOC_FAILED = CRUST_MK_ERROR(0x0400),
    CRUST_SEAL_DATA_FAILED = CRUST_MK_ERROR(0x0401),
    CRUST_UNSEAL_DATA_FAILED = CRUST_MK_ERROR(0x0402),
    CRUST_STORE_DATA_TO_FILE_FAILED = CRUST_MK_ERROR(0x0403),
    CRUST_GET_DATA_FROM_FILE_FAILED = CRUST_MK_ERROR(0x0404),
    CRUST_BAD_SEAL_DATA = CRUST_MK_ERROR(0x0405),
    CRUST_SGX_FAILED = CRUST_MK_ERROR(0x0406),
    CRUST_DOUBLE_SET_VALUE = CRUST_MK_ERROR(0x0407),
    CRUST_NOT_EQUAL = CRUST_MK_ERROR(0x0408),
    CRUST_SGX_SIGN_FAILED = CRUST_MK_ERROR(0x0409),
    CRUST_SGX_VERIFY_SIG_FAILED = CRUST_MK_ERROR(0x0410),
    CRUST_UNEXPECTED_ERROR = CRUST_MK_ERROR(0x0411),
    CRUST_INVALID_MERKLETREE = CRUST_MK_ERROR(0x0412),
    CRUST_NOTFOUND_MERKLETREE = CRUST_MK_ERROR(0x0413),
    CRUST_INVALID_SEALED_DATA = CRUST_MK_ERROR(0x0414),
    CRUST_VERIFY_MEANINGFUL_FAILED = CRUST_MK_ERROR(0x0415),
    CRUST_GET_FILE_BLOCK_FAILED = CRUST_MK_ERROR(0x0416),
    CRUST_WRONG_FILE_BLOCK = CRUST_MK_ERROR(0x0417),
    CRUST_SEAL_NOTCOMPLETE = CRUST_MK_ERROR(0x0418),
    CRUST_DESER_MERKLE_TREE_FAILED = CRUST_MK_ERROR(0x0419),
    CRUST_GET_MERKLETREE_FAILED = CRUST_MK_ERROR(0x0420),
    CRUST_MERKLETREE_DUPLICATED = CRUST_MK_ERROR(0x0421),
    CRUST_MALWARE_DATA_BLOCK = CRUST_MK_ERROR(0x0422),
    CRUST_DUPLICATED_SEAL = CRUST_MK_ERROR(0x0423),
    CRUST_OPEN_FILE_FAILED = CRUST_MK_ERROR(0x0424),
    CRUST_WRITE_FILE_FAILED = CRUST_MK_ERROR(0x0425),
    CRUST_DELETE_FILE_FAILED = CRUST_MK_ERROR(0x0426),
    CRUST_RENAME_FILE_FAILED = CRUST_MK_ERROR(0x0427),
    CRUST_RENAME_FILE_NOTFOUND = CRUST_MK_ERROR(0x0428),
    CRUST_MKDIR_FAILED = CRUST_MK_ERROR(0x0429),
    CRUST_ACCESS_FILE_FAILED = CRUST_MK_ERROR(0x0430),
    CRUST_INVALID_META_DATA = CRUST_MK_ERROR(0x0431),
    CRUST_METADATA_NOTFOUND = CRUST_MK_ERROR(0x0432),

    // Persistence related
    CRUST_PERSIST_ADD_FAILED = CRUST_MK_ERROR(0x0601),
    CRUST_PERSIST_DEL_FAILED = CRUST_MK_ERROR(0x0602),
    CRUST_PERSIST_SET_FAILED = CRUST_MK_ERROR(0x0603),
    CRUST_PERSIST_GET_FAILED = CRUST_MK_ERROR(0x0604),

    // IAS report related
    CRUST_IAS_QUERY_FAILED = CRUST_MK_ERROR(0x0701),
    CRUST_IAS_OK = CRUST_MK_ERROR(0x0702),
    CRUST_IAS_VERIFY_FAILED = CRUST_MK_ERROR(0x0703),
    CRUST_IAS_BADREQUEST = CRUST_MK_ERROR(0x0704),
    CRUST_IAS_UNAUTHORIZED = CRUST_MK_ERROR(0x0705),
    CRUST_IAS_NOT_FOUND = CRUST_MK_ERROR(0x0706),
    CRUST_IAS_UNEXPECTED_ERROR = CRUST_MK_ERROR(0x0707),
    CRUST_IAS_SERVER_ERR = CRUST_MK_ERROR(0x0708),
    CRUST_IAS_UNAVAILABLE = CRUST_MK_ERROR(0x0709),
    CRUST_IAS_INTERNAL_ERROR = CRUST_MK_ERROR(0x0710),
    CRUST_IAS_BAD_CERTIFICATE = CRUST_MK_ERROR(0x0711),
    CRUST_IAS_BAD_SIGNATURE = CRUST_MK_ERROR(0x0712),
    CRUST_IAS_BAD_BODY = CRUST_MK_ERROR(0x0713),
    CRUST_IAS_REPORTDATA_NE = CRUST_MK_ERROR(0x0714),
    CRUST_IAS_GET_REPORT_FAILED = CRUST_MK_ERROR(0x0715),
    CRUST_IAS_BADMEASUREMENT = CRUST_MK_ERROR(0x0716),
    CRUST_IAS_GETPUBKEY_FAILED = CRUST_MK_ERROR(0x0717),
    CRUST_SIGN_PUBKEY_FAILED = CRUST_MK_ERROR(0x0718),
    CRUST_GET_ACCOUNT_ID_BYTE_FAILED = CRUST_MK_ERROR(0x0719),
    
    // Storage related
    CRUST_STORAGE_FILE_NOTFOUND = CRUST_MK_ERROR(0x0801),
    CRUST_STORAGE_SER_MERKLETREE_FAILED = CRUST_MK_ERROR(0x0802),
    CRUST_STORAGE_UPDATE_FILE_FAILED = CRUST_MK_ERROR(0x0803),
    CRUST_STORAGE_UNSEAL_FILE_FAILED = CRUST_MK_ERROR(0x0804),
    CRUST_STORAGE_UNEXPECTED_FILE_BLOCK = CRUST_MK_ERROR(0x0805),

    // Validation related
    CRUST_VALIDATE_INIT_WSS_FAILED = CRUST_MK_ERROR(0x0901),
    CRUST_VALIDATE_WSS_REQUEST_FAILED = CRUST_MK_ERROR(0x0902),

    // Report related
    CRUST_REPORT_NO_ORDER_FILE = CRUST_MK_ERROR(0x1001),
} crust_status_t;

#endif /* !_CRUST_CRUST_STATUS_H_ */
