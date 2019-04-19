# CentOS 7 终端清空回收站命令 - sxf_123456的博客 - CSDN博客
2017年02月26日 11:28:13[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：4785
个人分类：[Centos 安装](https://blog.csdn.net/sxf_123456/article/category/6745709)
Linux 下的回收站其实是一个文件夹，该文件夹下边有两个文件夹：files和info
在终端执行rm -rf /root/.local/share/Trash/files
清空回收站中的文件
该文件夹info，存放的是删除文件的相关信息
