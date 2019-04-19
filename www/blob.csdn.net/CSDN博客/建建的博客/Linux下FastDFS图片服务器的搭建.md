# Linux下FastDFS图片服务器的搭建 - 建建的博客 - CSDN博客
2018年01月17日 17:50:07[纪建](https://me.csdn.net/u013898698)阅读数：86
个人分类：[Linux](https://blog.csdn.net/u013898698/article/category/6754383)
> 
在真正的大型Web后台系统中，上传图片的操作可以算是耗时操作，而且其中最大的问题就是，在一个分布式系统中，图片上传不可能上传到当前服务器，这样如果下次访问的是其他服务器，在其他服务器上是没有上传的图片的。解决这些问题，可以采用分布式文件系统FastDFS来搭建Web后端的图片服务器。
## 一、FastDFS简介
### 1.1 什么是FastDFS
> 
FastDFS是用c语言编写的一款开源的分布式文件系统。FastDFS为互联网量身定制，充分考虑了冗余备份、负载均衡、线性扩容等机制，并注重高可用、高性能等指标，使用FastDFS很容易搭建一套高性能的文件服务器集群提供文件上传、下载等服务。
![这里写图片描述](https://img-blog.csdn.net/20160523133115133)
### 1.2 文件上传的流程
![这里写图片描述](https://img-blog.csdn.net/20160523133145337)
### 1.3 文件下载的流程
![这里写图片描述](https://img-blog.csdn.net/20160523133203712)
### 1.4 上传文件的文件名
> 
客户端上传文件后存储服务器将文件ID返回给客户端，此文件ID用于以后访问该文件的索引信息。文件索引信息包括：组名，虚拟磁盘路径，数据两级目录，文件名。 
![这里写图片描述](https://img-blog.csdn.net/20160523133258885)
- 组名：文件上传后所在的storage组名称，在文件上传成功后有storage服务器返回，需要客户端自行保存。 
- 虚拟磁盘路径：storage配置的虚拟路径，与磁盘选项store_path*对应。如果配置了store_path0则是M00，如果配置了store_path1则是M01，以此类推。
- 数据两级目录：storage服务器在每个虚拟磁盘路径下创建的两级目录，用于存储数据文件。
- 文件名：与文件上传时不同。是由存储服务器根据特定信息生成，文件名包含：源存储服务器IP地址、文件创建时间戳、文件大小、随机数和文件拓展名等信息。
## 二、 FastDFS搭建
> 
这里我们搭建一个Tracker和一个Storage，实现图片的上传。之后结合Nginx提供虚拟路径实现图片的访问。 
![这里写图片描述](https://img-blog.csdn.net/20160523220020011)
**准备工作**([所需工具下载地址](http://download.csdn.net/download/qq_25689397/9528976))：
![这里写图片描述](https://img-blog.csdn.net/20160523222244166)
**具体步骤如下：**
### 2.1 把FastDFS所需要的都上传到linux系统。
> 
可以使用[FileZillaFTP上传工具](http://download.csdn.net/download/qq_25689397/9527738)或者[SecureCRT](http://download.csdn.net/download/qq_25689397/9527737)。
### 2.2 安装FastDFS之前，先安装libevent工具包。
> 
`sudo apt-get install libevent-dev`
### 2.3 安装libfastcommon工具包
> - 解压缩
- `./make.sh`
- `./make,sh install`
- 如果lib64文件下存在有`libfastcommon.so`则把`/usr/lib64/libfastcommon.so`文件向`/usr/lib`下复制一份。
### 2.4 安装Tracker服务
> - 解压缩FastDFS源码压缩包。
- `./make.sh`
- `./make.sh install`
安装后在/usr/local/bin/目录下有以fdfs开头的文件都是编译出来的。 
配置文件都放到/etc/fdfs文件夹
- 把FastDFS根目录下的`/conf`目录下的所有的配置文件都复制到`/etc/fdfs`
- 配置tracker服务。修改`/etc/fdfs/tracker.conf`文件。 
![这里写图片描述](https://img-blog.csdn.net/20160523224239675)
- 启动Tracker。 
启动命令：`/usr/local/bin/fdfs_trackerd /etc/fdfs/tracker.conf`
重启命令：`/usr/local/bin/fdfs_trackerd /etc/fdfs/tracker.conf restart`
### 2.5 安装Storage服务
> - 如果是在不同的服务器安装，2.4的1～4需要重新执行。
- 配置Storage服务。修改`/etc/fdfs/storage.conf`文件 
![这里写图片描述](https://img-blog.csdn.net/20160523224651572)
![这里写图片描述](https://img-blog.csdn.net/20160523224702245)
![这里写图片描述](https://img-blog.csdn.net/20160523224710417)
- 启动Storage服务 
启动命令：`/usr/local/bin/fdfs_storaged /etc/fdfs/storage.conf`
重启命令：`/usr/local/bin/fdfs_storaged /etc/fdfs/storage.conf restart`
### 2.6 测试服务
> - 修改配置文件`/etc/fdfs/client.conf`
![这里写图片描述](https://img-blog.csdn.net/20160523224925060)
![这里写图片描述](https://img-blog.csdn.net/20160523224934013)
- 测试 
`/usr/local/bin/fdfs_test /etc/fdfs/client.conf upload anti-steal.jpg`
上传成功之后，你会在你配置的文件存放路径下看到上传的图片。
### 2.7 搭建Nginx提供HTTP服务
> **：安装fastdfs-nginx-module**
**      9.0  获取 fastdfs-nginx-module_v1.16.tar.gz       9.1 创建文件目录 mkdir -p /usr/local/src**
**      9.2 将包fastdfs-nginx-module_v1.16.tar.gz放到/usr/local/src**
**      9.3 解压  tar -zvxf  fastdfs-nginx-module_v1.16.tar.gz**
**    9.4 修改  fastdfs-nginx-module的配置文件      9.5  cd /usr/local/src/fastdfs-nginx-module/src      9.6  vi config **      9.7  修改 CORE_INCS="$CORE_INCS /usr/local/include/fastdfs /usr/local/include/fastcommon/"
              为 CORE_INCS="$CORE_INCS /usr/include/fastdfs /usr/include/fastcommon/"
10 ：安装nginx
       10.0 创建目录 mkdir -p  /usr/local/nginx
       10.1 将文件`nginx-1.10.0.tar.gz` 放置到  /usr/local/nginx
       10.2 解压 `tar -zxvf nginx-1.10.0.tar.gz`
       10.3 
```
cd  /usr/local/nginx/nginx-1.10.0.tar.gz
       10.4 安装nginx  `./configure --add-module=/usr/local/src/fastdfs-nginx-module/src`
```
`       10.5  `make && make install````11  : 配置  fastdfs-nginx-module 和nginx 关联信息``
user nobody;
worker_processes 1;
events {
    worker_connections 1024;
}
http {
    include mime.types;
    default_type application/octet-stream;
    sendfile on;
    keepalive_timeout 65;
    server {
        listen 8888;
        server_name localhost;
        location ~/group([0-9])/M00 {
            ngx_fastdfs_module;
        }
        error_page 500 502 503 504 /50x.html;
        location = /50x.html {
            root html;
        }
    }
}
8888 端口值是要与/etc/fdfs/storage.conf 中的 http.server_port=8888 相对应, 因为 http.server_port 默认为 8888,如果想改成 80,则要对应修改过来。``
````
**备注：成功安装之后  ：  上传的文件可以通过以下路径访问**
**http://192.168.61.129:8888/group1/M00/00/00/wKg9gVmYCT6AfE33AAAptg_20B4551.jpg**
``把`/fastdfs-nginx-module/src/mod_fastdfs.conf`文件复制到`/etc/fdfs`目录下。编辑：
![这里写图片描述](https://img-blog.csdn.net/20160523225845045)
![这里写图片描述](https://img-blog.csdn.net/20160523225854682)
![这里写图片描述](https://img-blog.csdn.net/20160523225903573)
![这里写图片描述](https://img-blog.csdn.net/20160523225911089)Nginx的配置 
在nginx的配置文件中添加一个Server： 
```
server { 
listen 80; 
server_name 192.168.101.3; 
location /group1/M00/{ 
#root /home/FastDFS/fdfs_storage/data; 
ngx_fastdfs_module; 
} 
}
```
启动nginx 
在`/usr/local/nginx/sbin/`下执行`./nginx`
## 三、 图片服务的使用
> 
官方提供一个jar包 
使用方法： 
1. 把FastDFS提供的jar包添加到工程中 
2. 初始化全局配置。加载一个配置文件。 
3. 创建一个TrackerClient对象。 
4. 创建一个TrackerServer对象。 
5. 声明一个StorageServer对象，null。 
6. 获得StorageClient对象。 
7. 直接调用StorageClient对象方法上传文件即可。
**文章中需要的所有工具下载地址：[请点击](http://download.csdn.net/download/qq_25689397/9528976)**
