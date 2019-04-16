# Git本地仓库操作 - 一个跳popping的quant的博客 - CSDN博客





2019年01月15日 14:29:17[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：592标签：[git																[github](https://so.csdn.net/so/search/s.do?q=github&t=blog)](https://so.csdn.net/so/search/s.do?q=git&t=blog)
个人分类：[Git](https://blog.csdn.net/FrankieHello/article/category/8613179)








## 一、对本地仓库的操作

### 1、本地账户设置

安装完Git之后，windows的Git的目录下会有两个exe：
- git-bash.exe
- git-cmd.exe

顾名思义，git-bash是linux风格的命令行，git-cmd则是windows下的命令行，而windows系统PATH环境变量下的命令，如java、python，只能在git-cmd下运行，而git-bash则会提示不存在对应的命令。下面的例子基于git-bash.exe。

打开git-bash.exe，设置一下本地账户，包括用户名和邮箱。

```bash
git config --global user.name "xxx"
git config --global user.email "xxx@xxx.com"
```

设置完成之后，可以通过下面的命令查看是否配置成功。

```bash
git config --list
```

### 2、创建本地版本库（Repository）

进入一个目录作为Repository，并进行初始化。

```bash
git init
```

初始化之后会在当前的工作区创建一个隐藏文件 .git，它是Git的版本库，会为我们创建第一个分支master，以及一个暂存区（stage）。

具体可以参考 [工作区和暂存区 - 廖雪峰的官方网站](https://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/0013745374151782eb658c5a5ca454eaa451661275886c6000)

### 3、提交文件到版本库

当我们把文件添加到版本库时，需要两步执行，先将文件添加到暂存区，再通过提交将暂存区的文件添加到当前分支。

```bash
git add ./filename
```

添加到暂存区后可以查看一下当前状态：
`git status`
 它会提示你当前在暂存区，还没有被commit的文件。

```bash
$ git status
On branch master

No commits yet

Changes to be committed:
  (use "git rm --cached <file>..." to unstage)

        new file:   a.txt
```

将暂存区文件进行提交，这时查看一下状态，会显示当前暂存区没有需要提交的文件。
`git commit -m "info"`
```
$ git status
On branch master
nothing to commit, working tree clean
```

### 4、删除版本库中的文件

删除文件一般有两种方法：

先在工作区删除文件，再通过git命令将版本库中文件删除。

```bash
rm ./a.txt
git commit -am "del a.txt"
```

 直接通过git命令删除。

```bash
git rm ./a.txt
git commit -m "del a.txt"
```

### 5、版本回退

查看版本号：

```bash
git log
```

 一次commit就是一次版本。

```bash
git reset --hard HEAD^   回退到上一个版本
git reset --hard xxxx    回退到 xxxx  版本
```






