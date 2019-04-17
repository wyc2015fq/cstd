# Windows通过DockerToolBox安装轻量级Linux系统Boot2Docker - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月04日 09:31:43[boonya](https://me.csdn.net/boonya)阅读数：513标签：[DockerToolBox																[docker																[boot2docker](https://so.csdn.net/so/search/s.do?q=boot2docker&t=blog)
个人分类：[Docker](https://blog.csdn.net/boonya/article/category/6559498)





Windows通过DockerToolBox安装Boot2Docker也是一个不错的选择。docker kvm虚拟化服务无所不在，这为我们开发、测试、生产都提供了很多便利，因此有必要好好学习下Docker。文章主要讲述在Windows下使用Boot2Docker进行安装。


## Boot2Docker

最新版 Boot2Docker 下载地址：[https://github.com/boot2docker/windows-installer/releases/latest](https://github.com/boot2docker/windows-installer/releases/latest)

## DockerToolBox

### 下载

官方地址：[https://www.docker.com/products/docker-toolbox](https://www.docker.com/products/docker-toolbox) 下载后安装即可。

![](https://img-blog.csdn.net/20170704095737464)


### 安装

安装很简单，自己选定目录一步步操作即可，最终得到两个操作快捷图标，如下图：


![](https://img-blog.csdn.net/20170704092330810)

安装后我们主要用Docker Quickstart Terminal。

启动后会去下载最新的Boot2Docker，有可能会失败，失败可以手动添加到下载目录下，如：C:\Users\Administrator.louyi-PC\.docker\machine\cache

将boot2docker.iso放于其目录下就可以正常启动了，如图：

![](https://img-blog.csdn.net/20170704092919036)

在宿主机器ping 192.168.99.100：

![](https://img-blog.csdn.net/20170704100341135)

至此，DockerToolBox的boot2docker安装成功。](https://so.csdn.net/so/search/s.do?q=docker&t=blog)](https://so.csdn.net/so/search/s.do?q=DockerToolBox&t=blog)




