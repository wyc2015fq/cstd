# Go语言入门系列1：安装，How to Write Go Code - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月30日 10:36:00[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：257
[https://golang.org/doc/code.html](https://golang.org/doc/code.html)
src contains Go source files,
pkg contains package objects, and
bin contains executable commands.
The go tool
 builds source packages and installs the resulting binaries to the pkg and bin directories.
需要先设置：
The GOPATH
 specifies the location of your workspace. 
GOPATH must **not**be
 the same path as your Go installation.
安装
下载[https://golang.org/dl/](https://golang.org/dl/)
如
[https://storage.googleapis.com/golang/go1.8.3.linux-amd64.tar.gz](https://storage.googleapis.com/golang/go1.8.3.linux-amd64.tar.gz)
tar -C /usr/local/ -xzf go1.8.3.linux-amd64.tar.gz
export PATH=$PATH:/usr/local/go/bin
GOROOT must
 be set when installing to a custom
location. 所以最好安装在/usr/local目录下
为什么要使用 Go 语言？Go 语言的优势在哪里？
部署简单。
我发现我花了四年时间锤炼自己用 C 语言构建系统的能力，试图找到一个规范，可以更好的编写软件。结果发现只是对 Go 的模仿。缺乏语言层面的支持，只能是一个拙劣的模仿。
对于大多数后台应用场景，选择Golang是极为明智的选择。
如果你是C爱好者，强烈建议你学习和使用Go。Go可以调用C/C++程序
你用来学89个C++高级特性的时间，估计已经用Go写了64个开源项目了。


