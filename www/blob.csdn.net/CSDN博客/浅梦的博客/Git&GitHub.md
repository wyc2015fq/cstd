# Git&GitHub - 浅梦的博客 - CSDN博客





2017年08月07日 11:29:22[浅梦s](https://me.csdn.net/u012151283)阅读数：146标签：[Git																[GitHub](https://so.csdn.net/so/search/s.do?q=GitHub&t=blog)](https://so.csdn.net/so/search/s.do?q=Git&t=blog)
个人分类：[工具介绍](https://blog.csdn.net/u012151283/article/category/6341630)








# Git基础

```python
git config --global user.name "myUserName"
git config --global user.email myName@email.com
```

`git init`初始化git仓库 
`git status`查看git仓库状态 
`git add file`添加追踪文件 
`git rm --cached file`取消追踪文件 
`git commit -m "message"`提交 
`git reset --soft HEAD~`撤回最近的一次commit
# Git重要特性

`git help [command]`帮助 
`git log`查看提交日志 
`git diff file`查看未暂存文件相对于上一次提交的修改 
`git checkout file`去除对当前文件所有操作，恢复到上一次提交的状态
# 分支

`git brancg`列出所有分支 
`git branch branch-name`添加新分支 
`git checkout branch-name`切换分支 
`git branch -d branch-name`删除分支 
`git checkout -b branch-name`创建并切换至新分支 
`git merge other-branch`将其他分支合并到当前分支
# Github

`git remote`查看远端仓库 
`git remote add origin url`添加远端仓库 
`git push -u origin master`推送到远端仓库 
`git pull`拉取远端仓库提交







