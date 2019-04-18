# 【服务器】CentOS下Jenkins安装及配置 - weixin_33985507的博客 - CSDN博客
2019年03月01日 15:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## 一、Jenkins安装启动
### 1、安装和配置jdk
> 
此处忽略jdk的配置
### 2、下载jenkins.jar包
- [官网下载](https://jenkins.io/)
> 
[https://jenkins.io/](https://jenkins.io/)
![1802116-b288ac6e35518b17.png](https://upload-images.jianshu.io/upload_images/1802116-b288ac6e35518b17.png)
01
### 3、启动Jenkins
> 
将下载下来的jenkins.war包解压到你所需要的位置
- 基本的启动脚本
> 
我的jenkins.war包放在路径：`/usr/local/jenkins/`下，日志放在`/usr/local/jenkins/logs`下
```
#!/bin/sh
. /etc/profile
. ~/.bash_profile
JENKINS=`ps -ef|grep jenkins.war|grep -v grep|wc -l`
LOG=/usr/local/jenkins/logs
echo "start jenkins...."
if [ $JENKINS -ne 1 ];then
  cd /usr/local/jenkins/
  /usr/bin/nohup java -Dhudson.util.ProcessTree.disable=true -jar jenkins.war >/dev/null 2>&1 &
  echo "Jenkins is starting  --date:`date "+%Y%m%d_%H:%M:%S"` " >>${LOG}/jenkins_start_counter.log
fi
```
### 4、访问jenkins
> 
jenkins的默认端口为8080，确保服务器开放了该端口，并且没有被占用
- 服务器验证jenkins是否启动
```
ps -ef | grep jenkins
```
或者: 因为jenkins是一个java项目
```
ps -ef | grep java
```
> 
![1802116-d0180af32b5554c4.png](https://upload-images.jianshu.io/upload_images/1802116-d0180af32b5554c4.png)
002
- 浏览器访问
> 
ip:8080 例如：127.0.0.1:8080
## 二、Jenkins的基本配置界面
### 1、系统设置
> 
![1802116-1ee73ac7648ef757.png](https://upload-images.jianshu.io/upload_images/1802116-1ee73ac7648ef757.png)
1
### 2、管理用户
> 
![1802116-d73c90c15760b020.png](https://upload-images.jianshu.io/upload_images/1802116-d73c90c15760b020.png)
2
### 3、全局安全配置
> 
1、此处非常重要，如果不进行配置，默认是任何人（包括未登录用户）可以做任何事，会让Jenkins处于非安全状态，在用git自动获取代码等操作时，受到限制，导致报错，无法进行。
> 
2、可以管理匿名用户，当处于非登录状态时，直接跳转到登录界面
> 
![1802116-3fd3b1f765cc655e.png](https://upload-images.jianshu.io/upload_images/1802116-3fd3b1f765cc655e.png)
3
![1802116-511e6bf49cdcd08b.png](https://upload-images.jianshu.io/upload_images/1802116-511e6bf49cdcd08b.png)
4
## 三、git、maven等配置
> 
全局工具配置：主要来配置git、maven、jdk以及执行脚本等信息
### 1、配置git
> 
首先自行在Linux服务器中安装git
![1802116-f46f8019a061141a.png](https://upload-images.jianshu.io/upload_images/1802116-f46f8019a061141a.png)
5
### 2、配置maven
> 
首先在服务器中安装maven
- 配置maven路径
> 
![1802116-5740c9a953f924d3.png](https://upload-images.jianshu.io/upload_images/1802116-5740c9a953f924d3.png)
6
- 配置maven的setttings.xml文件位置
> 
![1802116-05538a596dba5343.png](https://upload-images.jianshu.io/upload_images/1802116-05538a596dba5343.png)
7
### 3、配置jdk
> 
当本地已经配置过jdk后可以选择不配置
![1802116-5d889f4f5c9e89c3.png](https://upload-images.jianshu.io/upload_images/1802116-5d889f4f5c9e89c3.png)
8
## 四、特定任务(jobs)中的配置
> 
主要配置一下两个，其余的都可以不用配置
> 
![1802116-a5cfcb346d7a89db.png](https://upload-images.jianshu.io/upload_images/1802116-a5cfcb346d7a89db.png)
10
> 
![1802116-be5a9d770334bed8.png](https://upload-images.jianshu.io/upload_images/1802116-be5a9d770334bed8.png)
9
### 1、构建git的分支管理
- 安装插件Git Parameter
> 
![1802116-d8f7dbe48d4e6d9f.png](https://upload-images.jianshu.io/upload_images/1802116-d8f7dbe48d4e6d9f.png)
11
![1802116-85e3862a80ed9eaa.png](https://upload-images.jianshu.io/upload_images/1802116-85e3862a80ed9eaa.png)
12
![1802116-66a18aafc6cc27ab.png](https://upload-images.jianshu.io/upload_images/1802116-66a18aafc6cc27ab.png)
13
- 分支管理配置
> 
![1802116-a339c87b994b13c8.png](https://upload-images.jianshu.io/upload_images/1802116-a339c87b994b13c8.png)
14
### 2、需要执行的脚本配置
> 
![1802116-bc960df1a6ff41f6.png](https://upload-images.jianshu.io/upload_images/1802116-bc960df1a6ff41f6.png)
15
