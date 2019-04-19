# airflow 安装 - sxf_123456的博客 - CSDN博客
2018年03月07日 22:11:54[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：133
转载：http://blog.csdn.net/Excaliburace/article/details/53818530
# airflow最简安装方法 centos 6.5
python新司机, 所以使用anaconda安装python及其所需要的包, 不用担心缺包导致的各种各样的问题 
1. anaconda下载地址:[https://www.continuum.io/downloads](https://www.continuum.io/downloads)
linux版的直接戳[这里-linux 64位](https://repo.continuum.io/archive/Anaconda3-4.2.0-Linux-x86_64.sh) 和[这里-linux 32位](https://repo.continuum.io/archive/Anaconda3-4.2.0-Linux-x86.sh)
2. 将所下载的anaconda脚本考到linux中, 我是放在用户目录下: 
/home/airflow/tools/Anaconda3-4.2.0-Linux-x86_64.sh 
3. 执行这个脚本, 会安装一大~~~~对包, 具体 
![anaconda安装的包](https://img-blog.csdn.net/20161222185639942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhjYWxpYnVyYWNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 
注意, 在普通用户下执行完安装脚本后, 运行python会发现还是系统自带的2.6.6版本, 这时候需要进入到anaconda安装路径的bin下, 才能直接使用最新安装的python及其相关的包, 以及后续的airflow安装也都在这个路径下执行, 设置软连接可以解决此问题, 此处不介绍软连接的设置方法.
4.设置环境变量
```bash
export AIRFLOW_HOME=~/airflow
```
- 1
```
因为airflow的configuration.py中需要这个环境变量来确定生成的配置文件、日志、dags等文件会存在哪里. 指定后, 以后通过修改~/airflow/airflow.cfg文件就可以修改airflow的配置.
```
5.下载
`airflow pip install airflow`- 1
6.初始化数据库:
`airflow initdb`- 1
> 
airflow默认使用sqlite作为数据库, 直接执行数据库初始化命令后, 会在刚刚设置的环境变量路径下新建一个数据库文件airflow.db
7.运行airflow的webserver(默认端口为8080):
`airflow webserver`- 1
至此airflow安装完成. 放几个成功的截图 
![airflow webserver首页](https://img-blog.csdn.net/20161222191043367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhjYWxpYnVyYWNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![DAG example_bash_operator2222](https://img-blog.csdn.net/20161222191154540?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhjYWxpYnVyYWNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
