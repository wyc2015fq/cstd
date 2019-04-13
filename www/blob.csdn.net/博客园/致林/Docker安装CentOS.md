
# Docker安装CentOS - 致林 - 博客园






# [Docker安装CentOS](https://www.cnblogs.com/bincoding/p/6171266.html)
系统环境： 腾讯云公共镜像 CoreOS 7.1 X64
\#docker 下载centos镜像
docker  pull   centos   \#下载centos所有的镜像
docker  pull   centos:centos6    下载centos6镜像
\#docker 下ubutu镜像包：
docker  pull   ubuntu   \#下载ubuntu所有的镜像
docker  pull   ubuntu:14.10    下载ubuntu14.10镜像
\#查看本机所有的镜像包：
docker  images
\#docker运行镜像：
docker run  -it   centos  /bin/bash
docker run  -it   centos:centos6  /bin/bash   echo 'hello  jk409!'
在镜像里输入exit，就退出并停止运行
\#docker构建新的镜像：
docker build  -t  centos6:memcached    ./memcached              \#./memcached目录下有Dockerfile文件
\#docker运行新的镜像：
docker run  -d -p 11211:11211  centos6:memcached
\#docker停止后台运行的镜像
docker stop/kill    (container_id)
\#docker  stop  `docker  ps|grep  memcached |awk  { 'print  $1'}`





