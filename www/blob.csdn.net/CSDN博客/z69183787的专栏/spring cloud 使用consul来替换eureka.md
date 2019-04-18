# spring cloud: 使用consul来替换eureka - z69183787的专栏 - CSDN博客
2019年01月22日 16:52:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：99
个人分类：[Spring Cloud](https://blog.csdn.net/z69183787/article/category/8620817)
[https://www.cnblogs.com/yjmyzz/p/replace-eureka-with-consul.html](https://www.cnblogs.com/yjmyzz/p/replace-eureka-with-consul.html)
eureka官方已经正式宣布：自2.0起不再维护该项目，并在[github 项目wiki](https://github.com/Netflix/eureka/wiki)上放出了一段吓唬人的话:
![点击查看原图](https://images2018.cnblogs.com/blog/27612/201807/27612-20180708162532108-1049429679.png)
大意就是：从2.x起，官方不会继续开发了，如果需要使用2.x，风险自负。但其实我觉得问题并不大，eureka目前的功能已经非常稳定，就算不升级，服务注册/发现这些功能已经够用。
如果想寻找替代方案的话，建议采用功能更为丰富的consul，除了服务注册、发现，consul还提供了k-v存储等其它功能，consul的官网针对其它同类软件也做了详细比较，详见 [consul vs other software](https://www.consul.io/intro/vs/index.html)，有兴趣的可以看看，特别是有一句话，翻译成大白话就是：我不是针对在座的各位，我想说除我之外其它的都是渣渣（足见其相当的自信！）
![点击查看原图](https://images2018.cnblogs.com/blog/27612/201807/27612-20180708163621951-675566595.png)
进入正题，先来看consul的部署安装：
一、集群规划
consul借助agent来运行，类似elk的logstash agent 或 zabbix监控系统的agent ，每个需要被发现的服务上，通过consul agent client 来收集服务本身的信息，然后向consul agent server汇报, consul server 可以集群部署。
规划一下：
|序号|节点ip|节点名称|角色|
|----|----|----|----|
|1|10.0.xx.55|server1|server|
|2|10.0.xx.203|server2|server|
|3|10.0.xx.204|server3|server|
|4|10.0.xx.205|client1|client & web ui|
这是按正式生产环境来规划的，如果本机开发，有一个方便的dev模式（后面再讲）。 上述表格中，我们打算组建3个server节点的consul server cluster，另外有1个client，模拟客户端以及充当consul的web admin ui(管理界面)。
二、下载安装
目前consul的最高版本为1.2.0，只需要把相应的release压缩包 下载到机器上解压即可。
2.1 wget [https://releases.hashicorp.com/consul/1.2.0/consul_1.2.0_linux_amd64.zip](https://releases.hashicorp.com/consul/1.2.0/consul_1.2.0_linux_amd64.zip)
2.2 unzip consul_1.2.0_linux_amd64.zip
假设都解压到~/consul/bin目录下，解压后会得到1个名为consul的可执行文件
2.3 sudo cp ./consul /usr/local/bin
为了方便，可以将其复制到/usr/local/bin下（本步骤可选，需要root权限）
然后检查下是否安装成功：
`➜  ~ consul version`
`Consul v1.2.0`
`Protocol 2 spoken by default, understands 2 to 3 (agent will automatically use protocol >2 when speaking to compatible agents)`
如果出现版本输出，就表示可以了(4个节点上，每台机器上都重复上述操作，全安装好）
三、启动
3.1 启动server端
基本的命令为：
`consul agent -server -bind=10.0.xx.55 -client=0.0.0.0 -bootstrap-expect=3 -data-``dir``=``/data/application/consul_data/``-node=server1`
看着一大堆参数，其实不复杂
-server 表示是以服务端身份启动
-bind 表示绑定到哪个ip（有些服务器会绑定多块网卡，可以通过bind参数强制指定绑定的ip）
-client 指定客户端访问的ip(consul有丰富的api接口，这里的客户端指浏览器或调用方)，0.0.0.0表示不限客户端ip
-bootstrap-expect=3 表示server集群最低节点数为3，低于这个值将工作不正常(注：类似zookeeper一样，通常集群数为奇数，方便选举，consul采用的是raft算法)
-data-dir 表示指定数据的存放目录（该目录必须存在）
-node 表示节点在web ui中显示的名称
启动成功后，终端窗口不要关闭，可以在浏览器里，访问下，类似 http://10.0.xx.55:8500/，正常的话，应该会看到一行文字：Consul Agent。
为了防止终端关闭后，consul退出，可以在刚才命令上，加点东西，类似：
nohup xxx  > /dev/null 2>&1 & 
即:
`nohup``consul agent -server -bind=10.0.xx.55 -client=0.0.0.0 -bootstrap-expect=3 -data-``dir``=``/data/application/consul_data/``-node=server1 > ``/dev/null``2>&1 &`
将其转入后台运行。
另外2台节点上，也做类似操作：
|1|`nohup``consul agent -server -bind=10.0.xx.203 -client=0.0.0.0 -bootstrap-expect=3 -data-``dir``=``/data/application/consul_data/``-node=server2 > ``/dev/null``2>&1 &`|
注意更改bind参数的ip，以及node参数里的节点名称。
|1|`nohup``consul agent -server -bind=10.0.xx.204 -client=0.0.0.0 -bootstrap-expect=3 -data-``dir``=``/data/application/consul_data/``-node=server3 > ``/dev/null``2>&1 &`|
3.2 启动client端
几乎完全一样，只是把-server 去掉，在10.0.xx.205上运行：
|1|`nohup``consul agent -client=0.0.0.0 -data-``dir``=``/data/application/consul_data/``-node=client1  -ui  > ``/dev/null``2>&1 &`|
四、组建cluster
现在我们有了3个server node + 1个client node，但是这4个节点都是相互独立的，可以在任一节点上运行：
consul members
![](https://images2018.cnblogs.com/blog/27612/201807/27612-20180708172235483-1779228628.png)
可以看到，只有自身节点的信息。
要加自己加入集群，可以运行以下命令（假设：其它3个节点，都加入10.0.xx.55）
consul join 10.0.xx.55
成功后，会输出：
Successfully joined cluster by contacting 1 nodes.
其它2个节点（指：10.0.xx.55之外的节点）上类似上述操作，都加入集群中，完成后，可以再次验证
![](https://images2018.cnblogs.com/blog/27612/201807/27612-20180708172548360-1882466074.png)
可以看到4台节点的信息都有了。
tips: 如果反过来，要将1个节点从集群中撤掉，可以在该节点上运行consul leave 即可。
五、web ui
10.0.xx.205上，可能有同学注意到了，启动consul时，我们加了一个  -ui 的参数，这代表将启动consul自带的web管理界面，访问 http://10.0.xx.205:8500/ui
![](https://images2018.cnblogs.com/blog/27612/201807/27612-20180708172752476-13180594.png)
可以看到各节点的信息。
六、服务注册/发现/注销
![](https://images2018.cnblogs.com/blog/27612/201807/27612-20180708173003574-1869144812.png)
现在services里，除了consul外，并没有其它服务注册进来。可以通过api 手动注册一个服务：
用postman（或其它rest api工具、curl都行），向 http://10.0.xx.55:8500/v1/agent/service/register ，发送以下json，http method指定为PUT， Content-Type指定为application/json
|12345678910111213141516171819|`{``    ``"ID"``: ``"my-service-id"``,``    ``"Name"``: ``"my-service-name"``,``    ``"Tags"``: [``        ``"release=1"``,``        ``"mytag=xyz"``    ``],``    ``"Address"``: ``"192.168.1.1"``,``    ``"Port"``: 8000,``    ``"Meta"``: {``        ``"my_version"``: ``"4.0"``    ``},``    ``"EnableTagOverride"``: ``false``,``    ``"Check"``: {``        ``"DeregisterCriticalServiceAfter"``: ``"90m"``,``        ``"HTTP"``: ``"http://yjmyzz.cnblogs.com/"``,``        ``"Interval"``: ``"10s"``    ``}``}　`|
其中的参数名一看就能大概猜出含义，就不详细解释了，想深入研究的，建议参看[consul api](https://www.consul.io/api/agent/service.html)文档。发送完成后，再看web ui
![](https://images2018.cnblogs.com/blog/27612/201807/27612-20180708174246805-876331064.png)
就能看到新注册的服务my-service-name，特别要提一下的是tags，eureka中有metadata-map可以提供元数据，consul中对应的特性为tags。
除了web ui，还可以通过rest接口，发现服务的详细信息：
http://10.0.21.55:8500/v1/agent/services 这个rest api可以列出所有服务：
|123456789101112131415161718192021|`{``    ``"my-service-id"``: {``        ``"Kind"``: ``""``,``        ``"ID"``: ``"my-service-id"``,``        ``"Service"``: ``"my-service-name"``,``        ``"Tags"``: [``            ``"release=1"``,``            ``"mytag=xyz"``        ``],``        ``"Meta"``: {``            ``"my_version"``: ``"4.0"``        ``},``        ``"Port"``: 8000,``        ``"Address"``: ``"192.168.1.1"``,``        ``"EnableTagOverride"``: ``false``,``        ``"CreateIndex"``: 0,``        ``"ModifyIndex"``: 0,``        ``"ProxyDestination"``: ``""``,``        ``"Connect"``: ``null``    ``}``}`|
服务发现，其实就是通过这个api实现的，有服务注册就会有反向操作：服务注销。同样也是通过api完成
curl -X PUT http://10.0.21.55:8500/v1/agent/service/deregister/my-service-id　
这样就把my-service-id这个服务给注销了
七、dev开发者模式
前面这一阵折腾需要好几台机器，本机调试开发时不太方便，为此consul体贴的提供了dev模式，使用方式极为简单
|1|`consul agent -dev`|
开发模式，自带了web ui，直接http://localhost:8500/ 即可，非常方便。　　
八、与spring-cloud的集成
前面这些铺垫，其实就是为了与spring-cloud的集成，非常简单：
|1|`compile(``'org.springframework.cloud:spring-cloud-starter-consul-discovery'``)　　`|
添加对org.springframework.cloud:spring-cloud-starter-consul-discovery的依赖
然后application.yml中配置如下节点：
|1234567891011|`spring:``  ``...``  ``cloud:``    ``consul:``      ``host: ``127.0``.``0.1``      ``port: ``8500``      ``discovery:``        ``tags: version=``1.0``,author=yjmyzz``        ``healthCheckPath: /info.json``        ``healthCheckInterval: 5s``        ``instanceId: ${spring.application.name}:${vcap.application.instance_id:${spring.application.instance_id:${random.value}}}`|
注：
一般在需要部署的微服务机器上，都约定安装consul agent，所以这里host通常指定为127.0.0.1 (本机测试consul可用dev模式启动);
tags 相当于eureka里的metadata-map，大家根据实际需要设置
healthCheckPath 为健康检查的url，可以配置成/health或其它能检测出微服务运行状态的url
healthCheckInterval 为healthcheck的时间间隔
instanceId这一长串，spring cloud官网文档的推荐，为了保证生成一个唯一的id ，也可以换成
|1|`${spring.application.name}:${spring.cloud.client.ipAddress}`|
（即：以ip结尾），这个instanceId即为consul service列表中的service id
![](https://images2018.cnblogs.com/blog/27612/201807/27612-20180708181604283-1894983002.png)
最后提醒一下：如果使用consul来替换eureka，而你的项目中又依赖了eureka的jar包，最好将eureka的自动配置从启动类里排除掉，参考下面：
![](https://images2018.cnblogs.com/blog/27612/201807/27612-20180708181740624-1017901941.png)
注：如果项目的pom/gradle依赖项里，已经移除了org.springframework.cloud:spring-cloud-starter-eureka-server 就不用exclude排除EurekaClientAutoConfiguration，另外上图中@EnableEurekaClient 也可以换成@EnableDiscoveryClient
参考文档：
1、[https://www.consul.io/api/index.html](https://www.consul.io/api/index.html)
2、[https://www.consul.io/intro/getting-started/install.html](https://www.consul.io/intro/getting-started/install.html)
3、[https://cloud.spring.io/spring-cloud-static/spring-cloud-consul/1.3.3.RELEASE/multi/multi_spring-cloud-consul-discovery.html](https://cloud.spring.io/spring-cloud-static/spring-cloud-consul/1.3.3.RELEASE/multi/multi_spring-cloud-consul-discovery.html)
