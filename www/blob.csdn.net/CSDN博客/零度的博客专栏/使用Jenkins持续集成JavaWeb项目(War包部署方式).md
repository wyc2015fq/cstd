# 使用Jenkins持续集成JavaWeb项目(War包部署方式) - 零度的博客专栏 - CSDN博客
2018年01月12日 17:51:07[零度anngle](https://me.csdn.net/zmx729618)阅读数：862
#### 安装配置
Jenkins有两种安装配置方式：
- 
1 war包部署到tomcat容器(跨平台)
- 
2 RPM包等安装（难度较大）
由于上篇博客已经介绍过RPM包安装的方式，本次只介绍war包部署的方式，另war包部署的方式较为简单易上手。
##### 开始安装
- 去Jenkins官网下载最新的[war包](http://mirrors.jenkins.io/war-stable/latest/jenkins.war)
将war包丢在tomcat容器中，该tomcat容器不允许存放其他的war包,启动tomcat会出现以下画面(红字标注部分是管理员初始密码所在位置，楼主用windows做的测试,linux路径会有所变化),如果希望配置jenkins工作空间的位置,需要配置JENKINS_HOME作为环境变量,该变量存放工作空间位置,去这个位置将密码复制出来粘到下面的文本框中
![img](http://7xwp5w.com1.z0.glb.clouddn.com/%E5%88%9D%E5%A7%8B%E5%AF%86%E7%A0%81.png)
输入完成后开始插件的安装，由于GFW的影响有可能比较慢或者有的插件会安装失败,不过不影响,后续我们可以去[Jenkins官网](https://plugins.jenkins.io/)安装我们需要的插件
![img](http://7xwp5w.com1.z0.glb.clouddn.com/%E5%AE%89%E8%A3%85%E6%8F%92%E4%BB%B6.png)
插件安装完成会出现如下的页面，需要创建一个管理员用户，不创建的话还会以刚才的admin用户处理，这里建议创建
![img](http://7xwp5w.com1.z0.glb.clouddn.com/%E5%88%9B%E5%BB%BA%E7%AE%A1%E7%90%86%E5%91%98%E8%B4%A6%E6%88%B7.png)
![插件下载](http://7xwp5w.com1.z0.glb.clouddn.com/jenkins%E6%8F%92%E4%BB%B6%E4%B8%8B%E8%BD%BD.png)
**一般来说，需要安装以下几个插件：**
- SVN 
- SSH
- Publish Over SSH
- Maven
这些插件均可以在[Jenkins官网](https://plugins.jenkins.io/)下载安装(下载后在系统系统管理、插件管理、高级页面可上传已经下载好的插件)
这些准备步骤完成后,就可以正常使用了
#### Quick Start
##### 创建一个项目
![创建一个项目](http://7xwp5w.com1.z0.glb.clouddn.com/%E5%88%9B%E5%BB%BA%E4%B8%80%E4%B8%AA%E9%A1%B9%E7%9B%AE.png)
创建项目的时候,这里对项目的要求不是特别严格，如果上一步骤没有安装maven插件，那么这一步可以选择构建一个自由风格的项目，填写项目名称即可
##### 配置源码管理
![](http://7xwp5w.com1.z0.glb.clouddn.com/%E9%85%8D%E7%BD%AE%E6%BA%90%E7%A0%81%E7%AE%A1%E7%90%86.png)
创建完项目后，需要配置代码仓库位置，一般用git或者svn，在Repository URL中填写代码仓库的地址，Credentials是你的svn/git账号的信息，可以配置SSH的，也可以配置用户名、密码的，这里图省字数，就用用户名密码的吧。
![](http://7xwp5w.com1.z0.glb.clouddn.com/%E6%B7%BB%E5%8A%A0%E4%BB%A3%E7%A0%81%E4%BB%93%E5%BA%93%E6%AD%A5%E9%AA%A4.png)
![](http://7xwp5w.com1.z0.glb.clouddn.com/%E6%B7%BB%E5%8A%A0%E4%BB%A3%E7%A0%81%E4%BB%93%E5%BA%93%E6%8E%88%E6%9D%83%E4%BF%A1%E6%81%AF.png)
##### 配置构建步骤
![](http://7xwp5w.com1.z0.glb.clouddn.com/%E9%85%8D%E7%BD%AE%E6%9E%84%E5%BB%BA%E8%84%9A%E6%9C%AC.png)
构建这里可以选择执行shell脚本,构建的话我们可以用Maven进行构建，那么就需要在安装Jenkins的这台机器上安装Maven并配置MAVEN_HOME环境变量，这样在shell脚本中就可以调用maven的打包指令了,例如 mvn clean package
##### 配置定时构建
![](http://7xwp5w.com1.z0.glb.clouddn.com/%E9%85%8D%E7%BD%AE%E5%AE%9A%E6%97%B6%E6%9E%84%E5%BB%BA.png)
这里我们可以配置POLL SCM去触发定时构建，图中的配置代表每天的9点，12点，16点会触发构建操作（每次构建会自动去源码仓库拉取最新的代码并执行构建命令）
常用POLL SCM配置
```
每15分钟构建一次：H/15 * * * *   或*/5 * * * * 
每天8点构建一次：0 8 * * *
每天8点~17点，两小时构建一次：0 8-17/2 * * *
周一到周五，8点~17点，两小时构建一次：0 8-17/2 * * 1-5
每月1号、15号各构建一次，除12月：H H 1,15 1-11 *
```
##### 配置发布步骤
![](http://7xwp5w.com1.z0.glb.clouddn.com/%E9%85%8D%E7%BD%AE%E5%8F%91%E5%B8%83%E8%84%9A%E6%9C%AC.png)
![](http://7xwp5w.com1.z0.glb.clouddn.com/%E5%8F%91%E5%B8%83%E6%AD%A5%E9%AA%A4.png)
这里有个地方需要注意下，是个大坑，这里的source files是指刚才构建出来的war包所在位置（相对位置，从配置的远程主机目录开始计算），这个位置是在需要全局配一下远程主机的目录,然后用ssh连接时默认就会从这个位置执行shell
**remote prefix**
这个前缀会将刚才的目录的路径脱掉,具体的原因也是查了好久也没有,不配置的话上面的war包上传不到远程主机
**remote directory**
这个也是从全局配置的远程主机目录开始计算相对位置，需要在远程主机的那里创建个文件夹,并在文本框中填写目录的位置
**配置到此已经可以完成拉取最新版本代码、构建、发布到远程目录了，在构建后步骤里面在加个shell脚本，就可以完成发布到容器并重启容器了，这里就不多介绍了**
