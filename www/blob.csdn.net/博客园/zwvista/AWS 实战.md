# AWS 实战 - zwvista - 博客园

## [AWS 实战](https://www.cnblogs.com/zwvista/p/9282009.html)

### AWS （Amazon Web Service）

要使用亚马逊的免费云服务 AWS，必须先注册账号，然后绑定信用卡。

### 创建 AMI（Amazon Machine Image）
- 选择 EC2 服务
- EC2 Management Console（EC2 Dashboard）

打开 EC2 虚拟服务器的管理控制台（默认打开仪表盘）
- Launch Instance

点击创建实例按钮
- Amazon Machine Image

第一步选择 AMI 映像的类型，这里选择可免费的 Amazon Linux AMI。
- Instance Type

第二步选择实例类型，这里选择唯一可免费的 t2.micro。
- 确认并创建实例。

一个实例就是一个单独的虚拟服务器。
- 创建实例时如果创建新的密钥对 xxx，需要将私钥文件下载到本地 xxx.pem。

私钥文件如果丢失，就不再能连接服务器，所以需要妥善保存。

### EC2 管理控制台

在管理控制台里可选择查看
- EC2 仪表盘

显示各种统计数据。
- Instances（实例）

实例可以启动，停止，终止。

终止的实例不能再次启动，默认情况下会被删除。
- AMIs（映像）

快照可以用来制作映像，映像可以用来制作实例的备份。
- Volumes（卷）

实例在物理上存放于卷之上。
- Snapshots（快照）

实例可以制作快照，作备份之用。
- Key Pairs（密钥对）

在这里创建或删除连接实例所需要的密钥对。
- Security Groups（安全组）

在这里配置安全组（Inbound）

默认 Type: SSH Port Range:22

需要添加

Type: HTTP Port Range:80

Type: HTTPS Port Range:443

### EC2 实例

在实例这里可以查看用户创建的所有实例的列表。

其中值得注意的是：
- Availabity Zone

可用区：比如美国东部，太平洋地区等。
- Public DNS(IPv4)

DNS 地址： my.public.dns.amazonaws.com

EC2实例 通常是 `ec2-<Public IP>-<Availabity Zone>-compute.amazonaws.com`

DNS 地址包含了IP 地址和可用区的信息。
- IPv4 Public IP

IP 地址：xx.xxx.xxx.xx
- Key Name

创建实例时的密钥对名
- Security Groups

点击配置安全组

选择某个运行中的实例，可以连接该实例进行配置。

对于 EC2 实例，命令是
`ssh -i "xxx.pem" ec2-user@my.public.dns.amazonaws.com`

xxx.pem 是私钥文件名（须使用相对或绝对路径），

my.public.dns.amazonaws.com 是 DNS 地址。

连接成功之后便可在本地使用命令行远程操纵服务器。

```
$ ssh -i "xxx.pem" ec2-user@my.public.dns.amazonaws.com
Last login: ***(date) from ***(ip)

       __|  __|_  )
       _|  (     /   Amazon Linux AMI
      ___|\___|___|

https://aws.amazon.com/amazon-linux-ami/2018.03-release-notes/
[ec2-user ~]$
```

### 配置 LAMP 服务器

这一段参考教程： Install a LAMP Server (Amazon Linux AMI)
- 更新服务器
`[ec2-user ~]$ sudo yum update -y`
- 安装 Apache, PHP, MySQL
`[ec2-user ~]$ sudo yum install -y httpd24 php70 mysql56-server php70-mysqlnd`
- 启动 Apache 并设置文件权限

```
[ec2-user ~]$ sudo service httpd start
[ec2-user ~]$ sudo chkconfig httpd on
[ec2-user ~]$ sudo usermod -a -G apache ec2-user
临时退出
[ec2-user ~]$ exit
再连接
[ec2-user ~]$ groups
ec2-user wheel apache
[ec2-user ~]$ sudo chown -R ec2-user:apache /var/www
[ec2-user ~]$ sudo chmod 2775 /var/www
[ec2-user ~]$ find /var/www -type d -exec sudo chmod 2775 {} \;
[ec2-user ~]$ find /var/www -type f -exec sudo chmod 0664 {} \;
```

`http://my.public.dns.amazonaws.com/`

可查看 Apache 的测试页面。
- 配置 MySQL

```
启动服务器
[ec2-user ~]$ sudo service mysqld start
输入 root 账号的密码，关闭远程登陆等等
[ec2-user ~]$ sudo mysql_secure_installation
[ec2-user ~]$ sudo chkconfig mysqld on
```
- 安装 phpMyAdmin

```
[ec2-user ~]$ sudo yum install php70-mbstring.x86_64 php70-zip.x86_64 -y
[ec2-user ~]$ sudo service httpd restart
[ec2-user ~]$ cd /var/www/html
[ec2-user html]$ wget https://www.phpmyadmin.net/downloads/phpMyAdmin-latest-all-languages.tar.gz
[ec2-user html]$ mkdir phpMyAdmin && tar -xvzf phpMyAdmin-latest-all-languages.tar.gz -C phpMyAdmin --strip-components 1
[ec2-user html]$ rm phpMyAdmin-latest-all-languages.tar.gz
```

`http://my.public.dns.amazonaws.com/phpMyAdmin`

可打开 phpMyAdmin 配置服务器上的 MySQL 数据库。

### FileZilla

可以使用 FileZilla 操作服务器，上传或下载文件。

连接时需要使用如下配置：

协议：SFTP（SSH File Transfer Protocol）

主机：my.public.dns.amazonaws.com（DNS 地址）

端口：空白（默认为22）

用户名：ec2-user

连接方式：选择 key file

xxx.pem（私钥文件）

Apache 的测试页面所在位置：

/var/www/html

### 免费数字证书

配置完毕的服务器可以用 http（80）访问。

要想使用 https（443）访问，光配置端口是不够的，还必须向数字证书颁发机构（CA）购买数字证书。

数字证书一般不适用于 IP 地址，必须先购买域名。

有了数字证书，网站的访问级别就可以由不安全（惊叹号）变为安全（锁）。

[Let's Encrypt - Free SSL/TLS Certificates](https://letsencrypt.org/)

该网站是一个（免费，自动，开放的）CA，可提供免费的SSL/TLS服务器证书。

AWS 所提供的 DNS 地址 *.amazonaws.com 在该网站的黑名单上，必须使用其他域名。

[How to integrate free SSL certificate into AWS instance with Let’s Encrypt](https://medium.com/oceanize-geeks/how-to-integrate-free-ssl-certificate-into-aws-instance-with-lets-encrypt-d7f673721132)

使用 certbot-auto 可自动完成从 Let's Encrypt 网站下载安装数字证书等任务。

假设网站域名为 example.com, 别名是 www.example.com：

```
[ec2-user ~]$ sudo yum install -y mod24_ssl
[ec2-user ~]$ wget https://dl.eff.org/certbot-auto
[ec2-user ~]$ chmod a+x certbot-auto
[ec2-user ~]$ sudo ./certbot-auto --debug -v --server https://acme-v01.api.letsencrypt.org/directory certonly --webroot -w /var/www/html -d example.com  -d www.example.com
```

任务成功显示信息：

```
- Congratulations! Your certificate and chain have been saved at:
   /etc/letsencrypt/live/example.com/fullchain.pem
   Your key file has been saved at:
   /etc/letsencrypt/live/example.com/privkey.pem
   Your cert will expire on 2018-xx-xx. To obtain a new or tweaked
   version of this certificate in the future, simply run certbot-auto
   again. To non-interactively renew *all* of your certificates, run
   "certbot-auto renew"
```

可使用以下网址检查所生成的数字证书：

```
https://www.ssllabs.com/ssltest/analyze.html?d=example.com
https://www.ssllabs.com/ssltest/analyze.html?d=www.example.com
```

使用 root 权限查看所生成的 pem 文件。

```
[ec2-user ~]$ sudo ls /etc/letsencrypt/live/example.com/
README  cert.pem  chain.pem  fullchain.pem  privkey.pem
```

数字证书一共包含4个 pem 文件，需要配置 Apache 以利用这些数字证书文件。

[http to https apache redirection](https://stackoverflow.com/questions/16200501/http-to-https-apache-redirection)
[Redirect non www to www using httpd.conf](https://stackoverflow.com/questions/5150177/redirect-non-www-to-www-using-httpd-conf)
[How To Redirect www to Non-www with Apache on Ubuntu 14.04](https://www.digitalocean.com/community/tutorials/how-to-redirect-www-to-non-www-with-apache-on-ubuntu-14-04)

下面修改 Apache 的配置文件1
`[ec2-user ~]$ sudo vim /etc/httpd/conf/httpd.conf`

在`Listen 80`这行下面

添加以下几行：（希望 example.com 转向 www.example.com 的话）

```
<VirtualHost *:80>
    DocumentRoot /var/www/html
    ServerAlias example.com
    Redirect permanent / https://www.example.com/
</VirtualHost>
```

或者 （希望 www.example.com 转向 example.com 的话）

```
<VirtualHost *:80>
    DocumentRoot /var/www/html
    ServerAlias www.example.com
    Redirect permanent / https://example.com/
</VirtualHost>
```

下面修改 Apache 的配置文件2
`[ec2-user ~]$ sudo vim /etc/httpd/conf.d/ssl.conf`

注意这个文件的内容实际上是对443端口的配置

```
Listen 443 https
# ...
<VirtualHost _default_:443>
# ...
SSLEngine on
# ...
</VirtualHost>
```

在相应的提示性的注释下

分别添加以下几行：（希望 example.com 转向 www.example.com 的话）

```
SSLCertificateFile /etc/letsencrypt/live/example.com/cert.pem
SSLCertificateKeyFile /etc/letsencrypt/live/example.com/privkey.pem
SSLCertificateChainFile /etc/letsencrypt/live/example.com/fullchain.pem

RewriteEngine On
RewriteCond %{HTTP_HOST} !^www\. [NC]
RewriteRule ^(.*)$ http://www.%{HTTP_HOST}%{REQUEST_URI} [R=301,L]
```

或者 （希望 www.example.com 转向 example.com 的话）

```
SSLCertificateFile /etc/letsencrypt/live/example.com/cert.pem
SSLCertificateKeyFile /etc/letsencrypt/live/example.com/privkey.pem
SSLCertificateChainFile /etc/letsencrypt/live/example.com/fullchain.pem

RewriteEngine On
RewriteCond %{HTTP_HOST} ^www\.(.*)$ [NC]
RewriteRule ^(.*)$ http://%1/$1 [R=301,L]
```

最后重启服务器
`[ec2-user ~]$ sudo service httpd restart`

大功告成 Congratulations!!!

https://example.com 和 https://www.example.com 以及其子文件夹

都可以安全访问了。

并且输入以下URL时
- example.com
- www.example.com
- http://example.com
- http://www.example.com
- https://example.com
- https://www.example.com

浏览器都会统一按照设置转向 https://www.example.com/（或者 https://example.com/）。

免费证书过期时需要更新证书，然后再重启服务器。

```
[ec2-user ~]$ sudo ./certbot-auto renew
[ec2-user ~]$ sudo service httpd restart
```

### 免费域名

配置完毕的服务器可以用 IP 地址或 DNS 地址访问，但每次重启地址会变。

要想地址稳定，需要购买域名，并映射到 IP 地址或 DNS 地址。

[Freenom - A Name for Everyone](http://www.freenom.com/)

该网站提供 xxx.tk, xxx.ml, xxx.ga, xxx.cf, xxx.gq 一共5种后缀的免费域名，输入 xxx 可查询免费域名是否可用。

如果免费域名可用，可以花0美元免费购买，当然需要注册。

免费使用期间最多一年，到期应该可以重新激活。

登录之后在主页选择 Services -> My domains 查看已购买的域名。

点击 Manage Domain 可以配置域名。

Management Tools -> Nameservers 选择 Use default nameservers (Freenom Nameservers)

Manage Freenom DNS

在这里配置域名，添加 DNS 记录。

需要添加两条 A 记录 或 CNAME 记录，分别对应域名以及 www 子域名。

其中
- A记录能把域名映射为 IP 地址。

Type 填 A，Target 填 IP 地址，Name 空白不填。

Type 填 A，Target 填 IP 地址，Name 填 www。
- CNAME 记录能把域名映射为 DNS 地址。

Type 填 CNAME，Target 填 DNS 地址，Name 空白不填。

Type 填 CNAME，Target 填 DNS 地址，Name 填 www。


