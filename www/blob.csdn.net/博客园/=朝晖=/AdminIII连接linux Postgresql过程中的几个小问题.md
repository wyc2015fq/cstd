# AdminIII连接linux Postgresql过程中的几个小问题 - =朝晖= - 博客园
# [AdminIII连接linux Postgresql过程中的几个小问题](https://www.cnblogs.com/dhcn/p/7105554.html)
1、postgresql.conf主配置文件中要配置postgresql绑定的IP，如果不设置，可能只绑定本地闭环地址：127.0.0.1,可以设定为0.0.0.0；就包括了一切IPv4地址
2、pg_hba.conf中的设置，这个里面设置，主要是添加连接客户端所在的机器IP,并且在IP后面要加个/32,/32是Ipv4的，ipv6就是/128.方法直接写成password.设置方法参考：[http://wiki.postgresql.org/wiki/9.1%E7%AC%AC%E5%8D%81%E4%B9%9D%E7%AB%A0](http://wiki.postgresql.org/wiki/9.1%E7%AC%AC%E5%8D%81%E4%B9%9D%E7%AB%A0)
3、配置修改完，可以用init.d目录中的启动脚本reload重载配置，restart不行，用它，修改后的配置无法生效，
4;说句题外话，我安装的Postgresql 9.2这个版本，如果机器上安装的VC++2008包就可能导致PostgresQL的AdminIII和Postgresql服务都无法启动

