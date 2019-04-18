# ZXing版本问题 - z69183787的专栏 - CSDN博客
2015年08月18日 14:27:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5138
、因为使用的是最新版本，没有留意到最新版本的JDK依赖至少是1.7+，在运行的时候报：
Exception in thread "AWT-EventQueue-0" java.lang.UnsupportedClassVersionError: 
com/google/zxing/WriterException : Unsupported major.minor version 51.0
2、在把版本切回到2.2就可以了。
好吧，好丑的界面：
![](https://img-blog.csdn.net/20150213093447006?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU2V2ZW5fY20=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
