# Git 指令 - weixin_33985507的博客 - CSDN博客
2019年03月01日 09:48:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
### log
```
git diff
查看没有被 Add 的改动内容
git diff --cached
查看缓存区的改动内容
git diff --staged
查看缓存区的改动内容
git diff HEAD
效果等同于 git diff 与 git diff --cached 同时执行
git log
查看提交记录
git log -p
查看每个 commit 的改动内容，精确到每一行的改动
git log --stat
查看每个 commit 的大致改动
git show CommitId/FileName
查看某个 commit或文件 的具体改动内容
git log --oneline
将日志输出为一行
git log -[length]
显示 length 条提交记录，可配合 --oneline 使用
git log --skip=[length]
跳过前面 length 条提交记录。例如：git log --skip=1 -2 --oneline，跳过第一条记录，显示接下来的2条记录，并都是作为一行显示
git log --pretty=raw
显示更多的提交记录
git log --graph
会打印出提交的线索。比如分支的切换与合并
git log --name-status
显示每次提交有内容变动的文件列表
git log --grep [keywords]
显示提交内容包含有 keywords 的所有 commit。例如：git log --grep 增加；凡是提交内容有【增加】的 commit 都会被打印出来
git log --author authorname
打印 authorname 对应的开发者的所有 commit
```
