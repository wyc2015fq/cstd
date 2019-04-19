# Ngrok内网穿透服务器解决方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年03月28日 13:33:38[boonya](https://me.csdn.net/boonya)阅读数：130标签：[云主机																[内网穿透																[ngrok](https://so.csdn.net/so/search/s.do?q=ngrok&t=blog)](https://so.csdn.net/so/search/s.do?q=内网穿透&t=blog)](https://so.csdn.net/so/search/s.do?q=云主机&t=blog)
个人分类：[应用云服务](https://blog.csdn.net/boonya/article/category/6370343)
原文地址：[https://www.cnblogs.com/irich/p/8674796.html](https://www.cnblogs.com/irich/p/8674796.html)
不用花生壳、nat123等内网穿透软件的原因：
1、免费的限制多
2、收费的价格高
自己搞个网站、博客什么的成本高了自然不合适，当然土豪另当别论了。
此次方案：279元/3年（购买云主机） 自己手动实现无限制内网穿透
**一、前提条件**
　　1、需要一台服务器
这里推荐阿里云，1核2G 现在价格3年只要279元（相当的便宜啊！）
**3年只要279元**
![](https://images2018.cnblogs.com/blog/1099612/201803/1099612-20180329101848154-1920456367.png)
建立自己的网站，打造自己的博客，只差一个云主机，阿里云3年279元 你值得拥有 赶紧[点击链接](https://promotion.aliyun.com/ntms/act/group/team.html?group=sl3S60iPJ7)领取你的服务器吧！
[https://promotion.aliyun.com/ntms/act/group/team.html?group=sl3S60iPJ7](https://promotion.aliyun.com/ntms/act/group/team.html?group=sl3S60iPJ7)
** 　　2、购买好之后：**
域名解析配置*.ngrok和ngrok
![](https://images2018.cnblogs.com/blog/1099612/201803/1099612-20180330095422740-1470526514.png)
**　　本机环境变量配置**
vim /etc/profile
export NGROK_DOMAIN="ngrok.yourdomain.com"
红色的改成你自己的域名
**　　本机安装ngrok所需lib安装**
yum -y install zlib-devel openssl-devel perl hg cpio expat-devel gettext-devel curl curl-devel perl-ExtUtils-MakeMaker hg wget gcc gcc-c++ git
**二、搭建ngrok内网穿透**
　　这里原理就不详述了，可自行百度！
**1、安装go语言**
//请下载合适自己的go语言包  我是centos 6.8 64位 所以选择以下包
wget https://storage.googleapis.com/golang/go1.8.3.linux-amd64.tar.gz
tar -C /usr/local -xzf go1.8.3.linux-amd64.tar.gz
vim /etc/profile
//添加以下内容：
export PATH=$PATH:/usr/local/go/bin
source /etc/profile
//检测是否安装成功go
go version
**2、下载ngrok源码包**
　　github地址：https://github.com/inconshreveable/ngrok
//创建目录
mkdir /ngrok
cd /ngrok
git clone https://github.com/inconshreveable/ngrok.git
**3、生成证书**
cd /ngrok
mkdir cert
cd cert
openssl genrsa -out rootCA.key 2048
openssl req -x509 -new -nodes -key rootCA.key -subj "/CN=$NGROK_DOMAIN" -days 5000 -out rootCA.pem
openssl genrsa -out server.key 2048
openssl req -new -key server.key -subj "/CN=$NGROK_DOMAIN" -out server.csr
openssl x509 -req -in server.csr -CA rootCA.pem -CAkey rootCA.key -CAcreateserial -out server.crt -days 5000
**4、覆盖证书**
cp rootCA.pem /ngrok/ngrok/assets/client/tls/ngrokroot.crt  #复制rootCA.pem到assets/client/tls/并更名为ngrokroot.crt
cp server.crt /ngrok/ngrok/assets/server/tls/snakeoil.crt #复制server.crt到assets/server/tls/并更名为snakeoil.crt
cp server.key /ngrok/ngrok/assets/server/tls/snakeoil.key #复制server.key到assets/server/tls/并更名为snakeoil.key
**5、编译生成ngrokd**
cd /ngrok/ngrok
go env //查看环境
GOOS=linux GOARCH=amd64 make release-server
**6、测试运行**
cd /ngrok/ngrok
./bin/ngrokd -tlsKey="assets/server/tls/snakeoil.key" -tlsCrt="assets/server/tls/snakeoil.crt" -domain="ngrok.yourdomain.com" -httpAddr=":8081" -httpsAddr=":8082" -tunnelAddr=":8083"
 红色部分改成你的域名
**7、后台运行**
cd /ngrok/ngrok
setsid ./bin/ngrokd -tlsKey="assets/server/tls/snakeoil.key" -tlsCrt="assets/server/tls/snakeoil.crt" -domain="ngrok.yourdomain.com" -httpAddr=":8081" -httpsAddr=":8082" -tunnelAddr=":8083"
这样就可以让服务跑在后台了
**8、编译客户端**
GOOS=windows GOARCH=amd64 make release-client
#编译成功后会在ngrok/bin/下面生成一个windows_amd64目录下面有ngrok.exe
#Linux 平台 32 位系统：GOOS=linux GOARCH=386
#Linux 平台 64 位系统：GOOS=linux GOARCH=amd64
#Windows 平台 32 位系统：GOOS=windows GOARCH=386
#Windows 平台 64 位系统：GOOS=windows GOARCH=amd64
#MAC 平台 32 位系统：GOOS=darwin GOARCH=386
#MAC 平台 64 位系统：GOOS=darwin GOARCH=amd64
#ARM 平台：GOOS=linux GOARCH=arm
**9、运行客户端**
 将编译好的客户端下载到本地，客户端的目录在
![](https://images2018.cnblogs.com/blog/1099612/201803/1099612-20180330101723620-1145122155.png)
在这个目录下有个ngrok.exe下载下来
在ngrok.exe的同级目录下，新建一个ngrok.cfg和start.bat两个文件
![](https://images2018.cnblogs.com/blog/1099612/201803/1099612-20180330101950571-47386463.png)
**ngrok.cfg内容**
server_addr: "myngrok.com:8083"
trust_host_root_certs: false
**start.bat内容**
ngrok -config=ngrok.cfg -subdomain=test 80
红色的是域名的前缀，可以自己定义
**执行start.bat**
//出现以下内容表示成功链接：
ngrok
Tunnel Status                 online
Version                       1.7/1.7
Forwarding                    http://test.ngrok.yourdomian.com:8081 -> 127.0.0.1:80
Forwarding                    https://test.ngrok.yourdomain.com:8081 -> 127.0.0.1:80
Web Interface                 127.0.0.1:4040
# Conn                        0
Avg Conn Time                 0.00ms
本文参考：
https://segmentfault.com/a/1190000010338848
https://blog.csdn.net/zhangguo5/article/details/77848658?utm_source=5ibc.net&utm_medium=referral
