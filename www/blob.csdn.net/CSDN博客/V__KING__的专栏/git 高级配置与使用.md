# git 高级配置与使用 - V__KING__的专栏 - CSDN博客





2016年12月21日 18:55:55[v__king__](https://me.csdn.net/V__KING__)阅读数：909








# git 高级配置与使用
- log配置成lg

```
git log --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr)%Creset' --abbrev-commit --date=relative

配置成下：

git config --global alias.lg "log --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr)%Creset' --abbrev-commit --date=relative
```
- 配置成有颜色的,例如git status

```
git config --global --color.ui true
```
- git st === git status

```
git config --global alias.st status
```
- git difftool 修改

```
//配置其实和.gitconfig是一样的
git config --global core.editor vim
git config --global difftool.prompt false
git config --global diff.tool bc3

//修改文件.gitconfig
//下载地址：http://www.scootersoftware.com/bcompare-3.3.13.18981.tar.gz
[user]
	name = vk
	email = liu.bao.long.vk@gmail.com
[core]
	editor = vim
	filemode = false
[diff]
	tool = bc3
[difftool]
	prompt = false
[difftool "bc3"]
	path = /usr/bin/bcompare

[merge]
	tool = bc3
[mergetool "bc3"]
	path = /usr/bin/bcompare
[alias]
	lg = log --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr)%Creset' --abbrev-commit --date=relative
	st = status
	branch = b
```
- git diff 与 git difftool都可以查看两个commit之间的diff

```
git diff adfd35 9394esd6

git difftool adfd35 9394esd6
```

git diff：       查看working Dir与index(你git add后会进入到index)的差别的。

git diff --cached：查看index与local repository的差别的。

git diff HEAD：  查看working DIR和local repository的差别的。

git diff origin/master master

查看local repository主分支和remoterepository主分支的差别。
## GIT删除远程分支

删除远程分支和tag

在Git v1.7.0 之后，可以使用这种语法删除远程分支：

```
$ git push origin --delete <branchName>
```

删除tag这么用：

```
git push origin --delete tag <tagname>
```

否则，可以使用这种语法，推送一个空分支到远程分支，其实就相当于删除远程分支：

```
git push origin :<branchName>
```

这是删除tag的方法，推送一个空tag到远程tag：

```
git tag -d <tagname>
git push origin :refs/tags/<tagname>
```

两种语法作用完全相同。

# git diff提示filemode变化：

diff --git a/Android.mk b/Android.mk

old mode 100644

new mode 100755

原来是filemode的变化，文件chmod后其文件某些位是改变了的，如果严格的比较原文件和chmod后的文件，两者是有区别的，但是源代码通常只关心文本内容，因此chmod产生的变化应该忽略，所以设置一下：
切到源码的根目录下，

```
git config --add core.filemode false
```





