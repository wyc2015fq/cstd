# 使用node.js创建自签名https服务器 - tifentan的专栏 - CSDN博客





2018年02月11日 14:19:57[露蛇](https://me.csdn.net/tifentan)阅读数：458








由于浏览器的一些特性在https及http下表现不同，有时候需要进行对比测试。想用nodejs搞一个简单的https服务器，将搭建流程记录下来。

## 一些概念

> 
CA(Certificate Authority)是数字证书认证中心的简称，是指发放、管理、废除数字证书的机构。


以下是我自己的理解，不保证严谨性。 
**证书**：建立一个称为证书的文档，把B的信息写上去，由一个可信的A（一般为CA）在这文档上签名（数字签名）确认这些信息是真实的。 

关于数字证书的介绍可[参看](https://www.jianshu.com/p/42bf7c4d6ab8)
## 一些准备

先安装一个openssl，主要是拿来生成证书及过程文件。装完后cmd到bin目录，然后在bin目录下创建一个文件夹，我的名为ttfca。

## CA根证书创建和注册
- 生成CA的私钥
`openssl genrsa -out ttfca/ca-key.pem -des 2048`
-des:启动des对称密码保护私钥 

  2048：RSA私钥长度，可以1024，2048
- 准备CA的配置文件 

火狐浏览器需要有v3拓展的根证书，我们在ttfca下建一个ca.cnf的文件

```
#ca.cnf
[req]  
distinguished_name = req_distinguished_name  
x509_extensions = v3_req  

[req_distinguished_name]  
countryName = Country Name (2 letter code)  
countryName_default = CN  
stateOrProvinceName = State or Province Name (full name)  
stateOrProvinceName_default = GuangDong  
localityName = Locality Name (eg, city)  
localityName_default = GuangZhou  
organizationalUnitName  = Organizational Unit Name (eg, section)  
organizationalUnitName_default  = tifen
commonName = tifen root CA
commonName_max  = 64  

[ v3_req ]  
# Extensions to add to a certificate request  
subjectKeyIdentifier = hash
#authorityKeyIdentifier = keyid:always,issuer
basicConstraints = critical,CA:true  
keyUsage = critical, digitalSignature, cRLSign, keyCertSign
```
- 通过私钥生成证书请求csr
`openssl req -new -config ttfca/ca.cnf -key ttfca/ca-key.pem -out ttfca/ca-csr.pem`- 通过CSR文件和私钥生成CA证书
`openssl x509 -req -extfile ttfca/ca.cnf -extensions v3_req -days 7300 -in ttfca/ca-csr.pem -signkey ttfca/ca-key.pem -out ttfca/ca-cert.pem`- 转换证书的格式 

在windows下使用crt格式的证书，我们转换一下好了。
`openssl x509 -outform der -in ttfca/ca-cert.pem -out ttfca/ca-cert.crt`- 注册根证书 

到此为止，根证书已经创建完成了。由于我是自己搞的证书，需要注册到系统。 

双击一下ca-cert.crt安装： 
![这里写图片描述](https://img-blog.csdn.net/20180211135321631?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180211135335602?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

安装完后可以在系统的证书管理器（命令行certmgr.msc）查看： 
![这里写图片描述](https://img-blog.csdn.net/20180211135530452?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 在火狐浏览器注册证书 

火狐浏览器不使用系统的证书系统，自带证书系统，因此要独立为火狐浏览器注册一次。 

在  *选项/隐私与安全/证书/查看证书* 打开后导入： 
![这里写图片描述](https://img-blog.csdn.net/20180211140101513?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 颁发服务器证书
- 创建服务器私钥
`genrsa -out ttfca/server-key.pem 2048`- 创建服务器证书配置文件 

在目录下创建一个server.cnf的文件：

```
[req]  
distinguished_name = req_distinguished_name  
x509_extensions = v3_req  

[req_distinguished_name]  
countryName = Country Name (2 letter code)  
countryName_default = CN  
stateOrProvinceName = State or Province Name (full name)  
stateOrProvinceName_default = GuangDong  
localityName = Locality Name (eg, city)  
localityName_default = GuangZhou  
organizationalUnitName  = Organizational Unit Name (eg, section)  
organizationalUnitName_default  = tifen
commonName = tifen node server
commonName_max  = 64  

[ v3_req ]  
# Extensions to add to a certificate request  
subjectKeyIdentifier = hash
authorityKeyIdentifier = keyid,issuer:always
basicConstraints = CA:FALSE  
keyUsage = nonRepudiation, digitalSignature, keyEncipherment  
subjectAltName = @alt_names  

[alt_names]  
#注意这个IP.1的设置，IP地址需要和你的服务器的监听地址一样
IP.1 = 192.168.1.77
IP.2 = 127.0.0.1
```

注意一下拓展字段填入你的ip地址。当然也可以添加dns的。
- 创建服务器证书请求csr
`openssl req -new -config ttfca/server.cnf -key ttfca/server-key.pem -out ttfca/server-csr.pem`- 使用根证书颁发服务器证书
`openssl x509 -req -CA ttfca/ca-cert.pem -CAkey ttfca/ca-key.pem -CAcreateserial -extfile ttfca/server.cnf -extensions v3_req -days 3650 -in ttfca/server-csr.pem -out ttfca/server-cert.pem`
到此为止，证书的都搞定了。接下来可以搭建node服务器。

## node服务器代码

```java
var https = require('https');
var fs = require('fs');
var express = require('express');
var app = express();

var options = {
    key: fs.readFileSync('./keys/server-key.pem'),
    ca: [fs.readFileSync('./keys/ca-cert.pem')],
    cert: fs.readFileSync('./keys/server-cert.pem')
};

app.get('/', function (req, res) {
   res.writeHead(200);
    res.end('hello world\n');
})

var server = https.createServer(options,app).listen(3000,function () {

  var host = server.address().address;
  var port = server.address().port;

  console.log("http://%s:%s", host, port);

})
```

## 运行效果

![这里写图片描述](https://img-blog.csdn.net/20180211141430486?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

嗯，还能看到证书链呢~~~~ 
![这里写图片描述](https://img-blog.csdn.net/20180211141419775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
nice，可以使用https测试你的web代码了！☻

参考： 
[http://cnodejs.org/topic/54745ac22804a0997d38b32d](http://cnodejs.org/topic/54745ac22804a0997d38b32d)
[http://www.cnblogs.com/f-ck-need-u/p/6091027.html](http://www.cnblogs.com/f-ck-need-u/p/6091027.html)







