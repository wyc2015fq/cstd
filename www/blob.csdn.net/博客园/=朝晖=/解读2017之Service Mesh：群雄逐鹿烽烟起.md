# 解读2017之Service Mesh：群雄逐鹿烽烟起 - =朝晖= - 博客园
# [解读2017之Service Mesh：群雄逐鹿烽烟起](https://www.cnblogs.com/dhcn/p/8206824.html)
http://www.infoq.com/cn/articles/2017-service-mesh
## 前言
在过去的2016年和2017年，微服务技术得以迅猛普及，和容器技术一起成为这两年中最吸引眼球的技术热点。而以Spring Cloud为代表的传统侵入式开发框架，占据着微服务市场的主流地位，它甚至一度成为微服务的代名词。
直到2017年年底，当非侵入式的Service Mesh技术终于从萌芽到走向了成熟，当Istio/Conduit横空出世，人们才惊觉：微服务并非只有侵入式一种玩法，更不是Spring Cloud的独角戏！
这一次的新生力量，完全不按照常理出牌，出场就霸道地掀翻桌子，直接摆出新的玩法：Service Mesh，下一代微服务！这一场大战，在 2017 年的最后一个月，终于上演到白热化，被摆上了台面，受到越来越多人关注。往日霸主 Spring Cloud，此时只能沦为看客。
2017 年的 Service Mesh 历程，在平淡中开始，如戏剧般结束，留给我们一个充满想象和憧憬的 2018。让我们一起来回顾这堪称精彩的一年。
## Service Mesh 的萌芽期
在我们正式开始 2017 年回顾之前，我们将时间稍微放前一点，回到 2016 年，有些故事背景需要预先交代一下。
虽然直到 2017 年年底，Service Mesh 才开始较大规模被世人了解，这场微服务市场之争也才显现，但是其实 Service Mesh 这股微服务的新势力，早在 2016 年年初就开始萌芽：
- 2016 年 1 月 15 日，离开 Twitter 的基础设施工程师 William Morgan 和 Oliver Gould，在 GitHub 上发布了 Linkerd 0.0.7 版本，他们同时组建了一个创业小公司 Buoyant，业界第一个 Service Mesh 项目诞生。
- 2016 年，Matt Klein 在 Lyft 默默地进行 Envoy 的开发。Envoy 诞生的时间其实要比 Linkerd 更早一些，只是在 Lyft 内部不为人所知。
在 2016 年年初，“Service Mesh”还只是 Buoyant 公司的内部词汇，而之后，它开始逐步走向社区：
- 2016 年 9 月 29 日在 SF Microservices 上，“Service Mesh”这个词汇第一次在公开场合被使用。这标志着“Service Mesh”这个词，从 Buoyant 公司走向社区。
- 2016 年 10 月，Alex Leong 开始在 Buoyant 公司的官方 Blog 中连载系列文章“A Service Mesh for Kubernetes”。随着“The Services must Mesh”口号的喊出，Buoyant 和 Linkerd 开始 Service Mesh 概念的布道。
在这一年中，第一代的 Service Mesh 产品在稳步推进：
- 2016 年 9 月 13 日，Matt Klein 宣布 Envoy 在 GitHub 开源，直接发布 1.0.0 版本。
- 2016 年下半年，Linkerd 陆续发布了 0.8 和 0.9 版本，开始支持 HTTP/2 和 gRPC，1.0 发布在即；同时，借助 Service Mesh 在社区的认可度，Linkerd 在年底开始申请加入 CNCF。
而在这个世界的另外一个角落，Google 和 IBM 两位巨人，握手开始合作，他们联合 Lyft，启动了 Istio 项目。这样，在第一代 Service Mesh 还未走向市场主流时，以 Istio 为代表的第二代 Service Mesh 就迫不及待地上路。
现在我们可以进入主题，开始 2017 年 Service Mesh 发展历程的回顾。
## 急转而下的 Linkerd
- 2017 年，Linkerd 迎来了一个梦幻般的开局，喜讯连连：
- 2017 年 1 月 23 日，Linkerd 加入 CNCF。
- 2017 年 3 月 7 日，Linkerd 宣布完成千亿次产品请求。
- 2017 年 4 月 25 日，Linkerd 1.0 版本发布。
可谓各条战线都进展顺利：产品完成 1.0 release，达成最重要的里程碑；被客户接受并在生产线上成功大规模应用，这代表着市场的认可；进入 CNCF 更是意义重大，这是对 Linkerd 的极大认可，也使得 Linkerd 声名大噪，一时风光无量。
需要特别指出的是，Linkerd 加入 CNCF，对于 Service Mesh 技术是一个非常重要的历史事件：这代表着社区对 Service Mesh 理念的认同和赞赏，Service Mesh 也因此得到社区更大范围的关注。
趁热打铁，就在 Linkerd 1.0 版本发布的同一天，创作者继续 Service Mesh 的布道：
- 2017 年 4 月 25 日，William Morgan 发布博文“What’s a service mesh? And why do I need one?”。正式给 Service Mesh 做了一个权威定义。
然而现实总是那么残酷，这个美好的开局，未能延续多久就被击碎：
- 2017 年 5 月 24 日，Istio 0.1 release 版本发布，Google 和 IBM 高调宣讲，社区反响热烈，很多公司在这时就纷纷站队表示支持 Istio。
Linkerd 的风光瞬间被盖过，从意气风发的少年一夜之间变成过气网红。当然，从产品成熟度上来说，linkerd 作为业界仅有的两个生产级 Service Mesh 实现之一，暂时还可以在 Istio 成熟前继续保持市场。但是，随着 Istio 的稳步推进和日益成熟，外加第二代 Service Mesh 的天然优势，Istio 取代第一代的 Linkerd 只是个时间问题。
面对 Google 和 IBM 加持的 Istio，Linkerd 实在难有胜算：
- Istio 作为第二代 Service Mesh，通过控制平面带来了前所未有的控制力，远超 Linkerd。
- Istio 通过收编和 Linkerd 同为第一代 Service Mesh 的 Envoy，直接拥有了一个功能和稳定性与 Linkerd 处在一个水准的数据平面（也就是作为 sidecar 模式部署的 proxy）。
- 基于 C++ 的 Envoy 在性能和资源消耗上本来就强过基于 Scala/JVM 的 Linkerd。
- Google 和 IBM 组合在人力、资源和社区方面的影响力远非 Buoyant 这样的小公司可以比拟。
Linkerd 的发展态势顿时急转而下，未来陷入一片黑暗。出路在哪里？
在一个多月后，Linkerd 给出一个答案：和 Istio 集成，成为 Istio 的数据面板：
- 2017 年 7 月 11 日，Linkerd 发布版本 1.1.1，宣布和 Istio 项目集成。Buoyant 发表博文“Linkerd and Istio: like peanut butter and jelly”。
这个方案在意料之中，毕竟面对 Google 和 IBM 的联手威胁，选择低头和妥协是可以理解的，只是这里边存在两个疑问：
- 1、和 Envoy 相比，Linkerd 并没有特别优势，考虑编程语言的天生劣势，Linkerd 想替代 Envoy 难度非常之大。
- 2、即使替代成功，在 Istio 的架构下，只是作为一个数据平面存在的 Linkerd，可以发挥的空间有限。这种境地的 Linkerd，是远远无法承载起 Buoyant 的未来的。
Linkerd 的这个谜团，直到 2017 年即将结束的 12 月，在 Conduit 发布之后才被解开。
## 波澜不惊的 Envoy
自从在 2016 年决定委身于 Istio 之后，Envoy 就开始波澜不惊地平稳发展，这和 Linkerd 的跌宕起伏完全不同。
在功能方面，由于定位在数据平面，因此 Envoy 无需考虑太多，很多工作在 Istio 的控制平面完成就好，Envoy 从此专心于将数据平面做好，完善各种细节。在市场方面，Envoy 和 Linkerd 性质不同，不存在生存和发展的战略选择，也没有正面对抗生死大敌的巨大压力。Envoy 在 2017 年有条不紊地陆续发布了 1.2、1.3、1.4 和 1.5 版本，稳步地完善自身，表现非常稳健。
稳扎稳打的 Envoy 在 2017 年一方面继续收获独立客户，一方面伴随 Istio 一起成长。作为业界仅有的两个生产级 Service Mesh 实现之一，Envoy 随后收获了属于它的殊荣：
- 2017 年 9 月 14 日，Envoy 加入 CNCF，成为 CNCF 的第二个 Service Mesh 项目。
可谓名至实归，水到渠成。作为一个无需承载一家公司未来的开源项目，Envoy 在 2017 年的表现，无可挑剔。
## 背负使命的 Istio
从 Google 和 IBM 联手决定推出 Istio 开始，Istio 就注定永远处于风头浪尖，无论成败。
Istio 背负了太多的使命：
- 建立 Google 和 IBM 在微服务市场的统治地位。
- 为 Google 和 IBM 的公有云打造杀手锏级特性。
- 在 k8s 的基础上，延续 Google 的战略布局。
Google 在企业市场的战略布局，是从底层开始，一步一步向上，一步一步靠近应用。刚刚大获全胜的 k8s 为 Istio 准备了一个非常好的基石，而 Istio 的历史使命，就是继 k8s 拿下容器编排之后，更进一步，拿下微服务！
![](articles/2017-service-mesh/zh/resources/761-1515690947479.jpg)
2017 年，Istio 稳步向前，先后发布四个版本：
- 2017 年 5 月 24 日，Istio 0.1 release 版本发布。
- 2017 年 10 月 4 日，Istio 0.2 release 版本发布。
- 2017 年 11 月 30 日，Istio 0.3 release 版本发布。
- 2017 年 12 月 15 日，Istio 0.4 release 版本发布。
在社区方面，Istio 借助 Google 和 IBM 的大旗，外加自身过硬的实力、先进的理念，很快获得了社区的积极响应和广泛支持。包括 Oracle 和 Red Hat 在内的业界大佬都明确表示对支持 Istio。
在平台支持方面，Istio 的初期版本只支持 k8s 平台，从 0.3 版本开始提供对非 k8s 平台的支持。从策略上说，Istio 借助了 k8s，但是没有强行绑定在 k8s 上。
Istio 面世之后，赞誉不断，尤其是 Service Mesh 技术的爱好者，可以说是为之一振：以新一代 Service Mesh 之名横空出世的 Istio，对比 Linkerd，优势明显。同时产品路线图上有一大堆令人眼花缭乱的功能。假以时日，如果 Istio 能顺利地完成开发，稳定可靠，那么这会是一个非常美好、值得憧憬的大事件，它的意义重大：
- 重新定义微服务开发方式，让 Service Mesh 成为主流技术。
- 大幅降低微服务开发的入门门槛，让更多的企业和开发人员可以落地微服务。
- 统一微服务的开发流程，标准化开发 / 运维方式。
奈何，事情的发展总是不会这么简单地如人所愿。Istio 发布之后，试用中就被发现问题较多，0.1 版本时还比较容易被接受，但是接下来的 0.2、0.3 和 0.4，Istio 在可用性上并没有明显的改观，导致迄今在全球范围内都几乎没有听到 Istio 上生产的案例，公司都将其停留在简单试用阶段。
此时再看 Istio 琳琅满目的各种功能，不禁让人疑惑 Istio 的产品策略：为什么一开场就将摊子铺的如此之大？以至于开发时间长达一年 (注意，虽然开源才半年多，但是开源前已经在开发)，却无法得到一个稳定可用的版本。
这有悖于互联网产品的开发理念。下边这个经典图片相信大家并不陌生：
![](articles/2017-service-mesh/zh/resources/672-1515690947146.jpg)
从目前情景看，Istio 已经在图上“不应该”的产品迭代路径上走了一年。从 5 月份 0.1 版本发布开始，我们就满心期待，却陷入“过尽千帆皆不是”的尴尬境地：每一次新版本试用后的结果，都不理想。
身处局外，无法了解 Istio 项目开发的背景和真实情况，也自然无法得知为何会如此，我们只能由衷地希望，Istio 能在 2018 年尽快完成计划中的产品开发，实现生产可用。个人意见：哪怕推迟某些特性的实现，也希望能做到主体部分尽快完善。
2018 年 Service Mesh 的整体走势，很大程度取决于 Istio：如果 Istio 能在 2018 年上半年实现生产可用，哪怕是牺牲部分高级特性，也足以推动整个 Service Mesh 向前大步迈进。反之如果进展不顺，市场会呈现观望和等待的态势，也会给竞争对手机会，比如说，下面将要出场的 Conduit。
## 背水一战的 Conduit
2017 年底的 KubeConf，在 Service Mesh 成为大会热点、Istio 备受瞩目时，Buoyant 公司出人意料地给了踌躇满志又稍显拖沓的 Istio 重重一击：
- 2017 年 12 月 5 日，Conduit 0.1.0 版本发布，Istio 的强力竞争对手亮相 KubeConf。
Conduit 的整体架构和 Istio 一致，借鉴了 Istio 数据平面 + 控制平面的设计，同时别出心裁地选择了 Rust 编程语言来实现数据平面，以达成 Conduit 宣称的更轻、更快和超低资源占用。
继 Isito 之后，业界第二款第二代 Service Mesh 产品就此诞生。话说得有些拗口，但是一场大战就此浮出水面。Buoyant 在 Linkerd 不敌 Istio 的恶劣情况下，绝地反击，祭出全新设计的 Conduit 作为对抗 Istio 的武器。
需要额外指出的是，作为一家初创型企业，在第一款主力产品 Linkerd 被 Istio 强力阻击之后，Buoyant 已经身陷绝境，到了生死存亡之秋，作为背负公司期望，担负和 Istio 正面抗衡职责的 Conduit，可谓压力巨大。
从目前得到的信息分析，Conduit 明显是有备而来，针对 Istio 当前状况，针锋相对的：
- 编程语言：为了达成更轻、更快和更低资源消耗的目标，考虑到 Istio 的数据面板用的是基于 C++ 语言的 Envoy，Conduit 跳过了 Golang，直接选择了 Rust，颇有些剑走偏锋的意味。不过，单纯以编程语言而言，在能够完全掌握的前提下，Rust 的确是做 proxy 的最佳选择。考虑到 Envoy 在性能方面的良好表现，Conduit 要想更进一步，选择 Rust 也是可以理解。
- 架构设计：在借鉴 Istio 整体架构的同时，Conduit 做了一些改进。首先 Conduit 控制平面的各个组件是以服务的方式提供功能的，极富弹性。另外，控制平面特意为定制化需求进行了可扩展设计，可以通过编写 gPRC 插件来扩展 Conduit 的功能而无需直接修改 Conduit，这对于有定制化需求的客户是非常便利的。
- 产品演进：这是最重要的一点！Conduit 完全吸取了 Istio 的教训，因此它的产品迭代路径会是我们最期待的方式。在本文撰写期间，笔者特意和 Conduit 的 CEO William 深入探讨过这个话题，得到了一个非常令人欣慰的答复：Minimal feature set，prod ready as quickly as possible。
然而，要抗衡 Istio 和其身后的 Google 与 IBM，谈何容易。Conduit 2018 年的发展道路，注定是充满挑战的，艰难险阻可想而知。但是，不得不佩服 Buoyant 公司，以及以 CEO William 为首的那支充满挑战精神的团队，有理想、有追求、有魄力、有勇气！期待他们在 2018 年的表现。
让我们回到 Istio 和 Conduit 的竞争格局。从目前局面看，Istio 先天优势明显，但是产品策略上的选择给了 Conduit 一个难得的机会。接下来的 2018 年，在 Conduit 的威胁和刺激下，希望 Istio 能打起精神，给出一份令大家满意的答卷。期待 Istio 和 Conduit 能在 2018 年形成良性竞争，共同引领 Service Mesh 的大潮。
## 低调的参与者
2017 年的 Service Mesh，除了业界先驱 Linkerd/Envoy，和后起之秀 Istio/Conduit，还有一些其它的竞争者进入这个市场，只是它们都非常低调。
首先是 nginMesh，来自大名鼎鼎的 Nginx：
- 2017 年 9 月，在美国波特兰举行的 nginx.conf 大会上，Nginx 宣布了 nginMesh。随即在 GitHub 上发布了 0.1.6 版本。
- 2017 年 12 月 6 日，nginMesh 0.2.12 版本发布。
- 2017 年 12 月 25 日，nginMesh 0.3.0 版本发布。
nginMesh 的定位是作为 Istio 的服务代理，也就是替代 Envoy，思路和 Linkerd 之前和 Istio 集成很相似。nginMesh 在发布后的两个多月，GitHub 上提交非常少，直到最近突然发力，先后发布了 0.2 和 0.3 版本。不过 nginMesh 极度低调，GitHub 上的 star 也只有不到 100。
然后是 Kong，但是这个比默默无闻的 nginMesh 更加低调，只是曾经有传闻 Kong 有意 Service Mesh，但是后来没了下文。不过 Kong 的 GitHub 项目介绍里，悄悄地加上了 Service Mesh 的字样：Kong is a ××× Microservice Abstraction Layer (also known as an API Gateway， API Middleware or in some cases Service Mesh)。
在 2017 年，这些低调的参与者，几乎没有引起外界任何注意，也无法预期他们在 2018 年会如何表现。从社区的角度，还是希望有更多的参与者进入 Service Mesh 市场，以推动整个市场的健康发展。
就在本文撰写之时，在2017年的最后几天，大名鼎鼎的F5 Networks公司突然放出了他们的Service Mesh类产品“Aspen Mesh”，基于Istio构建，目标“企业服务网格”。需要特别强调的是，F5在Service Mesh上的坚定决心：砍掉原有传统产品思路的项目，以内部孵化项目的方式组建独立自治团队，并在新方向上重新开始。而且在Istio才0.1版本的时候F5就做好战略决策，之后默默耕耘，其决策者的胆识令人敬佩。F5在Service Mesh这个新兴技术领域表现出积极进取的姿态，立足Istio完善企业级特性，这也是一条值得探索的路线，期待2018年Aspen Mesh的进展。
## 快速升温的国内
2017 年，随着 Servic Mesh 的发展，国内技术社区也开始通过新闻报道 / 技术文章等接触 Service Mesh，但是传播范围和影响力都非常有限。直到年底才剧烈升温，开始被国内技术社区关注：
- 2017 年 10 月 16 日，在 2017 QCon 上海大会上，我做了一个“Service Mesh：下一代微服务”的演讲，成为 Service Mesh 技术在国内大型技术峰会上的第一次亮相。
- 2017 年 11 月，国内第一个 Service Mesh 技术社区“Service Mesh 中文网”(http://servicemesh.cn) 成立。
- 2017 年 12 月，在全球架构师峰会（ArchSummit）2017 北京站上，来自华为的田晓亮做了名为“Service Mesh 在华为云的实践”的分享。
- 2017 年 12 月 16 日，来自新浪微博的周晶做了名为“微博 Service Mesh 实践”的演讲，分享了 Service Mesh 在微博的落地情况。
此外，作为 Servic Mesh 国内最早的开发和实践者的华为和新浪微博，都积极参与开源。其中新浪微博 Service Mesh 的核心实现，跨语言通信和服务治理已经在 Motan 系列项目中提供，而华为也将稍后开源他们基于 Golang 的 Service Mesh 代码实现。
特别要指出的是，华为目前已经在公有云上将 Service Mesh 作为公共服务提供，这在国内公有云中是第一家。预计随着 Service Mesh 的落地和普及，公有云提供生产级别的 Service Mesh 服务将成为标配。在国外 Google/IBM/Amazon 等公有云都有提供 Service Mesh 的计划，相信国内公有云也会陆续跟进。
## 展望 2018
2017 年的 Service Mesh 市场，从 Linkerd 的风光无限开始，到 Istio 的横空出世，最后止于 Conduit 的绝地反击，可谓一波三折；产品也经历从第一代的 Linkerd/Envoy，跨越性的演化出第二代的 Istio/Conduit；同时，技术社区的态度也从年初的逐步接受发展到年底的热烈追捧，下面这张 KubeConf 上的图片非常有代表性地展示了社区的热切期望：
![](articles/2017-service-mesh/zh/resources/573-1515690947328.jpg)
然而 Service Mesh 终究是一个新兴的技术，尤其作为未来主流的 Istio/Conduit 迄今还没有实现产品级别可用，因此 2018 年对 Service Mesh 而言，必然不是一帆风顺，必然是充满荆棘和坎坷的。如何实现从技术理念到产品落地，如何实实在在地解决实践中遇到的各种问题，将会是这一年中至关重要的事情。
衷心祝愿 Istio 和 Conduit（也许还有其他的产品）可以在 2018 年快速成长，实现社区期待的功能和可用性，可以真正地实现降低微服务门槛的目标，让 Service Mesh 成为名副其实的下一代微服务。
2018 年的 Service Mesh，值得期望！
## 作者介绍
敖小剑，数人云资深架构师。十五年软件开发经验，微服务专家，专注于基础架构，Cloud Native 拥护者，敏捷实践者，坚守开发一线打磨匠艺的架构师。曾在亚信、爱立信、唯品会和 ppmoney 任职，机缘巧合下对基础架构和微服务有过深入研究和实践，目前在数人云主持微服务和基础架构相关产品的设计和开发。

