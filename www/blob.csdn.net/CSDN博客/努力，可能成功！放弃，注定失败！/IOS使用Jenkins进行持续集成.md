# IOS使用Jenkins进行持续集成 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月08日 18:03:03[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3332
概述：为了方便测试人员测试，利用jenkins进行自动化打包并部署，减少开发人员手动打包。解除开发与测试之间的耦合，提高开发效率。
## 1.软件下载
需要的软件包下载地址：[软件包](http://pan.baidu.com/s/1dE4D7qT)
#### 1.1 Tomcat安装
（1） 到[apache官方主页](http://tomcat.apache.org/)下载 Mac 版本的完整 tar.gz文件包。解压拷贝到 /Library目录下,命名 为Tomcat，其他目录也可。
（2） 修改目录权限
到终端输入
> 
sudo chmod 755 /Library/Tomcat/bin/*.sh
（3）启动Tomcat
按回车键之后会提示输入密码，请输入管理员密码。之后输入并回车：
> 
sudo sh startup.sh
若出现如下提示则表示安装并运行成功：
Using CATALINA_BASE: /Library/Tomcat
Using CATALINA_HOME: /Library/Tomcat
Using CATALINA_TMPDIR: /Library/Tomcat/temp
Using JRE_HOME: /System/Library/Frameworks/JavaVM.framework/Versions/CurrentJDK/Home
4、 打开浏览器，输入 http://localhost:8080/
回车之后如果看到Apache Tomcat，表示已经成功运行Tomcat
> 
推荐使用brew安装tomcat   [brew 使用方法](http://blog.sina.com.cn/s/blog_76550fd70101fn14.html)
### 1.2 Jenkins安装
（1）打开[Jenkins 官网](http://jenkins-ci.org/)，官网右侧可以下载最新版本的[jenkins.war](http://mirrors.jenkins-ci.org/war/latest/jenkins.war)。
（2）把jenkins.war放到tomcat的webapp目录下面就行
![](http://upload-images.jianshu.io/upload_images/1654251-60d3c6804e2adcbc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
（3）打开浏览器，输入http://localhost:8080/jenkins/   
回车之后看到jenkins启动界面，表示已经运行成功
![](http://upload-images.jianshu.io/upload_images/1654251-1a24d2296e95443f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### 2.证书配置；
到苹果开发者网站配置开发证书和生产证书 [参考](http://dev.umeng.com/push/ios/license-configuration-guide)
企业证书和公司证书的区别：企业证书无需发布到AppStore就能安装到苹果设备上面，而公司证书必须通过苹果官方审核才能显示到AppStore上供用户使用。
开发证书和生产证书的区别：无论是企业证书还是公司证书，配置的开发证书，只有导入了uuid的设备才能安装，否则会提示 “无法下载应用程序，此时无法下载/安装”，企业级别的生产证书可以安装到任意苹果设备，而公司级别的生产证书无法安装。
由于苹果的种种限制，所以我们开发，测试，预发布用公司开发证书，演示用企业生产证书
## 3. jenkins配置；
#### 3.1 添加必要插件
![](http://upload-images.jianshu.io/upload_images/1654251-cbdc73a979bbfa90.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1654251-f40845de8c29197f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1654251-472981d41eb1317a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
  插件名称：                                                                插件作用：[](http://wiki.hudson-ci.org/display/HUDSON/FTP-Publisher+Plugin)
> 
[Xcode integration                                                         xcode打包命令](https://wiki.jenkins-ci.org/display/JENKINS/Xcode+Plugin)[](https://wiki.jenkins-ci.org/display/JENKINS/Xcode+Plugin)
[](https://wiki.jenkins-ci.org/display/JENKINS/Xcode+Plugin)[FTP publisher plugin](http://wiki.hudson-ci.org/display/HUDSON/FTP-Publisher+Plugin)[把ipa包上传到ftp服务器](http://wiki.jenkins-ci.org/display/JENKINS/Keychains+and+Provisioning+Profiles+Plugin)
[Keychains and Provisioning Profiles Management         管理证书和授权文件](http://wiki.jenkins-ci.org/display/JENKINS/Keychains+and+Provisioning+Profiles+Plugin)
[Post-Build Script Plug-in                                              执行脚本](http://wiki.jenkins-ci.org/display/JENKINS/Keychains+and+Provisioning+Profiles+Plugin)
[Subversion Plug-in                                                       版本控制](http://wiki.jenkins-ci.org/display/JENKINS/Keychains+and+Provisioning+Profiles+Plugin)
## 3.2 创建项目
![](http://upload-images.jianshu.io/upload_images/1654251-44b8cdbb78716556.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1.点击>新建>输入item名称，选择构建一个自由风格的软件项目
![](http://upload-images.jianshu.io/upload_images/1654251-d90c6662452f0041.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
2.添加svn地址
![](http://upload-images.jianshu.io/upload_images/1654251-b6d5cf2910ed75f3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
3.每30分钟更新update 如果有更新，自动打包
![](http://upload-images.jianshu.io/upload_images/1654251-7c44eabad5a5ef66.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
4.添加钥匙串配置证书
![](http://upload-images.jianshu.io/upload_images/1654251-0fd56b54a4188a9a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
5.配置项目
![](http://upload-images.jianshu.io/upload_images/1654251-4afb030904455e40.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
6.配置项目
![](http://upload-images.jianshu.io/upload_images/1654251-ae3aa38745dc8918.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
7.配置项目
![](http://upload-images.jianshu.io/upload_images/1654251-2737f00849a1ee52.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
8.构建完成把ipa包发送到部署服务器
![](http://upload-images.jianshu.io/upload_images/1654251-74a81a5dd8ef58d6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
9.点击》立即构建
![](http://upload-images.jianshu.io/upload_images/1654251-b1a2c8af2668ffca.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
10.build成功
## 4.上传ipa包
#### 4.1 上传到自己的部署服务器
把打好的ipa文件发送到ftp服务器，配置plist文件（bundle identity必须和plist文件中的一致否则无法安装，而且plist文件的下载链接必须是https）（[plist文件下载](http://pan.baidu.com/s/1dE4D7qT) ）配置完成后在Safari打开
![](http://upload-images.jianshu.io/upload_images/1654251-3af555f62414c88d.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### 4.2 上传到第三方发布平台（[蒲公英](https://www.pgyer.com/) 或[fir](http://fir.im/) ）
在第三方平台注册账号，利用脚本把打好的ipa文件直接上传到第三方平台
![](http://upload-images.jianshu.io/upload_images/1654251-9efdb3a169030474.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
使用curl命令上传文件 （[参考](https://www.pgyer.com/doc/api#uploadApp)）
> 
cp ${WORKSPACE}/PICC/build/Debug-iphoneos/build/*.ipa ${WORKSPACE}/PICC/build/Debug-iphoneos/build/PICC.ipa
curl -F "file=@${WORKSPACE}/PICC/build/Debug-iphoneos/build/PICC.ipa" -F
"uKey=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" -F
"_api_key=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
http://www.pgyer.com/apiv1/app/upload
上传成功后可以扫描二维码下载安装
