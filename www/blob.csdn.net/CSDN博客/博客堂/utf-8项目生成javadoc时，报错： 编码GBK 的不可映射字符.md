# utf-8项目生成javadoc时，报错： 编码GBK 的不可映射字符 - 博客堂 - CSDN博客





2017年02月19日 22:32:12[最小森林](https://me.csdn.net/u012052268)阅读数：2595










# [utf-8项目生成javadoc时，报错： 编码GBK 的不可映射字符](http://blog.csdn.net/u012052268/article/details/55826345)










这是因为：由于JDK是国际版的，在编译的时候，如果我们没有用-encoding参数指定我们的[Java](http://lib.csdn.net/base/javase)源程序的编码格式，则javac.exe首先获得我们[操作系统](http://lib.csdn.net/base/operatingsystem)默认采用的编码格式，也即在编译java程序时，若我们不指定源程序文件的编码格式，JDK首先获得操作系统的file.encoding参数(它保存的就是操作系统默认的编码格式，如WIN2k，它的值为GBK)，然后JDK就把我们的java源程序从file.encoding编码格式转化为JAVA内部默认的UNICODE格式放入内存中。然后，javac把转换后的unicode格式的文件进行编译成.class类文件，此时.class文件是UNICODE编码的，它暂放在内存中，紧接着，JDK将此以UNICODE编码的编译后的class文件保存到我们的操作系统中形成我们见到的.class文件。对我们来说，我们最终获得的.class文件是内容以UNICODE编码格式保存的类文件，它内部包含我们源程序中的中文字符串，只不过此时它己经由file.encoding格式转化为UNICODE格式了。当我们不加设置就编译时，相当于使用了参数：javac
 -encoding gbk XX.java，当然就会出现不兼容的情况。




解决办法：

打开eclipse，右键点击project -> Generate javadoc 一项一项的选你要输出javadoc的项目，

最后一步中VM设置行中加入以下代码


Java代码 







-encoding utf-8 -charset utf-8


![](https://img-blog.csdn.net/20170219223143390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjA1MjI2OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




