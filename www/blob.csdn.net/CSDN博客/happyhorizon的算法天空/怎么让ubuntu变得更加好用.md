# 怎么让ubuntu变得更加好用 - happyhorizon的算法天空 - CSDN博客
2015年12月23日 16:11:43[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：1040
ubunut14.04LTS版本其实已经很好用了,但是也有一些小小的美中不足.以下设置是陆续收集\摸索到的可以让系统更好用的方法.
1、在终端打开已经安装的应用程序时，总是会显示一些错误信息
       在~/bin下添加x文件：cd ～/bin; vim x
       打开vim编辑器后输入以下语句：
                #!/bin/sh                                                                   
               "$@" 1>/dev/null 2>&1 &  #将这些屏幕的输出信息丢到/dev/null这个垃圾筒中          
2、ubuntu截屏工具：安装sudo apt-get install ksnapshot，运行该软件，在终端输入 x ksnapshot即可。
3、关机。在用户的.bashrc文件中添加
         vim ~/.bashrc
         打开文件添加一行：
         alias myshutdown="sync; sync; sudo shutdown -h now"
        sync是一个linux同步命令，含义为迫使缓冲块数据立即写盘并更新超级块。其中包含已修改的 i-node、已延迟的块 I/O 和读写映射文件。
       shutdown是linux的关机命令，需要root权限，因此需要在前面加上sudo语句。
4 清理无用文件
当ubuntu使用久了, 就会积累一些无用的文件,或者旧的系统更新文件, 出现根目录空间不够, 或者"/var/cache/apt/archives/ 上没有足够的可用空间, 最好用的方法是如下的自动化傻瓜命令，可以时不时用它为 Ubuntu 系统释放一些可用硬盘空间： 
```shell
sudo apt autoremove
```
其他方法可以参考: [https://jingyan.baidu.com/article/2a138328f1e8ae074a134ff8.html](https://jingyan.baidu.com/article/2a138328f1e8ae074a134ff8.html)
