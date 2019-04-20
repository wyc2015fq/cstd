# 麒麟芯片AI首席科学家，解读AI芯片如何让手机更智能 - 人工智能学家 - CSDN博客
2018年09月06日 21:15:49[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：125
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXpicCAot2qapmlGLVAhwfOqO8IEpXn9xWOo9KglskBiagwGeHpfuu5ibW5oeibUSBEy8GsuZAaronw0A/640?wx_fmt=png)
来源：华为
摘要：8月31日，华为发布了新一代顶级人工智能手机芯片——麒麟980，成为全球首款采用7nm制程工艺的手机芯片。麒麟980能做到人脸识别、物体识别、物体检测、图像分割、智能翻译等，实现AI识别质的飞跃。
今天，我们请来麒麟芯片与解决方案AI首席科学家芮祥麟博士，解读端侧AI芯片如何让手机更智能。

**AI芯片加持，从拍照说起**
有了AI的加持，现在的手机拍照有多智能呢？
它可以对物体和场景进行识别，自动选择成像从“隐性”变得更加“显性”。AI芯片自动选择相应的拍摄模式，带来更清晰的摄影效果，5秒钟就能识别100张图片。
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/koCmZQeYoD2KBUl9cEehMVeWHGHoR9CPktyKWjefSTTVFKia2PZdYW75cdcjIgVM6q8j2bUu1S8X4e1jL1B0lkw/640?wx_fmt=gif)
芮祥麟表示，AI芯片也被称为AI加速器或计算卡，专门用于处理人工智能应用中的大量计算任务的模块（其他非计算任务仍由CPU负责）。
AI芯片的计算场景可分为云端AI和终端AI。深度学习的计算场景可分为三类，分别是数据中心的训练、数据中心的推断和嵌入式设备的推断。前两者可以总结为云端的应用，后者可以概括为终端的应用。
手机AI芯片对于各种AI算子能够以30倍到50倍左右的速度处理，以拍照场景为例，能够做更好的一个图像检测，图像分割和图像语义理解。另外，对声音可以听清、听懂，并根据所了解的客户意图提供客户真正想要的服务。
2017年，华为在业内率先发布麒麟970处理器，这是华为首个人工智能端侧芯片，从硬件层面解决了在端侧运行AI模型的性能与功耗问题，这让智能终端具备了强大的运算能力。
以图像识别速度为例，麒麟970可达到约2005张每分钟，而在没有NPU的情况下每分钟只能处理97张图像，与上一代相比，图形处理性能提升20%，能效提升50%，可以更长时间支持3D大型游戏的流畅运行，支持AR/VR 等全新一代移动互联网体验。
而麒麟980在双NPU的移动端强大算力加持下，实现每分钟图像识别4500张，识别速度相比上一代提升120%，远高于业界同期水平。
**构筑平台，打智能组合拳**
芮祥麟表示，未来是云、管道、AI芯片和终端能力等四个方面整合。云侧和端侧能够做更好的协同，能够更快速地交换能力，做能力上的协调。我们必须要有这么一个开放的平台，能够把我们生态系统内所有的能力都对接进来。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/kEGEtXoR4pHFjrXlY1zIoIzdHoMTR55VgNpN9O5dDhAaCp26WFZazvIdZy2e9SBEVDvWf2NLqO5icKVssBomLSA/640?wx_fmt=png)
为了发挥华为NPU在处理AI任务时的最大性能，并且把端侧最好的AI能力开放给全球Android开发者，来构建丰富的AI应用，华为在2017年打造了HiAI开放平台。构建了三层AI开放生态：服务能力开放、应用能力开放和芯片能力开放。使第三方开发者可以便捷、实时、高效、灵活地在端侧实现AI特性，以此极大限度地降低 AI功能的端侧构建成本。
HiAI开放平台在自然语言理解（NLU），自动语音识别（ASR）和计算机视觉（CV）三大类，20+开放API和完备的工具平台，实现开箱即用，助力三方开发者快速丰富其产品的能力与应用场景，让AI触手可及。
目前，有一些合作伙伴已“享受”到了通过HiAI平台的赋能与NPU计算力的加 持所带来产品能力与体验的显著提升。
如微软翻译，通过HiAI平台的加速，离线图片以及文本翻译的速度端到端整体提升了3倍。
在计算机视觉方面，性能的提升尤为突出，如在Prisma上，一张照片的风格转换的时间大抵10秒左右，通过HiAI平台加速4倍，仅需3秒左右处理完成。
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/koCmZQeYoD3SGLAUv2uW5tGWQWaUZFkDFKOibictx8W7A2grVWibT85aNpH2ESgZ9xNC2PCictXCOpzU7HzUXKw9TQ/640?wx_fmt=gif)
同时，还能应用HiAI开放的场景识别能力，智能推荐当前照片适合的风格滤镜，用户无需在动辄几十上百个的滤镜中去选择，进一步提升用户的转换效率。
能效比上也表现不俗，快手用户在终端设备上直播，超过5分钟，有一些终端就会发热，壳温超过36度，NPU的能效比比CPU高50倍，通过HiAI平台优化，长时间直播毫无压力。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
