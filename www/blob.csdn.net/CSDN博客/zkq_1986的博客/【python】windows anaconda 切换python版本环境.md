# 【python】windows anaconda 切换python版本环境 - zkq_1986的博客 - CSDN博客





2019年03月30日 19:24:08[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：24
个人分类：[程序设计语言](https://blog.csdn.net/zkq_1986/article/category/6534515)









python中双路径的切换：

先安装某一个集成环境,例如anaconda3.5

它自带的是python3.6环境,如果你需要再安装一个python2.7,那么在终端执行下列命令

conda create -n 环境的别名 python=版本号

例如  conda create -n py27 python=2.7.12

终端命令执行完成之后输入activate激活，输入python便可出现python2的环境，此时若想在pycharm中切换到Python2环境，点击file-->settings.按照普通的方法切换即可。注：python.exe文件在Anaconda3文件夹下找到envs文件夹下的py27文件中。

如若不想用Python2的环境，可在运行框中输入exit（）回车之后再输入deavtivate退出python2环境


转载自：[https://blog.csdn.net/dayun555/article/details/79363584](https://blog.csdn.net/dayun555/article/details/79363584)



