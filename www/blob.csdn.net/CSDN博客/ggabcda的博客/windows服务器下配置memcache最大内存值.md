# windows服务器下配置memcache最大内存值 - ggabcda的博客 - CSDN博客





2017年08月28日 20:29:27[ggabcda](https://me.csdn.net/ggabcda)阅读数：745








windows服务器下修改memcache配置要求如下： 


1、用内网ip的方式提供web应用服务器调用，不允许直接通过外网调用，如将memcache服务放在192.168.1.168的服务器上 


2、修改端口，如改为11200 


3、分配内存，如分配1024M(1G内存) 



方法如下：

1>开始>运行：regedit(回车) 


2>在注册表中找到：HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\memcached Server 


3>默认的ImagePath键的值是："c:\memcached\memcached.exe" -d runservice，改为："c:\memcached\memcached.exe" -d runservice -m 512 -p 11200 -l 192.168.1.168 


4>运行DOS重启memcache的服务：memcached -d restart 

注：如果将上述IP192.168.1.168
 改为127.0.0.1，则可以控制只能本机访问memcached服务，其他任何机器都不能访问，更安全。

可以使用PHP编写的 MemAdmin 应用来监控memcahce运行状态，下载地址：http://www.junopen.com/memadmin/，如果是本机安装，直接运行默认设置即可监控。



