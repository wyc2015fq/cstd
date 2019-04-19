# Git操作说明 - _天枢 - 博客园
## [Git操作说明](https://www.cnblogs.com/yhleng/p/7735310.html)
2017-10-26 09:47 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7735310)
**Git操作说明**
 1、**将本地项目上传到GitHub**
1)    首先在GitHub上注册帐户
2)    在GitHub上创建仓库
3)    Pc安装Git客户端(Git Bach)
4)    打开Git Bach进入到项目目录
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026093213051-1046539332.png)
5)    初始化项目为Git仓库：Git init命令
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026093225160-1635490646.png)
　　会在项目目录，下生成一个.Git的隐藏文件夹，仓库初始化成功。
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026093234691-83707991.png)
6)    将项目增加到暂存区：Git add .
注意命令git add . 后边的点代表，增加所有文件夹及文件；
也可以单独增加某个文件例如:git add redme.txt
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026093245254-1618332689.png)
7)    将本地仓库关联到远程GitHub
Git remote add xxxx [https://xxxx.git](https://xxxx.git/)  关联远程仓库
Git remote rm xxxx  删除远程仓库
首先在GitHub上查到一下远程仓库地址，这里以https的为例：
https://github.com/Isinopec/E_BUSINESS_WIN.git
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026093301098-537723726.png)
使用命令进行关联:Git remote add 远程名称(自定义) 远程仓库地址
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026093314816-1167930662.png)
8)    提交本地代码git commit am “注释”
Git commit –m “first commit” 提交
Git status 查看是否文件未提交
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026093329473-1179630192.png)
9)    拉取GitHub项目到本地仓库
为什么要做这一步呢，不先pull拉取直接push推送，可能会出现错误
原因，是因为远程版本仓库，比本地要新。
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026093347926-1334374716.png)
Git pull business master命令
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026093400051-553471635.png)
10) 推送本地代码到远程仓库
Git push business master   提交让输入用户名，密码验证
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026093422754-1287369712.png)
如果，还报第9步的错误，直接强行推送git push business master –f
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026093432363-65118586.png)
上图推送成功，到GitHub上刷新看看；项目以上传到github库
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026093446738-433526090.png)
11) 总结一下用到的命令：Git Bash进入到项目目录:
Git init 将项目初始化成git仓库
Git add . 将所有文件增加到暂存区
Git commit –m “commit” 提交所有文件到暂存区
Git status 查看未提交文件
Git remote add <远程名称> [https://xxxxx.xxxx.git](https://xxxxx.xxxx.git/)  关联远程和本地仓库
Git pull <远程名称> master 拉取远程仓库到本地合并，以保证本地最新
Git push <远程名称> master 或 git push <远程名称> master –f  推送本地到远程仓库，第一次提交可以在远程名称前加 –u 这样可以，自动提交到远程合并分支。
**2、暂存区与工作区的区别**
暂存区：就是项目录隐藏的.git文件夹，commit提交之后，会将更改存在暂存区
工作区：.git以外的项目为工作区。
**3、 分支和合并**
分支合并：程序新增一功能，为了不影响主程序，创建分支，在分支上开发，全部功能完成后，在进行合并到主分支。
命令：前提是进入到仓库目录
1）Git branch 显示所有分支，并且分支名称前带*星号的是当前分支
2）Git branch xxxx 创建分支xxxx
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026094106348-601844956.png)
3）Git checkout xxxx 切换当前分支
从上图可以看出，当前分支为带*星号的master，那么我们要怎么切换到其它分支呢
需使用命令Git checkout add001 ，下图可以看出，当前分支以从主分支切换到了add001
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026094118457-304374764.png)
切换分支，到底有什么作用呢，前边已经解释过，在分支上编辑代码，并不影响master主分支。只有当交分支合并到主分支的时候，主分支才会改变。
4）Git checkout –b xxxx创建并切换分支(此命令，相当于实现在第2和第3步两步命令)
Git checkout –b xxxx 创建一个新的分支xxxx并切换新分支为当前分支。相当于执行以下两个命令:
Git branch xxxx
Git checkout xxxx
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026094133269-20577896.png)
5)Git branch –d xxxx删除xxxx分支
使用Git branch –d add002 删除名称为add002的分支；
另外提醒一下，当前分支是不能删除的。因为是checkout状态。
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026094147582-687195813.png)
那么要怎么删除当前分支呢，转换一下思想，我把当前的分支切换了不就好了。
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026094204504-1657139270.png)
切换后，再删除add002，成功删除掉
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026094218738-2099678854.png)
**小结：有一种情况，在新分支上开发了新代码，现在发现功能没有用，想直接把分支删除掉，于是使用命令：git branch –d xxxx;然后发现报错了，提示此分支还没有被合并。**
**删除不掉，那怎么办，强制删除呗，命令：git branch –D xxxx 强制删除，参数是-D是大写。**
6）合并分支到master
当某个分支开发完毕，需要合并到主分支，需要合并分支。
Git merge xxxx 将名称为xxxx的分支，合并到当前分支
所以使用git merge xxxx合令时，要注意，这个命令是将某分支，合并到当前分支
我们要将分支合并到master主分支那么，需要切换当前分支为master
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026094407676-949160196.png)
7）总结
查看分支：`git branch`
创建分支：`git branch <name>`
切换分支：`git checkout <name>`
创建+切换分支：`git checkout -b <name>`
合并某分支到当前分支：`git merge <name>`
删除分支：`git branch -d <name>`
分支使用基本原则：
1）首先master分支，是用来发布新版本的，不要在这个分支上干活
2）合并分支时，默认Git会选择fast forward模式，此模式下，合并后，删除分支
会丢掉分支信息。所以合并的时候可以加上参数—no—ff方式禁用掉fast forward模式
Git merge –no—ff  xxxx
**4、 解决冲突**
理想是美好的，现实是残酷的，合并也并不会一帆风顺，项目中的共同文件变化，合并时会产生冲突，这时怎么解决冲突呢？
执行完git merge xxxx产生冲突，可以使用git status命令，会告诉我们哪些文件冲突了，需要手动去解决。
**5、 从远程仓库克隆到本地**
多人协作开发，只需将远程代码克隆到本地就可以了。
前边只讲了，怎么把本地项目上传到GitHub，那么要把GitHub上的项目克隆到本地怎么办呢？我们需要使用以下命令。
前提：进入本地要存储仓库目录，执行命令
命令：Git clone [https://github.com/Isinopec/E_BUSINESS_WIN.git](https://github.com/Isinopec/E_BUSINESS_WIN.git)
具体仓库地址怎么获取，前边说过了，这里就不再提了。
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026094453457-582118999.png)
如下图，已经从GitHub，将代码克隆到了本地
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026094507738-1754945180.png)
小结：
1)Git支持https、ssh、原生态的git协议这里只讲了，https协议的地址;
2)当从远程克隆到本地git clone [https://xxxxx.git](https://xxxxx.git/)时，其实本地的master和远程的master已经默认关联起来了，并默认远程名称为origin
使用git remote 或git remote –v显示详细信息
如果要推送分支git push origin master就可以将本地，推送到远程的master分支上。
**6、 Git stash工作区暂存功能**
命令:Git stash
什么时候会用到，这项功能呢，举例：
比如你正在开发某一功能，还没有开发完成，现在有一项新任务，要修复一个bug
那么怎么办呢，新建一个bug分支，但是当前分支，还没有提交，因为没有开发完成。创建新分支会出问题，因为工作区不是干净的。
那么就要引用Git stash命令来，暂存当前工作区。然后在创建新的分支来修复bug
完成bug修复后，切回到原来的分支，使用git status命令发现工作区是干净的，原来暂存的去哪里了。使用Git stash list命令查看。
要恢复原来的暂存分支内容两个办法：
1） 是用git stash apply恢复，但是恢复后，stash内容并不删除，你需要用git stash drop来删除；
Git stash apply switch@{0}  ;switch@{0}是通过 git stash list命令查到的暂存名称。
2）另一种方式是用git stash pop，恢复的同时把stash内容也删了
**7、 分支打标签**
为分支打上标签，标记某分支的功能，不至于在很久后，不知道分支是作什么用的。
命令：
Git checkout xxxx  切换到要打标签的分支
Git tag v1.0  为当前分支打上标签为v1.0
Git tag  查看所有标签
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026094643707-2066957985.png)
删除标签：
Git tag –d v2.0
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026094651785-490972859.png)
小结：
本地标签不会主动推送到远程，因此安全操作，要推到远程使用命令：git push origin –tags  ;从远程删除标签命令：git push origin :refs/tags/v2.0
看远程删没删除标签，可以在GitHub上看
**8、 Git常用命令**
![](https://images2017.cnblogs.com/blog/1149221/201710/1149221-20171026094701082-918821551.png)
