# windows下pip安装库遇到了Microsoft Visual C++ 9.0 is required的问题 - 一个跳popping的quant的博客 - CSDN博客





2017年09月24日 13:21:46[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：2158
所属专栏：[学python的路上](https://blog.csdn.net/column/details/20561.html)









今天在通过pip安装BaseMap库的时候，遇到了Microsoft Visual C++ 9.0 is required的问题，一看原因是python在编译一些通过c编写的库的时候需要用到c的编译器，去就网上找了一下，还好Microsoft提供了vcforpython27的编译器，并附上了网址，[http://aka.ms/vcpython27](http://aka.ms/vcpython27)但是进不去呐！！！！ 

   于是就又在网上找了半天终于找到了个microsoft的安装包，下载了直接安装上就好了，![这里写图片描述](https://img-blog.csdn.net/20170924132024845?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

   vcforpython27.msi上传到了我的百度云上去，需要的可以去下载： 

   链接：[http://pan.baidu.com/s/1nuCrCff](http://pan.baidu.com/s/1nuCrCff) 密码：ood4



