# Centos Svn服务器搭建与配置 - BlueSky - CSDN博客
2015年12月22日 20:44:54[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：434

## 1.yum安装svn
`yum install -y subversion    `
## 2.验证是否安装完成
`svnserve --version  `
## 3.创建svn版本库
```
mkdir /home/iienet/svn   #把版本库放在前面文件夹，方便管理
svnadmin create /home/iienet/svn/repo #我这里将svn作为所有版本库的目录，并创建了一个名为repo的版本库
```
## 4.配置当前的版本库
说明：
           （1）svnserve.conf：  svn服务综合配置文件。
           （2）passwd： 用户名口令文件。
           （3）authz： 权限配置文件。
### 4.1 修改passwd文件
添加用户名与密码，是以后再svn中使用的用户名与密码
eg：
`asia = 654321`
### 4.2修改authz文件
```
[aliases]  
# joe = /C=XZ/ST=Dessert/L=Snake City/O=Snake Oil, Ltd./OU=Research Institute/CN=Joe Average  
#这里实现了别名的定义  
[groups]  
# harry_and_sally = harry,sally  
# harry_sally_and_joe = harry,sally,&joe  
team0=asia,wzw,szx 
# [/foo/bar]  
# harry = rw  
# &joe = r #上面的别名定义在这里实现 &在这里理解为指针就很容易了  
# * =  
# [repository:/baz/fuz]  
# @harry_and_sally = rw  
# * = r  
[/]  
@team0=rw  
#小组成员有读写的权限
```
### 4.3 修改svnserve.conf
```
### This file controls the configuration of the svnserve daemon, if you
### use it to allow access to this repository.  (If you only allow
### access through http: and/or file: URLs, then this file is
### irrelevant.)
### Visit http://subversion.tigris.org/ for more information.
[general]
### These options control access to the repository for unauthenticated
### and authenticated users.  Valid values are "write", "read",
### and "none".  The sample settings below are the defaults.
anon-access = read
auth-access = write
### The password-db option controls the location of the password
### database file.  Unless you specify a path starting with a /,
### the file's location is relative to the directory containing
### this configuration file.
### If SASL is enabled (see below), this file will NOT be used.
### Uncomment the line below to use the default password file.
password-db = passwd
### The authz-db option controls the location of the authorization
### rules for path-based access control.  Unless you specify a path
### starting with a /, the file's location is relative to the the
### directory containing this file.  If you don't specify an
### authz-db, no path-based access control is done.
```
## 5 停止和启动svn
`svnserve -d -r /home/iienet/svn/  --listen-port 3399`
OK,以上启动完毕之后就会看到运行成功了
将库文件导入到服务器的项目版本库，如下：
svn import /home/cs file:///data/svn/cs -m "new project"
下载路径 svn://10.10.16.3:3399/ 

