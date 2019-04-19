# 安装vs产生的垃圾文件（eula.1028.txt,install.res.1028.dll,VC_RED.MSI等） - 三少GG - CSDN博客
2013年02月21日 15:34:35[三少GG](https://me.csdn.net/scut1135)阅读数：6500

### [安装vs产生的垃圾文件（eula.1028.txt,install.res.1028.dll,VC_RED.MSI等）](http://blog.csdn.net/swq0553/article/details/6731307)
分类： [杂谈](http://blog.csdn.net/swq0553/article/category/739719)
[dll](http://blog.csdn.net/tag/details.html?tag=dll)[2010](http://blog.csdn.net/tag/details.html?tag=2010)[微软](http://blog.csdn.net/tag/details.html?tag=%E5%BE%AE%E8%BD%AF)[ini](http://blog.csdn.net/tag/details.html?tag=ini)[c](http://blog.csdn.net/tag/details.html?tag=c)
安装vs2008、2010时候会在E盘或F盘或其他盘符下产生产生一系列的txt、dll、ini、等文件如
9个[文本文件](http://wenwen.soso.com/z/Search.e?sp=S%E6%96%87%E6%9C%AC%E6%96%87%E4%BB%B6&ch=w.search.yjjlink&cid=w.search.yjjlink) 名字分别为 
eula.1028.txt ， 
eula.1031.txt ， 
eula.1033.txt ， 
eula.1036.txt ， 
eula.1040.txt ， 
eula.1041.txt ， 
eula.1042.txt ， 
eula.2052.txt ， 
eula.3082.txt 。 
九个.dll档 名字分别为 
install.res.1028.dll , 
install.res.1031.dll , 
install.res.1033.dll , 
install.res.1036.dll , 
install.res.1040.dll , 
install.res.1041.dll , 
install.res.1042.dll , 
install.res.2052.dll , 
install.res.3082.dll . 
两个.[ini文件](http://wenwen.soso.com/z/Search.e?sp=Sini%E6%96%87%E4%BB%B6&ch=w.search.yjjlink&cid=w.search.yjjlink)，名字分别为 
globdata.ini ， 
install.ini 。 
一个.exe文件 名字为 
install.exe 
一个.[cab文件](http://wenwen.soso.com/z/Search.e?sp=Scab%E6%96%87%E4%BB%B6&ch=w.search.yjjlink&cid=w.search.yjjlink) 名字为 
VC_RED.cab 
一个.MSI文件 
VC_RED.MSI 
一个BMP图象 名字为 
vcredist.bmp
这些文件是VS2008（VS2010）再发行[组件](http://wenwen.soso.com/z/Search.e?sp=S%E7%BB%84%E4%BB%B6&ch=w.search.yjjlink&cid=w.search.yjjlink)包安装时产生的[临时文件](http://wenwen.soso.com/z/Search.e?sp=S%E4%B8%B4%E6%97%B6%E6%96%87%E4%BB%B6&ch=w.search.yjjlink&cid=w.search.yjjlink),但这个软件却因为bug问题而错误的将临时文件放到了C盘更目录(本应该是temp目录并会被自动删除的,并且这个bug是[微软](http://wenwen.soso.com/z/Search.e?sp=S%E5%BE%AE%E8%BD%AF&ch=w.search.yjjlink&cid=w.search.yjjlink)已知的bug)
那么这些文件可以手动删除
VC2008安装后遗留的文件？他会选择剩余空间最大的磁盘~
这是老版本的VC2008才有的吧？新的貌似没有这些垃圾了……
