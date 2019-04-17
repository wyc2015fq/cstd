# Linux下服务器rzsz上传限制时如何给服务器上传文件？ - lijunweiyhn的博客 - CSDN博客





2018年10月24日 11:07:00[_Junwei](https://me.csdn.net/lijunweiyhn)阅读数：203








很多时候公司服务器上传文件需要用到rzsz，但是文件大小限制为4GB，这时候需要把一个大的文件压缩成几个小文件，解压时候是一起解压。

命令如下：

**压缩:**

将大文件或目录打包、 压缩并分割成制定大小的文件，在Linux下可以通过组合使用tar,bzip2(或者gzip),split命令来实现。

命令格试 tar zcvf - [ file_directory ] |sqlit -b [ file_size ][ m,k ] - [ file.tar.gz ]



将file目录的文件压缩并分割成每个大小为4G文件





压缩文件格式命令为：

tar zcvf - [想压缩的文件的名字] |split -b [大小] - [想生成的文件名前缀].tar.gz

例：tar zcvf - file_name |split -b 4096m - file_name.tar.gz

ls

-rw-r--r-- 1 root root 4294967296 Mar  9 10:40 file_name.tar.gzaa

-rw-r--r-- 1 root root 4294967296 Mar  9 10:48 file_name.tar.gzab

-rw-r--r-- 1 root root 2282762240 Mar  9 10:52 file_name.tar.gzac



**解压：**

cat file_name.tar.gza* |tar zxv

解释：

用cat来读所有的压缩包，利用tar来进行解压



关于下载：



开发人员在开发时有时会遇到需要在本机和开发机之间传文件的问题。虽然Mac下的scp命令可以完成文件的上传和下载功能，但如果开发机的登陆需要经过跳板机时，scp命令就没有办法正常使用了。

Window上可以用rx，sz命令上传，下载文件，Mac下也可以。下面介绍一下Mac OS下如何配置rz，sz。

### 安装brew

[brew](https://link.jianshu.com?t=https%3A%2F%2Fbrew.sh%2Findex_zh-cn.html)是Mac OS上的一个包管理工具，可以使用它来安装接下来要使用到的iTerm，wget等工具。

Mac默认安装了ruby，打开终端执行如下命令：

```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

Homebrew安装成功后，会自动创建目录 /usr/local/Cellar 来存放Homebrew安装的程序。 这时就可以在命令行状态下面就可以使用 brew 命令了.

### 安装iTerm2

[iTerm](https://link.jianshu.com?t=https%3A%2F%2Fwww.iterm2.com%2F)是一个Mac下的终端工具，非常好用的命令行工具。Mac自带的终端是不支持lrzsz的，需要先下载支持它的iterms。

从官网下载，在本地解压，并将文件拖入Applications，安装完成。

### 安装lrzsz

lrzsz是一款在linux里可代替ftp上传和下载的程序。通过下载它来使用rz，sz。

```
brew install lrzsz
```

### 安装wget

下载lrzsz之后，需要使用`wget`下载iterm2-zmodem。Mac默认不安装wget，可以通过brew安装。

```
brew install wget
```

### 下载iterm2-zmodem

在iTerm2中使用Zmodem传输文件。

```
cd /usr/local/bin

wget https://raw.github.com/mmastrac/iterm2-zmodem/master/iterm2-send-zmodem.sh

wget https://raw.github.com/mmastrac/iterm2-zmodem/master/iterm2-recv-zmodem.sh

chmod 777 /usr/local/bin/iterm2-*
```

如果命令需要权限，尝试在命令前面加上`sudo`。

### 添加trigger

打开一个iTerm终端，点击菜单的Profiles，选择某个profile之后然后继续选择advanced → triggers，添加triggers。
|Regular expression|Action|Action|
|----|----|----|
|**B0100|Run Silent Coprocess|/usr/local/bin/iterm2-send-zmodem.sh|
|**B00000000000000|Run Silent Coprocess|/usr/local/bin/iterm2-recv-zmodem.sh|

### 使用

重启iterm2，连接远程Linux，输入rz命令尝试一下。

```
ssh username@remote.mechine.com
rz
```




作者：RebeccaYan

链接：https://www.jianshu.com/p/1166eb16413a

來源：简书

简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。







