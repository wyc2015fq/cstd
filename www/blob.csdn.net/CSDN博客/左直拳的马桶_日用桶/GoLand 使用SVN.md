# GoLand 使用SVN - 左直拳的马桶_日用桶 - CSDN博客
2018年11月29日 10:12:16[左直拳](https://me.csdn.net/leftfist)阅读数：461
GoLand是jetbrains公司的go语言开发工具。近期使用GoLand来进行一些开发，因为公司采用SVN进行代码管理。那么，GoLand如何使用SVN呢？
**一、在GoLand里安装SVN插件**
方法是在file-setting-plugin里搜索“subversion”，然后安装![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128175329452.png)？大致如此。
**二、以上仅为装在GoLand里的插件**
真正使用SVN的，应该SVN客户端。我本机装的是TortoiseSvn小乌龟。如果没有安装，那需要安装此客户端。
**三、打开GoLand，打开项目。这时多半是使用不了SVN，GoLand会报错**：
```
can't use subversion command line client : svn
Probably the path to Subversion executable is wrong. Fix it..
```
这是什么原因呢？
这是因为GoLand要调用svn客户端的命令。找不到这个命令，所以报错。
对于小乌龟tortoisesvn来说，它的命令行程序是：svn.exe，这个文件装在我机器中位于：
```
C:\Program Files\TortoiseSVN\bin
```
但是，默认情况下，是没有这个命令行的，因为安装的时候，默认是没有安装命令行。所以如果找不到这个文件，那就要将SVN命令行安装上去。打开tortoisesvn安装包，安装时记得选上命令行
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128181530962.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
装好后也许还要设置系统PATH。以前的客户端貌似不会自己设置；我今天装的1.11版可以。
四、在GoLand里设置SVN
老规矩，file-setting,version control-subversion
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129095951107.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
路径要带上svn.exe。估计GoLand是“面向接口”的，SVN工具除了小乌龟，可能还有别的客户端工具，命令行命令不一定名称就是svn.exe。
**五、要在SVN客户端里填上账号和密码等认证信息**
到了这一步可能还不行，如果是初次使用或者是重装了SVN客户端工具，还要在客户端工具里输入SVN账号及密码。否则会提示什么协议无效之类的错误。
