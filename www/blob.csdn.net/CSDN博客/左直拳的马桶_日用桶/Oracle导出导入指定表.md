# Oracle导出导入指定表 - 左直拳的马桶_日用桶 - CSDN博客
2016年01月27日 19:19:24[左直拳](https://me.csdn.net/leftfist)阅读数：16630
从源数据库导出：
`exp user1/pwd@server1 file=c:\temp\exp.dmp tables=(table1, table2)`
导入到目标数据库：
`imp user2/pwd@server2 file=c:\temp\exp.dmp tables=(table1, table2)`
Oracle就是有这个好处，可以直接将远程的数据导出到本地；或者将本地的数据文件导入到远程数据库。而SQL SERVER的备份、还原文件，一定是放在远程服务器本机的。
补充一下，如果有许多表，那么以上语句可能会换行。但以上命令是在DOS命令窗口执行的，命令行方式，容不得换行。那咋办呢？我的笨办法是将语句拷贝到WORD文档，然后将回车换行符替换掉。替换的方法：在替换窗口，查找内容输入 ^p(注意是小写)，替换内容为空，然后替换即可。
=====================================
以 sysdba 身份运行，导出导入指定用户所属表：
exp ‘sys/123456@Service1 as sysdba’ file=c:\temp\exp.dmp table 
s=(sde.ISLAND)
imp ‘sys/888888@Service2 ’ file=c:\temp\exp.dmp fromuser 
=sde tables=(ISLAND)
================================================= 
如果表空间的名字不一样，会报 ORA-00959 错误，咋办？
1、先从源数据库，导出建表脚本（DDL）
2、在新库新表空间里运行这些DDL
3、imp,加上fromuser,touser,ignore=Y选项
`imp 'sys/lt@HPM as sysdba' file=c:\temp\base.dmp fromuser=hydg touser=hpm tables=(org_user,SYS_ONLINE,sys_userRoles,Org_Role,Org_Group,sys_Event) ignore=Y`
有关ORA-00959 错误的应对之道，参考文章：
[快速解决IMP中的ORA-00959](http://blog.csdn.net/lwei_998/article/details/6587339)
