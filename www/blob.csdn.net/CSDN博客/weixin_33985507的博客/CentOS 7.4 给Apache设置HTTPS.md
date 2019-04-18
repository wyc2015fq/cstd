# CentOS 7.4 给Apache设置HTTPS - weixin_33985507的博客 - CSDN博客
2018年06月26日 19:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
> 
因为我配置了虚拟服务器，所以比正常配置多一步内容。
##### 1. 安装
```bash
yum install mod_ssl openssl
```
##### 2. 导入证书
```bash
cd /etc/httpd/
mkdir cert
# 拷贝证书文件xxx.pem、私钥文件xxx.key、公钥文件public.pem、证书链文件chain.pem到cert目录下
```
##### 3. 配置ssl
```bash
vim /etc/httpd/conf.d/ssl.conf
# 添加 SSL 协议支持协议，去掉不安全的协议
SSLProtocol all -SSLv2 -SSLv3
# 修改加密套件如下
SSLCipherSuite HIGH:!RC4:!MD5:!aNULL:!eNULL:!NULL:!DH:!EDH:!EXP:+MEDIUM
SSLHonorCipherOrder on
# 证书公钥配置
SSLCertificateFile cert/public.pem
# 证书私钥配置
SSLCertificateKeyFile cert/214758805480726.key
# 证书链配置，如果该属性开头有 '#'字符，请删除掉
SSLCertificateChainFile cert/chain.pem
```
##### 4. 配置虚拟主机VirtualHost
```bash
vim /etc/httpd/conf.d/httpd-vhosts.conf
# 增加配置，80端口强制跳转443：
<VirtualHost *:80>
    ServerName xx.ont.io
    DocumentRoot /var/www/html/**/
    RewriteEngine on
    RewriteCond %{SERVER_PORT} !^443$
    RewriteRule ^(.*)?$ https://%{SERVER_NAME}/$1 [L,R]
    <Directory "/var/www/html/**/">
        Options none
        AllowOverride none
        Order allow,deny
        Allow from all
        Require all granted
    </Directory>
</VirtualHost>
<VirtualHost *:443>
    ServerName xx.ont.io
    DocumentRoot /var/www/html/**/
    SSLEngine on
    SSLCertificateFile /etc/httpd/cert/public.pem
    SSLCertificateKeyFile /etc/httpd/cert/xxx.key
    <Directory "/var/www/html/**/">
        Options FollowSymLinks
        AllowOverride All
        Require all granted
    </Directory>
</VirtualHost>
```
The end.
