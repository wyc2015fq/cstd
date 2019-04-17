# Docker for Windows在windows系统中安装不成功解决方法 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年08月30日 17:57:54[boonya](https://me.csdn.net/boonya)阅读数：3950








下载都知道去哪里下自己先准备吧：[https://store.docker.com/editions/community/docker-ce-desktop-windows](https://store.docker.com/editions/community/docker-ce-desktop-windows)

需要登录下载，自己注册一个账号即可。

### 安装失败的可以参考下面的方式来解决方法

1、检查Windows版本是否支持Hyper-V,可以在控制面板中启用此功能。

2、如果没有需要通过命令来安装Hyper-V，前提是需要卸载掉系统里面的虚拟机如：VMWare或Oracle VirtualBox。

3、安装支持Hyper-V的系统版本:如window10专业版（企业版和家庭版似乎不支持Hyper-V）,Windows可采用更新或者重装系统来实现。

### Docker安装成功后验证

注意：安装失败了使用docker version指令仍然有可能执行成功，所以不妨多验证几个指令。

![](https://img-blog.csdn.net/20180830174637684?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

docker ps  查看运行的容器实例

![](https://img-blog.csdn.net/20180830174901881?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

docker images 查看本地构建镜像或已下载镜像

![](https://img-blog.csdn.net/2018083017495769?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###  Kitematic镜像和容器管理客户端

在Windows下安装docker for windows成功后，运行的图标右键可以找到下载解压即可使用。

下图是正在运行的容器和镜像：

![](https://img-blog.csdn.net/2018083017534086?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

容器管理和操作：

![](https://img-blog.csdn.net/20180830175603519?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下图可以更为直观地看宿主与虚拟路径的对应关系：

![](https://img-blog.csdn.net/20180830175718730?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



