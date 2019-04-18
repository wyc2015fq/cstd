# docker下搭建fastFDS单机版（附storage端口号不显示解决方案） - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月06日 22:56:50[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：953


本文参考：[点击打开链接](https://blog.csdn.net/feng_qi_1984/article/details/69803249)

并进行一些改正，附解决方案。

1 下载FastDFS的依赖包

[https://download.csdn.net/download/weixin_40247263/10525946](https://download.csdn.net/download/weixin_40247263/10525946)

2 解压后上传到Linux上

 yum -y install tar  下载解压工具

3 解压libfastcommon-1.0.7.tar.gz 并编译

# tar -zxvf libfastcommon-1.0.7.tar.gz 

# cd libfastcommon-1.0.7 

# ./make.sh

  (若此处报错，可能是没有安装gcc 无法编译 )

   # yum -y install gcc

    		 # yum -y install perl

   然后再运行 yum -y install perl则成功  

# ./make.sh install

libfastcommon.so默认安装到了/usr/lib64/libfastcommon.so，而FastDFS主程序设置的lib目录是/usr/local/lib，所以设置软连接

# ln -s /usr/lib64/libfastcommon.so /usr/local/lib/libfastcommon.so

# ln -s /usr/lib64/libfastcommon.so /usr/lib/libfastcommon.so

# ln -s /usr/lib64/libfdfsclient.so /usr/local/lib/libfdfsclient.so

# ln -s /usr/lib64/libfdfsclient.so /usr/lib/libfdfsclient.so 

4 解压FastDFSV5.05.tar.gz 并编译

# tar -zxvf FastDFSV5.05.tar.gz

# cd fastdfs-5.05

# ./make.sh     或许不用输入这行

       # ./make.sh install

FastDFS的默认配置文件放在了/etc/fdfs中我们可以进入目录看一下

# cd /etc/fdfs

现在里面有三个配置文件,我们需要各copy一份把.sample后缀名去掉

# cp tracker.conf.sample tracker.conf

# cp storage.conf.sample storage.conf

# cp client.conf.sample client.conf

5 配置FastDFS中的 tracker

配置文件中需要配置一个basepath路径 我们可以先提前创建出该路径

# mkdir -p /data/fdfs

修改tracker.conf配置文件 主要修改basepath路径和http.server_port即可 

# vi tracker.conf

base_path=/data/fdfs

http.server_port=80

创建软连接

        #  ln -s /usr/bin/fdfs_trackerd /usr/local/bin

 #  ln -s /usr/bin/stop.sh /usr/local/bin

 #  ln -s /usr/bin/restart.sh /usr/local/bin 

启动服务 

       # service fdfs_trackerd start

6 配置FastDFS中的storage

创建storage存放路径 

# mkdir -p /data/fastdfs/storage

修改 storage.conf 配置文件

# vi storage.conf

base_path=/data/fastdfs/storage

store_path0=/data/fastdfs/storage

tracker_server=trackerIP:22122  （ip ： 172.17.0.12 docker的ip地址）

注意此处有坑  如果是客户端是外网那么此处一定要用外网IP  不允许用 本机ip

因为 是 tracker 告诉 eclipse 去哪找 storage 。 

创建软连接 

# ln -s /usr/bin/fdfs_storaged /usr/local/bin

启动服务 

# service fdfs_storaged start

至此 tracker和storage都已经启动了

7  解压nginx-1.8.0.tar.gz 和 fastdfs-nginx-module_v1.16.tar.gz 并编译 

# tar -zxvf nginx-1.8.0.tar.gz 

# tar -zxvf fastdfs-nginx-module_v1.16.tar.gz

Nginx 需要几个依赖

安装gcc  # yum -y install gcc-c++

安装 pcre  # yum -y install pcre pcre-devel

安装 zlib # yum -y install zlib zlib-devel

安装 openssl # yum -y install  openssl openssl-devel

创建一个临时目录  

# mkdir -p /var/temp/nginx

进入Nginx目录

# cd nginx-1.8.0

进入目录fastdfs-nginx-module/src

# cd  /soft/fastdfs-nginx-module/src

修改配置文件  

# vi config

CORE_INCS="$CORE_INCS /usr/local/include/fastdfs /usr/local/include/fastcommon/"

改成

CORE_INCS="$CORE_INCS /usr/include/fastdfs /usr/include/fastcommon/"

很重要  否则Nginx编译会报错 

切回到nginx目录下

# cd /soft/nginx-1.8.0

执行 configure命令

# ./configure \

--prefix=/usr/local/nginx \

--pid-path=/var/run/nginx/nginx.pid \

--lock-path=/var/lock/nginx.lock \

--error-log-path=/var/log/nginx/error.log \

--http-log-path=/var/log/nginx/access.log \

--with-http_gzip_static_module \

--http-client-body-temp-path=/var/temp/nginx/client \

--http-proxy-temp-path=/var/temp/nginx/proxy \

--http-fastcgi-temp-path=/var/temp/nginx/fastcgi \

--http-uwsgi-temp-path=/var/temp/nginx/uwsgi \

--http-scgi-temp-path=/var/temp/nginx/scgi \

--add-module=/soft/fastdfs-nginx-module/src

编译并安装

# make 

# make install

8 配置 Nginx

配置 fastdfs-nginx-module

# cd /soft/fastdfs-nginx-module/src

# vi mod_fastdfs.conf

store_path0=/data/fastdfs/storage

url_have_group_name = true

tracker_server=IP:22122

配置好之后将配置文件copy到/etc/fdfs下

cp mod_fastdfs.conf /etc/fdfs

接着我们需要把fastdfs-5.05下面的配置中还没有存在/etc/fdfs中的拷贝进去

# cd /soft/fastdfs-5.05/conf

# cp anti-steal.jpg http.conf mime.types /etc/fdfs/

最后修改Nginx配置文件

# cd /usr/local/nginx/conf

# vi nginx.conf

用下面的部分 替换原有的location部分

location /group1/M00 {

           root /data/fastdfs/storage/;

           ngx_fastdfs_module;

        }

创建目录和软连接

#mkdir /data/fastdfs/storage/data/group1

# ln -s /data/fastdfs/storage/data /data/fastdfs/storage/data/group1/M00


将刚配置的docker 打包成镜像

docker commit 324ceb50e598 fastdfs:v1

停止 docker

docker stop 324ceb50e598

删除 docker 

docker rm 324ceb50e598

运行容器创建docker 

docker run --privileged=true -itd -v /soft/:/back -p 192.168.46.229:80:80 -p 192.168.46.229:22122:22122 -p 192.168.46.229:23000:23000 -p 192.168.46.229:8080:8080 镜像id /bin/bash

docker exec -it docker的id /bin/bash

查看监听 --能看见两个端口号就对了，22122是tracker的 ，23000是storage的

# netstat -unltp|grep fdfs

![](https://img-blog.csdn.net/20180706225012978?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#cd /etc/fdfs

#vi client.conf

base_path=/data/fdfs

tracker_server=docker的ip:22122

启动nginx

# cd /usr/local/nginx/sbin

# ./nginx 


测试

传个文件，注意我用的是Xshell5，无法连接到docker，我们需要往linux下，挂载到docker的文件夹中传图片，这样就会传到docker的指定文件夹中。

然后 执行 

/usr/bin/fdfs_test /etc/fdfs/client.conf upload 文件物理路径

        如果成功以后会显示如下结果

![](https://img-blog.csdn.net/20180706225152840?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180706225322840?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果无法通过linux的ip访问到docker容器 做以下步骤进行尝试
      ------------------------------------------------------------------------------------------------------------

       进入   window下的  命令提示符（管理员）

![](https://img-blog.csdn.net/20180707144813151?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

      ------------------------------------------------------------------------------------------------------------


如果只显示一个端口号，23000没有出来 做以下步骤进行尝试

------------------------------------------------------------------------------------------------------------

关闭防火墙

service iptables stop

重启 tracker 和 storage 

service fdfs_trackerd stop

service fdfs_trackerd start

service fdfs_storaged stop

service fdfs_storaged start

如果还不行

删除 data 文件夹

# rm -rf data/

重新创建 目录

# mkdir -p /data/fdfs

# mkdir -p /data/fastdfs/storage

#mkdir /data/fastdfs/storage/data/group1

# ln -s /data/fastdfs/storage/data /data/fastdfs/storage/data/group1/M00

再重启 两个服务

------------------------------------------------------------------------------------------------------------





