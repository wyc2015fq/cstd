# Git clone远程分支 - z69183787的专栏 - CSDN博客
2015年05月30日 21:18:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9563
git clone只能clone远程库的master分支，无法clone所有分支，解决办法如下：
1. 找一个干净目录，假设是git_work
2. cd git_work
3. git clone http://myrepo.xxx.com/project/.git ,这样在git_work目录下得到一个project子目录
4. cd project
5. git branch -a，列出所有分支名称如下：
remotes/origin/dev
remotes/origin/release
6. git checkout -b dev origin/dev，作用是checkout远程的dev分支，在本地起名为dev分支，并切换到本地的dev分支
7. git checkout -b release origin/release，作用参见上一步解释
8. git checkout dev，切换回dev分支，并开始开发。
git clone默认会把远程仓库整个给clone下来 
但只会在本地默认创建一个master分支 
如果远程还有其他的分支，此时用git branch -a查看所有分支：
```
* master
```
remotes/origin/HEAD -> origin/master
remotes/origin/master
remotes/origin/python_mail.skin
remotes/origin/udisk
remotes/origin/vip 
能看到远程的所有的分支，如remotes/origin/python_mail.skin可以使用checkout命令来把远程分支取到本地，并自动建立tracking 
$ git checkout -b python_mail.skin origin/python_mail.skin
Branch python_mail.skin set up to track remote branch python_mail.skin from origin.
Switched to a new branch 'python_mail.skin' 
或者使用-t参数，它默认会在本地建立一个和远程分支名字一样的分支
```
$ git checkout -t origin/python_mail.skin
```
也可以使用fetch来做：
```
$ git fetch origin python_mail.skin:python_mail.skin
```
不过通过fetch命令来建立的本地分支不是一个track branch，而且成功后不会自动切换到该分支上
