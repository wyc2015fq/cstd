# Linux 修改本机 /etc/hosts 的 hostName 后经常不生效 - 文章 - 伯乐在线
原文出处： [真实的归宿](http://blog.csdn.net/hguisu/article/details/49278355)
### 1、Linux修改本机别名/etc/hosts的hostName后经常不生效解决
Linux修改本机别名/etc/hosts的hostName后经常不生效， 比如我们/etc/hosts的内容如下：
#192.68.1.10 message.xxx.com
192.68.1.11 message.xxx.com
但是ping
message.xxx.com还是指向192.68.1.10。
一般2种解决方案：
1). 重启，这是最直接最可靠稳定的一种方法，如果有些时候不方便重启，则可以使用第二种方法。
2). 修改/etc/sysconfig/network文件，将HOSTNAME修改成对应的别名，如下：


```
NETWORKING=yes
HOSTNAME=host1
```
修改后重启网络服务
service network restart (本质是/etc/init.d/network) 3).
如果重启服务器后，还是不起作用，应该是本机的dns缓存引起的。
查看nscd是否启用： ps -ef|grep nscd
直接关闭Linux nscd 缓存服务： /etc/init.d/nscd stop
### 2、Linux的DNS
linux本身是没有dns缓存的,想使用dns缓存的话需要自己安装一个服务程序NSCD(name service cache daemon).
nscd缓存三种服务passwd group hosts，所以它会记录三个库，分别对应源/etc/passwd, /etc/hosts 和 /etc/resolv.conf每个库保存两份缓存，一份是找到记录的，一份是没有找到记录的。每一种缓存都保存有生存时间（TTL）.
安装：
yum install nscd
修改配置文件/etc/nscd.conf,开启dns缓存,修改这一行
enable-cache hosts yes。
如果启用了nscd 服务，才有dns 缓存，否则是没有dns缓存的。
开启 、停止、 重启服务 service nscd start | stop | restart
nscd的配置：
通过编辑/etc/nscd.conf文件，在其中增加如下一行可以开启本地DNS cache：
enable-cache hosts yes
阿里云主机上的配置如下：


```
[root@iZ2571ykq ~]# cat /etc/nscd.conf   
#logfile        /var/log/nscd.log  
threads         6  
max-threads     128  
server-user     nscd  
debug-level     5  
paranoia        no  
enable-cache    passwd      no  
enable-cache    group       no  
enable-cache    hosts       yes  
positive-time-to-live   hosts   5  
negative-time-to-live   hosts       20  
suggested-size  hosts       211  
check-files     hosts       yes  
persistent      hosts       yes  
shared          hosts       yes  
max-db-size     hosts       33554432
```
相关参数的解释如下：
logfile debug-file-name：指定调试信息写入的文件名。
debug-level value：设置希望的调试级别。
threads number：这是启动的等待请求的线程数。最少将创建5个线程。
server-user user：如果设置了该选项，nscd将作为该用户运行，而不是作为root。如果每个用户都使用一个单独的缓存（-S参数），将忽略该选项
enable-cache service <yes|no>：启用或禁用制定的 服务 缓存。
positive-time-to-live service value：设置 service 在指定缓存中正的项目（成功的请求）的TTL（存活时间）。 Value 以秒为单位。较大的值将增加缓存命中率从而减低平均响应时间，但是将增加缓存的一致性问题。
negative-time-to-live service value：设置 service 在指定缓存中负的项目（失败的请求）的TTL（存活时间）。 Value 以秒为单位。如果存在由不在系统数据库中的uid（用户ID）（例如在以root身份解包linux 内核源代码时）所拥有的文件将明显改善性能；应该维持较小的值以降低缓存一致性问题。
suggested-size service value：这是内部散列表的大小， value 应该保持一个素数以达到优化效果。
check-files service <yes|no>：启用或禁用检查属于指定 服务 的文件的改变。这些文件是 /etc/passwd， /etc/group， 以及/etc/hosts。
nscd 的查看和清除
nscd缓存DB文件在/var/db/nscd下。
nscd -g 查看统计信息
清除 nscd缓存：
nscd -i passwd
nscd -i group
nscd -i hosts
当然也先删除缓存库或者停掉nscd服务：
rm -f /var/db/nscd/hosts
service nscd restart
或者是直接停止nscd 服务
service nscd stop。
