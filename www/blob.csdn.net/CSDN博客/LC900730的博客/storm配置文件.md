# storm配置文件 - LC900730的博客 - CSDN博客
2017年12月05日 17:19:34[lc900730](https://me.csdn.net/LC900730)阅读数：268
#### storm.zookeeper.servers:
```
- "centos701"
- "centos702"
```
#### storm.local.dir:”/data/storm”
- storm的numbus和worker进程需要一个目录来存放一小部分状态数据，如jars、confs等等。我们需要在每台机器上创建这个目录并且赋予响应权限。
#### numbus.host:”centos701”
- worker节点需要知道哪个机器是master节点，便于自己从mastre节点上下载jars和confs。
#### supervisor.slots.ports:
```
- 6700
- 6701
- 6702
- 6703
```
对于每一台worker机器，它决定了这台机器一共可以运行多少个worker进程。每个进程会独占一个端口来接收消息，这个参数就是配置了哪些端口会分配给worker进程。配置5个端口storm会分配5个worker进程给这台机器，配置3个会分配3个。默认会分配4个6700-6703
### 启动：
- Nimbus:   bin/storm nimbus
- Supervisor: 在每一台worker节点运行”bin/storm supervisor”,supervisor进程负责在worker节点上启动和停止相应的worker进程。
- UI：运行bin/storm ui 一个通过页面管理和展示storm集群运行状态的工具。nimbus host:8080来访问。
- log目录会保存在storm/logs目录中。storm通过logback来管理它的日志，可以通过修改其logback.xml文件来修改log目录及内容。
## 1.编程模型
##### DataSource：外部数据源
##### Spout：接受外部数据源的组件，将外部数据源转化成storm内部数据，以Tuple为基本的传输单元下发给bolt
##### Bolt：接受Spout发送的数据，或上游bolt发送的数据，根据业务逻辑进行处理。发送给下一个Bolt或者是存储到某种介质上，如redis、mysql。
##### Tuple：Storm内部中数据传输的基本单元，封装了一个List对象，用来保存数据。
##### StreamGrouping：数据分组策略，7种。
## 2.并发度
##### 用户指定的一个任务，可以被多个线程执行，并发度的数量等于线程数量。一个任务的多个线程，会被运行在多个worker(JVM)上，有一种类似于平均算法的负载均衡策略，尽可能减少网络IO。
## 3.架构
##### Nimbus：任务分配。
##### Supervisor：接受任务，并启动worker。worker数量根据端口号来的。
##### Worker：执行任务的具体组件(其实就是一个JVM)，可以执行两种类型的任务，spout或者bolt任务。
##### Task：=线程=executor。一个task属于一个spout或者bolt并发任务。
##### Zookeeper：保存任务分配的信息、心跳信息、元数据信息。
## 4.Worker与topology
一个worker只属于一个topology，反之一个topology包含多个worker。
## storm.yaml配置文件
```
storm.zookeeper.servers:
      - "centos701"
      - "centos702"
      - "centos703"
storm.local.dir: "/data/local/storm"
storm.cluster.mode: "distributed"
storm.zookeeper.root: "/data/stormroot"
storm.zookeeper.session.timeout: 60000
nimbus.host: centos701
storm.log.dir: /data/stormlog/
```
