# 百度Apollo发布中国首个自动驾驶安全报告，L3级别产品2020年量产上市 - 人工智能学家 - CSDN博客
2018年07月04日 23:10:19[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：186
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVlQibeia9tAQMxiakWq9X0G5VgbHJkvd3mPaSgvXkicc0hiaF3rzMsuVEC2icUro9cLVPe1r7nO2Hn8mpA/640?wx_fmt=png)
来源：雷锋网
去年 10 月，全球自动驾驶研发先行者 Waymo 发布了长达 43 页的安全报告，里面详细说明了如何装备和训练自动驾驶车辆，从而避免驾驶过程中一些意外情况的发生。
今年 1 月，美国车企巨头通用也发布了“2018 自动驾驶安全报告”，详尽介绍了其自动驾驶汽车的理念、设计过程和技术特点。
发布详尽的安全报告，似乎已经成为这些巨头们推动自动驾驶向前发展的一项基础工作。
作为国内最大的自动驾驶开放平台，百度 Apollo 在 2018 百度 AI 开发者大会（Creat 2018）上，重磅发布了**“Apollo Pilot 安全报告”**。
### **APC：中国首个 L3 自动驾驶产品**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YjhmbbkdV6vVdRpUxPpNYBgJh0opDN51Et3NdmV3iaHDBVicwg3Vu9qiaHoWSwYKeY9TiaW7SAibeujsy1VK4ibX5kKA/640?)
这里的 Apollo Pilot，是百度 Apollo 平台专门针对量产所设计的自动驾驶产品的总称，所以它自然包含了乘用车、商用车、物流车等自动驾驶车型产品以及针对每种车型设计的自动驾驶软硬件系统方案。
借着这个报告，百度 Apollo 首推的一款产品就是 Apollo Pilot For PassengerCar（简称“APC”），百度将其称为“中国首个 L3 级自动驾驶产品”。顾名思义，PassengerCar 的主要功能就是载人，而更为官方的定义是：APC 指“提供有驾驶员的车辆在限定场景下的自动驾驶系统；根据系统请求，驾驶员需要做出适当的响应，系统将会至少提前 10 秒发出请求，以便给驾驶员保留安全响应时间”。
报告提到，现在市面上存在的自动驾驶产品，都要求用户时刻保持接管能力，一旦发生事故，责任属于用户，这反而需要用户更加集中精神，容易诱发疲劳驾驶。APC 为了解决这样的问题，提出了“可适用场景”（Operational Design Domain）的概念，简言之，就是当车辆处在可适用场景范围内时，车辆基本上可以自己驾驶；而在即将驶出该范围时，系统会给用户预留至少 10 秒的接管时间。通过这种设定，用户能在有需要的场景下真正享用安全的自动驾驶服务。
百度为 APC 设定的量产上市时间点是 2020 年，将优先在三个高频场景下实现安全自动驾驶能力。这三个高频场景所对应的自动驾驶系统分别是：高速自动驾驶系统、城市交通拥堵辅助系统以及自动泊车系统。
其中，高速自动驾驶系统能让车辆在高速和城市环路上实现自动驾驶功能，将对车辆进行加速减速、车道保持和自主变道等控制；城市交通拥堵辅助系统则通过堵车场景中自动驾驶系统的启用降低追尾风险，环节用户一直专注跟车所造成的疲劳感；自动泊车系统是最好的理解的，包括自主泊车和召唤车辆两项能力。
### **安全报告探讨了哪些内容？**
为了保证 APC 的安全稳定运行，百度 Apollo 为其设定了一个整体的安全框架，包括安全感、安全驾驶、事故预防、事故介入以及事故后处理五大逐层递进的安全设计。也想以此来引导用户形成正确的自动驾驶系统使用习惯。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YjhmbbkdV6vVdRpUxPpNYBgJh0opDN51UhenlCxtxjfKXeUjWbcmMiav70yhK0rGTFWUdWQ9UYvkHgrIglzGlAA/640?)
在完成对 APC 产品的定义之后，百度 Apollo 用安全报告接下来长达 60 多页的篇幅详细介绍了该产品的“安全设计”和“安全运行”两大方面的重要内容。
其中“安全设计”一部分花了 4 个章节来介绍，主要包括操作安全、环境安全、行为安全和功能安全这些内容，可以看出这部分内容主要是针对车辆与系统本身的设计来说的；而“安全运行”部分则包含 3 个章节，分别是质量安全、安全进化以及机制安全，其中讨论的细节问题有诸如路测、信息安全、OTA 系统、法律法规以及事故处理机制。
为了保证自动驾驶车辆能对自身所处的静态环境和动态环境有清晰明确的认知，百度 Apollo 为 APC 设计了 3 大系统，分别是 XMI 系统、环境建模系统以及 DPS 驾驶策略系统。
其中，XMI 包括 HMI 和 EMI，EMI 指的是自动驾驶系统与环境进行输入和输出的接口，XMI 就是车内交互和车外交互的集合；环境建模系统则包括通过车身传感器、地图以及 V2X 设施；而DPS 驾驶策略系统则希望达到两个目标：全面性和可解释性，主要还是为了应对监管和法律问题。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YjhmbbkdV6vVdRpUxPpNYBgJh0opDN51wGLnZImmXTolNicIoArglQdvuACeRsic8ofr6CG9URpDU1YFCMian64xQ/640?)
以 APC 产品的 HMI 设计为例，雷锋网新智驾从安全报告中了解到的信息是，百度 Apollo 特意设计了共享全面屏、环绕氛围灯、智能方向盘和体感座椅等组件，为自动驾驶汽车驾驶员带来更为沉浸的交互体验。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YjhmbbkdV6vVdRpUxPpNYBgJh0opDN51KvMerLn7FlDu6NZ4bP083U6WBWm4iajZsddgZA691H5ezsqckW4H63w/640?)
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YjhmbbkdV6vVdRpUxPpNYBgJh0opDN51MXQlLkdZiaNHTnSqS9Ud2BG8tjwxiaAqZuLWT22pvM9e7icvGELBXNXyg/640?)
另外，在自动驾驶汽车的接管机制上，百度 Apollo 针对其 APC 产品有一套明确逻辑：
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YjhmbbkdV6vVdRpUxPpNYBgJh0opDN51jLO8Tk7xo3IGCQq5HB0SicHtZicO1HiawmeEc4l7ELdId6XGSlOwlwo2w/640?)
目前阶段的 APC 还只能在经过大量训练和测试的限定操作环境（上文提及的 ODD）内自动驾驶，超出 ODD 边界时需要用户接管。
而且，APC 的接管系统并不想让用户一直在需要接管和不需要接管的状态之间频繁切换，所以该系统会进行绝大部分的自动处理，包括实时监控、诊断、暂时的降级运行等，其要实现的目标是，在自动驾驶场景下的 95% 以上的时间里，都不会让用户做好接管车辆的准备，这才能真正解放用户。
雷锋网新智驾了解到，APC 专门设计了“特别关怀安全停车”的功能，当自动驾驶系统遇到人员状态异常时（比如驾驶员睡着或者突发疾病），APC 在保证安全刹停之外，还会一直持续自动驾驶到可停泊的安全区域进行停车，保证车辆和人员安全。
有意思的是，在这份安全报告中，百度 Apollo 还引入了 Mobileye 的自动驾驶责任敏感安全模型（Responsibility-Sensitive Safety，RSS 模型），会将其融合应用在 APC 的安全模型中。未来双方将合作打造适用于中国驾驶环境的安全模型和驾驶策略，并在 Apollo 平台上进行开放。 
安全报告在后续的内容当中还探讨了如何对自动驾驶汽车的驾驶员进行培训以及如何进行安全测试，同时还有如何推动相关法律法规、保障机制建设等介绍。
在驾驶员培训方面，APC 系统将通过 4 个方面的数据收集和训练，来培养一个具备全面能力的自动驾驶“老司机”。具体包括：情景训练、监控发现能力训练、路况训练以及事故应对能力训练。
在路测和数据收集方面，百度 Apollo 搭建了一个名为 ADS 的场景库，覆盖了前文所述的 APC 的三大高频应用场景，还包含了光照、季节、时间段、天气、全国各种区域和地形、障碍物类型、路面状况、隧道匝道、分流合流等各种常见及极限驾驶状况的数据。为了拿到这些数据，百度 Apollo 方面称他们已经收集了数亿公里的真实道路场景数据。
以高速/环路自动驾驶功能的整车测试为例，在交付前，百度 Apollo 将确保其进行 120 万公里以上的整车测试，相当于对全国 30 万公里的高速和环路的每一公里都进行 4 次全量覆盖。
除了实实在在的物理安全，自动驾驶系统的网络信息安全问题也同样重要，所以百度 Apollo 在设计系统时为对外通信层、接入网关层以及车内应用层都设置了防护。这些防护将成为其 APC 产品的标准配置。
当然，APC 产品要走向量产，还需要满足各类车规级的严苛要求，保证稳定性。在报告中，百度 Apollo 提到，他们在车辆设计之初就在和整车厂的工程师进行合作，对各种有可能出现的失效和风险进行把控，并且在软件、硬件、系统以及整车层面进行标准测试。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YjhmbbkdV6vVdRpUxPpNYBgJh0opDN51Jyn4Aztl2jiaIJOMjlvUwHDCZAzQibdxiaqc1jJ23IZCodmSIbjuvxblA/640?)
在法律法规建设方面，目前国内多个城市已经开始下发自动驾驶路测牌照，百度 Apollo 也拿到了其中的多块牌照，未来还将为中国自动驾驶相关法律法规建设提供更多经验。
报告下载：https://pan.baidu.com/s/1IfTZHuwnRZIkhJ4coWn3DA 密码:s8ra
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVlQibeia9tAQMxiakWq9X0G5VuZpjK4KiasLoHO1eD6sED2PLgj04OmrjR2ZT1GT5nIicNyox15YsdKfg/640?wx_fmt=jpeg)
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
