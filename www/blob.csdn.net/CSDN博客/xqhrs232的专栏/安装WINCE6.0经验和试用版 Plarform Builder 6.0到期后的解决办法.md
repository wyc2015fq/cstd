# 安装WINCE6.0经验和试用版 Plarform Builder 6.0到期后的解决办法 - xqhrs232的专栏 - CSDN博客
2014年08月11日 14:16:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1338
原文地址::[http://www.cnblogs.com/Torres_fans/archive/2010/03/29/1699743.html](http://www.cnblogs.com/Torres_fans/archive/2010/03/29/1699743.html)
相关文章
1、[WINCE6.0经验和试用版
 Plarform Builder 6.0到期后的解决办法](http://blog.csdn.net/tidavery/article/details/6180001)----[http://blog.csdn.net/tidavery/article/details/6180001](http://blog.csdn.net/tidavery/article/details/6180001)
2、
# VS2005 建立工程是出现 thank you for choosing to evaluate platform builder your 180 day evalution---[http://zhidao.baidu.com/link?url=4sxeWeO4A4X4zoFmy6SzYho1LAC1_xqn2FbOR7bbbWASB5WStvDHZmLl8HVRDDh88lzZiEM-y_YBrJgy2WzlDq](http://zhidao.baidu.com/link?url=4sxeWeO4A4X4zoFmy6SzYho1LAC1_xqn2FbOR7bbbWASB5WStvDHZmLl8HVRDDh88lzZiEM-y_YBrJgy2WzlDq)
今天师妹的WinCE6.0 Platform Builder过期了，打开工程提示已超过180天，问我，我以前只是关注了一下，知道有人问过类似的问题，但是具体怎么做也说不上来。只好google了一下，找到了解决办法：
把gooogleman大侠提供的方法贴出来与大家共享之:
"在PB安装目录下有个etkremov.inf，试用期是以此文件的创建日期来计算的。将其删除，然后新建一个文件，再重命名为 etkremov.inf，不要直接新建这个文件，否则由于filesystem tunnel，会导致新建的文件总是沿用老文件的创建时间"
又看了一下何老师的空间才发现方法类似，于是也贴上来：
## [试用版 Plarform Builder 6.0到期后的解决办法](http://www.cnblogs.com/we-hjb/archive/2010/01/20/1652226.html)
      a)删除目录C:\Program Files\Microsoft Platform Builder\6.00下的EtkRemov.inf文件
      b)新建一个文本文档
      c)重命名新建文本文档为EtkRemov.inf
      d)重新Make Runtime Image试一下，应该没有问题了
最后分享一个小经验：
一方面，可以在安装的时候备份，也就是我采用的方法。
另一方面，网友[wowocpp](http://blog.163.com/wowocpp/)提供了一个VS2005的正版的序列号，具体情况如下：
[分享]安装WINCE6.0经验
最近安装了WINCE6.0,把经验根大家分享一下。
(1)下载 VS2005，不用我说，大家已经知道WINCE6.0已经与VS2005结合到一块儿了。所以必须安装VS2005。以下是破解方法：
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
VS2005破解180天限制
在控制面板的“添加或删除程序” 中选择“更改/删除”Microsoft Visual Studio 2005，然后输入正式版的序列号（ KYTYH-TQKW6-VWPBQ-DKC8F-HWC4J ），就可以把 VS2005 试用版升级为正式版（破解180天试用限制）
Visual Studio 2005注册升级
可升级正式版key：KYTYH-TQKW6-VWPBQ- DKC8F-HWC4J
找到SETUP文件夹下的setup.sdb,用记事本打 开它，找到[Product Key]，将下面的一行序列号删除，改为正式Key，保存后再安装就是正式版了，记住，中间没有横线!!
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
我采用的是Team Suit版本的VS2005。
（2）下载VS2005的SP1，如果VS2005装的是中文版，就必须使用中文版的SP1。如果 是英文版，就装英文版（废话...）
下载地址：[http://www.microsoft.com/downloads/details.aspx?displaylang=zh-cn&FamilyID=bb4a75ab-e2d4-4c96-b39d-37baf6b5b1dc](http://www.microsoft.com/downloads/details.aspx?displaylang=zh-cn&FamilyID=bb4a75ab-e2d4-4c96-b39d-37baf6b5b1dc)
（3）下载WINCE6.0，网上有的下载，大概3G多；也可以从微软下载一个 setup.EXE，执行后会边下载边安装。
如果网上下载安装，一定要使 用.msi文件安装，不要用setup.exe安装。
另外注意，在这个安装中要指 定好安装的目录，否则会装在C盘下。
微软的地址：[https://www.microsoft.com/downloads/details.aspx?familyid=7E286847-6E06-4A0C-8CAC-CA7D4C09CB56&displaylang=en](https://www.microsoft.com/downloads/details.aspx?familyid=7E286847-6E06-4A0C-8CAC-CA7D4C09CB56&displaylang=en)
注册的序列号：H8RQR-MMKRP-XFRFC-9HKGJ-82R6J
