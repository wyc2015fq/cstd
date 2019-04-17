# 平台调用之如何利用VS2013 C#调试C++DLL库 - DoubleLi - 博客园






对于托管代码调用[非托管DLL](http://blog.csdn.net/gggg_ggg/article/details/50899495)文件，已经是非常普遍的事情，下面写一下如何通过托管代码(C#)像调试托管代码一样调试DLL中的代码。







注意：(1)【[dll工程](http://blog.csdn.net/gggg_ggg/article/details/50899495)和调用dll的exe工程需要在同一个解决方案中】

           (2) 64位与32位生成的路径不一样，这个在设置路径的时候，一定要千万小心。否则可能会导致32位运行正常，当转换为64位的时候，失败的情况。


调试方法：
(1) C++ dll工程：属性-配置属性-调试-把 命令 为 调用该dll的exe工程的bin\Debug\*.exe，
(2) C++ dll工程：属性-配置属性-调试-把 调试器类型 设置为 仅限本机 如图：
![](https://img-blog.csdn.net/20160407150820024?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



(3)C++ dll工程：属性-配置属性-常规-把输出目录设置为 调用该dll的exe工程的bin\Debug目录下，如图：

![](https://img-blog.csdn.net/20160407150843024?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


(4)在dll工程中打好断点，然后重新生成[dll工程](http://blog.csdn.net/gggg_ggg/article/details/50899495)，为了输出dll到设置的目录下

然后




对于exe工程：

(1)属性-调试-  将启动[本机代码](http://write.blog.csdn.net/postedit/51086089?ticket=ST-26956-1m9F57eaUemK7KInMqhU-passport.csdn.net)调试选项打上勾，如图所示：

![](https://img-blog.csdn.net/20160407150911743?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


(2)设置为启动项目，就可以进行[断点调试](http://blog.csdn.net/gggg_ggg/article/details/50899495)了









