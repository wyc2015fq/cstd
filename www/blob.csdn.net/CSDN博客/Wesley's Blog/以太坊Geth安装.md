
# 以太坊Geth安装 - Wesley's Blog - CSDN博客


2018年05月16日 15:20:48[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：1273


# Geth是什么
Geth 又名Go Ethereum.是以太坊协议的三种实现之一，由Go语言开发，完全开源的项目。Geth可以被安装在很多操作系统上，包括Windows、Linux、Mac的OSX、Android或者IOS系统.
Geth官网：[https://geth.ethereum.org/](https://geth.ethereum.org/)
Geth的Github地址：[https://github.com/ethereum/go-ethereum](https://github.com/ethereum/go-ethereum)
# Geth能干什么
Geth是以太坊协议的具体落地实现，通过Geth，你可以实现以太坊的各种功能,如账户的新建编辑删除，开启挖矿，ether币的转移，智能合约的部署和执行等等
# Geth安装
首先需要保证已安装Golang和C Complier．
### 安装Golang
Golang官方下载地址：[https://golang.org/\#](https://golang.org/#)
下载好后解压：
`tar -C /usr/local -xzf go1.10.2.linux-amd64.tar.gz`添加环境变量：
`export PATH=$PATH:/usr/local/go/bin`
### 安装Geth
克隆Github项目:
`git clone https://github.com/ethereum/go-ethereum.git`进入到文件目录，源码安装：
`cd go-ethereum
make geth`
### 检验是否安装成功
`wesley@wesley-PC:~/Code/blockchain/go-ethereum/build/bin$./geth version
Geth
Version: 1.8.9-unstable
Git Commit: f6bc65fc681177b190522c92887979690f9ceefa
Architecture: amd64
Protocol Versions: [63 62]
Network Id: 1
Go Version: go1.10.2
Operating System: linux
GOPATH=
GOROOT=/usr/local/go`如果信息显示正确，则安装成功．

