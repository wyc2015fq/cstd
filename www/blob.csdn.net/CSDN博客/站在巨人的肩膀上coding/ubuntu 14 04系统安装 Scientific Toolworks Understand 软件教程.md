# ubuntu 14.04系统安装 Scientific Toolworks Understand 软件教程 - 站在巨人的肩膀上coding - CSDN博客





2018年07月23日 21:06:09[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：330








    为了能方便的看代码，想安装 Scientific Toolworks Understand 软件，以图表形式学习代码编写，可是整了将近一天时间，总算满意了。诶，走的弯路太多，现在把具体教程分享如下：

       参考网址：[http://blog.csdn.net/u011722133/article/details/52742599](http://blog.csdn.net/u011722133/article/details/52742599)

     首先说明一下，笔者是ubuntu 14.04（64位）操作系统，安装Understand-3.1.670-Linux-64bit。

一、安装包准备

       还要说明一点，这个软件类似windows里的破解软件，需要破解。所以，在ubuntu里安装比较麻烦，和以往安装方式不同，也因此，笔者走了许多弯路。

1、方式一

（1）安装包下载

part1：[http://download.csdn.net/detail/p942005405/9711128](http://download.csdn.net/detail/p942005405/9711128)

part2：[http://download.csdn.net/detail/p942005405/9711134](http://download.csdn.net/detail/p942005405/9711134)

（2）一般通过默认安装的ubuntu是不能解压rar文件的，只有在安装了rar解压工具之后，才可以解压。其实在ubuntu下安装rar解压工具是非常简单的，只需要两个步骤就可以迅速搞定。

ubuntu 下rar解压工具安装方法：

```
$   sudo apt-get install rar

$  sudo apt-get install unrar
```



（3）把两个RAR文件放在同一个目录下，用rar命令解压其中一个RAR文件，会自动找到另一个RAR文件，开始提取，最后生成Understand-3.1.670-Linux-64bit.tgz文件。

2、方式二（推荐）

      直接下载 Understand-3.1.670-Linux-64bit.tgz文件：[https://pan.baidu.com/s/1i52nrut](https://pan.baidu.com/s/1i52nrut)

      感谢博主提供资源！

二、开始安装

       因为要破解，涉及读写liscense key，所以不能安装在/opt目录下（笔者没有尝试更改文件属性，类似windows下的C盘，还是不要动的比较好），我在/home目录下新建了一个application文件夹，利用命令：
`$      sudo nautilus`
      更改文件夹属性，和主文夹（即username文件夹）保持一致。安装方法也有两个：

方式一（推荐）：直接“右击”---> "提取到此处"，生成scitools文件。

方式二：  
`$   gzip -cd Understand-3.1.670-Linux-64bit.tgz | tar xvf -`
三、破解

1、添加路径
`$     gedit ~/.bashrc`
 打开启动文件，添加如下两条命令：

```
export PATH=$PATH:/home/application/scitools/bin/linux64
STIHOME=/home/application/scitools
```

2、运行、破解

    命令行定位到安装目录：

```
$   cd  /home/application/scitools/bin/linux64
$    ./understand
```

![](https://img-blog.csdn.net/20170317143939726?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzYzNTU2NjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

  点击中间的命令框Add Permanent Liscense，再点击Add Eval or SDL (RegCode)，如下：

![](https://img-blog.csdn.net/20170317143845974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzYzNTU2NjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    输入证书CODE(32/64 都可用)：09E58CD1FB79

![](https://img-blog.csdn.net/20170317144214989?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzYzNTU2NjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

         这样就能顺利的运行understand了。



