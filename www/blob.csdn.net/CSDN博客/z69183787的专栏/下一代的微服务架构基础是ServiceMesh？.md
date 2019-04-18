# 下一代的微服务架构基础是ServiceMesh？ - z69183787的专栏 - CSDN博客
2019年03月06日 18:19:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：88
今年，ServiceMesh(服务网格) 概念在社区里头非常火，有人提出 2018 年是 ServiceMesh 年，还有人提出 ServiceMesh 是下一代的微服务架构基础。作为架构师，如果你现在还不了解 ServiceMesh 的话，是否感觉有点落伍了？
那么到底什么是 ServiceMesh？它的诞生是为了解决什么问题？企业是否适合引入 ServiceMesh？通过这篇文章，将为你一一解答这些问题。
微服务架构的核心技术问题
在业务规模化和研发效能提升等因素的驱动下，从单块应用向微服务架构的转型 (如下图所示)，已经成为很多企业 (尤其是互联网企业) 数字化转型的趋势。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VPJYN51bAIj6cDfSxhUs6eCM9tK84rkJtsjukQjFs7ZibuFMSx9wmiboTpAWIVg80icKZWmrDLgIibzqg/640?wx_fmt=png)
在微服务模式下，企业内部服务少则几个到几十个，多则上百个，每个服务一般都以集群方式部署，这时自然产生两个问题 (如下图所示)：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VPJYN51bAIj6cDfSxhUs6eCyicXHHIEBtoOczwjicLkVQP9UD3DQvVlzR3jeSjb5arOvodUdonB8vkA/640?wx_fmt=png)
**一、服务发现：** 服务的消费方 (Consumer) 如何发现服务的提供方 (Provider)？
**二、负载均衡：** 服务的消费方如何以某种负载均衡策略访问集群中的服务提供方实例？
作为架构师，如果你理解了这两个问题，也就理解了微服务架构在技术上最核心问题。
三种服务发现模式
服务发现和负载均衡并不是新问题，业界其实已经探索和总结出一些常用的模式，这些模式的核心其实是代理 (Proxy，如下图所以)，以及代理在架构中所处的位置。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VPJYN51bAIj6cDfSxhUs6eCVy3ibPzFVDiaz9ghyAjSDs0krYRgxK5DficmxDOWwY2qalZVWqeozuUkQ/640?wx_fmt=png)
在服务消费方和服务提供方之间增加一层代理，由代理负责服务发现和负载均衡功能，消费方通过代理间接访问目标服务。根据代理在架构上所处的位置不同，当前业界主要有三种不同的服务发现模式：
模式一：传统集中式代理
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VPJYN51bAIj6cDfSxhUs6eCa5xNg1Nyk21BkP4zbLNM1rTl5EqpUr49FsqAKYzDhXnbgmicgZ05gJg/640?wx_fmt=png)
这是最简单和传统做法，在服务消费者和生产者之间，代理作为独立一层集中部署，由独立团队 (一般是运维或框架) 负责治理和运维。常用的集中式代理有硬件负载均衡器 (如 F5)，或者软件负载均衡器 (如 Nginx)，F5(4 层负载)+Nginx(7 层负载) 这种软硬结合两层代理也是业内常见做法，兼顾配置的灵活性 (Nginx 比 F5 易于配置)。
这种方式通常在 DNS 域名服务器的配合下实现服务发现，服务注册 (建立服务域名和 IP 地址之间的映射关系) 一般由运维人员在代理上手工配置，服务消费方仅依赖服务域名，这个域名指向代理，由代理解析目标地址并做负载均衡和调用。
国外知名电商网站 eBay，虽然体量巨大，但其内部的服务发现机制仍然是基于这种传统的集中代理模式，国内公司如携程，也是采用这种模式。
模式二：客户端嵌入式代理
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VPJYN51bAIj6cDfSxhUs6eCEAq9S8o68dAx587F5bocAMiabNXtdyDuS8BAy9SYtvVj71dTC3nsfJA/640?wx_fmt=png)
这是很多互联网公司比较流行的一种做法，代理 (包括服务发现和负载均衡逻辑) 以客户库的形式嵌入在应用程序中。这种模式一般需要独立的服务注册中心组件配合，服务启动时自动注册到注册中心并定期报心跳，客户端代理则发现服务并做负载均衡。
Netflix 开源的 Eureka(注册中心)[附录 1] 和 Ribbon(客户端代理)[附录 2] 是这种模式的典型案例，国内阿里开源的 Dubbo 也是采用这种模式。
模式三：主机独立进程代理
这种做法是上面两种模式的一个折中，代理既不是独立集中部署，也不嵌入在客户应用程序中，而是作为独立进程部署在每一个主机上，一个主机上的多个消费者应用可以共用这个代理，实现服务发现和负载均衡，如下图所示。这个模式一般也需要独立的服务注册中心组件配合，作用同模式二。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VPJYN51bAIj6cDfSxhUs6eCQzamvWB6RlmYaVnLeMGC6sXb4vfI2Luib87wd6iaIRIjic7qG6ghkkztw/640?wx_fmt=png)
Airbnb 的 SmartStack[附录 3] 是这种模式早期实践产品，国内公司唯品会对这种模式也有探索和实践。
三种服务发现模式的比较
上面介绍的三种服务发现模式各有优劣，没有绝对的好坏，可以认为是三种不同的架构风格，在不同的公司都有成功实践。下表总结三种服务发现模式的优劣比较，业界案例和适用场景建议，供架构师选型参考：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VPJYN51bAIj6cDfSxhUs6eC9cyaNMSEZiaPTicrZ8JwaEuWmy9DbWQCMPgAZBicHU6aPP8jol8yq46vQ/640?wx_fmt=png)
服务网格 ServiceMesh 
所谓的 ServiceMesh，其实本质上就是上面提到的模式三：主机独立进程模式，这个模式其实并不新鲜，业界 (国外的 Airbnb 和国内的唯品会等) 早有实践，那么为什么现在这个概念又流行起来了呢？我认为主要原因如下：
- 
上述模式一和二有一些固有缺陷，模式一相对比较重，有单点问题和性能问题；模式二则有客户端复杂，支持多语言困难，无法集中治理的问题。模式三是模式一和二的折中，弥补了两者的不足，它是纯分布式的，没有单点问题，性能也不错，应用语言栈无关，可以集中治理。
- 
微服务化、多语言和容器化发展的趋势，企业迫切需要一种轻量级的服务发现机制，ServiceMesh 正是迎合这种趋势诞生，当然这还和一些大厂 (如 Google/IBM 等) 的背后推动有关。
模式三 (ServiceMesh) 也被形象称为边车 (Sidecar) 模式，如下图，早期有一些摩托车，除了主驾驶位，还带一个边车位，可以额外坐一个人。在模式三中，业务代码进程 (相当于主驾驶) 共享一个代理 (相当于边车)，代理除了负责服务发现和负载均衡，还负责动态路由、容错限流、监控度量和安全日志等功能，这些功能是具体业务无关的，属于跨横切面关注点 (Cross-Cutting Concerns) 范畴。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VPJYN51bAIj6cDfSxhUs6eChlab5Azm4A8opUCnQdqjkAFvngaQdFt4biavxTZBjKfMz65KKB1pQRQ/640?wx_fmt=png)
在新一代的 ServiceMesh 架构中 (下图上方)，服务的消费方和提供方主机 (或者容器) 两边都会部署代理 SideCar。ServiceMesh 比较正式的术语也叫数据平面 (DataPlane)，与数据平面对应的还有一个独立部署的控制平面 (ControlPlane)，用来集中配置和管理数据平面，也可以对接各种服务发现机制 (如 K8S 服务发现)。术语数据平面和控制平面，估计是偏网络 SDN 背景的人提出来的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VPJYN51bAIj6cDfSxhUs6eC3cnic4vAmpfJT3QjEAiaMJibTRE2j1ldhU5AMUFST8fwakrP5Ad0yTALg/640?wx_fmt=png)
上图左下角，每个主机上同时居住了业务逻辑代码 (绿色表示) 和代理 (蓝色表示)，服务之间通过代理发现和调用目标服务，形成服务之间的一种网络状依赖关系，控制平面则可以配置这种依赖调用关系，也可以调拨路由流量。如果我们把主机和业务逻辑剥离，就出现一种网格状架构 (上图右下角)，服务网格由此得名。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VPJYN51bAIj6cDfSxhUs6eC7HiclsZAEsMeXNLqmCWbUicTtic713kOliaChEmtQ7V2vOicQWMG1wlQEIA/640?wx_fmt=png)
Istio[附录 4] 是 Google/IBM 等大厂支持和推进的一个 ServiceMesh 标准化工作组，上图是 Istio 给出的 ServiceMesh 参考架构 (注意这个是老版架构，新版有一些调整，但是大框架没变)。Istio 专注在控制平面的架构、功能、以及控制平面和数据平面之间 API 的标准化，它的控制平面功能主要包括：
- 
Istio-Manager：负责服务发现，路由分流，熔断限流等配置数据的管理和下发
- 
Mixer：负责收集代理上采集的度量数据，进行集中监控
- 
Istio-Auth：负责安全控制数据的管理和下发
Envoy[附录 5] 是目前 Istio 主力支持的数据平面代理，其它主流代理如 nginx/kong 等也正在陆续加入这个阵营。kubernetes 是目前 Isito 主力支持的容器云环境。
我的建议
目前我本人并不特别看好 ServiceMesh，也不是特别建议企业在生产上试水 ServiceMesh，主要原因如下：
- 
本质上，ServiceMesh 其实并不是新东西，它只是模式三主机独立进程模式，这个模式早就有公司在探索和实践了，但是并未流行起来，可见这个模式也是存在落地挑战的。
表面上看，模式三既是模式一和模式二的折中，也解决了模式一和模式二存在的问题。
但是在每个主机上独立部署一个代理进程，是有很大运维管理开销的，一方面是规模化部署的问题 (考虑服务很多，机器也很多的场景)；另一方面是如何监控治理的问题，代理挂了怎么办？你的团队是否具备自动化运维和监控的能力？另外开发人员在服务调试的时候，会依赖于这个独立的代理，调试排错比较麻烦，这个问题怎么解决？
- 
Istio 的确做了一些标准化工作，但是没有什么特别的创新，可是说换汤不换药，就是把模式三规范化和包装了一下。透过现象看本质，Google/IBM 等行业大厂在背后推 Isito/ServiceMesh，背后有一些市场利益诉求考虑，例如 Google 要推进它的 kubernates 和公有云生态。
- 
ServiceMesh 在年初声音比较大，最近渐渐安静下来，我听到国内只有一些大厂 (华为，新浪微博，蚂蚁金服等) 在试水，实际生产级落地的案例聊聊无几。大多数企业对 ServiceMesh 只是观望，很多架构师对 ServiceMesh 实际落地都存在疑虑。
**所以我的个人建议，对于大部分企业 (一般运维和研发能力不是特别强)，采用模式一集中代理模式就足够了**。这个模式比较传统不新鲜，但是在很多一线企业已经切实落地，我甚至认为，除了一些大厂，大部分中小企业的服务发现架构采用的就是集中代理。我本人经历过三家互联网公司，大的有 eBay，中等有携程，小的有拍拍贷，都是采用集中式代理模式，而且玩得都很好。我的架构理念很简单，对于生产级应用，不追新，老实采用大部分企业落地过的方案。
模式一的最大好处是集中治理，应用不侵入，语言栈无关，另外因为模式一是集中部署的，不像模式三是分布式部署，所以模式一的运维开销也远小于模式三。对于模式一，大家最大的顾虑是性能和单点问题，其实性能还是 OK 的，如果架构和容量规划合理的话，实际生产中经过集中代理的性能开销一般可以控制在小于 10 个 ms，eBay 和携程等大流量企业的成功实践已经验证了这点。单点问题一般建议采用两层负载结构，例如硬件 F5+ 软件 nginx 两层负载，F5 以主从 HA 部署，nginx 则以集群多实例部署，这种架构兼顾了高可用和配置的灵活性。
另外，模式一还可以和服务注册中心结合，从而降低手工配置的复杂性，实现 DevOps 研发自助部署，一种方案如下图所示：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VPJYN51bAIj6cDfSxhUs6eCf5l63NtPb8VF0icQVHTfWugTeibf77YaiaUaibNpNCib4PO74pdzLP2qPKw/640?wx_fmt=png)
服务启动时自动注册到服务注册中心并定期报心跳，Proxy 则定期到服务注册中心同步实例。这种方式下，不需要为每个服务申请一个域名，只需一个泛域名即可，消费者访问服务时采用服务名 + 泛域名即可，整个服务上线流程可以做到 DevOps 研发自助。目前社区流行的一些开源代理如 traefik[附录 7] 和 kong[附录 8] 等都支持和多种服务注册中心 (Consul/Eureka/Etcd/Zookeeper 等) 进行集成。目前这种方案在拍拍贷有初步成功实践，采用 kong[附录 7] 和自研服务注册中心 Radar[附录 8]，同时和容器云调度平台配合，实现了研发全自助式发布上线。
结   论
1、服务注册发现和负载均衡是微服务架构在技术上的根本问题，解决的办法是采用代理 Proxy。根据代理在架构上的位置不同，服务发现代理一般有三种模式：
- 
模式一：集中式代理
- 
模式二：客户端嵌入式代理
- 
模式三：主机独立进程代理
这三种模式没有绝对的好坏之分，只是三种不同的架构风格，各有优劣和适用场景，在不同企业都有成功落地案例。
2、ServiceMesh 本质上就是模式三中主机独立进程代理，它结合了模式一和模式二的优势，但是分布式部署运维管理开销大。Istio 对 ServiceMesh 的架构、功能和 API 进行了标准化。
3、ServiceMesh 还在演进中，生产落地仍有挑战，一般企业不建议生产级使用。集中式代理最成熟，对于一般中小企业，建议从集中式代理开始，等达到一定规模和具备一定的研发运维能力，再根据需要考虑其它服务发现模式。
4、架构师不要盲目追新，在理解微服务架构原理的基础上，可以学习和试点新技术，但是对于生产级应用，应该以成熟稳定，有大规模落地案例作为选型第一准则。
