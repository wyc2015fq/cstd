# 服务器数据库系列 - Nginx HTTPS配置 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年05月19日 17:46:21[initphp](https://me.csdn.net/initphp)阅读数：1235








同事测试一ssl加密接口，但是负责该接口的同事有事请假了没在，所以我就临时给配置了一个https服务，写了一个简单接口供同事使用，配置nginx的https记录一下：

一、生成私钥和证书

创建带密钥口令的私钥

root@mysqlmaster:/tmp# openssl genrsa -des3 -out ng.key 1024

Generating RSA private key, 1024 bit long modulus

........++++++

...........................................++++++

e is 65537 (0x10001)

Enter pass phrase for ng.key:       输入口令

Verifying - Enter pass phrase for ng.key:   确认口令

二、创建csr文件

root@mysqlmaster:/tmp# openssl req -new -key ng.key -out ng.csr

Enter pass phrase for ng.key:

You are about to be asked to enter information that will be incorporated

into your certificate request.

What you are about to enter is what is called a Distinguished Name or a DN.

There are quite a few fields but you can leave some blank

For some fields there will be a default value,

If you enter '.', the field will be left blank.

-----

Country Name (2 letter code) [AU]:国家 如cn，hk

State or Province Name (full name) [Some-State]:州或省的名称 如Beijing

Locality Name (eg, city) []:什么地方级别，是城市还是乡镇

Organization Name (eg, company) [Internet Widgits Pty Ltd]:什么组织，如公司，政府

Organizational Unit Name (eg, section) []:组织单位名称

Common Name (eg, YOUR name) []:名字

Email Address []:邮件地址


Please enter the following 'extra' attributes 额外信息

to be sent with your certificate request

A challenge password []: 复杂密码

An optional company name []：


1，创建私钥（去除密钥口令）

openssl rsa -in ng.key -out server.key

输入口令


2，创建CA证书

openssl req -new -x509 -days 3650 -key server.key -out server.crt

You are about to be asked to enter information that will be incorporated

into your certificate request.

What you are about to enter is what is called a Distinguished Name or a DN.

There are quite a few fields but you can leave some blank

For some fields there will be a default value,

If you enter '.', the field will be left blank.

-----

Country Name (2 letter code) [AU]:

State or Province Name (full name) [Some-State]:

Locality Name (eg, city) []:

Organization Name (eg, company) [Internet Widgits Pty Ltd]:

Organizational Unit Name (eg, section) []:

Common Name (eg, YOUR name) []:

Email Address []:

和上面的步骤差不多，根据提示输入证书的信息，国家，管理人邮件，姓名，城市等



三、修改nginx配置文件

root@mysqlmaster:/tmp# cp server.crt server.key /etc/nginx/

默认需要证书放到nginx/conf/目录，那么

 vi /etc/nginx/nginx.conf


增加


   server{

       listen      443;

       server_name  localhost;

       ssl                 on;

       ssl_certificate     server.crt;  #证书

       ssl_certificate_key  server.key; #私钥

       location / {

           root   html;

           index  index.html index.htm;

       }

    }

root@mysqlmaster:/tmp# service nginx restart

重启服务
            


