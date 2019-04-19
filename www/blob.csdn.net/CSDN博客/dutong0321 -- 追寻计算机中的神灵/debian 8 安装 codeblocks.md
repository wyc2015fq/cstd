# debian 8 安装 codeblocks - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2015年10月01日 15:03:03[dutong0321](https://me.csdn.net/dutong0321)阅读数：2820
OS：Debian 8
Software:CodeBlocks
我认为CodeBlocks在IDE里面可以称得上是最好的软件之一了，绝对可以进入IDE排名的前三。
我和CodeBlocks结缘于ACM-ICPC中，但是从此以后就离不开了。
写C/C++代码的时候，无论是使用什么编译器或者什么第三方库，都会用这个软件来进行，即使，他们有自己的IDE。
CodeBlocks优点：
  1.所占资源少，对电脑硬件的要求低 
  2.开源软件，可以查看源代码，虽然我不会修改，但是如果我做一个IDE的话，我一定会参考这个伟大的软件的。
  3.最重要的是，它完全跨了三大平台（Windows、Linux、MAC），而且支持的编译器几乎囊括了所有的C/C++编译器，堪称完美。
各平台的安装：
MAC下，我没有使用过，不敢妄妄言，但估计不难。
Windows下，和大部分软件一样，下载安装包，不停的下一步即可。
Linux下，安装比较复杂。我会以Debian为例，但是我已经升级为Debian 8了，当然这里也会简单的讲一下Debian 7。尤其是Debian 8,很有可能无法安装成功。在Debian中，如果我不特别强调，都是用网易的源。在安装前，需要注意的是，无论7还是8都需要安装好GCC/G++/GDB这三个软件包。命令：
`  sudo apt-get install gcc g++ gdb`
Debian 7:
  在Debian 7中可以直接利用apt进行安装，不过安装成功的是CodeBlocks10。命令：
`    sudo apt-get install codeblocks`
  1.首先到codeblocks的主页，依次到Downloads下的Binary Release下，下载debian下的安装包。
  2.解压xz文件后，改变当前文件目录到解压后的文件夹中，然后进行安装，命令：
`    sudo dpkg -i *`
  3.安装后，会发现要求修复依赖（很重要），输入命令：
`    sudo apt-get -f install`  这样安装成功后的版本是CodeBlocks13.12。如果你是从codeblocks10升级过来的，可以省略第三步。当然如果直接安装codeblocks13失败，也可以先安装codeblock10,然后通过第一步和第二步升级到codeblocks13。
Debian 8:
  今天弄了两个多小时才搞定的。
  首先通过安装包进行安装时，是出现这个错误：
```
Unknown media type in type 'all/all'
    Unknown media type in type 'all/allfiles'
    Unknown media type in type 'uri/mms'
    Unknown media type in type 'uri/mmst'
    Unknown media type in type 'uri/mmsu'
    Unknown media type in type 'uri/pnm'
    Unknown media type in type 'uri/rtspt'
    Unknown media type in type 'uri/rtspu'
```
  经过搜索，到[Unknown media type in type 'all/all' 问题解决方案](http://blog.csdn.net/g457499940/article/details/17008769)此处发现解决方案：将/usr/share/mime/packages/kde.xml中的<mime-type
 type="xxxx/xxxx"> 到</mime-type>格式的语句全部删除。删完后就没多少了，建议删除以前先备份一下。
  解决后，然后出现了新的错误：
```
dpkg: dependency problems prevent configuration of codeblocks:
     codeblocks depends on libwxbase2.8-0 (>= 2.8.12.1); however:
      Package libwxbase2.8-0 is not installed.
     codeblocks depends on libwxgtk2.8-0 (>= 2.8.12.1); however:
      Package libwxgtk2.8-0 is not installed.
```
  然后利用apt进行安装，通过apt-cache search发现目前的版本是3.0，安装，安装后发现依然是上述错误，然后就郁闷了，说好大于2.8就行吗，3.0怎么就不可以呢？我去哪给你找2.8去啊？？？然后忽然，我想明白了。gtk2.0和gtk3.0是不一样的，看来debian是不支持gtk2.0了。Debian啊，你怎么可以学习微软那个bitch呢？说不支持就不支持了呢？
  但是，人家既然不支持了，我就搜索一下吧，然后发现我大天朝竟然没有人遇到这个问题，无奈，去官网找找解决方案吧。
  结果终于在官网的某个地方发现了这个链接：[Jens' (un-)official debian-repository for code::blocks IDE](http://apt.jenslody.de/)，看域名，貌似是德国的，竟然没有被墙，不幸中的万幸啊。
  解决方案看这里：
  1.因为在软件源中要用到HTTPS进行传输，所以，首先安装apt-transport-https，否则会出现：
```
E: The method driver /usr/lib/apt/methods/https could not be found.
    N: Is the package apt-transport-https installed?
```
   命令：`    sudo apt-get install apt-transport-https`  2.改变软件源，先将以前的软件源备份好，注释掉正在使用的软件源，添加下面的软件源：
  稳定版：
```
deb [arch=amd64,i386] https://apt.jenslody.de/stable jessie main
    deb-src https://apt.jenslody.de/stable jessie main
```
  测试版：
```
deb [arch=amd64,i386] https://apt.jenslody.de/testing stretch main
    deb-src https://apt.jenslody.de/testing stretch main
```
  3.更新软件源，安装Debian Keyring，命令：
```
sudo apt-get update
    sudo apt-get install jens-lody-debian-keyring
```
  4.进行安装，命令：`    sudo apt-get install codeblocks`  5.移除已经没有用的Keyring，命令：`    sudo apt-get remove jens-lody-debian-keyring`
  6.恢复原来的软件源，更新软件源，命令：
`    sudo apt-get update`
  7.修复软件的依赖，命令：
`    sudo apt-get -f install`好了，到此为止，安装成功了。虽然版本处显示的是svn 10512 。但是，还是和codeblocks13.12一样一样的。
如果，有疑问，欢迎留言。
另：关于Debian 7，我是凭记忆写的，如果有不对的地方，可以留言纠正，谢谢了。
