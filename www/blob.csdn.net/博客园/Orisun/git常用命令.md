# git常用命令 - Orisun - 博客园







# [git常用命令](https://www.cnblogs.com/zhangchaoyang/articles/7078357.html)





#在github上通过GUI创建一个repository
https://guides.github.com/activities/hello-world/

#新建一个文件
echo "# my_fisrt_repository" >> README.md
#在当前目录初始化一个git环境
git init
#把文件添加到本地仓库
git add README.md
#从本地仓库中删除文件
git rm README.md
#从本地仓库中删除文件夹
git rm -r directory
#关联一个远程仓库，将其命名为origin
git remote add origin https://github.com/Orisun/my_fisrt_repository.git
#把代码提交到远程仓库之前必须先commit
git commit -m "first commit"
#把本地代码push到origin远程仓库的master分支上（没有指定本定分支时，使用当前所在有本地分支）
git push -u origin master

#在本地创建一个新分支
git branch dev_orisun
#删除本地分支
git brach -d dev_orisun
#切换到本地的新分支
git checkout dev_orisun
#从远程分支上把代码检出到本地的当前分支
git fetch origin master
#把代码从远程分支检出到本地分支，如果本地分支不存在则创建
git checkout -b dev_orisun origin/master
#把本地分支dev_orisun上的代码push到远程分支master上，如果远程分支不存在则创建
git push origin dev_orisun:master
#如果当前所在的本地分支名与远程分支名一致，则push时不用指定本地分支名
git push origin master
#如果本地分支名为空，则表示删除远程分支
git push origin :master
#删除远程分支的另一种方法
git push origin -d master


git fetch origin master	#取下远程分支上的代码（存在本地的一个临时目录里）
git log -p dev_orisun..origin/master	#拿本地dev_orisun分支上的代码与origin/master上的代码进行比较	
git merge origin/master	#把远程origin/master上的代码合并到本地的当前分支上
上面3行命令等价于：
git fetch origin master:tmp
git diff tmp
git merge tmp
merge后可能会有冲突，解决冲突后需要再使用git commit和git push把最新代码push到远程分支上
也可以使用git pull origin master直接把远程origin/master上的代码合并到本地的当前分支上，但是先fetch下来diff一下比较保险，说不定diff之后你不打算merge了呢。

#查看本地分支
git branch
#查看远程分支 
git branch -a

#检查文件状态，即哪些文件被修改了，哪些文件还没有被add
git status

#查看commit日志
git log
#查看某一个次commit的详情
git show <commit_log_id> #commit_log_id是个很长的字符串，只使用前4个字母就可以了。最后的一次commit_log_id也可以用HEAD代替。
#查看某两次commit之间所有的变更
git diff <commit_from>..<commit_to>

#把某个文件回滚到之前的某个版本
git checkout <commit_log_id> a.java

#Git覆盖本地代码 
1. 第一种方法
git fetch --all 
git reset --hard origin/master 
git fetch下载远程最新的， 然后，git reset master分支重置
2. 第二种方法
git reset --hard HEAD 
git pull

#Git回滚到任意版本
git log -3
git reset --hard e377f60e28c8b84158
git push -f origin master












