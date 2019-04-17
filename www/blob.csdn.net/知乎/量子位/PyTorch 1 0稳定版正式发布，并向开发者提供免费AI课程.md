# PyTorch 1.0稳定版正式发布，并向开发者提供免费AI课程 - 知乎
# 



> 晓查 发自 凹非寺
量子位 出品 | 公众号 QbitAI



![](https://pic4.zhimg.com/v2-093f70e8b2312000b5eab2e57fcfb7eb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='567'></svg>)



Facebook今天在NeurIPS大会上正在发布PyTorch 1.0**稳定版**。

Facebook在5月份公布了该版本的新功能，并在10月举办的PyTorch开发者大会上首次推出它的预览版。

开发者现在可以利用PyTorch 1.0的新功能了。

比如混合前端，可以让开发者在eager和图形模式之间无缝转换。

另外还有改进分布式训练、提供一个高性能的纯C++前端、与云平台深度集成等特性。

## **1.0版本新特性**

PyTorch 1.0的主要新特性包括JIT编译器、更快的分布式、C++扩展等。



![](https://pic3.zhimg.com/v2-44d933ea1bdc3b896d5f519246422ada_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='540'></svg>)



**JIT**是一组编译工具，用来弥补研究和产品部署之间的差距。

新版本增加了改进的**torch.distributed**库，开发者可以在Python和C ++环境中实现更快的训练。新的分布式库主要亮点有：
- 新的torch.distributed是性能驱动的，并且对所有后端（Gloo，NCCL和MPI）完全异步操作
- 显著的分布式数据并行性能改进，尤其适用于网络较慢的主机，如基于以太网的主机
- 为torch.distributed包中的所有分布式集合操作添加异步支持
- 在Gloo后端添加以下CPU操作：send，recv，reduce，all_gather，gather，scatter
- 在NCCL后端添加障碍操作
- 为NCCL后端添加new_group支持

C++前端是连接PyTorch后端的纯C++接口，它遵循已建立的Python前端的API和框架结构，旨在实现高性能、低延迟的C++应用程序。

它提供了与Python前端torch.nn，torch.optim，torch.data等组件的等价接口。



![](https://pic2.zhimg.com/v2-42d9540aeea0e5a3ef3bbfce40a145c1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='579'></svg>)





![](https://pic1.zhimg.com/v2-31755fb83f147fc3595be55a75a47bac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='548'></svg>)



对此，广大C++程序员表示点赞，感谢自己终于得到了Facebook的重视。


C++的API不仅能用于训练，也能用于推理。

一名在Facebook AI研究院（FAIR）的程序员说，现在C++的API感觉非常像在PyTorch中使用Python，在模型中完全使用C++处理张量也非常方便。

最近FAIR的星际争霸AI——**TorchCraftAI**——就是用C++实现的。



![](https://pic4.zhimg.com/v2-25010f559ca1069692ee8ac7a2734d5b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='628'></svg>)



PyTorch 1.0的**其他新功能**还有：
- N维空张量
- 新的算符
- 新的分布式
- 稀疏API改进
- 对现有算符和分布式的补充

从0.4.1到1.0版本，代码是否可以不改动直接运行呢？可以查看文档中的**Breaking Changes**部分，如果你的代码涉及到其中的内容，则需要修改。

[https://github.com/pytorch/pytorch/releases#breaking-changes](https://link.zhihu.com/?target=https%3A//github.com/pytorch/pytorch/releases%23breaking-changes)

## **提供免费课程与开源软件库**

Facebook宣布与Udacity合作，免费提供成人AI课程：

[https://cn.udacity.com/course/deep-learning-pytorch--ud188](https://link.zhihu.com/?target=https%3A//cn.udacity.com/course/deep-learning-pytorch--ud188)

上个月，Udacity和Facebook推出了一个新课程，“PyTorch深度学习简介”，以及“PyTorch挑战计划”。该计划为AI教育提供持续的奖学金。



![](https://pic4.zhimg.com/v2-32973f1845624ce97e6c9b7dc3ac5ce3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='472'></svg>)



除了在线教育课程之外，像fast.ai这样的组织还提供软件库，帮助开发者学习如何使用PyTorch构建神经网络。

fast.ai是一个简化训练、快速准确的神经网络软件库，发布两个月以来，已经在GitHub上获得了10,000颗星。

目前已有多名开发者使用该软件库成功创立了新项目。例如Jason Antic创建了一个名为DeOldify的项目，该项目使用深度学习来着色和恢复旧图像。



![](https://pic2.zhimg.com/v2-890bf1298483cb5df66d4e3048385e6d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='740'></svg>)



## **与三大云平台深度集成**

为了使PyTorch更友好、更易于访问，Facebook继续深化与云平台的合作伙伴关系，如亚马逊AWS、谷歌云平台和微软Azure。



![](https://pic3.zhimg.com/v2-c2f370bfc717f815c54380009ab266ee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='579'></svg>)



最近，AWS推出了支持PyTorch的Amazon SageMaker Neo，允许开发人员在PyTorch中构建机器学习模型，训练完成后在云端部署，性能提升高达2倍。

开发人员现在还可以通过创建新的深度学习VM实例，在谷歌云平台上试用PyTorch 1.0。

此外，微软的Azure机器学习服务也已经推出，它允许数据科学家在Azure上无缝地训练、管理和部署PyTorch模型。

## **扩展项目**

PyTorch已应用于从图像识别到机器翻译等各个领域，Facebook列举了一些来自开发者社区的扩展项目：

**Horovod**： 一个分布式培训框架，使开发者可以轻松地使用单GPU程序，并在多个GPU上快速进行训练。
**PyTorch Geometry**：PyTorch的计算机几何视觉库，提供一组常例和可区分的模块。
**TensorBoardX**：用于将PyTorch模型记录到TensorBoard的模块，允许开发者使用可视化工具进行模型训练。

此外，Facebook的团队还在为PyTorch构建和开源项目，如**Translate**，这是一个基于Facebook机器翻译系统的序列到序列模型训练库。

## **资源**

发行版说明：
[https://github.com/pytorch/pytorch/releases/tag/v1.0.0](https://link.zhihu.com/?target=https%3A//github.com/pytorch/pytorch/releases/tag/v1.0.0)

下载地址：
[https://pytorch.org](https://link.zhihu.com/?target=https%3A//pytorch.org)




— **完** —
量子位 · QbitAI
վ'ᴗ' ի 追踪AI技术和产品新动态

[量子位​www.zhihu.com![图标](https://pic4.zhimg.com/v2-ca6e7ffc10a0d10edbae635cee82d007_ipico.jpg)](https://www.zhihu.com/org/liang-zi-wei-48)
欢迎大家关注我们，以及订阅[我们的知乎专栏](https://zhuanlan.zhihu.com/qbitai)
**诚挚招聘**
量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！
相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。




