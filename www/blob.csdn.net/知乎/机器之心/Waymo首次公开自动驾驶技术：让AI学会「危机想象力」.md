# Waymo首次公开自动驾驶技术：让AI学会「危机想象力」 - 知乎
# 



机器之心报道，机器之心编辑部。

> Waymo 的[自动驾驶出租车「Waymo One」已经上线](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzUyODA3MDUwMA%3D%3D%26mid%3D2247492201%26idx%3D1%26sn%3Dd2b41b1ab1882d671119e65465068773%26chksm%3Dfa774062cd00c974785ae35f5d2a11034b3a0ac6174dd69fc469dab93f27013d6afb82f2fdb3%26scene%3D21%23wechat_redirect)了，现在美国亚利桑那州凤凰城的居民可以像叫个滴滴一样用手机 app 打来一辆自动驾驶汽车。这些高度智能化的无人车使用的是哪些技术？最近这家谷歌旗下的公司终于公开了自己的第一篇论文，介绍了 Waymo 自动驾驶算法的细节。

今年，自动驾驶公司 Waymo 取得了一些里程碑式的进展。8 月份，Waymo 的实地自动驾驶里程突破 1000 万英里，上周，Waymo 又推出了一款商用自动驾驶车——Waymo One。但该公司的研究人员始终着眼于未来。

Medium 上今天出现了一篇博文，Mayank Bansal、Abhijit Ogale 两位研究人员详细介绍了利用标注数据（即来自专业驾驶演示的 Waymo 数百万英里的已标注数据）通过监督学习训练 AI 驾驶员的方法。




**学习人类经验**

「近年来，深度神经网络中的监督训练使用大量标注数据在许多领域迅速提高了当前最佳水准，尤其是在目标检测及预测领域。这些技术在 Waymo 得到了广泛应用，」研究人员写道。「随着感知神经网络取得成功，我们自然而然地会问自己：我们可以利用纯监督深度学习方法训练出一个熟练的自动驾驶员吗？」
![](https://pic3.zhimg.com/v2-efdb63aeef650e55618012264775f2c2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='400'></svg>)
*模拟环境中，Waymo 的智能体绕过一辆停泊的车。青色路径描绘了输入路径，绿色框是智能体，蓝色点是智能体的过去位置，绿色点是预测的未来位置。*
![](https://pic2.zhimg.com/v2-4123741b99207646b5fd0a39ffebf7ed_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='200' height='200'></svg>)
*模拟环境中，Waymo 的 AI 系统根据前方车辆位置减速停车。带有褪色尾部的黄色方块是场景中过去一秒内的动态目标（其它车辆）的位置。*

为了创建一个能够模拟专业驾驶的系统，他们精心设计了一个神经网络——ChauffeurNet，该网络通过观察真实和模拟数据（包括地图、周围目标、交通灯状态和汽车过去的运动）学会了生成驾驶轨迹。一个低级的控制器将十点轨迹转化为转向和加速指令，允许 AI 模型驾驶真实和模拟汽车。

该模型从「相当于专业驾驶 60 天的驾驶数据」中得到样本，其使用的技术确保了它不会根据过去的运动推断下一步动作，而是根据环境中的变化做出反应。在测试中，它会对交通控制标识做出反应，如停车标识和交通灯。但遇到以前从未见过的情况时，它的表现不佳。

**模拟异常情况**

研究人员指出，造成这种问题的原因在于其从现实驾驶中获得的驾驶 demo 是有偏差的——它们只包含正常情况（无事故）下的驾驶案例。为了教网络从边缘案例中恢复，该团队合成了接近事故和与物体发生碰撞的情况，后者鼓励 AI 模型避开这些物体。
![](https://pic4.zhimg.com/v2-036ee388b620f25c0f9b9773b3dbfa23_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='281'></svg>)
*Waymo 的模型在转弯时的表现。图源：Waymo*

ChauffeurNet 在模拟环境中表现较好，它将损失和合成的示例都考虑在内，甚至还能绕过停泊的车辆，在信号灯由黄变红时停下以及在其轨道上修复些微的偏差。在现实世界的私人试车跑道驾驶 Waymo 的克莱斯勒 Pacifica 小型货车时，ChauffeurNet 成功地驶过了弯道，并正确判别了停止信号和转弯。

「全自动驾驶系统需要具备处理现实世界种种状况的能力，」研究人员写道。「在 Waymo 车辆上运行的现有规划器将机器学习与外显推理相结合，连续不断地评估大量概率变量并在多种不同的情境下做出最佳驾驶决策……因此，用完全的机器学习系统替代 Waymo 规划器的门槛很高，尽管这一系统的组成也可以用在 Waymo 规划器内，或者用于在规划器的模拟测试期间创造更加实用的『智慧智能体』」。




**模型在真实世界驾驶的情况**
![](https://pic2.zhimg.com/v2-e903378cf31c9e96ff881d9711dfb0b5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='245' height='245'></svg>)
*红色轨迹表示场景中动态目标的过去轨迹，绿色轨迹表示每个目标未来 2 秒之内的预测轨迹。*
[Stop Sign & Turn_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/e0811n664eh.html)
**模型结构**
![](https://pic2.zhimg.com/v2-e9e2530a84cc25be692dc10a7671273d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='546'></svg>)
ChauffeurNet 内部由两个部分组成——FeatureNet 和 AgentRNN。AgentRNN 的输入包括一张展示过去智能体姿态的图像、一组由卷积网络FeatureNet 从展示的输入中计算的特征、一张展示过去智能体框的图像、展示预测未来智能体姿态以预测智能体下一个姿态的外显记忆，以及俯视视角下的下一个智能体框。这些预测用于更新 AgentRNN 的输入，从而预测下一个时间步。

最后，让我们看看这篇介绍 ChauffeurNet 的论文：《ChauffeurNet: Learning to Drive by Imitating the Best and Synthesizing the Worst》。
![](https://pic2.zhimg.com/v2-9095b5ba4947e96be5981b7363dde499_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='589'></svg>)
论文链接：[https://sites.google.com/view/waymo-learn-to-drive](https://link.zhihu.com/?target=https%3A//sites.google.com/view/waymo-learn-to-drive)

**摘要：**我们的目标是通过模仿学习来为自动驾驶训练可用于驾驶真实汽车的鲁棒策略。我们发现通常的行为复制不足以适应复杂的驾驶环境，即使已经使用了感知系统来预处理输入，以及使用控制器来将输出执行到汽车上：3 千万个案例仍然是不够的。我们在本研究中提出，应该以专业驾驶数据的扰动形式向学习器展示合成数据，这会产生有趣的情况，例如碰撞和/或离开道路。我们不是纯粹模仿所有数据，而是通过额外的损失补充模仿损失，以惩罚不良事件并鼓励进步，扰动为这些损失函数提供了重要信号，并实现了学习模型的鲁棒性。结果表明，ChauffeurNet 模型可以处理模拟中的复杂情况，我们还提供了控制变量实验，强调我们提出的每个变化的重要性，并表明模型可以对适当的因果因素进行响应。最后，我们展示了在现实世界中驾驶汽车的模型。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
参考内容：
- [https://medium.com/waymo/learning-to-drive-beyond-pure-imitation-465499f8bcb2](https://link.zhihu.com/?target=https%3A//medium.com/waymo/learning-to-drive-beyond-pure-imitation-465499f8bcb2)
- [https://venturebeat.com/2018/12/10/waymo-tests-ai-driving-system-that-learns-from-labeled-data/](https://link.zhihu.com/?target=https%3A//venturebeat.com/2018/12/10/waymo-tests-ai-driving-system-that-learns-from-labeled-data/)
- [https://sites.google.com/view/waymo-learn-to-drive](https://link.zhihu.com/?target=https%3A//sites.google.com/view/waymo-learn-to-drive)






