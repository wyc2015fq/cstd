# Docker容器化虚拟机：抛弃传统虚拟机 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年10月12日 15:35:29[boonya](https://me.csdn.net/boonya)阅读数：642








Win10 Docker与VMware和Oracle VirtualBox冲突，使得在包含有VMware或Oracle VirtualBox的主机上不能成功安装Win10 Docker，同理在Win10 Docker机器上也不能正常使用VMware和Oracle VirtualBox。Windows 10 Docker依赖于Hyper-V这是致命的冲突。这里我们就必须完全依靠docker模拟的虚拟机功能了。

### 测试运行冲突

VMware 不能正常启动：

![](https://img-blog.csdn.net/20181012153025730?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Oracle VirtualBox不能启动虚拟机实例：

![](https://img-blog.csdn.net/20181012153231470?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

所以，即使我们卸载并重装了 VMware和Oracle VirtualBox，系统仍然不能正常运行。

**注：如果想在本机使用虚拟机的方式，只要安装了Win10 Docker 就不要再妄想了。**

**那我们怎么使用虚拟机呢，答案还是Docker给我们的，只需要下载对应的Linux版本镜像运行就好了。**

### Docker 容器虚拟机

下面我们来操作启动一个Docker的CentOS容器实例.

1、查看本地 镜像:  docker images

![](https://img-blog.csdn.net/2018101215525596?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2、查询远程仓库镜像：docker search centos

![](https://img-blog.csdn.net/20181012155401435?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3、拉取仓库镜像：docker pull centos

![](https://img-blog.csdn.net/20181012155632635?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4、运行centos ：docker run -it --name centos -d centos

![](https://img-blog.csdn.net/20181012155822159?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

5、进入“虚拟机” 容器：docker exec -it ae84f7093fe1 /bin/bash

![](https://img-blog.csdn.net/20181012160028898?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

6、挂载本机目录到“虚拟机”容器 ：docker run -it -v D:\DockerWorkspace\vm-centos:/boonya --name centos2 -d centos

![](https://img-blog.csdn.net/20181012161259446?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

以后虚拟机需要的文件都可以放在 D:\DockerWorkspace\vm-centos下了，自动会同步到虚拟机容器。如下 所示：

![](https://img-blog.csdn.net/20181012161513308?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20181012161531380?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

操作比虚拟机快太多了：

![](https://img-blog.csdn.net/20181012162406872?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Docker里面装Docker也是可以的：

![](https://img-blog.csdn.net/20181012162644996?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**注：唯一不足的是Docker镜像做了瘦身，少了些依赖的环境安装程序。**



