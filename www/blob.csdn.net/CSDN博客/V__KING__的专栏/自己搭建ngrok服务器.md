# 自己搭建ngrok服务器 - V__KING__的专栏 - CSDN博客





2015年11月23日 11:02:10[v__king__](https://me.csdn.net/V__KING__)阅读数：7031








# 自己搭建ngrok服务器

需要有一个公网的vps

## 1、GO环境安装

```
wget https://storage.googleapis.com/golang/go1.4.1.linux-amd64.tar.gz
tar -C /usr/local -xzf go1.4.1.linux-amd64.tar.gz
sudo echo "export PATH=$PATH:/usr/local/go/bin" >> /etc/profile
sudo echo "export GOROOT=/usr/local/go" >> /etc/profile
export PATH=$PATH:/usr/local/go/bin
export GOROOT=/usr/local/go
sudo chmod 755 /usr/local/go -R
sudo chown `whoami`:root /usr/local/go -R
source /etc/profile
```

## 2. ngrok下载

```
cd /usr/local/src
git clone https://github.com/inconshreveable/ngrok.git
sudo chmod 755 /usr/local/src/ngrok -R
sudo chown `whoami`.root /usr/local/src/ngrok -R
```
3.证书生成
```
export GOPATH=/usr/local/src/ngrok
cd $GOPATH
mkdir ssl
cd ssl
openssl genrsa -out rootCA.key 2048
openssl req -x509 -new -nodes -key rootCA.key -subj "/C=CN/ST=GuangDong/L=ShenZhen/O=vkingman.tk/CN=ng.vkingman.tk" -days 3650 -out rootCA.pem
openssl genrsa -out device.key 2048
openssl req -new -key device.key -subj "/C=CN/ST=GuangDong/L=ShenZhen/O=vkingman.tk/CN=ng.vkingman.tk" -out device.csr
sudo openssl x509 -req -in device.csr -CA rootCA.pem -CAkey rootCA.key -CAcreateserial -out device.crt -days 730
cp rootCA.pem $GOPATH/assets/client/tls/ngrokroot.crt
cp device.crt $GOPATH/assets/server/tls/snakeoil.crt 
cp device.key $GOPATH/assets/server/tls/snakeoil.key
```

## 4.编译

1）编译server端

下面中编译服务端只选择一种就可以了

> 
GOOS:是目标的OS

GOARCH:是目标的架构


GOOS=linux GOARCH=386 ./make.bash是更换 `交叉编译环境`

```
cd $GOROOT/src
GOOS=linux GOARCH=386 ./make.bash
cd $GOPATH
GOOS=linux GOARCH=386 make release-server

cd $GOROOT/src
GOOS=linux GOARCH=amd64 ./make.bash
cd $GOPATH
GOOS=linux GOARCH=amd64 make release-server

cd $GOROOT/src
GOOS=linux GOARCH=arm ./make.bash
cd $GOPATH
GOOS=linux GOARCH=arm make release-server

cd $GOROOT/src
GOOS=darwin GOARCH=386 ./make.bash
cd $GOPATH
GOOS=darwin GOARCH=386 make release-server

cd $GOROOT/src
GOOS=darwin GOARCH=amd64 ./make.bash
cd $GOPATH
GOOS=darwin GOARCH=amd64 make release-server
```
- 
**错误Error**：`GOOS="" GOARCH="" go get github.com/jteeuwen/go-bindata/go-bindata`
- 方法1： $GOPATH没有权限，执行`sudo chown vk:root $GOPATH -R`
- 方法2：或者就去github.com/jteeuwen/go-bindata中下载，按照
- 执行命令 :
		
```
> cd $GOPATH
    > go get -u github.com/jteeuwen/go-bindata/go-bindata
```


你会发现在$GOPATH/src中有了github.com/jteeuwen/go-bindata目录
[https://github.com/jteeuwen/go-bindata](https://github.com/jteeuwen/go-bindata)

- 
**错误**：package code.google.com/p/log4go: exec: “hg”: executable file not found in $PATH
- 原因：code.google.com的东西转移到了github上面，code.google.com上没有了log4go酷
- 找到code.google.com/p/log4go的字符串`grep -Rn code.google.com/p/log4go $GOPATH`，修改/log/logger.go,替换为：`github.com/V-KING/log4go`

- 
**看看bin目录：有了ngrokd**


2）编译client端

选择下面的一种

```
cd $GOROOT/src
GOOS=windows GOARCH=386 ./make.bash
cd $GOPATH
GOOS=windows GOARCH=386 make release-client

cd $GOROOT/src
GOOS=windows GOARCH=amd64 ./make.bash
cd $GOPATH
GOOS=windows GOARCH=amd64 make release-client

cd $GOROOT/src
GOOS=linux GOARCH=386 ./make.bash
cd $GOPATH
GOOS=linux GOARCH=386 make release-client

cd $GOROOT/src
GOOS=linux GOARCH=amd64 ./make.bash
cd $GOPATH
GOOS=linux GOARCH=amd64 make release-client

cd $GOROOT/src
GOOS=linux GOARCH=arm ./make.bash
cd $GOPATH
GOOS=linux GOARCH=arm make release-client

cd $GOROOT/src
GOOS=darwin GOARCH=386 ./make.bash
cd $GOPATH
GOOS=darwin GOARCH=386 make release-client

cd $GOROOT/src
GOOS=darwin GOARCH=amd64 ./make.bash
cd $GOPATH
GOOS=darwin GOARCH=amd64 make release-client
```

### 5. 服务端ngrokd后台启动运行

例如1：系统管理员经常需要SSH 或者telent 远程登录到Linux 服务器，经常运行一些需要很长时间才能完成的任务，比如系统备份、ftp 传输等等。通常情况下我们都是为每一个这样的任务开一个远程终端窗口，因为它们执行的时间太长了。必须等待它们执行完毕，在此期间不能关掉窗口或者断开连接，否则这个任务就会被杀掉，一切半途而废了.

例如2：执行一个程序的时候如果窗口关闭，或者按住了ctrl+c就会中中止。
- 输入下面命令

```
sudo apt-get install screen
screen -S keepNgrok
sudo /usr/local/src/ngrok/bin/ngrokd -domain="ng.vkingman.tk" -httpAddr=":8888" -httpsAddr=":8800" -tunnelAddr=":8080"
```
- 按住组合键（多按几次）：

	CTRT + A + D

### 6. 内网ngrok来连接ngrokd服务端

1. 创建一个配置文件ngrok.cfg，内容如下

```
server_addr: "ng.vkingman.tk:8080"
trust_host_root_certs: false
tunnels:
  pi:
    proto:
      http: "80"
  ssh:
    remote_port: 8022
    proto:
      tcp: "22"
```

2. 执行ngrok：

> 
ngrok -config ngrok.cfg start pi ssh




