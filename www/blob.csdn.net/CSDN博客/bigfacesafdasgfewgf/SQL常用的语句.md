# SQL常用的语句 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 17:09:58[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：518
个人分类：[Database](https://blog.csdn.net/puqutogether/article/category/2599071)








为了查看数据库里的数据，我们可以通过在SQL编译器中编写一些代码来查看、增加、删除、更新数据。



首先使用的MySQL数据库管理工具是Navicat for MySQL，安装包在我的百度云上面。

![](https://img-blog.csdn.net/20140927171028860?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







需要连接之后才能够看到数据：



![](https://img-blog.csdn.net/20140927171059405?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





![](https://img-blog.csdn.net/20140927170949203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







其中在表那一栏中出现了几个表，显示的都是这些表名；

双击就可以看到那些表里面的详细内容。



![](https://img-blog.csdn.net/20140927171158217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


最重要的地方来了，那就是我们一般都需要选出其中的某些数据来进行处理。

这个时候就需要借助一些SQL的语句：

泡哥是数据库的大神，他告诉了我几个常用的SQL语句：

select tarQue,solutionId from question_train where webId=1 and groupId=77 and del=0 and answerStatus=0

这句中，select是选择命令，后面的tarQue,solutionId都是SQL中对应的字段（即每一列），from question_train是表名，就是说我需要从question_train表中选择查看数据，然后where是约束范围，即对应的符合：webId=1 and groupId=77 and del=0 and answerStatus=0条件的数据才是我需要的。

![](https://img-blog.csdn.net/20140927171244300?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





点击运行之后，就可以看到我们需要的数据了：



![](https://img-blog.csdn.net/20140927171334469?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


其余的SQL常用语句还有：order，update，insert，drop，delete，set（赋值）。

其中selet * 表示查询所有数据。例如：

select * from question_train




关于SQL其余的语句，后面再详细熟悉，这几句已经够了我现在使用Python读取数据库中的数据了。







