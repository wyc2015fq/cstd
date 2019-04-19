# git push代码到远程新分支 - 数据之美的博客 - CSDN博客
2019年04月11日 19:22:45[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：10
Git push
获取远程代码修改后,想要push到远端与原来不同的新分支，可以使用下面的命令实现：
git push origin 本地分支:远端希望创建的分支
例如git下来的分支为master
git branch
>>> *master
git push origin master:my_remote_new_branch
#远端即可创建新的分支my_remote_new_branch,提交本地修改
