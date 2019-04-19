# c配置库ccl使用小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年04月25日 09:24:31[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：314
配置文件为key=value键值对形式
## 下载与安装
库文件下载：ccl-0.1.1.tar.gz
安装：
 tar -zxvf ccl-0.1.1.tar.gz 
cd ccl-0.1.1
.**/**configure --prefix=**/**usr**/****local****make  && ****make ****install**
 ldconfig
## 示例
新建一个test.conf配置文件，内容如下：
## Sample configuration file 
Desktop-Picture**=** /usr/images/earth.jpg Position**=** Centered "Background Color"**=** Black
测试源码：
*/* * test.c * Author: elesos.com */*#include "ccl/ccl.h"#include <stdio.h>int main(int argc,char**argv){struct ccl_t            config;conststruct ccl_pair_t *iter;char*szjpg = NULL;*/* Set configuration file details */*
	config.comment_char='#';
	config.sep_char='=';
	config.str_char='"';*/* Parse the file */*if(0!= ccl_parse(&config,"test.conf")){printf("parse configuration file error!**\n**");}*/* Iterate through all key/value pairs */*while((iter = ccl_iterate(&config))!=0){printf("%s=%s**\n**", iter->key, iter->value);}
 
	szjpg = ccl_get(&config,"Desktop-Picture");printf("Picture=%s**\n**", szjpg);*/* Clean up */*
	ccl_release(&config);}
源码与配置文件打包下载：[艺搜下载](http://dl.vmall.com/c0lt0r4g24)
程序输出：
Background Color=Black
Desktop-Picture=**/**usr**/**images**/**earth.jpg
Position=Centered
Picture=**/**usr**/**images**/**earth.jpg
