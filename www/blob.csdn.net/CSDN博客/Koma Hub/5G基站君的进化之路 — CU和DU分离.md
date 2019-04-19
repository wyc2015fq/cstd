# 5G基站君的进化之路 — CU和DU分离 - Koma Hub - CSDN博客
2019年03月19日 21:44:43[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：347
个人分类：[通信技术](https://blog.csdn.net/Rong_Toa/article/category/8758615)
原文：[5G基站君的进化之路 — CU和DU分离](http://mp.weixin.qq.com/s?__biz=MzU3NzM4OTI1OA==&mid=2247484441&idx=1&sn=b82285b469dd3603ae4eb2430d82c029&chksm=fd0413bcca739aaaed8c615d775c96f07f1cc2ff0cb153091bb205e6c8e5d6ea5809b5467228&mpshare=1&scene=1&srcid=0318SFxCca8CWPi54bJo73N4#rd)
**Table of Contents**
[为什么CU和DU要分离？](#%E4%B8%BA%E4%BB%80%E4%B9%88CU%E5%92%8CDU%E8%A6%81%E5%88%86%E7%A6%BB%EF%BC%9F)
[CU和DU到底有什么好处？](#CU%E5%92%8CDU%E5%88%B0%E5%BA%95%E6%9C%89%E4%BB%80%E4%B9%88%E5%A5%BD%E5%A4%84%EF%BC%9F)
[1.实现基带资源的共享](#1.%E5%AE%9E%E7%8E%B0%E5%9F%BA%E5%B8%A6%E8%B5%84%E6%BA%90%E7%9A%84%E5%85%B1%E4%BA%AB)
[2.有利于实现无线接入的切片和云化](#2.%E6%9C%89%E5%88%A9%E4%BA%8E%E5%AE%9E%E7%8E%B0%E6%97%A0%E7%BA%BF%E6%8E%A5%E5%85%A5%E7%9A%84%E5%88%87%E7%89%87%E5%92%8C%E4%BA%91%E5%8C%96)
[3. 满足5G复杂组网情况下的站点协同问题](#3.%20%E6%BB%A1%E8%B6%B35G%E5%A4%8D%E6%9D%82%E7%BB%84%E7%BD%91%E6%83%85%E5%86%B5%E4%B8%8B%E7%9A%84%E7%AB%99%E7%82%B9%E5%8D%8F%E5%90%8C%E9%97%AE%E9%A2%98)
[现在开始部署的5G基站都是基于CU和DU分离架构的吗？](#%E7%8E%B0%E5%9C%A8%E5%BC%80%E5%A7%8B%E9%83%A8%E7%BD%B2%E7%9A%845G%E5%9F%BA%E7%AB%99%E9%83%BD%E6%98%AF%E5%9F%BA%E4%BA%8ECU%E5%92%8CDU%E5%88%86%E7%A6%BB%E6%9E%B6%E6%9E%84%E7%9A%84%E5%90%97%EF%BC%9F)
不论你是否能意识到，移动基站君就隐藏于我们身边，默默地在为我们服务，越来越好的信号质量就是明证。
这不，5G要来了，为了迎接这新的时代，基站君正在默默地酝酿着一场大变身。而要说5G基站君在架构方面的演进，就不得不提CU和DU分离的事情。
![](https://img-blog.csdnimg.cn/20190319213851474.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
CU的全称是Centralized Unit，顾名思义就是集中单元；DU的全称是Distributed Unit，含义自然就是分布单元了。
坦白说，CU和DU这两个名称是云里雾里，除了集中和分布这两个含义之外，没有提供更多的信息。
- 
为什么CU和DU要分离？
- 
CU和CU分离了到底有什么好处？
- 
现在开始部署的5G基站都是基于CU和DU分离架构的吗？
# **为什么CU和DU要分离？**
我们先来看看4G和5G无线接入网部分的架构有什么不同：
![](https://img-blog.csdnimg.cn/20190319213943358.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
由上图可以看出，4G基站内部分为BBU，RRU和天线几个模块，每个基站都有一套BBU，并通过BBU直接连到核心网。
而到了5G时代，原先的RRU和天线合并成了AAU，而BBU则拆分成了DU和CU，每个站都有一套DU，然后多个站点共用同一个CU进行集中式管理。
看了5G的架构，尤其是矗立在中间的CU，总是让人感觉莫名熟悉，似曾相识。这不就和2G/3G的架构如出一辙么？
![](https://img-blog.csdnimg.cn/20190319214029418.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
这就奇了怪了，撤销控制器，基站直连核心网，构建扁平化网络这个刚刚在4G时代兴起的架构，到了5G时代，就又要走回2G/3G时代的老路？
这是因为，4G的网络架构跟2G和3G相比可谓剧变，带来了时延的降低和部署的灵活性，但同时也带来了一些问题，尤其是站间信息交互的低效。
![](https://img-blog.csdnimg.cn/20190319214053352.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
从上图可以看出，基站数量多了之后，每个基站都要独立和周围的基站建立连接交换信息，和两个基站相比，情况就变得复杂了起来。这还只是4个基站的情况，如果数量更多的话，连接数将呈指数级增长。这个问题导致了4G基站间干扰难以协同的痼疾。
![](https://img-blog.csdnimg.cn/20190319214115766.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
而2G和3G网络则不同，因为有了控制器这个全知全能的中心节点存在，所有基站的信息一目了然，统筹管理全局资源也就更容易一些。
所谓历史总是在否定之否定中螺旋式上升，5G网络架构的出现正是如此。
在5G时代，对基站，核心网的各项功能进行了重构：首先把原先BBU的一部分物理层处理功能下沉到RRU，RRU和天线结合成为AAU；然后再把BBU拆分为CU和DU，同时CU还融合了一部分从核心网下沉的功能，作为集中管理节点存在。
![](https://img-blog.csdnimg.cn/20190319214141583.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
CU和DU的切分是根据不同协议层实时性的要求来进行的。在这样的原则下，把原先BBU中的物理底层下沉到AAU中处理，对实时性要求高的物理高层，MAC，RLC层放在DU中处理，而把对实时性要求不高的PDCP和RRC层放到CU中处理。
![](https://img-blog.csdnimg.cn/20190319214221876.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# **CU和DU到底有什么好处？**
CU和DU的切分可以带来几大好处。
## **1.实现基带资源的共享**
由于各个基站的忙闲时候不一样，传统的做法是给每个站都配置为最大容量，而这个最大容量在大多数时候是达不到的。比如学校的教学楼在白天话务量很高，而到了晚上就会很空闲，而学生宿舍的情况则正好相反，而这两个地方的基站却要按最大容量设计，造成很大的资源浪费。
如果教学楼的和宿舍的基站能够统一管理，把DU集中部署，并由CU统一调度，就能够节省一半的基带资源。
可以看出，这种方式和之前提出的C-RAN架构非常相似，而C-RAN架构由于对于光纤资源的要求过高而难以普及。在5G，虽然DU可能由于同样的原因难以集中部署，但CU也是基站的一部分，其本身的集中管理也能带来资源的共享，算是5G时代对于C-RAN架构的一种折中的实现方式。
## **2.有利于实现无线接入的切片和云化**
网络切片作为5G的目标，能更好地适配eMBB，mMTC和uRLLC这三大场景对网络能力的不同要求。
切片实现的基础是虚拟化，但是在现阶段，对于5G的实时处理部分，通用服务器的效率还太低，无法满足业务需求，因此还需要采用专用硬件，而专用硬件又难以实现虚拟化。
这样一来，就只好把需要用专用硬件的部分剥离出来成为AAU和CU，剩下非实时部分组成CU，运行在通用服务器上，再经过虚拟化技术，就可以支持网络切片和云化了。
因此，CU加上边缘计算及部分核心网用户面功能的下沉，就被称为“接入云引擎”。
## **3. 满足**5G**复杂组网情况下的站点协同问题**
5G和传统的2G/3G/4G网络不同的是高频毫米波的引入。
由于毫米波的频段高，覆盖范围小，站点数量将会非常多，会和低频站点形成一个高低频交叠的复杂网络。
要在这样的网络中获取更大的性能增益，就必须有一个强大的中心节点来进行话务聚合和干扰管理协同。
毫无悬念，这样的中心节点就是CU。
但是，在DU和CU的拆分在带来诸多的好处的同时，也会带来一些**不利影响**。
首当其冲的就是时延的增加，网元的增加会带来相应的处理时延，再加上增加的的传输接口带来的时延，增加的虽然不算太多，但也足以对超低时延业务带来很大的影响。
其次是网络复杂度的提高。5G不同业务对实时性要求的不同，eMBB对时延不是特别敏感，看高清视频只要流畅不卡顿，延迟多几个毫秒是完全感受不到的；mMTC对时延的要求就更宽松了，智能水表上报读数，有个好几秒的延迟都可以接受；而uRLLC就不同了，对于关键业务，如自动驾驶，可能就是“延迟一毫秒，亲人两行泪”。
因此对于eMBB和mMTC业务可以把CU和DU分开来在不同的地方部署，而要支持uRLLC，就必须要CU和DU合设了。这样一来，不同业务的CU位置不同，大大增加了网络本身的复杂度，管理的复杂度也就蹭蹭上去了。
所以说，CU和DU虽然可以在逻辑上分离，但物理上是不是要分开部署，还要看具体业务的需求才行。对于5G的终极网络，CU和DU必然是合设与分离这两种架构共存的。
# **现在开始部署的5G基站都是基于CU和DU分离架构的吗？**
2019年是5G元年，首先商用的功能是能支持超高下载速率的eMBB业务，具备CU和DU分开部署的条件。那么是否要这么做呢？
首先，最早部署的5G站点都采用低频来覆盖，国际上采用3.5GHz的居多。这个频段的覆盖能力和4G主流频段相当，因此5G大概率是和4G共用机房和铁塔的，这样的成本也最低。
在5G和4G共站址的情况下，只需要对原先机房内部的传输，电源，电池，空调等配套设备升级之后，再把5G基站（CU和DU一体）放进去就可以快速开通5G了，而搞CU和DU分离，还需要专门为CU去建设新的数据中心，成本太大。
因此，5G初期只会进行CU和DU的逻辑划分，实际还都是运行在同一个基站上的，后续随着5G的发展和新业务的拓展，才会逐步进行CU和DU的物理分离。
这一切变化，都正在悄无声息地发生，普通用户只会看到手机上亮起的5G Logo，难以觉察到这背后的汗水。为了5G的梦想，基站君就是这样在默默地酝酿着自身的变革，润物细无声。
> 
原创： 蜉蝣采采  无线深海 **无线深海**
微信号 wuxian_shenhai
功能介绍 移动通信交流，无线通信发展趋势，最新动态，原创科普文章发表。
