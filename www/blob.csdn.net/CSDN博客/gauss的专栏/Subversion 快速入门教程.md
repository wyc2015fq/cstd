# Subversion 快速入门教程 - gauss的专栏 - CSDN博客
2011年12月05日 12:24:18[gauss](https://me.csdn.net/mathlmx)阅读数：191
                
何快速建立Subversion服务器，并且在项目中使用起来，这是大家最关心的问题，与CVS相比，Subversion有更多的选择，也更加的容易，几个命令就可以建立一套服务器环境，可以使用起来，这里配套有[动画教程](http://www.subversion.org.cn/media/all.swf)。
本文是使用Subversion最快速的教程，在最短的时间里帮助您建立起一套可用的服务器环境，只需略加调整就可以应用到实际项目当中。 
本教程分为以下几个部分，为了说明简单，教程是在windows下使用的方式，以方便资源有限的项目使用。在UNIX环境下，只是安装方式不同，命令执行并无太大区别。
- 软件下载
- 服务器和客户端安装
- 建立版本库（Repository） 
- 配置用户和权限
- 运行独立服务器 
- 初始化导入 
- 基本客户端操作
# 1，软件下载 
### 下载Subversion服务器程序。 
到[官方网站](http://subversion.tigris.org/)的下载二进制安装文件，来到[二进制包下载部分](http://subversion.tigris.org/getting.html#binary-packages)，找到 Windows NT, 2000, XP and 2003部分，然后选择[Apache
 2.0](http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=91) 或者 [Apache 2.2](http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=8100)，这样我们可以看到许多下载的内容，目前可以下载[Setup-Subversion-1.5.3.msi](http://subversion.tigris.org/files/documents/15/44049/Setup-Subversion-1.5.3.msi) 。
### 下载Subversion的Windows客户端TortoiseSVN。 
TortoiseSVN是扩展Windows Shell的一套工具，可以看作Windows资源管理器的插件，安装之后Windows就可以识别Subversion的工作目录。
官方网站是[TortoiseSVN](http://tortoisesvn.net/)，下载方式和前面的svn服务器类似，在[Download](http://tortoisesvn.net/downloads)页面的我们可以选择下载的版本，目前的最高稳定版本的安装文件为[TortoiseSVN-1.5.5.14361-win32-svn-1.5.4.msi](http://downloads.sourceforge.net/tortoisesvn/TortoiseSVN-1.5.5.14361-win32-svn-1.5.4.msi?download)。
# 2，服务器和客户端安装 
服务器安装，直接运行[Setup-Subversion-1.5.3.msi](http://subversion.tigris.org/files/documents/15/44049/Setup-Subversion-1.5.3.msi) ，根据提示安装即可，这样我们就有了一套服务器可以运行的环境。
安装TortoiseSVN，同样直接运行[TortoiseSVN-1.5.5.14361-win32-svn-1.5.4.msi](http://downloads.sourceforge.net/tortoisesvn/TortoiseSVN-1.5.5.14361-win32-svn-1.5.4.msi?download)按照提示安装即可，不过最后完成后会提示是否重启，其实重启只是使svn工作拷贝在windows中的特殊样式生效，与所有的实际功能无关，这里为了立刻看到好的效果，还是重新启动机器。
# 3，建立版本库（Repository）
运行Subversion服务器需要首先要建立一个版本库（Repository），可以看作服务器上存放数据的数据库，在安装了Subversion服务器之后，可以直接运行，如：
svnadmin create E:\svndemo\repository
就会在目录E:\svndemo\repository下创建一个版本库。 
我们也可以使用TortoiseSVN图形化的完成这一步： 
在目录E:\svndemo\repository下"右键->TortoiseSVN->Create Repository here...“， 然后可以选择版本库模式， 这里使用默认即可， 然后就创建了一系列目录和文件。
# 4，配置用户和权限 
来到E:\svndemo\repository\conf目录，修改svnserve.conf： 
# [general] 
# password-db = passwd 
改为： 
[general] 
password-db = passwd 
然后修改同目录的passwd文件，去掉下面三行的注释： 
# [users] 
# harry = harryssecret 
# sally = sallyssecret 
最后变成： 
[users] 
harry = harryssecret 
sally = sallyssecret 
 在passwd文件中，“=”前的字符就是用户名，后面的就是密码。还要注意“[users]”前面的注释“#”一定要删除掉。
# 5，运行独立服务器 
在任意目录下运行： 
svnserve -d -r E:\svndemo\repository 我们的服务器程序就已经启动了。注意不要关闭命令行窗口，关闭窗口也会把svnserve停止。
# 6，初始化导入 
来到我们想要导入的项目根目录，在这个例子里是E:\svndemo\initproject，目录下有一个readme.txt文件： 
右键->TortoiseSVN->Import... 
在URL of repository输入“svn://localhost/trunk” 
在Import Message中输入你的日志信息
完成之后目录没有任何变化，如果没有报错，数据就已经全部导入到了我们刚才定义的版本库中。 
需要注意的是，这一步操作可以完全在另一台安装了TortoiseSVN的主机上进行。例如运行svnserve的主机的IP是133.96.121.22，则URL部分输入的内容就是“svn://133.96.121.22/trunk”。
# 7，基本客户端操作 
取出版本库到一个工作拷贝： 
来到任意空目录下，在本例中是E:\svndemo\wc1，运行右键->Checkout，在URL of repository中输入svn://localhost/trunk，这样我们就得到了一份工作拷贝。 
在工作拷贝中作出修改并提交： 
打开readme.txt，作出修改，然后右键->Commit...，这样我们就把修改提交到了版本库，我们可以运行。 
察看所作的修改： 
readme.txt上右键->TortoiseSVN->Show Log，这样我们就可以看到我们对这个文件所有的提交。在版本1上右键->Compare with working copy，我们可以比较工作拷贝的文件和版本1的区别
