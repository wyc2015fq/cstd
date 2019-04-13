
# Redis集群添加节点 - 高科的专栏 - CSDN博客

2016年02月26日 12:21:41[高科](https://me.csdn.net/pbymw8iwm)阅读数：698


**Redis****集群添加节点**
1：首先把需要添加的节点启动
|cd /usr/local/cluster/
|mkdir 7006
|cp /usr/local/cluster/redis.conf  /usr/local/cluster/7006/
|cd /usr/local/cluster/7006/
|vi redis.conf
|\#\#|修改|redis.conf|中的|port|参数的值为|7006
|redis-server redis.conf
|
2：执行以下命令，将这个新节点添加到集群中
|cd /usr/local/redis3.0/src/
|./redis-trib.rb add-node 127.0.0.1:7006 127.0.0.1:7000
|
![](https://img-blog.csdn.net/20150121184531187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHU0NzA0MzgwMDA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3：执行命令redis-cli -c -p 7000 cluster nodes，查看刚才新增的节点
![](https://img-blog.csdn.net/20150121184626998?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHU0NzA0MzgwMDA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4：增加了新的节点之后，这个新的节点可以成为主节点或者是从节点
4.1把这个节点变成主节点，使用redis-trib程序，将集群中的某些哈希槽移动到新节点里面， 这个新节点就成为真正的主节点了。
执行下面的命令对集群中的哈希槽进行移动
|cd /usr/local/redis3.0/src
|./redis-trib.rb reshard 127.0.0.1:7000
|
系统会提示我们要移动多少哈希槽，这里移动1000个
![](https://img-blog.csdn.net/20150121184649321?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHU0NzA0MzgwMDA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后还需要指定把这些哈希槽转移到哪个节点上，
![](https://img-blog.csdn.net/20150121184636406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHU0NzA0MzgwMDA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
输入我们刚才新增的节点的ID
f32dc088c881a6b930474fc5b52832ba2ff71899
然后需要我们指定转移哪几个几点的哈希槽
![](https://img-blog.csdn.net/20150121184724000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHU0NzA0MzgwMDA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
输入all表示从所有的主节点中随机转移，凑够1000个哈希槽
然后再输入yes，redis集群就开始分配哈希槽了。
至此，一个新的主节点就添加完成了，执行命令查看现在的集群中节点的状态
|redis-cli -c -p 7000 cluster nodes
|
![](https://img-blog.csdn.net/20150121184740817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHU0NzA0MzgwMDA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4.2：把这个节点变成从节点
前面我们已经把这个新节点添加到集群中了，现在我们要让新节点成为127.0.0.1:7001的从节点，只需要执行下面的命令就可以了，命令后面的节点ID就是127.0.0.1:7001的节点ID
|redis-cli -c -p 7006 cluster replicate 0b00721a509444db793d28448d8f02168b94bd38
|
使用下面命令来确认一下127.0.0.1:7006是否已经成为127.0.0.1:7001的从节点
|redis-cli -p 7000 cluster nodes | grep slave | grep 0b00721a509444db793d28448d8f02168b94bd38
|
看到下面图片中的情况就表示添加成功
![](https://img-blog.csdn.net/20150121184757758?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHU0NzA0MzgwMDA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



### Redis
### 集群删除节点
1：如果删除的节点是主节点，这里我们删除127.0.0.1:7006节点，这个节点有1000个哈希槽
首先要把节点中的哈希槽转移到其他节点中，执行下面的命令
|cd /usr/local/redis3.0/src
|./redis-trib.rb reshard 127.0.0.1:7000
|
系统会提示我们要移动多少哈希槽，这里移动1000个，因为127.0.0.1:7006节点有1000个哈希槽
然后系统提示我们输入要接收这些哈希槽的节点的ID，这里使用127.0.0.1:7001的节点ID
然后要我们选择从那些节点中转出哈希槽，这里一定要输入127.0.0.1:7006这个节点的ID，最后输入done表示输入完毕
最后一步，使用下面的命令把这个节点删除
|cd /usr/local/redis3.0/src/
|./redis-trib.rb del-node 127.0.0.1:7006 127.0.0.1:7006
|
2：如果节点是从节点的，直接使用下面的命令删除即可。
|cd /usr/local/redis3.0/src/
|./redis-trib.rb del-node 127.0.0.1:7006 127.0.0.1:7006

|

