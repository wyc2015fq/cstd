# 转git的使用 - _天枢 - 博客园
## [转git的使用](https://www.cnblogs.com/yhleng/p/7640016.html)
2017-10-09 10:44 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7640016)
## [git的使用（包括创建远程仓库到上传代码到git的详细步骤以及git的一些常用命令）](http://www.cnblogs.com/mantou811/p/6688517.html)
**A创建远程仓库到上传代码到git**
**1**）登陆或这注册**git**账号
[https://github.com](https://github.com/)
**2**）创建远程仓库
![](https://images2015.cnblogs.com/blog/977960/201704/977960-20170410125140329-772544795.tiff)
![](https://images2015.cnblogs.com/blog/977960/201704/977960-20170410125153188-979237197.png)
**3**）打开终端输入命令(前提，本地已经安装了Git Bash；这个安装window版git时会自动安装)
cd到你的本地项目根目录下，执行如下git命令
git init   初始化，本地目录为git仓库
git add . 增加仓库内容为要上传（注意git add . 后边带个点，这是将当前仓库所有文件都增加；也可以git add filename 单个增加文件）
git commit -m "注释语句"     这个提交要拉加的内容
git remote add origin [https://自己的仓库url地址](https://xn--url-tq9dm69a0cp48cchb813kx2xa/)    关联本地和远程仓库  git remote add "别名" https://github.com/xxxxx/Demo.git  ;之后操作可以直接写这个别名
git pull origin master   从远程仓库上拉取项目到本地
git push -u origin master 推送本地仓库到远远程
**如果如下报错**
failed to push some refs to 'https://github.com/mantou811/demo.git'
hint: Updates were rejected because the remote contains work that you do
hint: not have locally. This is usually caused by another repository pushing
hint: to the same ref. You may want to first integrate the remote changes
hint: (e.g., 'git pull ...') before pushing again.
hint: See the 'Note about fast-forwards' in 'git push --help' for details.
解决方法有
**1.**使用强制**push**的方法：（首次上传代码可用这个方法）
$ git push -u origin master -f
这样会使远程修改丢失，一般是不可取的，尤其是多人协作开发的时候。
**2.push**前先将远程**repository**修改**pull**下来
$ git pull origin master
$ git push -u origin master
**3.**若不想**merge**远程和本地修改，可以先创建新的分支：
$ git branch [name]
然后push
$ git push -u origin [name]
**B，Github常用的命令：**
- git branch 查看本地所有分支
- git status 查看当前状态
- git commit 提交
- git branch -a 查看所有的分支
- git branch -r 查看远程所有分支
- git commit -am "init" 提交并且加注释
- git remote add origin git@192.168.1.119:ndshowgit
- push origin master 将文件给推到服务器上
- git remote show origin 显示远程库origin里的资源
- git push origin master:develop
- git push origin master:hb-dev 将本地库与服务器上的库进行关联
- git checkout --track origin/dev 切换到远程dev分支
- git branch -D master develop 删除本地库develop
- git checkout -b dev 建立一个新的本地分支dev
- git merge origin/dev 将分支dev与当前分支进行合并
- git checkout dev 切换到本地dev分支
- git remote show 查看远程库
- git add .git rm 文件名(包括路径) 从git中删除指定文件* git clone git://github.com/schacon/grit.git 从服务器上将代码给拉下来
- git config --list 看所有用户
- git ls-files 看已经被提交的
- git rm [file name] 删除一个文件
- git commit -a 提交当前repos的所有的改变
- git add [file name] 添加一个文件到
- git indexgit commit -v 当你用－v参数的时候可以看commit的差异
- git commit -m "This is the message describing the commit" 添加commit信息
- git commit -a -a是代表add，把所有的change加到git index里然后再commit
- git commit -a -v 一般提交命令
- git log 看你commit的日志
- git diff 查看尚未暂存的更新
- git rm a.a 移除文件(从暂存区和工作区中删除)
- git rm --cached a.a 移除文件(只从暂存区中删除)
- git commit -m "remove" 移除文件(从Git中删除)
- git rm -f a.a 强行移除修改后文件(从暂存区和工作区中删除)
- git diff --cached 或 $ git diff --staged 查看尚未提交的更新
- git stash push 将文件给push到一个临时空间中
- git stash pop 将文件从临时空间pop下来
git remote add origin git@github.com:username/Hello-World.git
git push origin master 将本地项目给提交到服务器中
git pull 本地与服务器端同步
git push (远程仓库名) (分支名) 将本地分支推送到服务器上去。
git push origin serverfix:awesomebranch
git fetch 相当于是从远程获取最新版本到本地，不会自动merge
git commit -a -m "log_message" (-a是提交所有改动，-m是加入log信息) 本地修改同步至服务器端 ：
git branch branch_0.1 master 从主分支master创建branch_0.1分支
git branch -m branch_0.1 branch_1.0 将branch_0.1重命名为branch_1.0
git checkout branch_1.0/master 切换到branch_1.0/master分支du -hs
git branch 删除远程branchgit push origin :branch_remote_namegit branch -r -d branch_remote_name
** C，删除远程仓库（比如GitHub）的目录或文件**
具体操作
- 拉取远程的Repo到本地（如果已经在本地，可以略过） 
- $ git clone xxxxxx
- 在本地仓库删除文件 
- $ git rm 我的文件
- 在本地仓库删除文件夹 
- $ git rm -r 我的文件夹/
此处-r表示递归所有子目录，如果你要删除的，是空的文件夹，此处可以不用带上-r。
- 提交代码 
- $ git commit -m"我的修改"
- 推送到远程仓库（比如GitHub） 
- $ git push origin xxxxxx
补充**: git rm**
查看git rm的说明文档:
$ git rm -h
用法：git rm [<选项>] [--] <文件>...
    -n, --dry-run         演习
    -q, --quiet           不列出删除的文件
    --cached              只从索引区删除
    -f, --force           忽略文件更新状态检查
    -r                    允许递归删除
    --ignore-unmatch      即使没有匹配，也以零状态退出
