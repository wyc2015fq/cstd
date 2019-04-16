# Linux系统的用户 - 紫芝的博客 - CSDN博客





2018年10月19日 18:31:00[紫芝](https://me.csdn.net/qq_40507857)阅读数：32








# 什么是用户？

**系统中每个进程（应用程序）都为一个指定用户运行，每一个文件属于一个特定用户，对文件和目录的访问受到用户的限制；**

**id命令：显示当前已登录用户的信息；**

**id username 命令：显示其他用户信息**

![](https://img-blog.csdn.net/20181019213226759?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**ls -l 命令： 查看与某一文件或目录相关联的用户，第3列显示用户名**

![](https://img-blog.csdn.net/2018101921431581?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**ps命令：显示当前shell中的进程；**

**ps a 命令：查看与某一终端相关的所有进程；**

**ps au 命令：查看与进程相关联的用户，第1列显示用户名**

![](https://img-blog.csdn.net/20181019215254160?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## Linux系统采用平面文件（/etc/passwd文件）存储有关本地用户的信息

**/etc/passwd采用以下格式（7个冒号分隔字段）**：

**username：password：UID：GID：GECOS：/home/dir：shell**
- username：UID到用户名的映射，便于用户使用
- password：以前是以加密方式保存密码的位置；现在密码存储在/etc/shadow的单独文件中
- UID：用户ID，最基本的标识用户的编号；
- GID：用户主要组ID编号
- GECOS：用户名称的别名（通常是实际姓名）
- /home/dir:用户个人数据和配置文件
- shell：用户登录时运行的程序，对于普通用户，通常是用户命令行提示符的程序；

# UID范围：
- **UID 0：始终分配给超级用户**
- **UID 1-200：一系列系统用户，静态分配给系统进程**
- **UID 201-999：一系列系统用户，供文件系统中没有自己的文件的系统进程使用；通常在安装需要他们的软件时，从可用池中动态分配给他们，程序以无特权系统用户身份运行，以便限制他们仅访问正常运行所需的资源**
- **UID 1000+：可供分配给普通用户的资源**

# 管理本地用户：

**useradd username：创建用户，登录密码随机设置**

**passwd username：修改密码**

**usermod：修改现有用户**

**usermod --help：显示可用于修改账户的基本选项**

![](https://img-blog.csdn.net/20181020144957647?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**userdel username：将用户从/etc/passwd中删除，默认情况下，保留主目录不变**

**usedel -r username:同时删除用户和主目录**



## windows10 Subsystem Linux ：Ubuntu root pass word

**安装完Ubuntu之后，忽然意识到没有设置root密码；Ubuntu的默认root密码是随机的，每次重启都有一个新的root密码；**

**更改方法是：输入sudo passwd,然后输入密码，再次确认密码即可。**

![](https://img-blog.csdn.net/20181019212115950?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**[username @ hostname~]$**

username：当前用户的用户名；

hostname：主机名称

~：代表当前目录

$：普通用户；

#：root用户

# Linux下用户之间的切换

切换用户的命令是su，su是(switch user)切换用户的缩写。通过su命令，

**1.从普通用户切换到root用户；从普通用户切换到root用户需要密码(该密码是root的密码)**

在终端输入su或者su root，然后回车；输入root密码即可

![](https://img-blog.csdn.net/20181019210628905?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**2.从root用户切换到普通用户；从root用户切换到普通用户不需要密码。**

在终端输入su。然后回车；

![](https://img-blog.csdn.net/20181019210029107?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

***由普通用户返回root用户：输入exit，然后回车即可；***

![](https://img-blog.csdn.net/20181019210343927?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**3.从普通用户切换到另一个普通用户**

终端输入su username，然后回车，输入用户密码即可

![](https://img-blog.csdn.net/2018101921095098?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



