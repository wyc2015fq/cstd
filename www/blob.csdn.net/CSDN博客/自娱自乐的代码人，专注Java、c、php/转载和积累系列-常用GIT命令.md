# 转载和积累系列 - 常用GIT命令 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年12月02日 10:00:03[initphp](https://me.csdn.net/initphp)阅读数：876







git branch 查看本地所有分支

git status 查看当前状态 

git commit 提交 

git branch -a 查看所有的分支

git branch -r 查看远程所有分支

git commit -am "init" 提交并且加注释 

git remote add origin git@192.168.1.119:ndshow

git push origin master 将文件给推到服务器上 

git remote show origin 显示远程库origin里的资源 

git push origin master:develop

git push origin master:hb-dev 将本地库与服务器上的库进行关联 

git checkout --track origin/dev 切换到远程dev分支

git branch -D master develop 删除本地库develop

git checkout -b dev 建立一个新的本地分支dev

git merge origin/dev 将分支dev与当前分支进行合并

git checkout dev 切换到本地dev分支

git remote show 查看远程库

git add .

git rm 文件名(包括路径) 从git中删除指定文件

git clone git://github.com/schacon/grit.git 从服务器上将代码给拉下来

git config --list 看所有用户

git ls-files 看已经被提交的

git rm [file name] 删除一个文件

git commit -a 提交当前repos的所有的改变

git add [file name] 添加一个文件到git index

git commit -v 当你用－v参数的时候可以看commit的差异

git commit -m "This is the message describing the commit" 添加commit信息

git commit -a -a是代表add，把所有的change加到git index里然后再commit

git commit -a -v 一般提交命令

git log 看你commit的日志

git diff 查看尚未暂存的更新

git rm a.a 移除文件(从暂存区和工作区中删除)

git rm --cached a.a 移除文件(只从暂存区中删除)

git commit -m "remove" 移除文件(从Git中删除)

git rm -f a.a 强行移除修改后文件(从暂存区和工作区中删除)

git diff --cached 或 $ git diff --staged 查看尚未提交的更新

git stash push 将文件给push到一个临时空间中

git stash pop 将文件从临时空间pop下来

---------------------------------------------------------

git remote add origin git@github.com:username/Hello-World.git

git push origin master 将本地项目给提交到服务器中

-----------------------------------------------------------

git pull 本地与服务器端同步

-----------------------------------------------------------------

git push (远程仓库名) (分支名) 将本地分支推送到服务器上去。

git push origin serverfix:awesomebranch

------------------------------------------------------------------

git fetch 相当于是从远程获取最新版本到本地，不会自动merge

git commit -a -m "log_message" (-a是提交所有改动，-m是加入log信息) 本地修改同步至服务器端 ：

git branch branch_0.1 master 从主分支master创建branch_0.1分支

git branch -m branch_0.1 branch_1.0 将branch_0.1重命名为branch_1.0

git checkout branch_1.0/master 切换到branch_1.0/master分支









设置git的user name和email：
$ git config --global user.name "xuhaiyan"$ git config --global user.email "haiyan.xu.vip@gmail.com"

<div id="sina_keyword_ad_area2" class="articalContent  "><strong>1.创建本地分支</strong><div>git branch 分支名，例如：git branch 2.0.1.20120806</div><div>注：<span style="line-height: 21px;">2.0.1.20120806是分支名称，可以随便定义。</span></div><div>
</div><div><strong>2.切换本地分支</strong></div><div>git checkout 分支名，例如从master切换到分支：git checkout2.0.1.20120806</div><div>
</div><div><strong>3.远程分支就是本地分支push到服务器上。比如master就是一个最典型的远程分支（默认）。</strong></div><div>git push origin <wbr><span style="line-height: 21px;">2.0.1.20120806</span></wbr></div><div><span style="line-height: 21px;">
</span></div><div><div><span style="line-height: 21px;"><strong>4.远程分支和本地分支需要区分好，所以，在从服务器上拉取特定分支的时候，需要指定远程分支的名字。</strong></span></div><div><span style="line-height: 21px;">git checkout --trackorigin/</span><span style="line-height: 21px;">2.0.1.20120806</span></div><div>注意该命令由于带有--track参数，所以要求git1.6.4以上！<span style="line-height: 21px;">这样git会自动切换到分支。</span></div><div><span style="line-height: 21px;">
</span></div><div><div><span style="line-height: 21px;"><strong>5.提交分支数据到远程服务器</strong></span></div><div><span style="line-height: 21px;">git push origin<local_branch_name>:<remote_branch_name></span></div><div>例如：</div><div><span style="line-height: 21px;">git pushorigin <wbr></wbr></span><span style="line-height: 21px;">2.0.1.20120806</span><span style="line-height: 21px;">:</span><span style="line-height: 21px;">2.0.1.20120806</span></div><div>一般当前如果不在该分支时，使用这种方式提交。如果当前在 <wbr><span style="line-height: 21px;">2.0.1.20120806</span> <wbr>分支下，也可以直接提交</wbr></wbr></div><div><span style="line-height: 21px;">git push</span></div><div><span style="line-height: 21px;">
</span></div><div><strong>6.删除远程分支</strong></div><div><span style="line-height: 21px;">git push origin:develop</span></div></div></div></div>



二、生成SSH密钥过程：
1.查看是否已经有了ssh密钥：cd ~/.ssh
如果没有密钥则不会有此文件夹，有则备份删除
2.生存密钥：
> 

$ ssh-keygen -t rsa -C “haiyan.xu.vip@gmail.com”

按3个回车，密码为空。



Your identification has been saved in /home/tekkub/.ssh/id_rsa.

Your public key has been saved in /home/tekkub/.ssh/id_rsa.pub.

The key fingerprint is:

………………






最后得到了两个文件：id_rsa和id_rsa.pub


![](https://img-blog.csdn.net/20131218103529140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




