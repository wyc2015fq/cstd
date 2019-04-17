# 使用Intellij Idea打包java为可执行jar包 - qq_37385726的博客 - CSDN博客





2018年09月28日 20:40:01[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：38








转载自[这里](https://blog.csdn.net/xuemengrui12/article/details/74984731)



步骤：

1. 选中Java项目工程名称，在菜单中选择 File->project structure... (快捷键Ctrl+Alt+Shift+S)。

![](https://img-blog.csdn.net/20170711163939901?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVlbWVuZ3J1aTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

2. 在弹出的窗口中左侧选中"Artifacts"，点击"+"选择jar，然后选择"from modules with dependencies"。

![](https://img-blog.csdn.net/20170711163945572?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVlbWVuZ3J1aTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3. 在配置窗口中配置"Main Class"。

![](https://img-blog.csdn.net/20170711163956890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVlbWVuZ3J1aTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

4.配置“Directory for META-INF/MAINFEST.MF”，此项配置的缺省值是：D:\Intellij\SeriesPublish\src\main\java，需要改成：D:\Intellij。如果不这样修改，打成的jar包里没有包含META-INF/MAINFEST.MF文件，这个应该是个IDEA的BUG（参考：[http://stackoverflow.com/questions/15724091/how-to-run-a-jar-file-created-using-intellij-12](http://stackoverflow.com/questions/15724091/how-to-run-a-jar-file-created-using-intellij-12)），配置完成后如下图所示，点击OK进入下一步。我个人建议选择“extract to the target JAR”，这样所有依赖的jar包都会放在生成的jar包中。

![](https://img-blog.csdn.net/20170711164001397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVlbWVuZ3J1aTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

5. 完成后，点击OK，Apply等按钮，回到IDEA的主菜单，选择“Build - Build Artifacts”下的“Build”或者“Rebuild”即可生成最终的可运行的jar，在D:\Intellij\SeriesPublish\out\artifacts\SeriesPublish_jar下面找到生成的目标jar，可以看到META-INF/MAINFEST.MF文件被正确包含，内容也正确。

![](https://img-blog.csdn.net/20170711164005920?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVlbWVuZ3J1aTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

--------------------- 本文来自 爆米花9958 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/xuemengrui12/article/details/74984731?utm_source=copy



