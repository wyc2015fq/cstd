# 简化深度学习实践流程：新鲜出炉的TensorFlow项目模板来了 - 知乎
# 



> 林鳞 编译自 GitHub
量子位 出品 | 公众号 QbitAI

新的TensorFlow项目模板来了。

昨天，用户mrgemy95在Reddit上发帖，称这是“TensorFlow项目模板架构的最佳实践”。几小时内，帖子热度迅速上升。

此外，mrgemy95还将代码公布在GitHub上，短短几天就收获了200+个star。

量子位带大家去GitHub上探查一下这个模板。
![](https://pic3.zhimg.com/v2-dfeee315b7ed9aedd4e65a64be0b3f7e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='287'></svg>)
一个简单且精心设计的结构对于任何深度学习项目来说都是必不可少的，所以在TensorFlow项目中经过大量实践后，mrgemy95等人总结出一个集简洁、文件夹结构的最佳实践和良好的OOP（面向对象的程序设计）的TensorFlow项目模板。
![](https://pic4.zhimg.com/v2-48310acdad5bb701d80344b7cba98a97_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='483'></svg>)△ 项目架构如上图所示![](https://pic1.zhimg.com/v2-40f14820c801a73bed5e20c08ce523d0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)△ 文件夹结构如上图所示
如何使用这个模板呢？举个例子，假设你想实现VGG模型，你需要按照以下步骤执行：

在模型文件夹中创建一个从“base_model”分类继承来的VGG分类。
![](https://pic4.zhimg.com/v2-bfcc9109735133aff7e8dcb2da6d8477_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1027' height='178'></svg>)
重写“build_model”和“init_saver”这两个函数。

前者是实现VGG模型的地方，后者是定义TensorFlow saver的地方。
![](https://pic1.zhimg.com/v2-08c5ef04dc6d25a2a596bcf3fef2d894_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1049' height='208'></svg>)
在trainer文件夹中创建一个从“base_train”分类继承来的VGG trainer。
![](https://pic4.zhimg.com/v2-c825acfa9f22efa1685ab11de3d8cab3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='984' height='110'></svg>)
重写“train_step”和“train_epoch”这两个函数。
![](https://pic2.zhimg.com/v2-f4962a109549d16c04004d792ffd7bad_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='250'></svg>)
在主文件中创建一个会话，并创建“Model”、“Logger”、“Data_Generator”、“Trainer”对象的实例和配置。
![](https://pic3.zhimg.com/v2-2b91de1b7860030ad553a1c0f976e0ea_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='204'></svg>)
将所有这些对象传递给trainer对象，通过调用“trainer.train()”开始你的训练。
![](https://pic4.zhimg.com/v2-d232d5e811fb4b5554910c19e03677c3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='141'></svg>)
之后，你将在模型和trainer文件夹中创建模板文件和一个简单的示例，它将向你展示如何简单尝试第一个模型。

作者表示，之后会考虑用新的TensorFlow数据集API替换数据加载器部分。

最后，附项目GitHub地址：
[Mrgemy95/Tensorflow-Project-Template​github.com![图标](https://pic3.zhimg.com/v2-3dfa1becbbb899adcd85f29df7e338ce_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/Mrgemy95/Tensorflow-Project-Template%23in-details)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


