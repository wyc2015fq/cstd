# 在安装了python2的情况下，如何安装python3 - 数据之美的博客 - CSDN博客
2019年01月14日 09:28:17[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：61
在使用python时，大多数情况下，python2和python3时可以互相兼容的，两个版本使用哪个都是可以的，但是在少数情况下，其中一个版本的功能可能无法满足要求。
那么假设原来的默认环境是anconda2下面的python2执行环境，现在如果需要在把持python2环境的情况下，多重来一个python3与之共存，做法如下：
步骤：
- 下载 anconda3的执行安装文件，下载地址 [https://www.anaconda.com/download/](https://www.anaconda.com/download/)
- 执行安装脚本 bash Anaconda3-2018.12-MacOSX-x86_64.sh，其中，bash后面为在步骤1中下载下来的文件名字；另外，在安装Anaconda3-2018.12-MacOSX-x86_64.sh脚本的过程中，需要选择安装位置，安装位置选择之前安装anconda2下面的envs文件夹下面即可
- 步骤2结束之后，执行命令 conda create -n python3 python=3.7 anaconda，即可创建一个anconda3的环境，需要输入y/n的时候就输入y即可。
- 静静等待步骤3过程结束，执行 source activate python3 ，则可以将python环境切换到python3了；执行 source deactivate 则可以重新切换会原来的python2
