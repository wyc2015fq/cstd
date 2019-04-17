# GitHub上25个最受欢迎的开源机器学习库 - 知乎
# 



> ​作者 | Khoa Pham    译者 | Shawn Lee
原标题：25 Open-Source Machine Learning Repos to Inspire Your Next Project  


 在过去的几年里，机器学习为各行各业开创了新纪元，诞生了许多成功的案例： Facebook 的面部识别，Netflix 的智能电影推荐系统，PrimaAI 的图像风格转换，Siri 的语音识别，Google Allo 的自然语言处理，及其他很多开发中的项目。




除了这些案例以外，GitHub 上有大量托管的开源机器学习项目。我们挑选了最受大家欢迎的项目，**这些项目涵盖 CV、NLP 及语音三大领域的 25 个开源项目，包括：如何让机器学习创作音乐、歌曲；如何为草图、灰度图像上色；图片风格转换；语音风格迁移；在 IOS 或 Android 上进行面部检测与情感分类；提供游戏研究平台；最先进的物体检测算法的实现；无人机与汽车的模拟器；损毁图像的恢复工具、人体关键点检测等内容。**希望大家能从中学到新的知识并得到启发。







**▌TensorFlow**




TensorFlow 是一个用于研究和生产的开源机器学习库。 TensorFlow 为初学者和专家提供了各种API，以便对桌面、移动终端、Web和云进行开发。



![](https://pic2.zhimg.com/v2-c9a70c857f3579ee0a0dc41784a8923d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='688' height='387'></svg>)



TensorFlow 由 Google Brain 团队开发，最初仅供 Google 内部使用。2015年11月 9 日，TensorFlow 在 Apache 2.0 开源许可下发布。最新版本的 TensorFlow 支持  Keras，它是一种用 Python 编写的高级神经网络 API，能够在 TensorFlow， CNTK 或 Theano 之上运行。Keras 也包含 Javascript 和 Swift 的接口。



![](https://pic2.zhimg.com/v2-7a56c245a34a6c9725940e1df3c45995_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='553'></svg>)






**▌Scikit-learn**




Scikit-learn 是基于 NumPy、SciPy 和 Matplotlib 的一款简单有效的数据挖掘和数据分析工具。 由于其简单和便捷的功能，它常与 TensorFlow 一起使用。



![](https://pic4.zhimg.com/v2-f18a9f0c749516ddb53a933c7da59a03_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='520' height='390'></svg>)![](https://pic2.zhimg.com/v2-c22184769f645062d742e8170f785df9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='360'></svg>)









**▌MXNet**



![](https://pic3.zhimg.com/v2-3d8110c3b926da6473512caa4e2399be_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='171'></svg>)



除了 TensorFlow 、Keras 和 Scikit-learn 之外，Apache 的 MXNet 也是一款深度学习的框架工具。 它专为提高效率和灵活性而设计，允许混合使用符号和命令式编程，以最大限度地提高效率和生产力。



![](https://pic4.zhimg.com/v2-82b954e7f030f6345ac3fb3a562985bf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='270'></svg>)









**▌PyTorch**



![](https://pic3.zhimg.com/v2-c5eae07621ee1b8e710a9fdc066b9972_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='300' height='210'></svg>)



PyTorch 绝对是这个列表里一定要提到的一款应用，它在机器学习爱好者里享有很高的知名度。 PyTorch 基于 Torch ，并由 Facebook 作为其机器学习的框架发布。 PyTorch 是一个 Python 包，它提供两个高级功能：由强 GPU 支持的加速张量计算（如 NumPy ），其深度神经网络是建立在基于磁盘的 autograd 包上。



![](https://pic4.zhimg.com/v2-c02d21b304636ddd324528dc73b98b6b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='508'></svg>)
（来源：

[https://twitter.com/pytorch/status/966324198758006784](https://link.zhihu.com/?target=https%3A//twitter.com/pytorch/status/966324198758006784)）










**▌magenta **



![](https://pic1.zhimg.com/v2-e9c4f6ec4098843aba44a4a3d89d9b34_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='960' height='341'></svg>)



Magenta 目前还在研究过程中，它致力于探索如何让机器学习参与艺术和音乐的创作。 它主要涉及创新开发深度学习和强化学习的算法以用于生成歌曲、图像、绘图和其他素材。 同时，Magenta 也是对于如何构建智能工具和界面的探索，它允许艺术家和音乐家使用这些模型扩展（而不是取代！）他们的创作路径。




它主要的库适配于 python ，除此之外也有 Javascript 的版本 — magenta.js 。 在艺术创作里，大部分人可能从未想过高级机器学习可以使用在这个领域，但 Megenta 向人们完美地展示了应用的可能性。点击这里观看这些令人惊叹的声音和绘图生成器的演示吧。










**▌style2paints**



![](https://pic4.zhimg.com/v2-a774d5294241b8d1731cc24e71e22f97_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='794'></svg>)



该项目旨在为线稿着色。 AI 可以根据既定的颜色样式在草图上绘制、或在草图上创建绘制自己的颜色风格、亦或者将现有的风格转换至另一种风格。




有一些值得一看的新的功能，例如色彩锚和图像过渡。










**▌Image-to-image translation in PyTorch **



![](https://pic4.zhimg.com/v2-6797f02e12791fe24e1fc01c5ef1cb7b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='519'></svg>)



该项目有两个组成部分，CycleGAN 和 pix2pix 。基于 PyTorch ，它们可以实现用于未配对和成对的图像到图像转换。 乍看之下，它仿佛只是一种相当普通的风格转移解决方案，事实上，它跟其他应用不一样。比如它能将图片里普通的马转换为斑马或从实景照片转换为莫奈风格的画作。并且它的处理速度快到足以在实时视频上应用。










**▌Deep voice conversion**



![](https://pic1.zhimg.com/v2-93c83acdd19e5ab45490cd05b59994e4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='423'></svg>)



列表上已经罗列了一些用于图像和视频的风格转换工具了，但是语音呢？ 深度语音转换便是此功能的完美示例。




如果你可以模仿名人的声音或拥有著歌手一样歌喉，你会怎么干点什么？ 深度语音转换的目的是将任意人的声音转换为特定的目标声音，也就是所谓的语音风格转移。项目开始时，我们目标将任意人的声音转换为著名的英国女演员 Kate Winslet 的声音。 我们使用了一个深度神经网络和由 Kate Winslet 朗读的2小时有声读物作为数据集来实现这一目标。










**▌StarGAN in PyTorch**



![](https://pic2.zhimg.com/v2-ee27c4818f5afa47c1343662fd3ba6f1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='487'></svg>)



StarGAN 是这篇文章中提到的一个 PyTorch 应用：Unified Generative Adversarial Networks for Multi-Domain Image-to-Image Translation。它可以将源图像转换成不同的发型、皮肤类型、年龄、性别和不同的情绪。










**▌Face detection**



![](https://pic3.zhimg.com/v2-8a2aa36a44cc2954c3a40d63f455419a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='498'></svg>)



面部检测听起来不太吸引人，因为我们可以在 iOS 和 Android 上使用 Core ML 或 ML Kit 轻松地完成这项工作。 但是随着深入了解，你会发现它不仅可以检测面部，还可以检测情绪和性别。




使用 Keras CNN 模型和 OpenCV 在 fer2013 / IMDB 数据集的进行实时人脸检测和情感/性别分类。










**▌Deep universal probabilistic programming**



![](https://pic1.zhimg.com/v2-28313c09063ad96fade3884af6351c70_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='499' height='499'></svg>)



Uber AI Labs 构建了这个深度概率编程库，用于简化其运输服务的预测和优化收益。 任何处理概率建模的人都会对这个库感兴趣。




机遇无处不在，从匹配乘客和驾驶员，建议最佳路线，找到最明智的搭配组合，甚至创造下一代智能车辆。 为了解决这些挑战，我们将最先进的人工智能（AI）技术与数据科学家、工程师和其他用户的丰富专业知识相结合。 我们正在探索一种工具优先的方法，使我们和其他人能够制作下一代AI解决方案。










**▌ParlAI**



![](https://pic4.zhimg.com/v2-d0208a3f4038570b900fb4bf93eedf47_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='200'></svg>)



作为 Facebook 研究项目的一部分，ParlAI 是一个用于在各种公开可用的对话数据集上训练和评估 AI 模型的框架。 ParlAI 是一个在为研究人员提供访问许多流行数据集的权限的同时，也可以共享和测试对话模型的统一框架。 



![](https://pic4.zhimg.com/v2-f1a9e8a260d6827d04948cc068c464db_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='736' height='263'></svg>)









**▌Facets **



![](https://pic4.zhimg.com/v2-fa81de29980c0fe8c2c166798e6ada93_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='746'></svg>)



Facets 一种可视化机器学习数据集的工具。




可视化由 Polymer Web 组件实现，由 Typescript 代码支持，可以轻松嵌入到 Jupyter 笔记本或网页中。




可视化的关键是跨多个数据集的异常检测和分布比较。 有趣的值（例如，大量的缺失数据，或跨多个数据集非常不同的特征分布）以红色突出显示。










**▌ELF with AlphaGoZero **



![](https://pic4.zhimg.com/v2-38b7c2412137a207ea05fc229ac54fb7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='599'></svg>)



ELF 是 AlphaGoZero / AlphaZero 实现的游戏研究平台。 ELF 为游戏研究提供端到端解决方案。 它包括微型实时战略游戏环境，并发模拟，数千台机器的分布式培训，直观的 API ，基于 Web 的可视化以及由 PyTorch 提供支持的强化学习框架。










**▌Detectron**



![](https://pic3.zhimg.com/v2-8f9c55445d756f0bb004627866dc6eb6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='678'></svg>)



Detectron 是 Facebook AI Research 的软件系统，它实现了最先进的物体检测算法，包括 Mask R-CNN 。 它是用 Python 编写的，由 Caffe2 深度学习框架提供支持。










**▌Fast Style Transfer **



![](https://pic2.zhimg.com/v2-9907df442cad026001987eba9e766d21_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='900' height='673'></svg>)



使用 TensorFlow CNN 实现，这可能是图像样式传输的最佳示例之一，顾名思义，它的完成速度非常快。 该项目基于 Gatys 的艺术风格的神经算法， Johnson 对实时样式转换和超分辨率的感知损失以及 Ulyanov 的实例规范化的组合 。










**▌Face recognition **



![](https://pic3.zhimg.com/v2-30c1cc86bea2875cf974a0f8a9f75c96_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1016' height='536'></svg>)



此工具提供简单的面部识别 API 。 它可以找到面部特征，并猜出照片中的人物。




使用 dlib 最先进的面部识别功能构建而成，并通过深度学习构建。 该模型在 Wild 标记的 Labeled Faces 中具有 99.38％ 的准确度。

这还提供了一个简单的 face_recognition 命令行工具，可以让您从命令行对图像文件夹进行人脸识别！










**▌Deep photo style transfer**



![](https://pic2.zhimg.com/v2-31baa42fb347bea9dcfc28c812136be1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='748'></svg>)



另一个十分好用的图片风格转换工具。 这篇文章里包含本应用的代码和研究论文： Deep Photo Style Transfer 。 它提供了简单的API来合并样式和源图像。 令人印象深刻的图像样式转移工具。










**▌Fast Text**



![](https://pic3.zhimg.com/v2-237770e27c256edc92f2d5cab633e326_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='345'></svg>)



FastText 是一个有效学习单词意思和句子分类的库。




为了更好地了解这个项目，请转到他们的文本分类教程，该教程展示了如何在监督学习中使用该库。 文本分类的目标是将文档（例如电子邮件，帖子，文本消息，产品评论等）分配给一个或多个类别。










**▌AirSim**



![](https://pic1.zhimg.com/v2-6ca1b2c37dc0ef64dd2486acc1d0278c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='624'></svg>)



AirSim 是一款基于 Unreal Engine 的无人机，汽车的模拟器。 它是开源的，跨平台的，它支持硬件在环，并支持市面上流行的飞行控制器（如 PX4 ）用于物理和视觉逼真的模拟。 这是一个 Unreal 插件，可以简单地插入到你想要的任何 Unreal 的环境中。










**▌Image restoration**




机器学习可以做的比我们想象的要多。 Deep Image Prior 是基于神经网络的修复图像工具 – 并不是机器学习。



![](https://pic2.zhimg.com/v2-1639bdfeef56f745f3e52ab5c168256d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='658'></svg>)



此工具可以恢复带有划痕、坏点和/或不需要的文本标记的损坏图像。










**▌Open Pose**



![](https://pic3.zhimg.com/v2-e5f1a3a695f224dfcd8353feacdc5792_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='568'></svg>)



Open Pose 代表了第一个在单个图像上联合检测人体、手、面部和足部关键点（总共 135 个关键点）的实时多人系统。










**▌PirateAT **



![](https://pic2.zhimg.com/v2-f59bdea9affa692d116746509543dec9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='672' height='381'></svg>)



PirateAI 在模拟环境（岛屿）中训练自主代理（海盗）。 这个仓库运行一个训练管道，在游戏（寻找宝藏）和模型训练课程（ Keras + hyperopt ）之间交替。










**▌EmojiIntelligence **



![](https://pic1.zhimg.com/v2-5d62121e0394fadf33ac046f92369b90_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='652'></svg>)



与此列表中的许多项目相比，这个项目相当简单，但它是学习神经网络如何工作的良好起点。 项目实现是在没有任何库的纯 Swift 中，并且很容易模仿。










**▌Deep Exemplar-Based Colorization**




它是第一个使用深度学习的基于样本的局部着色工具。 给定参考的彩色图像后，我们的卷积神经网络直接将灰度图像映射到输出彩色图像。 这是基于 Deep Exemplar-based Colorization 论文的实现。



![](https://pic4.zhimg.com/v2-9aa9c03f3a1482f47bed0724efa7c427_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='628'></svg>)



**分享一哈：**

关注微信公众号：**七月在线实验室**，回复数字：**7**，免费进群**领取【Python干货大礼包】，**还有机会赢Python相关书籍，想要的亲们抓紧时间喽，限时免费领取喔~







**今日学习推荐：**

为了让小伙伴们更好的学习，我们为你准备了 **深度学习，机器学习，Python基础入门****推荐系统实战** 相关方面知识，详情点击下方学习~
[深度学习集训营 第二期「线上线下结合，线下在北京和上海」- 七月在线​www.julyedu.com](https://link.zhihu.com/?target=http%3A//www.julyedu.com/weekend/dl2)[机器学习集训营 第七期「线上线下结合，线下在北上深广杭沈济郑成武西长十二城」- 七月在线​www.julyedu.com](https://link.zhihu.com/?target=http%3A//www.julyedu.com/weekend/train7)[Python基础入门 第三期 [一门真正的从零学Python课]​www.julyedu.com](https://link.zhihu.com/?target=https%3A//www.julyedu.com/course/getDetail/130)[推荐系统实战 [首次全面公开推荐技术在BAT中的核心应用，3人拼团直降100多]​www.julyedu.com![图标](https://pic1.zhimg.com/v2-b9a0320c0b99a4e9a6ce10d52632189c_180x120.jpg)](https://link.zhihu.com/?target=https%3A//www.julyedu.com/course/getDetail/124)

