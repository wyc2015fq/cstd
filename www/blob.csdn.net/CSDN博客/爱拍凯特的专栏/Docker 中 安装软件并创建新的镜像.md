# Docker 中 安装软件并创建新的镜像 - 爱拍凯特的专栏 - CSDN博客
2016年06月14日 15:14:44[爱拍凯特](https://me.csdn.net/apacat)阅读数：5241
个人分类：[Docker](https://blog.csdn.net/apacat/article/category/6244421)
1、在docker中根据已有的镜像然后安装新的软件在生成新的镜像
docker中镜像是个静态的概念，而容器是一个动态的概念，自我感觉有点像程序和进程之间的关系似的。
现在已经有的镜像是  learn/tutorial  现在安装 ping 和vim 生成自己的新的镜像
首先根据镜像运行-->容器并安装软件ping
#docker run -t  learn/tutorial  apt-get install -y ping 
或者先运行容器在安装
#docker run -t -i learn/tutorial
然后 运行
apt-get Install ping
apt-get install vim
如果这个时候会报错提示：
Reading package lists... Done
Building dependency tree       
Reading state information... Done
E: Unable to locate package vim
这是因为镜像中的apt列表不是最新的软件包列表，没有同步导致
这时候需要敲：**apt-get update**，这个命令的作用是：同步 /etc/apt/sources.list 和 /etc/apt/sources.list.d 中列出的源的索引，这样才能获取到最新的软件包。然后在执行
 apt-get install vim 就可以了。这个时候容器中就包含了所需的软件，但是容器结束之后是没有保存的。
在执行 **docker commit 容器ID 新的镜像名(假如是ping-vim)** 就可以添加一个新的镜像，下次如果想在镜像中运行ping 程序就可以直接使用命令
**docker run -i -t （--name=vim-ping，可选） ping-vim ping www.baidu.com**
就可以了。
