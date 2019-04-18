# PyTorch使用总览 - AI之路 - CSDN博客





2018年01月31日 21:24:22[AI之路](https://me.csdn.net/u014380165)阅读数：5041
所属专栏：[PyTorch使用及源码解读](https://blog.csdn.net/column/details/19413.html)[faster-rcnn.pytorch](https://blog.csdn.net/column/details/25674.html)









**深度学习框架训练模型时的代码主要包含数据读取、网络构建和其他设置三方面，基本上掌握这三方面就可以较为灵活地使用框架训练模型。PyTorch是Facebook的官方深度学习框架之一，到现在开源1年时间，势头非常猛，相信使用过的人都会被其轻便和快速等特点深深吸引，因此这篇博客从整体上介绍如何使用PyTorch**。 

PyTorch的官方github地址：[https://github.com/pytorch/pytorch](https://github.com/pytorch/pytorch)

PyTorch官方文档：[http://pytorch.org/docs/0.3.0/](http://pytorch.org/docs/0.3.0/)
建议先看看：[PyTorch学习之路（level1）——训练一个图像分类模型](http://blog.csdn.net/u014380165/article/details/78525273)，对Pytorch的使用有一个快速的了解。

**接下来就按照上述的3个方面来介绍如何使用PyTorch。**

#### 一、数据读取

数据读取部分包含如何将你的图像和标签数据转换成PyTorch框架的Tensor数据类型，官方代码库中有一个接口例子：torchvision.ImageFolder，这个接口在[PyTorch学习之路（level1）——训练一个图像分类模型](http://blog.csdn.net/u014380165/article/details/78525273) 中有简单介绍。因为这个接口针对的数据存放方式是每个文件夹包含一个类的图像，但是实际应用中可能你的数据不是这样维护的，或者你的数据是多标签的，或者其他更复杂的形式，那么就需要自定义一个数据读取接口，这个时候就不得不提一个**PyTorch中数据读取基类：torch.utils.data.Dataset**，包括前面提到的torchvision.ImageFolder接口的对应类也是继承torch.utils.data.Dataset实现的，因此torch.utils.data.Dataset类是PyTorch框架中数据读取的核心。**那么如何自定义一个数据读取接口呢？**可以看博客：[PyTorch学习之路（level2）——自定义数据读取](http://blog.csdn.net/u014380165/article/details/78634829)，这篇博客中从剖析torchvision.ImageFolder接口切入，然后引出如何自定义数据读取接口。这样就完成了数据的第一层封装。

**在自定义数据读取接口时还有一步很重要的操作：数据预处理**。常常我们在论文中看到的data argumentation就是指的数据预处理，对实验结果影响还是比较大的。该操作在PyTorch中可以通过torchvision.transforms接口来实现，具体请看博客：[PyTorch源码解读之torchvision.transforms](http://blog.csdn.net/u014380165/article/details/79167753) 的介绍。

**经过上述的两个操作后，还需再进行一次封装，将数据和标签封装成数据迭代器，这样才方便模型训练的时候一个batch一个batch地进行，这就要用到torch.utils.data.DataLoader接口**，该接口的一个输入就是前面继承自torch.utils.data.Dataset类的自定义了的对象（比如torchvision.ImageFolder类的对象），具体可以参考博客：[PyTorch源码解读之torch.utils.data.DataLoader](http://blog.csdn.net/u014380165/article/details/79058479)

至此，从图像和标签文件就生成了Tensor类型的数据迭代器，后续仅需将Tensor对象用torch.autograd.Variable接口封装成Variable类型（比如`train_data=torch.autograd.Variable(train_data)`，如果要在gpu上运行则是：`train_data=torch.autograd.Variable(train_data.cuda())`）就可以作为模型的输入了。

其他自定义的数据读取接口例子可以参考：[https://github.com/miraclewkf/MobileNetV2-PyTorch](https://github.com/miraclewkf/MobileNetV2-PyTorch)，该项目中的read_ImageNetData.py脚本自定义了读取ImageNet数据集的接口，训练数据的读取和验证数据的读取采取不同的接口实现，比较有特点。

#### 二、网络构建

**PyTorch框架中提供了一些方便使用的网络结构及预训练模型接口：torchvision.models**，具体可以看博客：[PyTorch源码解读之torchvision.models](http://blog.csdn.net/u014380165/article/details/79119664)。该接口可以直接导入指定的网络结构，并且可以选择是否用预训练模型初始化导入的网络结构。

**那么如何自定义网络结构呢？在PyTorch中，构建网络结构的类都是基于torch.nn.Module这个基类进行的**，也就是说所有网络结构的构建都可以通过继承该类来实现，包括torchvision.models接口中的模型实现类也是继承这个基类进行重写的。自定义网络结构可以参考：1、[https://github.com/miraclewkf/MobileNetV2-PyTorch](https://github.com/miraclewkf/MobileNetV2-PyTorch)。该项目中的MobileNetV2.py脚本自定义了网络结构。2、[https://github.com/miraclewkf/SENet-PyTorch](https://github.com/miraclewkf/SENet-PyTorch)。该项目中的se_resnet.py和se_resnext.py脚本分别自定义了不同的网络结构。

**如果要用某预训练模型为自定义的网络结构进行参数初始化**，可以用torch.load接口导入预训练模型，然后调用自定义的网络结构对象的load_state_dict方式进行参数初始化，具体可以看[https://github.com/miraclewkf/MobileNetV2-PyTorch](https://github.com/miraclewkf/MobileNetV2-PyTorch)项目中的train.py脚本中if args.resume条件语句。

#### 三、其他设置

**优化函数**通过torch.optim包实现，比如torch.optim.SGD()接口表示随机梯度下降。更多优化函数可以看官方文档：[http://pytorch.org/docs/0.3.0/optim.html](http://pytorch.org/docs/0.3.0/optim.html)。

**学习率策略**通过torch.optim.lr_scheduler接口实现，比如torch.optim.lr_scheduler.StepLR()接口表示按指定epoch数减少学习率。更多学习率变化策略可以看官方文档：[http://pytorch.org/docs/0.3.0/optim.html](http://pytorch.org/docs/0.3.0/optim.html)。

**损失函数**通过torch.nn包实现，比如torch.nn.CrossEntropyLoss()接口表示交叉熵等。

**多GPU训练**通过torch.nn.DataParallel接口实现，比如：`model = torch.nn.DataParallel(model, device_ids=[0,1])`表示在gpu0和1上训练模型。




