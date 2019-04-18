# Setting up Jupyter with Python 3 on Ubuntu - 专注于数据挖掘算法研究和应用 - CSDN博客
2018年12月12日 11:09:01[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：100
Ubuntu16.04操作系统，获取有权限的用户。
1、更新包库：apt-get update
2、默认安装python2.7，这里安装python3.5：apt-get install python3
3、安装pip3：apt-get install python3-pip
4、安装IPython Shell：apt-get install ipython3
5、安装Jupyter：pip3 install jupyter
6、加入到命令：export PATH=$PATH:~/.local/bin/
7、打开jupyter服务：jupyter notebook
参考：
[https://datawookie.netlify.com/blog/2017/06/setting-up-jupyter-with-python-3-on-ubuntu/](https://datawookie.netlify.com/blog/2017/06/setting-up-jupyter-with-python-3-on-ubuntu/)
