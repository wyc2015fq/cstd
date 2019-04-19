# Git常用命令整理记录 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年08月07日 14:49:24[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：232

![](https://img-blog.csdn.net/20170807144826840)
[原文地址:http://www.jianshu.com/p/94b6add4237c](http://www.jianshu.com/p/94b6add4237c)
帮助Git理解  图片来源:http://www.cnblogs.com/houpeiyong/p/5890748.html
## Git命令：
#### --基础使用--
用户名：git config --global user.name "<Your Name>"
邮箱：git config --global user.email "<email@example.com>"
换行符自动转换：git config --global core.autocrlf false
      param: commit & checkout 
                  true:  -> LF & -> CRLF 
                  false: null & null
                  input: -> LF & null
回车换行符避免混淆：git config --global core.safecrlf true
      true/false:拒绝/允许提交包含混合换行符的文件
      warn:给出警告
创建版本库：git init
克隆：git clone git@github.com:<UserName/ProjectName>.git
关联远程库：git remote add origin git@server-name:path/repo-name.git
提交到远程库：git push -u origin master(第一次推送带参数-u  之后不带)
查看远程库：git remote
从远程库拉取并merge：git pull origin master
从远程库拉取不自动merge：git fetch origin master
减小仓库体积：
      git filter-branch--tree-filter 'rm -f path/to/large/files' --tag-name-filter cat -- --all
      git push origin--tags --force
      git push origin--all --force
#### --版本控制--
查看当前状态：git status
差异比较：git diff
添加到暂存区(stage)：git add
提交到版本库：git commit -m "<Note>" <FileName>(FileName可填)
查看日志：git log （按q退出日志查看）
      每条log只显示一行：git log --pretty=oneline
查看命令历史：git reflog
版本回退：git reset --hard HEAD^ (HEAD^是上个版本，HEAD^^是送上个版本，HEAD~100是往上100个版本)
      git reset --hard <Commit_Id>(版本号,版本号可不全写)(即调整内部HEAD指针)
                    --soft 只修改版本库的内容
                    --mixed (默认值) 修改版本库与暂存区的内容，不修改工作区内容
                    --hard 全部回退
文件检出：git checkout -- <FileName>(eg. git checkout -- ReadMe.txt)
      (reset --hard 回退会覆盖已修改的内容，checkout -- 不会覆盖已修改的内容)  
创建回退版本：git revert HEAD^
      备注：只修改了工作区文件，没add到暂存区，使用git checkout -- <FileName>
                已经add到了暂存区，使用git reset HEAD,然后执行git checkout -- <FileName>
                已经commit到了版本库中，还没推送到远程库时，使用git reset --hard HEAD^
                revert回退版本为，创建一个新的旧版本，并不修改Git版本库历史内容
文件删除：git rm <FileName>
#### --分支管理--
查看分支：git branch
创建分支：git branch <BranchName>
切换分支：git checkout <BranchName>
重置分支：git reset <BranchName>
创建+切换分支：git checkout -b <BranchName>
合并某分支到当前分支：git merge <BranchName>
合并某分支到当前分支(不使用Fast Forward模式）：git merge --no-ff <BranchName>
删除分支(merge后)：git branch -d <BranchName>
删除分支(merge前)：git branch -D <BranchName>
查看分支合并图：git log --graph
      查看特定分支日志：git log <BranchName>
储存分支当前状态：git stash
查看储存列表：git stash list
恢复储存状态：git stash apply
指定恢复某个状态：git stash apply stash@{0}
删除储存内容：git stash drop
恢复储存状态并删除储存内容：git stash pop
#### --标签管理--
查看所有标签：git tag
新建标签：git tag <TagName> <Commit_Id>(Commit_Id(版本号)可不填，默认值为HEAD)
创建带有说明的标签：git tag -a <TagName> -m "<Note>" <Commit_Id> (Commit_Id可不填，默认值为HEAD)
创建带有签名的标签：git tag -s <TagName> -m "<Note>" <Commit_Id> (Commit_Id可不填，默认值为HEAD)(前提是必须有GPG私钥)
查看标签信息：git show <TagName>
推送一个本地标签：git push origin  <TagName>
推送全部未推送过的本地标签：git push origin --tags
删除一个本地标签：git tag -d <TagName>
删除一个远程标签：git push origin :refs/tags/<TagName>
第一次尝试写文章，只是用于本人学习过程中的记录，并方便之后使用时查看。如果能帮助到你那就给个赞吧。
参考：
[码云平台帮助文档](http://git.mydoc.io/)
[廖雪峰的Git教程](https://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000)
[git的reset和checkout的区别](http://www.cnblogs.com/houpeiyong/p/5890748.html)
[](http://www.cnblogs.com/houpeiyong/p/5890748.html)[代码回滚：git reset、git checkout和git revert区别和联系](http://www.cnblogs.com/houpeiyong/p/5890748.html)
作者：Dollkey
链接：http://www.jianshu.com/p/94b6add4237c
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
