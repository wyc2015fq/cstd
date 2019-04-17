# Linux Ubuntu15.04 rz快速上传文件命令 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月17日 15:25:51[boonya](https://me.csdn.net/boonya)阅读数：1750








使用rz命令可以快速上传本地文件到Linux系统目录。

## 正常传输

cd进入到需要上传文件的路径：


`$rz`


![](https://img-blog.csdn.net/20170317152359135)

![](https://img-blog.csdn.net/20170317152421708)

查看上传情况：

![](https://img-blog.csdn.net/20170317152541120)


## 传输异常处理

如果提示传输失败需要使用以下命令：


`$rz -ary --o-sync`
注：centos可以通过yum install lrzsz 镜像命令安装。








