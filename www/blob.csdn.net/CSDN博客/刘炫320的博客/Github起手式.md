# Github起手式 - 刘炫320的博客 - CSDN博客
2016年09月03日 10:38:33[刘炫320](https://me.csdn.net/qq_35082030)阅读数：360
所属专栏：[github官方指南](https://blog.csdn.net/column/details/16517.html)
Github作为一个开源代码库以及版本控制系统，现在越来越受到开发者的欢迎，不过根据统计，大概最多的还是要数JS开发者了，大概有40%的用户是JS开发者，JAVA和C的开发者，各自只占到了5%的份额，而C#语言的开发者，只占到了2%。不过相信随着Github越来越流行，这些都会慢慢增长的。
那么，既然这么流行，如何使用？对于一个从来没有用过SVN，CVS的孩子来讲，Git的分布式版本控制可谓是相当复杂。除了什么是文件、工作目录、资源库、版本、分支和标签等概念等这些常规的版本控制概念外，Git还有快照、工作树、索引、本地资源库、远程资源库、远程、提交、分支和 Stash 等，其中有很多非常棒的功能，但是对于初学者来讲，暂时用不到，在接下来的过程中，我们会慢慢介绍的。
现在一个首要的任务，就是创建一个项目，然后把本地的项目上传上去即可！就是这么简单的一个过程，其他的什么都不用考虑，也不考虑集成开发环境中使用，最简单的，现在有一个项目，我需要把这个项目提交到Github上，如何进行。
首先了解一下Github的最基本的工作模式，首先它在云端有一个仓库，用来存放数据文件。其次，它在本地有一个仓库，用来暂时存放数据，这样，你工作环境和最终版本是分开的，需要你使用Git命令来进行同步。同步的方式一个是上传，一个是下载，而其中就会有很多命令，我们只介绍本文的目标，上传一个项目所用到的命令。
接下来进入正题。为了让整个过程更加有条理，我个人把整个步骤分为以下4个部分：
1、  进行Github账号的注册。
2、  在Github网站上进行仓库创建。
3、  创立本地仓库。
4、  使用Git客户端进行SSH配置（这个只需要一次即可。）
5、  同步本地仓库与云端仓库。
接下来，我们进行详细介绍。
**一、进行Github账号的注册**
首先，你得拥有一个Github账号。这个只需要去github.com上面注册一个即可，具体过程不再介绍。
**二、在Github网站上进行仓库创建**
当登入之后，创建一个项目的地方在此：
![](https://img-blog.csdn.net/20160903104250025?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击New repository（仓库）即可创建一个新的项目。
![](https://img-blog.csdn.net/20160903104316573?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
填写完毕后，点击确认即可，就进入到下面这个画面了。
![](https://img-blog.csdn.net/20160903104336900?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上面写的很清楚。但是如何进行操作呢？
**三、创立本地仓库**
在云端上面的事情已经做完了。下面就要做我们本地的事情了。
首先，安装一个git客户端：[http://msysgit.github.com/](http://msysgit.github.com/)
接下来就是要配置Git了，这一点通常是初学者需要注意的地方。首先，找到你项目的所在文件夹。
![](https://img-blog.csdn.net/20160903104402370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
类似这种，现在只有一个项目文件夹。右键，选择Git Bash，之后就会弹出一个控制台了。
首先执行下面语句，进行初始化仓库：
![](https://img-blog.csdn.net/20160903104423558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**四、使用Git客户端进行SSH配置**
然后如果第一次的话，还需要进行SSH配置，也就是你和云端的安全协议，命令如下：
$ssh-keygen -t rsa -C [your_email@youremail.com](mailto:your_email@youremail.com)
全部选择回车，会在C盘用户目录下的.ssh文件夹里生成一个id-rsa文件。
![](https://img-blog.csdn.net/20160903104453184?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接下来，就是进入到这个目录下，打开id_rsa.pub，复制里面的key，记住是全部复制。
然后，回到github网站上，点击1里的settings进入到如下界面，然后点击SSH下的New SSH key。
![](https://img-blog.csdn.net/20160903104524466?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后输入title和key，点击Add即可。
![](https://img-blog.csdn.net/20160903104545826?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这样就创建完毕了。
接下来，就使用命令来验证一下是否成功。
$ ssh -T[git@github.com](mailto:git@github.com)
如果看到You’vesuccessfully authenticated, but GitHub does not provide shell access
则说明已经成功连接了。
**五、同步本地仓库与云端仓库**
然后，要配置一下用户名和邮箱
$ gitconfig --global user.name "your name"
$ gitconfig --global user.email “[your_email@youremail.com](mailto:your_email@youremail.com)”
然后键入以下命令：
$ gitremote add origin [git@github.com:yourName/yourRepo.git](mailto:git@github.com:yourName/yourRepo.git)
后面的yourName和yourRepo表示你再github的用户名和刚才新建的仓库，与github上的对应。
接下来就是重头戏了。为了保证没有问题，我们应该先使用如下命令：
$gitpull --rebase origin master
如果之前没有README.MD的话，就不需要了。
直接添加我们的项目。
$git add项目名
然后使用
$gitcommit –m “要说的话”
最后就剩下一步了，就是上传到github上。
$ gitpush origin master
这样就把整个项目提交到github上面了。
以上就是整个项目的提交过程，其实也不是太复杂，对不对？
