# 修改windows service中的依存关系 - z69183787的专栏 - CSDN博客
2012年11月14日 22:36:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3793
最近项目中，需要吧本来 负载均衡的 A B 两台机器 中的一台用于其他用途，故需要将A B 的负载均衡功能停止，经过查找发现 中间件的 80端口上跑着负载均衡，而8088端口是真正的服务。本来以为直接停止80端口的服务就可以了，没想到一看才发现，80端口的服务 竟然与8088端口的服务有依存关系。
结果当然是取消2个端口的依存关系，经过晚上查找，得出以下办法。
1.使用regedit进入注册表，在\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\
 找到需要设置的服务名。 
2.找到一个多字符串的DependOnService的项，删除。 
3.重启服务即可（我后来是重启服务器才有用的，不知道为何）。
同样 如果是增加依存关系的话
1.使用regedit进入注册表，在\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\
 找到需要设置的服务名。 
2.建立一个多字符串的DependOnService的新项，value设置成需要依赖启动的服务。 
3.重启服务即可。
