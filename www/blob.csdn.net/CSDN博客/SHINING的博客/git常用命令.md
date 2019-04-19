# git常用命令 - SHINING的博客 - CSDN博客
2013年02月25日 17:04:08[snsn1984](https://me.csdn.net/snsn1984)阅读数：3555
                git branch
显示当前所在的分支，以及目前本地所有分支
前面有*标注的是当前所在分支
git branch XXX
创建新的分支
git checkout -b XXX
创建新的分支并且切换到新分支上
git checkout XXX
切换到新的分支上（此命令也可以用于切换到本地库没有，而远程库有的分支上）
git status
检查当前提交空间的状态
git add 
将修改加入到提交空间中
git commit -m "XXXX"
提交修改，并且加上修改描述信息
git push origin XXXX
将本地分支XXXX的修改推送到远端分支XXXX上
一般提交代码的顺序：
第一次新建分支：
git checkout -b XXXX
修改代码
git status
git add 
git status(检查是否添加成功)
git commit -m "xxxxxxxx"
git push origin XXXX
不是第一次新建分支：
git checkout  XXXX
git pull
修改代码
git status
git add 
git status(检查是否添加成功)
git commit -m "xxxxxxxx"
git push origin XXXX            
