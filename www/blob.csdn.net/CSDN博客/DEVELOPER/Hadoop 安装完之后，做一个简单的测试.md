# Hadoop 安装完之后，做一个简单的测试 - DEVELOPER - CSDN博客





2017年11月09日 21:03:03[学术袁](https://me.csdn.net/u012827205)阅读数：771
所属专栏：[Hadoop-学习](https://blog.csdn.net/column/details/20399.html)









本来今晚的兴致很高，一直在加班撸Android。好久没有学习Hadoop了，今晚本来兴致很高涨，准备动手写命令的时候，突然发现Xshell不能用了-卧槽！搞了半天还是“已经过了评估期”什么的，老子不用了，直接撸！！

然后接着上一篇的Hadoop 安装开始，安装成功之后，我们接下来做一个简单的测试。

![](https://img-blog.csdn.net/20171109205753536?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


看到上面的图没？我是这样进行操作的，我先获取了ROOT权限，然后进入根目录。从根目录切换到我之前安装的Hadoop所在的路径。然后进入Hadoop所在路径之后，我查看了一下当前目录下的文件列表。哇~好多！然后我执行了一下Hadoop的运行命令，紧接着有看到 namenode datanode jobtracker tasktracker都在逐个执行，并要求我输入密码。最后我 jps 一下，查看正在进行的任务。

![](https://img-blog.csdn.net/20171109211106916?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后，我在 “~/”目录下新建了一个文件夹-work_file，然后使用"echo" ">"（输出重定向符）在work_file下新建了两个文件。切换到Hadoop 所在的目录，执行命令对分布式系统进行操作把work_file放到hdfl文件的目录里面去。然后查看是否存在，上面的/in/input/是我之前创建的，红色划了很多线的是敲错的命令。看到上面结果说明操作到此是成功的。






















































