# WINCE5.0打ARMV4I补丁问题 - xqhrs232的专栏 - CSDN博客
2014年02月21日 23:38:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：961
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.chinaunix.net/uid-21696322-id-430821.html](http://blog.chinaunix.net/uid-21696322-id-430821.html)
按照开发板公司提供的文档安装winCE开发环境
安装成功以进行到打ARMV4I补丁这一步：
---------------------------------------------------------------------------
Step10：安装WINCE5.0补丁文件 
请安装微软07年的Armv4I补丁，注意：WINCE5.0默认安装以后是没有.NET Framework2.0
组件的，一定要打上补丁才有。 
补丁下载页面： 
[http://www.microsoft.com/downloads/details.aspx?familyid=A54779D5-F4A5-49F0](http://www.microsoft.com/downloads/details.aspx?familyid=A54779D5-F4A5-49F0)-
9E36-979D461F536C&displaylang=en
只需下载WinCEPB50-071231-Product-Update-Rollup-Armv4I.msi文件就可以。
---------------------------------------------------------------------------
打开网址后提示
对不起！找不到您请求的页面
然后Google搜索WinCEPB50-071231-Product-Update-Rollup-Armv4I.msi
找到这个网页
[http://bbs.driverdevelop.com/htm_data/48/0903/115871.html](http://bbs.driverdevelop.com/htm_data/48/0903/115871.html)
在论坛的回复中找到了微软官方对于WINCE更新的网址
[http://msdn.microsoft.com/en-us/embedded/aa731256.aspx](http://msdn.microsoft.com/en-us/embedded/aa731256.aspx)
找到了2009年5月对于WINCE5.0的更新
网址如下
[http://www.microsoft.com/downloads/details.aspx?displaylang=en&FamilyID=5236be51-805a-4ee3-8b5d-dae557c70ce0](http://www.microsoft.com/downloads/details.aspx?displaylang=en&FamilyID=5236be51-805a-4ee3-8b5d-dae557c70ce0)
选择下载这个文件
WinCEPB50-090531-2009M05-Armv4I.msi
![](http://blogimg.chinaunix.net/blog/upfile2/090612123509.jpg)
![](http://blogimg.chinaunix.net/blog/upfile2/090612123524.jpg)
就在我要下载的时候
我无意中打开WINCE5.0源程序的文件夹
发现在update这个子文件夹下面就有我们所需要的这个文件大小是230MB
所以我就取消了下载
双击WinCEPB50-071231-Product-Update-Rollup-Armv4I文件进行更新
更新大约进行了15分钟
完成后弹出对话框
![](http://blogimg.chinaunix.net/blog/upfile2/090612131253.jpg)
点击确定后弹出html文件D:\WINCE500\Updates\Windows CE5.0_Product_Update_Rollup_2007.htm
和如下窗口
![](http://blogimg.chinaunix.net/blog/upfile2/090612131504.jpg)
