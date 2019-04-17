# 我也用github - xiahouzuoxin - CSDN博客





2013年07月20日 20:54:42[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：10792











GitHub（网址 [https://github.com/](https://github.com/)）是一个面向开源及私有软件项目的托管平台，因为只支持Git作为唯一的版本库格式进行托管，故名GitHub。

GitHub的注册用户已经超过百万，托管的版本库数量已超三百万，其中不乏知名的开源项目，如：Ruby on Rails、Hibernate、phpBB、jQuery、Prototype、Homebrew等。

GitHub于2008年4月10日正式发布，相比始于1999年的SourceForge和2005年的GoogleCode，GitHub后来者居上。以2011年的数据从代码提交数量上看，GitHub已经超越其前辈，如图1所示。

![../images/survival-of-the-forges.png](http://www.worldhello.net/gotgithub/images/survival-of-the-forges.png)
图1：开源项目托管平台提交数量对照





***1 创建github帐号及环境配置***

到[https://github.com/](https://github.com/)创建用户帐号，这个大家都会。创建完进入后界面如下：

![](https://img-blog.csdn.net/20130720201053328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




在右上角依次有用户名（我的是xiahouzuoxin）、create a new rope图标、Account Settings图标、Sign Out图标。

点击Account Settings图标进入用户设置界面（如下图），我们这里只讨论如何设置SSH
 Keys，其它的都是些信息，自己设置。


![](https://img-blog.csdn.net/20130720201448531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在设置SSH Keys之前，先运行下面的命令试试，

```
$ ssh -T git@github.com
Permission denied (publickey).
```
显示登录失败。这是因为没有设置公钥（publickey）的缘故，这也是为什么要对SSH Keys设置的原因。


下面为设置步骤：


（1）生成公钥和私钥


$ ssh-keygen -C "xiahouzuoxin@163.com" -f ~/.ssh/gotgithub
xiahouzuoxin@163.com为你的邮箱，gotgithub为私钥文件名，对应的公钥文件名为gotgithub.pub，这个文件名可以自己命名，这连个密钥文件都保存在~/.ssh/目录下。





（2）配置config

当生成的公钥/私钥对不在缺省位置（~/.ssh/id_rsa等）时，使用ssh命令连接远程主机时需要使用参数-i<filename>指定公钥/私钥对。或者在配置文件~/.ssh/config中针对相应主机进行设定。例如对于上例创建了非缺省公钥/私钥对~/.ssh/gotgithub，可以在~/.ssh/config配置文件中写入如下配置。

Host github.com
  User git
  Hostname github.com
  PreferredAuthentications publickey
  IdentityFile ~/.ssh/gotgithub


（3）在github网站的账户设置的SSH
 Keys选项卡中新建SSH Key （Add SSH Key）

![](https://img-blog.csdn.net/20130720202654187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Title中随便填入一个名字。

进入~/.ssh/目录，打开公钥文件gotgithub.pub，将其中的内容拷贝到Key框中。注意，拷贝时切莫在其中插入多余的换行符、空格等，否则在公钥认证过程因为服务器端和客户端公钥不匹配而导致认证失败。




（4）验证是否配置成功，
`$ ssh -T git@github.com`若出现类似



Hi xiahouzuoxin! You've successfully authenticated, but GitHub does not provide shell access.

的提示，说明密钥配置成功。




（5）设置用户信息


$ git config --global user.name "xiahouzuoxin"          //给自己起个用户名
$ git config --global user.email  "xiahouzuoxin@163.com"//填写自己的邮箱





注：github仓库对应多种地址，一个是通过http访问的地址，一个是通过ssh访问的地址。举例，




HTTP格式地址为：[https://github.com/xiahouzuoxin/mp3-edcode.git](https://github.com/xiahouzuoxin/mp3-edcode)


SSH格式地址为：[git@github.com:xiahouzuoxin/mp3-edcode.git](http://blog.csdn.net/xiahouzuoxin/article/details/9398629)

通过http地址访问时可以不用配置公钥（PublicKey），但每次访问时都需要输入github的用户名和密码。




2
 初次使用——创建一个项目


用户登录后系统，在github首页，点击页面右下角“New Repository”

填写项目信息：

**project name**: zx-libsvm

**description **: my first project

点击“**Create Repository**” ； 现在完成了一个项目在github上的创建。

说明：我们创建的是一个github仓库，一个仓库里只能存放（或叫对应）一个项目。






如果是从头创建版本库，可以采用先克隆，建立提交数据，最后再通过推送完成GitHub版本库的初始化。步骤如下：
- 
克隆版本库。
$ git clone git@github.com:xiahouzuoxin/zx-libsvm.git
克隆过程会显示警告，不过这个警告可以忽略，因为GitHub创建的版本库本来就是一个空白的版本库。克隆地址在项目页面右下角可以看到，选中SSH URL方式，如下图右下角。![](https://img-blog.csdn.net/20130720204439687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


- 
创建文件README.md。

READNE文字采用Markdown格式，您也可以使用其他支持的格式，只要确保README文件使用正确的扩展名。本书附录部分介绍了Markdown及其他GitHub支持的标记语言。关于Markdown，目前我们只需知道这一个易于识别和理解的纯文本格式，可以方便的转换为HTML。Markdown语法非常像我们在写邮件（纯文本）时用空行来分隔段落、用星号开启列表、用缩进表示引用内容等等。

- 
添加README.md文件并提交。注意：此时是提交到本地的git库中。

$ git add README.md
$ git commit -m "README for this project."

- 
向GitHub推送，完成版本库初始化。此时提交到github上。

$ git push origin master


这就完成了一个基本的git clone和源文件修改后往github上推送的功能。





3
 常用命令（转自Ref[2]）




**假如你现在新创建了一个项目，想把它提交到github上面？**



假设你创建好了一个项目，并切换到项目的根目录下面：

**$ git status** //查看当前项目下所有文的状态，如果第一次，你会发现都红颜色的，因为它还没有交给git/github管理。

**$ git add . ** //（.）点表示当前目录下的所有内容，交给git管理，也就是提交到了git的本地仓库。

**Ps**:git的强大之处就是有一个本地仓库的概念，在没有网络的情况下可以先将更新的内容提交到本地仓库。

**$ git commit –m”discription ”**  //对你更新或修改了哪些内容做一个描述。

**$ git remote add origin git@github.com:xiahouzuoxin/zx-libsvm.git**

// 如果你是第一次提交项目，这一句非常重要，这是你本地的当前的项目与远程的哪个仓库建立连接。

**Ps**: **origin**可以改为别人的名字，但是在你下一次push（提交）时，也要用你修改之后的名字。

**$ git remote -v ** //查看你当前项目远程连接的是哪个仓库地址。

**$ git push -u origin master ** //将本地的项目提交到远程仓库中。



 ------------------------------------------------------------

**假如，你回到了家，想把公司提交的项目克隆到本地？**



如果你是第一次想把github上面的项目克隆到本地或者要克隆别人的项目到地。

**$ git clone git@github.com:**xiahouzuoxin/zx-libsvm.git****


//在git下面切换到想存放此项目的文件目录下，运行这条命令就可以将项目克隆下来。



假如本地已经存在了这个项目，而仓库中又有一新的更新，如何把更的合并到本地的项目中？

**$ git fetch origin **   //取得远程更新，这里可以看做是准备要取了

**$ git merge origin/master**  //把更新的内容合并到本地分支/master



------------------------------------------- 

**项目中删除了一些文件，如何提交？**



假如远程仓库中已经存了aaa这个文件，我fetch了下来，并删除了aaa这个文件，想再push上到远程仓库中，并使远程仓库中的项目被新的修改覆盖（也就是远程仓库中的aaa也被删除）

**$ git status**   //可以看到我们删除的哪些文件

**$ git add .**   //删除之后的文件提交git管理。

**$ git rm   src/com/hzh/hibernate/dao/aaa.java**    //移除我们删除的那个文件，不然git不允许我们往远程仓库提交。

Ps: 如果你想删除的是某个目录（java包），这里想移除整个目录的内容。

**$ git rm  src/com/hzh/hibernate/bbb/ -r **  // **-r** 会把bbb/目录下的所有内容一次性移动。



------------------------------------------------------------------------

**远程创建了一个新仓库，本地创建了一个新项目，如何使新的项目与仓库对应起来？**



其实，这个也很简单，只是我当时对那些命令不太理解，所以比较模糊，不知如何对应。



**$ git remote add origin git@github.com:**xiahouzuoxin/zx-libsvm.git****

//还是这个命令，在你push项目之前加上这一句就OK了。

**git@github.com:xiahouzuoxin/zx-libsvm.git**就是你常见的新仓库的地址啊。git切换到新项目下，在push之前，加上这一句，我们创建的新仓库就与新项目建立了连接。


**Refrences**

[1] http://www.worldhello.net/gotgithub/index.html

[2] http://www.cnblogs.com/fnng/archive/2012/01/07/2315685.html

[3] https://help.github.com/articles/set-up-git

[4] [https://code.google.com/p/msysgit/](https://code.google.com/p/msysgit/)   Windows下的github客户端









