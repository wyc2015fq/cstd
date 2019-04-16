# Go安装 - youbingchen的博客 - CSDN博客





2016年08月02日 16:46:31[youbingchen](https://me.csdn.net/youbingchen)阅读数：10467








# Go的三种安装方式
- Go源码安装：这是一种标准的软件安装方式
- Go标准包安装:Go提供了方便的安装包，支持Windows、Linux、Mac等系统。这种方式适合快速安装，可根据自己的系统位数下载好相应的安装包，一路next就可以轻松安装了。
- 第三方工具安装：目前有很多方便的第三方软件包工具，例如Ubuntu的apt-get、Mac的homebrew等。这种安装方式适合那些熟悉相应系统的用户。

最后，如果你想在同一个系统中安装多个版本的Go，你可以参考第三方工具GVM，这是目前在这方面做得最好的工具，除非你知道怎么处理。

# Go源码安装

在Go的源代码中，有些部分是用Plan9C和AT&T汇编写的，因此假如你要想从源码安装，就必须安装C的编译工具。 

在Mac系统中，只要你安装了Xcode，就已经包含了相应的编译工具。 

在类Unix系统中，需要安装gcc等工具。例如Ubuntu系统可通过在终端中执行`sudo apt-get install gcc libc6-dev`来安装编译工具。 

在Windows系统中，你需要安装MinGW，然后通过MinGW安装gcc，并设置相应的环境变量。 

可以直接去官网下载源码，找相应的`goVERSION.src.tar.gz`的文件下载，下载之后解压缩到`$HOME`目录，执行如下代码：
```cpp
cd go/src
./all.bash
```

运行`all.bash`后出现”ALL TESTS PASSED”字样时才算安装成功。 

上面是Unix风格的命令，Windows下的安装方式类似，只不过是运行all.bat，调用的编译器是MinGW的gcc。 

如果是Mac或者Unix用户需要设置几个环境变量，如果想重启之后也能生效的话把下面的命令写到.bashrc或者.zshrc里面。
```cpp
export GOPATH=$HOME/gopath  # 设置个人的工作目录，可以包含多个工作目录，取决于你的个人情况。如果你设置了多个工作目录，那么当你在之后                               使用 go get（远程包安装命令）时远程包将会被安装在第一个目录下。
export GOROOT=$HOME/go  # 安装目录 
export PATH=$PATH:$GOROOT/bin
```

当你设置完毕之后在命令行里面输入go，看到如下图片即说明你已经安装成功（图片） 

如果出现Go的Usage信息，那么说明Go已经安装成功了；如果出现该命令不存在，那么可以检查一下自己的PATH环境变中是否包含了Go的安装目录。

# Go安装1.4以上的版本出现的问题

> 
个人在安装 go1.6.3的时候，一直 无法 按照前面的步骤安装9，一直 提醒的错误是：


```
##### Building Go bootstrap tool.
cmd/dist
ERROR: Cannot find /root/go1.4/bin/go.
Set $GOROOT_BOOTSTRAP to a working Go tree >= Go 1.4.
```

> 
后面经过查阅-资料，大家可以 直接 参考 这篇文章进行安装 [在中国网络环境下从源代码安装Go1.6到CentOS 7](https://github.com/northbright/Notes/blob/master/Golang/china/install-go1.6-from-source-on-centos7-in-china.md)






