# 解决Unbantu  eclipse  菜单栏目消失的问题 - 建建的博客 - CSDN博客
2017年06月16日 16:40:23[纪建](https://me.csdn.net/u013898698)阅读数：330
1. 新建一个eclipse.sh文件，加入如下内容，下面的路径是ecipse的路径
export UBUNTU_MENUPROXY=0
/usr/bin/eclipse              //这一行是安装路径一定不能错
2. 在终端 sh ./eclipse.sh
