# 手把手教你构建食物识别AI：小白轻易可上手，人气高赞有Demo - 知乎
# 



铜灵 编译整理

量子位 出品 | 公众号 QbitAI
![](https://pic4.zhimg.com/v2-eb111c37ba8340280a88ef5ac4f249c7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='480'></svg>)
谁说深度学习任务都是步骤复杂、高深难懂的？

其实，小白也可以轻易上手，还能做出好玩有用的作品。

最近，工程师Nidhin Pattaniyil和Reshama Shaikh写了一篇从零开始构建网页&App版食物识别AI的教程，有数据、有教程、有Demo，简洁好懂，赢得了大批推特网友点赞。

连fast.ai创始人、Kaggle前主席及首席科学家Jeremy Howard都点赞推荐：
![](https://pic2.zhimg.com/v2-8ba6a34771b042d09ac510a87924a06d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='338'></svg>)
以下来看详细步骤：
![](https://pic2.zhimg.com/v2-5e9f94c0f4fcc910b732c8c1cc8ae485_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='631'></svg>)
**热身运动**

先大致了解下这次的任务，就用深度学习的方法分别在网页和手机端构建食物图像分类器。

学完这个项目，你将get以下技能：
- 用fastai库训练一个给食物照片分类的深度学习模型
- 用Heroku和Flask将这个模型部署到网页和移动端

这篇教程共分为三部分，目录如下：

**第一部分**：训练分类器

**第二部分**：将模型部署到网页端

**第三部分**：将模型部署到移动端

不多说了，下面正式开始具体讲解具体步骤。

**训练分类器**

要构建这样一个好玩的应用，需要先获取**模型权重文件**，你可以在任何深度学习库中获取，两位作者小哥用到的是fastai库。

打开下面这个GitHub repo，里面Jupyter Notebook中的代码可以帮你训练模型。

地址：

[https://github.com/npatta01/web-deep-learning-classifier](https://link.zhihu.com/?target=https%3A//github.com/npatta01/web-deep-learning-classifier)

此外，还需要准备模型训练的相关软件，两位小哥用的是：
- fastai: version 1.0.42
- PyTorch: version 1.0.0
- Python: version 3.6

1.0版本的深度学习fastai库是可用的，需要在PyTorch上运行。如果对fastai的操作不熟悉，这有一套fastai课程可以学习：

[https://docs.fast.ai/](https://link.zhihu.com/?target=https%3A//docs.fast.ai/)

我们用到的数据是在谷歌云平台上进行检索和分析的，当然你可以随意选择云平台（亚马逊、Paperspce）等等都可以。

在fastai上可以找到安装GPU工作环境的操作指导，这个项目中两位小哥用到的设备是英伟达Tesla P100和100GB的硬盘。

小哥还推荐了一个好玩的食物分类数据集：Food-101。

Food-101数据集中包含了101种类别的食物，共有101k（也就是101000）张图像，每个类别中有1000张图片，其中250张是测试图像，750张是训练图像。

不过需要注意，这个数据集中的训练图像是不干净的，还包含了一些噪音数据，有一些标签是错误的。

在这个数据集中，所有的图像都被重新调整了大小，最大边长为512像素。数据集共5GB大小，可以用下面的代码进行检索：
![](https://pic4.zhimg.com/v2-607a5b63d0c382a02311c5a1ca823cbb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1036' height='108'></svg>)
用下面的代码块打开tar.gz文件：
![](https://pic2.zhimg.com/v2-c736b47f20ddb0b0ef7a8eb32f53ddc9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='96'></svg>)
软硬件准备齐全，数据集也配好了，下面可以开始训练了。小哥用Resnet-34 CNN架构开始训练模型，在谷歌云平台上的运行时间约一个小时。

训练模型的代码可以在下面这个地址中找到：

[https://github.com/npatta01/web-deep-learning-classifier](https://link.zhihu.com/?target=https%3A//github.com/npatta01/web-deep-learning-classifier)

用ImageDataBunch读取这些图像：
![](https://pic4.zhimg.com/v2-736b8af1bf776fd8be9a32cc82c5124b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='164'></svg>)
这些数据已经在resnet34上预训练过了，现在只需要直接运行确定最佳学习率。
![](https://pic3.zhimg.com/v2-79b167bdc26797185aa997933759e6c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='158'></svg>)
在这个案例中，当学习率=0.01时运行情况最好：
![](https://pic1.zhimg.com/v2-dc36da0b1815fe978a1eda65921e89dc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='88'></svg>)
研究人员还运行了更多复杂的架构，考虑到数据有101种类别和GPU的处理时间及存储条件，他们最终选定的是一个简单的Resnet34架构。

然后，用0.01的学习率进行了8次迭代，错误率减小到了28.9%。
![](https://pic2.zhimg.com/v2-c686465b0ae695244a436ad462803f49_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='267'></svg>)
然后，小哥解冻了最后几层网络，寻找新的学习率。
![](https://pic1.zhimg.com/v2-65ce1e01645b165a72db1b85f62d935c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='185'></svg>)
5次迭代后，错误率从28.9%减小到28.1%。
![](https://pic3.zhimg.com/v2-9e0b1bc332a9b81da0d36be260e9026a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='240'></svg>)
混淆矩阵显示，以下类别的食物经常被错误分类：
![](https://pic3.zhimg.com/v2-94a0e6e532350fa10a0e112237eac576_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='477'></svg>)
**输出深度学习模型**

输出这个深度学习模型时还伴随着一个权重文件，即model.pth (或final.pth)。如果你按照这个教程来训练的话，模型的存储位置在models代码块中。

final.pth文件可以从Jupyter中下载到你的电脑中。

model.pth文件可能太大了放不进git commit中，这里有一些处理方法：

1、存储在google drive中

2、存储在GitHub releases中

3、存储在云端的bucket中

这两位外国小哥将他们的模型存储在了GitHub releases中，地址：

[https://github.com/npatta01/web-deep-learning-classifier/releases](https://link.zhihu.com/?target=https%3A//github.com/npatta01/web-deep-learning-classifier/releases)

**在网页端部署**

模型训练完毕，现在终于可以开始部署阶段了。小哥把这个repo当成模板：

[https://github.com/npatta01/web-deep-learning-classifier](https://link.zhihu.com/?target=https%3A//github.com/npatta01/web-deep-learning-classifier)

并将分类器算法model.pth的输出作为Heroku app的输入。

**需要进行的文件更新**

如果要运行这个网页端应用，还得先进行以下操作：

1、在src/config.yaml文件中更新这些内容：
- title
- description
- about
- code
- sampleImages

2、把docs/2_heroku_app.md文件中的应用名改成APP_NAME=”food-img-classifier。

**安装Heroku**

如果你没有Heroku的账户，先去注册（ [http://www.heroku.com](https://link.zhihu.com/?target=http%3A//www.heroku.com)）。

记得在你的代码中，更新APP_NAME：
![](https://pic4.zhimg.com/v2-c29a775d4d8ebb7dcd1abdfb7f8143b7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='338'></svg>)
注意：如果在15分钟内没有任何操作，Heroku将暂停运行，当再次唤醒网页端应用时，Heroku会被再次唤醒。

**在移动端部署模型**

先送上repo地址：

[https://github.com/npatta01/mobile-deep-learning-classifier](https://link.zhihu.com/?target=https%3A//github.com/npatta01/mobile-deep-learning-classifier)

**需要进行的文件更新**

如果要在手机上运行这个应用，需要进行如下操作：

在 config.js文件中，更新下面的代码块：
- AppConfig
- update the title
- point host to the heroku app url
- description

小哥还放上了手机端食物分类应用的Demo，地址：

[https://www.youtube.com/watch?reload=9&v=7d2qFLeYvRc](https://link.zhihu.com/?target=https%3A//www.youtube.com/watch%3Freload%3D9%26v%3D7d2qFLeYvRc)

**成本预计**

最后，小哥预计了完成这些操作所需要的经济成本，包括：

1、Apple store开发者账号注册费用：100美元

2、安卓Google Play注册费用：30美元

好了~详细教程就介绍到这了，可以开始动手了。

**传送门**

教程原文：
[https://reshamas.github.io/deploying-deep-learning-models-on-web-and-mobile/​reshamas.github.io](https://link.zhihu.com/?target=https%3A//reshamas.github.io/deploying-deep-learning-models-on-web-and-mobile/)
网页版Demo地址：
[Food Classifier​food-img-classifier.herokuapp.com](https://link.zhihu.com/?target=https%3A//food-img-classifier.herokuapp.com/)
iOS版Demo地址：
[https://itunes.apple.com/us/app/food-classifier/id1445356461?mt=8​itunes.apple.com](https://link.zhihu.com/?target=https%3A//itunes.apple.com/us/app/food-classifier/id1445356461%3Fmt%3D8)
安卓版Demo地址：
[https://play.google.com/store/apps/details?id=com.rsnp.foodclassifier​play.google.com](https://link.zhihu.com/?target=https%3A//play.google.com/store/apps/details%3Fid%3Dcom.rsnp.foodclassifier)
— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


