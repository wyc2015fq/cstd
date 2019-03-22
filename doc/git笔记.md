# git笔记



## git安装

我用的电脑是windons系统，请根据你们自己的电脑紫铜安装对应的git版本，去Git官网上下载安装即可，在这里将不给大家做详细的说明，安装步骤请自行百度。

## ssh -key配置

在这一步我已经默认你安装好了Git，打开Git，在Git命令行输入cd  ~/.ssh检查电脑本机是否有ssh key设置，**注意:** .ssh是隐藏文件，一般在电脑C/用户/Administrator下面就能够找到。

一般情况下 ，之前没有用过git的同学电脑本机一般不会 有ssh key 的，下面我讲给大家介绍如何配置ssh key

### 1、进入~路径下，必须保证当前路径在~路径下

在git命令行敲击 ssh-keygen -t  rsa -C "xxx.@yyy.zzz" //建议写自己真实有效的邮箱地址。**注意：**在敲代码是不要将双引号也敲击进去。

然后命令行会出现如下代码：

Enter file in which to save the key (/c/Users/xxxx_000/.ssh/id_rsa):   //此时我们什么都不需要操作，直接回车就好

Enter passphrase (empty for no passphrase):            //此时要你输入码（可以为空，直接回车就好，也可以输入你的密码，这个密码在你最后把本地资源推送到github上面的时候回会让你填写密码，此时密码隐藏，你输入进去是看不到的）

Enter same passphrase again: //再次确认密码（如果你第一次有输入密码，这次就再输一次，如果没有直接回车就行了）

Your identification has been saved in /c/Users/xxxx_000/.ssh/id_rsa. //生成的密钥

Your public key has been saved in /c/Users/xxxx_000/.ssh/id_rsa.pub. //生成的公钥

The key fingerprint is:

e3:51:33:xx:xx:xx:xx:xxx:61:28:83:e2:81 xxxxxx@yy.com

* 本机已完成ssh key设置，其存放路径为：c:/Users/xxxx_000/.ssh/下。其中xxxx_000为你的用户名。

### 2、添加ssh key 到Github上

首先登陆Github,点击右上角的“▼”→Settings→SSH kyes→Add SSH key。
然后在打开c:/Users/xxxx_000/.ssh里面的id_rsa.pub文件，全选复制公钥内容
也可以在git bush中的命令行输入cat ~/.ssh/id_rsa.pub，将得到公钥

Title自定义，将公钥粘贴到GitHub中Add an SSH key的key输入框，最后“Add Key“

### 3、配置账户

```shell
git config --global user.name “your_username” #设置用户名
git config --global user.email “your_registered_github_Email” #设置邮箱地址(建议用注册giuhub的邮箱)
```

### 4、测试ssh keys是否设置成功。

ssh -T git@github.com

The authenticity of host 'github.com (192.30.252.129)' can't be established.

RSA key fingerprint is 16:27:xx:xx:xx:xx:xx:4d:eb:df:a6:48.

Are you sure you want to continue connecting (yes/no)? yes #确认你是否继续联系，输入yes

Warning: Permanently added 'github.com,192.30.252.129' (RSA) to the list of known hosts.

Enter passphrase for key '/c/Users/xxxx_000/.ssh/id_rsa': #生成ssh kye是密码为空则无此项，若设置有密码则有此项且，输入生成ssh key时设置的密码即可。

Hi xxx! You've successfully authenticated, but GitHub does not provide shell access. #出现此句话，说明设置成功。

到这里，git相关的所有配置已经完成，下面我将给大家介绍最常使用的命令

## 常用命令

### 在GitHub创建仓库（公开）

点击github右上角的+里面的new repository
在repository name栏中输入仓库的名称
description 栏中可以设置仓库的说明
piblic（公开）通用 private（非公开）收费
勾选Initialize this repository with a README ，如果想向GitHub添加手中已有的Git仓库，建议不要勾选
Add.gitignore 下拉框中选择要使用的框架
Add a license选择添加许可协议文件
点击create repository按钮，完成仓库的创建

### 用git创建仓库

```shell
mkdir nnn #仓库名
cd hhh
git init #初始化仓库
git status #查看仓库状态
touch README.md #创建READEME.md文件
git add ERADME.md #添加ERADME.md至暂存区
git commit -m "hhh" #如果想要提交信息记录的更详细，请不要加 -m
git diff #可以查看工作树，暂存区，最新提交之间的差别
git diff HEAD #查看工作树与最新提交的差别
```

### clone已有仓库

```shell
git clone git@github.com:XXX/yyyy.git #XXX为github的用户名，yyy为仓库名
```

### 在对应的文件夹中添加新有项

```shell
git status
```

### 提交

```shell
git add mmm.sss #mmm为文件名称，sss为文件拓展名（常用git add .）
git add -A  #提交所有变化
git add -u  #提交被修改(modified)和被删除(deleted)文件，不包括新文件(new)
git add .  #提交新文件(new)和被修改(modified)文件，不包括被删除(deleted)文件
git commit -m "hhh" #hhh为git commit 提交信息，是对这个提交的概述
git log  #用于查看提交日志
git push #更新GitHub上的仓库 = git fetch + git merge
```

### 日志

```shell
git log # 列出所有历史记录，最近的排在最上方，显示提交对象的哈希值，作者、提交日期、和提交说明，按Page Up、Page Down、↓、↑来控制显示，按q退出历史记录列表
git log --pretty=short #加--pretty=short 只显示提交信息的第一行
git log ggg #ggg是指指定的文件或目录，用于查看指定的目录、文件的日志
git log -p #查看提交所带来的改动
git log -p ggg #查看指定文件的改动
git log --graph #以图表形式查看分支
```

### 分支操作

```shell
git branch #显示分支一览表，同时确认当前所在的分支
git checkout -b aaa #创建名为aaa的分支，并且切换到aaa分支
git branch aaa #创建名为aaa的分支
git checkout aaa # 切换到aaa分支
#能和git branch -b aaa 得到同样的效果
git checkout - #切换到上一分支
```

### 合并分支

```shell
git checkout master //切换到master分支
git merge --no--ff aaa // 加--no--ff 参数可以在历史记录中明确地记录本次分支的合并
```

### 更改提交的操作

```shell
git reset //回溯历史版本
git reset --hrad //回溯到指定状态，只要提供目标时间点的哈希值
```

### 推进历史

```shell
git reflog #查看仓库的操作日志，找到要推历史的哈希值
git checkout master
git reset --hrad ddd #ddd为要推进历史的哈希值
```

### 修改提交信息 git commit --amend

压缩历史 git rebase -i 错字漏字等失误称作typo

根据以前的步骤在GitHub上创建仓库，应于本地的仓库名相同 GitHub上面创建的仓库的路径为git@github.com: 用户名/仓库名.git

```shell
git remote add eee git@github.com: 用户名/仓库名.git #添加远程仓库，并将git@github.com: 用户名/仓库名.git远程仓库的名称改为eee

git push -u eee master #推送至远程仓库 master分支下 -u 参数可以在推送的同时，将eee仓库的master分支设置为本地仓库的当前分支的的upstream（上游）。添加这个参数，将来运行git pull命令从远程仓库获取内容时，本地仓库的这个分支就可以直接从eee的master分支中获取内容

git checkout -b feature d eee/feature d #获取远程的feature d分支到本地仓库，-b参数后面是本地仓库中新建的仓库的名称

git pull eee feature d #将本地的feature d分支更新为最新状态
```


在GitHub上面查看两个分支之间的差别，只需要在地址栏中输入http://github.com/用户名/仓库名/分支1...分支2

### 查看master分支在最近七天内的差别

http://github.com/用户名/仓库名/master@{7.day.ago}...master （同样，day，week，month，year都是可以哒）

### 查看与指定日期之间的差别

http://github.com/用户名/仓库名/master@{xxxx-xx-xx}...master （xxxx-xx-xx代表年月日）

到这里，git相关的命令已经全部介绍完成，剩下的就靠大家自己学习了。





## Svn与Git的区别

这篇主要是谈谈两者的区别，至于谁优谁劣看官自己思考吧！

把第一条理解到位思想到位了做起来才会有的放矢，其他几条都是用的时候才能体会到

   1) 最核心的区别Git是分布式的，而Svn不是分布的。能理解这点，上手会很容易，声明一点Git并不是目前唯一的分布式版本控制系统，还有比如Mercurial等，所以说它们差不许多。话说回来Git跟Svn一样有自己的集中式版本库和Server端，但Git更倾向于分布式开发，因为每一个开发人员的电脑上都有一个Local Repository,所以即使没有网络也一样可以Commit，查看历史版本记录，创建项 目分支等操作，等网络再次连接上Push到Server端。

从上面看GIt真的很棒，但是GIt adds Complexity,刚开始使用会有些疑惑，因为需要建两个Repositories(Local Repositories & Remote Repositories),指令很多，除此之外你需要知道哪些指令在Local Repository，哪些指令在Remote Repository。

   2)Git把内容按元数据方式存储，而SVN是按文件：因为,.git目录是处于你的机器上的一个克隆版的版本库，它拥有中心版本库上所有的东西，例如标签，分支，版本记录等。.git目录的体积大小跟.svn比较，你会发现它们差距很大。

   3) Git没有一个全局版本号，而SVN有：目前为止这是跟SVN相比Git缺少的最大的一个特征。

   4) Git的内容的完整性要优于SVN: GIT的内容存储使用的是SHA-1哈希算法。这能确保代码内容的完整性，确保在遇到磁盘故障和网络问题时降低对版本库的破坏。

   5) Git下载下来后，在OffLine状态下可以看到所有的Log,SVN不可以。

   6) 刚开始用时很狗血的一点，SVN必须先Update才能Commit,忘记了合并时就会出现一些错误，git还是比较少的出现这种情况。

   7) 克隆一份全新的目录以同样拥有五个分支来说，SVN是同时复製5个版本的文件,也就是说重复五次同样的动作。而Git只是获取文件的每个版本的 元素，然后只载入主要的分支(master)在我的经验,克隆一个拥有将近一万个提交(commit),五个分支,每个分支有大约1500个文件的 SVN,耗了将近一个小时！而Git只用了区区的1分钟！

   8) 版本库（repository):SVN只能有一个指定中央版本库。当这个中央版本库有问题时，所有工作成员都一起瘫痪直到版本库维修完毕或者新的版本库设立完成。而 Git可以有无限个版本库。或者，更正确的说法，每一个Git都是一个版本库，区别是它们是否拥有活跃目录（Git Working Tree）。如果主要版本库（例如：置於GitHub的版本库）发生了什麼事，工作成员仍然可以在自己的本地版本库（local repository）提交，等待主要版本库恢复即可。工作成员也可以提交到其他的版本库！

   9)分支（Branch）在SVN，分支是一个完整的目录。且这个目录拥有完整的实际文件。如果工作成员想要开啟新的分支，那将会影响“全世界”！每个人都会拥有和你一样的分支。如果你的分支是用来进行破坏工作（安检测试），那将会像传染病一样,你改一个分支，还得让其他人重新切分支重新下载，十分狗血。而 Git，每个工作成员可以任意在自己的本地版本库开啟无限个分支。举例：当我想尝试破坏自己的程序（安检测试），并且想保留这些被修改的文件供日后使用， 我可以开一个分支，做我喜欢的事。完全不需担心妨碍其他工作成员。只要我不合并及提交到主要版本库，没有一个工作成员会被影响。等到我不需要这个分支时， 我只要把它从我的本地版本库删除即可。无痛无痒。

​    Git的分支名是可以使用不同名字的。例如：我的本地分支名为OK，而在主要版本库的名字其实是master。

​    最值得一提，我可以在Git的任意一个提交点（commit point）开启分支！（其中一个方法是使用gitk –all 可观察整个提交记录，然后在任意点开啟分支。）

​    10)提交（Commit）在SVN，当你提交你的完成品时，它将直接记录到中央版本库。当你发现你的完成品存在严重问题时，你已经无法阻止事情的发生了。如果网路中断，你根本没办法提交！而Git的提交完全属於本地版本库的活动。而你只需“推”（git push）到主要版本库即可。Git的“推”其实是在执行“同步”（Sync）。

最后总结一下：

SVN的特点是简单，只是需要一个放代码的地方时用是OK的。

Git的特点版本控制可以不依赖网络做任何事情，对分支和合并有更好的支持(当然这是开发者最关心的地方)，不过想各位能更好使用它，需要花点时间尝试下。

参考:

[    git和SVN的区别 - 生命在于折腾 - 博客频道 - CSDN.NET](http://blog.csdn.net/bruce_6/article/details/38299677) 

​    [svn - Why is Git better than Subversion? - Stack Overflow](http://stackoverflow.com/questions/871/why-is-git-better-than-subversion) 

## 常见错误

### Please move or remove them before you merge

这是因为本地有修改,与云端别人提交的修改冲突,又没有merge.

```shell
git clean  -d  -fx ""  # 确定使用云端的代码,删除本地修改
d  # 删除未被添加到git的路径中的文件
f  # 强制运行
x  # 删除忽略文件已经对git来说不识别的文件
# 注意:该命令会删除本地的修改,最好先备份再使用

git clean -n  # 显示将要删除的文件和目录
git clean -df # 删除文件和目录
git clean -f  # 删除文件
```

