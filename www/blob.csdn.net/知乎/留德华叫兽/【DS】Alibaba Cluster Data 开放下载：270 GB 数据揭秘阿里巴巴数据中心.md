# 【DS】Alibaba Cluster Data 开放下载：270 GB 数据揭秘阿里巴巴数据中心 - 知乎
# 

> **文章作者：临石**
*欢迎原链接转发，转载请私信*[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)*获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：[『运筹OR帷幄』大数据人工智能时代的运筹学](https://zhuanlan.zhihu.com/operations-research)

## 打开一篇篇 IT 技术文章，你总能够看到“大规模”、“海量请求”这些字眼。如今，这些功能强大的互联网应用，都运行在大规模数据中心上。然而，对于大规模数据中心，你又了解多少呢？
![](https://pic3.zhimg.com/v2-c8c4bdfd46b79f5a5515ae8113852aaa_b.jpg)
## 实际上，除了阅读一些科技文章之外，得到关于数据中心的信息非常难得。数据中心每个机器的运行情况如何？这些机器上运行着什么样的应用？这些应用有什么特点？对于这些问题，除了少数资深从业者之外，普通学生和企业的研究者很难了解其中细节。

## **什么是 Alibaba Cluster Data?**

2015 年，我们尝试在阿里巴巴的数据中心，将延迟不敏感的批量离线计算任务和延迟敏感的在线服务部署到同一批机器上运行，让在线服务用不完的资源充分被离线使用以提高机器的整体利用率。

经过 3 年多的试验论证、架构调整和资源隔离优化，目前这个方案已经走向大规模生产。我们通过混部技术将集群平均资源利用率从 10% 大幅度提高到 45%。另外，通过各种优化手段，可以让更多任务运行在数据中心，将“双11”平均每万笔交易成本下降了 17%。

那么，实施了一系列优化手段之后的计算机集群究竟是什么样子？混部的情况究竟如何？

为了让有兴趣的学生以及相关研究人员，可以从数据上更加深入地理解大规模数据中心，我们特别发布了这份数据集（Alibaba Cluster Data V2018）。数据集中记录了某个生产集群中服务器以及运行任务的详细情况。我们希望这波数据的发布可以拉近我们与学术研究、业界同行之间的距离。

在数据集中，你可以详细了解到我们是如何通过混部把资源利用率提高到 45%；我们每天到底运行了多少任务；以及业务的资源需求有什么特点。如何使用这份数据集，完全取决于你的需要。

## **这个数据可以做什么？**

刚刚发布的 Alibaba Cluster Data V2018 包含 6 个文件，压缩后大小近 50GB（压缩前 270+GB），里面包含了 4000 台服务器、相应的在线应用容器和离线计算任务长达 8 天的运行情况。

通过这份数据，你可以：
- 了解当代先进数据中心的服务器以及任务运行特点；
- 试验你的调度、运筹等各种任务管理和集群优化方面的各种算法并撰写论文；
- 利用这份数据学习如何进行数据分析，揭示更多我们自己都未曾发现的规律。

上面这几点，没有接触过类似数据的朋友，可能对于这份数据的用处并没有直观的印象，下面我举几个简单的例子：
- 电商业务在白天和晚上面临的压力不同，我们如何在业务存在波峰波谷的情况下提高整体资源利用率？
- 你知道我们最长的 DAG 有多少依赖吗？
- 一个典型的容器存在时间是多久？
- 一个计算型任务的典型存在时间是多少？一个 Task 的多个 Instance 理论上彼此很相似，但是它们运行的时间都一样吗？

实际上，学者们甚至可以用这些数据作出更加精彩地分析。2017年，我们开放的第一波数据（Alibaba Cluster Data V2017），已经产生了多篇优秀的学术成果。

以下是学者们在论文中引用数据（Alibaba Cluster Data V2017）的例子，其中不乏被 OSDI 这样顶级学术会议收录的优秀文章。我们期待，未来你也能与我们共同分享你用这份数据产生的成果！

**"LegoOS: A Disseminated, Distributed OS for Hardware Resource Disaggregation, Yizhou Shan, Yutong Huang, Yilun Chen, and Yiying Zhang, Purdue University. OSDI'18"** (Best paper award!)
![](https://pic4.zhimg.com/v2-950f0ae10fa045ceb077185ebdcd580b_b.jpg)![](https://pic1.zhimg.com/v2-8775e9486bf228c81fb0bd9c252194e4_b.jpg)
**"CharacterizingCo-located Datacenter Workloads: An Alibaba Case Study, Yue Cheng, Zheng Chai,Ali Anwar. APSys2018"**

![](https://pic4.zhimg.com/v2-ee61bcc6dbbd27ba7b4734fc37c7ec5b_b.jpg)![](https://pic4.zhimg.com/v2-9af815272d0731567fcdbfb154b3e8a7_b.jpg)![](https://pic1.zhimg.com/v2-4c89f9c9a6de5a134db147e5d4f34748_b.jpg)![](https://pic1.zhimg.com/v2-8c0ffc1bab59eaba4b1f18a95a91f81c_b.jpg)![](https://pic1.zhimg.com/v2-cc0e40f20aacea05c9a3a0e63afe5084_b.jpg)

## **Cluster Data V2018 更出色**

新版本 V2018 与 V2017 存在两个最大的区别：

**DAG 信息加入**

我们加入了离线任务的 DAG 任务信息，据了解，这是目前来自实际生产环境最大的 DAG 数据。

**究竟什么是 DAG？**离线计算任务，例如 Map Reduce、Hadoop、Spark、Flink 中常用的任务，都是以有向无环图（Directed Acyclic Graph，DAG）的形式进行编排的，其中涉及到任务之间的并行、依赖等方面。下面是一个 DAG 的例子。

![](https://pic1.zhimg.com/v2-dcba757f00e71e3f810f7bdbc1cdc080_b.jpg)

**规模更大**

上一版数据包含了约 1300 台机器在约 24 小时的内容数据，而新版 Cluster Data V2018 中包括了 4000 台机器 8 天的数据。


## **获取数据方式**

访问公众号原文：[【DS】Alibaba Cluster Data 开放下载：270 GB 数据揭秘你不知道的阿里巴巴数据中心](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/_tjJW7X05kUQY3fqXFXr6Q)

点击“**阅读原文**”，完成问卷即可获取数据格式描述和数据的下载链接。加入钉钉群，与我们的团队取得联系。

> **文章来源申明：**本篇文章经公众号“阿里系统软件技术”授权转载，原文链接：[Alibaba Cluster Data 开放下载：270 GB 数据揭秘你不知道的阿里巴巴数据中心](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUzNzYxNjAzMg%3D%3D%26mid%3D2247484331%26idx%3D1%26sn%3D63c28a2e9a80b285e28170bc03f82564%26scene%3D21%23wechat_redirect)

扫二维码关注『运筹OR帷幄』公众号：
![](https://pic1.zhimg.com/v2-872dc2d2c5eaf08b38d693490549136c_b.jpg)
[点击查看『运筹OR帷幄』志愿者招募介绍及加入方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/4ganskchRA_iO1CiNtWmOA)：
![](https://pic1.zhimg.com/v2-d1fbc7022dc62969dcf8a6afdacdfcb4_b.jpg)

