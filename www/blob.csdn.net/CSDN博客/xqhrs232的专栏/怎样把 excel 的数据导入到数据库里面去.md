# 怎样把 excel 的数据导入到数据库里面去 - xqhrs232的专栏 - CSDN博客
2018年05月25日 18:55:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：273
原文地址::[https://blog.csdn.net/zhangyu19881125/article/details/28411309](https://blog.csdn.net/zhangyu19881125/article/details/28411309)
相关文章
1、[sqlserver怎么将excel表的数据导入到数据库中](https://www.cnblogs.com/qianzf/p/7069117.html)----[https://www.cnblogs.com/qianzf/p/7069117.html](https://www.cnblogs.com/qianzf/p/7069117.html)
2、如何将excel表格的数据导入到mysql数据中去----[https://jingyan.baidu.com/article/fc07f9891cb56412ffe5199a.html](https://jingyan.baidu.com/article/fc07f9891cb56412ffe5199a.html)
1. 把 excel 另存为 .csv 格式
2. 用 Notepad 打开 .csv 文件, 第一行就是所有的字段
3. 创建表结构
[html][view plain](https://blog.csdn.net/zhangyu19881125/article/details/28411309#)[copy](https://blog.csdn.net/zhangyu19881125/article/details/28411309#)
- create table yu_rt_01 as  
- select Transaction_Id,Last_Update_Date,Last_Updated_By,Creation_Date,Created_By,Last_Update_Login,Request_Id,... ,Lcm_Shipment_Line_Id  
- from rcv_transactions where 1=2;  
4. 检查表里面没有数据
5. 打开 Toad for Oracle
6. 连接数据库
7. Database -> Import -> Import Table Data
8. 选择表 yu_rt_01
9. 点击 show data, 现在是空的
10. Next
11. Import form = text, File 是刚才保存的 .csv 文件
12. Next
13. First Row 改成 2
14. 其他所有的保持默认, 点击 commit
15. select * from yu_rt_01
-----------------------------------------2015年9月6日 更新------------------------------------------
Oracle 官方 SQL Developer 也可以导入数据, 之前一直都不会用...
![](https://img-blog.csdn.net/20150906103938128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我用的是 4.1 版本. 之前版本不知道如何. 网上说之前版本可能有问题, 所以就懒得试了;
