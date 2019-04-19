# Docker入门系列7 动态映射端口port mapping - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月09日 12:57:19[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：519
为何想要动态映射端口呢？
因为刚开始run启动容器时,并不知道里面需要映射哪些端口，等容器已创建了，想映射端口。
当然可以通过先commit成镜像，然后再次run时指定端口，但会生成中间的镜像，对于有轻微洁癖的我是不允许这种情况发生的。
我们可以对比映射前后iptables-save的变化（即没有加-p与加了-p选项后对比）：
docker run -i -t --name test -p 8080:80 1f4fd79edf6d /bin/bash
iptables-save的结果
*****nat
-A DOCKER **!**-i docker0 -p tcp -m tcp --dport8080-j DNAT --to-destination 172.17.0.2:80*****filter
-A FORWARD -d 172.17.0.2**/**32**!**-i docker0 -o docker0 -p tcp -m tcp --dport80-j ACCEPT
通过“iptables -t table名 -L”查看：
filter表
Chain FORWARD
ACCEPT     tcp  --  anywhere             localhost            tcp dpt:http
 
nat表
Chain DOCKER
DNAT       tcp  --  anywhere             anywhere             tcp dpt:webcache to:172.17.0.2:80
可以看到filter表的FORWARD链和nat表的DOCKER链各添加了一条规则。
受此启发，从而我们可以通过如下方法动态映射端口：
iptables -t nat -A DOCKER **!**-i docker0 -p tcp -m tcp --dport8080-j DNAT --to-destination 172.17.0.4:80
iptables -I FORWARD -d 172.17.0.4**/**32**!**-i docker0 -o docker0 -p tcp -m tcp --dport80-j ACCEPT
把“iptables -A FORWARD”改成“-I”放在第一条，用-A是添加到最后，用-I是放在最前面。
如果误操作了，可以通过如下方法删除：
iptables -t table名 -D chain名 rulenum **[**options**]****//**rulenum 是规则的编号。从1 开始。可以使用--line-numbers 列出规则的编号
iptables -t nat -L--line-numbers
iptables -t nat -D DOCKER 1
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%977:%E5%8A%A8%E6%80%81%E6%98%A0%E5%B0%84%E7%AB%AF%E5%8F%A3port_mapping&action=edit&section=1)]艺搜参考
[http://jar-c.blog.163.com/blog/static/116401250201061541150973/](http://jar-c.blog.163.com/blog/static/116401250201061541150973/)
[http://stackoverflow.com/questions/19897743/exposing-a-port-on-a-live-docker-container](http://stackoverflow.com/questions/19897743/exposing-a-port-on-a-live-docker-container)
