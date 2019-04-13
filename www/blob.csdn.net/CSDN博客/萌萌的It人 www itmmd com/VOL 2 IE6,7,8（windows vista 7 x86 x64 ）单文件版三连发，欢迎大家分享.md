
# VOL.2 IE6,7,8（windows vista-7 x86-x64 ）单文件版三连发，欢迎大家分享 - 萌萌的It人 www.itmmd.com - CSDN博客


2013年08月07日 18:08:23[Jlins](https://me.csdn.net/dyllove98)阅读数：4981


在上期[
VOL.1 利用vmware ThinApp 制作非XP下可以运行的IE6 【无插件版】（windows vista/7/8  x86/x64 ）](http://www.cnblogs.com/AzureBlog/p/3232062.html)中，简要介绍了如何利用vmware Thinapp制作单文件版IE6，根据大家的需求，我又做了2个版本的单文件版，这里打包到一起，供网友们下载，**欢迎各位分享给你的同学，同事或同行们**。![](https://img-blog.csdn.net/20130807180805140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)（windows
 8下的问题正在解决中。。。）
**下载地址：**
[http://pan.baidu.com/share/link?shareid=1495689163&uk=3424911262](http://pan.baidu.com/share/link?shareid=1495689163&uk=3424911262)
密码：azure
**如果运行出错，或者有什么更好的建议，请留言~**
解压之后是这么几个文件
![](https://img-blog.csdn.net/20130807180806468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果你想自己试一试抽出IE7，8的话，可以看下面的一些提示。本以为IE7，8会和6一样简单，但是过程并不顺利，重启了几十次（大家想想看，安装要重启，卸载要重启，过程中IE7拆装了不下10遍![](https://img-blog.csdn.net/20130807180807125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)）。后来查阅了一下官方文档[Virtualizing
 Internet Explorer 7 or 8 with ThinApp (1026674) ](http://kb.vmware.com/selfservice/microsites/search.do?cmd=displayKC&docType=kc&externalId=1026674&sliceId=1&docTypeID=DT_KB_1_1&dialogID=5876608&stateId=1%200%205886859)，最后成功了。主要流程：
**原文：**
### To Capture Internet Explorer 7 or Internet Explorer 8:
**Note**: The supported method of capturing the upgrade to a new version of Internet Explorer is to put the new Internet Explorer 7 or 8 installer on the desktop before the prescan and leave it there until after the postscan (see Additional
 Information section below).
Using a Windows XP capture machine:
Prescan.
Begin the Setup Capture.
Note: Do not select theInternet Explorerbutton on the Setup Capture application. This feature only works for capturing Internet Explorer 6. (If you want to capture Internet Explorer 6, see[
Virtualizing Internet Explorer 6 with ThinApp 4.6 (1026565)](http://www.cnblogs.com/AzureBlog/admin/search.do?cmd=displayKC&docType=kc&docTypeID=DT_KB_1_1&externalId=1026565).)
During the application installation phase of Setup Capture, capture the upgrade from Internet Explorer 6 to the version of Internet Explorer you are virtualizing (IE7 or IE8).
Configure the browser by opening the newly installed native Internet Explorer.
Postscan.
Configure the project and package in the subsequent Setup Capture windows.
When the Native Browser Redirection window appears in Setup Capture, you have the option of creating aThinDirect.txtfile. This file allows you to implement ThinDirect redirection of web pages from native Internet Explorer to the virtual Internet Explorer browser.
For more information on setting up redirection with a ThinDirect.txt file, see:[Using ThinApp ThinDirect to Set Up Redirection from the Native Internet Explorer
 to a Virtual Browser Such As Virtual Internet Explorer 6 (Virt IE6) (1026566)](http://www.cnblogs.com/AzureBlog/admin/search.do?cmd=displayKC&docType=kc&docTypeID=DT_KB_1_1&externalId=1026566)
[ThinDirect.txt Syntax for Redirection from Native Internet Explorer to a Virtual
 Browser Such As Virtual Internet Explorer 6 (1026635)](http://www.cnblogs.com/AzureBlog/admin/search.do?cmd=displayKC&docType=kc&docTypeID=DT_KB_1_1&externalId=1026635)
[Installation and Registration of a Virtual Browser That Includes ThinApp ThinDirect
 to Redirect Web Pages from Native Internet Explorer to a Virtual Browser Like Internet Explorer 6 (1026636)](http://www.cnblogs.com/AzureBlog/admin/search.do?cmd=displayKC&docType=kc&docTypeID=DT_KB_1_1&externalId=1026636)
Build.
**简要翻译：**
**捕获IE7，IE8**
**注意：**IE版本的升级需要在thinapp prescan（预扫描）之前，将安装文件放到桌面上（重要）
流程：
1.prescan(预扫描)
2.开始捕获
![](https://img-blog.csdn.net/20130807180810109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHlsbG92ZTk4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里注意选择 don't include an Internet Explorer entry point（截图下面第三项灰掉了是因为我已经升级了IE，大家操作的时候选择第一项就对了）
3.开始安装（升级）
**注意****：**这里有一点，原文没有指出来，IE装完了之后是要重启的，尽管重启就是了，重启完之后选择第一项continue install （继续安装）。
4.设置一下升级之后的IE
5.postscan
6.设置调整
7.修改配置文件
8.编译
**过程和之前都差不多，注意描红的部分。GOOD LUCK~**



