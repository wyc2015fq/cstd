# 常用git命令 - qq_30006593的博客 - CSDN博客





2017年08月14日 15:19:22[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：101标签：[git](https://so.csdn.net/so/search/s.do?q=git&t=blog)
个人分类：[git](https://blog.csdn.net/qq_30006593/article/category/7099609)








- git init：初始化仓库。
- git add .：添加所有文件。
- git rm --cached xxx：取消跟踪文件。已经被跟踪的文件即使在.gitignore中包含了仍然会被跟踪
- git commit -m "something"：提交本次更改。
- git remote add <远程主机名> <分支名> $url：添加远程仓库。
- git remote -v ：查看远程仓库网址。
- git fetch <远程主机名> <分支名>：下载远程仓库，并创建分支origin master。[这里是其与merge的配合使用](http://www.jianshu.com/p/d265f7763a3a)
- git merge：合并，与fetch配合，git merge origin master。
- git pull <远程主机名> <分支名>：下载远程仓库并与本地合并。相当于fetch加merge
- git push <远程主机名> <分支名>：更新远程仓库。
- git branch <分支名>：新建分支 -a：查看所有分支 -r：查看当前分支 -d <分支名>：删除分支
- git checkout <分支名>：转到分支 -b：新建并转到分支



