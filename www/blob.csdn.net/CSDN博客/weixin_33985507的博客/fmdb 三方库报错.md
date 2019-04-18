# fmdb 三方库报错 - weixin_33985507的博客 - CSDN博客
2017年04月11日 18:06:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
我在项目中当往数据库插入数据，
sql =  @"INSERT INTO VISIT_STORE(store_id,) values( ?)"，**100** ；
 会报错
正确的处理
数字100得封装一下，写成@（100）就可以了。
数字类型的必须包装成对象类型 。
原文地址：
http://blog.csdn.net/axing1991/article/details/51925382
