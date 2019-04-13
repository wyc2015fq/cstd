
# ubuntu的apache配置https - 高科的专栏 - CSDN博客

2018年04月20日 09:44:14[高科](https://me.csdn.net/pbymw8iwm)阅读数：142标签：[https																](https://so.csdn.net/so/search/s.do?q=https&t=blog)[apache																](https://so.csdn.net/so/search/s.do?q=apache&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=https&t=blog)个人分类：[开发环境的配置																](https://blog.csdn.net/pbymw8iwm/article/category/1219848)[游戏																](https://blog.csdn.net/pbymw8iwm/article/category/1252249)[
							](https://blog.csdn.net/pbymw8iwm/article/category/1219848)


﻿﻿
一、配置Apache
1、开启SSL模块
```python
a2enmod ssl
```
2、启用SSL站点
```python
a2ensite
```
```python
default
```
```python
-
```
```python
ssl
```
3、加入监听端口
```python
vi
```
```python
/
```
```python
etc
```
```python
/
```
```python
apache2
```
```python
/
```
```python
ports
```
```python
.
```
```python
conf
```
```python
#编辑Apache端口配置，加入443端口
```
```python
Listen
```
```python
443
```
注：Ubuntu启用SSL站点后，缺省已经加入了
##### ( 1 ) 在Apache的安装目录下创建cert目录，并且将下载的全部文件拷贝到cert目录中。如果申请证书时是自己创建的CSR文件，请将对应的私钥文件放到cert目录下并且命名为214261584760237.key；
##### ( 2 ) 打开 apache 安装目录下 conf 目录中的 httpd.conf 文件，找到以下内容并去掉“\#”：
\#LoadModule ssl_module modules/mod_ssl.so (如果找不到请确认是否编译过 openssl 插件)
\#Include conf/extra/httpd-ssl.conf
##### ( 3 ) 打开 apache 安装目录下 conf/extra/httpd-ssl.conf 文件 (也可能是conf.d/ssl.conf，与操作系统及安装方式有关)， 在配置文件中查找以下配置语句:
\# 添加 SSL 协议支持协议，去掉不安全的协议
SSLProtocol all -SSLv2 -SSLv3
\# 修改加密套件如下
SSLCipherSuite HIGH:!RC4:!MD5:!aNULL:!eNULL:!NULL:!DH:!EDH:!EXP:+MEDIUM
SSLHonorCipherOrder on
\# 证书公钥配置
SSLCertificateFile cert/public.pem
\# 证书私钥配置
SSLCertificateKeyFile cert/214261584760237.key
\# 证书链配置，如果该属性开头有 '\#'字符，请删除掉
SSLCertificateChainFile cert/chain.pem


