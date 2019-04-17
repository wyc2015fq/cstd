# 网页版Rstudio︱RStudio Server多人在线协作开发 - 素质云笔记/Recorder... - CSDN博客





2016年02月17日 16:39:15[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：6410










网页版Rstudio︱RStudio Server多人在线协作开发



想了解一下RStudio Server，太给力的应用，可以说成是代码分布式运行，可以节省时间，放大空间。



RStudio是一个非常优秀的R语言IDE。 RStudio除了在各种系统中有桌面版本外还有Server版，它可以装载在linux主机上，用户可以通过浏览器远程登录使用R进行数据分析。笔者在之前也曾经折腾过好几回都没成功，不过今天运气不错居然实现了。



最近更新（2016-11-29）：

[R︱Linux+Rstudio Server尝鲜笔记（打造最佳Rstudio体验+报错的解决方案）](http://blog.csdn.net/sinat_26917383/article/details/53390787)



——————————————————————————————



# 一、Rstudio Server下载



部署RStudio Server并不是件困难的事情，根据官方说明，一共包括四个步骤：
- 安装Extra Packages for Enterprise Linux (EPEL);
- 安装R;
- 安装RStudio Server;
- 配置RStudio Server.

## 1.安装EPEL

安装EPEL可以使你更方便的从EPEL来安装R语言，而无需通过其他途径来安装R。如果你运行的是RedHat或CentOS系统的话，你也要安装EPEL来确保RStudio Server正常运行。

不同系统版本安装EPEL的代码如下：

RHEL 5.x / CentOS 5.x

32位系统
|`1`|`$ rpm -Uvh [http://download.fedoraproject.org/pub/epel/5/i386/epel-release-5-4.noarch.rpm](http://download.fedoraproject.org/pub/epel/5/i386/epel-release-5-4.noarch.rpm)`|
|----|----|

64位系统
|`1`|`$ rpm -Uvh [http://download.fedoraproject.org/pub/epel/5/x86_64/epel-release-5-4.noarch.rpm](http://download.fedoraproject.org/pub/epel/5/x86_64/epel-release-5-4.noarch.rpm)`|
|----|----|

RHEL 6.x / CentOS 6.x

32位系统
|`1`|`$ rpm -Uvh [http://download.fedoraproject.org/pub/epel/6/i386/epel-release-6-8.noarch.rpm](http://download.fedoraproject.org/pub/epel/6/i386/epel-release-6-8.noarch.rpm)`|
|----|----|

64位系统
|`1`|`$ rpm -Uvh [http://download.fedoraproject.org/pub/epel/6/x86_64/epel-release-6-8.noarch.rpm](http://download.fedoraproject.org/pub/epel/6/x86_64/epel-release-6-8.noarch.rpm)`|
|----|----|

接下来运行：
|`1`|`$ yum upgrade`|
|----|----|

等待安装完成即可。

## 2.安装R

当EPEL成功安装后，使用以下代码便可以轻松的进行R语言的安装：
|`1`|`$ sudo yum install R`|
|----|----|

## 3.安装RStudio Server

对于RedHat/CentOS (5.4+)，安装RStudio Server的代码为：

32位系统
|`1`|`$ wget [http://download2.rstudio.org/rstudio-server-0.98.501-i686.rpm](http://download2.rstudio.org/rstudio-server-0.98.501-i686.rpm)`|
|----|----|
|`2`|`$ sudo yum install --nogpgcheck rstudio-server-0.98.501-i686.rpm`|
|----|----|

64位系统
|`1`|`$ wget [http://download2.rstudio.org/rstudio-server-0.98.501-x86_64.rpm](http://download2.rstudio.org/rstudio-server-0.98.501-x86_64.rpm)`|
|----|----|
|`2`|`$ sudo yum install --nogpgcheck rstudio-server-0.98.501-x86_64.rpm`|
|----|----|

## 4.配置RStudio Server

RStudio Server默认使用的端口是8787，因此在浏览器中输入以下地址便可以看到登录界面：
|`1`|`[http://<server-ip>:8787](/)`|
|----|----|



但是你现在还不能使用你的管理员账号来登录，因为RStudio Server禁止系统级用户登录（包括所有ID小于100的用户），这可能是出于安全因素考虑。因此，必须先添加一个满足要求的新用户。





——来源于[Hawkin](http://www.hawkinqian.com/zh/author/qianhaoqi/)博客：http://www.hawkinqian.com/zh/r-cloud-deploying-rstudio-server/

还可以参考博客：http://www.linuxidc.com/Linux/2012-02/54610.htm



——————————————————————————————

# 二、RStudio Server的使用



通过浏览器，我们访问RStudio Server: http://192.168.1.13:8787

![rstudio-login](http://blog.fens.me/wp-content/uploads/2013/07/rstudio-login.png)

通过Linux系统用户登陆: conan:conan111

![rstudio-web](http://blog.fens.me/wp-content/uploads/2013/07/rstudio-web.png)

太帅了是不是，web版的RStudio!!



——————————————————————————————



# 三、RStudio Server多人在线协作开发

## 1、增加新用户  案例1



```
~ sudo groupadd hadoop 
~ sudo useradd hadoop -g hadoop;
~ sudo passwd hadoop 
~ sudo adduser hadoop sudo
~ sudo mkdir /home/hadoop 
~ sudo chown -R hadoop:hadoop /home/hadoop

#测试账号设置成功
~ ssh hadoop@localhost
~ bash
~ pwd
/home/hadoop
```



新打开浏览器窗口通过 hadoop账号登陆
![rstudio-web2](http://blog.fens.me/wp-content/uploads/2013/07/rstudio-web2.png)





## 2、添加新用户 案例2



添加用户的命令为：
|`1`|`useradd username -u uid -p password`|
|----|----|

千万记得将uid设定为大于100的数字，大于500更好。

也许你会发现使用新创建的用户名依然无法登录，至少我的情况是这样。对此，我找到了解决办法，即通过修改RStudio Server的用户组限制来解决。

在设置用户组权限之前，必须先创建2个配置文件，RStudio Server在安装时不会自动创建这两个文件，必须手动操作。代码如下：
|`1`|`$ touch /etc/rstudio/rserver.conf`|
|----|----|
|`2`|`$ touch /etc/rstudio/rsession.conf`|
|----|----|
|`3`|`$ ls`|
|----|----|

编辑rserver.conf文件并添加以下代码：
|`1`|`auth-required-user-group=rstudio_users`|
|----|----|

其中，你可以将“rstudio_users”命名为任何你想要的群组名字。

接下来，将刚才新建立的用户名添加到该用户组：
|`1`|`$ groupadd rstudio_users`|
|----|----|
|`2`|`$ usermod -g rstudio_users -G rstudio_users username`|
|----|----|
|`3`|`$ rstudio-server restart`|
|----|----|

重启RStudio Server之后，你会发现终于可以用新用户登录啦！赶紧来体验在iPad上操作R的感觉吧！

还有一件事情必须注意噢！如果你使用其他电脑的浏览器来远程使用R的话，请确保至少采用以下版本的浏览器：
- Firefox 10;
- Safari 5.0;
- Google Chrome 15.

RStudio Server原生并不支持IE浏览器，但是你可以通过安装[谷歌浏览器内嵌框架](http://www.google.com/chromeframe)这个IE插件来实现IE操作。

如果你希望了解更多部署RStudio Server的信息，请前往[RStudio Server Website](https://www.rstudio.com/ide/download/server)进行了解。



——————————————————————————————



# 四、其他功能



## 1、Git代码共享

安装git



```
sudo apt-get install git

#生成rsa密钥对
ssh-keygen -t rsa

#查看公钥并
cat /home/conan/.ssh/id_rsa.pub
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDMmnFyZe2RHpXaGmENdH9kSyDyVzRas4GtRwMNx+qQ4QsB8xVTrIbFayG2ilt+P8UUkVYO0qtUJIaLRjGy/SvQzzL7JKX12+VyYoKTfKvZZnANJ414d6oZpbDwsC0Z7JARcWsFyTW1KxOMyesmzNNdB+F3bYN9sYNiTkOeVNVYmEQ8aXywn4kcljBhVpT8PbuHl5eadSLt5zpN6bcX7tlquuTlRpLi1e4K+8jQo67H54FuDyrPLUYtVaiTNT/xWN6IU+DQ9CbfykJ0hrfDU1d1LiLQ4K2Fdg+vcKtB7Wxez2wKjsxb4Cb8TLSbXdIKEwSOFooINw25g/Aamv/nVvW1 conan@conan-deskop
```

把公钥并上传到github

创建一个新的Project: rstudio-demo

https://github.com/bsspirit/rstudio-demo

绑定本定项目和rstudio-demo

```
~ cd /home/conan/R/github
~ git init
~ git add .
~ git commit -m 'first comment'
~ git remote add origin git@github.com:bsspirit/rstudio-demo.git
~ git push -u origin master
```

打开RStudio设置到/home/conan/R/github目录，tools–>version control –> project setup
![rstudio-git](http://blog.fens.me/wp-content/uploads/2013/07/rstudio-git.png)

在RStudio中修改sayHello.r的代码

```
sayHello<-function(name){
  print(paste("hello",name))
}

sayHello("Conan")
sayHello("World")
```

提交：点击tools–>version control–> commit
![rstudio-git2](http://blog.fens.me/wp-content/uploads/2013/07/rstudio-git2.png)

上传到github：点击tools–>version control–> push
![rstudio-git3](http://blog.fens.me/wp-content/uploads/2013/07/rstudio-git3.png)

RStudio有如此强大的功能，极大的降低了编程的门槛。还没有用过的同学，赶紧去体验一把吧。极客的感觉！



## 2、云平台中的RStudio

如果有同道中人想进一步折腾可以考虑和云平台相接合，例如EC2。Amazon EC2是Amazon Web Services（简称Aws）中的一个主要产品。 Amazon EC2可以看作是一台云主机，云主机上不仅可以搭建网站，也可以运行RStudio Sever。目前Amazon对微型主机提供了新注册用户免费试用一年的活动，爱折腾的同学可以试试。当然国内的盛大云也有超微型主机提供，一年400多元，比较适合个人或小型公司使用。


## 使用浏览器运行R的好处：
- 你可以从任何一台能上网的计算机去操作R
- 共享代码、数据和其它文档
- 多用户共享高性能计算资源与协同工作
- 集中管理R扩展包



设想一下，你作为公司数据分析师到客户那里做咨询或培训，使用客户电脑和RStudio Server就能直接利用公司主机上的数据集和程序代码作演示，而不必携带厚重的笔记本出差，这样岂不轻松又显得有科技含量。





报错：

### 解決登入 DOCKER 的 RSTUDIO 遇到錯誤 RSTUDIO INITIALIZATION ERROR: UNABLE TO CONNECT TO SERVICE

打開瀏覽器網址: [http://localhost:8787/](http://localhost:8787/)


![](https://3.bp.blogspot.com/-XYhmRZ0HMPw/WvaGfTzFnCI/AAAAAAAAL9c/APLHkTmEou4RRqc-NTzIqdP4QYRx0xkugCLcBGAs/s320/Sign%2Bin%2Bto%2BRStudio%2B%25E8%259E%25A2%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%2B2018-05-12.png)



在 Username 與 Password 輸入剛剛建立的帳號密碼，卻遇到錯誤 Rstudio Initialization Error: Unable to connect to service


![](https://2.bp.blogspot.com/-HDI6BGIuYjQ/WvaGhvvE_VI/AAAAAAAAL9g/MkNYc6giAggokLIxS0TX2r94bp9IUlhLACLcBGAs/s320/RStudio%2BInitialization%2BError%2B%25E8%259E%25A2%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%2B2018-05-12.png)

**解決方式**


法1: 不需要額外建立帳號，直接輸入預設的帳號 rstudio 與密碼 rstudio


法2: 啟動 Container 指定帳號密碼。

(如果 Container 已經啟動，需要停止，避免衝到相同 port )



 综合参考博客：

1、http://www.linuxidc.com/Linux/2012-02/54610.htm

2、http://blog.fens.me/r-rstudio-server/

3、http://www.hawkinqian.com/zh/r-cloud-deploying-rstudio-server/







