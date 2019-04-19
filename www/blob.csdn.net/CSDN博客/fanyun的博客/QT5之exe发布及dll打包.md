# QT5之exe发布及dll打包 - fanyun的博客 - CSDN博客
2019年02月28日 23:01:03[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：44
1.新建一个目录例如comSoft.
2.把要打包的可执行文件拷贝到comSoft文件下
3.使用cmd命令，进入到目标文件夹
![](https://img-blog.csdn.net/20180530160629218)
执行打包命令：windeployqt  ComDataSend.exe
![](https://img-blog.csdn.net/20180530160929949)
执行完，查看打包目录
![](https://img-blog.csdn.net/20180530161202796?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
ok，搞定！
