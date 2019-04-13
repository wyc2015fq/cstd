
# Linux下快速安装MySQL教程 - Magician的博客 - CSDN博客


2018年08月20日 15:48:03[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：104所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)


转自：[https://blog.csdn.net/sl1992/article/details/53634674](https://blog.csdn.net/sl1992/article/details/53634674)
[目录：](#目录)[前言：](#前言)
[1.执行yum install mysql-server进行安装](#1执行yum-install-mysql-server进行安装)
[2.输入y进行确认](#2输入y进行确认)
[3.安装成功](#3安装成功)
[4.查看MySQL是否启动](#4查看mysql是否启动)
[5.启动MySQL](#5启动mysql)
[6.查看是否运行](#6查看是否运行)
[7.设置开机启动MySQL](#7设置开机启动mysql)
[8.创建MySQL管理员root](#8创建mysql管理员root)
[9.登录MySQL](#9登录mysql)
[10.查看数据库](#10查看数据库)


# 目录：
## 前言：
亲测整个过程非常快捷，基本1分钟就能搞定。
Linux下安装MySQL
虚拟机使用的是CentOS6.7环境
## 1.执行yum install mysql-server进行安装
Linux系统需要能进行网络连接
![这里写图片描述](https://img-blog.csdn.net/20180820154119615?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2.输入y进行确认
![这里写图片描述](https://img-blog.csdn.net/20180820154139175?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180820154139175?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
再次确认
输入y进行再次确认
![这里写图片描述](https://img-blog.csdn.net/20180820154326958?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 3.安装成功
出现下述信息表明MySQL安装成功
![这里写图片描述](https://img-blog.csdn.net/20180820154401236?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 4.查看MySQL是否启动
```python
chkconfig
```
```python
-
```
```python
-
```
```python
list|grep
```
```python
mysql
```
查看MySQL是否启动
都是off说明MySQL没有启动
![这里写图片描述](https://img-blog.csdn.net/20180820154509833?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 5.启动MySQL
```python
service mysqld
```
```python
start
```
启动MySQL
![这里写图片描述](https://img-blog.csdn.net/20180820154539927?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 6.查看是否运行
```python
/etc/init
```
```python
.d
```
```python
/mysqld status
```
查看MySQL是否允许
![这里写图片描述](https://img-blog.csdn.net/20180820154605384?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 7.设置开机启动MySQL
```python
chkconfig mysqld
```
```python
on
```
查看状态`chkconfig --list|grep mysql`2,3,4,5是on，说明MySQL开机启动
设置开机启动MySQL
![这里写图片描述](https://img-blog.csdn.net/20180820154624178?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 8.创建MySQL管理员root
MySQL安装成功需要使用用户名密码操作MySQL，设置MySQL的root管理员，密码是root
```python
mysqladmin
```
```python
-uroot
```
```python
password root
```
创建MySQL管理员用户root
![这里写图片描述](https://img-blog.csdn.net/20180820154642941?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 9.登录MySQL
```python
mysql
```
```python
-uroot
```
```python
-proot
```
登录到MySQL
![这里写图片描述](https://img-blog.csdn.net/20180820154700940?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 10.查看数据库
```python
show
```
```python
databases;
```
安装后默认的数据库
显示所有数据库
![这里写图片描述](https://img-blog.csdn.net/20180820154719767?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180820154719767?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
此时MySQL安装成功。
[            ](https://img-blog.csdn.net/20180820154719767?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

