
# android adt升级到版本22后，就无法再向lib库里面添加jar了 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月19日 18:21:38[Jlins](https://me.csdn.net/dyllove98)阅读数：1518标签：[android																](https://so.csdn.net/so/search/s.do?q=android&t=blog)个人分类：[android																](https://blog.csdn.net/dyllove98/article/category/1130032)


目前有一个android app 大项目，引用了很多的类库，一直运行的好好的，今天我把Eclipse ADT插件升级到了最新版本（v22）,同时把sdl也升级了。在整个
升级过程中很好，没有任何问题，当我达成apk在手机上运行时，报错了NoClassDefFoundError
java.lang.NoClassDefFoundError：org.acra.ACRA
....
该ARCA库一斤包含到了lib库里面了，我能在Android Private Libraries 中看到，并且编译错误。在其他人没有升级adt电脑，运行良好.
我已经尝试了一大堆东西，包括但不限于：
1、重新安装Android SDK
2、下载一个新的ADT包
3、删除我所有的代码再重新编译
4、有关复制库的应用程序项目
5、注释掉使用这个库中的代码
求救了....
Android的Eclipse的ADT

### 处理方法

在升级时，新的“Android Private Libraries”的 Order and Export“并非必检项目。,android-support-v4.jar已经在'Android Private Libraries' 中
要解决此问题，请转到'Order and Export'”，并勾选'Android Private Libraries'。然后refresh/clean/rebuild.
当你这样做'修复'的库项目，您可能需要只是关闭并重新打开工程


原文地址：[http://www.itmmd.com/201410/76.html ](http://www.itmmd.com/201410/76.html)
该文章由[萌萌的IT人](http://www.itmmd.com/)整理发布,转载须标明出处。

