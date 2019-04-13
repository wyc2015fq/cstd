
# 腾讯云CentOS Apache开启HTTPS - 致林 - 博客园






# [腾讯云CentOS Apache开启HTTPS](https://www.cnblogs.com/bincoding/p/5952411.html)
**1.申请SSL证书**
https://console.qcloud.com/ssl?utm_source=yingyongbao&utm_medium=ssl&utm_campaign=qcloud
**2.配置Linux服务器**
首先安装openssl
\#yuminstallmod_ssl openssl
然后将我们第一步生成的SSL上传到服务器
\#cpwww.bincoding.cn.crt /etc/pki/tls/certs/\#cpwww.bincoding.cn.key /etc/pki/tls/private/
进入/etc/pki/tls/private/目录：生成csr文件
openssl req -new -key www.bincoding.cn.key -out www.bincoding.cn.csr
中间需要输入证书信息
[root@VM_238_215_centos private]\# openssl req -new -key www.bincoding.cn.key -out www.bincoding.cn.csr 
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter'.', the field will be left blank.-----Country Name (2letter code) [XX]:CN
State or Province Name (full name) []:ChongQing
Locality Name (eg, city) [Default City]:ChongQing
Organization Name (eg, company) [Default Company Ltd]:
Organizational Unit Name (eg, section) []:
Common Name (eg, your name or your server's hostname) []:Email Address []:
**3.配置Apache 服务器**
\# vim /etc/httpd/conf.d/ssl.conf
\#\#\# overwrite the following parameters \#\#\#
SSLCertificateFile/etc/pki/tls/certs/www.bincoding.cn.crt
SSLCertificateKeyFile/etc/pki/tls/private/www.bincoding.cn.key
\#\#\# The following parameter does not need to be modifiedincaseof a self-signed certificate. \#\#\#
\#\#\# If you are using a real certificate, you may receive a certificate bundle. The bundle is added using the following parameters \#\#\#
SSLCertificateChainFile/etc/pki/tls/certs/example.com.ca-bundle
然后重启httpd
\# service httpd restart
重启成功后apache已经支持https了~





