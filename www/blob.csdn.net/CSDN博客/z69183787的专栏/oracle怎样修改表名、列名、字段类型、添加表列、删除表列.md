# oracle怎样修改表名、列名、字段类型、添加表列、删除表列 - z69183787的专栏 - CSDN博客
2013年11月25日 16:01:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2313
**ALTER ****TABLE **SCOTT.TEST **RENAME ****TO **TEST1--修改表名
**ALTER ****TABLE **SCOTT.TEST **RENAME ****COLUMN **NAME **TO **NAME1
 --修改表列名
**ALTER ****TABLE **SCOTT.TEST **MODIFY **NAME1 **NUMBER**(20) --修改字段类型
**ALTER ****TABLE **SCOTT.TEST **ADD **ADDRESS **VARCHAR2**(40)--添加表列
**ALTER ****TABLE **SCOTT.TEST **DROP **NAME **CASCADE****CONSTRAINTS --删除表列**
