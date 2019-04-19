# SQL SERVER 2008远程数据库移植到本地的方法 - xqhrs232的专栏 - CSDN博客
2018年07月24日 23:09:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：98
原文地址::[https://blog.csdn.net/wuzhanwen/article/details/77449229](https://blog.csdn.net/wuzhanwen/article/details/77449229)
  Winform程序或网站后台的SQL SERVER 2008放置在远程服务器上，用MicroSoft SQL Server ManageMent Studio进行表、视图、存储过程进行增删改操作没有问题，但是想把远程数据库移植到本地，一是让远程网管把备份发过来，二是用复制的方法，三是先在本地生成与远程同名的空数据库，从远程导入数据（数据表结构和数据以及视图），后从远程生成存储过程，再在本地数据库上执行存储过程脚本的方法。复制方法很容易在执行过程中出错，本文介绍第三种方法。
一、连接本地数据库引擎
新建一个数据库，如：rbrbsoft
二、连接远程数据库
![](https://img-blog.csdn.net/20170821142003520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
二、选择自己的远程数据库，在其上点键->任务->导出数据库
![](https://img-blog.csdn.net/20170821142104715?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170821142159207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择远程数据源
![](https://img-blog.csdn.net/20170821142301909?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择本地目标数据库
![](https://img-blog.csdn.net/20170821142353355?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170821142435211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170821142522359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击编辑映射按钮
![](https://img-blog.csdn.net/20170821142603911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击确定后，点下一步：
![](https://img-blog.csdn.net/20170821142647519?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点下一步：
![](https://img-blog.csdn.net/20170821142728231?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点完成：
![](https://img-blog.csdn.net/20170821142812432?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170821142851175?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
三、选择远程自己的数据库，点右键->任务->生成脚本(E)...
![](https://img-blog.csdn.net/20170821142937677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170821143019763?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170821143055951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170821143130613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170821143204984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170821143239301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170821143313849?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击完成按钮
四、关闭Microsoft SQl Server Management Studio后，重新连接本地数据库引擎
五、在Microsoft SQl Server Management Studio,点击菜单文件->打开，打开刚才生成的rbsoft.sql文件，点击工具栏的![](https://img-blog.csdn.net/20170821143419499?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3V6aGFud2Vu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
按钮执行SQL语句，这样就把远程数据库：表、视图、存储过程完整地生成了本地数据库了！
