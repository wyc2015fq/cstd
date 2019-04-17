# 什么是SaaS、PaaS和IaaS？ - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月08日 17:07:22[boonya](https://me.csdn.net/boonya)阅读数：1660









**我们生活中随时都被计算机术语所覆盖，有必要了解更多，如：SaaS、PaaS和IaaS。**
![](https://img-blog.csdn.net/20160808164735348)

根据NIST（NationalInstituteofStandardsandTechnology，美国国家标准与技术研究院）的权威定义，云计算的服务模式有SPI（即SaaS、PaaS和IaaS）这三个大类或层次。这是目前被业界最广 泛认同的划分。PaaS和IaaS源于SaaS理念。PaaS和IaaS可以直接通过SOA/Web Services向平台用户提供服务， 也可以作为SaaS模式的支撑平台间接向最终用户服务[3]。
**1. SaaS：**提供给客户的服务是运营商运行在[云计算](http://baike.baidu.com/view/1316082.htm)基础设施上的应用程序，用户可以在各种设备上通过[客户端](http://baike.baidu.com/view/930.htm)界面访问，如[浏览器](http://baike.baidu.com/view/7718.htm)。消费者不需要管理或控制任何[云计算](http://baike.baidu.com/view/1316082.htm)基础设施，包括网络、服务器、[操作系统](http://baike.baidu.com/view/880.htm)、存储等等；

**2. PaaS：**提供给消费者的服务是把客户采用提供的开发语言和工具（例如Java，python, .Net等）开发的
![云计算SPI关系图](http://d.hiphotos.baidu.com/baike/s%3D220/sign=21333d50738da9774a2f81298050f872/58ee3d6d55fbb2fb28b5a6714f4a20a44623dc33.jpg)云计算SPI关系图



或收购的应用程序部署到供应商的云计算基础设施上去。客户不需要管理或控制底层的云基础设施，包括网络、服务器、操作系统、存储等，但客户能控制部署的应用程序，也可能控制运行应用程序的托管环境配置；

**3. IaaS：**提供给消费者的服务是对所有计算基础设施的利用，包括处理CPU、内存、[存储](http://baike.baidu.com/view/87682.htm)、网络和其它基本的计算资源，用户能够部署和运行任意软件，包括[操作系统](http://baike.baidu.com/view/880.htm)和应用程序。消费者不管理或控制任何云计算基础设施，但能控制[操作系统](http://baike.baidu.com/view/880.htm)的选择、存储空间、部署的应用，也有可能获得有限制的网络组件（例如路由器、，[防火墙](http://baike.baidu.com/view/3067.htm)，、[负载均衡器](http://baike.baidu.com/view/486970.htm)等）的控制。


### SAAS



        SaaS是Software-as-a-Service（[软件即服务](http://baike.baidu.com/view/1559138.htm)）的简称，随着互联网技术的发展和应用软件的成熟， 在21世纪开始兴起的一种完全创新的软件应用模式。它与“on-demand software”（按需软件)，the application
 service provider(ASP，应用服务提供商)，hosted software(托管软件)所具有相似的含义。它是一种通过Internet提供软件的模式，厂商将应用软件统一部署在自己的服务器上，客户可以根据自己实际需求，通过互联网向厂商定购所需的应用软件服务，按定购的服务多少和时间长短向厂商支付费用，并通过互联网获得厂商提供的服务。用户不用再购买软件，而改用向提供商租用基于Web的软件，来管理企业经营活动，且无需对软件进行维护，服务提供商会全权管理和维护软件，软件厂商在向客户提供互联网应用的同时，也提供软件的离线操作和本地数据存储，让用户随时随地都可以使用其定购的软件和服务。对于许多小型企业来说，SaaS是采用先进技术的最好途径，它消除了企业购买、构建和维护基础设施和应用程序的需要。

SaaS 应用软件的价格通常为“全包”费用，囊括了通常的应用软件许可证费、软件维护费以及技术支持费，将其统一为每个用户的月度租用费。 对于广大中小型企业来说，SaaS是采用先进技术实施信息化的最好途径。但SaaS绝不仅仅适用于[中小型企业](http://baike.baidu.com/view/1404619.htm)，所有规模的企业都可以从SaaS中获利。


      SaaS提供商为企业搭建信息化所需要的所有网络基础设施及软件、硬件运作平台，并负责所有前期的实施、后期的维护等一系列服务，企业无需购买软硬件、建设机房、招聘IT人员，即可通过互联网使用信息系统。就像打开自来水龙头就能用水一样，企业根据实际需要，从SaaS提供商租赁软件服务。

SaaS 是一种软件布局模型，其应用专为网络交付而设计，便于用户通过互联网托管、部署及接入。 SaaS 应用软件的价格通常为“全包”费用，囊括了通常的应用软件许可证费、软件维护费以及技术支持费，将其统一为每个用户的月度租用费。 对于广大中小型企业来说，SaaS是采用先进技术实施信息化的最好途径。但SaaS绝不仅仅适用于[中小型企业](http://baike.baidu.com/view/1404619.htm)，所有规模的企业都可以从SaaS中获利。

      2008 年前，IDC 将SaaS 分为两大组成类别：托管应用管理 (hosted AM) － 以前称作应用服务提供 ([ASP](http://baike.baidu.com/view/2616.htm))，以及“按需定制软件”，即 SaaS 的同义词。从 2009 年起，托管应用管理已作为IDC应用外包计划的一部分，而按需定制软件以及
 SaaS 被视为相同的交付模式对待。

SaaS已成为软件产业的一个重要力量。只要SaaS的品质和可信度能继续得到证实，它的魅力就不会消退。
![](https://img-blog.csdn.net/20160808165945996)





![](https://img-blog.csdn.net/20160808165351743)


### PAAS


           定义：PaaS（Platform-as-a-Service：平台即服务）全称:(Platform as a service)中文: 平台即服务所谓PaaS实际上是指将软件研发的平台（计世资讯定义为业务基础平台）作为一种服务，以SaaS的模式提交给用户。因此，PaaS也是SaaS模式的一种应用。但是，PaaS的出现可以加快SaaS的发展，尤其是加快SaaS应用的开发速度。在2007年国内外SaaS厂商先后推出自己的PAAS平台。PAAS之所以能够推进SaaS的发展，主要在于它能够提供企业进行定制化研发的中间件平台，同时涵盖数据库和应用服务器等。PAAS可以提高在Web平台上利用的资源数量。例如，可通过远程Web服务使用数据即服务（Data-as-a-Service：数据即服务），还可以使用可视化的API。用户或者厂商基于PaaS平台可以快速开发自己所需要的应用和产品。同时，PaaS平台开发的应用能更好地搭建基于SOA架构的企业应用。

       PaaS是Platform-as-a-Service的缩写，意思是[平台](http://baike.baidu.com/view/58664.htm)即[服务](http://baike.baidu.com/subview/133203/11217422.htm)。 把[服务器](http://baike.baidu.com/view/899.htm)平台作为一种服务提供的商业模式。通过网络进行程序提供的服务称之为SaaS(Software
 as a Service)，而[云计算](http://baike.baidu.com/view/1316082.htm)时代相应的服务器平台或者开发环境作为服务进行提供就成为了PaaS(Platform as a Service)。

       所谓PaaS实际上是指将[软件](http://baike.baidu.com/view/37.htm)研发的平台（计世资讯定义为[业务基础平台](http://baike.baidu.com/view/6862499.htm)）作为一种服务，以[SaaS](http://baike.baidu.com/view/369107.htm)的模式提交给用户。因此，PaaS也是[SaaS](http://baike.baidu.com/view/369107.htm)模式的一种应用。但是，[PaaS](http://baike.baidu.com/view/1413359.htm)的出现可以加快SaaS的发展，尤其是加快SaaS应用的开发速度。在2007年国内外SaaS厂商先后推出自己的PAAS平台。




![](https://img-blog.csdn.net/20160808170147653)

![](https://img-blog.csdn.net/20160808170228050)

### IAAS
         IaaS（Infrastructure as a Service），即基础设施即服务。消费者通过Internet 可以从完善的计算机基础设施获得服务。这类服务称为基础设施即服务。基于 Internet 的服务（如存储和数据库）是 IaaS的一部分。Internet上其他类型的服务包括平台即服务（Platform as a Service，PaaS）和软件即服务（Software as a Service，SaaS）。PaaS提供了用户可以访问的完整或部分的应用程序开发，SaaS则提供了完整的可直接使用的应用程序，比如通过
 Internet管理企业资源。

        作为Infrastructure as a Service (IaaS) 在实际应用中的一个例子，The New York Times 使用成百上千台 Amazon EC2虚拟机实例在 36 小时内处理 TB 级的文档数据。如果没有 EC2，The New York Times 处理这些数据将要花费数天或者数月的时间。

IaaS通常分为三两种用法：公有云共的和、私有云的和混合云。AmazonEC2在基础设施云中使用公共服务器池（公有云）。更加私有化的服务会使用企业内部[数据中心](http://baike.baidu.com/view/754520.htm)的一组公用或私有服务器池（私有云）。如果在企业[数据中心](http://baike.baidu.com/view/754520.htm)环境中开发软件，那么这两种类型公有云、私有云、混合云都能使用，而且使用EC2临时扩展资源的成本也很低，如—比方说开发和测试，混合云。结合使用两者可以更快地开发应用程序和服务，缩短开发和测试周期。
同时，IaaS也存在[安全漏洞](http://baike.baidu.com/view/160026.htm)，例如服务商提供的是一个共享的基础设施，也就是说一些组件或功能，例如CPU[缓存](http://baike.baidu.com/view/907.htm)，、GPU等对于该系统的使用者而言并不是完全隔离的，这样就会产生一个后果，即当一个攻击者得逞时，全部服务器都向攻击者敞开了大门，即使使用了hypervisor，有些客户机[操作系统](http://baike.baidu.com/view/880.htm)也能够获得基础平台不受控制的访问权。解决办法：开发一个强大的分区和防御策略，IaaS供应商必须监控环境是否有未经授权的修改和活动。






