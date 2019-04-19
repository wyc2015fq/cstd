# ubuntu搭建Git服务器 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月12日 14:57:56[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：279
远程仓库实际上和本地仓库没啥不同，纯粹为了7x24小时开机并交换大家的修改。
搭建Git服务器非常简单，通常10分钟即可完成
Ubuntu下面的
第一步，安装：
`$ su**do** apt-**get** install git`
第二步，创建一个`git`用户，用来运行`git`服务：
`$ su**do** adduser git``git用户，要设置个密码`
第三步，创建证书登录：
su git 
如果目录不存在在就运行ssh-keygen 
收集所有需要登录的用户的公钥，就是他们自己的`id_rsa.pub`文件，把所有公钥导入到`/home/git/.ssh/authorized_keys`文件里，一行一个。
cat id_ras.pub >> authorized_keys
注意`authorized_keys`文件权限为git
第四步，初始化Git仓库：
先选定一个目录作为Git仓库，假定是`/srv/``sample.git`，在`/srv`目录下输入命令：
`$ su**do** git init --bare sample.git`Git就会创建一个裸仓库，裸仓库没有工作区，因为服务器上的Git仓库纯粹是为了共享，所以不让用户直接登录到服务器上去改工作区，并且服务器上的Git仓库通常都以`.git`结尾。然后，把owner改为`git`：
`su**do** chown -R git:git sample.git`
```
```
```
```
``
```
禁用shell登录
```
``
git用户不允许登录shell，这可以通过编辑`/etc/passwd`文件完成。找到类似下面的一行：
`git:x:1001:1001:,,,:/home/git:/bin/bash`
改为：
`git:x:1001:1001:,,,:/home/git:/usr/bin/git-shell`
```
```
最后一步：
现在，可以通过`git clone`命令克隆远程仓库了，在各自的电脑上运行：
`$ git clone git@server:/srv/sample.git`
```
```
如果团队很小，把每个人的公钥收集起来放到服务器的`/home/git/.ssh/authorized_keys`文件里就是可行的。如果团队有几百号人，就没法这么玩了，这时，可以用[Gitosis](https://github.com/res0nat0r/gitosis)来管理公钥。
这里我们不介绍怎么玩[Gitosis](https://github.com/res0nat0r/gitosis)了，几百号人的团队基本都在500强了，相信找个高水平的Linux管理员问题不大。
### 管理权限
有很多不但视源代码如生命，而且视员工为窃贼的公司，会在版本控制系统里设置一套完善的权限控制，每个人是否有读写权限会精确到每个分支甚至每个目录下。因为Git是为Linux源代码托管而开发的，所以Git也继承了开源社区的精神，不支持权限控制。不过，因为Git支持钩子（hook），所以，可以在服务器端编写一系列脚本来控制提交等操作，达到权限控制的目的。[Gitolite](https://github.com/sitaramc/gitolite)就是这个工具。
这里我们也不介绍[Gitolite](https://github.com/sitaramc/gitolite)了，不要把有限的生命浪费到权限斗争中。
```
1. 用户home目录755权限  rwx  r-x r-x
 2. .ssh目录700权限  700=rwx  --- --- 
 3. authorized_keys 600权限   rx- --- ---
```
