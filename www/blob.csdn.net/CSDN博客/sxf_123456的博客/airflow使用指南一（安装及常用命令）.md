# airflow使用指南一（安装及常用命令） - sxf_123456的博客 - CSDN博客
2018年03月10日 09:29:18[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1446
**安装airflow**
1、将下载的Anaconda3-5.1.0-Linux-x86_64.sh添加可执行权限
chmod +x Anaconda3-5.1.0-Linux-x86_64.sh
2、安装Anaconda3
./Anaconda3-5.1.0-Linux-x86_64.sh 
然后，遇见选yes/no的选择yes
3、在/root/.bashrc文件中添加路径
export PATH=/root/anaconda3/bin:$PATH
export AIRFLOW_HOME=/home/airflow/airflow
4、使添加路径生效
source /root/.bashrc
5、创建用户和组
```
[root@airflow ~]# groupadd airflow
[root@airflow ~]# useradd airflow -g airflow
```
6、安装airflow
pip install airflow //下载了一系列依赖的python包放在了python的site-packages下
7、初始化数据库
airflow initdb //这条命令会创建airflow文件夹，默认会放在~/airflow，也可指定路径
8、启动airflow webserver服务
airflow webserver -p 8080  & //启动后台服务
9、web浏览器上输入http://0.0.0.0:8080/admin/
10、在/home/airflow/airflow/创建dags文件夹
mkdir /home/airflow/airflow/dags
将airflow程序都存放在dags目录下
11、执行dags下的所有文件
airflowscheduler
12、设置DAG在某个日期范围内执行
airflow backfill tutorial -s 2015-06-01 -e 2015-06-07
**airflow 常用命令**
airflow list_dags  //列出所有活跃的dags
airflow list_tasks test //列出dag_id 为test的所有task_id
[root@localhost dags]# airflow list_tasks test
[2018-03-08 14:52:34,339] {__init__.py:57} INFO - Using executor SequentialExecutor
[2018-03-08 14:52:34,928] {models.py:167} INFO - Filling up the DagBag from /root/airflow/dags
print_date
sleep
templated
airflow list_tasks test --tree //以结构化查看dag_id为test的所有task_id
[root@localhost dags]# airflow list_tasks test --tree
[2018-03-08 14:56:48,439] {__init__.py:57} INFO - Using executor SequentialExecutor
[2018-03-08 14:56:49,024] {models.py:167} INFO - Filling up the DagBag from /root/airflow/dags
<Task(BashOperator): sleep>
    <Task(BashOperator): print_date>
<Task(BashOperator): templated>
    <Task(BashOperator): print_date>
task_id为sleep的上游task_id是print_date
task_id为templated的上游task_id是print_date
测试单个实例（第二个test是dag_id）
[root@localhost dags]# airflow  test test print_date 2015-06-01
[2018-03-08 14:59:09,691] {models.py:1342} INFO - Executing <Task(BashOperator): print_date> on 2015-06-01 00:00:00
[2018-03-08 14:59:09,706] {bash_operator.py:71} INFO - tmp dir root location: 
/tmp
[2018-03-08 14:59:09,708] {bash_operator.py:80} INFO - Temporary script location :/tmp/airflowtmpzj4pl7vn//tmp/airflowtmpzj4pl7vn/print_datesusgtpk5
[2018-03-08 14:59:09,708] {bash_operator.py:81} INFO - Running command: date
[2018-03-08 14:59:09,713] {bash_operator.py:90} INFO - Output:
[2018-03-08 14:59:09,716] {bash_operator.py:94} INFO - 2018年 03月 08日 星期四 14:59:09 CST
[2018-03-08 14:59:09,716] {bash_operator.py:97} INFO - Command exited with return code 0
[root@localhost dags]# airflow  test test sleep 2015-06-01
[2018-03-08 15:08:25,289] {models.py:1342} INFO - Executing <Task(BashOperator): sleep> on 2015-06-01 00:00:00
[2018-03-08 15:08:25,304] {bash_operator.py:71} INFO - tmp dir root location: 
/tmp
[2018-03-08 15:08:25,305] {bash_operator.py:80} INFO - Temporary script location :/tmp/airflowtmpkasawv8p//tmp/airflowtmpkasawv8p/sleepiii7f_aq
[2018-03-08 15:08:25,305] {bash_operator.py:81} INFO - Running command: sleep 5
[2018-03-08 15:08:25,309] {bash_operator.py:90} INFO - Output:
[2018-03-08 15:08:30,341] {bash_operator.py:97} INFO - Command exited with return code 0
[root@localhost dags]# airflow test test templated 2015-06-01
--------------------------------------------------------------------------------
Starting attempt 1 of 2
--------------------------------------------------------------------------------
[2018-03-08 15:12:17,822] {models.py:1342} INFO - Executing <Task(BashOperator): templated> on 2015-06-01 00:00:00
[2018-03-08 15:12:17,872] {bash_operator.py:71} INFO - tmp dir root location: 
/tmp
[2018-03-08 15:12:17,873] {bash_operator.py:80} INFO - Temporary script location :/tmp/airflowtmpneff9ruo//tmp/airflowtmpneff9ruo/templatedhpcl688i
[2018-03-08 15:12:17,873] {bash_operator.py:81} INFO - Running command: 
        echo "2015-06-01"
        echo "2015-06-08"
        echo "Parameter I passed in"
        echo "2015-06-01"
        echo "2015-06-08"
        echo "Parameter I passed in"
        echo "2015-06-01"
        echo "2015-06-08"
        echo "Parameter I passed in"
        echo "2015-06-01"
        echo "2015-06-08"
        echo "Parameter I passed in"
        echo "2015-06-01"
        echo "2015-06-08"
        echo "Parameter I passed in"
[2018-03-08 15:12:17,878] {bash_operator.py:90} INFO - Output:
[2018-03-08 15:12:17,879] {bash_operator.py:94} INFO - 2015-06-01
[2018-03-08 15:12:17,879] {bash_operator.py:94} INFO - 2015-06-08
[2018-03-08 15:12:17,879] {bash_operator.py:94} INFO - Parameter I passed in
[2018-03-08 15:12:17,879] {bash_operator.py:94} INFO - 2015-06-01
[2018-03-08 15:12:17,879] {bash_operator.py:94} INFO - 2015-06-08
[2018-03-08 15:12:17,879] {bash_operator.py:94} INFO - Parameter I passed in
[2018-03-08 15:12:17,879] {bash_operator.py:94} INFO - 2015-06-01
[2018-03-08 15:12:17,879] {bash_operator.py:94} INFO - 2015-06-08
[2018-03-08 15:12:17,879] {bash_operator.py:94} INFO - Parameter I passed in
[2018-03-08 15:12:17,879] {bash_operator.py:94} INFO - 2015-06-01
[2018-03-08 15:12:17,880] {bash_operator.py:94} INFO - 2015-06-08
[2018-03-08 15:12:17,880] {bash_operator.py:94} INFO - Parameter I passed in
[2018-03-08 15:12:17,880] {bash_operator.py:94} INFO - 2015-06-01
[2018-03-08 15:12:17,880] {bash_operator.py:94} INFO - 2015-06-08
[2018-03-08 15:12:17,880] {bash_operator.py:94} INFO - Parameter I passed in
[2018-03-08 15:12:17,880] {bash_operator.py:97} INFO - Command exited with return code 0
