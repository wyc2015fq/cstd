# 史上最浅显易懂的Git教程2 github - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月11日 17:00:34[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：158

Git是分布式版本控制系统，同一个Git仓库，可以分布到不同的机器上。怎么分布呢？最早，肯定只有一台机器有一个原始版本库，此后，别的机器可以“克隆”这个原始版本库，而且每台机器的版本库其实都是一样的，并没有主次之分。
实际情况往往是这样，找一台电脑充当服务器的角色，每天24小时开机，其他每个人都从这个“服务器”仓库克隆一份到自己的电脑上，并且各自把各自的提交推送到服务器仓库里，也从服务器仓库中拉取别人的提交。
可以自己建一个git服务器，也可以先直接使用GitHub。
由于你的本地Git仓库和GitHub仓库之间的传输是通过SSH加密的，所以，需要一点设置：
第1步：创建SSH Key。
在用户主目录下，看看有没有.ssh目录，如果有，再看看这个目录下有没有id_rsa和id_rsa.pub这两个文件，如果已经有了，个人建议删除掉。
$
ssh-keygen -t rsa -C
"youremail@elesos.com"
一路回车，使用默认值即可，由于这个Key也不是用于军事目的，所以也无需设置密码。
如果一切顺利的话，可以在用户主目录里找到.ssh目录，里面有id_rsa和id_rsa.pub两个文件，这两个就是SSH
 Key的秘钥对，id_rsa是私钥，不能泄露出去，id_rsa.pub是公钥，可以放心地告诉任何人。
第2步：登陆GitHub，打开“Account settings”，“SSH
 Keys”页面：
然后，点“Add SSH Key”，填上任意Title，在Key文本框里粘贴id_rsa.pub文件的内容：
为什么GitHub需要SSH Key呢？因为GitHub需要识别出你推送的提交确实是你推送的，而不是别人冒充的，而Git支持SSH协议，所以，GitHub只要知道了你的公钥，就可以确认只有你自己才能推送。
最后友情提示，在GitHub上免费托管的Git仓库，任何人都可以看到喔（但只有你自己才能改）。所以，不要把敏感信息放进去。
如果你不想让别人看到Git库，有两个办法，一个是交点保护费，让GitHub把公开的仓库变成私有的，这样别人就看不见了（不可读更不可写）。另一个办法是自己动手，搭一个Git服务器，因为是你自己的Git服务器，所以别人也是看不见的。这个方法我们后面会讲到的，相当简单，公司内部开发必备。

添加完成之后是这样的
然后回到终端输入指令：ssh -T [git@github.com](mailto:git@github.com) 执行完这条指令之后会输出
 Are you sure you want to continue connecting (yes/no)? 输入 yes 回车
回到github，刷新网页就可以看到钥匙旁的灰色小圆点变绿，就表明已经添加成功了。
-T      Disable pseudo-tty allocation.禁止分配伪终端,当用ssh或telnet等登录系统时，系统分配给我们的终端就是伪终端。
[https://help.github.com/articles/testing-your-ssh-connection/](https://help.github.com/articles/testing-your-ssh-connection/)
添加文件后，先commit,然后push
现在的情景是，你已经在本地创建了一个Git仓库后，又想在GitHub创建一个Git仓库，并且让这两个仓库进行远程同步，这样，GitHub上的仓库既可以作为备份，又可以让其他人通过该仓库来协作，真是一举多得。
首先，登陆GitHub，然后，在右上角找到“Create a new repo”按钮，创建一个新的仓库test：
其他保持默认设置，不要选GPL协议，不然不是空库了。可以选择gitignore,最好是默认！！！
目前，在GitHub上的这个仓库还是空的，GitHub告诉我们，可以从这个仓库克隆出新的仓库，也可以把一个已有的本地仓库与之关联，然后，把本地仓库的内容推送到GitHub仓库。
在本地的仓库test下运行命令：
$
git remote add origin
git@github.com:michaelliao/learngit.git
注意，把上面的michaelliao替换成你自己的GitHub账户名，如[https://github.com/elesos/test.git](https://github.com/elesos/test.git)
否则，你在本地关联的就是我的远程库，关联没有问题，但是你以后推送是推不上去的，因为你的SSH
 Key公钥不在我的账户列表中。
添加后，远程库的名字就是origin，这是Git默认的叫法，
下一步，就可以把本地库的所有内容推送到远程库上：
git push -u origin master
实际上是把当前分支master推送到远程。
由于远程库是空的，我们第一次推送master分支时，加上了-u参数，Git不但会把本地的master分支内容推送的远程新的master分支，还会把本地的master分支和远程的master分支关联起来，在以后的推送或者拉取时就可以简化命令。

推送成功后，可以立刻在GitHub页面中看到远程库的内容已经和本地一模一样：
从现在起，只要本地作了提交add和commit之后，就可以通过命令：
3.$ git push origin master
把本地master分支的最新修改推送至GitHub
分布式版本系统的最大好处之一是在本地工作完全不需要考虑远程库的存在，也就是有没有联网都可以正常工作，而SVN在没有联网的时候是拒绝干活的！当有网络的时候，再把本地提交推送一下就完成了同步！
2，从远程库克隆
上面我们讲了先有本地库，后有远程库的时候，如何关联远程库。

现在，假设我们从零开发，那么最好的方式是先创建远程库，然后，从远程库克隆。
1，登陆GitHub，创建一个新的仓库，名字叫gitskills
我们勾选Initialize this
 repository with a README，这样GitHub会自动为我们创建一个README.md文件。[gitignore](https://github.com/elesos/FFmpeg_Demos/blob/master/.gitignore)也可以选择。创建完毕后，可以看到README.md文件：
现在，远程库已经准备好了，下一步是用命令git
 clone克隆一个本地库：
git clone git@github.com:michaelliao/gitskills.git
 git clone [git@github.com](mailto:git@github.com):elesos/FFmpeg_Demos
还可以用https://github.com/michaelliao/gitskills.git这样的地址。实际上，Git支持多种协议，默认的git://使用ssh，但也可以使用https等其他协议。
使用https除了速度慢以外，还有个最大的麻烦是每次推送都必须输入口令，但是在某些只开放http端口的公司内部就无法使用ssh协议而只能用https。
使用GitHub
如何参与一个开源项目呢？
点“Fork”就在自己的账号下克隆了一个bootstrap仓库，然后，从自己的账号下clone：
git clone git@github.com:michaelliao/bootstrap.git
一定要从自己的账号下clone仓库，这样你才能推送修改。如果从bootstrap的作者的仓库地址git@github.com:twbs/bootstrap.git克隆，因为没有权限，你将不能推送修改
如果你想修复bootstrap的一个bug，或者新增一个功能，立刻就可以开始干活，干完后，往自己的仓库推送。
如果你希望bootstrap的官方库能接受你的修改，你就可以在GitHub上发起一个pull request。当然，对方是否接受你的pull
 request就不一定了。
git push origin master后，可以在github网上自己的fork库处pull
 request
