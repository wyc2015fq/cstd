# [如何使用docker部署c/c++程序](https://www.cnblogs.com/hailun1987/p/9697236.html) 		



**原文地址：https://blog.csdn.net/len_yue_mo_fu/article/details/80189035**

**Docker介绍**

Docker是一个开源的容器引擎，它有助于更快地交付产品。Docker可将应用程序和基础设施层隔离，并且将基础设施当作程序一样进行管理。使用Docker，可以更快地打包，测试以及部署应用程序，并可以缩短从编程到部署运行代码的周期 

**docker部署c/c++程序** 

​        关于docker的使用网上有很多的教程但是很少有介绍如何使用docker来部署一个c/c++程序，笔者作为一个c/c++程序员，在学习docker的时候没有找到相关的使用，经过博主这几天的研究使用终于在docker中成功的运行了c/c++程序，下面博主就来介绍一下使用方法：  想要把c/c++程序运行在docker的容器中，就必须先创建一个docker镜像，通过镜像创建容器，来使我们的程序在容器中运行起来。

\#简单介绍一下docker中镜像和容器的关系：

 • Images (镜像)

 Docker镜像是一个只读模板，包含创建Docker容器的说明。Docker镜像可以运行Docker镜像中的程序。 

• Container （容器）

 容器是镜像的可运行实例。镜像与容器类似与面向对象中类与对象的关系。可通过Docker API或者CLI命令起停，移动，删除等。

 明白了docker中镜像和容器的关系之后，我们想要把程序执行起来，其实就是将程序放在镜像中，通过镜像启动一个容器，在容器中执行我们的程序。 

那么我们运行一个c/c++程序到底该选择怎么样的镜像呢？其实也很简单，我们只要知道我们的程序如果不使用docker他是在什么系统或者说是环境中使用，那么我们就可以通过docker官方的仓库去下载这样的镜像来供我们创建包含我们程序的镜像了。 

我们现在有这样的一段代码，功能就是给一个叫t.txt的文件中写hello world!!!。

下面我们就通过这个简单的代码来示范如果把一个c/c++程序放到docker镜像中制作一个新的镜像

```
`#include <stdio.h>``#include <stdlib.h>``#include <string.h>``#include <unistd.h>`  `int` `main()``{``    ``FILE``* file = ``fopen``(``"t.txt"``,``"w+"``);``    ``if` `(file == NULL)``    ``{``        ``return` `0;``    ``}` `    ``char` `buf[20]=``"hello world!!!\n"``;``    ``int` `len = ``strlen``(buf);` `    ``while``(1)``    ``{``        ``fputs``(buf,file);``        ``fflush``(file);``    ``//  printf("%s",buf);``        ``sleep(1);``    ``}` `    ``fclose``(file);` `    ``return` `0;``}`
```

 **1. 镜像的选择：**

   c/c++程序我们都是直接运行在linux系统上，所以我们可以直接选择centos或者ubuntu镜像，在这里我们是要把.c文件编译成一个可执行程序，docker还有一个gcc或者g++的镜像，使用gcc或者g++镜像的话，我们就不需要在ubunt或者centos镜像中再安装gcc  、g++了。

 

推荐三种镜像：

\1. gcc 或者g++

\2. ubuntu

\3. centos 

 

**2. 下载镜像到宿主主机：**

 

```
查看版本：

docker search gcc
```

 下面是执行结果： 

![img](https://img2018.cnblogs.com/blog/359921/201809/359921-20180924225052722-1545685444.png)

 

 这里我们选择第一个下载：

 //下载镜像 

docker pull gcc 

//查看本地已经下载的镜像

 docker images 

 ![img](https://img2018.cnblogs.com/blog/359921/201809/359921-20180924225127940-149553427.png)

**3. 使用gcc镜像制作我们自己的镜像：**

 制作镜像有两种方式，在这里就不做赘述了，可以查看博主单独接受制作镜像的博客： 在这里我们通过Dockerfile的方式创建自己的镜像,下面是Dockerfile的内容： 

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 FROM gcc:latest
 2 
 3 RUN  mkdir /usr/src/myapp
 4 
 5 COPY test.c /usr/src/myapp
 6 
 7 WORKDIR /usr/src/myapp
 8 
 9 RUN  gcc test.c 
10 
11 CMD ["./a.out"]
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

执行命令：

 docker build -t mygcc-test:v1 .

 //再次执行 

docker images 

//这时我们就可以看到在本地镜像中多了一个叫做mygcc-test的镜像，TAG为v1 

 ![img](https://img2018.cnblogs.com/blog/359921/201809/359921-20180924225342255-552828015.png)

**4. 启动制作好的镜像：**

 //执行命令： 

docker run -d mygcc-test:v1 

//启动镜像

 //执行命令： 

docker ps                           //查看容器的运行情况 

 ![img](https://img2018.cnblogs.com/blog/359921/201809/359921-20180924225449230-1804661781.png)

 //接下来我们进入到容器当中，查看a.out程序在后台执行的情况：

 docker exec -it mystifying_mclean /bin/bash 

进入到容器之后我们执行top命令,查看a.out： 

![img](https://img2018.cnblogs.com/blog/359921/201809/359921-20180924225556954-150149383.png)

查看当前目录发现已经生成了t.txt文件说明，a.tou程序执行正常：

 ![img](https://img2018.cnblogs.com/blog/359921/201809/359921-20180924225640069-1443314370.png)

 

 **进阶教程：**

 **1.直接将一个可执行程序复制到镜像中，制作镜像**

 还是刚才那个test.c文件，我们直接在本地虚拟机，将他编译成可执行程序a.out，下面是Dockerfile的内容：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
FROM gcc:latest

RUN  mkdir /usr/src/myapp

COPY a.out /usr/src/myapp

WORKDIR /usr/src/myapp

#RUN  gcc test.c 

CMD ["./a.out"]
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

我们通过下面命令制作一个名字是mygcc-test，但是tag是v2的镜像 

**docker build -t mygcc-test:v2 .**

 镜像制作好了之后，我们再运行一个该镜像的容器，还是通过上面的命令，在这里就不列出来了，直接上截图了，看看效果：

![img](https://img2018.cnblogs.com/blog/359921/201809/359921-20180924225917512-1633704578.png)

还是top，查看a.out是否在后台运行，在查看t.txt的执行情况: 

 ![img](https://img2018.cnblogs.com/blog/359921/201809/359921-20180924225948822-1640865758.png)

**2.将一个启动之前需要设置环境变量的可执行程序复制到镜像中，制作镜像：**

​       现在我们有DataImportClient.tar.gz这样的一个可执行程序压缩包，解压之后的目录如下，lib是存放client程序所依赖的动态库，startup.sh是启动client程序的脚本,在startup.sh启动之前我们首先会设置环境变量LD_LIBRARY_PATH：  

![img](https://img2018.cnblogs.com/blog/359921/201809/359921-20180924230123939-1956539219.png)

我们还是通过Dockerfile 来创建一个镜像，这次我们选用ubuntu作为基础镜像，Dockerfile的内容如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
FROM ubuntu:latest

ADD ./DataImportClient.tar.gz  /usr/src/myapp

WORKDIR /usr/src/myapp/DataImportClient

CMD ["./startup.sh"]
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

直接上截图了，通过上面的方法我们发现我们期待的结果并没有出现，docker ps 发现什么也没有： 

 ![img](https://img2018.cnblogs.com/blog/359921/201809/359921-20180924230229433-1632336832.png)

这就很让人困扰了，按理说通过./startup.sh会将环境变量设置，并且执行./client，但是我们事实却并不是这样，我们查看日志，发现退出的原因是找不到动态库：

 ![img](https://img2018.cnblogs.com/blog/359921/201809/359921-20180924230259140-57444938.png)

其实在docker中一个容器运行的时间长短，其实就是CMD后面跟的命令的执行时间，startup.sh就是一个脚本，执行结束之后就退出了，所以容器很快也就退出了。这个地方环境变量没有设置成功的可能原因是，docker容器总执行shell命令可能不是在同一个终端，也就是不是同一个进程中，设置了变量在./client中并不能生效。  startup.sh的内容如下：

```
#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib

./client &
```

但是我们观察COMMAND发现命令是“./client”,所有可以大致推断出，CMD一定是将startup中的命令，分成多个CMD来执行，

但是Dockerfile的语法是，CMD只执行最后一个，这样就能解释为什么找不到动态库的原因了。 

 ![img](https://img2018.cnblogs.com/blog/359921/201809/359921-20180924230415746-902796431.png)

正确的Dockerfile： 

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
FROM ubuntu:latest

ADD ./DataImportClient.tar.gz  /usr/src/myapp

WORKDIR /usr/src/myapp/DataImportClient

ENV  LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH.

#RUN echo $LD_LIBRARY_PATH

CMD ["./client"]
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  通过ENV来设置环境变量LD_LIBRARY_PATH 创建镜像之后，我们再次启动一个容器，看看效果：

 ![img](https://img2018.cnblogs.com/blog/359921/201809/359921-20180924230557237-756536869.png)

以上就是如何使用docker来部署一个c/c++的程序了 

原文地址：https://blog.csdn.net/len_yue_mo_fu/article/details/80189035