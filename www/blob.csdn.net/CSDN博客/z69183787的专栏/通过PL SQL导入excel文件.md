# 通过PL/SQL导入excel文件 - z69183787的专栏 - CSDN博客
2015年02月16日 13:56:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：820
个人分类：[数据库-Sql&Hql](https://blog.csdn.net/z69183787/article/category/2185915)
    PL/SQL 和SQL Sever导入excel数据的原理类似，就是找到一个导入excel数据的功能项，按照步骤走就是了。下面是一个些细节过程，希望对像我这样的菜鸟有帮助。
      1、准备excel表。
           右击数据表—选择edit data.
选择数据，右击，选择Copy to Excel 
![](https://img-my.csdn.net/uploads/201212/15/1355537759_5941.png)
        2、调整excel表
           可以删除A列和F列，然后把你的数据粘到BCDE列，这样做的好处就是在导入excel的时候，绝对不会出现因为格式或其他文字问题导致错误。
![](https://img-my.csdn.net/uploads/201212/15/1355537745_9348.png)
        3、准备导入
            选择tool—ODBC Importer，在User/SystemDSN里有三项，这里我们需要选择第二项Excel Files
![](https://img-my.csdn.net/uploads/201212/15/1355537995_1005.png)
             选择Connect在弹出框里选择你要导入的excel文件
![](https://img-my.csdn.net/uploads/201212/15/1355538383_1764.png)
            预览你的数据，之后单击Data to Oracle 
![](https://img-my.csdn.net/uploads/201212/15/1355538524_8476.png)
     添加上所有者，和你要添加数据的数据表，这里是T_Admin,之后单击import就可以了
![](https://img-my.csdn.net/uploads/201212/15/1355538657_9615.png)
![](https://img-my.csdn.net/uploads/201212/15/1355538785_9920.png)
            4、查看数据：
                  导入的数据已存在了。
![](https://img-my.csdn.net/uploads/201212/15/1355538920_1424.png)
