# ActiveMQ集群方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年04月20日 13:19:02[boonya](https://me.csdn.net/boonya)阅读数：1028








# 1、综述

通过之前的文章，我们讨论了ActiveMQ的基本使用，包括单个ActiveMQ服务节点的性能特征，关键调整参数；我们还介绍了单个ActiveMQ节点上三种不同的持久化存储方案，并讨论了这三种不同的持久化存储方案的配置和性能特点。但是这还远远不够，因为在生产环境中为了保证让我们设计的消息服务方案能够持续工作，我们还需要为消息中间件服务搭建集群环境，从而在保证消息中间件服务可靠性和处理性能。

# 2、ActiveMQ多节点方案

集群方案主要为了解决系统架构中的两个关键问题：高可用和高性能。ActiveMQ服务的高可用性是指，在ActiveMQ服务性能不变、数据不丢失的前提下，确保当系统灾难出现时ActiveMQ能够持续提供消息服务，高可靠性方案**最终目的是减少整个ActiveMQ停止服务的时间**。

ActiveMQ服务的高性能是指，在保证ActiveMQ服务持续稳定性、数据不丢失的前提下，确保ActiveMQ集群能够在单位时间内吞吐更高数量的消息、确保ActiveMQ集群处理单条消息的时间更短、确保ActiveMQ集群能够容纳更多的客户端稳定连接。

下面我们分别介绍如何通过多个ActiveMQ服务节点集群方式，分别提供热备方案和高性能方案。最后我们讨论如何将两种方案结合在一起，最终形成在生成环境下使用的推荐方案。

## 2-2、ActiveMQ高性能方案

ActiveMQ的多节点集群方案，主要有动态集群和静态集群两种方案。所谓动态集群就是指，**同时提供消息服务的ActiveMQ节点数量、位置（IP和端口）是不确定的**，当某一个节点启动后，会通过网络组播的方式向其他节点发送通知（同时接受其他节点的组播信息）。当网络中其他节点收到组播通知后，就会向这个节点发起连接，最终将新的节点加入ActiveMQ集群；所谓静态集群是指**同时提供消息服务的多个节点的位置（IP和端口）是确定的**，每个节点不需要通过广播的方式发现目标节点，只需要在启动时按照给定的位置进行连接。
- 
静态集群方案

![这里写图片描述](https://img-blog.csdn.net/20160417161519803)

- 
动态集群方案


![这里写图片描述](https://img-blog.csdn.net/20160417161738347)

### 2-1-1、基于组播（multicast）的节点发现

在使用动态集群配置时，当某个ActiveMQ服务节点启动后并不知道整个网络中还存在哪些其他的服务节点。所以ActiveMQ集群需要规定一种节点与节点间的发现机制，以保证能够解决上述问题。**ActiveMQ集群中，使用“组播”原理进行其他节点的发现**。

**组播（multicast）基于UDP协议**，它是指在一个可连通的网络中，某一个数据报发送源向一组数据报接收目标进行操作的过程。在这个过程中，数据报发送者只需要向这个组播地址（一个D类IP）发送一个数据报，那么加入这个组播地址的所有接收者都可以收到这个数据报。组播实现了网络中单点到多点的高效数据传送，能够节约大量网络带宽，降低网络负载。

![这里写图片描述](https://img-blog.csdn.net/20160415103927283)

在IP协议中，规定的D类IP地址为组播地址。224.0.0.0~239.255.255.255这个范围内的IP都是D类IP地址，其中有一些IP段是保留的有特殊含义的：
- 
224.0.0.0～224.0.0.255：这个D类IP地址段为保留地址，不建议您在开发过程中使用，因为可能产生冲突。例如224.0.0.5这个组播地址专供OSPF协议（是一种路由策略协议，用于找到最优路径）使用的组播地址；224.0.0.18这个组播地址专供VRRP协议使用（VRRP协议是虚拟路由器冗余协议）。

- 
224.0.1.0～224.0.1.255：这个D类IP地址为公用组播地址，用于在整个Internet网络上进行组播。除非您有顶级DNS的控制/改写权限，否则不建议在局域网内使用这个组播地址断。

- 
239.0.0.0～239.255.255.255：这个D类IP地址段为推荐在局域网内使用的组播地址段。**注意，如果要在局域网内使用组播功能，需要局域网中的交换机/路由器支持组播功能**。幸运的是，目前市面上只要不是太过低端的交换机/路由器，都支持组播功能（组播功能所使用的主要协议为IGMP协议，关于IGMP协议的细节就不再进行深入了）。


下面我们使用java语言，编写一个局域网内的组播发送和接受过程。以便让各位读者对基于组播的节点发现操作有一个直观的理解。虽然ActiveMQ中关于节点发现的过程，要比以下的示例复杂得多，但是基本原理是不会改变的。
- 组播数据报发送者：
<code class="hljs java has-numbering"><span class="hljs-keyword">package</span> multicast;

<span class="hljs-keyword">import</span> java.net.DatagramPacket;
<span class="hljs-keyword">import</span> java.net.InetAddress;
<span class="hljs-keyword">import</span> java.net.MulticastSocket;
<span class="hljs-keyword">import</span> java.util.Date;

<span class="hljs-keyword">public</span> <span class="hljs-class"><span class="hljs-keyword">class</span> <span class="hljs-title">SendMulticast</span> {</span>
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">static</span> <span class="hljs-keyword">void</span> <span class="hljs-title">main</span>(String[] args) <span class="hljs-keyword">throws</span> Throwable {
        <span class="hljs-comment">// 组播地址</span>
        InetAddress group = InetAddress.getByName(<span class="hljs-string">"239.0.0.5"</span>);
        <span class="hljs-comment">// 组播端口，同时也是UDP 数据报的发送端口</span>
        <span class="hljs-keyword">int</span> port = <span class="hljs-number">19999</span>;
        MulticastSocket mss = <span class="hljs-keyword">null</span>;  

        <span class="hljs-comment">// 创建一个用于发送/接收的MulticastSocket组播套接字对象</span>
        mss = <span class="hljs-keyword">new</span> MulticastSocket(port);
        <span class="hljs-comment">// 创建要发送的组播信息和UDP数据报</span>
        <span class="hljs-comment">// 携带的数据内容，就是这个activeMQ服务节点用来提供Network Connectors的TCP/IP地址和端口等信息</span>
        String message = <span class="hljs-string">"我是一个活动的activeMQ服务节点（节点编号:yyyyyyy），我的可用tcp信息为：XXXXXXXXXX : "</span>;  
        <span class="hljs-keyword">byte</span>[] buffer2 = message.getBytes(); 
        DatagramPacket dp = <span class="hljs-keyword">new</span> DatagramPacket(buffer2, buffer2.length, group, port);
        <span class="hljs-comment">// 使用组播套接字joinGroup(),将其加入到一个组播</span>
        mss.joinGroup(group);

        <span class="hljs-comment">// 开始按照一定的周期向加入到224.0.0.5组播地址的其他ActiveMQ服务节点进行广播</span>
        Thread thread = Thread.currentThread();
        <span class="hljs-keyword">while</span> (!thread.isInterrupted()) {
            <span class="hljs-comment">// 使用组播套接字的send()方法，将组播数据包对象放入其中，发送组播数据包</span>
            mss.send(dp);
            System.out.println(<span class="hljs-keyword">new</span> Date() + <span class="hljs-string">"发起组播："</span> + message);
            <span class="hljs-keyword">synchronized</span> (SendMulticast.class) {
                SendMulticast.class.wait(<span class="hljs-number">5000</span>);
            }
        }

        mss.close();
    }
}</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li></ul>- 组播数据报接收者：
<code class="hljs java has-numbering"><span class="hljs-keyword">package</span> multicast;

<span class="hljs-keyword">import</span> java.net.DatagramPacket;
<span class="hljs-keyword">import</span> java.net.InetAddress;
<span class="hljs-keyword">import</span> java.net.MulticastSocket;

<span class="hljs-javadoc">/**
 * 测试接收组播信息
 *<span class="hljs-javadoctag"> @author</span> yinwenjie
 */</span>
<span class="hljs-keyword">public</span> <span class="hljs-class"><span class="hljs-keyword">class</span> <span class="hljs-title">AcceptMulticast</span> {</span>
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">static</span> <span class="hljs-keyword">void</span> <span class="hljs-title">main</span>(String[] args) <span class="hljs-keyword">throws</span> Throwable {
        <span class="hljs-comment">// 建立组播套接字，并加入分组</span>
        MulticastSocket multicastSocket = <span class="hljs-keyword">new</span> MulticastSocket(<span class="hljs-number">19999</span>);
        <span class="hljs-comment">// 注意，组播地址和端口必须和发送者的一直，才能加入正确的组</span>
        InetAddress ad = InetAddress.getByName(<span class="hljs-string">"239.0.0.5"</span>);
        multicastSocket.joinGroup(ad);

        <span class="hljs-comment">// 准备接收可能的组播信号</span>
        <span class="hljs-keyword">byte</span>[] datas = <span class="hljs-keyword">new</span> <span class="hljs-keyword">byte</span>[<span class="hljs-number">2048</span>];
        DatagramPacket data = <span class="hljs-keyword">new</span> DatagramPacket(datas, <span class="hljs-number">2048</span> ,ad , <span class="hljs-number">19999</span>);
        Thread thread = Thread.currentThread();

        <span class="hljs-comment">// 开始接收组播信息，并打印出来</span>
        System.out.println(<span class="hljs-string">".....开始接收组播信息....."</span>);
        <span class="hljs-keyword">while</span>(!thread.isInterrupted()) {
            multicastSocket.receive(data);
            <span class="hljs-keyword">int</span> leng = data.getLength();
            System.out.println(<span class="hljs-keyword">new</span> String(data.getData() , <span class="hljs-number">0</span> , leng , <span class="hljs-string">"UTF-8"</span>));
        }

        multicastSocket.close();
    }
}</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li></ul>
另外，我们之前讲过的DUBBO框架中，也有基于“组播”的发现/注册管理。具体可参考DUBBO框架中的com.alibaba.dubbo.registry.multicast.MulticastRegistry类和其引用类（以下为MulticastRegistry类中，创建组播套接字和接受组播数据报的关键代码段）：
<code class="hljs avrasm has-numbering">......
mutilcastAddress = InetAddress<span class="hljs-preprocessor">.getByName</span>(url<span class="hljs-preprocessor">.getHost</span>())<span class="hljs-comment">;</span>
mutilcastPort = url<span class="hljs-preprocessor">.getPort</span>() <= <span class="hljs-number">0</span> ? DEFAULT_MULTICAST_PORT : url<span class="hljs-preprocessor">.getPort</span>()<span class="hljs-comment">;</span>
mutilcastSocket = new MulticastSocket(mutilcastPort)<span class="hljs-comment">;</span>
mutilcastSocket<span class="hljs-preprocessor">.setLoopbackMode</span>(false)<span class="hljs-comment">;</span>
mutilcastSocket<span class="hljs-preprocessor">.joinGroup</span>(mutilcastAddress)<span class="hljs-comment">;</span>
Thread thread = new Thread(new Runnable() {
    public void run() {
        byte[] buf = new byte[<span class="hljs-number">2048</span>]<span class="hljs-comment">;</span>
        DatagramPacket recv = new DatagramPacket(buf, buf<span class="hljs-preprocessor">.length</span>)<span class="hljs-comment">;</span>
        while (! mutilcastSocket<span class="hljs-preprocessor">.isClosed</span>()) {
            try {
                mutilcastSocket<span class="hljs-preprocessor">.receive</span>(recv)<span class="hljs-comment">;</span>
                String msg = new String(recv<span class="hljs-preprocessor">.getData</span>())<span class="hljs-preprocessor">.trim</span>()<span class="hljs-comment">;</span>
                int i = msg<span class="hljs-preprocessor">.indexOf</span>(<span class="hljs-string">'\n'</span>)<span class="hljs-comment">;</span>
                if (i > <span class="hljs-number">0</span>) {
                    msg = msg<span class="hljs-preprocessor">.substring</span>(<span class="hljs-number">0</span>, i)<span class="hljs-preprocessor">.trim</span>()<span class="hljs-comment">;</span>
                }
                MulticastRegistry<span class="hljs-preprocessor">.this</span><span class="hljs-preprocessor">.receive</span>(msg, (InetSocketAddress) recv<span class="hljs-preprocessor">.getSocketAddress</span>())<span class="hljs-comment">;</span>
                Arrays<span class="hljs-preprocessor">.fill</span>(buf, (byte)<span class="hljs-number">0</span>)<span class="hljs-comment">;</span>
            } catch (Throwable e) {
                if (! mutilcastSocket<span class="hljs-preprocessor">.isClosed</span>()) {
                    logger<span class="hljs-preprocessor">.error</span>(e<span class="hljs-preprocessor">.getMessage</span>(), e)<span class="hljs-comment">;</span>
                }
            }
        }
    }
}, <span class="hljs-string">"DubboMulticastRegistryReceiver"</span>)<span class="hljs-comment">;</span>
thread<span class="hljs-preprocessor">.setDaemon</span>(true)<span class="hljs-comment">;</span>
thread<span class="hljs-preprocessor">.start</span>()<span class="hljs-comment">;</span>
......</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li></ul>
### 2-1-2、桥接Network Bridges

为了实现ActiveMQ集群的横向扩展要求和高稳定性要求，ActiveMQ集群提供了Network Bridges功能。通过Network Bridges功能，技术人员可以将多个ActiveMQ服务节点连接起来。并让它们通过配置好的策略作为一个整体对外提供服务。

这样的服务策略主要包括两种：主/从模式和负载均衡模式。对于第一种策略我们会在后文进行讨论。本节我们要重点讨论的是基于Network Bridges的负载均衡模式。

![这里写图片描述](https://img-blog.csdn.net/20160416091341282)

### 2-1-3、动态Network Connectors

既然已经讲述了ActiveMQ中的动态节点发现原理和ActiveMQ Network Bridges的概念，那么关于ActiveMQ怎样配置集群的方式就是非常简单的问题了。我们先来讨论如何进行基于组播发现的ActiveMQ负载均衡模式的配置——动态网络连接Network Connectors；再来讨论基于固定地址的负载均衡模式配置——静态网络连接Network Connectors。

要配置基于组播发现的ActiveMQ负载均衡模式，其过程非常简单。开发人员只需要在每一个ActiveMQ服务节点的主配置文件中（activemq.xml），添加/更改 以下配置信息即可：
<code class="hljs xml has-numbering">......
<span class="hljs-tag"><<span class="hljs-title">transportConnectors</span>></span>
    <span class="hljs-comment"><!-- 在transportConnector中增加discoveryUri属性，表示这个transportConnector是要通过组播告知其它节点的：使用这个transportConnector位置连接我 --></span>
    <span class="hljs-tag"><<span class="hljs-title">transportConnector</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"auto"</span> <span class="hljs-attribute">uri</span>=<span class="hljs-value">"auto+nio://0.0.0.0:61616?maximumConnections=1000&wireFormat.maxFrameSize=104857600&org.apache.activemq.transport.nio.SelectorManager.corePoolSize=20&org.apache.activemq.transport.nio.SelectorManager.maximumPoolSize=50&consumer.prefetchSize=5"</span> <span class="hljs-attribute">discoveryUri</span>=<span class="hljs-value">"multicast://239.0.0.5"</span> /></span>
<span class="hljs-tag"></<span class="hljs-title">transportConnectors</span>></span>

......

<span class="hljs-comment"><!-- 关键的networkConnector标签， uri属性标示为组播发现--></span>
<span class="hljs-tag"><<span class="hljs-title">networkConnectors</span>></span>
    <span class="hljs-tag"><<span class="hljs-title">networkConnector</span> <span class="hljs-attribute">uri</span>=<span class="hljs-value">"multicast://239.0.0.5"</span> <span class="hljs-attribute">duplex</span>=<span class="hljs-value">"false"</span>/></span>
<span class="hljs-tag"></<span class="hljs-title">networkConnectors</span>></span>

......</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li></ul>
#### 2-1-3-1：networkConnector标签

如果使用ActiveMQ的组播发现功能，请在networkConnector标签的uri属性中添加如下格式的信息：
<code class="hljs markdown has-numbering">multicast://[<span class="hljs-link_label">组播地址</span>][<span class="hljs-link_reference">:端口</span>]</code><ul style="" class="pre-numbering"><li>1</li></ul><ul style="" class="pre-numbering"><li>1</li></ul>
例如，您可以按照如下方式使用ActiveMQ默认的组播地址来发现网络种其他ActiveMQ服务节点：
<code class="hljs haskell has-numbering"><span class="hljs-preprocessor">#ActiveMQ集群默认的组播地址（239.255.2.3）：</span>
<span class="hljs-title">multicast</span>://<span class="hljs-default"><span class="hljs-keyword">default</span></span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li></ul>
也可以按照如下方式，指定一个组播地址——这在高安全级别的网络中很有用，因为可能其他的组播地址已经被管理员禁用。注意组播地址只能是D类IP地址段：
<code class="hljs vala has-numbering"><span class="hljs-preprocessor">#使用组播地址239.0.0.5</span>
multicast:<span class="hljs-comment">//239.0.0.5</span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li></ul>
以下是通过抓包软件获得的的组播UDP报文：

![这里写图片描述](https://img-blog.csdn.net/20160415155643426)

从上图中我们可以获得几个关键信息：
- 
192.168.61.138和192.168.61.139这两个IP地址分别按照一定的周期（1秒一次），向组播地址239.0.0.5发送UDP数据报。以便让在这个组播地址的其它服务节点能够感知自己的存在

- 
另外，以上UDP数据报文使用的端口是6155。您也可以更改这个端口信息通过类似如下的方式：

<code class="hljs vala has-numbering"><span class="hljs-preprocessor">#使用组播地址239.0.0.5:19999</span>
multicast:<span class="hljs-comment">//239.0.0.5:19999</span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li></ul>- 每个UDP数据报中，包含的主要信息包括本节点ActiveMQ的版本信息，以及连接到自己所需要使用的host名字、协议名和端口信息。类似如下：
<code class="hljs mel has-numbering"><span class="hljs-keyword">default</span>.ActiveMQ-<span class="hljs-number">4.</span>ailve<span class="hljs-variable">%localhost</span><span class="hljs-variable">%auto</span>+nio:<span class="hljs-comment">//activemq:61616</span></code><ul style="" class="pre-numbering"><li>1</li></ul><ul style="" class="pre-numbering"><li>1</li></ul>
#### 2-1-3-2：transportConnector标签的关联设置

任何一个ActiveMQ服务节点A，要连接到另外的ActiveMQ服务节点，都需要使用当前节点A已经公布的transportConnector连接端口，例如以下配置中，能够供其它服务节点进行连接的就只有两个transportConnector连接中的任意一个：
<code class="hljs xml has-numbering">......
<span class="hljs-tag"><<span class="hljs-title">transportConnectors</span>></span>
    <span class="hljs-comment"><!-- 其它ActiveMQ服务节点，只能使用以下三个连接协议和端口进行连接 --></span>
    <span class="hljs-comment"><!-- DOS protection, limit concurrent connections to 1000 and frame size to 100MB --></span>
    <span class="hljs-tag"><<span class="hljs-title">transportConnector</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"tcp"</span> <span class="hljs-attribute">uri</span>=<span class="hljs-value">"tcp://0.0.0.0:61614?maximumConnections=1000&wireFormat.maxFrameSize=104857600"</span>/></span>
    <span class="hljs-tag"><<span class="hljs-title">transportConnector</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"nio"</span> <span class="hljs-attribute">uri</span>=<span class="hljs-value">"nio://0.0.0.0:61618?maximumConnections=1000"</span> /></span>
    <span class="hljs-tag"><<span class="hljs-title">transportConnector</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"auto"</span> <span class="hljs-attribute">uri</span>=<span class="hljs-value">"auto://0.0.0.0:61617?maximumConnections=1000"</span> /></span>   
<span class="hljs-tag"></<span class="hljs-title">transportConnectors</span>></span>
......</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li></ul>
那么要将哪一个连接方式通过UDP数据报向其他ActiveMQ节点进行公布，就需要在transportConnector标签上使用discoveryUri属性进行标识，如下所示：
<code class="hljs lasso has-numbering"><span class="hljs-attribute">...</span><span class="hljs-attribute">...</span>
<span class="hljs-subst"><</span>transportConnectors<span class="hljs-subst">></span>
    <span class="hljs-attribute">...</span><span class="hljs-attribute">...</span>
    <span class="hljs-subst"><</span>transportConnector name<span class="hljs-subst">=</span><span class="hljs-string">"ws"</span> uri<span class="hljs-subst">=</span><span class="hljs-string">"ws://0.0.0.0:61614?maximumConnections=1000&wireFormat.maxFrameSize=104857600"</span><span class="hljs-subst">/</span><span class="hljs-subst">></span>
    <span class="hljs-subst"><</span>transportConnector name<span class="hljs-subst">=</span><span class="hljs-string">"auto"</span> uri<span class="hljs-subst">=</span><span class="hljs-string">"auto+nio://0.0.0.0:61616?maximumConnections=1000&wireFormat.maxFrameSize=104857600"</span> discoveryUri<span class="hljs-subst">=</span><span class="hljs-string">"multicast://239.0.0.5"</span> <span class="hljs-subst">/</span><span class="hljs-subst">></span>
<span class="hljs-subst"><</span>/transportConnectors<span class="hljs-subst">></span>

<span class="hljs-attribute">...</span><span class="hljs-attribute">...</span>
<span class="hljs-subst"><</span>networkConnectors<span class="hljs-subst">></span>
    <span class="hljs-subst"><</span>networkConnector uri<span class="hljs-subst">=</span><span class="hljs-string">"multicast://239.0.0.5"</span><span class="hljs-subst">/</span><span class="hljs-subst">></span>
<span class="hljs-subst"><</span>/networkConnectors<span class="hljs-subst">></span>
<span class="hljs-attribute">...</span><span class="hljs-attribute">...</span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li></ul>
#### 2-1-3-3：其他注意事项
- 
关于防火墙：请记得关闭您Linux服务器上对需要公布的IP和端口的限制；

- 
关于hosts路由信息：由于基于组播的动态发现机制，能够找到的是目标ActiveMQ服务节点的机器名，而不是直接找到的IP。所以请设置当前服务节点的hosts文件，以便当前ActiveMQ节点能够通过hosts文件中的IP路由关系，得到机器名与IP的映射：
<code class="hljs asciidoc has-numbering">
# hosts文件

<span class="hljs-code">......
192.168.61.139          activemq1
192.168.61.138          activemq2
......</span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li></ul>
- 
关于哪些协议能够被用于进行Network Bridges连接：根据笔者以往的使用经验，**只有tcp头的uri格式（openwire协议）能够被用于Network Bridges连接**；当然您可以使用auto头，因为其兼容openwire协议；另外，您还可以指定为附加nio头。


### 2-1-4、静态Network Connectors

相比于基于组播发现方式的动态Network Connectors而言，虽然静态Network Connectors没有那样灵活的横向扩展性，但是却可以适用于网络环境受严格管理的情况。例如：管理员关闭了交换机/路由器的组播功能、端口受到严格管控等等。

配置静态Network Connectors的ActiveMQ集群的方式也很简单，只需要更改networkConnectors标签中的配置即可，而无需关联改动transportConnectors标签。但是配置静态Network Connectors的ActiveMQ集群时，需要注意非常关键的细节：**每一个节点都要配置其他所有节点的连接位置**。

为了演示配置过程，我们假设ActiveMQ集群由两个节点构成，分别是activemq1：192.168.61.138 和 activemq2：192.168.61.139。那么配置情况如下所示：
- 192.168.61.138：需要配置activemq2的位置信息以便进行连接：
<code class="hljs xml has-numbering">......
<span class="hljs-tag"><<span class="hljs-title">transportConnectors</span>></span>
    <span class="hljs-tag"><<span class="hljs-title">transportConnector</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"auto"</span> <span class="hljs-attribute">uri</span>=<span class="hljs-value">"auto+nio://0.0.0.0:61616?maximumConnections=1000&wireFormat.maxFrameSize=104857600&consumer.prefetchSize=5"</span>/></span>
<span class="hljs-tag"></<span class="hljs-title">transportConnectors</span>></span>
......

<span class="hljs-comment"><!-- 请注意，一定需要192.168.61.139(activemq2)提供了这样的连接协议和端口 --></span>
<span class="hljs-tag"><<span class="hljs-title">networkConnectors</span>></span>
    <span class="hljs-tag"><<span class="hljs-title">networkConnector</span> <span class="hljs-attribute">uri</span>=<span class="hljs-value">"static:(auto+nio://192.168.61.139:61616)"</span>/></span>
<span class="hljs-tag"></<span class="hljs-title">networkConnectors</span>></span>
......</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li></ul>- 192.168.61.139：需要配置activemq1的位置信息以便进行连接：
<code class="hljs xml has-numbering">......
<span class="hljs-tag"><<span class="hljs-title">transportConnectors</span>></span>
    <span class="hljs-tag"><<span class="hljs-title">transportConnector</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"auto"</span> <span class="hljs-attribute">uri</span>=<span class="hljs-value">"auto+nio://0.0.0.0:61616?maximumConnections=1000&wireFormat.maxFrameSize=104857600&consumer.prefetchSize=5"</span>/></span>
<span class="hljs-tag"></<span class="hljs-title">transportConnectors</span>></span>

......
<span class="hljs-comment"><!-- 请注意，一定需要192.168.61.138(activemq1)提供了这样的连接协议和端口 --></span>
<span class="hljs-tag"><<span class="hljs-title">networkConnectors</span>></span>
   <span class="hljs-tag"><<span class="hljs-title">networkConnector</span> <span class="hljs-attribute">uri</span>=<span class="hljs-value">"static:(auto+nio://192.168.61.138:61616)"</span>/></span>
<span class="hljs-tag"></<span class="hljs-title">networkConnectors</span>></span>
......</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li></ul>
同理，如果您的ActiveMQ集群规划中有三个ActiveMQ服务节点，那么**任何一个节点都应该配置其它两个服务节点的连接方式**。在配置格式中使用“,”符号进行分割：
<code class="hljs asciidoc has-numbering"><span class="hljs-code">......
<networkConnectors>
    <networkConnector uri="static:(tcp://host1:61616,tcp://host2:61616,tcp://..)"/>
</networkConnectors>
......</span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li></ul>
以下是配置完成后可能的效果：
- 192.168.61.138（activemq1）：

![这里写图片描述](https://img-blog.csdn.net/20160416111412861)
- 192.168.61.139（activemq2）：

![这里写图片描述](https://img-blog.csdn.net/20160416111435827)

2-1-5、其他配置属性

下表列举了在networkConnector标签中还可以使用的属性以及其意义。请特别注意其中的duplex属性。如果只从字面意义理解该属性，则被称为“双工模式”；**如果该属性为true，当这个节点使用Network Bridge连接到其它目标节点后，将强制目标也建立Network Bridge进行反向连接。**其目的在于让消息既能发送到目标节点，又可以通过目标节点接受消息，但实际上大多数情况下是没有必要的，因为目标节点一般都会自行建立连接到本节点。所以，该duplex属性的默认值为false。
|属性名称|默认值|属性意义|
|----|----|----|
|name|bridge|名称|
|dynamicOnly|false|如果为true, 持久订阅被激活时才创建对应的网路持久订阅。|
|decreaseNetworkConsumerPriority|false|如果为true，网络的消费者优先级降低为-5。如果为false，则默认跟本地消费者一样为0.|
|excludedDestinations|empty|不通过网络转发的destination|
|dynamicallyIncludedDestinations|empty|通过网络转发的destinations，注意空列表代表所有的都转发。|
|staticallyIncludedDestinations|empty|匹配的都将通过网络转发-即使没有对应的消费者，如果为默认的“empty”，那么说明所有都要被转发|
|duplex|false|已经进行详细介绍的“双工”属性。|
|prefetchSize|1000|设置网络消费者的prefetch size参数。如果设置成0，那么就像之前文章介绍过的那样：消费者会自己轮询消息。显然这是不被允许的。|
|suppressDuplicateQueueSubscriptions|false|如果为true, 重复的订阅关系一产生即被阻止（V5.3+ 的版本中可以使用）。|
|bridgeTempDestinations|true|是否广播advisory messages来创建临时destination。|
|alwaysSyncSend|false|如果为true，非持久化消息也将使用request/reply方式代替oneway方式发送到远程broker（V5.6+ 的版本中可以使用）。|
|staticBridge|false|如果为true，只有staticallyIncludedDestinations中配置的destination可以被处理（V5.6+ 的版本中可以使用）。|

以下这些属性，只能在静态Network Connectors模式下使用
|属性名称|默认值|属性意义|
|----|----|----|
|initialReconnectDelay|1000|重连之前等待的时间(ms) (如果useExponentialBackOff为false)|
|useExponentialBackOff|true|如果该属性为true，那么在每次重连失败到下次重连之前，都会增大等待时间|
|maxReconnectDelay|30000|重连之前等待的最大时间(ms)|
|backOffMultiplier|2|增大等待时间的系数|

请注意这些属性，并不是networkConnector标签的属性，而是在uri属性中进行设置的，例如：
<code class="hljs ini has-numbering"><span class="hljs-setting">uri=<span class="hljs-value"><span class="hljs-string">"static:(tcp://host1:61616,tcp://host2:61616)?maxReconnectDelay=5000&useExponentialBackOff=false"</span></span></span></code>转自：[http://blog.csdn.net/yinwenjie/article/details/51124749](http://blog.csdn.net/yinwenjie/article/details/51124749)



