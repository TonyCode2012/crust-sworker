# Crust TEE
Implement the trusted layer based on TEE technology, functionally connect  the consensus layer, and be responsible for the trusted verification of the resource layer.

## Preparation work
- Hardware requirements: 

  CPU must contain SGX module, and make sure the SGX function is turned on in the bios
- Operating system requirements:

  Unbantu 16.04

## Dependent library and project
- [Intel SGX](https://software.intel.com/en-us/sgx)
- [Crust](https://github.com/crustio/crust)
- [Crust API](https://github.com/crustio/crust-api)

## Development launch
### Install dependent libs
Install gcc, git, openssl, boost, curl,
```shell
sudo apt install build-essential
sudo apt install git
sudo apt install libboost-all-dev
sudo apt install openssl
sudo apt install libssl-dev
sudo apt install curl
```

### Package resources
Run '**scripts/package.sh**' to package whole project, you will get a **crust-\<version\>.tar** package.

### Install crust TEE
1. Run '**tar -xvf crust-\<version\>.tar**' to extract package.
1. Cd to the extract folder, run '**scripts/install.sh**' to install TEE application. Related dependencies will be installed on your machine. TEE application will be installed on '**/opt/crust/crust-tee**' directory.

### Configure your crust TEE
In /opt/crust/crust-tee/etc/Config.json file you can configure your TEE application.
```json
{
    "base_path" : "/opt/crust/crust-tee/tee_base_path",    # All files will be stored in this directory, must be absolute path
    "empty_capacity" : 4,                                                # empty disk storage in Gb
    
    "ipfs_api_base_url" : "http://127.0.0.1:5001/api/v0",                # for connect to ipfs
    "api_base_url": "http://127.0.0.1:12222/api/v0",                     # your tee node api address
    "validator_api_base_url": "http://127.0.0.1:12222/api/v0",           # the tee validator address (**if you are genesis node, this url must equal to 'api_base_url'**)

    "crust_api_base_url" : "http://127.0.0.1:56666/api/v1",              # the address of crust api
    "crust_address" : "",                                                # your crust chain identity
    "crust_account_id" : "",
    "crust_password" : "",
    "crust_backup" : "",
    ......
}
```

### Start
Crust TEE apllication is installed in /opt/crust/crust-tee.
#### Lanuch ipfs
```shell
cd /opt/crust/crust-tee
./scripts/start-ipfs.sh # please make sure your machine ports: 14001, 15001, 18080 are free
```

#### Lanuch crust TEE
```shell
cd /opt/crust/crust-tee
./bin/crust-tee --offline # if you want to run crust TEE with crust chain, please remove '--offline' flag
```

## Launch crust chain and API
Crust TEE will wait for the chain to run before uploading identity information and performing file verification. So if you want to test whole TEE flow, please lanuch crust chain and API. Please reference to [crust chain readme](https://github.com/crustio/crust) and [crust api readme](https://github.com/crustio/crust-api) .

## Client launch
### Package resources
Run '**scripts/package.sh**' to package whole project, you will get a **crust-\<version\>.tar** package.

### Launch by using crust client
Please follow [crust client](https://github.com/crustio/crust-client) to launch.

## Crust tee executable file
1. Run '**bin/crust-tee --help**' to show how to use **crust-tee**.
1. Run '**bin/crust-tee \<argument\>**' to run crust-tee in different mode, argument can be daemon/server/status/report.
   1. **daemon** option lets tee run in daemon mode.
   1. **server** option lets tee run in server mode.
   1. **status** option shows tee current status, make sure daemon or server mode has been running.
   1. **report** option shows tee work report, make sure daemon or server mode has been running.
1. Run '**bin/crust-tee --config \<config_file_path\>**' to use customized configure file, if not provided **etc/Config.json** will be used as the default one.
1. Run '**bin/crust-tee --offline**', program will not interact with the chain.

## API
- Use 'curl http://<api_base_url_in_Config.json>/api/v0/status' to get validation status
- Use 'curl http://<api_base_url_in_Config.json>/api/v0/report' to get work report

## Contribution

Thank you for considering to help out with the source code! We welcome contributions from anyone on the internet, and are grateful for even the smallest of fixes!

If you'd like to contribute to crust, please **fork, fix, commit and send a pull request for the maintainers to review and merge into the main codebase**.

### Rules

Please make sure your contribution adhere to our coding guideliness:

- **No --force pushes** or modifying the master branch history in any way. If you need to rebase, ensure you do it in your own repo.
- Pull requests need to be based on and opened against the `master branch`.
- A pull-request **must not be merged until CI** has finished successfully.
- Make sure your every `commit` is [signed](https://help.github.com/en/github/authenticating-to-github/about-commit-signature-verification)

### Merge process

Merging pull requests once CI is successful:

- A PR needs to be reviewed and approved by project maintainers;
- PRs that break the external API must be tagged with [`breaksapi`](https://github.com/crustio/crust-tee/labels/breakapi);
- No PR should be merged until **all reviews' comments** are addressed.

## License

[GPL v3](LICENSE)
