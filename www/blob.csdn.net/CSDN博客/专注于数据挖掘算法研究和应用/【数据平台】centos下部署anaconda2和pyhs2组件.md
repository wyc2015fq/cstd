# 【数据平台】centos下部署anaconda2和pyhs2组件 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年11月13日 19:34:22[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：498
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









1、下载：官网https://www.anaconda.com/download/#linux

#wget https://repo.continuum.io/archive/Anaconda2-5.0.1-Linux-x86_64.sh




2、安装： 


#bash Anaconda2-5.0.1-Linux-x86_64.sh

参考：https://docs.anaconda.com/anaconda/install/linux




3、配置环境变量：

#vi /home/user/.bashrc

加入：export PATH=/home/user/anaconda2/bin:$PATH

这里安装路径是：/home/user/anaconda2




4、部署pyhs2组件，python访问hive

#sudo yum install gcc-c++ cyrus-sasl-devel 

#pip install pyhs2







