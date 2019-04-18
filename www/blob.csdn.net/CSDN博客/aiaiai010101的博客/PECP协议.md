# PECP协议 - aiaiai010101的博客 - CSDN博客

2018年11月17日 16:05:56[aiaiai010101](https://me.csdn.net/aiaiai010101)阅读数：42


**本文转载自：[https://blog.csdn.net/baidu_38316985/article/details/81905281](https://blog.csdn.net/baidu_38316985/article/details/81905281)**

1.Path Computation Element Protocol (PCEP) as a southbound plugin in ONOS.

路径计算单元协议，ONOS的南向接口协议之一。

（来源[https://wiki.onosproject.org/display/ONOS/PCEP+Protocol](https://wiki.onosproject.org/display/ONOS/PCEP+Protocol)）

2.ONOS是专门面向服务提供商和企业骨干网的开源SDN网络操作系统。ONOS控制器是由一家名为开放网络实验室（ON.Lab）的非盈利性组织打造的基于ONOS的一款开源的商用控制器，并于美国时间2014年12月5日全球首发，当前版本Hummingbird。ONOS旨在为服务提供商和企业骨干网提供高可用性、可横向扩展及高性能的网络需求。

（来源 [https://developer.huawei.com/ict/cn/site-sdn-onos/](https://developer.huawei.com/ict/cn/site-sdn-onos/)）

3.**PCE是一个典型的域内交换路径的集中控制方案。**PCE对IP领域里的人而言相对比较陌生，其实在光网络早有应用。PCE本身是在ASON（自动交换光网络）解决方案标准中定义的一个组件，用于端到端路径的集中计算的方法。**其理念就是用一个专门的服务器，用于集中算路，其它的网元都不管。**你可能会问，光网络本身就是集中控制的呀，为啥要PCE这玩意。没错，光网络主要是通过网管或OSS，人工静态配置的，而ASON是，平时事先静态配置好E2E路径，但故障时，自动触发重路由计算。ASON最早是每个网元自己算，这样就出现了管理规模问题：涉及到全网拓扑数据同步，以及E2E计算，因此**需要PCE来解决分布式ASON的单机性能有限，导致管理规模无法快速增长的问题。PCE自身计算能力如果不足，还可以级联。**

PCE的集中计算这种核心思路跟现在的SDN很匹配，因此，PCE的思路可以用于SDN中，作为MPLS网络向SDN网络迁移的方案之一。

（来源：[https://blog.csdn.net/huawei_esdk/article/details/50920823](https://blog.csdn.net/huawei_esdk/article/details/50920823)）

4.South Bound Interface (SBI)

MPLS TE tunnel management 

port 4189 

**流量工程**（traffic engineering，简称TE），是设计流量使之能够在现有的网络中正常传送；

（来源[https://www.sohu.com/a/164078277_505810](https://www.sohu.com/a/164078277_505810)）

MPLS TE相关的技术包括：[多协议标签交换](https://baike.baidu.com/item/%E5%A4%9A%E5%8D%8F%E8%AE%AE%E6%A0%87%E7%AD%BE%E4%BA%A4%E6%8D%A2)（MPLS），进行了流量工程扩展的[资源预留协议](https://baike.baidu.com/item/%E8%B5%84%E6%BA%90%E9%A2%84%E7%95%99%E5%8D%8F%E8%AE%AE)(RSVP-TE)，基于约束的标签分配协议(CR－LDP),基于约束的[路由协议](https://baike.baidu.com/item/%E8%B7%AF%E7%94%B1%E5%8D%8F%E8%AE%AE)（QoS[路由技术](https://baike.baidu.com/item/%E8%B7%AF%E7%94%B1%E6%8A%80%E6%9C%AF)），进行了流量工程扩展的[链路状态路由协议](https://baike.baidu.com/item/%E9%93%BE%E8%B7%AF%E7%8A%B6%E6%80%81%E8%B7%AF%E7%94%B1%E5%8D%8F%E8%AE%AE)（[OSPF-TE](https://baike.baidu.com/item/OSPF-TE)和IS-IS TE），快速重路由技术(FRR)。

（来源[https://baike.baidu.com/item/MPLS%20TE/3745342](https://baike.baidu.com/item/MPLS%20TE/3745342)）

