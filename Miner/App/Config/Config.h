#ifndef _CRUST_CONFIG_H_
#define _CRUST_CONFIG_H_

#include <stdio.h>
#include <string>
#include <fstream>
#include <cpprest/json.h>

class Config
{
public:
    std::string empty_path;        /* Empty validation files base path */
    size_t empty_capacity;         /* Hard drive storage space for empty validation files, The unit is GB */
    std::string ipfs_api_base_url; /* Used to connect to IPFS */
    std::string api_base_url;      /* External API base url */

    Config(std::string path);
    void show(void);
};

Config *new_config(const char *path);
Config *get_config(void);

#endif /* !_CRUST_CONFIG_H_ */
