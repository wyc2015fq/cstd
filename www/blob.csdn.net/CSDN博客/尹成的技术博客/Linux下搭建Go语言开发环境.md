
# Linux下搭建Go语言开发环境 - 尹成的技术博客 - CSDN博客

2018年11月16日 09:38:49[尹成](https://me.csdn.net/yincheng01)阅读数：324


**@安装编译器**
**方案一**
下载安装包
解压到特定目录
`cd /usr/local
tar -zxvf goxxx.tar.gz`配置PATH
`export PATH=$PATH:/usr/local/go/bin`使配置永久生效
`sudo gedit /etc/profile
export PATH=$PATH:/usr/local/go/bin
source /etc/profile 或 reboot`**方案二**
`sudo apt install golang-go`**@编译HelloWorld**
`go build hello.go
./hello`**@安装GoLand**
下载安装包，解压到任意目录
`cd ~
tar -zxvf goland-xxx.tar.gz`[执行bin目录下的goland.sh](http://xn--bingoland-2b6nn53mnyhwi2elhbl78l.sh)
`cd ~/golandxxx/bin
./goland.sh`在线激活：[http://idea.youbbs.org](http://idea.youbbs.org)
配置GOROOT和GOPATH
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116092700978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

