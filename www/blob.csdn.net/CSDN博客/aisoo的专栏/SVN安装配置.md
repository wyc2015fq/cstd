# SVN安装配置 - aisoo的专栏 - CSDN博客
2009年11月09日 19:02:00[aisoo](https://me.csdn.net/aisoo)阅读数：502标签：[svn																[tortoisesvn																[subversion																[服务器																[文本编辑																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=文本编辑&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=subversion&t=blog)](https://so.csdn.net/so/search/s.do?q=tortoisesvn&t=blog)](https://so.csdn.net/so/search/s.do?q=svn&t=blog)
SVN是CVS的升级版，功能完善了很多，又是开源的。下面对SVN的安装及配置进行详细说明（基于Windows 平台）：
一、准备工作 
1、获取 Subversion 服务器程序 
到官方网站（http://subversion.tigris.org/）下载最新的服务器安装程序。目前最新的是1.5.1版本，具体下载地址在：http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=8100&expandFolder=8100&folderID=91 ，注意找for apache 2.2.x 版本的。 
2、获取 TortoiseSVN 客户端程序 
从官方网站 http://tortoisesvn.net/downloads 获取最新的TortoiseSVN 。TortoiseSVN 是一个客户端程序，用来与 subversion 服务器端通讯。Subversion 自带一个客户端程序 svn.exe ,但 TortoiseSVN 更好操作，提高效率。 
二、安装服务器端和客户端 
首先安装 Subversion 服务器程序,其次安装 TortoiseSVN 客户端程序。下载下来的服务器端是个 zip压缩包，直接解压缩即可，比如我解压到 E:/subversion 。客户端安装文件是个 exe 可执行文件，直接运行按提示安装即可，客户端安装完成后提示重启。
三、建立版本库（Repository）
   1  先建立空目录 e:/Repository，注意一定是要空的。然后在 Repository 文件夹上“右键->TortoiseSVN->Create Repository here...”，就创建了一系列文件夹和文件。
  2  配置svn服务成window service自动运行的工具: 
    SVNService.rar（[http://www.cnblogs.com/Files/changchangcc520/SVNService.rar](http://www.cnblogs.com/Files/changchangcc520/SVNService.rar)）
  3．安装SVN服务，因为WINDOWS默认是没有的，那就要自己安装，把SVNService解压到SVN安装文件的BIN目录下，在命令行中输入：
        svnservice -install -d -r e:/Repository
然后再启动SVN服务，输入：
        net start svnservice，
        SVN在默认情况下是不会自动启动的，需要到控制面板管理下的服务中将SVNSERVER设为自动启动
四、配置用户和权限 
用文本编辑器打开‘e:/Repository/conf’目录，修改svnserve.conf： 
password-db = passwd
anon-access = read
auth-access = write 
即去掉前面的 # 注释符，注意前面不能有空格。 
然后修改同目录的passwd文件，增加一个帐号： 
将： 
[users] 
# harry = harryssecret 
# sally = sallyssecret 
修改后为： 
[users] 
#harry = harryssecret 
#sally = sallyssecret 
test = test 
五、初始化导入 
下面就是将我们的数据（项目）导入到这个版本库，以后就由版本库管理我们的数据。我们的任何改动都回被版本库记录下来，甚至我们自己丢失、改错数据时版本库也能帮我们找回数据。 
比如，我在 d:/wwwroot 下有个 guestbook 文件夹，里面存放的是我编写的留言簿程序。在此文件夹上“右键 -> TortoiseSVN -> Import...” ，在弹出对话框的“URL of repository”输入“svn://localhost”。在“Import message”输入“导入整个留言簿”作为注释。 
点 OK 后要求输入帐号。我们在用户名和密码处都输入 test 。完成后 guestbook 中的内容全部导入到了 svn://localhost 。
