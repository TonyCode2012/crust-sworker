#include "Config.h"

Config *config = NULL;

/**
 * @description: new a global config
 * @param path -> configurations file path
 * @return: new config point
 */
Config *new_config(const char *path)
{
    if (config != NULL)
    {
        delete config;
    }

    config = new Config(path);
    return config;
}

/**
 * @description: get the global config
 * @return: config point
 */
Config *get_config(void)
{
    if (config == NULL)
    {
        printf("Please use new_config(path) frist.\n");
        exit(-1);
    }

    return config;
}

/**
 * @description: constructor
 * @param path -> configurations file path 
 */
Config::Config(std::string path)
{
    /* Read user configurations from file */
    std::ifstream config_ifs(path);
    std::string config_str((std::istreambuf_iterator<char>(config_ifs)), std::istreambuf_iterator<char>());

    /* Fill configurations */
    web::json::value config_value = web::json::value::parse(config_str);
    this->empty_path = config_value["emptyPath"].as_string();
    this->ipfs_api_base_url = config_value["ipfsApiBaseUrl"].as_string();
    this->api_base_url = config_value["apiBaseUrl"].as_string();
    this->empty_capacity = (size_t)config_value["emptyCapacity"].as_integer();
}

/**
 * @description: show configurations
 */
void Config::show(void)
{
    printf("Config:\n{\n");
    printf("    'empty path' : '%s',\n", this->empty_path.c_str());
    printf("    'empty capacity' : %lu,\n", this->empty_capacity);
    printf("    'ipfs api base url' : '%s',\n", this->ipfs_api_base_url.c_str());
    printf("    'api base url' : '%s',\n", this->api_base_url.c_str());
    printf("}\n");
}
