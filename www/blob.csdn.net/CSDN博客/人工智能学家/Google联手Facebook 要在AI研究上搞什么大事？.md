# Google联手Facebook 要在AI研究上搞什么大事？ - 人工智能学家 - CSDN博客
2018年10月04日 23:26:08[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：61
![640?wx_fmt=jpegwebp](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/00yGoQJ8PHdMxoo0syy1ulrgebNw9Yt1JNcoC1azUOfcyTkuepD29mhGK58Lr7VK6Iu98xCkVfHick6YozpQEXA/640?wx_fmt=jpegwebp)
来源：网易智能 
摘要：Google和Facebook宣布，使开源机器学习框架PyTorch与Tensor-Processing Units（TPU）进行合作。这种伙伴关系标志着人工智能研究合作进入新时代。
“今天，我们很高兴地宣布，Google TPU团队的工程师正积极与PyTorch核心开发人员合作，将PyTorch与Cloud TPU连接起来。长期目标是让每个人都能享受PyTorch的简便性和灵活性，同时能够从Cloud TPU的性能、可扩展性和成本效益上获益。”Google产品总监Rajen Sheth说道。
PyTorch是Facebook的开源框架，可以开发人工智能研究中使用的数学程序。这样的框架允许研究人员开发任意复杂的数学计算图并自动计算衍生产物。
TPU是由Google专门为AI系统设计的计算机芯片。据Google描述，TPU比传统的图形处理单元（GPU）快15到30倍。
![640?wx_fmt=jpegwebp](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/00yGoQJ8PHdMxoo0syy1ulrgebNw9Yt1YOK7nm0j3U6BZCYGLtTR5xC9ePCwYYHDPIkE2gUKJvOF1cPMibcwj5w/640?wx_fmt=jpegwebp)
**PyTorch有什么新功能？**
近日，Facebook发布的深度学习框架 PyTorch 1.0主要有三大更新：
一是添加了一个新的混合前端，支持从 Eager 模式到图形模式的跟踪和脚本模型，以弥合研究和生产部署之间的差距。
二是一个经过改进的 Torch 分布式库，可以在 Python 和 C++环境中实现更快的训练。
三是添加了针对关键性能研究的 Eager 模式 C++接口，将在测试版中发布。
目前，研究人员和工程师必须面对许多框架和工具，以创建新的深度学习模型并将其转移到生产环境中大规模运运行，而这里多数框架和工具通常是不兼容的。如此，将会降低开发者在规模化生产中部署 AI 能力的速度。通过这个最新版本，将现有 PyTorch 框架的灵活性与 Caffe2的生产能力结合，提供从研究到生产性 AI 的无缝路径。
**Google的TPU有什么优势？**
虽然 TPU 是一种专用集成电路，但它运行的程序来自 TensorFlow 框架下的神经网络，驱动了谷歌数据中心的许多重要应用，包括图像识别、翻译、搜索和游戏。通过专门为神经网络重新分配芯片计算资源，TPU 在真实数据中心负载环境下效率要比通用类型的计算机高30-80 倍，目前已为全球 10 亿人提供日常服务。
另外，神经网络的推理阶段通常会有严格的响应时间要求，这降低了通用计算机所使用技术的有效性；通用计算机通常运行得较快，但某些情况下也会较慢。
![640?wx_fmt=jpegwebp](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/00yGoQJ8PHdMxoo0syy1ulrgebNw9Yt1JNcoC1azUOfcyTkuepD29mhGK58Lr7VK6Iu98xCkVfHick6YozpQEXA/640?wx_fmt=jpegwebp)
**为什么Pytorch与TPU建立连接很重要**
将GPU上的大量数据与神经网络训练结合是当前深度学习系统成功的催化剂。可能需要花费数月训练的神经网络，在使用GPU训练时，时间缩短到短短的几个小时。随着深度学习的成熟，神经网络和数据集变得越来越大，这些网络现在需要数月才能在GPU上进行训练。
谷歌专有的TPU为这些庞大的系统提供了一种更快速的训练方法。更快的训练意味着研究人员可以更快地进行实验，从而提高AI研究的速度。
**为什么这种伙伴关系有利于人工智能研究**
传统上，Google和Facebook通过Google Deepmind，Google Brain和Facebook AI Research进行了独立的AI研究。因此，人工智能工具生态系统已经在Tensorflow（谷歌的AI框架）与Pytorch的辩论中分立，虽然竞争使得两个框架都以极快的速度发展，但其也使得研究在可重复性方面困难重重。
如果此公告标志着一种更加协作的AI研究方法，我们可能会看到这两个框架之间的互操作性得到改善。此外，这样的结果使得AI在智能手机上的部署更容易，也便于多种框架建构统一的工具生态系统，并改善研究结果的可重复性。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
