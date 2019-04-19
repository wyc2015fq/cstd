# Qt Creator 中使用 Git / Github - baidu_33570760的博客 - CSDN博客
2017年07月07日 17:21:27[carman_风](https://me.csdn.net/baidu_33570760)阅读数：2922
> 
Github是目前最流行的代码仓库，通过 Qt
 Creator 可以很方便的访问 Github ，下面来看看如何配置。
原文链接：http://blog.csdn.net/hebbely/article/details/52495073
# 1、下载并安装git
> 
首先安装 [Git](http://lib.csdn.net/base/git)(msysGit) 最新版是2.10.0下载地址 http://[git](http://lib.csdn.net/base/git)-scm.com/downloads
![](https://img-blog.csdn.net/20160910151353463)
> 
或者https://git-scm.com/downloads/win
![](https://img-blog.csdn.net/20160910151613995)
> 
这里下载的是32bit的
> 
安装很简单，请按图操作 
![](https://img-blog.csdn.net/20160910152250050)![](https://img-blog.csdn.net/20160910152259263)![](https://img-blog.csdn.net/20160910152307029)![](https://img-blog.csdn.net/20160910152315576)![](https://img-blog.csdn.net/20160910152326832)![](https://img-blog.csdn.net/20160910152333894)![](https://img-blog.csdn.net/20160910152343779)
![](https://img-blog.csdn.net/20160910153451204)
# 2、github注册
> 
要托管到github，那你就应该要有一个属于你自己的github帐号，所以你应该先到github.com注册
> 
① 打开浏览器
> 
② 在地址栏输入github.com地址：
> 
③ 填写用户名、邮箱、密码
> 
④ 点击Sign up即可简单地注册
![](https://img-blog.csdn.net/20160910153935085)
# 3、设置用户名、邮箱
> 
安装完成后，打开 Git
 Bash，设置用户名和邮箱，邮箱最好与github注册的邮箱一致，便于记忆。代码如下：
git config --globaluser.name "你自己的用户名"
git config --globaluser.email "你自己的邮箱"
配置完成后，我们可以再次输入，不包括名称，可以看到配置成功了
![](https://img-blog.csdn.net/20160910155430027)
# 4、生成密钥并设置
> 
生成密匙，注意参数大小写并且不要设置密码（直接回车）。
ssh-keygen
 -t rsa -C"你自己的邮箱"
![](https://img-blog.csdn.net/20160910155416588)
> 
生成后的公匙文件在 C:\Users\用户名\.ssh\id_rsa.pub
![](https://img-blog.csdn.net/20160910155902892)
> 
打开密匙文件并复制其中内容将该密匙添加到github 
 (Settings-> SSH and GPG keys-> new SSH key），然后添加公共密钥
![](https://img-blog.csdn.net/20160912135247198)
![](https://img-blog.csdn.net/20160910163150252)
> 
为什么GitHub需要SSH
 Key呢？因为GitHub需要识别出你推送的提交确实是你推送的，而不是别人冒充的，而Git支持SSH协议，所以，GitHub只要知道了你的公钥，就可以确认只有你自己才能推送。
git推荐使用SSH协议的原因：
使用SSH协议可以避免每次push输入密码。前提是账户里配置好SSH密钥。git@github.com开头的就是SSH协议。使用Https协议则会导致每次都输入用户名和密码。 
# 5、在GitHub中新建一个库
![](https://img-blog.csdn.net/20160910163210528)
> 
创建操作如图：
![](https://img-blog.csdn.net/20160912135320276)
> 
创建成功后会出现设置提示信息界面，会看到如下类似内容
![](https://img-blog.csdn.net/20160912135342597)
# 6、配置Qt Creator的git设置
 
   打开工具/Tools-->选项/Options
 -->[版本控制](http://lib.csdn.net/base/git)-->Git，在Prepend
 toPATH中设置Git的所在目录后确定并重启QtCreator。
![](https://img-blog.csdn.net/20160912135359479)
> 
重新打开QtCreator在Tools菜单就会看到Git项，选择 Create
 Repository..添加一个仓库
![](https://img-blog.csdn.net/20160910164912207)
> 
来到工程的目录下，右键-->“Git
 Bash Here”；或者“菜单”-->“所有程序”-->“Git”-->“Git
 Hash”，输入工程目录“cd
  /e/Windows7_share/Qt5.5/project/myStyle”
![](https://img-blog.csdn.net/20160912104517381)
![](https://img-blog.csdn.net/20160912104527319)
> 
输入"git
 init"代码仓库就创建了，继续输入“ls
 -al”就可以看到目录下有个.git文件夹。
![](https://img-blog.csdn.net/20160912114608348)
> 
并输入刚才 GitHub 的 SSH 地址信息(https://github.com/hebbely/QtDemo.git)
git remote add origin https://github.com/hebbely/QtDemo.git
3、编辑项目
4、git add . （将改动添加到暂存区）
5、git commit -m "提交说明"
6、git push origin master 将本地更改推送到远程master分支
如果在github的remote上已经有了文件，会出现错误。此时应当先pull一下，即：
`git pull origin master`
然后再进行：
`git push origin master`
这样就完成了第一次推送（push）。以后就可以直接在QtCreator中操作，如
![](https://img-blog.csdn.net/20160912114613020)
# 7、git submodule
> 
Git归并有两种策略：递归，章鱼。
A.递归策略：当分支数只有两个的时候。
B.章鱼策略：当分支数大于两个的时候。Git会自动选择归并的方法。
C.子树策略：是Git另一种归并方法。（submodule）
> 
为工程添加子模块：
**[cpp]**[view plain](http://blog.csdn.net/hebbely/article/details/52495073#)[copy](http://blog.csdn.net/hebbely/article/details/52495073#)
-  cd demo_project  
-  git submodule add git://github.com/felixge/node-mysql.git deps/mysql
-  git submodule init  
-  git submodule update  
- # 提交更改
git submodule init and update
 就可以直接获取依赖模块了
**[cpp]**[view plain](http://blog.csdn.net/hebbely/article/details/52495073#)[copy](http://blog.csdn.net/hebbely/article/details/52495073#)
- git submodule init  
- git submodule status  
> 
删除：
submodule的删除稍微麻烦点：首先，要在“.gitmodules”文件中删除相应配置信息。然后，执行“git rm –cached ”命令将子模块所在的文件从git中删除
下载的工程带有submodule：
当使用git clone下来的工程中带有submodule时，初始的时候，submodule的内容并不会自动下载下来的，此时，只需执行如下命令：
**[cpp]**[view plain](http://blog.csdn.net/hebbely/article/details/52495073#)[copy](http://blog.csdn.net/hebbely/article/details/52495073#)
- git submodule update --init --recursive  
> 即可将子模块内容下载下来后工程才不会缺少相应的文件
