# MySQL集群系列2：通过keepalived实现双主集群读写分离 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月11日 21:18:39[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：388
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)

在上一节基础上，通过添加keepalived实现读写分离。
首先关闭防火墙
## 安装keepalived
[keepalived](http://www.keepalived.org/download.html)
2台机器都要安装
rpm -q openssl kernel-devel
**yum install **-y kernel-devel openssl-devel
 
.**/**configure --prefix=**/**usr**/**local**/**keepalived --with-kernel-dir=**/**usr**/**src**/**kernels**/**2.6.32-279.el6.x86_64**/****注意上面要替换成你的内核路径**
如果你没有用到LVS可以把lvs去掉，即：
.**/**configure --prefix=**/**usr**/**local**/**keepalived --with-kernel-dir=**/**usr**/**src**/**kernels**/**2.6.32-431.5.1.el6.x86_64**/  **--disable-lvs-syncd  --disable-lvs**make ****&& ****make ****install**
下面部分可以全部复制并粘贴到终端里：
**cp  ****/**usr**/**local**/**keepalived**/**sbin**/**keepalived  **/**usr**/**sbin**/****cp ****/**usr**/**local**/**keepalived**/**etc**/**sysconfig**/**keepalived    **/**etc**/**sysconfig**/****cp ****/**usr**/**local**/**keepalived**/**etc**/**rc.d**/**init.d**/**keepalived   **/**etc**/**init.d**/****mkdir ****/**etc**/**keepalived
设置开机启动
chkconfig --add keepalived
chkconfig keepalived on
## 配置keepalived
2台仅优先级不同，主为100，从为99，都为BACKUP状态
#vim /etc/keepalived/keepalived.conf  
global_defs **{**
	router_id mysql_ha
**}**
 
vrrp_instance **vi ****{**
state BACKUP
interface eth0
virtual_router_id 51
priority 90
advert_int 1
nopreempt
 
authentication **{**
auth_type PASS
auth_pass 1234**}**
 
virtual_ipaddress **{**
172.30.23.200
**}****}**
注：“vrrp_instance vi {” 里vi后面一定要有空格。也尽量不要让“｛”单独一行，因为很容易让vi后没有空格。
启动 service keepalived start
## 测试keepalived
查看相关日志/var/log/messages
- 启动主A：
Keepalived_vrrp[4955]: VRRP_Instance(vi) Entering BACKUP STATE
先进入设定的backup状态，然后转换成master状态。
Keepalived_vrrp[4749]: VRRP_Instance(vi) Transition to MASTER STATE
Keepalived_vrrp[4749]: VRRP_Instance(vi) Entering MASTER STATE
此时可以ping 通vip（172.30.23.200）
通过ip add也可以查看到eth0上的虚拟IP
- 启动优先级低的另一台从服务器B
Keepalived_vrrp[3158]: VRRP_Instance(vi) Entering BACKUP STATE
可见B进入BACKUP状态。
注：如果防火墙不关闭，会导致2台机器都进入MASTER状态。
chkconfig --level 0123456 iptables off
- 关闭主节点A的网卡,测试切换是否正常
ifdown eth0 
观察节点B的日志：
Keepalived_vrrp[3158]: VRRP_Instance(vi) Transition to MASTER STATE
Keepalived_vrrp[3158]: VRRP_Instance(vi) Entering MASTER STATE
可见从机器进入master状态了- 启动节点A的网卡测试切换是否正常
ifup eth0
观察节点B的日志：
状态不变，节点A变成了BACKUP状态
现在可以通过vip登录mysql了
mysql -u root -h 172.30.23.200 -p
但目前是在一台机器上实现读和写，另一台闲置。
## 测试读写分离
配置文件中2台机器的读写优先级互换。即主的vi_w为100,则从的vi_w为90，主的vi_r为90，则从的vi_r为100。
global_defs **{**
	router_id mysql_ha
**}**
 
vrrp_instance vi_w **{**
state BACKUP
interface eth0
virtual_router_id 51   //另一台一样
priority 90
advert_int 1
nopreempt
 
authentication **{**
auth_type PASS
auth_pass 1234**}**
 
virtual_ipaddress **{**
172.30.23.200
**}****}**
 
vrrp_instance vi_r **{**
state MASTER
interface eth0
virtual_router_id 44
priority 100
advert_int 1
 
authentication **{**
auth_type PASS
auth_pass 1234**}**
 
virtual_ipaddress **{**
172.30.23.210
**}****}**
只有主(IP201)一台启动时，主都进入Master状态。
此时mysql可利用vip200写，vip210读。
2个虚拟IP均在主201上(ip add)，从而读和写都在主201上。
- 下面启动从202，重启Keepalived即可
vip210出现短暂漂移。
发现主201上的读已转换为backup状态，通过ip add也只有vip200一个了。
在从202上，vip写直接进入设定的backup状态，vip读进入master状态了。此时
通过ip add发现vip210已移动到从202上，
从而此时写在主201，读在备202。
- 现在模拟从202挂掉
此时主201上都进入master状态，实现主可读可写
- 现在启动从202，将和上面情况一样。写在主，读在备。
- 现在模拟主201挂掉
挂掉前，主上写为master,读为backup,从202上写为backup,读为master,即主写从读。
挂掉后，
发现从202上，写已进入master状态，读仍为master状态，即从可读可写。
- 下面模拟主故障修复完成，重新启动成功。
发现主都进入了backup状态。正常应该是主把写抢回来，可能是从服务器设置了非抢占参数nopreempt
网上说这个参数只在优先级高的机器上设置即可，优先级低的机器不设置，实验后发现主仍然都是backup.
下面将主和从的nopreempt参数都删除。
发现正常，主写进入master,从写转换成backup
从而写在主201，读在202
这里仅仅是监控了网络故障和keepalived本身进程，在网络或者keepalived进程出现问题的时候会切换，但是我的节点A里面还有很多服务呢，
例如nginx，PHP，mysql进程出问题或高负载的时候怎么办，怎么切换呢，这时就要用到脚本了，下一节我们来看看keepalived是如何通过脚本实现对服务器的监控和切换的。
## 小结
此文实现:
1,当只有一台机器时，通过2个虚拟ip可以实现可读可写；
2，当有2台机器时，一台读，一台写，
3，当其中一台故障时，同1，即另一台可读可写。
4，当另一台正常后，同2，一台读一台写（根据配置文件中设置的固定的读写优先级，即1为写，若1故障，2可读可写，1恢复后，1继续写）。
可在php脚本中对读的ip进行数据库查询操作，写的ip进行插入操作,
当只有一台机器时，2个vip(虚拟ip)在一台机器上，从而读写在一个机器上，当2台机器时，各占一个vip，从而读写在2台机器上，所以要配置好mysql主从数据同步。
这种方式感觉php中连接数据库比较复杂啊。
最好配置一个ip,让内部自动分配读写。这样php就简单些了
## 艺搜参考
[http://bbs.ywlm.net/thread-855-1-1.html](http://bbs.ywlm.net/thread-855-1-1.html)
[http://94j69.blog.51cto.com/542780/1054768](http://94j69.blog.51cto.com/542780/1054768)
