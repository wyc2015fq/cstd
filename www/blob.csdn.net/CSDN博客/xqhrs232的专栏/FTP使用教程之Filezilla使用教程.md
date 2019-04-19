# FTP使用教程之Filezilla使用教程 - xqhrs232的专栏 - CSDN博客
2016年03月23日 10:45:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：22920
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址：[http://www.yiweihy.com/knowledge/used/215-ftp-filezilla.html](http://www.yiweihy.com/knowledge/used/215-ftp-filezilla.html)
相关文章
1、FileZilla——百度百科----[http://baike.baidu.com/link?url=UgMpTWpKwnlZx3bFmucxEUuBGceXZ7xKsMMAnHA_QH7dEIA9bD6ZZJCLXVvppnPBpHG27ajpklcR3s_g4g1oBK](http://baike.baidu.com/link?url=UgMpTWpKwnlZx3bFmucxEUuBGceXZ7xKsMMAnHA_QH7dEIA9bD6ZZJCLXVvppnPBpHG27ajpklcR3s_g4g1oBK)
2、怎么使用FileZilla上传和下载文件----[http://jingyan.baidu.com/article/0f5fb099e385206d8334eacc.html](http://jingyan.baidu.com/article/0f5fb099e385206d8334eacc.html)
3、filezilla如何使用----[http://jingyan.baidu.com/article/4dc408488466b8c8d846f177.html](http://jingyan.baidu.com/article/4dc408488466b8c8d846f177.html)
FileZilla是一个免费开源的FTP客户端软件，功能齐全易于使用，本文只要介绍如何使用FileZilla上传文件到服务器。如果你不清楚为什么使用FTP可以参考另一篇网文：[如何自己去换网页上的图片](http://www.yiweihy.com/knowledge/used/208-rhhuantu.html)。
## [1. 下载并安装FileZilla](http://www.yiweihy.com/knowledge/used/215-ftp-filezilla.html#art1)
访问FileZilla官网：http://filezilla-project.org/，可以看不到下载两个下载链接：Download FileZilla Client  和 Download FileZilla Server 我们需要的是客户端所以选择： “**Download FileZilla Client**” 进入下载页面。见下图：
![FileZilla下载](http://www.yiweihy.com/images/articles/knowledge/used/filezilla_download.jpg)
FileZilla下载
再点击 FileZilla_3.6.0.2_win32.zip 下载windows下使用版本
![FillaZilla 客户端 Windows版](http://www.yiweihy.com/images/articles/knowledge/used/filezilla_download2.jpg)
FillaZilla 客户端 Windows版
 下载解压后点击其中的exe文件就可以直接使用了，FileZilla是不需要安装的。
## [2. Filezilla面板介绍](http://www.yiweihy.com/knowledge/used/215-ftp-filezilla.html#art2)
 打开FileZilla后可见以下面板：
![FileZilla面板](http://www.yiweihy.com/images/articles/knowledge/used/filezilla_panel1.jpg)
FileZilla面板
### 1. 站点管理器
保存各个FTP站点，不用每次都输入用户名、密码等信息。
### 2. 本地文件夹
指向本地文件路径，可以在文件上右键 “上传” 来上传文件。（也可以直接拖拽到远程文件夹中上传）
### 3.远程文件夹
服务器端文件夹，可以将文件拖拽到本地实现FTP下载。
## [3. 快速链接服务器并上传文件](http://www.yiweihy.com/knowledge/used/215-ftp-filezilla.html#art3)
在工具按钮下边有快速链接栏，可以不使用站点管理器链接到服务器上传文件。见下图：
![FileZilla快速链接服务器](http://www.yiweihy.com/images/articles/knowledge/used/filezilla_fast.jpg)
快速链接服务器
- 
主机（H）：主机名，一般使用IP地址即可。
- 
用户名（U）：FTP的用户名。
- 
密码（W）：FTP的密码。
- 
端口（P）： 端口号，如果没有特别之处默认不用填写。
填完链接参数后，点击“快速链接”按钮就可以链接到服务器了，链接成功后可以看到如下图说明：
![FileZilla链接成功](http://www.yiweihy.com/images/articles/knowledge/used/filezilla-connect.jpg)
链接成功
在状态栏显示IP地址，和用户名，在远程站点路径部分，会列出远程服务器的文件夹列表。
注：如果未链接成功，一般是用户名、密码、主机IP等填写错误。
## [4.FTP使用注意事项](http://www.yiweihy.com/knowledge/used/215-ftp-filezilla.html#art4)
- 
FTP替换文件时要注意在本地备份一下，以免替换了错误时可以及时恢复。
- 
不同于后台上传文件，如果替换掉了程序文件，可能造成无法挽回的错误。
- 
上传之前，可以对要上传的文件进行病毒扫描，以免上传木马或者病毒。
