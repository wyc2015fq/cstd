# AI算力需求6年增长30万倍、3.5月翻一番——OpenAI数据分析 - 知乎
# 



选自OpenAI，作者：DARIO AMODEI、DANNY HERNANDEZ，机器之心编译。

> 人工智能技术近年来的发展不仅仰仗于大数据，更是计算机芯片算力不断增强的结果。然而，如果 AI 算力需求的增长速度大大超过了芯片算力的进步，我们又该怎么办？这种担心似乎正在成为现实。根据 OpenAI 最新的分析，近年来人工智能训练任务所需求的算力每 3.43 个月就会翻倍，这一数字大大超越了芯片产业长期存在的摩尔定律（每 18 个月芯片的性能翻一倍）。

OpenAI 近日发布的分析表明，自 2012 年以来，人工智能训练任务中使用的算力正呈指数级增长，其目前速度为每 3.5 个月翻一倍（相比之下，摩尔定律是每 18 个月翻倍）。自 2012 年以来，人们对于算力的需求增长了超过 300,000 倍（而如果是以摩尔定律的速度，只会有 12 倍的增长）。在此期间，硬件算力的提升一直是人工智能快速发展的重要因素。因此，如果我们希望目前的发展趋势持续下去，我们就需要为实现远超当前方法负载的全新系统做好准备。
![](https://pic4.zhimg.com/v2-2f673029b9d6b5dc9fd9098e6d2cabaf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='846'></svg>)![](https://pic3.zhimg.com/v2-0f5b404d681705128d428fa40bd2a6e2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='846'></svg>)上图中的计算方法参见下文附录
上表显示了不同模型以 petaflop/s-days 计的计算总量。深度模型需要耗费大量时间和算力，因此若给定足够信息，我们可以估计已知训练结果的总算力需求。一个 petaflop/s-day（pfs-day）代表在一天时间内每秒执行 10^15 次神经网络操作，总计约为 10^20 次操作（operations）。这种计法类似于电能的千瓦时。在这里，我们不测量硬件 FLOPS 数的理论峰值，而是尝试估计执行的实际操作数量。OpenAI 在这里将任何加法或乘法计为单个操作，而不考虑数值精度（使「FLOP」略微用词不当），同时忽略集成模型。通过计算，每次算力需求翻倍的时间为 3.43 个月。




**概论**

推动人工智能发展的动力有三个：算法创新、数据（其可以是有监督数据或者交互式环境），以及可以用于训练的算力。算法创新和大数据体量或许难以跟踪，但算力通常是可以量化的，这为我们探究 AI 发展进程提供了机会。显然，使用大量算力有时候只会暴露人类当前算法低效的现实，但至少目前在很多领域中，使用更多的算力通常会带来更为优秀的结果，而且这通常也与算法的进步相辅相成。

在算力的分析中，OpenAI 相信起决定作用的数字并不是单个 CPU 的速度，也不是数据中心的最大容量，而是用于训练单个模型所需的算力数字——这一数字最有可能代表当前最佳算法的强大程度。以模型计的算力需求与总算力有很大不同，因为并行计算的限制（硬件和算法上）定义了模型不能过大，训练的效率也不会太高。当然，重要的技术突破仍然是在适量的计算上实现的——在这里我们仅分析算力需求。

OpenAI 发现，目前发展的趋势是每年大约增加 10 倍。这一过程部分是由更为专业的硬件（如 GPU 和 TPU）使得给定价格下芯片每秒能够执行更多操作，但主要还是由研究人员一再推动寻找和使用并行方法，花费大量资金来实现的。




**时期**

从图中我们可以看到四个时期：
- 2012 年之前：使用 GPU 研究 ML 还不常见，图中的任何结果都很难达到。
- 2012-2014：在多个 GPU 上进行训练的基础架构不常见，因此大多数结果使用 1-8 个速度为 1-2 TFLOPS 的 GPU 进行训练，得到 0.001-0.1 pfs-days 的结果。
- 2014-2016：使用 10-100 个速度为 5-10 TFLOPS 的 GPU 进行大规模训练，得到 0.1-10 pfs-days 的结果。减少数据并行化的返回值意味着更大的训练运行产生的值是有限的。
- 2016-2017：允许更大算法并行的方法（如较大的批量大小、架构搜索和专家迭代）以及专用硬件（如 TPU 和更快的互联）极大地突破了限制，尤其是对某些应用来说。

AlphaGoZero/AlphaZero 是公众可见的大型算法并行化例子，而很多其他同等规模的应用现在在算法层面上是可行的，而且可能已经用于生产。




**展望**

有很多理由让我们相信图中展示的趋势将会继续。很多硬件创业公司正在开发 AI 专用的芯片，一些企业声称他们将在接下来一两年大幅提高芯片的 FLPOS/Watt（和 FLOPS/$相关）。此外，也可以仅通过重配置硬件，以更少的经济成本得到相同的计算次数。在并行性方面，上述的很多近期的算法创新在原则上可以相乘地结合，例如，架构搜索和大规模并行 SGD。

另一方面，成本最终会限制该趋势的并行性方面，物理学将会限制芯片效率方面。OpenAI 认为如今最大规模的训练运行使用的硬件在个位数数量上就要以百万计美元的成本来采购（尽管已摊销成本低得多）。但今天的神经网络计算的主体仍然在于推理（部署）阶段，而不是训练过程，这意味着企业可以重新利用或采购更大规模的芯片机群用于训练。因此，如果出现了足够的经济诱因，我们甚至可以见证更大规模的平行训练，为此该趋势将继续保持多年。全世界的总体硬件预算是每年 1 万亿美元，因此绝对的限制仍然远未达到。总体而言，给定上面的数据，计算的指数增长趋势的先例、在机器学习专用的硬件上运行，以及正在发挥作用的经济诱因，OpenAI 认为那些相信这个趋势在短期内将消失的看法是错误的。

过去的趋势并不足以预测该趋势将在未来持续多久，或当它继续时会发生什么。但容量的快速增长的合理潜力意味着开始解决安全（[https://blog.openai.com/concrete-ai-safety-problems/](https://link.zhihu.com/?target=https%3A//blog.openai.com/concrete-ai-safety-problems/)）和 [AI 的恶意](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650738149%26idx%3D4%26sn%3D0d02fc17c1e8eb96b15ecf5709223e21%26chksm%3D871ac99bb06d408d341623b62e17978d38b5d405202fb4305de35e7ec8b966513d3b677f9026%26scene%3D21%23wechat_redirect)使用成为重要的问题，我们必须抢在该趋势之前率先行动，而不是延迟地反应。




**附录：方法**

OpenAI 使用了两种方法来生成这些数据点。当我们有足够的信息时，可以直接计算各种架构在每个训练样本所花费的 FLOPs 数（加和乘运算数），并乘以训练过程中前向传播与反向传播的总数量。当我们没有足够的信息直接计算 FLOPs 数时，可以查看 GPU 训练时间与 GPU 的使用数量，并假设一个使用效率（通常为 0.33）。对于大多数论文，我们可以直接使用第一种方法，不过仍然有少数模型需要使用第二种方法计算，OpenAI 尽可能使用两种方法计算以作为一致性检查。此外，OpenAI 也会将大多数计算结果送与原作者确定。计算并不是非常精确，但 OpenAI 旨在 2-3 倍的范围内保持准确，以下提供了一些计算示例。




**方法 1 示例：计算模型中的操作数**

这种方法在作者给出了前向传播中的操作数（operations）时非常容易处理，例如在论文 Going deeper with convolutions 中构建的 ResNet-151：


```
(add-multiplies per forward pass) * (2 FLOPs/add-multiply) * (3 for forward and backward pass) * (number of examples in dataset) * (number of epochs)
= (11.4 * 10^9) * 2 * 3 * (1.2 * 10^6 images) * 128
= 10,000 PF = 0.117 pfs-days
```


已知模型架构的操作数可以通过一些深度学习框架程式地计算出来，或者我们也可以简单地手动计算操作数。如果论文给出了足够的信息以计算操作数，那么方法 1 会非常精确. 但有些时候论文并没有包含所有的必要信息，且作者也不能公开地展示详细细节，这样的计算就会比较麻烦。




**方法 2 示例：GPU 时间**

如果我们不能直接计算操作数，那么就要查看模型需要多少 GPU 训练多长时间，然后再根据对 GPU 利用率的合理假设估计执行的操作数量。需要强调的是，我们不能只计算 FLOPs 的理论峰值，相反需要使用理论 FLOPs 数的假定分数来尝试预测实际的 FLOPs 数。根据 OpenAI 的实验经验，除非有更加具体的信息，他们一般假设 GPU 的利用率为 33%、CPU 的利用率为 17%。

例如在 AlexNet 论文中，原文表示「我们的网络在两块 GTX 580 3GB 显存的 GPU 上需要花 5 到 6 天进行训练。」根据 OpenAI 的假设，那么它的总计算数可计算为：


```
Number of GPUs * (peta-flops/GTX580) * days trained * estimated utilization
= 2 * (1.58 * 10 ^ -3 PF) * 5.5 * 0.33
= 500 PF = 0.0058 pfs-days
```


这个方法只是一种近似计算，并且很容易超出实际的上下两倍。因此，OpenAI 的目标是仅估计模型操作数的数量级。在实践中，这两种方法都可以用且结果也比较相似，例如直接计算 AlexNet 的操作数将得出 0.0054 pfs-days，而通过 GPU 执行时间估计可得出 0.0058 pfs-days。


```
1.2M images * 90 epochs * 0.75 GFLOPS * (2 add-multiply) * (3 backward pass) 
= 470 PF = 0.0054 pfs-days
```





**选定的附加计算**

dropout


```
Method 2:

1 GPU * 4 days * 1.54 TFLOPS/GTX 580 * 0.33 utilization 
= 184 PF = 0.0021 pfs-days
```


可视化和理解卷积神经网络


```
Method 2:

1 GPU * 12 days * 1.54 TFLOPS/GTX 580 * 0.33 utilization 
= 532 PF = 0.0062 pfs-days
```





DQN


```
Method 1:

Network is 84x84x3 input, 16, 8x8, stride 4, 32 4x4 stride 2, 256 fully connected
First layer: 20*20*3*16*8*8 = 1.23M add-multiplies
Second layer: 9*9*16*32*4*4 = 0.66M add-multiplies
Third layer: 9*9*32*256 = 0.66M add-mutliplies
Total ~ 2.55M add-multiplies
2.5 MFLOPs * 5M updates * 32 batch size * 2 multiply-add * 3 backward pass
= 2.3 PF = 2.7e-5 pfs-days
```





Seq2Seq


```
Method 1:

(348M + 304M) words * 0.380 GF * 2 add-multiply * 3 backprop * 7.5 epoch
= 7,300 PF = 0.085 pfs-days
```






```
Method 2:

10 days * 8 GPU’s * 3.5 TFLOPS/ K20 GPU * 0.33 utilization 
= 8,100 PF = 0.093 pfs-days
```





VGG


```
Method 1:

1.2 M images * 74 epochs * 16 GFLOPS * 2 add-multiply * 3 backward pass 
= 8524 PF = 0.098 pfs-days
```






```
Method 2:

4 Titan Black GPU’s * 15 days * 5.1 TFLOPS/GPU * 0.33 utilization 
= 10,000 PF = 0.12 pfs-days
```





DeepSpeech2


```
Method 1:

1 timestep = (1280 hidden units)^2 * (7 RNN layers * 4 matrices for bidirectional + 2 DNN layers) * (2 for doubling parameters from 36M to 72M) = 98 MFLOPs
20 epochs * 12,000 hours * 3600 seconds/hour * 50 samples/sec * 98 MFLOPs * 3 add-multiply * 2 backprop 
= 26,000 PF = 0.30 pfs-days
```






```
Method 2:

16 TitanX GPU’s * 5 days * 6 TFLOPS/GPU * 0.50 utilization 
= 21,000 PF = 0.25 pfs-days
```





Xception


```
Method 2:

60 K80 GPU’s * 30 days * 8.5 TFLOPS/GPU * 0.33 utilization 
= 4.5e5 PF = 5.0 pfs-days
```





神经架构搜索


```
Method 1:

50 epochs * 50,000 images * 10.0 GFLOPSs * 12800 networks * 2 add-multiply * 3 backward pass 
= 1.9e6 PF = 22 pfs-days
```









```
Method 2:

800 K40’s * 28 days * 4.2 TFLOPS/GPU * 0.33 utilization 
= 2.8e6 PF = 31 pfs-days
Details given in a [later paper](https://arxiv.org/pdf/1707.07012.pdf).
```





神经机器翻译


```
Method 2:

sqrt(10 * 100) factor added because production model used 2-3 orders of magnitude more data, but only 1 epoch rather than 10.
96 K80 GPU’s * 9 days * 8.5 TFLOPS * 0.33 utilization * sqrt(10 * 100)  
= 6.9e6 PF = 79 pfs-days
```





**附录：使用适当计算量得到的近期新结果**

产生重要的结果不一定需要大量的计算。很多近期值得注意的结果仅使用了适当的计算量。以下是一些相关的案例，它们能为计算评估给出足够的信息。OpenAI 并没有使用多种方法来评估这些模型的计算，对于上边界他们对可能的丢失信息进行的保守估计，因此它们会有更多的总不确定度。这些结果并不包含在 OpenAI 的定量分析中，但它们仍然是有趣和值得分享的：
- Attention is all you need: 0.089 pfs-days (6/2017)（[https://arxiv.org/abs/1706.03762](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1706.03762)）
- Adam Optimizer: less than 0.0007 pfs-days (12/2014)（[https://arxiv.org/abs/1412.6980](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1412.6980)）
- Learning to Align and Translate: 0.018 pfs-days (09/2014)（[https://arxiv.org/abs/1409.0473](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1409.0473)）
- GANs: less than 0.006 pfs-days (6/2014)（[https://arxiv.org/abs/1406.2661](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1406.2661)）
- Word2Vec: less than 0.00045 pfs-days (10/2013)（[https://arxiv.org/abs/1310.4546](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1310.4546)）
- Variational Auto Encoders: less than 0.0000055 pfs-days (12/2013)（[https://arxiv.org/abs/1312.6114](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1312.6114)）

原文链接：[https://blog.openai.com/ai-and-compute/](https://link.zhihu.com/?target=https%3A//blog.openai.com/ai-and-compute/)


