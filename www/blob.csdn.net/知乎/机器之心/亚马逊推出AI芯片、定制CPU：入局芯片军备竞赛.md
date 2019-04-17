# 亚马逊推出AI芯片、定制CPU：入局芯片军备竞赛 - 知乎
# 



机器之心报道，参与：李泽南、刘晓坤。

> 与谷歌、百度、阿里这样的科技巨头一样，亚马逊希望掌握自身业务的全部。在昨天拉斯维加斯举行的 AWS re:Invent 大会上，这家公司发布了一款名为 Inferentia 的机器学习芯片。
![](https://pic4.zhimg.com/v2-d566a8bd6e9601fb8de55adaecdb3ab7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='577' height='328'></svg>)
「Inferentia 将会是一款超高吞吐量、低延迟、性能强大，且功耗比极佳的处理器，」AWS 首席执行官 Andy Jassy 在发布中介绍道。

对于亚马逊来说，这或许是必然要走的一步。来自 Constellation Research 的分析师 Holger Mueller 对此表示，亚马逊在自研芯片上远远落后于其他巨头，但这一步颇具意义，在未来这家公司或许会尝试不同的机器学习方法。

Inferentia 支持 INT8、FP16 等流行框架以及混合精度。此外，它也支持 TensorFlow、Caffe2 和 ONNX 等机器学习框架。

当然，作为亚马逊的产品，它也支持处理 AWS 产品中的数据，例如 EC2、SageMaker，以及今天公布的 Elastic 推理引擎。这款产品专为推理而设计，这方面与专为训练机器学习模型设计的谷歌 TPU 不同。Jassy 表示，目前为机器学习计算设计加速器的芯片公司——如其中最大的英伟达——已经投入巨大精力对于模型训练进行优化，这就是 AWS 决定专注于设计更好的推理芯片的原因。
![](https://pic3.zhimg.com/v2-62118a611b05bee1525065f2e1702566_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='480'></svg>)
亚马逊称，若在已训练模型的基础上进行推理任务，Inferentia 芯片可以担负 90% 的算力。通过使用 Amazon Elastic Inference，开发者们可以通过将 GPU 驱动的推理加速功能附加到 Amazon EC2 和 Amazon SageMaker 实例上，将推理成本降低多至 75％左右。
![](https://pic1.zhimg.com/v2-68d3ec2290d3cef3e828f7c7b2f2b068_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='646'></svg>)
*亚马逊 Inferentia 机器学习芯片的原型，这款芯片由 Annapurna Labs 设计。图片来自亚马逊副总裁、杰出工程师 James Hamilton。*




与亚马逊此前的定制 Arm 处理器 Graviton 一样，Inferentia 是这家公司在 2015 年收购的以色列创业公司 Annapurna 的帮助下设计的。在 Inferentia 之前不久，亚马逊还发布了 Graviton 芯片——这是一款基于 ARM Cortex-A72 架构的定制化 CPU 处理器，其最高时钟速度为 2.3GHz。

开启科技巨头自研机器学习芯片风潮的公司是谷歌。2016 年，谷歌就正式发布了 TPU（其时谷歌已在内部使用 TPU 超过一年时间了），时至今天，TPU 已经发展到了第三代。这款产品的算力也已经成为谷歌云服务的一部分，可为所有开发者提供算力支持。亚马逊的最大竞争对手微软 Azure 尚未推出自己的处理器。当然，所有三家公司的机器学习云服务算力主要基于英伟达 GPU。此外，AWS 和 Azure 还提供用于机器学习的 FPGA 服务。

虽然亚马逊今天对外发布了此芯片，但 AWS CEO Andy Jassy 表示 Inferentia 在明年之前不会上线。在发布会上，亚马逊也没有公布有关这款芯片的更多技术细节。该公司表示，每块 Inferentia 芯片可提供「提供数百 TOPS」的推理吞吐量...... 为获得更高性能，可以将多个 AWS Inferentia 芯片并联实现数千 TOPS 的吞吐量。

目前的 AI 专用芯片在性能指标上最为强大的是华为今年 10 月推出的[昇腾 910](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650749745%26idx%3D1%26sn%3De0485ef5255261446cf19a130311cf42%26chksm%3D871aff4fb06d7659c613fdcaf709c422b3ae17cd46af439fda0ad8a00dde2ad5906496a6d323%26scene%3D21%23wechat_redirect)，据称其半精度（FP16）算力可达 256TFLOPS，最大功耗为 350W。

在云服务方面，亚马逊正在成为行业领导者，而其推出的 AI 芯片与定制化 CPU，势必更将巩固这家公司的领先地位。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
*参考链接：*[https://techcrunch.com/2018/11/28/aws-announces-new-inferentia-machine-learning-chip/](https://link.zhihu.com/?target=https%3A//techcrunch.com/2018/11/28/aws-announces-new-inferentia-machine-learning-chip/)


