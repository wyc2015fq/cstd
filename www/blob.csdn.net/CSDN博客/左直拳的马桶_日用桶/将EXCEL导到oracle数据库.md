# 将EXCEL导到oracle数据库 - 左直拳的马桶_日用桶 - CSDN博客
2017年05月27日 17:00:55[左直拳](https://me.csdn.net/leftfist)阅读数：447标签：[oracle																[excel																[csv																[plsql developer](https://so.csdn.net/so/search/s.do?q=plsql developer&t=blog)](https://so.csdn.net/so/search/s.do?q=csv&t=blog)](https://so.csdn.net/so/search/s.do?q=excel&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[应用																[oracle](https://blog.csdn.net/leftfist/article/category/5734215)](https://blog.csdn.net/leftfist/article/category/2778939)
将EXCEL导到oracle数据库，我目前了解到一种途径是： 
1、将excel另存为csv文件 
2、在PL/SQL Developer里将csv文件内容导进oracle
展开说下步骤2： 
1、打开PL/SQL Developer，登录数据库
2、工具 - 文本导入器
3、在文本导入器里打开csv文件 
![这里写图片描述](https://img-blog.csdn.net/20170527164820311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注意同一时间没有别的程序在打开这个csv文件。我曾经在excel里将文件另存为csv文件，然后excel没关闭，在PL/SQL Developer打开，结果打开失败，一条记录也没有。
4、文本导入器中，切换到“到 oracle 的数据” 
将列与字段挂钩。 
挂钩时，可以应用各种函数，或者表达式。例如： 
1）用GUID作为主键 
![这里写图片描述](https://img-blog.csdn.net/20170527165512489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2）case语句 
![这里写图片描述](https://img-blog.csdn.net/20170527165712680?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这些匹配、定义可能搞得很辛苦，可以将它保存下来，以备再用： 
![这里写图片描述](https://img-blog.csdn.net/20170527165906855?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
5、按下下面的“导入”按钮，即可导入矣。有选项“清除表”，颇为毒辣。
