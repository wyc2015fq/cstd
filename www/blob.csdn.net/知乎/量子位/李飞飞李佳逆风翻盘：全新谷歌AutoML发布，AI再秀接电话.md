# 李飞飞李佳逆风翻盘：全新谷歌AutoML发布，AI再秀接电话 - 知乎
# 



> 夏乙 问耕 发自 凹非寺
量子位 出品 | 公众号 QbitAI

 今天凌晨，Google再次挥出AI重拳。

李飞飞在Google Cloud Next 18大会上，公布了一系列最新的人工智能产品进展，其中最核心的亮点有两个：

1、Cloud AutoML新增语言分类和翻译两大功能。很多媒体集团已经开始使用。

2、通过图灵测试的AI，现在可以投入商用了，现场展示了AI如何通过语音交流，完美的帮电商用户退货。

每次Google能打电话的AI登场，都有炸裂的效果，这次也是一样。下文我们详细展开，有视频展示~

在这些高光时刻的背后，李飞飞的搭档、Google Cloud AI研发主管李佳首次坦露心迹，直言一年前的这段时间“佳飞🐱”（加菲猫）组合作为新人，在Google内部遭遇举步维艰的时刻。

她们最终“艰难而大胆”的作出取舍决定，聚焦在上面提到的两个核心任务上，才有了今天的成绩，堪称“逆风翻盘”。

下面，请看详细报道。
![](https://pic4.zhimg.com/v2-46e42a5cf9282438836a00b29d1d3c93_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
## **Cloud AutoML再添新功能**

今年1月，Google发布Cloud AutoML，一个里程碑事件。

通俗点说，Cloud AutoML是个开发利器，即便你不懂机器学习，也能训练出一个定制化的机器学习模型。

如果预训练模型不能满足你的需求，而你也不是经验丰富的AI专家，这个功能就是为你准备的。李飞飞也把这个称为AI“民主化的下一步”。

现在Cloud AutoML又有了新进展。

一方面，针对图像分类的Cloud Vision API现在正式发布公共测试版。这个API可以快速将图片归到数千个类别，检测图片内的各个对象和面孔，找到并读取图片内的印刷文字。还能进行图片情感分析等。
![](https://pic2.zhimg.com/v2-d593389ab6c724bee40fed9b406ba14d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
另一方面，李飞飞团队兑现承诺，Cloud AutoML又有新功能上线。而且一次就有两个新功能发布：

1、AutoML Natural Language，可以解析文本的结构和意义。它可用于从文本文档、新闻或博文中提取有关人物、地点、事件等的信息。

2、AutoML Translation，可使用最新的神经机器翻译技术将字符串翻译成任何支持的语言。

这些新功能，目前已经被一些媒体应用。例如赫斯特集、日经集团、金融时报、彭博社等。
![](https://pic1.zhimg.com/v2-457e89deed332c9cf74eff96f0543be8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='203'></svg>)
 “至此，AutoML可以为各行业缺少AI经验的企业和开发者提供自动生成图像功能，语言分类功能和翻译功能。”李佳说。

李佳还透露，目前AutoML的注册用户也已经超过1.8万家，服务行业横跨媒体、零售、金融、保险、能源、医疗、环境等等。

“AutoML超过10%的用户来自医疗和生命医学行业，我们的产品也推动了用户在医疗影像辅助检测，以及及时检测预警中风，哮喘，婴儿猝死综合征方面的创新。”

目前三个功能已经全部上线，如果你感兴趣，可以直接访问，Google的页面上提供了详细的介绍，包括详细的价格介绍（都有中文版本和免费试用）。

> 以下是传送门：

CLOUD VISION [https://cloud.google.com/vision/](https://link.zhihu.com/?target=https%3A//cloud.google.com/vision/)

CLOUD NATURAL LANGUAGE [https://cloud.google.com/natural-language/](https://link.zhihu.com/?target=https%3A//cloud.google.com/natural-language/)

TRANSLATION API [https://cloud.google.com/translate/](https://link.zhihu.com/?target=https%3A//cloud.google.com/translate/)

## **接电话AI**

除了面向各行各业的AutoML，谷歌云今天还专门针对客服行业推出了“客服中心AI（Contact Center AI）”。

它的任务，一是**替**人类客服接电话，二是**帮**人类客服更好地接电话。

李佳说，这是“佳飞组合”推出的第一个Solution Product (行业解决方案产品）。

客服中心AI，与谷歌今年5月在I/O开发者大会上展示的打电话AI Duplex，可以说是兄弟产品，有很多共同的底层组件，但各自有不同的产品构成和目标。
![](https://pic3.zhimg.com/v2-b8b9a5110ddcf4904498fcc01f6ac00a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='491'></svg>)
这个产品包含了谷歌云的对话式交互API Dialogflow的一系列升级，比如来自DeepMind WaveNet的语音合成，通过Dialogflow Phone Gateway获得了接打电话的功能。

当用户打电话给客服中心，会先由虚拟助理来接，这个接电话AI会跟用户打招呼，回答用户的问题，独立完成一些任务。如果它搞不定，就会将电话转接给人类客服。
[Google展示呼叫中心AI_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=http%3A//v.qq.com/x/page/j0733fqff9s.html)
谷歌以电商退换货为例展示了客服中心AI的能力。这个AI的接电话水平会比“充值请按1，查询请按2”高出一大截。

在上面的视频里，这个AI和用户是完全通过自然语言交流的，AI可以根据订单信息猜测人类用户的大概意图，在人类用户提出“退货”的时候，能正确理解人类向干什么，还能给人发送退货信息的邮件。

更厉害的是，它接电话还接出了“One More Thing”：问人类要不要找eBay的时尚专家帮选一下尺码。于是，就到了人类出场的时刻。

当然，这个人类，也是AI分析了用户之后，挑选出来的。
![](https://pic1.zhimg.com/v2-cca20c47480333b0bf5c40f1fde6e5c0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='548'></svg>)
我们可以看到，人类客服接了电话之后，客服中心AI的工作也没有停止。它有一个Agent Assist系统，能从公司的知识库中提取最关键的文档，为人类客服提供对话相关的信息，还会列出一些建议问题。

人类客服和用户的对话，也都会实时转成文本显示在桌面上。

李飞飞在博客里说，接下来几周还会分享更多关于客服中心AI的信息。

## **TPU 3.0上岗，众多API更新**

新功能亮眼，基础设施也同样有新升级。

谷歌云今天还宣布，第三代TPU正式上岗了，进入Alpha内测阶段。TPU 3.0是谷歌在今年5月I/O大会上发布的，谷歌CEO Pichai说，它的性能比上一代高8倍，高达100 petaflops。
![](https://pic2.zhimg.com/v2-412b718cc1c0dfa58cf7419ba358d705_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='576'></svg>)
除此之外，谷歌云的机器学习API们今天也迎来了一波更新。

Cloud Vision API现在可以识别手写字体了，增加了对PDF和TIFF格式文件的支持，还支持产品搜索，增加了目标检测能力。

Cloud Text-to-Speech API的语音合成能力也有提升，它现在支持用DeepMind推出的WaveNet来合成多个语种的声音，还能专门面向说话人的类型来优化。

Cloud Speech-to-Text API，也就是语音识别API，获得了识别语种、在对话中识别不同说话人的能力。

## **李飞飞、李佳闯过艰难岁月**

2017年初，李飞飞和李佳这对组合，正式加入Google Cloud团队，负责人工智能和机器学习相关业务。

她们还推动建立了Google AI中国中心，这个中心也由李飞飞和李佳共同领导，李佳担任Google AI中国中心总裁。

在今天的Google Cloud大会光鲜的新产品发布之后，李佳进行了概要总结，并且首次坦露心迹。

“回想一年前的这段时间，正是我们举步维艰的时刻。”

李佳回忆当时的情景说：“佳飞🐱”谷歌新人，团队产品主管空缺已几个月，工程总监离职，人力资源青黄不接……

就在这种局面下，李飞飞和李佳在产品布局方面做出艰难而大胆的决定：大幅度投入AutoML和Contact Center，砍掉一些成功可能性比较小的产品。

也正是这个战略聚焦的举措，加上团队的共同努力，最终换来今日的成绩。 
![](https://pic2.zhimg.com/v2-c919a468844f5d950075ebeccda0f8fd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='554' height='736'></svg>)李佳和李飞飞
 以下是李佳的今日总结全文：

今天是“佳飞🐱”组合的一个新里程碑，我们宣布了我们第一个Solution Product (行业解决方案产品）Contact Center AI集虚拟助理，智能信息发掘和情感分析等功能于一身，帮助Contact Center 的工作人员更有效的解决问题和用户提升体验。

如我们年初发布AutoML Vision时承诺，AutoML 也迎来了新的产品成员 AutoML Natural Language和 AutoML Translation. 至此，AutoML可以为各行业缺少AI经验的企业和开发者提供自动生成图像功能，语言分类功能和翻译功能。AutoML的注册用户也已经超过18，000家，服务行业横跨媒体，零售，金融，保险，能源，医疗，环境等等。产品和创新环环相扣, 合作也带来进步（这里为研究者们跑题一下，我们的Progressive neural architecture search文章刚被ECCV收录Oral)，希望我们能够不断提高，把最好的技术带给大家。

各个现有产品如AI Platform, APIs 等都有不同程度的升级和功能增加。垂直行业的解决方案及和partners的合作也有非常大的进展。以医疗为例，我们的AI platform将合作者的基因疾病检测算法训练时间从一周降到1天，AutoML 超过10%的用户来自医疗和生命医学行业，我们的产品也推动了用户在医疗影像辅助检测，以及及时检测预警中风，哮喘，婴儿猝死综合征方面的创新。

成千上万企业及用户的交流反馈让我们受益匪浅，我们的产品方向也最大限度以赋能各行业为目标。回想一年前的这段时间，正是我们举步维艰的时刻，“佳飞🐱”谷歌新人，团队产品主管空缺已几个月，工程总监离职，人力资源青黄不接。大幅度投入AutoML 和 Contact Center， 砍掉一些成功可能性比较小的产品， 是当时条件下在战略，产品布局方面的艰难而大胆的决定。谢谢团队的小伙伴们契而不舍的支持我们的工作，你们的成绩今天终于和大家见面了！有幸和你们一起工作，创造，为改变世界一点一滴的努力，为你们感到骄傲！

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


