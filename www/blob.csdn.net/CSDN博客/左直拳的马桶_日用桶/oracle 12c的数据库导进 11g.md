# oracle 12c的数据库导进 11g - 左直拳的马桶_日用桶 - CSDN博客
2016年03月21日 19:26:15[左直拳](https://me.csdn.net/leftfist)阅读数：6313
从oracle 12c 备份（expdp）出来的包，还原到11g里，想想都知道会有兼容性问题。
果不其然，报错了： 
ORA-39142: 版本号 4.1 (在转储文件 “叉叉叉.expdp” 中) 不 兼容 
之类
后来才知道，在12C中备份出来时，应指定版本信息，例如我要导进11g的库（具体版本为11.1.0.6.0，打开sqlplus就能看到），则version=11.1.0.6.0
语句： 
在12C备份
`expdp system/password@SID12c directory=dbbak dumpfile=20160321.expdp logfile=20160321.log schemas=leftfist version=11.1.0.6.0`
在11g中还原
`impdp sys/password@SID11g directory=dbbak dumpfile=20160321.expdp logfile=20160321.log schemas=leftfist table_exists_action=replace`
当然这里面忽略了创建备份文件存放目录dbbak，创建表空间、用户等细节。这类细节可参考拙作 
[oracle 10g的备份和还原](http://blog.csdn.net/leftfist/article/details/47616785)
参考文章： 
[ORA-39142 IMPDP跨版本导入数据报错解决](http://www.oracleonlinux.cn/2011/11/ora-39142_impdp_err/)
