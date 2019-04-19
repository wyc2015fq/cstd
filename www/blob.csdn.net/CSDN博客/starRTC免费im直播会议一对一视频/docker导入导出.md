# docker导入导出 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年10月10日 14:42:18[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：239
所属专栏：[后台架构](https://blog.csdn.net/column/details/31054.html)

导出镜像
docker save -o centos7.tar centos
#
导入本地镜像
docker load --input centos7.tar
docker ps -a
docker export 7691a814370e > ubuntu.tar
cat ubuntu.tar | docker import -
 test/ubuntu:v1.0
docker imagesREPOSITORY TAG IMAGE ID CREATED VIRTUAL SIZE**test/ubuntu v1.0 **9d37a6082e97 About a minute ago 171.3 MB
*注：用户既可以使用 docker
 load 来导入镜像存储文件到本地镜像库，也可以使用 docker import 来导入一个容器快照到本地镜像库。
这两者的区别在于容器快照文件将丢弃所有的历史记录和元数据信息（即仅保存容器当时的快照状态），
而镜像存储文件将保存完整记录，体积也要大。此外，从容器快照文件导入时可以重新指定标签等元数据信息。
