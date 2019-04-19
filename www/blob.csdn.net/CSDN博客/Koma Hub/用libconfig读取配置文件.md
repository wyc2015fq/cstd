# 用libconfig读取配置文件 - Koma Hub - CSDN博客
2019年03月04日 21:09:12[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：30
**Table of Contents**
[demo1](#demo1)
[配置文件libconfigdemo.cfg](#%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6libconfigdemo.cfg)
[读取代码config_read_file.c](#%E8%AF%BB%E5%8F%96%E4%BB%A3%E7%A0%81config_read_file.c)
[结果：](#%E7%BB%93%E6%9E%9C%EF%BC%9A)
[demo2](#demo2)
[头文件EmbGW.h](#%E5%A4%B4%E6%96%87%E4%BB%B6EmbGW.h)
[配置文件default.cfg](#%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6default.cfg)
[源代码config.c](#%E6%BA%90%E4%BB%A3%E7%A0%81config.c)
[编译运行](#%E7%BC%96%E8%AF%91%E8%BF%90%E8%A1%8C)
# demo1
## 配置文件libconfigdemo.cfg
```
# authenticator
    
name = "JP";
enabled = false;
length = 186;
    
ldap = {
	host = "ldap.example.com";
   	base = "ou=usr,o=example.com";  /* adapt this */
   	retries = [10, 15, 20, 60];	// Use more than 2
};
```
## 读取代码config_read_file.c
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#define CONFIG_FILE "libconfigdemo.cfg"
int main(void)
{
	//the type of config
	config_t cfg, *cf;
	const config_setting_t *retries;//the setting of config
	const char *base = NULL;
	int count, n, enabled;
	cf = &cfg;
	config_init(cf); //initial config
	if (!config_read_file(cf, CONFIG_FILE)) { //read the config file
		fprintf(stderr, "%s:%d - %s\n",
			config_error_file(cf),
			config_error_line(cf),
			config_error_text(cf));
		config_destroy(cf);
		return(EXIT_FAILURE);
	}
	if (config_lookup_bool(cf, "enabled", &enabled)) //search "enabled"
		printf("Enabled: %s\n", enabled ? "Yep" : "Nope");
	else 
		printf("Enabled is not defined\n");
	if (config_lookup_string(cf, "ldap.base", &base)) //read string
		printf("Host: %s\n", base);
	retries = config_lookup(cf, "ldap.retries"); //read a array
	count = config_setting_length(retries);
	printf("I have %d retries:\n", count);
	for (n = 0; n < count; n++) {
		printf("\t#%d. %d\n", n + 1,
			config_setting_get_int_elem(retries, n));
	}
	config_destroy(cf);
	return 0;
}
```
## 结果：
```
$ ./a.out 
Enabled: Nope
Host: ou=usr,o=example.com
I have 4 retries:
	#1. 10
	#2. 15
	#3. 20
	#4. 60
```
# demo2
## 头文件EmbGW.h
```cpp
/*************************************************************************
 > File Name: EmbGW.h
 > Author: Hac
 > Mail: hac@zju.edu.cn
 > Created Time: Thu 12 May 2016 09:26:50 AM CST
 ************************************************************************/
/*
 * Macros
 */
#define MAXSERVERNUM 16
#define MAXSENSORNUM 4
#define BT          1
#define R430        2
#define BIN         3
#define HTTP        4
/*
 * Configuration
 */
unsigned int server_num = 0;
char server_name[MAXSERVERNUM][16];
char server_addr[MAXSERVERNUM][64]; // ip address or domain name of web server
unsigned int server_type[MAXSERVERNUM]; // BIN or HTTP
unsigned int sensor_num = 0;
char sensor_name[MAXSENSORNUM][16];
char sensor_dev[MAXSENSORNUM][32]; // path of serial devices
unsigned int sensor_type[MAXSENSORNUM]; // BlueTooth or Radio430
```
## 配置文件default.cfg
```
gateway: {
    upload = (
        {
            name = "server1";
            port = 8080;
            addr = "10.214.128.100";
            type = "BIN";
        },
        {
            name = "server2";
            port = 8080;
            addr = "10.214.128.112";
            type = "HTTP";
        }
    );
    download = (
        {
            name = "sensor1";
            device = "/dev/ttyAMA0";
            type = "BT";
        },
        {
            name = "sensor2";
            device = "/dev/ttyUSB0";
            type = "R430";
        }
    );
};
```
## 源代码config.c
```cpp
/*************************************************************************
 > File Name: config.c
 > Author: Hac
 > Mail: hac@zju.edu.cn
 > Created Time: Thu 12 May 2016 08:55:36 AM CST
 ************************************************************************/
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>
#include "EmbGW.h"
#define path "default.cfg"
void Load_Conf() {
    config_t cfg;
    config_setting_t *upload, *download;
    //const char *path = getenv("CONF_PATH");
    config_init(&cfg);
    /* Read the configuration file. If not exists, exit */
    if (!config_read_file(&cfg, path)) {
        fprintf(stderr, "Error: %s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        exit(1);
    }
    printf("Open file '%s' --- DONE\n", path);
    /* check UPLOAD && DOWNLOAD */
    upload = config_lookup(&cfg, "gateway.upload");
    download = config_lookup(&cfg, "gateway.download");
    if (upload == NULL || download == NULL) {
        fprintf(stderr, "Error: Incomplete configuration: you have to specify both #upload and #download.\n");
        exit(1);
    }
    /* Load configuration into global variables */
    // UPLOAD
    {
        int i;
        int count = config_setting_length(upload);
        if (count > MAXSERVERNUM) {
            printf("Warning: More than %d servers are provided, while only the first %d will be used.\n", MAXSERVERNUM
, MAXSERVERNUM);
            count = MAXSERVERNUM;
        }
        for (i = 0; i < count; ++i) {
            config_setting_t *server = config_setting_get_elem(upload, i);
            const char *name, *addr, *type;
            int port;
            if (!(config_setting_lookup_string(server, "name", &name)
                && config_setting_lookup_int(server, "port", &port)
                && config_setting_lookup_string(server, "addr", &addr)
                && config_setting_lookup_string(server, "type", &type))) {
                fprintf(stderr, "Error: Cannot resolve the %dth item in #UPLOAD field. Please review the config file.\
n", i);
                exit(1);
            }
            strncpy(server_name[server_num], name, sizeof(server_name[i-1]));
            strncpy(server_addr[server_num], addr, sizeof(server_addr[i-1]));
            if (strcmp(type, "BIN") == 0)
                server_type[server_num] = BIN;
            else if(strcmp(type, "HTTP") == 0)
                server_type[server_num] = HTTP;
            else {
                fprintf(stderr, "Error: Cannot resolve 'type = %s' in the %dth item of #UPLOAD field.\n", type, i);
                exit(1);
            }
            server_num++;
        }
    }
    // DOWNLOAD
    {
        int i;
        int count = config_setting_length(download);
        if (count > MAXSENSORNUM) {
            printf("Warning: More than %d sensors are provided, while only the first %d will be used.\n", MAXSENSORNUM
, MAXSENSORNUM);
            count = MAXSENSORNUM;
        }
        for (i = 0; i < count; ++i) {
            config_setting_t *sensor = config_setting_get_elem(download, i);
            const char *name, *device, *type;
            if (!(config_setting_lookup_string(sensor, "name", &name)
                && config_setting_lookup_string(sensor, "device", &device)
                && config_setting_lookup_string(sensor, "type", &type))) {
                fprintf(stderr, "Error: Cannot resolve the %dth item in #DOWNLOAD field. Please review the config file
.\n", i);
                exit(1);
            }
            strncpy(sensor_name[sensor_num], name, sizeof(sensor_name[i-1]));
            strncpy(sensor_dev[sensor_num], device, sizeof(sensor_dev[i-1]));
            if (strcmp(type, "BT") == 0)
                sensor_type[sensor_num] = BT;
            else if(strcmp(type, "R430") == 0)
                sensor_type[sensor_num] = R430;
            else {
                fprintf(stderr, "Error: Cannot resolve 'type = %s' in the %dth item of #DOWNLOAD field.\n", type, i);
                exit(1);
            }
            sensor_num++;
        }
    }
    printf("Load configuration --- DONE\n");
}
int main(int argc, char *argv[])
{
    Load_Conf();
    /* test */
    unsigned int i;
    for (i = 0; i < server_num; i++) {
        printf("%s %s %d\n", server_name[i], server_addr[i], server_type[i]);
    }
    for (i = 0; i < sensor_num; i++) {
        printf("%s %s %d\n", sensor_name[i], sensor_dev[i], sensor_type[i]);
    }
    return 0;
}
```
## 编译运行
```
$ gcc config.c -lconfig
$ ./a.out 
Open file 'default.cfg' --- DONE
Load configuration --- DONE
server1 10.214.128.100 3
server2 10.214.128.112 4
sensor1 /dev/ttyAMA0 1
sensor2 /dev/ttyUSB0 2
```
> 
[https://github.com/ZJUemb/Config/blob/master/config.c](https://github.com/ZJUemb/Config/blob/master/config.c)
