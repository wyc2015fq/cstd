
# 使用python3连接hiveserver2的方法 - Magician的博客 - CSDN博客


2018年08月30日 10:34:55[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：715


[前言：](#前言)
[1、启动HiveServer2](#1启动hiveserver2)
[2、在Linux中安装impyla（前提是安装Python相关的环境、虚拟环境（可选））](#2在linux中安装impyla前提是安装python相关的环境虚拟环境可选)


## 前言：
需求：需要通过windows端的pycharm来操作hive。
于是就搜集资料寻找解决方案。
大概有三种方式：
pyhs2 连接 hive，但是这个项目已经没有人维护了，弃用。
Pyhive连接hive。
imply连接hive。
协商后决定还是用imply。
以下是整个安装以及验证的过程：
## 1、启动HiveServer2
```python
shell > cd /usr/local/apache-hive-
```
```python
2.3
```
```python
.
```
```python
1
```
```python
-bin
shell > nohup hiveserver2
```
```python
1
```
```python
>
```
```python
/root/apps
```
```python
/hive-2.1.1/logs
```
```python
/hiveserver.log 2>/root
```
```python
/apps/hive
```
```python
-
```
```python
2.1
```
```python
.
```
```python
1
```
```python
/logs/hiveserver.err &（后台启动并将hivesever的日志导入对应的目录）
```
## 2、在Linux中安装impyla（前提是安装Python相关的环境、虚拟环境（可选））
```python
# 安装依赖
```
```python
shell > yum -
```
```python
y
```
```python
install gcc gcc-c++ cyrus-sasl-devel cyrus-sasl-plain
```
```python
# 创建虚拟环境（linux下安装虚拟环境参考：）
```
```python
shell > virtualenv --no-site-packages -p python3 venv
```
```python
# 启用虚拟环境
```
```python
shelll > source venv/bin/activate
```
```python
#linux下python的安装教程见：
```
```python
(venv) shell > python -V
Python
```
```python
3.6
```
```python
.3
```
```python
# 安装 impyla 及所需依赖包
```
```python
(venv) shell > pip install ipython six bit_array thriftpy thrift_sasl==
```
```python
0.2
```
```python
.1
```
```python
sasl impyla
(venv) shell > ipython
```
```python
In
```
```python
[
```
```python
1
```
```python
]: from impala
```
```python
.dbapi
```
```python
import connect
```
```python
In
```
```python
[
```
```python
2
```
```python
]: conn = connect(host=
```
```python
"192.168.10.45"
```
```python
, port=
```
```python
10000
```
```python
, database=
```
```python
"logsdb"
```
```python
, auth_mechanism=
```
```python
"PLAIN"
```
```python
)
```
```python
In
```
```python
[
```
```python
3
```
```python
]: cur = conn
```
```python
.cursor
```
```python
()
```
```python
In
```
```python
[
```
```python
4
```
```python
]: cur
```
```python
.execute
```
```python
(
```
```python
"select count(*) from log_bftv_api"
```
```python
)
```
```python
In
```
```python
[
```
```python
5
```
```python
]: cur
```
```python
.fetchone
```
```python
()
```
```python
Out
```
```python
[
```
```python
5
```
```python
]: (
```
```python
1379094425
```
```python
,)
```
```python
In
```
```python
[
```
```python
6
```
```python
]: conn
```
```python
.close
```
```python
()
```
```python
# 程序查出了 hive table log_bftv_api 中总共有 1379094425 条数据。你也可以到对应yarn的web界面去查看对应执行进度，或者在1中配置的日志文件中查看执行的过程。
```
```python
# 其中，连接配置中 auth_mechanism 的值由 hive-site.xml 配置文件中 hive.server2.authentication 配置项指定，你需要到对应的hiveserver2节点的hive-site中添加如下配置:
```
```python
<property>
  <name>hive
```
```python
.server
```
```python
2
```
```python
.authentication
```
```python
</name>
  <value>PLAIN</value>
</property>。
```
```python
# PLAIN 代表不启用认证，也就是 hive.server2.authentication 的默认值：NONE。
```
转自：[https://www.cnblogs.com/wangxiaoqiangs/p/7850953.html](https://www.cnblogs.com/wangxiaoqiangs/p/7850953.html)

