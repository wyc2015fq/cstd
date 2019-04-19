# 在Eclipse中调试运行launcher源码之系列文章 - xqhrs232的专栏 - CSDN博客
2012年02月09日 14:18:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1568
 1。文章一
没有Launcher2.2源码的看这儿： 
在Eclipse中调试运行launcher源码（一）——获取launcher源码
[http://gqdy365.iteye.com/blog/737642](http://gqdy365.iteye.com/blog/737642)
2。文章二
在Eclipse中调试运行launcher源码（二）——向Eclipse中导入Launcher源码
[http://gqdy365.iteye.com/blog/737645](http://gqdy365.iteye.com/blog/737645)
3。文章三
在Eclipse中调试运行launcher源码（三）——修改工程的包名
[http://gqdy365.iteye.com/blog/738679](http://gqdy365.iteye.com/blog/738679)
4。文章四第一部分
在Eclipse中调试运行launcher源码（四）——修改所有类中的错误_1
[http://gqdy365.iteye.com/blog/763543](http://gqdy365.iteye.com/blog/763543)
5。文章四第二部分
在Eclipse中调试运行launcher源码（四）——修改所有类中的错误_2
[http://gqdy365.iteye.com/blog/768567](http://gqdy365.iteye.com/blog/768567)
6。文章五
在Eclipse中调试运行launcher源码（五）——运行自己修改的Launcher
[http://gqdy365.iteye.com/blog/788647](http://gqdy365.iteye.com/blog/788647)
//=========================================================
备注:
1>launcher工程用到了几个JAR文件----framework/core/android-common/android16-2----要知道在那个地方去找到对应的JAR文件，并怎么让工程引用到
1，准备好编译后的Android源码。
2，在该源码的out目录下寻找包含你所用隐藏类的jar文件，通常文件名为classes.jar。----如果用2.3的.classpath/.project/default.properties就几乎不需要下面的JAR文件
   a>framework的jar文件为out\target\common\obj\JAVA_LIBRARIES\framework_intermediates\classes.jar
   b>core的jar文件为out\target\common\obj\JAVA_LIBRARIES\core_intermediates\classes.jar
   c>android-common的jar文件为out\target\common\obj\JAVA_LIBRARIES\android-common_intermediates\classes.jar
   d>android16_2的jar文件就不清楚了，好像不需要
3，在eclipse的Android项目中，选择项目属性->Java Build Path->Libraries->Add Library->User Library->Next-> UserLibraries进入到User Libraries管理界面，点击New新建一个User Library，比如android_framework，点击Add Jars把Jar包加入到建立的User Library中，最后点击OK就可以了。
 2>[ANDROID Installation error: INSTALL_FAILED_CONFLICTING_PROVIDER](http://blog.csdn.net/sfezst/article/details/6295142)----[http://blog.csdn.net/sfezst/article/details/6295142](http://blog.csdn.net/sfezst/article/details/6295142)
今天在测试一个系统自带的应用的时候，用adb安装，结果出现[INSTALL_FAILED_CONFLICTING_PROVIDER]
后来改了AndroidMainfest.xml中的provider的名字就OK了。
3>怎么修改包名，因为系统里面已经有一个对应的包名了
 看来AndroidManifest.xml移进ECLIPSE会发生很多不明白的变化，这些变化会造成编译出错！！！
就是选择包名按F2修改包名后就会出这样的状况！！！还是GEN下的包名按F2进行修改。
4>launcher工程导入到ECLIPSE IDE里面要注意使用的.classpath/.project/default.properties文件的版本是2.3的还是2.2的
