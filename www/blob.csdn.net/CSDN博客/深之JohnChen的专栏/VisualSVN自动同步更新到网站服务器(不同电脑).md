# VisualSVN自动同步更新到网站服务器(不同电脑) - 深之JohnChen的专栏 - CSDN博客

2017年03月17日 13:58:44[byxdaz](https://me.csdn.net/byxdaz)阅读数：3386


一、如何将VisualSVN中的代码同步到web服务器上？

使用[SVN设置钩子将代码同步到Web目录下面](http://blog.csdn.net/xiaoxiaoqiye/article/details/51911011)，要在SVN服务中，找到仓库文件夹的位置，在相应的项目中找到hooks文件夹。在该文件中添加一个post-commit文件；当有commit动作发生时（提交到SVN服务是就会执行这个post-commit文件，在该文件实现数据复制到web目录下。

二、操作步骤

1、在svn服务器上安装VisualSVN和
 TortoiseSVN。

2、将web服务器中的一个磁盘映射到svn服务器上。具体磁盘映射方式使用net
 use命令。net use z: \\IP\c$ "密码" /user:"帐号" 将对方的c盘映射为自己的z盘。比如：net
 use \\192.168.1.202\d$ admin20162016. /user:administrator

net use使用说明参考：[http://bluepig.blog.51cto.com/440658/108636](http://bluepig.blog.51cto.com/440658/108636)

3、在svn服务器上找到刚才网络映射的磁盘，在磁盘上建立一个需要同步web项目的目录，比如：z:\work\website,同时使用TortoiseSVN将需要web项目checkout到此目录。

4、在visualSVN的存储目录中找到hooks,在hooks下建立一个post-commit.bat的文件内容如下:

```
@echo off
“C:\visualSVN\VisualSVN Server\bin\svn.exe” update “\\192.168.1.202\d$\ work\website” –username svn –password 000000
```
``
解析:“svn的程序” update “服务器web目录”–username用户名 –password密码。

如果web服务器与svn服务器同一台电脑的情况，直接将服务器web目录换成本地目录即可。

@echo off
“C:\visualSVN\VisualSVN Server\bin\svn.exe” update “d:\work\website”–username svn –password 000000

这里hooks下有
 post-commit模板文件，不过是 linux下用的例子
post-commit.bat 的作用就是在你用 Tortoise进行
 commit成功后，执行一个批处理，就是post-commit.bat的文件。

完整的post-commit.bat文件

```
@echo off 
 
SET SVN="C:\Program Files\VisualSVNServer\bin\svn.exe"
SET REPOS=%1
SET REV=%2 
 
SET DIR=%REPOS%/hooks
SET PATH=%PATH%; 
 
net use \\192.168.1.202\d$ admin20162016./user:administrator //这一句去掉也行
SET WORKING_COPY="\\192.168.1.202\d$\work\website"
SVN update %WORKING_COPY% --username=liwei--password=liwei
```

三、注意问题：

1、visualSVN必须设置成不用
 https来访问的方式连接
2、如果你现在遇到上面错误时已经使用了这种https方法，那么就重新设置一下，设置为没有https的方法进行访问!并重启
 visualSVN，然后将你的已经 checkout的文件夹删除，重新以新的连接方式
 checkout下来 ,然后再试因为你以前
 checkout用的是 443的方法，再执行
 post-commit.bat时总是调用 443这个连接,所以要删除并checkout一下。

3、“服务器web目录”不是跟svn服务器同一台电脑的话，使用
 SVN HOOK 功能的时候遇到如下问题：

SVN:E020024：Error
 resolving case of xxxx

解决方法：如果web服务器是远程电脑，需要做磁盘映射，将远程电脑磁盘网络映射到svn服务器电脑上，同时在“服务器web目录”中带上远程web服务器共享路径地址。

net use \\192.168.1.X\xxx /usr: username password

SVN UPDATE \\192.168.1.X\XXX -username-password

