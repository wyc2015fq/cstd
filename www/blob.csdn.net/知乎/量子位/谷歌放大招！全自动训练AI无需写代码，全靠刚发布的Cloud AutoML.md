# 谷歌放大招！全自动训练AI无需写代码，全靠刚发布的Cloud AutoML - 知乎
# 



> 夏乙 问耕 发自 凹非寺
量子位 出品 | 公众号 QbitAI

这真是大事！

谷歌放大招了！
![](https://pic3.zhimg.com/v2-77f622dca9b0461594f37363512f8e9a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='587' height='643'></svg>)
昨天李飞飞和Jeff Dean就在推特上激动人心预告，说谷歌将有重要的里程碑事件发布。现在谜底揭晓了：谷歌今天发布了**Cloud AutoML**。

这是个什么东西？
![](https://pic2.zhimg.com/v2-aa0b4605510fc3154617a0d185216ba5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='502' height='206'></svg>)
通俗点说，Cloud AutoML是个开发利器，即便你不懂机器学习，也能训练出一个定制化的机器学习模型。由于还在Alpha测试版的阶段，目前这个服务仅支持计算机视觉模型，但谷歌表示稍后会支持所有标准机器学习模型，包括语音、翻译、视频、自然语言处理等。
![](https://pic2.zhimg.com/v2-d7214e21fbf558ba05af57328ace79d1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='846' height='468'></svg>)
目前已经可用的服务是Cloud AutoML Vision。

谷歌Cloud AutoML系统基于监督学习，所以需要提供一系列带有标签的数据。具体来说，开发者只需要上传一组图片，然后导入标签或者通过App创建，随后谷歌的系统就会自动生成一个定制化的机器学习模型。

据说，模型会在一天之内训练完成。
![](https://pic3.zhimg.com/v2-522a0bf2ec8914397fc81edd969d78c2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
整个过程，从导入数据到打标签到训练模型，所有的操作都是通过拖拽完成。在这个模型生成以及训练的过程中，不需要任何人为的干预。

这有段视频介绍。
![](https://pic1.zhimg.com/80/v2-bfca7cae5c7214ec7665a5c32e887078_b.jpg)https://www.zhihu.com/video/937118339009617920
过去几个月里，有几家公司一直在测试Cloud AutoML。其中就包括迪士尼。这套系统让迪士尼在线商城的搜索功能更加强大。

所以，照这个势头发展下去，也许企业以后可能就不用雇佣机器学习和数据专家了。

Cloud AutoML，顾名思义就是云上的AutoML。谷歌去年5月发布AutoML，当时谷歌CEO劈柴哥说，现在设计神经网络非常耗时，对专业能力要求又高，只有一小撮科学家和工程师能做。为此，谷歌创造了一种新方法：AutoML，让神经网络去设计神经网络。

去年11月，谷歌对AutoML进行升级。之前的AutoML虽能设计出与人类设计的神经网络同等水平的小型神经网络，但始终被限制在CIFAR-10和Penn Treebank等小型数据集上。升级之后，AutoML也能应对ImageNet这种规模的数据集了。

总之，这个方法就是让AI设计AI。现在谷歌又把这个技能放到云上了。

现在唯一的问题是，谷歌没有公布Cloud AutoML的服务价格，而且也暂时没有对外开放。想要试用这个服务，需要向谷歌发出申请。
![](https://pic1.zhimg.com/v2-c51241a9f761ed984aff7a05fd567ac8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='848' height='193'></svg>)
这个申请，大概要回答十几项的提问。

为了Cloud AutoML的发布，谷歌云人工智能和机器学习首席科学家李飞飞，谷歌云人工智能研发负责人李佳，联合发布了一篇博客，详细介绍了Cloud AutoML。

量子位将这篇博客翻译如下：
![](https://pic1.zhimg.com/v2-64b6854dad42678e95545023f5addd84_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1057' height='439'></svg>)
我们一年多之前加入Google Cloud，开启了AI民主化的使命。我们的目标，是降低进入门槛，将AI提供给最广大的开发者、研究人员和企业群体。

向着这个目标，我们的Google Cloud AI团队进展顺利。2017年，我们推出了Google Cloud Machine Learning Engine（机器学习引擎），帮助有机器学习专业知识的开发者轻松构建适用于任何数据类型的机器学习模型。我们展示了能怎样在预训练模型之上构建现代机器学习服务，也就是视觉、语音、自然语言处理（NLP）、翻译、Dialogflow等API，为商业应用带来无与伦比的规模和速度。我们的数据科学家和ML研究人员社区Kaggle已经发展到百万人规模。现在，使用Google Cloud AI服务的企业数量超过10000，Box、劳斯莱斯船业、玩具公司丘比、和网上超市奥卡多都在用。

但我们能做的远不止于此。目前，世界上只有少数企业能获取足够的人才和预算来享受ML和AI发展带来的益处，能够创建先进机器学习模型的人才非常有限。就算你们公司有ML或者AI工程师，要自己构建定制化的ML模型仍然要经理一个耗时、复杂的过程。虽然Google通过API提供了能完成特定任务的预训练机器学习模型，但要把AI带给每个人，还有很长的路要走。

为了缩小差距，让每家企业都能用上AI，我们推出了Cloud AutoML。

Cloud AutoML通过使用learning2learn、迁移学习等先进技术，帮助ML专业技能有限的企业构建自己的高品质定制化模型。我们相信，Cloud AutoML将帮AI专家提升工作效率，开拓AI新领域，并帮助能力不足的工程师构建他们以前梦寐以求的强大AI系统。

我们发布的第一个Cloud AutoML功能是Cloud AutoML Vision，这个服务能让定制化图像识别ML模型的创建更快、更轻松。它有一个拖放式的界面，让你能轻松地上传图像、训练并管理模型，然后将训练好的模型直接部署在Google Cloud上。之前，Google展示过Cloud AutoML Vision模型在ImageNet、CIFAR等热门数据集上的分类成绩，错误率比通用的ML API更低。

以下是关于Cloud AutoML Vision的更多信息：

**更高的准确率：**Cloud AutoML Vision基于Google的图像识别方法，包括迁移学习、神经架构搜索技术等，这意味着即使你的企业没有足够的机器学习专业技能，也能获得更准确的模型。

**更快：**用Cloud AutoML创建一个简单的机器学习模型来对AI应用做尝试，只需要几分钟，构建一个完整的商用模型，也只需要一天。

**易用：**AutoML Vision有一个简单的图形化用户界面，你可以制定数据，并将其转换为专为你的需求定制的高质量模型。

服装品牌Urban Outfitters的数据科学家Alan Rosenwinkel说：”我们一直在寻找优化客户购物体验的新方法。要向客户提供相关产品推荐、准确的搜索结果和有用的产品筛选器，创建、维护一组全面的产品属性非常重要。但是，手动创建产品属性非常费时费力。为了解决这个问题，我们的团队尝试了用Cloud AutoML通过识别花纹、领口样式等细微产品特征，来将产品归类流程自动化。在帮助我们的客户更好地发现、推荐和搜索产品这件事上，Cloud AutoML前景非常光明。“

迪士尼消费产品和互动媒体CTO及高级副总裁Mike White说：“Cloud AutoML的技术能帮我们创建计算机视觉模型，根据迪士尼的角色、产品类别和颜色来标注我们的产品，这些标注可以整合到我们的搜索引擎中，在shopDisney商店中通过更相关的搜索结果、更快的发现速度和产品推荐，来加强用户体验。”

伦敦动物学会（ZSL）保护技术主管Sophie Maxwell告诉我们：“ZSL是一个国际慈善组织，在全球范围内为保护动物及其栖息地而努力。要履行这一使命，一个关键要求是要追踪野生动物种群来进一步了解他们的分布，更好地理解人类对这些物种的影响。为了达到这一目标，ZSL在野外设置了一系列相机陷阱，当有热量或运动出现时，为经过的动物拍照。然后，这些设备拍下的数据需要人工分析，根据相关的物种进行标注，比如这个是大象、那个是狮子、那个是长颈鹿，这是一个耗资巨大劳动力密集型任务。ZSL的保护技术部门在与Google的Cloud ML团队密切合作，帮助推进这项激动人心的技术，ZSL想用这项技术来自动分类图像，这样可以削减成本、扩大部署范围，帮我们深入了解该如何更有效地保护世界上的野生动物。”

如果有兴趣试用AutoML Vision，可以在这里填表申请：[https://services.google.com/fb/forms/cloudautomlalphaprogram/](https://link.zhihu.com/?target=https%3A//services.google.com/fb/forms/cloudautomlalphaprogram/)

AutoML Vision是我们和Google Brain以及其他Google AI团队密切合作的成果，也是Cloud AutoML系列产品中的第一个。在降低AI门槛的道路上，我们只是刚刚开始，人工智能帮Cloud AI产品的10000多名客户所实现的能力大大激励了我们，我们希望Cloud AutoML的发布，能帮更多企业通过AI发现更多可能。

cloud— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


