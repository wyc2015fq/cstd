# 使用linux提交pbs任务：运行matlab或perl程序 - qq_32515081的博客 - CSDN博客
2017年12月24日 12:21:43[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：922
连接服务器使用的软件是Xmanager Enterprise
其中，xftp能够将本地文件上传到服务器
      xshell能够使用命令行在服务器上操作
提交任务需要用PBS提交
PBS使用方式：您的代码，您的脚本
您的代码指的是可以运行的代码，用Matlab的就是.m文件，perl的就是.pl文件。
您的脚本指的是一个.pbs文件，
简而言之PBS脚本只有以下几行：
#!/bin/bash
//提交任务的名称
#PBS -N  randomNet
// 设置，选几个CPU等
#PBS -l select=1:ncpus=1 
#PBS -q x86_small 
//命令
执行代码需要的软件 代码文件(参数)
如执行.m文件：
matlab -nojvm -nodesktop -r  "test（m,n)"
执行.pl的写法自然就是
perl test.pl
注1：想要得到程序运行的时间，可以使用
date > test.time
命令行。。。
date >> test.time
注2：使用matlab调用.m文件时，往往需要将某目录添加到搜索路径，可使用addpath
matlab  -nojvm -nodesktop -r  "addpath('/path1/','/path2/');test(参数);exit; "
