# 用intellij idea调试  CAS 5.1.8 - 左直拳的马桶_日用桶 - CSDN博客
2018年02月28日 19:20:54[左直拳](https://me.csdn.net/leftfist)阅读数：1494
能够[用intellij idea编译cas 5.1.8之](http://blog.csdn.net/leftfist/article/details/79301213)后，怎么调试呢？
在我机器里面，intellij idea中直接按下Run或Debug，妄图运行或调试，是会报错的。一大堆的错误，什么“restartedMain ERROR Error creating converter for xwEx java.lang.reflect.InvocationTargetException”之类，据说是log4j和spring boot版本不兼容，云云。作为一个java为入门者，面对如此“庞大”的一个系统，有太多的东西不明白，跑出来这么多的错误，望而生畏，无从下手。
因为默认情况下，对于CAS 5.1.8,intellij idea是将它作为spring boot程序运行的： 
![这里写图片描述](https://img-blog.csdn.net/20180228190827812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
但我们部署的时候，只是将cas-5.1.8\webapp\cas-server-webapp\build\libs\cas-server-webapp-5.1.8.war扔到tomcat下即可，那么，其实我们可以在intellij idea中run at tomcat就行了啊。
run - edit configurations - + - tomcat server，在deployment里添加“cas-server-webapp-5.1.8.war”即可
![这里写图片描述](https://img-blog.csdn.net/20180228191531793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20180228191547571?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20180228191559925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
设置断点，点击那个运行旁边的bug图标，就可以调试了。
单步调试：F8
鼠标悬停在断点行，可以看到变量的值
如果想观察语句的运行结果，可以鼠标右键，在快捷菜单中“Add watch”，输入即可。
也挺方便的。
