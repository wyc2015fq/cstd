# hive udf - zj360202的专栏 - CSDN博客





2014年08月07日 19:32:49[zj360202](https://me.csdn.net/zj360202)阅读数：836标签：[hive																[udf																[源码																[hive-exec																[hiveUDF](https://so.csdn.net/so/search/s.do?q=hiveUDF&t=blog)
个人分类：[Hive](https://blog.csdn.net/zj360202/article/category/1298103)





有两种方式写UDF

一、就是网上比较常见就是，写一个java project然后导入hive依赖包，写一个继承UDF的一个类，实现功能

![](https://img-blog.csdn.net/20140807192431203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




然后通过以下脚本注入




```java
add jar /home/hadoop/hive_udf/hiveUDF.jar;
create temporary function hash_code as 'org.apache.hadoop.hive.ql.udf.UDFHashCode';
```


二、第一种方法只能创建临时的udf函数，第二种方法是找到hive的源码，



如果版本不一样，那就算了，还是用第一种，

如果有源码的话，可以在hive-exec包下面的



```java
\ql\src\java\org\apache\hadoop\hive\ql\exec\FunctionRegistry.java
```
中添加 





```java
registerUDF("hash_code", UDFHashCode.class, false);
```


然后将这两个人类的class文件放到hive-exec对应位置替换，然后将这个包上传覆盖掉线上的jar包就可以了](https://so.csdn.net/so/search/s.do?q=hive-exec&t=blog)](https://so.csdn.net/so/search/s.do?q=源码&t=blog)](https://so.csdn.net/so/search/s.do?q=udf&t=blog)](https://so.csdn.net/so/search/s.do?q=hive&t=blog)




