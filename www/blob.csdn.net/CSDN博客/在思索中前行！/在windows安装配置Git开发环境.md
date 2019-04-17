# 在windows安装配置Git开发环境 - 在思索中前行！ - CSDN博客





2014年10月19日 20:31:50[_Tham](https://me.csdn.net/txl16211)阅读数：898标签：[windows Git 配置																[Git 安装配置](https://so.csdn.net/so/search/s.do?q=Git 安装配置&t=blog)](https://so.csdn.net/so/search/s.do?q=windows Git 配置&t=blog)
个人分类：[linux学习																[SVN/Github版本控制](https://blog.csdn.net/txl16211/article/category/2340075)](https://blog.csdn.net/txl16211/article/category/2231071)








开始配置Git的开发环境。首先从[google  code](http://code.google.com/p/msysgit/downloads/list)下载最新的windows的git安装包msysgit，当时我下载的是Git-1.7.4-preview20110204.exe,然后就开始安装了，开始上图：

http://blog.csdn.net/jiguanghoverli/article/details/7902791  XP下搭配Git环境


## 安装Msysgit

![](http://www.xbc.me/wp-content/uploads/install-git-1.jpg)


下一步

![](http://www.xbc.me/wp-content/uploads/install-git-2.jpg)


同意GNU协议

![](http://www.xbc.me/wp-content/uploads/install-git-3.jpg)


选择安装位置，下一步

![](http://www.xbc.me/wp-content/uploads/install-git-4.jpg)


选择TrueType  Front，下一步

![](http://www.xbc.me/wp-content/uploads/install-git-5.jpg)


不创建启动文件夹

![](http://www.xbc.me/wp-content/uploads/install-git-6.jpg)


默认Git Bash，就可以了

![](http://www.xbc.me/wp-content/uploads/install-git-34.jpg)


选择使用OpenSSH

![](http://www.xbc.me/wp-content/uploads/install-git-33.jpg)


选择默认的Checkout Style

![](http://www.xbc.me/wp-content/uploads/install-git-7.jpg)


安装完成了

![](http://www.xbc.me/wp-content/uploads/install-git-8.jpg)

## 配置本地的Git


就这样安装好Msysgit后，就可以开始配置开发环境了，在你的工作目录建立一个新的文件夹Git，比如我的

![](http://www.xbc.me/wp-content/uploads/install-git-9.jpg)


选择Git文件夹，右键，选择Git Bash Here，会弹出shell命令行界面

![](http://www.xbc.me/wp-content/uploads/install-git-10.jpg)

### 生成你的Public Key


输入命令


ssh-keygen –C “你的email地址 “ –t rsa



就会为你生成一个 SSH Key，然后会询问一些保存文件的位置，设置密码神马的，直接回车，回车，回车，默认的就可以了


因为主要是在本地使用，可不使用口令，直接回车就可以了！

![](http://www.xbc.me/wp-content/uploads/install-git-12.jpg)


好了，现在为你生成了sshkey了。![](http://www.xbc.me/wp-content/uploads/install-git-13.jpg)

### 设置Github帐号的Public Key


如果你有一个github的账号的话，


登陆github.Com 网站，在[SSH Public Keys](https://github.com/account/ssh) 设置你的SSH Public
 Keys

![](http://www.xbc.me/wp-content/uploads/install-git-14.jpg)


创建一个数据仓库

![](http://www.xbc.me/wp-content/uploads/install-git-15.jpg)设置相关的信息

![](http://www.xbc.me/wp-content/uploads/install-git-16.jpg)

### 设置你的Email和Nickname


现在回到你的工作目录，可以选择设置你默认的email和nickname

输入命令


git config –global user.email 你的email地址
git config –global user.name "Arthur"


![](http://www.xbc.me/wp-content/uploads/install-git-17.jpg)

![](http://www.xbc.me/wp-content/uploads/install-git-18.jpg)

### Clone你的项目


右键你的工作目录Git，选择Git GUI，选择克隆已有版本库

![](http://www.xbc.me/wp-content/uploads/install-git-35.jpg)


输入你在github的项目地址，例如


git@github.com:caijiamx/Magento-Theme.git


![](http://www.xbc.me/wp-content/uploads/install-git-19.jpg)


会弹出一个框出来让你是否选择信任github，输入yes

![](http://www.xbc.me/wp-content/uploads/install-git-20.jpg)


然后就会从github拉取数据了，

![](http://www.xbc.me/wp-content/uploads/install-git-21.jpg)


前面简单的介绍如何克隆版本库，现在就要正式的开始工作了。

## 命令行下的Git开发


新建一个工作文件夹，右键打开 Git Bash，输入


ssh git@github.com



选择yes，加入到hosts中

![](http://www.xbc.me/wp-content/uploads/install-git-22.jpg)


信任[git@github.com](mailto:git@github.com)站点

![](http://www.xbc.me/wp-content/uploads/install-git-23.jpg)

### 配置你的Github上Username和Token


设置你的github 用户名


git config –global github.user caijiamx


![](http://www.xbc.me/wp-content/uploads/install-git-24.jpg)


设置你的git账户的token


Git config –global github.token your token



这个token在github,com的Account Settings->[Account
 Admin](https://github.com/account/admin)右侧找到。

![](http://www.xbc.me/wp-content/uploads/install-git-25.jpg)


我们重新开始上面介绍的从GUI拉取数据，现在我们从命令开始你的项目

![](http://www.xbc.me/wp-content/uploads/install-git-26.jpg)

## Git下的开发


为你的项目添加说明文件


touch readme.txt
//输入hello world


![](http://www.xbc.me/wp-content/uploads/install-git-27.jpg)

![](http://www.xbc.me/wp-content/uploads/install-git-28.jpg)


将新建的文件添加到暂存区，输入命令


git add readme.txt


![](http://www.xbc.me/wp-content/uploads/install-git-29.jpg)提交你的更改
 输入命令


git commit –m "first commit"


![](http://www.xbc.me/wp-content/uploads/install-git-30.jpg)


添加远程服务器仓库，输入命令


git remote add origin git@github.com:directoo/Magento-Theme.git


![](http://www.xbc.me/wp-content/uploads/install-git-31.jpg)


提交你的更改到服务器，输入命令


git push origin master


![](http://www.xbc.me/wp-content/uploads/install-git-32.jpg)参考资源

[github set up git](http://help.github.com/win-set-up-git/)

[如何设置你的SSH Key](http://github.com/guides/providing-your-ssh-key)

[github官方帮助教程](http://github.com/guides)

[初次运行 Git 前的配置](http://progit.org/book/zh/ch1-5.html?1303997080)





