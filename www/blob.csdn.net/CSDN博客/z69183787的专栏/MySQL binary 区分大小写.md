# MySQL binary 区分大小写 - z69183787的专栏 - CSDN博客
2016年05月05日 11:24:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4344
MySQL居然不区分大小写??
![](http://www.linuxidc.com/upload/2014_06/140608163818931.jpg)
我开始差点以为眼花了
![](http://www.linuxidc.com/upload/2014_06/140608163818932.jpg)
再做个查询..这是神马结果啊
![](http://www.linuxidc.com/upload/2014_06/140608163818933.jpg)
可以使用binary将字符串先转换为二进制字符串，在进行比较
![](http://www.linuxidc.com/upload/2014_06/140608163818934.jpg)
![](http://www.linuxidc.com/upload/2014_06/140608163818935.jpg)
![](http://www.linuxidc.com/upload/2014_06/140608163818936.jpg)
加上binary，终于正常了。
SELECT * FROM ACT_cdKey WHERE ActivityId=3577 AND CdKey= binary '223UUwax';
1、linux下mysql安装完后是默认：区分表名的大小写，不区分列名的大小写； 
2、用root帐号登录后，在/etc/my.cnf 中的[mysqld]后添加添加lower_case_table_names=1，重启MYSQL服务，这时已设置成功：不区分表名的大小写； 
lower_case_table_names参数详解： 
lower_case_table_names = 0 
其中 0：区分大小写，1：不区分大小写 
MySQL在Linux下数据库名、表名、列名、别名大小写规则是这样的： 
　　 1、数据库名与表名是严格区分大小写的； 
　　 2、表的别名是严格区分大小写的； 
　　 3、列名与列的别名在所有的情况下均是忽略大小写的； 
　　 4、变量名也是严格区分大小写的； 
MySQL在Windows下都不区分大小写。 
3、如果想在查询时区分字段值的大小写，则：字段值需要设置[BINARY](https://www.baidu.com/s?wd=BINARY&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y4m1wbPH0znW64PyPhrjn10ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnWcvP1nYnjmsP16vPjTYnW0zr0)属性，设置的方法有多种： 
A、创建时设置： 
CREATE TABLE T( 
A [VARCHAR](https://www.baidu.com/s?wd=VARCHAR&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y4m1wbPH0znW64PyPhrjn10ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnWcvP1nYnjmsP16vPjTYnW0zr0)(10) [BINARY](https://www.baidu.com/s?wd=BINARY&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y4m1wbPH0znW64PyPhrjn10ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnWcvP1nYnjmsP16vPjTYnW0zr0) 
); 
B、使用alter修改： 
ALTER TABLE `tablename` MODIFY [COLUMN](https://www.baidu.com/s?wd=COLUMN&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y4m1wbPH0znW64PyPhrjn10ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnWcvP1nYnjmsP16vPjTYnW0zr0) `cloname` [VARCHAR](https://www.baidu.com/s?wd=VARCHAR&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y4m1wbPH0znW64PyPhrjn10ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnWcvP1nYnjmsP16vPjTYnW0zr0)(45) [BINARY](https://www.baidu.com/s?wd=BINARY&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y4m1wbPH0znW64PyPhrjn10ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnWcvP1nYnjmsP16vPjTYnW0zr0); 
C、mysql table editor中直接勾选BINARY项。
