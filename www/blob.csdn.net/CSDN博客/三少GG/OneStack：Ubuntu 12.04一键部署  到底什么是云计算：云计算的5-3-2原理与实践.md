# OneStack：Ubuntu 12.04一键部署 || 到底什么是云计算：云计算的5-3-2原理与实践 - 三少GG - CSDN博客
2013年03月29日 21:54:02[三少GG](https://me.csdn.net/scut1135)阅读数：1771

### [OneStack：Ubuntu 12.04 (或11.10) 一键部署安装OpenStack云计算平台](http://blog.csdn.net/hilyoo/article/details/7696169)
分类： [OpenStack&云计算](http://blog.csdn.net/hilyoo/article/category/1174832)2012-06-27
 14:4715577人阅读[评论](http://blog.csdn.net/hilyoo/article/details/7696169#comments)(44)[收藏]()[举报](http://blog.csdn.net/hilyoo/article/details/7696169#report)
[ubuntu](http://blog.csdn.net/tag/details.html?tag=ubuntu)[云计算](http://blog.csdn.net/tag/details.html?tag=%E4%BA%91%E8%AE%A1%E7%AE%97)[平台](http://blog.csdn.net/tag/details.html?tag=%E5%B9%B3%E5%8F%B0)[service](http://blog.csdn.net/tag/details.html?tag=service)[mysql](http://blog.csdn.net/tag/details.html?tag=mysql)[network](http://blog.csdn.net/tag/details.html?tag=network)
OneStack：在Ubuntu12.04（precise）上一键安装部署Opentack Essex
提醒：如果你喜欢折腾，喜欢自己一步一步安装各个功能组件和配置conf文件，你可以略过此文。本文工具可以在裸机和虚拟机一键部署真实的OpenStack云计算平台，主要步骤是配置网络、数据库、keystone、glance、nova、dashboard、创建镜像和启动实例等，代码内有详细注释。这只是学习Opentack的入门的一步，深入研究可以参考附录3给出的部分资料和社区。
2012.07.14更新：
收到很多邮件和提问，精力有限没法一一回复，在此致歉。随手把记录的一些问题和解决方法整理了，供大家参考。
详见《[OpenStack安装部署管理中常见问题解决方法（OpenStack-Lite-FAQ）](http://blog.csdn.net/hilyoo/article/details/7746634)》。
2012.07.09更新：
很多人来信问我单网卡怎么设置，以及其它网络问题，我后面写了一篇介绍OpenStack网络的，详见《[OpenStack云平台的网络模式及其工作机制](http://blog.csdn.net/hilyoo/article/details/7721401)》。
*“*
三、详解FlatDHCP模式（Flat模式类似，只是少了dhcp的部分而已，就略过了）
可以有多种部署方式，比如为了实现高可用性，可以使用多网卡、外部网关、multi_host 等方法。这里主要介绍基本的部署方式（一个控制节点，或者说一个网络控制器）。
1、网卡与节点
由于网卡和节点数的不同，可以简单分为：单节点（all-in-one）单网卡、多节点单网卡、多节点多网卡
单节点的情况下，网络控制器（运行nova-network）与计算（运行nova-compute，或者更确切的说，运行虚拟机实例）部署在一个主机。这样就不需要控制节点与计算节点之间的通信，也就少了很多网络概念，这也是入门者常用的方式。
多节点时，网络控制器与计算节点分别在不同主机，普通部署方式下（不是multi_host），只有nova-network控制网络，而它仅仅在控制节点运行。因此，所有计算节点的实例都需要通过控制节点来与外网通信。
单网卡时，**网卡需要作为public网络的接口使用，也需要作为flat网络的接口，因此需要处于混杂模式。不过建立的网络与双网卡类似，都分为flat网络和public网络。**
使用单网卡，需要在nova.conf中使public_interface和flat_interface都为eth0。
”
2012.07.02更新：
1、受邀在vpsee上对这个项目进行了较为详细的介绍（[OneStack：Ubuntu
 12.04 上一键自动部署 OpenStack](http://www.vpsee.com/2012/07/onestack-all-in-one-installation-tool-for-openstack/)），对于项目来源、相关考虑、项目结构、具体安装步骤（参数设置和前期工作）和预期等做了说明，可以参考。
2、希望增加高可用性的云计算部署，[构建OpenStack的高可用性（HA，High
 Availability）](http://blog.csdn.net/hilyoo/article/details/7704280)对高可用性进行了初步介绍，欢迎提出意见和建议。
一、项目(OneStack)地址：
[http://onestack.googlecode.com](http://code.google.com/p/onestack/)
喜欢git或者github的请去 [https://github.com/Kayven/OneStack](https://github.com/Kayven/OneStack)
不希望同行们把过多精力花在OpenStack的安装部署上（以后应该会改进），而是对其机制原理、工程实践、性能调优、应用服务等深入研究探讨。 所以提供这个一键部署的工具，帮助大家快速建立环境实验生产实践。项目结构如下：
- oneStack.sh（一键部署 all-in-one 的 OneStack，简单安装的主要文件）；
- setup_base.sh（安装基本系统）；
- setup_test.sh（添加镜像和实例）；
- addComputeNode.sh（增加计算节点）；
- delStack.sh（只卸载nova、glance、keystone等）；
- delAll.sh（卸载所有安装的组件和工具）；
- resetStack.sh（清空数据库，镜像、网络和实例等，重新上传镜像、创建实例）；
- addClient.sh（添加客户端，nova管理等）；
- HAStack 目录（OneStack 的高可用性，希望更多人可以提出自己的解决方案）。
二、部署(OneStack)步骤
简单部署OneStack：
wget http://onestack.googlecode.com/files/oneStack.sh && chmod +x oneStack.sh && ./oneStack.sh
完整部署OneStack：
Just checkout and run it!
1. Setup a fresh Ubuntu Precise(12.04) Sever OS.
2. checkout
svn checkout http://onestack.googlecode.com/svn/trunk/ onestack-read-only
3. run it!
cd onestack-read-only/&&./oneStack.sh
（如何简单配置参数见留言说明，或者见[http://onestack.googlecode.com](http://code.google.com/p/onestack/)，或者见脚本注释）
（多节点的部署类似，只需要在计算节点设置并执行addComputeNode.sh）
三、项目(OneStack)说明：
- 在Ubuntu12.04（precise）上安装部署Opentack Essex
- 
只需要一个文件即可完成全部功能组件的部署：[http://onestack.googlecode.com/files/oneStack.sh](http://onestack.googlecode.com/files/oneStack.sh)
- 
里面含有详细的注释说明，看完整个脚本相当于看完了安装文档；
- 
svn整个项目只是多一些辅助工具，包括重置、重新安装、卸载等;
- 
也欢迎同道人补充和完善更多的功能，适用于更多的操作系统和应用场景。
四、项目(OneStack)来源：
- 
类似项目有DevStack，但是DevStack是使用screen安装OpenStack，重启服务器会出现一些问题，没有提供重启、重置等有用功能；
- 
由于组件独立分散，安装过程过于繁琐，可以抽象成通用的项目供大家方便使用；
- 
官方提供了一个比较完善的入门文档，但是，这一步一步下来得做很多无用功，容易漏错而引起很多莫名和头疼的问题；
- 
本项目不像devstack只是提供实验环境，而是可以实际部署使用，可以自己修改配置，按需增加组件和功能，实现一键部署，可扩展使用。
五、项目(OneStack)描述：
- 
一键完整部署OpenStack，可以自定义配置、方便地扩展功能组件，无交互地自动部署；
- 
安装过程不需要等待配置：
- 功能齐全，附带了卸载、重置、添加计算节点等工具
- 可以卸载主要的opentack组件，包括nova、glance、keystone等；
- 可以重置数据库和配置，重新安装openstack组件；
- 可以根据需要自行更改脚本，方便部署自己的云计算平台。
- 经过多次测试，完整在VMware虚拟机上部署OpenStack
      测试环境：VMware ESXi 5.0虚拟Dell R210服务器，其中一个虚拟机上部署Opentack，启动多个实例，正常运行；远程访问和web管理正常；重启、重置、重新安装都正常。
      系统要求：双网卡，64位机器，支持VT技术
- 脚本运行过程会做如下工作：
- 配置网络相关；
- 安装和配置数据库；
- 安装和部署身份管理组件keystone；
- 安装和部署镜像管理组件glance；
- 安装和部署控制与计算组件nova；
- 安装和部署web管理组件dashboard；
- 上传和添加ubuntu12.04镜像；
- 设置项目安全规则和密钥；
- 启动实例，并正常运行。
- 欢迎反馈，谢谢.
- 本人尽力做到不出差错，但是限于学识和眼界，难免有考虑不周、冗余、没有最优化或者表达不好之处；
- 希望大家有任何意见建议随时联系我。
附录1：
为了方便大家查阅、讨论、交流、review和完善，在此贴出主要代码。
附录2：安装后web登录（dashboard）截图
![](https://img-my.csdn.net/uploads/201206/27/1340788165_8953.jpg)
附录3：云计算平台OpenStack资料
官方OpenStack www.openstack.org
国内OpenStack社区 www.openstack.org.cn
国内技术博客-陈沙克 [http://hi.baidu.com/chenshake](http://hi.baidu.com/chenshake)
OpenStack 架构  [http://blog.csdn.net/anghlq/article/details/6543880](http://blog.csdn.net/anghlq/article/details/6543880)
Ubuntu12.04安装OpenStack文档（入门指南） [http://docs.openstack.org/essex/openstack-compute/starter/content/](http://docs.openstack.org/essex/openstack-compute/starter/content/)
Installing OpenStack Essex (2012.1) on Ubuntu 12.04 ("Precise Pangolin")（原文） [http://www.hastexo.com/resources/docs/installing-openstack-essex-20121-ubuntu-1204-precise-pangolin](http://www.hastexo.com/resources/docs/installing-openstack-essex-20121-ubuntu-1204-precise-pangolin)
“OpenStack Documentation”team（文档、bug报告） [https://launchpad.net/~openstack-doc](https://code.launchpad.net/~openstack-doc)
Quora上关于openstack的话题 [http://www.quora.com/OpenStack](http://www.quora.com/OpenStack)
stackoverflow上关于openstack资源的讨论 [http://stackoverflow.com/questions/5882333/good-tutorials-and-resources-for-openstack](http://stackoverflow.com/questions/5882333/good-tutorials-and-resources-for-openstack)
[Kayven](http://weibo.com/kayven)( Hily.Hoo@gmail.com )
### [到底什么是云计算：云计算的5-3-2原理与实践](http://blog.csdn.net/hilyoo/article/details/7698149)
分类： [OpenStack&云计算](http://blog.csdn.net/hilyoo/article/category/1174832)
[云计算](http://blog.csdn.net/tag/details.html?tag=%E4%BA%91%E8%AE%A1%E7%AE%97)[虚拟化](http://blog.csdn.net/tag/details.html?tag=%E8%99%9A%E6%8B%9F%E5%8C%96)[服务器](http://blog.csdn.net/tag/details.html?tag=%E6%9C%8D%E5%8A%A1%E5%99%A8)[存储](http://blog.csdn.net/tag/details.html?tag=%E5%AD%98%E5%82%A8)[security](http://blog.csdn.net/tag/details.html?tag=security)[网络](http://blog.csdn.net/tag/details.html?tag=%E7%BD%91%E7%BB%9C)
经常有人问我：“到底什么是云计算，它的核心是什么？”
自从某次的思考后，我每次都会回答：云计算的5-3-2原理，也就是具备5大核心特点，3种服务模式和2种部署方式。
一、引子
由于云计算的广泛热炒，很多东西都牵强附会地被”云“了。于是，”人云亦云“、“新瓶换旧酒”、“假大空”等各种帽子被扣在了云计算上。
各种厂商和机构也纷纷给出了自己的云计算的定义，纷纷扰扰，混乱不堪。这种情形，所有的努力都会分散到很多点，形不成中坚力量，对于云计算在产业界的作用和在人们心中的影响都会大打折扣。
在NIST（National Institute of Standards andTechnology，美国国家标准和技术研究院）的定义之前，比较普遍的提法是把云计算划分为一系列的"X
 as aService"业务。
二、云计算的5-3-2原理
NIST的Peter Mell和Tim Grance在2009年4月提出了一个云计算的定义，这可能是目前唯一一个得到广泛认同和支持的定义。
在参考文献1里，NIST对云计算定义如下：
云计算是一种能够通过网络以便利的、按需付费的方式获取计算资源（包括网络、服务器、存储、应用和服务等）并提高其可用性的模式，这些资源来自一个共享的、可配置的资源池，并能够以最省力和无人干预的方式获取和释放。这种模式具有5个关键功能，还包括3种服务模式和4种部署方式。
**5大核心特点**
NIST提出了云计算系统需提供的五大核心特点。
1.       按需自助服务（On Demand Self-Service）：
供应商的资源保持高可用和高就绪的状态，用户可以按需方便地自助地获得资源。
2.       泛在的网络访问（Broad Network Access）：
可以通过各种网络渠道，以统一标准的机制（如浏览器，相同的API等）获取服务,但是客户端可以是多种多样的瘦客户端或富客户端（例如移动电话、笔记本电脑、PDA等）。
3.       动态的资源池（Resource Pooling）：
供应商的计算资源可以被整合为一个动态资源池，以多租户模式服务所有客户，不同的物理和虚拟资源可根据客户需求动态分配。服务商需实现资源的位置无关性，客户一般不需要知道所使用的资源的确切地理位置，但在需要的时候客户可以指定资源位置（如哪个国家，哪个数据中心等）的要求。
4.       快速弹性（Rapid Elasticity）：
可以迅速、弹性地提供服务，能快速扩展，也可以快速释放实现快速缩小。对客户来说，可以租用的资源看起来似乎是无限的，可在任何时间购买任何数量的资源。
5.       可计量的服务（Measured Service）：
服务的收费可以是基于计量的一次一付，或基于广告的收费模式。系统以针对不同服务需求（例如，CPU时间、存储空间、带宽、甚至按用户账号的使用率高低）来计量资源的使用情况和定价，以提高资源的管控能力和促进优化利用。整个系统资源可以通过监控和报表的方式对服务提供者和使用者透明化。
**3种服务模式**
按NIST的定义，云计算的三个服务模式（Delivery Models）是：SaaS、PaaS和IaaS。
SaaS：提供给客户的服务是运营商运行在云计算基础设施上的应用程序，用户可以在各种设备上通过瘦客户端界面访问，如浏览器。消费者不需要管理或控制任何云计算基础设施，包括网络、服务器、操作系统、存储，等等。
PaaS：提供给消费者的服务是把客户采用提供的开发语言和工具（例如Java、python、.Net等）开发的或收购的应用程序部署到供应商的云计算基础设施上。客户不需要管理或控制底层的云基础设施，包括网络、服务器、操作系统、存储等，但客户能控制部署的应用程序，也可能控制运行应用程序的托管环境配置。
IaaS: 提供给消费者的服务是对所有设施的利用，包括处理、存储、网络和其他基本的计算资源，用户能够部署和运行任意软件，包括操作系统和应用程序。消费者不管理或控制任何云计算基础设施，但能控制操作系统的选择、储存空间、部署的应用，也有可能获得有限制的网络组件（例如，防火墙、负载均衡器等）的控制。
**2种部署方式**
NIST把云计算系统的部署方式或存在实体分为2大类：
l  私有云（Private Cloud）：
云基础设施被某单一组织拥有或租用，可以坐落在本地（on Premise）或（防火墙外的）异地，该基础设施只为该组织服务。
l  社区云（Community Cloud）：
或称机构云，云基础设施被一些组织共享，并为一个有共同关注点的社区或大机构服务（例如，任务、安全要求、政策和准则等），社区云可以被该社区拥有和租用，也可以坐落在本地、（防火墙外的）异地或多地。笔者认为，社区云可能是一组私有云通过VPN（Virtual
 Private Network）连接到一起的VPC（Virtual Private Cloud），是混合云的一种。
在更多场合，由于混合云是一种私有云与公有云的混合结构，而社区云是介于它们之间的结构，因此核心的部署模式是2种，即私有云和公共云。也就有了本文题目的5-3-2原理，可以借用以下结构图展示：
![](https://img-my.csdn.net/uploads/201206/28/1340854206_9112.jpg)
三、云计算的引申产物
1、云存储
云存储是在云计算(cloud computing)概念上延伸和发展出来的一个新的概念，是指通过集群应用、网格技术或分布式文件系统等功能，将网络中大量各种不同类型的存储设备通过应用软件集合起来协同工作，共同对外提供数据存储和业务访问功能的一个系统。
当云计算系统运算和处理的核心是大量数据的存储和管理时，云计算系统中就需要配置大量的存储设备，那么云计算系统就转变成为一个云存储系统，所以云存储是一个以数据存储和管理为核心的云计算系统。
2、云安全
2.1 云安全（Cloud Security）是互联网和云计算融合的时代信息安全的最新发展，有以下两方面的含义：
1）、云安全技术（云计算技术在安全领域的应用，安全厂商）
百度百科：“云安全（Cloud Security）”计划是[网络时代](http://baike.baidu.com/view/552605.htm)信息安全的最新体现，它融合了并行处理、网格计算、未知病毒行为判断等新兴技术和概念，通过网状的大量[客户端](http://baike.baidu.com/view/930.htm)对网络中[软件](http://baike.baidu.com/view/37.htm)行为的异常监测，获取互联网中[木马](http://baike.baidu.com/view/931.htm)、恶意程序的最新信息，推送到Server端进行自动分析和处理，再把病毒和木马的解决方案分发到每一个客户端。
病毒厂商的概念：简单理解就是通过互联网达到“反病毒厂商的计算机群”与“用户终端”之间的互动。云安全不是某款产品，也不是解决方案，它是基于云计算技术演变而来的一种互联网安全防御理念。[趋势科技]
主要原理：通过运行在"云"里的大量客户端对网络中软件行为的异常监测，获取互联云中木马、恶意程序的最新信息，传送到服务器端进行自动分析和处理，再把病毒和木马的解决方案分发到每一个客户端。
2）、云计算安全（安全技术在云计算平台的应用，云服务商，运营商）
云计算对信息安全和云服务本身的安全提出的新要求的解决方案和技术。
概念：利用安全技术，解决云计算环境的安全问题，提升云体系自身的安全性，保障云计算服务的可用性、数据机密性、完整性和隐私保护等，保证云计算健康可持续的发展。
主要集中在安全体系结构、虚拟化、隐私、审计、法律等方面，包括数据加密、密钥管理、应用安全、网络安全、管理安全、传输安全、虚拟化安全。
2.2 两者向两个不同的方向发展，有如下区别：
2.2.1后者（云计算安全）
关键技术主要分为数据安全、应用安全、虚拟化安全。数据安全的研究主要有数据传输安全、数据隔离、数据残留等方面，应用安全包括终端用户安全、服务安全、基础设施安全等，虚拟化安全主要来源于虚拟化软件的安全和虚拟化技术的安全。
云计算安全研究目前还在初步阶段，主要有云安全联盟CSA（cloud security alliance，主要推广云安全实践，提供安全指引）、云服务提供商（Amazon、Microsoft、IBM等，主要通过身份认证、安全审查、数据加密、系统冗余等技术和管理手段提高业务平台的健壮性、服务连续性和数据安全性）
2.2.2前者（安全云技术）
指的是信息安全产品和服务提供商利用云计算技术手段提供（Delivery）信息安全服务的模式，属于云计算SaaS模式的一种，可能叫"安全云（Security
 Cloud）"或"安全作为云服务（Security
 as a Cloud Service）"更合适。瑞星、趋势、卡巴斯基、MCAFEE、SYMANTEC、江民科技、PANDA、金山、360安全卫士等都推出了云安全解决方案。中国的奇虎360安全卫士提供的服务在这方面可以说是喊得最响，走在了世界的前面。
云安全核心：对海量未知恶意文件/网页的实时处理能力。
云安全需要的核心技术或者研究的方向有：
大规模分布式并行计算技术、海量数据存储技术、海量数据自动分析和挖掘技术、海量恶意网页自动检测、海量白名单采集及自动更新、高性能并发查询引擎、未知恶意软件的自动分析识别技术、未知恶意软件的行为监控和审计技术
3、其它云
在输入法、游戏、教育、通信和娱乐等领域，云计算的概念也被热炒，其基本思想也是类似。
四、云计算实践
1、虚拟化
NIST定义云计算是补充了3个基本技术。
Key enabling technologiesinclude: (1) fast wide-area networks, (2) powerful, inexpensive servercomputers, and (3) high-performance virtualization for commodity hardware. 
其中虚拟化就是实现云计算的一种基础而核心的技术，也有需要经过深入调研和艰难抉择的过程。
虚拟化，它主要包括：服务器虚拟化、客户端/桌面/应用程序虚拟化、网络虚拟化、存储虚拟化、服务/应用基础结构虚拟化。而今天的桌面虚拟化其实包括了三种虚拟化技术，分别为：服务器虚拟化、桌面虚拟化、应用虚拟化，它们的概念如下：
l  服务器虚拟化：将服务器物理资源抽象成逻辑资源，让一台服务器变成几台甚至上百台相互隔离的虚拟服务器，或者让几台服务器变成一台服务器来用，我们不再受限于物理上的界限，而是让CPU、内存、磁盘、I/O等硬件变成可以动态管理的“资源池”，从而提高资源的利用率，简化系统管理，实现服务器整合，让IT对业务的变化更具适应力。
l  桌面虚拟化：一种基于服务器的计算模型，并且借用了传统的瘦客户端的模型，但是让管理员与用户能够同时获得两种方式的优点：将所有桌面虚拟机在数据中心进行托管并统一管理；同时用户能够获得完整PC的使用体验。
l  应用虚拟化：应用虚拟化将应用程序与操作系统解耦合，为应用程序提供了一个虚拟的运行环境。在这个环境中，不仅包括应用程序的可执行文件，还包括它所需要的运行时环境。从本质上说，应用虚拟化是把应用对低层的系统和硬件的依赖抽象出来，可以解决版本不兼容的问题。
服务器虚拟化技术
在x86服务器上的虚拟化技术上可分为：
l  硬件协助虚拟化
l  操作系统虚拟化
n  全虚拟化
n  半虚拟化。
硬件协助虚拟化（Hardware-Assisted-Virtualization）
两大CPU厂商的Intel VT-x与 AMD-V技术。
应用：在BIOS设置中打开该功能选项，可以使Xen等半虚拟化软件支持全虚拟化。
全虚拟化（Full Virtualization）：中央处理器与其他的硬设备都虚拟化。
优点：操作系统只要有支持虚拟化系统提供的硬设备，就可以不修改操作系统而直接在虚拟机上安装该操作系统。
缺点：因为所有的硬设备都由虚拟产生，所以当虚拟机上的操作系统要读取某一个硬设备时，都需要通过虚拟化系统才能读取真实的硬设备，因而效能往往比较差。
代表：Vmware ESX Server
半虚拟化（Paravirtualization）：虚拟化系统仅提供中央处理器的虚拟化能力。
优点：硬件设备则与真实机器共享同一个硬件设备，因此使用硬件设备的效率较佳。
缺点：操作系统必须修改内核才能安装在半虚拟化系统中的虚拟机器中，这样就无法直接支持像Windows这样的非开放源代码操作系统。
代表：Citrix XenServer
2、三种服务模式的云计算
上面提到，云计算有3种服务模式。那么它们分别怎么体现呢？先来举几个常见的例子。
Hadoop为什么和云计算经常在一起？hadoop不就是可以分布式的计算和存储吗。
Amazon的云计算EC2，OpenStack建立的云计算平台，不就是可以自己按需创建和管理一些实例吗，这也是云计算吗？
Google的GAE、新浪的sae，不就是可以自己在上面创建自己的应用吗，这与上面的例子完全不一样，这也是云计算吗？
很多人容易混淆各种概念和技术，分不清它们之间的关系和区别。其实，只需要一直牢记基本的原理，宏观把握概念所处的层次结构和作用，理清这些花花色色的概念还是不难的。
我们可以看到，云计算的应用领域广泛，服务形态也是多种多样。但是，不管怎样，不外乎5-3-2原理介绍的3种服务模式。
计算机能提供的服务，不外乎存储和计算（当然，这里说的计算包括处理、控制等动态的过程），操作系统、网络通信、虚拟化、数据库、文件系统等等都是来帮助完成这两种服务的。只是可以提供的应用形态、提供的功能、封装的层次和面向的用户不同而已。
VMware Vsphere这样的套件能提供虚拟化的解决方案，能管理大规模的服务器，提供各种迁移、备份、容灾等功能，可以部署一个私有云。OpenStack这种软件项目，同样能建立私有和公共云。但是它们的抽象层次和对用户的服务形态不同。正如汇编语言和C语言都能写出同样功能的程序，但是是不同的实现方式。
在上面的例子里，**Hadoop主要提供分布式存储和计算；OpenStack它们主要提供基础设施，提供类似物理设备的逻辑设备，自己具有很高的控制管理权；GAE提供应用平台，可以自己创建和管理应用；网盘提供存储应用，对应用环境之类的没有控制权。**
**所以，都是提供云计算服务，只是不同的服务模式。**
3、云计算IaaS代表OpenStack
OpenStack作为开源云计算项目中的一种，提供了基础设施，实例和对象存储（还是不外乎计算和存储）。一个实例就是一个跑着操作系统的虚拟机，可以实现迁移、快照、备份、动态分配等等管理功能。它是学习云计算的一个很好的实践。前一篇文章【[OneStack：](http://blog.csdn.net/hilyoo/article/details/7696169)Ubuntu
 12.04("Precise") 一键安装部署云计算平台OpenStack Essex】给出了部署的一种简单的方法，可以参考。
临时记录，待完善。
参考文献：
【1】、NIST Cloud Computing Program [http://www.nist.gov/itl/cloud/](http://www.nist.gov/itl/cloud/)
【2】、Chou’s Theories of Cloud Computing: The5-3-2 Principle [http://blogs.technet.com/b/yungchou/archive/2011/03/03/chou-s-theories-of-cloud-computing-the-5-3-2-principle.aspx](http://blogs.technet.com/b/yungchou/archive/2011/03/03/chou-s-theories-of-cloud-computing-the-5-3-2-principle.aspx)

