# 查看oracle版本命令 - z69183787的专栏 - CSDN博客
2013年07月15日 15:15:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1038
1  查看oracle的版本信息
（1）用客户端连接到数据库，执行select * from v$instance
            查看version项
（2）select * from product_component_version
（3）或查询V$VERSION查看组件级信息
