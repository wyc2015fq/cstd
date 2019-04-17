# Linux环境下SVN服务器端的安装与配置 - DoubleLi - 博客园






最近尝试了下在Linux（CentOS6.5）环境下安装与配置SVN服务器端，安装过程中碰到了一些问题，参看了网友们分享的一些心得，并通过自己实际的操作，最终安装与配置成功！总的来说网上的说法芸芸，大致都相同，但是有些细节不够详细或者不够到位，所以根据自己亲身经历，记录下安装与配置过程，仅供大家参看以及自己之后再次使用提供方便。 
该编博客为安装配置完成之后写的，所以如果有遗漏操作，还请大家批评指正。

下面开始：

### **1.SVN服务器端压缩包下载**

下载地址：[http://mirrors.cnnic.cn/apache/subversion/](http://mirrors.cnnic.cn/apache/subversion/)
我下载的是subversion-1.8.15版本。 
下载好的压缩包置于目录 /opt/env 下(根据大家实际需要自行选择)。

### **2.解压安装**

Linux解压命令 
命令格式：
`# tar -zxvf subversion-1.8.15.tar.gz`- 1
- 1

解压后/opt/env目录下出现文件夹subversion-1.8.15， 
为了方便，顺便建立软链接： 
命令格式：
`# ln -s ./svn ./subversion-1.8.15 `- 1
- 1

建立软链接之后访问 svn目录即相当于访问subversion-1.8.15

安装非常简单，解压后即已经安装完成。接下来是配置。

### **3.SVN版本库的建立**

我选择的目录是 /home/svn/作为SVN版本库的根目录。 
新建/svn目录：
`# mkdir /home/svn`- 1
- 1

比如，现在我有一个名为“project”的项目需要用SVN做版本管理，那么我可以在svn根目录下建立一个 /project目录，我最终目的想让项目托管到/project目录下。接下来我需要新建这个目录：/home/svn/project 。 
命令如下：
`# mkdir -p /home/svn/project`- 1
- 1

然后需要将/project目录设定为版本库，命令如下：
`# svnadmin create /home/svn/project`- 1
- 1

设定/project目录为版本库后，会发现/project目录下会多出以下文件：

```
drwxr-xr-x 6 root root 4096 Jan 16 00:05 .
drwxr-xr-x 3 root root 4096 Jan 16 00:04 ..
drwxr-xr-x 2 root root 4096 Jan 16 00:08 conf
drwxr-sr-x 6 root root 4096 Jan 16 00:22 db
-r--r--r-- 1 root root    2 Jan 16 00:05 format
drwxr-xr-x 2 root root 4096 Jan 16 00:05 hooks
drwxr-xr-x 2 root root 4096 Jan 16 00:05 locks
-rw-r--r-- 1 root root  229 Jan 16 00:05 README.txt
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9

### **4.svnserve配置**

接下来的配置就在/conf文件夹中的文件：
- svnserve.conf
- passwd
- authz

我们可以看到，conf目录下共有三个文件：svnserve.conf、passwd、authz。 
svnserve的配置主要就是对这三个文件的配置。

### ***5.下面这些说明可以帮助理解：***

> 
**提示：**快速看一遍就好，尽快转入下方“言归正传”。


***- svnserve.conf***

svn服务配置文件，该文件版本库目录的conf目录下，文件名为svnserve.conf。 
[general]配置段中配置行格式： 
<配置项> = <值> 
配置项分为以下5项：

anon-access控制非鉴权用户访问版本库的权限。取值范围为”write”、”read”和”none”。 
即”write”为可读可写，”read”为只读，”none”表示无访问权限。 
缺省值：read

auth-access控制鉴权用户访问版本库的权限。取值范围为”write”、”read”和”none”。 
即”write”为可读可写，”read”为只读，”none”表示无访问权限。 
缺省值：write

password-db 指定用户名口令文件名。除非指定绝对路径，否则文件位置为相对conf 
目录的相对路径。 
缺省值：passwd

authz-db 指定权限配置文件名，通过该文件可以实现以路径为基础的访问控制。 
除非指定绝对路径，否则文件位置为相对conf目录的相对路径。 
缺省值：authz 
realm 指定版本库的认证域，即在登录时提示的认证域名称（就是个作为提示用的，不用太纠结）。

***- passwd***

用户名口令文件，该文件名在文件svnserve.conf中指定，缺省为同目录下的passwd。 
该文件仅由一个[users]配 置段组成。

[users]配置段的配置行格式如下： 
<用户名> = <口令> 
注意：配置行中的口令为未经过任何处理的明文。

例：用户名口令文件conf/passwd的内容如下： 
[users] 
admin = admin 
zhangsan = 123456

该文件中配置了两个用户，用户名分别为”admin”和”zhagnsan”。其中”admin”用户的口令为”admin”；”zhangsan”用户的口 令为”123456”。

***- authz***

权限配置文件，该文件名也在文件svnserve.conf中指定，缺省为同目录下的authz。

该配置文件由一个[groups]配置段和 若干个版本库路径权限段组成。

[groups]配置段中配置行格式如下： 
<用户组> = <用户列表> 
用户列表由若干个用户组或用户名构成，用户组或用户名之间用逗号”,”分隔，引用用户组时要使用前缀”@”(如：引用用户组”all”要使用字符 串”@all”)。

版本库路径权限段的段名格式如下： 
[<版本库名>:<路径>] 
如版本库abc路径/tmp的版本库路径权限段的段名为”[abc:/tmp]”。 
可省略段名中的版本库名。若省略版本库名，则该版本库路径权限段对所有版本库中相同路径的访问控制都有效。如：段名为”[/tmp]”的版本库路径权限段 设置了所有引用该权限配置文件的版本库中目录”/tmp”的访问权限。

版本库路径权限段中配置行格式有如下三种： 
<用户名> = <权限> 
<用户组> = <权限> 
* = <权限> 
其中，”*”表示任何用户；权限的取值范围为”、’r’和’rw’，”表示对该版本库路径无任何权限，’r’表示具有只读权限，’rw’表示有读写权 限。 
注意：每行配置只能配置单个用户或用户组。

例3：权限配置文件conf/authz的内容如下： 
[groups] 
g_admin = admin,zhangsan

[admintools:/] 
@g_admin = rw 
* =

[test:/home/zhangsan] 
zhangsan = rw 
* = r

在上述配置文件中，定义了一个用户组”g_admin”，该用户组包含用户”admin”和”zhangsan”。然后定义了2个版本库路径权限段。其中， 版本库”admintools”只有用户组”g_admin”可读写，其他用户无任何权限；版本库”test”中路径”/home/zhangsan”只有 用户”zhangsan”有读写权限，其他用户只有可读权限。

### **6.言归正传！开始配置！**

**(1). 配置svnserve.conf**

```
[general]
anon-access = none
auth-access = write
password-db = passwd
authz-db = authz
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5

vi命令编辑svnserve.conf，insert进入编辑模式，放开以上选项，ESC :wq保存。设置非鉴权用户无权访问、设置鉴权用户可读写、口令及权限配置参照conf目录下passwd和authz的配置。

> 
**重要提醒！！：**首行不可以留空，要顶行，否则会造成SVN识别为配置错误。


**(2). 配置passwd**

```
[users]
rokay = 123456
```
- 1
- 2
- 1
- 2

配置了一个用户为“rokay”，密码为“123456”的用户。

**(3). 配置authz**

```
[groups]
g_admin = rokay

[/]
@g_admin = rw
*=
```
- 1
- 2
- 3
- 4
- 5
- 6
- 1
- 2
- 3
- 4
- 5
- 6

配置了一个管理员群组“g_admin”，包含用户“rokay”； 
“[/]”表示对根目录“trunk”的权限配置。 
配置了“g_admin”群组对于该目录下的权限为“rw“（读、写）权限。 
* = 配置了除群组”g_admin“外的其他用户均无权访问。

> 
**注意：**也可以用“,”分隔多个用户或者@+群组名称的方式配置多用户，参看上方说明。


综上，就完成了对三个文件也就是svnserve的配置了。

### **7.启动SVN服务**

输入命令：
`# svnserve -d -r /home/svn`- 1
- 1

> 
**注意：**/svn目录为版本库的根目录，所以路径仅到/svn即可。


此时，SVN服务器的配置以及版本库已经建立完成！

### **8.SVN Windows客户端访问**

现以已集成了SVN插件的eclipse为例，在SVN资源库中 [新建]—[资源库位置]，URL输入：

***svn://[服务器IP地址]/project***

![这里写图片描述](http://img.blog.csdn.net/20160124011132797)

输入已授权的用户信息登录即可访问资源位置。 
同样的，可以通过右击项目[team]—[share]，输入以上URL将项目共享至新建的SVN版本库中。

> 
**注意：**上述地址中没有”/svn”这一级目录，”/svn”为版本库根目录，不用输入。


### **9.新建多个版本库**

若要在/svn目录下添加新的版本库，直接新建相应的目录，然后设置为版本库，最后配置即可，不需要重新运行svnserve -d -r /home/svn，也不需要把该命令应用到/svn目录下你所新建的目录，如果因为误操作执行了以上命令，会导致资源库不可用，再次运行svnserve -d -r /home/svn也会报：“不能绑定服务器套接字 地址已在使用”的错误字样“（若已至此，删除掉新建的资源库，并用ps -ef | grep svnserve查看进程，kill -9 [进程号] 杀掉，重新执行svnserve -d -r /home/svn，再次新建资源库，直接配置即可使用）

比如在/svn目录下另外新建一个project2的资源库，命令如下：

```
# mkdir -p /home/svn/project2
# svnadmin create /home/svn/project2
...（三个文件的配置）
（不需再运行svnserve -d -r /home/svn）
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4

一切搞定！

*本文为安装配置完成后回想步骤记录的博文，若有出入或遗漏之处还请指正，多多包涵！*









