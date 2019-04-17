# ESB-企业服务总线 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月18日 13:53:08[boonya](https://me.csdn.net/boonya)阅读数：7184








ESB全称为Enterprise Service Bus，即[企业服务总线](http://baike.baidu.com/item/%E4%BC%81%E4%B8%9A%E6%9C%8D%E5%8A%A1%E6%80%BB%E7%BA%BF)。它是传统[中间件](http://baike.baidu.com/item/%E4%B8%AD%E9%97%B4%E4%BB%B6)技术与XML、Web服务等技术结合的产物。ESB提供了网络中最基本的连接中枢，是构筑企业神经系统的必要元素。ESB的出现改变了传统的[软件架构](http://baike.baidu.com/item/%E8%BD%AF%E4%BB%B6%E6%9E%B6%E6%9E%84)，可以提供比传统[中间件产品](http://baike.baidu.com/item/%E4%B8%AD%E9%97%B4%E4%BB%B6%E4%BA%A7%E5%93%81)更为廉价的解决方案，同时它还可以消除不同应用之间的技术差异，让不同的应用服务器协调运作，实现了不同服务之间的通信与整合。从功能上看，ESB提供了[事件驱动](http://baike.baidu.com/item/%E4%BA%8B%E4%BB%B6%E9%A9%B1%E5%8A%A8)和文档导向的处理模式，以及分布式的运行管理机制，它支持基于内容的路由和过滤，具备了复杂数据的传输能力，并可以提供一系列的标准接口。


## ESB的基本概念

企业服务总线（EnterpriseServiceBus，ESB）从面向服务体系架构（Service-OrientedArchitecture，SOA）发展而来，是传统中间件技术与XML、Web服务等技术结合的产物。

ESB提供了网络中最基本的连接中枢，是构筑企业神经系统的必要元素。ESB采用了“总线”这样一种模式来管理和简化应用之间的集成拓扑结构，以广为接受的开放标准为基础来支持应用之间在消息、事件和服务级别上动态的互连互通，是一种在松散耦合的服务和应用之间标准的集成方式。它可以作用于：

①面向服务的架构—分布式的应用由可重用的服务组成；

②面向消息的架构—应用之间通过ESB发送和接受消息；

③事件驱动的架构—应用之间异步地产生和接收消息。

ESB的出现改变了传统的软件架构，可以提供比传统中间件产品更为低廉的解决方案，同时它还可以消除不同应用之间的技术差异，让不同的应用服务器协调运作，实现了不同服务之间的通信与整合。从功能上看，ESB提供了事件驱动和文档导向的处理模式，以及[分布式](http://baike.baidu.com/item/%E5%88%86%E5%B8%83%E5%BC%8F)的运行管理机制，它支持基于内容的路由和过滤，具备了复杂数据的传输能力，并可以提供一系列的标准接口。




## 基本功能


1)服务的MetaData管理：在总线范畴内对服务的注册命名及[寻址](http://baike.baidu.com/item/%E5%AF%BB%E5%9D%80)进行管理。

2)传输服务：确保通过企业总线互连的业务流程间的消息的正确交付，还包括基于内容的路由功能。

3)中介：提供位置透明的路由和定位服务；提供多种消息传递形式；支持广泛使用的[传输协议](http://baike.baidu.com/item/%E4%BC%A0%E8%BE%93%E5%8D%8F%E8%AE%AE)。

4)多服务集成方式： 如JCA，Web服务，Messaging ，Adapter等。

5)服务和事件管理支持： 调用服务的记录、测量和监控数据；提供事件检测、触发和分布功能；




## 扩展功能


1) 面向服务的元数据管理： 他必须了解被他中介的两端,即服务的请求以及请求者对服务的要求，以及服务的提供者和他所提供的服务的描述；

2) Mediation ：它必须具有某种机制能够完成中介的作用，如协议转换；

3) 通信：服务的发布/订阅、响应/请求、同步/异步消息、路由和[寻址](http://baike.baidu.com/item/%E5%AF%BB%E5%9D%80)等；

4) 集成： 遗留系统适配器，服务编排和映射，协议转换，数据变换，[企业应用集成](http://baike.baidu.com/item/%E4%BC%81%E4%B8%9A%E5%BA%94%E7%94%A8%E9%9B%86%E6%88%90)[中间件](http://baike.baidu.com/item/%E4%B8%AD%E9%97%B4%E4%BB%B6)的连续等。

5) 服务交互： 服务接口定义，服务实现的置换，服务消息模型，服务目录和发现等。

6) 服务安全： 认证和授权、不可否认和机密性、安全标准的支持等；

7) 服务质量： 事务，服务的可交付性等；

8) 服务等级： 性能、可用性等。

ESB 中最常提到的两个功能是消息转换和消息路由。




## ESB架构


ESB 是传统[中间件技术](http://baike.baidu.com/item/%E4%B8%AD%E9%97%B4%E4%BB%B6%E6%8A%80%E6%9C%AF)与XML、Web服务等技术相互结合的产物，ESB的出现改变了传统的[软件架构](http://baike.baidu.com/item/%E8%BD%AF%E4%BB%B6%E6%9E%B6%E6%9E%84)，可以提供比传统[中间件产品](http://baike.baidu.com/item/%E4%B8%AD%E9%97%B4%E4%BB%B6%E4%BA%A7%E5%93%81)更为廉价的解决方案，同时它还可以消除不同应用之间的技术差异，让不同的应用服务器协调运作，实现了不同服务之间的通信与整合。从功能上看，ESB提供了事件驱动和文档导向的处理模式，以及分布式的运行管理机制，它支持基于内容的路由和过滤，具备了复杂数据的传输能力，并可以提供一系列的标准接口。




## ESB的发展


在云计算应用时代，ESB将逐步发展到EBB（Enterprise Business Bus, *企业业务总线*），国际范围内EBB尚处于发展初期，当然许多人也将ESB在业务上的应用，未作区分对待而统一作为ESB看待。事实上，ESB更关注IT服务，而EBB更关注业务执行，具有不同的属性。

国内在2008年就有人就EBB的发展做了专题研究，并就潍柴动力与湘火炬汽车集团结成战略联盟后形成的集齿轮箱、变速器、发动机和整车为一体的汽车产业链实例，使用面向服务的架构SOA和企业协同理念，给出了协同服务平台的设计与分析。

目前国内在EBB领域研究较为超前的，是由中国最年青博士后导师之一、[协达软件](http://baike.baidu.com/item/%E5%8D%8F%E8%BE%BE%E8%BD%AF%E4%BB%B6)创先人[付勇](http://baike.baidu.com/item/%E4%BB%98%E5%8B%87/10385053)，所指导的协同软件博士后工作站。该团队所研究的成果，已经在产业应用上有良好表现，并广泛应用在办公自动化OA、企业资源计划ERP、制造执行系统MES、客户关系管理CRM等领域。




## 应用特征


大规模分布式的企业应用需要相对简单而实用的[中间件技术](http://baike.baidu.com/item/%E4%B8%AD%E9%97%B4%E4%BB%B6%E6%8A%80%E6%9C%AF)来简化和统一越来越复杂、繁琐的企业级信息系统平台。面向服务体系架构（SOA）是能够将应用程序的不同功能单元通过服务之间定义良好的接口和契约联系起来。SOA使用户可以不受限制地重复使用[软件](http://baike.baidu.com/item/%E8%BD%AF%E4%BB%B6)、把各种资源互连起来，只要IT人员选用标准接口包装旧的应用程序、把新的应用程序构建成服务，那么其他应用系统就可以很方便的使用这些功能服务。

支撑SOA的关键是其消息传递架构-[企业服务总线](http://baike.baidu.com/item/%E4%BC%81%E4%B8%9A%E6%9C%8D%E5%8A%A1%E6%80%BB%E7%BA%BF)（ESB）。ESB是传统[中间件技术](http://baike.baidu.com/item/%E4%B8%AD%E9%97%B4%E4%BB%B6%E6%8A%80%E6%9C%AF)与XML、Web服务等技术相互结合的产物，用于实现企业应用不同消息和信息的准确、高效和安全传递。让不同的应用服务协调运作，实现不同服务之间的通信与整合。ESB在不同领域具有非常广泛的用途:

电信领域：ESB能够在全方位支持电信行业OSS的应用整合概念。是理想的电信级[应用软件](http://baike.baidu.com/item/%E5%BA%94%E7%94%A8%E8%BD%AF%E4%BB%B6)承载平台。

电力领域：ESB能够在全方位支持电力行业EMS的[数据整合](http://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E6%95%B4%E5%90%88)概念，是理想的[SCADA系统](http://baike.baidu.com/item/SCADA%E7%B3%BB%E7%BB%9F)[数据交换平台](http://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E4%BA%A4%E6%8D%A2%E5%B9%B3%E5%8F%B0)。

金融领域：ESB能够在全方位支持银企间业务处理平台的流程整合概念，是理想的B2B交易支撑平台。

电子政务：ESB能够在全方位支持电子政务[应用软件](http://baike.baidu.com/item/%E5%BA%94%E7%94%A8%E8%BD%AF%E4%BB%B6)[业务基础平台](http://baike.baidu.com/item/%E4%B8%9A%E5%8A%A1%E5%9F%BA%E7%A1%80%E5%B9%B3%E5%8F%B0)、信息共享交换平台、决策分析支撑平台和政务门户的平台化实现。




## 主要结构


ESB提供了一种开放的、基于标准的消息机制，通过简单的标准适配器和接口，来完成粗粒度应用（服务）和其他组件之间的互操作，能够满足大型异构企业环境的集成需求。它可以在不改变现有基础结构的情况下让几代技术实现互操作。

通过使用ESB，可以在几乎不更改代码的情况下，以一种无缝的非侵入方式使企业已有的系统具有全新的服务接口，并能够在部署环境中支持任何标准。更重要的是，充当“缓冲器”的ESB（负责在诸多服务之间转换业务逻辑和数据格式）与服务逻辑相分离，从而使得不同的应用程序可以同时使用同一服务，用不着在应用程序或者数据发生变化时，改动服务代码。




### 7.1 Smart Service Bus


Smart Enterprise Service Bus™是神州数码秉承SOA理念，结合十数年[企业应用集成](http://baike.baidu.com/item/%E4%BC%81%E4%B8%9A%E5%BA%94%E7%94%A8%E9%9B%86%E6%88%90)领域的最佳实践，研发的一套功能完善、高效稳定、灵巧开放的[企业服务总线](http://baike.baidu.com/item/%E4%BC%81%E4%B8%9A%E6%9C%8D%E5%8A%A1%E6%80%BB%E7%BA%BF)中间件。作为核心的交换平台，能保证7*24小时永不间断提供服务。提供最优的扩容方式，保证扩展线性度达到100%，为组织提供高吞吐量的优质基础服务。提供灵活的部署方式，支持集中部署、分布式部署及总分结构部署。最佳的IT架构治理平台，提供基于元数据的服务治理工具和系统监控工具套件。




### 7.2 Oracle Service Bus


Service Bus的前身是BEA的AquaLogic Service Bus，BEA AquaLogic产品家族交付了最全面的服务基础架构产品线，可以帮助企业成功部署SOA。作为首款完全针对SOA而构建的产品套件，它为IT提供了一组统一的产品来处理消息传递、服务管理、数据一致和安全需要。

Oracle Service Bus为IT提供了一个理想的SOA基础，可以实现下列业务目标：

通过动态配置异构共享服务之间的集成来节省时间。

通过简单的集中式服务注册来减少维护工作。

通过经济有效的服务部署和自动配置来降低成本。

通过确保服务交互的正常进行和可用性来增加正常运行时间。

通过使用服务元数据来简化共享服务的配置、集成和管理。

提供支持基于SOA的业务解决方案所需的企业级可靠性和性能。[1]




### 7.3 IBM WebSphere ESB


IBM 提供了三种 ESB 产品：IBM WebSphere ESB、IBM WebSphere Message Broker、IBM WebSphere DataPower Integration Appliance XI50。根据您的需求选择 ESB 来增强您的 SOA。WebSphere ESB 是一种基于平台的 ESB，作为集成的 SOA 平台，针对 WebSphere[应用服务器](http://baike.baidu.com/item/%E5%BA%94%E7%94%A8%E6%9C%8D%E5%8A%A1%E5%99%A8)进行了优化。WebSphere Message Broker 是跨平台的 ESB，是为异构 IT 环境中的统一连接和转换而构建的。WebSphere DataPower Integration Appliance XI50 是一种基于设备的 ESB，是为简化的部署和更强的安全性而构建的。客户面临着从简单到复杂的各式各样的 ESB 需求。




### 7.4 Microsoft ESB


[微软](http://baike.baidu.com/item/%E5%BE%AE%E8%BD%AF)通过其应用平台提供了全面的ESB服务，包括：Windows Server®2003,.NET Framework, BizTalk®Server 2008 R2. 应用平台提供了一个基础架构，基于此可以灵活和安全地重复使用架构和商业服务，并具有协调原有的服务整合到新的端到端的业务流程中的能力。

[微软](http://baike.baidu.com/item/%E5%BE%AE%E8%BD%AF)通过一些列的产品Windows Server 2003, the .NET Framework 3.0, and BizTalk Server 2006作为对企业实现ESB的支撑,Microsoft ESB Guidance是基于BizTalk Server 2006一组应用，它提供以下公用的ESB组件：l Message routing (消息路由) l Message validation (消息验证) l Message transformation (消息转换) l Centralized exception management(集中的异常管理) l Extensible adapter framework(可扩展的适配器框架) l Service orchestration(服务的编制支持) l Business rules engine(业务[规则引擎](http://baike.baidu.com/item/%E8%A7%84%E5%88%99%E5%BC%95%E6%93%8E)) l Business activity monitoring([业务活动监视](http://baike.baidu.com/item/%E4%B8%9A%E5%8A%A1%E6%B4%BB%E5%8A%A8%E7%9B%91%E8%A7%86))微软 ESB 指南提供了架构指导，模式和实践，以及一套BizTalk Server 和 .NET Framework 组件来简化基于微软平台的大型或小规模的ESB解决方案的开发。它还可以帮助开发人员扩展现有的信息和集成解决方案，包括的一些服务和组件。




### 7.5 JBOSS SOA Platform


JBoss Enterprise SOA Platform提供了一个基于标准的平台，用以集成应用、SOA服务、业务事件和自动化业务流程。这一SOA平台集成了特定版本的JBoss ESB、jBPM、Drools、和已得到验证的JBoss企业应用平台，把它们组织在一起形成一个单一的企业级发布。JBoss Enterprise SOA Platform打包了不少流行组件如：

l JBoss ESB l JBoss jBPM jPDL l JBoss Rules (Drools) l JBoss Application Server l Hibernate l Hibernate Entity Manager l Hibernate Annotations l JBoss Seam l JBoss Web (嵌入式Tomcat 6.0) l JBoss Cache l JGroups l JBoss Messaging l JBoss Transactions l JBoss Web Services (JBossWS) l JBossXB l JBoss AOP l JBoss Remoting l JBoss Serialization l JacORB




### 7.6 ServiceMix对ESB的实现


ServiceMix是一个建立在JBI (JSR 208)语法规则和APIs上的开源ESB(Enterprise Service Bus:企业服务总线)项目。ServiceMix是基于JBI的ESB。它是开源的基于JBI语义和API的ESB和SOA工具包，以Apache许可证方式发布。 它是轻量的ESB实现，易于作为[嵌入式](http://baike.baidu.com/item/%E5%B5%8C%E5%85%A5%E5%BC%8F)ESB使用;集成了对Spring技术的支持;可以在[客户端](http://baike.baidu.com/item/%E5%AE%A2%E6%88%B7%E7%AB%AF)或服务器端运行;可以作为独立的ESB提供者，也可以作为另外ESB的服务组件; 可以在JavaSE或JavaEE服务器中使用；ServiceMix同Apache Geronimo以及JBoss服务器完全集成，并且在Apache Geronimo服务器中可以直接部署JBI组件和服务。Java Business Integration (JBI,Java业务集成)技术规范定义了SOA的[服务导向](http://baike.baidu.com/item/%E6%9C%8D%E5%8A%A1%E5%AF%BC%E5%90%91)集成的内核和组成架构。它对公共讯息路径架构、服务引擎与捆绑的[插件程序](http://baike.baidu.com/item/%E6%8F%92%E4%BB%B6%E7%A8%8B%E5%BA%8F)接口，以及复合型服务描述机制等都进行了标准化，这样就将多种服务结合成为一个单一的可执行的和可审核的工作单元。JBI和ServiceMix关系图JBI并不是一个为开发者设计的一个接口，更准确的说它是在JBI容器里为[集成商](http://baike.baidu.com/item/%E9%9B%86%E6%88%90%E5%95%86)提供相互集成的一个体系和一系列的接口。所以人们能集合他们所需要的所有部分，做出一个总体解决。例如在理论你能从BPEL引擎上，EJB容器上或者是数据传输产品上集合一个基础设施，并且能够集成的很合适。 ServiceMix 中包含完整的JBI容器，支持JBI规范的所有功能要求:l 规范化消息服务和路由 l JBI管理Beans (MBeans)l 组件管理和安装的Ant任务l 对JBI部署单元的完全支持，支持JBI组件的热部署




### 7.7 NEC WebOTX ESB


WebOTX Enterprise Service Bus（以下简称WebOTX ESB）是灵活地结合基于SOA 的系统上的业务应用的，具有消息交换功能的服务运行平台的[中间件](http://baike.baidu.com/item/%E4%B8%AD%E9%97%B4%E4%BB%B6)，是在WebOTX Application Server 的Java EE 环境上动作的ESB 运行环境。WebOTX ESB 处于[处理层](http://baike.baidu.com/item/%E5%A4%84%E7%90%86%E5%B1%82)和服务层中间的Hub产品的位置，使业务变更时系统能灵活对应。

WebOTX ESB 遵循JBI1.0（服务总线的Java 标准定义），提供标准的对应了各种协议的组件，能实现与业务应用的无缝连接。此外，提供了丰富的适配器群以致能与[大型计算机](http://baike.baidu.com/item/%E5%A4%A7%E5%9E%8B%E8%AE%A1%E7%AE%97%E6%9C%BA)上的业务应用、EAI 工具等连接。而且，提供了能吸收服务间消息差异的高速XML 变换引擎，使得不进行任何变更就能灵活地实现系统的构筑。




### 7.8 RES Infomatic Service Bus


RES Infomatic Service Bus是[锐易特软件](http://baike.baidu.com/item/%E9%94%90%E6%98%93%E7%89%B9%E8%BD%AF%E4%BB%B6)信息整合解决方案中最为核心的企业级信息服务总线产品。该产品理念与核心技术跟IBM、Oracle等国际主流厂商的ESB产品同步，自2004年至今，经过了为期两年的国外产品[原型设计](http://baike.baidu.com/item/%E5%8E%9F%E5%9E%8B%E8%AE%BE%E8%AE%A1)和四年的国内本土研发与多行业重量级客户实践检验。广泛应用于金融、电信、政府、公共卫生等行业。它是由七款子产品构成的产品家族，包含了Universal Adapters 通用适配器、Message Broker消息代理、Service Monitor服务监控中心、Service Proxy 服务代理、Registry and Repository 服务资源注册中心、Configuration Manager 配置管理中心、Integration Tools 整合开发工具集，这些子产品相互支撑、[协同工作](http://baike.baidu.com/item/%E5%8D%8F%E5%90%8C%E5%B7%A5%E4%BD%9C)，共同构成分布式信息服务[总线](http://baike.baidu.com/item/%E6%80%BB%E7%BA%BF)的开发、部署、运行、管理的SOA全生命周期支持。




### 7.9 Mule ESB


Mule ESB是一种基于java的、轻量级的企业服务总线和集成平台，它允许开发者快速的、简单的连接应用，并能够实现数据的转换。

Mule ESB的主要功能如下：

服务的创建与管理（Service creation and hosting）：用Mule ESB作为一个轻量级的服务容器来暴露和管理可重用的服务。

 服务调解（Service mediation）：隐藏服务消息的格式和协议，将业务逻辑从消息中独立出来，并可以实现本地独立的服务调用。

 消息路由（Message routing）：基于内容和规则的消息路由、消息过滤、消息合并和消息的重新排序。

 数据转换（Data transformation）：在不同的格式和传输协议中进行转换数据。




## ESB以太网插板：


ESB26 与ESB24 板的不同主要是在插板上提供的以太网接口的数量不一样ESB26 提供26 个接口，ESB24 上有24 个；另外ESB26 板前面板上有六个以太网接口一个com 口，ESB24 板前面板上有四个以太网接口一个com 口；通常构成EMB 的ESB 板也属于SWU 单元，还有LANU 上的ESB 板也是SWU单元。


# Apache synapse ESB

[http://synapse.apache.org/](http://synapse.apache.org/)




## Apache Synapse企业服务总线（ESB）

Apache Synapse是一种轻量级的高性能企业服务总线（ESB）。Apache Synapse由快速和异步的中介引擎提供支持，为XML，Web服务和REST提供了卓越的支持。除了XML和SOAP之外，Apache Synapse还支持多种其他内容交换格式，例如纯文本，二进制，Hessian和JSON。适用于Synapse的广泛的传输适配器使其能够通过许多应用和传输层协议进行通信。到目前为止，Apache Synapse支持HTTP / S，邮件（POP3，IMAP，SMTP），JMS，TCP，UDP，VFS，SMS，XMPP和FIX。

Apache Synapse是[Apache Software License 2.0](http://www.apache.org/licenses/LICENSE-2.0)下分发的免费开源软件。最新版本的Synaspe是[v3.0.0](http://synapse.apache.org/download/3.0.0/download.cgi)。此版本带来了大量新功能，错误修复，性能和稳定性改进。

Apache Synapse，Synapse，Apache，Apache羽毛徽标和Apache Synapse项目徽标是The[Apache Software Foundation的商标](http://www.apache.org/)



## 版本3.0.0中的新功能
- 高性能PassThrough HTTP传输支持所有调解方案- 超快速，低延迟的HTTP请求中介
- 同时支持大量入站（客户端 - > ESB）和出站（ESB - >服务器）连接
- 使用共享缓冲区来智能地处理内置在引擎中的消息内容和内容感知，以处理数据
- 在存在慢或故障的客户端和服务器的情况下，自动节流和性能下降

- HTTP传输的OCSP / CRL证书验证支持
- 响应中介者 - 调解人从中介流程中的任何地方回复客户端
- Loopback Mediator - 从IN序列跳转到OUT序列的中介者
- 标题中介者改进- 支持添加/删除传输头

- 新的xpath函数- url-encoded xpath函数
- 从get-property函数访问系统属性
- base64解码功能

- 消息处理器改进- 重新排序消息处理器
- 新的阻止客户端实现

- 消息注入任务改进- 支持向代理服务注入消息
- 支持向命名序列注入消息

- 标注调解员改进- 支持WS-Security
- 内联终端支持
- 能够使用'To'头动态设置EPR
- NTLM支持

- 脚本中介者改进- 支持删除属性

- REST API改进- 运输级别访问限制




## 主要特征
- 代理服务 - 促进传输，接口（WSDL / Schema / Policy），消息格式（SOAP 1.1 / 1.2，POX / REST，文本，二进制），QoS（WS-Addressing / WS-Security / WS-RM） MTOM / SWA）
- 用于快速HTTP交互的非阻塞HTTP / S传输，并支持数千个并发连接
- 用于文件操作和与FTP，SFTP，CIFS和WEBDAV交互的VFS传输
- JMS支持二进制，纯文本，XML和SOAP有效载荷
- 邮件传输，广泛支持POP3，IMAP和SMTP
- 支持业界推动的[财务信息交换（FIX）](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://www.fixprotocol.org/&usg=ALkJrhg99_zT20nDrwhSA-vsu0jt3cSDoQ)协议
- 内置注册表/存储库，便于动态重新配置和关联资源（如XSLT，XSD，JS等）
- 内置支持使用[Quartz](http://www.opensymphony.com/quartz)调度程序调度任务
- 负载均衡（带或不带粘性会话）和故障切换路由
- 支持许多Web服务标准，包括WS-Addressing，WS-Security和WS-Reliable Messaging
- 基于策略的消息限制和缓存（特别支持集群环境）
- 消息分割和聚合
- 使用数据库连接池进行数据库查找和更新支持
- 针对序列，端点和代理服务的细粒度统计信息收集
- JMX监控管理
- 可以使用Java，Spring或BSF脚本语言（Javascript，Ruby，Groovy等）轻松扩展



## 高级架构

Apache Synapse的设计是轻量级和快速的。非阻塞HTTP传输，多线程中介引擎和流式XML信息集合结合起来，以确保Synapse可以以最小的延迟和资源使用通过服务总线调解非常大量的消息。Synapse还具有全面的日志记录功能，统计信息收集和JMX监视支持，这在生产部署中至关重要。
![](http://synapse.apache.org/images/synapse-arch.png)
Synapse使用Apache Axis2作为底层Web服务引擎。因此，它对Web服务和相关标准（如SOAP和WSDL）有突出的支持。经过测试的Axis2模块，如Apache Rampart和Apache Sandesha2，可与Synapse一起使用，无需配置开销。使用这样的外部模块，Apache Synapse支持一系列Web服务标准，包括WS-Security和WS-Reliable Messaging。Synapse还利用Axis2集群框架提供企业级集群支持。

Synapse使用简单的基于XML的配置语言进行配置。配置语言和相关功能组件的设计考虑了SOA最佳实践。将配置片段存储在外部SOA注册表中并将其按需导入到中介引擎是微不足道的。Synapse提供了大量的调解器，可用于实现即使是最复杂的企业集成方案。如果需要，可以通过使用Java或您最喜欢的脚本语言开发定制调解器来扩展中介引擎。





开源ESB方案：[http://www.oschina.net/project/tag/333/esb](http://www.oschina.net/project/tag/333/esb)





