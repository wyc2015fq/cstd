# 快捷修复数据库try to repair it错误 - fanyun的博客 - CSDN博客
2016年03月18日 14:02:13[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2033
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
1.对于数据库表try to repair it异常的处理：
       报错如下所示：
        java.sql.SQLException:Incorrect key file for table './bd_jrdb/JYDB_DeleteRec.MYI'; try to repair it
      在数据库输入如下命令查看错误：CHECKTABLE NI_DynamicNews;
      检测错误结果如图1.9：
![](https://img-blog.csdn.net/20160315141750132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                                                                       图1.9
         测试发现有如下错误，使用修复：REPAIRTABLE NI_DynamicNews;
         修复结果如图1.10：
![](https://img-blog.csdn.net/20160315141858618?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                                                                    图1.10
