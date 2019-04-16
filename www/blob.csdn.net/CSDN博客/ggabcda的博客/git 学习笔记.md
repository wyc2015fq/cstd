# git 学习笔记 - ggabcda的博客 - CSDN博客





2017年09月07日 14:29:59[ggabcda](https://me.csdn.net/ggabcda)阅读数：124








# git基础

### 与其他版本控制工具的差异

> - svn和与其相似的cvs等版本控制工具一样，保存的可以看作是不同版本与初始版本随时间积累的差异（存储每个文件与初始版本的差异）

- 而git保存的是对每一个版本文件的快照的引索，文件每次更新git都会对文件进行一次快照，并保存快照引索，如果没有更改，则保存的是只保存一个储存链接指向文件（存储项目随时间改变的快照.）



### git的三个工作区

> - 工作目录（对应已修改状态：表示修改了文件，但还没保存到数据库中）

- 暂存区域（对应已暂存状态：表示对一个已修改文件的当前版本做了标记，使之包含在下次提交的快照中）

- git仓库（对应已提交状态：表示数据已经安全的保存在本地数据库中） 


### git安装后配置信息

```
//全局配置用户名和邮箱，配置以后，所有的操作都会是用该信息
    git config --global user.name 'TJEndless' //seting user.name
    git config --golbal user.email 13445@139.com //seting user.email
    /**
    *可以使用一下命令来查看配置星系
    */
    git config --list//view global message
    git congig --user.name //view user.name
```

# git的命令及使用

### 获取帮助

```
//获取config命令手册
    git help config//可离线获取git的操作命令
```

### git命令

##### 获取git仓库

> 
第一种方法是在现有的项目货目录下导入所有文件到git中去

第二种是从服务器克隆一个现有的git仓库

> 
在现有的目录中初始化仓库



```
git init //初始化一个仓库       
    //如果是在一个已经有文件的文件夹中，那么就需要对这行文件进行跟踪（git add，然后gitcommit进行提交）        
    git add *.c //      
    git add LICENSE //      
    git commit -m 'this is a project describe!'     
    //从远程服务器拉去克隆一个仓库
    git clone url
```

> 
> 
查看文件的当前状态



```
git status //输出详细的信息     
    git status -s || git status --short//输出较为简略的信息
```

##### 忽略文件

创建一个名为.gitignore的文件，在文件中列出需要忽略的文件或文件夹（可以使用简化的正则来匹配）

```
//其他的一些git命令
    git rm fileName || 正则 //移除文件
    git rm -f fileName  //强制删除
    git mv newFileName oldFileName
    git log //查看提交历史
    git log -p //查看每次提交内容的差异
    git log -2 //查看最近两次提交的内容
    git log --stat //显示每次更新的文件修改统计信息
    git log --shortstat//只显示 --stat 中最后的行数修改添加移除统计。
    git log --name-only//仅在提交信息后显示已修改的文件清单。
    git log --name-status//显示新增、修改、删除的文件清单。
    git log --abbrev-commit//仅显示 SHA-1 的前几个字符，而非所有的 40 个字符。
    git log --relative-date//使用较短的相对时间显示（比如，“2 weeks ago”）。
    git log --graph//显示 ASCII 图形表示的分支合并历史。
    git log --pretty//使用其他格式显示历史提交信息。可用的选项包括 oneline，short，full，fuller 和 format（后跟指定格式）。
    git log --since=2.weeks//查看最近两周的，也可以使用具体时间比；如说具体的某一天 "2008-01-15"，或者是相对地多久以前 "2 years 1 day 3 minutes ago"
    //同时还可以按条件如作者 --author等
    git log -sproce//可以列出那些添加或移除了某些字符串的提交。 比如说，你想找出添加或移除了某一个特定函数的引用的提交，你可以这样使用
```

### 撤销操作

##### 撤销操作

```
//如果提交后发现邮寄个文件没有提交或提交是信息填写错误，可执行以下命令再次提交
    git commit --amend //这个命令会将暂存区中的文件提交。 如果自上次提交以来你还未做任何修改（例如，在上次提交后马上执行了此命令），那么快照会保持不变，而你所修改的只是提交信息。
```

##### 取消暂存区的文件

```
//使用一下命令可以取消你误操作用 git add . 提交到暂存区的文件爱你
    git reset HEAD <fileName>
```

##### 撤销对文件的修改`（测试未成功，待验证)`
`    //使用git checkout --<fileName> 来将文件撤销都修改钱的样子  `
### 远程仓库的使用

##### 克隆远程仓库

```
git clone url//将远程仓库克隆到本地
    git remote//查看远程仓库名称的简写
    git remote -v//会显示需要读写远程仓库使用的 Git 保存的简写与其对应的 URL
```

##### 关联远程仓库（添加远程仓库）

```
git remote add <shortname> <url>//shortname:怎个url的短名称可已用他来代替URL
    git fetch [remote-name]//从远程仓库拉去remote-name数据
    //如果使用git clone简写将自动命名成origin的简写
```

> 
特别注意：`合并（pull）两个不同项目，出现问题的解决方法`

当你在本地有一个git仓库，然后又在远程建立了一个git仓库，想要将两个仓库合并，先添加远程仓库（关联远程仓库），git remote add 


##### 查看远程仓库信息
`    git remote show <shortName>//查看远程仓库的一些信息`
#### 远程仓库的移除或改名

```
git remote rename <oldname> <newname>//将远程仓库改名
    git remote rm name //移除远程仓库
```

### git打标签

> 
打标签的目的是为了标记某一次重要的提交，通常用于标记版本的发布（v1.0等等）。


```
git tag//展示当前已有的标签
    git tag -l '1.3.5' //查找含有''1.3.5'的标签
```

> 
Git 使用两种主要类型的标签：轻量标签（lightweight）与附注标签（annotated）(两种的区别可百度)


```
git tag -a v1.1 -m"my version 1.4"//常见一个附注标签，其中-m代表的是附注信息
    git tag v1.1 //创建轻标签
    //同时也可以对过去的提交打标签
    git tag -a tagNum log信息 
    
    //标签并不会和分支一起被提交到远程服务器上去，需要收的的提交
    git push origin v1.5//提交v1.5标签到远程服务器
    git push origin --tags//将所有的
```

### git分支

##### 新建分支

```
//创建一个分支
    git branch branchName  
    //git的分支会有一个HEAD，是一个特殊的指针（可以理解为当前分支的一个别名），能知道当前处于那个分支上，HEAD指向的就是当前分支
    git log --decorate//可以用该命令来查看各个分支当前指的对象
    git log --oneline//查看之前的提交操作
    
    git checktout test //分支切换，切换分支后当前就处于你切换的分支上，HEAD也指向test上
    /**
    *现在若在切换到master分支，那么这条命令将会做两件事， 一是使 HEAD 指回 master 分支，二是将工作目录恢复成 master 分支所指向的快照内容。 也就是说，你现在做修改的话，项目将始于一个较旧的版本。 本质上来讲，这就是忽略 testing 分支所做的修改，以便于向另一个方向进行开发。
    */
    //可使用以下这些命令来查看分叉历史
    git log --oneline
    git log --decorate
    git log --graph
    git log --all
```

##### 分支的合并

```
//新建并切换到该分支
    git checkout -b branchName
    //合并分支，如在master分支上合并test分支
    git merge test//如果master分支是test的父及，则合并只是将master向前推进到test分支所在的状态，合并完后可删除test分支`git branch -d test`。合并是遇到冲突时，使用`git status`查看冲突位置，然后可手动解决
```

具体git分支合并详情，可点击 这里查看

##### 查看分支

```
查看分支
    git branch //查看当前所有分支
    git branch -v //看的查看每一个分支的最后一次提交
    git branch --merge //项可以过滤这个列表中已经合并到当前分支的分支
    git branch --no-merge //项可以过滤这个列表中已经尚未合并到当前分支的分支
```

##### 远程分支

```
//如果有多个远程仓库与远程分支，可以运行 git remote add 命令添加一个新的远程仓库引用到当前的项目
    //但是新增一个远程链接remote会在push到远程仓库是是需要push两次，两者分开朴实，这样如果有多个远程仓库的话就会很麻烦，那么如何合并两个remote
    //首先增加第一个地址
    git remote add origin url1
    //然后增加第二个地址
    git remote set-url --add origin rul2
```

合并两个remote的有url详情可参考这里

##### 推送

> 
当你想要公开分享一个分支时，需要将其推送到有写入权限的远程仓库上


```
//可以直接使用
    git push origin master
    //亦可以使用
    git push origin master:master //及将本地master分支上的内容push到远程master分支上或推送本地的 serverfix 分支，将其作为远程仓库的 serverfix 分支
    //也可以将本地的master分支push到远程的其他分支上
    git push origin master:otherbranch
```

##### 删除远程分支

```
// 如果想要从服务器上删除 serverfix 分支，运行下面的命令
    git push origin --delete serverfix
    //基本上这个命令做的只是从服务器上移除这个指针。 Git 服务器通常会保留数据一段时间直到垃圾回收运行，所以如果不小心删除掉了，通常是很容易恢复的
```

### git分支--变基

> 
在git中整合来自不同分支的修改主要有两种方法：`merge`以及`rebase`


> 
具体内容可参考这里




