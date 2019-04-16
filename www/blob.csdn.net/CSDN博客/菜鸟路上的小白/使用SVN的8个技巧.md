# 使用SVN的8个技巧 - 菜鸟路上的小白 - CSDN博客





2016年09月24日 10:38:31[somnus_小凯](https://me.csdn.net/u012486840)阅读数：3344








## 1. 使用SVN钩子强制提交注释


一个好的SVN实践是文件提交时要求必须填写注释，并注明相关修改信息，如bug号、任务描述等，内容按照约定编写。这样在后期的代码审核和回溯过程中会非常方便，可以更快的定位到具体代码的修改记录。


所谓SVN钩子就是一些与版本库事件发生时触发的程序，例如新修订版本的创建，或者是未版本化属性的修改。目前subversion提供了如下几种钩子：`start-commit`、`pre-commit`、`post-commit`、`pre-unlock`、`post-unlock`、`pre-lock`、`post-lock`、`pre-revprop-change`和`post-revprop-change`，其中我们修改`post-commit`脚本即可实现强制提交注释的功能。


如下是一个实现强制提交注释的`post-commit`脚本例子：



```
`REPOS=``"$1"`

`TXN=``"$2"`




```
#
 Make sure that the log message contains some text.
```


`SVNLOOK=``/usr/bin/svnlook`


```
LOGMSG=`$SVNLOOK
 log -t
```
`"$TXN"``"$REPOS"`
```
|
```
`wc``-m``



`if``[``"$LOGMSG"`
```
-lt
 4 ]
```


`then`

`    ``echo``"拜托，写点注释吧！"``1>&2`

`    ``exit``1`

`fi`



`exit``0`
```



## 2. 创建本地仓库


有时候我们在没有网络的情况下无法连接外网上的SVN仓库，或者没有条件搭建SVN服务，这时我们同样可以使用SVN管理我们自己的代码。我们可以使用TortoiseSVN在本地创建代码仓库，并进行代码的版本管理。具体步骤如下：
- 新建一个空文件夹，用于存放本地的代码仓库；
- 在这个空文件夹上点击右键 -> TortoiseSVN -> Create Respository here，创建仓库；
- 在另一个目录Checkout，本地SVN路径格式类似于：file:///C:\Repo

## 3. SVN命令行操作


在Windows系统中TortoiseSVN是进行SVN代码管理的最佳利器，操作也非常简单方便。但是在一些特殊环境下，熟悉SVN命令行操作也是必须的，譬如想在一些自动化脚本程序中使用SVN的功能。 这里总结一些常用命令如下，更多命令请访问后面的参考链接：

#### (1) 从版本库获取信息
- svn help
- svn info $url
- svn list
- svn log
- svn diff

#### (2) 从版本库到本地
- svn [co|checkout] $url $local
- svn export $url $local
- svn [up|update]

#### (3) 从本地到版本库
- svn import $local $url -m "some comments..."
- svn add $file
- svn delete $file
- svn mv $oldfile $newfile
- svn rm $url
- svn [ci|commit]
- svn revert $file 和 svn revert -R $dir

## 4. 更换版本比较工具


在进行SVN提交时需要非常谨慎，每次提交之前应先做SVN更新或与资源库同步，要特别注意SVN关于冲突和错误的提示信息。对每个提交的文件进行多次检查，确认它们是不是你真正想要提交的。


在提交的文件上选择
```
Compare
 with base
```
可以将本地代码和版本库中的代码进行比较，确保修改的内容无误。TortoiseSVN默认使用自带的TortoiseMerge工具进行代码比较，也可以更换其他的代码比较工具：在TortoiseSVN -> Settings -> Diff Viewer选项中找到
```
Configure
 the program used for comparing different revisions of files
```
，选择`External`，然后选择你喜欢的比较工具，如：Beyond
 Compare。

## 5. SVN服务器迁移


这条其实并不算SVN技巧，但是在我们日常工作中确实经常会遇到这样的情况，想将一台SVN服务器上的仓库迁移到另一台服务器。如果SVN服务器是一台Windows服务器，可以直接将SVN仓库目录复制到新服务器上，然后在新服务器上重启SVN服务即可。如果SVN服务器是linux服务器，可以通过下面的命令操作，将一台服务器上的目录拷贝到另一台服务器。



```
`$``scp`
```
-r
 root@x.x.x.x:
```
`/home/svn``/home/svn`


```
$
 svnserve -d -r
```
`/home/svn`
```




在TortoiseSVN客户端，需要更新SVN地址：右键 -> TortoiseSVN -> Relocate...

## 6. svn:ignore


我们在用SVN提交代码时，常常有一些文件未版本化并且也不想提交，所以在提交时根本不想看到这些文件，譬如类似于Visual Studio工程的bin obj目录。


可以使用 
```
svn propset
 svn:ignore
```
 命令来将某个文件或目录添加到忽略列表中。可以在下面的链接中找到一些常见的ignore文件：[Best
 general SVN Ignore Pattern?](http://stackoverflow.com/questions/85353/best-general-svn-ignore-pattern)

## 7. SVN目录结构


Subversion有一个标准的目录结构，如下所示：



```
`[svn://projects/]()`

`|`


```
+-
 trunck
```



```
+-
 branches
```



```
+-
 tags
```
```




其中，trunk为主开发目录，branches为分支开发目录，tags为tag存档目录（不允许修改）。这几个目录具体怎么使用，svn并没有明确规范，一般有两种方式：trunk作为主开发目录或者trunk作为发布目录。

## 8. SVN使用原则
- 代码变更及时提交，避免本地修改后无法修复；
- 提交前确认代码可编译通过，保证新增的文件同时被提交；
- 不要将格式修正和代码修改混合提交。修正格式包括增加缩进、减少空格等，如果把这些和代码修改一起提交，很难从日志信息中发现代码修正记录；
- 每次提交尽量是一个最小粒度的修改，如果一次提交涉及到两个完全不同的功能，那么分两次提交，并在注释中写清楚提交内容；
- 所有代码文件使用UTF-8格式；
- 提交的文件必须是开发者公用的程序文件，不要提交私人测试程序、程序缓存、图片缓存、自动生成的文件等。

## 参考
- [SVN之使用原则](http://blog.csdn.net/hfahe/article/details/5568211)
- [svn强制写日志和可以修改历史日志(svn钩子的应用)](http://www.blogjava.net/jasmine214--love/archive/2010/10/14/335102.html)
- [SVN搭建本地文件版本管理](http://blog.csdn.net/rocket5725/article/details/10087429)
- [SVN
 命令参考](http://riaoo.com/subpages/svn_cmd_reference.html)
- [关于SVN
 目录结构](http://www.cnblogs.com/newstar/archive/2011/01/04/svn.html)



