# docker下搭建fastfds集群版 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月17日 20:42:18[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：1363


搭建过程参考 作者 [https://me.csdn.net/feng_qi_1984](https://me.csdn.net/feng_qi_1984) 的课程视频

声明：集群版是在我之前写的单机版基础之上进行搭建的，我将安装了fastfds单机版的docker打包成了镜像。

如对fastfds搭建流程不熟悉者，需先参阅之前的文章。[点击进入文章](https://blog.csdn.net/weixin_40247263/article/details/80947069)

首先借鉴一个公众号的两张图来说明一下 fastfds 上传下载的流程，公众号为 "Java架构师之路"

![](https://img-blog.csdn.net/20180717202107754?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180717202131146?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我搭建的思路是，两个tracker、四个storage，然后通过nginx进行反向代理访问图片。

![](https://img-blog.csdn.net/2018071720231940?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面演示搭建步骤

先配置几个虚拟ip

![](https://img-blog.csdn.net/20180717203004820?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

运行上次打包的镜像，创建docker容器

docker run --privileged=true -itd -v /soft/:/back -p 192.168.46.230:80:80 -p 192.168.46.230:22122:22122 -p 192.168.46.230:8080:8080 529c212ff162 /bin/bash

进入容器

docker exec -it 0d581e092e1d /bin/bash

（一）tracker

集群版中 配置 tracker 修改tracker.conf文件

# cd /etc/fdfs/

# vi tracker.conf

修改 store_lookup=2 （0代表轮询,1代表固定group,2代表负载均衡)

![](https://img-blog.csdn.net/20180717203048875?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

启动 tracker

# service fdfs_trackerd start

查看一下

# netstat -unltp | grep fdfs

![](https://img-blog.csdn.net/20180717203121896?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

备用 tracker 进行相同操作

（二）storage

配置storage的配置文件

# cd /etc/fdfs/

# vi storage.conf

修改tracker_server

配置group_name 我的是两个1两个2

![](https://img-blog.csdn.net/20180717203153739?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

把主备两个tracker都配置上

tracker_server=172.17.0.34:22122

tracker_server=172.17.0.36:22122

![](https://img-blog.csdn.net/20180717203249879?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

修改/etc/fdfs/目录下的mod_fastdfs.conf

connect_timeout=10

tracker_server 主备两个都要配置       且更改  group_count = 2

![](https://img-blog.csdn.net/20180717203321818?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我是设置了两个组 每个组两个storage

再配置group1和group2

![](https://img-blog.csdn.net/20180717203407149?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

修改nginx配置文件

# cd /usr/local/nginx/conf/

# vi nginx.conf

listen 8888; 由于storage的端口号为8888 所以这里要修改

如果是group2的话 需要修改group2

![](https://img-blog.csdn.net/20180717203447157?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

修改完以后 保存退出

将刚刚配置好的storage容器 打包成镜像

docker commit f790145a83a2 storage:v1

然后依旧是端口号映射、硬盘挂载创建容器，注意要更改ip地址，然后用新打包好的镜像

docker run --privileged=true -itd -v /soft/:/back -p 192.168.46.235:80:80 -p 192.168.46.235:23000:23000 -p 192.168.46.235:8888:8888 cea52388dba7 /bin/bash

注意 这里的 端口号 做了 修改

安装好以后 进入容器。 直接启动 storage服务 和 nginx 服务就行了

然后继续 安装 容器

这次进入以后 将 storage.conf 和 mod_fastdfs.conf 中的 groupname 修改为 group2

storage.conf

![](https://img-blog.csdn.net/20180717203508816?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

mod_fastdfs.conf

![](https://img-blog.csdn.net/20180717203710206?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

nginx 的配置文件 也要修改

![](https://img-blog.csdn.net/20180717203733575?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

再 启动 storage 服务 和 nginx 服务

然后再 安装一个容器 同上

安装并配置完以后

在cd /usr/bin 这个 文件夹下面 有很多可执行的脚本。

我们通过fdfs_monitor /etc/fdfs/storage.conf 命令

可以查看到 以下内容

![](https://img-blog.csdn.net/2018071720382793?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

再往下 可以看到 分别有哪些组，每个组里有哪些storage 以及这些storage的状态

（三） 测试一下

进入 主 tracker 也就是 第一个配置的那个 tracker

修改 /etc/fdfs/client/conf 文件

![](https://img-blog.csdn.net/20180717203908596?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# /usr/bin/fdfs_test /etc/fdfs/client.conf upload /back/3.jpg

![](https://img-blog.csdn.net/20180717203935910?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

添加路由

![](https://img-blog.csdn.net/20180717203956470?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

复制地址，输入端口号 8888 可以访问

![](https://img-blog.csdn.net/20180717204012296?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过linux的ip也可以访问

![](https://img-blog.csdn.net/20180717204028999?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

由于我这个是group2，那么group2中的另一个 storage中应该也存放了该图片

![](https://img-blog.csdn.net/20180717204052688?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上传虽然能成功 但是访问的时候IP都不同 所以访问起来很不方便 可以再用一个nginx做一个反向代理

# cd /usr/local/nginx/conf/

# vi nginx.conf

![](https://img-blog.csdn.net/201807172041203?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

启动 nginx

就可以通过 nginx 的ip地址 加上后面的 路径 访问图片了

