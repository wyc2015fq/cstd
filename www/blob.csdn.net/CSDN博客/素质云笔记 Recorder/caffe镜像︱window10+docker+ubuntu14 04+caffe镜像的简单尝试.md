
# caffe镜像︱window10+docker+ubuntu14.04+caffe镜像的简单尝试 - 素质云笔记-Recorder... - CSDN博客

2017年03月05日 14:16:33[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：3415



> win10专业版可以利用Hyper-V开启docker，一般升级而来的都是家庭版，现在要升级到win10专业版， 需要产品秘钥。

> github网址：
> [https://github.com/docker/for-win](https://github.com/docker/for-win)

> .

> 一、Docker for Windows

> 安装的教程可以看：
> [http://blog.csdn.net/zdy0_2004/article/details/52084452](http://blog.csdn.net/zdy0_2004/article/details/52084452)

> 大致的流程：

> 1、下载Docker for Windows

> [https://download.docker.com/win/stable/InstallDocker.msi](https://download.docker.com/win/stable/InstallDocker.msi)

> 2、开启win10的Hyper-V

> 控制面板 -> 程序 -> 启用或关闭Windows功能 -> 选中Hyper-V

> 3、安装Docker

> 用刚才下载的安装包安装，安装完成后，启动Docker

> 如果没有开启Hyper-V，启动Docker的时候会提示开启Hyper-V

> 4、使用Docker

> window，win+R,打开CMD键入后续内容：

> 查看版本等信息

> docker
> info
> docker
> run
> hello-world
> run hello-world运行成功之后，就代表安装完成了。但是，可能容易链接不到，要多执行几次。

> 报错内容为：

> docker: Error response
> from
> daemon: Get
> https
> ://registry-
> 1.
> docker.io/v2/library/hello-world/manifests/latest: Get
> https
> ://auth.docker.io/
> token
> ?scope=repository%
> 3
> Alibrary%
> 2
> Fhello-world%
> 3
> Apull&service=registry.docker.io: net/
> http
> : TLS handshake timeout.
See
> 'docker run --help'
> .
> .

> 二、安装ubuntu14.04

> 之前的博客直接用以下命令：

> docker
> run
> -
> it
> ubuntu bash
> 不能登录上，因为调取的外网。所以可以用国内Daocloud下载一个”加速器“，也有人推荐阿里云的容器镜像平台（
> [来自云端的容器镜像服务](https://dev.aliyun.com/search.html)
> ），另外也发现了一个比较好的开源镜像平台：
> [清华大学开源软件镜像站](https://mirrors.tuna.tsinghua.edu.cn/)

> 1、登录Daocloud+注册

> [https://cr.console.aliyun.com/](https://cr.console.aliyun.com/)

> 2、设置加速器

> 进入控制面板，点击加速器：

> 加速器地址：
> [https://www.daocloud.io/mirror\#accelerator-doc](https://www.daocloud.io/mirror#accelerator-doc)
![这里写图片描述](https://img-blog.csdn.net/20170305140250265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170305140250265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 然后右键右下角的docker，在桌面右下角状态栏中右键 docker 图标，加到”registry-mirrors”的数组里。点击 Apply 。

> [
](https://img-blog.csdn.net/20170305140250265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170305140400642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170305140400642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 设置好之后会重启

> [

](https://img-blog.csdn.net/20170305140400642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 3、直接加载Daocloud各类镜像

> [
](https://img-blog.csdn.net/20170305140400642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 镜像仓库里面有很多人上传的镜像，直接执行：

> [

](https://img-blog.csdn.net/20170305140400642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> docker run daocloud
> .
> io/ubuntu:
> 14.04
> grep
> -v
> '^\#'
> /etc/apt/sources
> .
> list
> [
](https://img-blog.csdn.net/20170305140400642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 就可以获得
> [ubuntu14.04的镜像](https://dashboard.daocloud.io/packages/4983bbad-a6eb-46c6-abb3-f8e3ff35b2c8)
> ，

> 同时我也发现了
> [caffe的镜像](https://hub.daocloud.io/repos/db7ebaa3-1244-49f0-884d-36f549187e4f)
> ：

> docker pull daocloud.
> io
> /zjxuan/caffe:latest
> 4、运行镜像

> docker
> run
> -
> it
> daocloud.io/zjxuan/caffe:latest
> 其中笔者打开镜像后，看到python里面是可以成功运行

> import
> caffe
> 可以新手试试

> 键入以上命令进入镜像，不过出现了很多报错：

> : Failed to fetch http://archive.ubuntu.com/ubuntu/dists/precise-updates/main/binary-i386/Packages
> Something wicked happened resolving
> 'archive.ubuntu.com:http'
> (-5 - No address associated with hostname)  
W: Failed to fetch http://archive.ubuntu.com/ubuntu/dists/precise-updates/restricted/binary-i386/Packages  Something wicked happened resolving
> 'archive.ubuntu.com:http'
> (-5 - No address associated with hostname)
> 有人说的解决方案是：设置妥当DNS服务器即可，没成功（
> [http://blog.csdn.net/npy_lp/article/details/42080553](http://blog.csdn.net/npy_lp/article/details/42080553)
> ）

> .

> 延伸一：设置docker的内存/CPU配比

> 右键右下角的小鲸鱼，然后settings，advanced就可以看到了。

> .

> 延伸二：docker的一些简单教程

> 可以见博客：
> [Docker简明教程](http://kb.cnblogs.com/page/536115/)

> [Docker入门教程](http://www.docker.org.cn/book/docker/what-is-docker-16.html)

> [DaoCloud Services 文档](http://guide.daocloud.io/dcs)


