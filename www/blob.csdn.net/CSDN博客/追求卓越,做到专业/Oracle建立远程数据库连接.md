
# Oracle建立远程数据库连接 - 追求卓越,做到专业 - CSDN博客


2012年09月24日 13:52:08[Waldenz](https://me.csdn.net/enter89)阅读数：1887个人分类：[Oracle																](https://blog.csdn.net/enter89/article/category/1078928)


CREATE   PUBLIC   DATABASE   LINKlinknameCONNECT   TOusernameIDENTIFIED   BYuserpwdUSING   '172.16.100.20:1521/databasename'
 ;
其中linkname为连接名，username和userpwd为该远程数据库中帐号和密码；172.16.100.20:1521/databasename为远程数据库实例名
重点:数据库用户密码不能全是数字,而且不能以数字开头.
建立连接完成后,使用下列查询语句查询
select * from table@linkname

