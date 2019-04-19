# subversion(server)与TortoiseSVN(client)的安装与简单使用 - 零度的博客专栏 - CSDN博客
2017年03月24日 11:13:55[零度anngle](https://me.csdn.net/zmx729618)阅读数：366
       本文介绍了 Subversion 的基本概念。详细讲解了在Windows下如何安装使用Subversion与TortoiseSVN。
### 一、基本概念
1、什么是版本控制
       简单点来说，版本控制就是数据仓库，它可以记录你对文件的每次更改。这样，就算你在昏天黑地的改了,几个月后老板说不要了，还是按照过去那样，你也不会抓狂，简单的恢复版本操作就搞定一切。
2、什么是Subversion
        Subversion是一个自由/开源版本控制系统，它管理文件和目录可以超越时间。一组文件存放在中心版本库，这个版本库很像一个普通的文件服务器，只是它可以记录每一次文件和目录的修改，这便使你可以取得数据以前的版本，从而可以检查所作的更改。从这个方面看，许多人把版本控制系统当作一种“时间机器”。
Subversion可以通过网络访问它的版本库，从而使用户可以在不同的电脑上使用。一定程度上可以说，允许用户在各自的地方修改同一份数据是促进协作。进展可能非常的迅速，并没有一个所有的改变都会取得效果的通道，由于所有的工作都有历史版本，你不必担心由于失去某个通道而影响质量，如果存在不正确的改变，只要取消改变。
3、版本库(repository)
      Subversion 的核心就是repository ，中文翻译成“版本库”。就是位于服务器端，统一管理和储存数据的地方。
### 二、安装
1、获取安装程序
        (1)获取 Subversion 安装程序（获取方法随时间推移可能有变）。
        到官方网站([http://subversion.tigris.org/](http://subversion.tigris.org/))下载最新的安装程序（如Setup-Subversion-1.6.5.msi）。
        (2)获取 TortoiseSVN 客户端程序（获取方法随时间推移可能有变）。
从官方网站 http://tortoisesvn.net/downloads 获取最新的 TortoiseSVN（如TortoiseSVN-1.6.5.16974-win32-svn-1.6.5.msi） 。TortoiseSVN 是一个客户端程序，用来与subserver服务器端通讯。Subversion 自带一个客户端程序 svn.exe ,但 TortoiseSVN 更好操作，提高效率。
2、安装服务器端和客户端
       首先双击安装 Subversion。按提示安装即可。
       其次双击安装TortoiseSVN。按提示安装即可。
3、建立版本库(Repository)
      运行Subversion服务器需要首先要建立一个版本库(Repository)。版本库可以看作是服务器上集中存放和管理数据的地方。
      这里我们建立版本库使用TortoiseSVN客户端方式。首先任意建立在一个空目录如 E:/svn/repos，注意一定要是空文件夹的。然后在repos 文件夹上“右键->TortoiseSVN->Create Repository here...”，按提示就可以创建一系列所需文件夹和文件。
![](https://img-blog.csdn.net/20170324110848502?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem14NzI5NjE4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4、验证工作
       为了验证svnserve正常工作，在桌面上右键TortoiseSVN -> Repo-browser 来查看版本库。在弹出的 URL 对话框中输入：**file:///e:/****svn/repos/**。点OK按钮后就可以看见repos版本库的目录树结构了，只不过这时repos是个空库。
三、基本使用
1、初始化导入
        下面就是将我们的数据(项目)导入到版本库里，以后就由版本库管理我们的数据。我们的任何改动都会被版本库记录下来，甚至我们自己丢失、改错数据时版本库也能帮我们找回数据。比如，我在 d:/wwwroot 下有个 guestbook 文件夹，里面存放的是我编写的留言簿程序。在此文件夹上“右键 -> TortoiseSVN -> Import...” ，在弹出对话框的“URL of repository”输入“**file:///e:/****svn/repos/guestbook**”。在“Import message”输入“导入整个留言簿”作为注释。完成后guestbook中的内容全部导入到了e:/svn/repos/guestbook。我们看到在 e:/svn/repos没有任何变化，连个guestbook 文件夹都没有建立，唯一的变化就是e:/svn/repos1容量变大了。实际上我们源guestbook中的内容已经导入 repos1 版本库了，源guestbook文件夹可以删除了。
四、基本操作流程
1、取出(check out)
      取出版本库到一个工作拷贝：来到任意空目录下，比如在f分区建立一个空文件夹 f:/work 。“右键-> SVN Checkout”。在“URL ofrepository”中输入"**file:///e:/svn/repos/guestbook**"，这样我们就得到了一份 guestbook 中内容的工作拷贝。
2、存入(check in)/提交(commit)
      在工作拷贝中作出修改并提交：在 guestbook 工作拷贝中随便打开一个文件，作出修改，然后“右键 -> SVN Commit... ”。这样我们就把修改提交到了版本库，版本库根据情况存储我们提交的数据。在修改过的文件上“右键 -> TortoiseSVN -> Show Log” ，可以看到对这个文件所有的提交。在不同的revision 条目上“右键 -> Compare with working copy”，我们可以比较工作拷贝的文件和所选 revision 版本的区别。
