# macOS 远程登录腾讯云(Ubuntu)的方法 - weixin_33985507的博客 - CSDN博客
2017年06月10日 21:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：30
相信mac用户在购买了腾讯云服务器后，操作不是很方便，每次都要使用网页的方法操作服务器。现在分享一个用mac终端即可登录腾讯云服务器的方法。
## 服务器端的设置
我们主要通过ssh的方法来远程登录，在服务器端需要设置ssh的链接服务，而ubuntu本身不自带该服务，因此我们需要安装服务：
`sudo apt-get update`
`sudo apt-get install openssh-server`
接着我们测试一下是否安装成功：
`ps -e | grep ssh`
出现 ‘sshd’则安装成功
## 使用终端登录
现在我们在mac的终端输入ssh链接的命令：
`ssh name@ip`
name是腾讯云给你的用户名，默认是ubuntu；ip是你主机的公网ip。输入后，会让你输入用户密码，用户名&密码验证成功之后，即登录了腾讯云服务器。
![1856999-c2e4b76958294565.png](https://upload-images.jianshu.io/upload_images/1856999-c2e4b76958294565.png)
退出ssh链接的命令是：`exit`
