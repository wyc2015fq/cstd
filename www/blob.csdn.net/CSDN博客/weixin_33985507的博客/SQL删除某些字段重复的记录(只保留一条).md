# SQL删除某些字段重复的记录(只保留一条) - weixin_33985507的博客 - CSDN博客
2008年06月19日 09:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
    前一阵做了个会员系统,又写了个程序把以往的会员数据导入到SQL数据库中.因为某些原因导了好几遍,造成了某些重复的记录存在.前两天使用人员才发现问题,于是想办法解决.
    搜啊搜,搜到了使用SQL句子删除重复记录的方法.又一次体会到了SQL的强大(其实是我的SQL水平太菜了而已).写下来,加强记忆.
    会员数据需要用到的是下面三个字段:ID(自增),MemberName,MemberAddress.只要会员姓名与会员地址相同就认为是重复记录,重复记录在删除时只保留ID最大的那个.SQL如下:
```
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)delete MemberInfo where ID not in (
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)  select max(ID) from MemberInfo group by MemberName, MemberAddress)
```
    not in的效率可能会低些,但因为是直接操作数据库,所以这并不重要.这个句子还是非常的简单有效的.
    在真正的删除操作前,通常会先了解一下重复记录的情况.可以使用下面的句子:
```
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)SELECT COUNT(MemberName) AS TheCount, MemberName, MemberAddress
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)FROM MemberInfo
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)GROUP BY MemberName, MemberAddress
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)HAVING (COUNT(*) > 1)
```
    因为工作中用的SQL太简单,以至于group by及having的用法都不了解,真是惭愧.
    P.S. 所有的内容都来自于网络,没有什么独创的东西.发出来只是为了加强一下自己的记忆.
