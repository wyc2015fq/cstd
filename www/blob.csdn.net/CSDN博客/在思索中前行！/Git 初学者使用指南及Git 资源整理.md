# Git 初学者使用指南及Git 资源整理 - 在思索中前行！ - CSDN博客





2014年10月19日 19:28:51[_Tham](https://me.csdn.net/txl16211)阅读数：2047











## Git 资源整理


> 
Git is a free and open source distributed version control system designed to handle everything from small to very large projects with speed and efficiency.


## 官方网站

[http://git-scm.com/](http://git-scm.com/)

## 教程
- [Git 官方教程](http://git-scm.com/docs/gittutorial)
- [Easy Version Control with Git](http://code.tutsplus.com/tutorials/easy-version-control-with-git--net-7449)
- [猴子都能懂 Git 的入门](http://backlogtool.com/git-guide/cn/)
- [A Visual Git Reference](http://marklodato.github.io/visual-git-guide/index-en.html)
- [Altassian's Git Tutorial](https://www.atlassian.com/git/)
- [廖雪峰的Git教程](http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000) &
[Gitbook版](http://lvwzhen.gitbooks.io/git-tutorial/)
- [git - 简易指南](http://www.bootcss.com/p/git-guide/)
- [Think like a Git](http://think-like-a-git.net/)
- [Learn Version Control with Git](http://www.git-tower.com/learn/)
- [专为设计师而写的GitHub快速入门教程](http://www.ui.cn/project.php?id=20957)
- [Learn Git Branching](http://pcottle.github.io/learnGitBranching/)
- [学习git笔记](http://blog.chinaunix.net/uid-22547469-id-4561454.html)
- [A successful Git branching model](http://nvie.com/posts/a-successful-git-branching-model/)
- [实用 GIT 工作流](http://yedingding.com/2013/09/11/practical-git-flow-for-startups.html)
- [Git Tutorial](http://www.tutorialspoint.com/git/)
- [GitHowTo](http://githowto.com/)

## 托管
- [Github](https://github.com/)
- [Bitbucket](https://bitbucket.org/)
- [Gitlab](https://www.gitlab.com/)
- [CodePlex](https://www.codeplex.com/)
- [CloudHost](http://cloudhost.io/)
- [CloudBees](http://www.cloudbees.com/)
- [Assembla](https://www.assembla.com/home)
- [Gitblit](http://gitblit.com/)
- [CodePlan](https://codeplane.com/)
- [Gitorious](http://www.getgitorious.com/)
- [ProjectLocker](http://projectlocker.com/)
- [Unfuddle](https://unfuddle.com/)
- [CSDN Code](https://code.csdn.net/)
- [JD Code](https://code.jd.com/)
- [Taobao Code](http://code.taobao.org/)
- [Gitcafe](https://gitcafe.com/)
- [GitShell](https://gitshell.com/)
- [GitCD](http://www.gitcd.com/index.htm)
- [集盒](https://geakit.com/)
- [OSChina](https://git.oschina.net/)
- [Coding](https://coding.net/)
- [Huihoo Code](http://code.huihoo.com/)
- [SVN China](http://www.svnchina.com/)

## 书籍
- [Git Community Book 中文版](http://gitbook.liuhui998.com/index.html)
- Git Magic
- Git Pocket Guide
- Git Recipes
- Git Succinctly
- Git 使用指南
- Git 魔法
- Git- Version Control for Everyone
- Gitolite Essentials
- Git使用体会
- Git权威指南
- Pragmatic Guide to Git
- Pro Git(cn)
- Pro Git-读书笔记
- [Pro Git](http://git-scm.com/book/en/v2)
- Version Control with Git(2nd）
- 版本控制之道：使用 Git
- 看日记学Git




## Git实验指南



## Git 安装使用简介：





在介绍安装和简单使用前，先看一下百度百科中的简介吧：

————————————————————————————————————————

    Git --- The stupid content tracker, 傻瓜内容跟踪器。

    Linux 是这样给我们介绍 Git 的：

    Git 是用于Linux 内核开发的版本控制工具。与常用的版本控制工具 CVS, Subversion 等不同，它采用了分布式版本库的方式，不必服务器端软件支持，使源代码的发布和交流极其方便。 Git 的速度很快，这对于诸如 Linux kernel 这样的大项目来说自然很重要。 Git 最为出色的是它的合并跟踪（merge tracing）能力。

————————————————————————————————————————

下面我用ubuntu 10.10上的命令为例：（这些命令都是在本地客户端处使用，非服务器操作裸库使用）

（本文中尖括号内包含尖括号都将是描述内容，请在输入实际命令时替换成描述内容所符的内容。）

1、安装：

$ sudo apt-get install git

$ sudo apt-get install gitk#此为安装官方的图形界面，不需要的可以不安装



2、cd到需要管理的代码、文件所在的第一级目录



3、初始化：

$ git init



4、添加当前目录所有内容：

$ git add .



5、查看状态：

$ git status



6、添加commit：

$ git commit -am "first commit."



7、版本对比：

$ git diff



8、查看历史记录：

$ git log



9、分支操作

查看分支：$ git branch

创建分支：$ git branch 分支名称 （注意：请不要在服务端建立分支）

切换分支：$ git checkout 分支名称

删除分支：$ git branch -d 分支名称



10、加入服务器

$ git remote add 用户名@计算机名或IP:~/某个目录



11、推送数据

$ git push master master #本地master推送到远端master

如果想快捷的使用git push就推送到默认远端分支master，可以做个一次性设置：

$ git remote add origin <实际的ssl用户名>@<IP地址>:<Git在远端的path>

    做完以上设置，以后直接使用git push 就会自动推送到上述设置地址了，但如果要推送到其他分支，还是需要加参数的，这个设置只是相当于一个默认参数而已。



12、接收数据

$ git pull origin master

如果想直接使用git pull直接接收，同样需要提前做一个一次性设置（同样也是不能应用多分支pull情况）：

$ git branch --set-upstream master origin/master



13、本地库设置个人姓名和邮件

$ git config --global user.name "你的姓名，最好由没有符合和空格的英文字母组成"

$ git config --global user.email <邮件名>@<邮箱服务商后缀>

如果不设置个人信息，提交的信息将不会有更改者信息，这样会加大项目管理的难度。



14、启动图形界面

$ gitk




## Git操作指南（1） —— Git for Linux裸库的建立

## 在linux上建立裸库的操作，其实是非常的简单，例如：我用jmcx帐号，在家目录下的文件夹jmcx_git_bare中建立一个裸库，操作如下：



























jmcx@mvp:~$ pwd    /home/jmcx

jmcx@mvp:~$ mkdir jmcx_git_bare

jmcx@mvp:~$ ls  jmcx_git_bare

jmcx@mvp:~$ **cd jmcx_git_bare**jmcx@mvp:~/jmcx_git_bare$ **git init --bare**  Initialized empty Git repository in /home/jmcx/jmcx_git_bare/

jmcx@mvp:~/jmcx_git_bare$ ls  branches  config  description  HEAD  hooks  info  objects  refs



这样就搞定了~




## Git操作指南（2） —— Git Gui for Windows的建库、克隆（clone）、上传（push）、下载（pull）、合并





**1、下载并安装**

    下载地址：

[http://code.google.com/p/msysgit/downloads/detail?name=Git-1.7.10-preview20120409.exe](http://code.google.com/p/msysgit/downloads/detail?name=Git-1.7.10-preview20120409.exe)

    安装的话，新手的话，全部“下一步（next）”即可。



**2、建库（init）**

    （如果你需要在本机计算机建库并管理自己的代码，请看此节。）

    首先，新建一个文件夹，进入文件夹后点击右键，选择“**Git Init Here**”：




    执行完这个操作后，会发现此文件夹中，多了一个“**.git**”的隐藏文件夹，说明执行成功。

    然后，将源代码copy到此目录中（也可以直接在源代码处直接init）：





    做完这个操作后，再点击鼠标右键后，点击“**Git Commit Tool**”，填写完commit后，点击“**提交**”即可：




    最后，我们来看一下History，右键点击鼠标选择“**Git History**”：




    Enjoy~！\(^o^)/~







**3、克隆（clone）**

   （如果你从属于某个项目下，需要将远程的库down到本机计算机，请看此节。）

    在需要建立库的目录下点击右键选择：“Git Gui” ：




    在弹出窗口点击“克隆已经版本库”：




**重点来了，之所以将这步称为重点，是因为网上大多数这一步的教程都错误的！**

然后在Source Location中输入完整的待克隆版本库所在地址，在Target Directory中输入或选择本地的目录（请注意此处会自动新建一个目录，不需要提前建立！）

    我以ssh为例，

    以下第一幅图是局域网内部为例的：




    上面是局域网案例的。

    下面是访问外网IP的方式：




    外网访问可以需要注意几点：

    （1）、因为Gui的source location这里其实**不能更换ssh默认端口**，就算加上“:实际端口号”也会返回以下错误信息：

————————————

ssh: connect to host 123.117.67.67 port 22: Bad file number

fatal: The remote end hung up unexpectedly

————————————

    （2）、**不能使用~号**来代替家目录的路径组成部分了，**必须使用git远端库的绝对地址**。




    然后点击“克隆”，会提示输入ssh对应的密码：




    输入密码后，就会自动克隆了，这里可能要**输入3次密码**，请一次次认真输入吧。成功后会提示类似信息：

——————————————————————————

From 192.168.31.130:~/jmcx

 * [new branch]      master     -> origin/master

——————————————————————————

    关闭掉当前窗口，会自动弹出git gui，然后可以在“版本库”下选择“浏览master上的文件”查看已下载文件，也可以直接去刚才的Target Directory中查看相关文件。



**4、上传（push）**

   （如果你从属于某个项目下，已经clone了远程的库，需要将本地代码修改后，上传到远端库，请看此节。）

    前提条件需要满足已经完成上面的“**2、建库**”操作了。修改文件后，在Git Gui下进行“缓存改动”，然后输入描述，点击“提交”，点击“上传”，输入密码后回自动上传。成功应该是：






**5、下载（pull - fetch）**

    右键在git库所在目录下打开Git Gui，在上方找到“远端（remote）”，点开之后选择“从..获取（fetch）”，自动展开后，点击“origin”：




    然后输入密码，点击OK，即可完成操作：




    这样就OK了，不过这样只是下载了，并没有和你本地的代码合并，要合并的话，还需要做一个操作，请看下节。



**6、合并（pull - merge）**

    fetch之后，到Git Gui的“合并（merge）”下点击“本地合并”，一般情况下是默认条件直接点击“合并（merge）”即可：






————————————————————————————————————————————

    到这里教程就告一段落了。









