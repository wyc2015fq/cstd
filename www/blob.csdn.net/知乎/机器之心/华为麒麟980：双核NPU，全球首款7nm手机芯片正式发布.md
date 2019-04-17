# 华为麒麟980：双核NPU，全球首款7nm手机芯片正式发布 - 知乎
# 



**机器之心报道，作者：李泽南。**

> 去年，华为发布的全球首款 AI 手机芯片麒麟 970 让世人惊艳。刚刚，在德国 IFA 2018 展会上，余承东发布了它的继任者：第二代 AI 芯片麒麟 980。这款芯片采用 7nm 工艺，搭载业内首款双核 NPU，将旗舰手机 CPU 的水平再次提升到了一个新高度。（注：文中图片大多来自「华为终端公司」微博账号）

在发布会上，华为表示：作为全球第一块消费级 7nm 制程工艺芯片，麒麟 980 相较前代 10nm 的工艺在性能上提升 20%，能效提升 40%，晶体管密度提升了 1.6 倍。

此外，华为还宣称麒麟 980 实现了几项世界第一：
- 全球最早商用 7nm 工艺的手机 SoC 芯片
- 全球首款 ARM Cortex-A76 商用 CPU 的芯片
- 全球首款搭载双核 NPU 的手机芯片
- 最先采用 Mali-G76 GPU 的手机芯片
- 全球最先支持 LTE Cat.21 的芯片，峰值下载速率 1.4Gbps，达到业内最高
- 支持全球最快的 LPDDR4X 内存颗粒，主频可达 2133MHz，业内最高

有关麒麟 980 的发布消息，这几个月来一直传闻不断，但其中最具可信度的还是荣耀在 8 月 30 日的「官方泄露」。在 IFA 2018 大会开始前一天，荣耀手机就在柏林举办了一场发布会，荣耀 CEO 赵明公布了采用新一代的 AI 芯片的第一款手机——荣耀 Magic2。

而关于麒麟 980 具体规格的更多细节，余承东在刚刚的 Keynote 中为我们一一揭晓。
![](https://pic1.zhimg.com/v2-dac4acfc6476bc8170c9746abcfb1888_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)麒麟 980 芯片
首先，上台后的余承东回顾了去年的 IFA，以及去年发布的麒麟 970，这是华为迈向设备上 AI 的第一步。一年后，大众所期待的麒麟 980 最终发布。从下图来看，华为表示麒麟 980 是当前最强大、最智能的手机端 AI 芯片。
![](https://pic1.zhimg.com/v2-805bf0440756b8c9dc6cdc8385e18cf0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='532'></svg>)



**2+2+4 的 CPU 形式**

麒麟 980 的 CPU 核心部分采用了新一代的 ARM CPU 架构 Cortex-A76，ARM 曾表示，相比于采用 10nm 制程的 Cortex-A75（骁龙 845 搭载的内核型号），采用 7nm 制程的 A76 性能提升 35%、能效提升 40%，机器学习速度则可以提升四倍。

本次华为与众不同地采用了 2+2+4 的大中小核形式，其中两个频率为 2.6GHz 的 A76 大核负责高负载任务，两个频率 1.92GHz 的 A76「中核」负责日常任务。四个 A76 大核之外，还有四个 A55 小核（1.8GHz）负责轻度运算。值得一提的是，相比前几代的公版设计，麒麟 980 的 CPU 部分采用了半定制架构，可见华为海思已经在充分利用 ARM 的新 DSU 集群以及异步 CPU 配置了。

可以看到，性能最高的 Cortex-A76 核心速度（2.6GHz）并未被设定为 ARM 公布的最高数字 3GHz，华为的保守设置或许是考虑到了功耗的限制。中等核心或许可以有效降低 SoC 整体功耗——此前，四个大核运行在相同的时钟和电压上，如果存在一个高性能线程，其他中等性能线程只能被迫作不必要的运算，降低了效率。
![](https://pic4.zhimg.com/v2-79a52cc68cb39b291369d2545d7a54e7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='546'></svg>)
而相比于去年发布的麒麟 970，采用了 Cortex-A76 架构之后，麒麟 980 性能有 75% 的提升，能效有 58% 的提升。
![](https://pic1.zhimg.com/v2-254a34fa09cd5efb4c7c175cc6c8ec9c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='569'></svg>)



**补齐 GPU 短板**

在 GPU 方面，这次麒麟 980 采用了 Mali G76，这是 ARM 重新设计架构后的新款高端 GPU，它有 4 到 20 个 shader core，其纹理单元、渲染单元、计算单元管道位宽均大幅增加，为复杂图形和机器学习的工作负载提供显著的提升。华为宣称，新 GPU 相比前代性能提高了 76%，这可以说弥补了华为一直以来 GPU 的短板。

即使是在纸面上，G76MP10 看起来要比麒麟 970 采用的 G72MP12 小，但它实际的计算资源增加了 66％——这还是在没有考虑新 IP 的微架构改进的情况下。

麒麟 980 的 GPU 工作频率为 720MHz，相较 970 的 747MHz 要慢。这种变化让新款产品能耗更低，但这并不影响新一代 GPU 性能的大幅提升。
![](https://pic4.zhimg.com/v2-b73ad7d50e566d92af859dff01a607db_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)华为对比了麒麟 980 与骁龙 845 的游戏帧率和能耗效率。
在网络方面，麒麟 980 率先支持 LTE Cat.21，峰值下载速率 1.4Gbps，但遗憾的是仍然不支持下一代 5G 网络。同时，这款芯片也支持 2133MHz LPDDR4X 的内存。
![](https://pic3.zhimg.com/v2-def11500c78514932cc30d71a9ca6d8a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='498'></svg>)
**双核 NPU**

去年，华为发布麒麟 970 的时候首次为手机端 SoC 引入了神经网络加速芯片 NPU。在麒麟 980 中，华为再次引入了业内首款双核 NPU，NPU 核心采用了寒武纪 1H，实现每分钟图像识别 4500 张，支持人脸识别、物体识别、物体检测等 AI 场景。
![](https://pic2.zhimg.com/v2-0ebf5bac95b8341ff13bc7a047240e1d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='556'></svg>)
麒麟 980 上的神经网络仍然一次只能处理一个深度学习负载，不过理论运行速度加倍了。余承东表示，新的 NPU 处理单元速度要比麒麟 970 上的快 2.2 倍。

下图对比了麒麟 980 与骁龙 845、苹果 A11 在图像识别性能上的表现。
![](https://pic4.zhimg.com/v2-e6228b82dcaf9518d6477039c249158f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='678' height='339'></svg>)
在这一切之下，我们看到了来自台积电 7nm 制程带来的提升：新款芯片在不到 100m2 的面积上堆积了 69 亿个晶体管，相比去年麒麟 970 的 55 亿个增长了 25%。

人类在芯片制程上的推进已经逐渐变得困难，台积电于 2015 年开始研发 7nm 工艺，直至今年夏天才得以正式投产。目前全球掌握 7nm 工艺的厂商只有三家：英特尔、三星和台积电。

麒麟 980 凭以上特性，打破了多个世界第一。
![](https://pic4.zhimg.com/v2-eefed7e22c470666f09163dde4b683df_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='533'></svg>)
在展示了新产品诸多性能优势后，华为还不忘把麒麟 980 和骁龙 845 做了一个全面对比。

海外媒体 AnandTech 也于第一时间整理了麒麟 980 的主要性能参数：
![](https://pic1.zhimg.com/v2-ce33d220567ce5007f44b8c6cf269174_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='727' height='885'></svg>)
除了昨天已经发布的荣耀 Magic2 以外，余承东也宣布，麒麟 980 将搭载于华为即将发布的旗舰手机 Mate20 系列中——Mate 20 即将于今年 10 月 16 日在英国伦敦发布。由于昨天赵明曾表示其发布的荣耀 Magic2 可能还不是最终版本，因此目前看来 Mate 20 仍然会是麒麟 980 的首发手机。

一个多月之后，我们就能感受到新世代 CPU 处理器的强大动力了。
![](https://pic3.zhimg.com/v2-47d4629662fb4234a820455d522c52ba_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='533'></svg>)
**延伸阅读：**
- [机器之心独家解读：华为首款手机端 AI 芯片麒麟 970](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650730488%26idx%3D1%26sn%3D4805c527738aca30a826ef00398c411c%26chksm%3D871b2b86b06ca29048844942820101a3497453db75048d6fc624920406cdef0fd9d2b0e720a7%26scene%3D21%23wechat_redirect)
- [华为麒麟的 AI 性能是高通的 3.5 倍？这是所有手机运行神经网络的能力](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650745762%26idx%3D1%26sn%3D7cf0f323b5f2253185fc9127aaef26f3%26chksm%3D871aefdcb06d66cadf121c5e62c4cb1980103c0cb55140bfb484cd1ef64fc9811abff3bfef95%26scene%3D21%23wechat_redirect)


