# 开发者实验室体验之Linux基础入门 - 刘炫320的博客 - CSDN博客
2017年08月02日 09:13:31[刘炫320](https://me.csdn.net/qq_35082030)阅读数：259标签：[linux																[创建文件																[移动文件																[删除文件																[常用命令](https://so.csdn.net/so/search/s.do?q=常用命令&t=blog)](https://so.csdn.net/so/search/s.do?q=删除文件&t=blog)](https://so.csdn.net/so/search/s.do?q=移动文件&t=blog)](https://so.csdn.net/so/search/s.do?q=创建文件&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[腾讯开发者实验室](https://blog.csdn.net/qq_35082030/article/category/7070255)
# 1. 前言
从这篇开始，我们将会对[腾讯云——开发者实验室](https://www.qcloud.com/developer/labs/list)中比较有兴趣的部分进行相关实验。本文主要是其笔记，目前执行计划的项目有Linux基础入门、基于Ubuntu搭建Discuz论坛、基于Ubuntu搭建WordPress个人博客、搭建Nginx静态网站、TensorFlow-学习体验相关API、TensorFlow-线性回归、TensorFlow-基于CNN数字识别、搭建Git服务器、搭建Docker环境、搭建JAVA Web开发环境、体验万象优图 by Python SDK、体验自然语言处理 by Python SDK等。本文主要介绍的是Linux基础入门。
# 2. 正文笔记
## 2.1. 目录操作
### 2.1.1创建目录
创建文件夹命令：
`mkdir $HOME/testFolder`
### 2.1.2切换目录
切换目录：
```bash
cd $HOME/testFolder
```
返回上一级目录
```bash
cd ../
```
### 2.1.3移动目录
使用 mv 命令移动目录
```php
mv $HOME/testFolder /var/tmp
```
### 2.1.4删除目录
使用 rm -rf 命令删除目录
`rm -rf /var/tmp/testFolder`
### 2.1.5查看目录下的文件
使用 ls 命令查看 /etc目录下所有文件和文件夹
`ls /etc`
## 2.2. 文件操作
### 2.2.1创建文件
使用 touch 命令创建文件
`touch ~/testFile`
执行 ls 命令, 可以看到刚才新建的 testFile 文件
`ls ~`
### 2.2.2复制文件
使用 cp 命令复制文件
`cp ~/testFile ~/testNewFile`
### 2.2.3删除文件
使用 rm 命令删除文件, 输入 y 后回车确认删除
`rm ~/testFile`
### 2.2.4查看文件内容
使用 cat 命令查看 .bash_history 文件内容
`cat ~/.bash_history`
## 2.3. 过滤、管道与重定向
### 2.3.1过滤
过滤出 /etc/passwd 文件中包含 root 的记录
`grep 'root' /etc/passwd`
递归地过滤出 /var/log/ 目录中包含 linux 的记录
`grep -r 'linux' /var/log/`
### 2.3.2管道
简单来说, Linux 中管道的作用是将上一个命令的输出作为下一个命令的输入, 像 pipe 一样将各个命令串联起来执行, 管道的操作符是 | 
比如, 我们可以将 cat 和 grep 两个命令用管道组合在一起
`cat /etc/passwd | grep 'root'`
过滤出 /etc 目录中名字包含 ssh 的目录(不包括子目录)
`ls /etc | grep 'ssh'`
### 2.3.3重定向
可以使用 > 或 < 将命令的输出重定向到一个文件中
```bash
echo 'Hello World' > ~/test.txt
```
## 2.4. 运维常用命令
### 2.4.1ping 命令
对 cloud.tencent.com 发送 4 个 ping 包, 检查与其是否联通
`ping -c 4 cloud.tencent.com`
### 2.4.2netstat 命令
netstat 命令用于显示各种网络相关信息，如网络连接, 路由表, 接口状态等等 
列出所有处于监听状态的tcp端口
```bash
netstat -lt
```
查看所有的端口信息, 包括 PID 和进程名称
`netstat -tulpn`
### 2.4.3ps 命令
过滤得到当前系统中的 ssh 进程信息
`ps -aux | grep 'ssh'`
