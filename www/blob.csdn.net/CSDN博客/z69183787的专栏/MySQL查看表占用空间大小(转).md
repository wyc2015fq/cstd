# MySQL查看表占用空间大小(转) - z69183787的专栏 - CSDN博客
2017年08月03日 12:16:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：284
```
//先进去MySQL自带管理库：information_schema   
//自己的数据库：dbwww58com_kuchecarlib   
//自己的表：t_carmodelparamvalue  
  
mysql> use information_schema;  
Database changed  
mysql> select data_length,index_length  
    -> from tables where  
    -> table_schema='dbwww58com_kuchecarlib'  
    -> and table_name = 't_carmodelparamvalue';  
+-------------+--------------+  
| data_length | index_length |  
+-------------+--------------+  
|   166379520 |    235782144 |  
+-------------+--------------+  
row in set (0.02 sec)  
  
mysql> select concat(round(sum(data_length/1024/1024),2),'MB') as data_length_MB,  
    -> concat(round(sum(index_length/1024/1024),2),'MB') as index_length_MB  
    -> from tables where  
    -> table_schema='dbwww58com_kuchecarlib'  
    -> and table_name = 't_carmodelparamvalue';  
+----------------+-----------------+  
| data_length_MB | index_length_MB |  
+----------------+-----------------+  
| 158.67MB       | 224.86MB        |  
+----------------+-----------------+  
row in set (0.03 sec)
```
