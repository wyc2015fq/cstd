# PyTorch学习之路（level1）——训练一个图像分类模型 - AI之路 - CSDN博客





2017年11月13日 21:55:35[AI之路](https://me.csdn.net/u014380165)阅读数：24318
所属专栏：[PyTorch使用及源码解读](https://blog.csdn.net/column/details/19413.html)









这是一个适合PyTorch入门者看的博客。PyTorch的文档质量比较高，入门较为容易，这篇博客选取[官方链接](http://pytorch.org/tutorials/beginner/transfer_learning_tutorial.html)里面的例子，介绍如何用PyTorch训练一个ResNet模型用于图像分类，代码逻辑非常清晰，基本上和许多深度学习框架的代码思路类似，非常适合初学者想上手PyTorch训练模型（不必每次都跑mnist的demo了）。接下来从个人使用角度加以解释。解释的思路是从数据导入开始到模型训练结束，基本上就是搭积木的方式来写代码。

首先是数据导入部分，这里采用官方写好的torchvision.datasets.ImageFolder接口实现数据导入。这个接口需要你提供图像所在的文件夹，就是下面的data_dir=‘/data’这句，然后对于一个分类问题，这里data_dir目录下一般包括两个文件夹：train和val，每个文件件下面包含N个子文件夹，N是你的分类类别数，且每个子文件夹里存放的就是这个类别的图像。这样torchvision.datasets.ImageFolder就会返回一个列表（比如下面代码中的image_datasets[‘train’]或者image_datasets[‘val]），列表中的每个值都是一个tuple，每个tuple包含图像和标签信息。

```
data_dir = '/data'
image_datasets = {x: datasets.ImageFolder(
                    os.path.join(data_dir, x),
                    data_transforms[x])， 
                    for x in ['train', 'val']}
```

另外这里的data_transforms是一个字典，如下。主要是进行一些图像预处理，比如resize、crop等。实现的时候采用的是torchvision.transforms模块，比如torchvision.transforms.Compose是用来管理所有transforms操作的，torchvision.transforms.RandomSizedCrop是做crop的。需要注意的是对于torchvision.transforms.RandomSizedCrop和transforms.RandomHorizontalFlip()等，输入对象都是PIL Image，也就是用python的PIL库读进来的图像内容，而transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])的作用对象需要是一个Tensor，因此在transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])之前有一个 transforms.ToTensor()就是用来生成Tensor的。另外transforms.Scale(256)其实就是resize操作，目前已经被transforms.Resize类取代了。

```
data_transforms = {
    'train': transforms.Compose([
        transforms.RandomSizedCrop(224),
        transforms.RandomHorizontalFlip(),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ]),
    'val': transforms.Compose([
        transforms.Scale(256),
        transforms.CenterCrop(224),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ]),
}
```

前面torchvision.datasets.ImageFolder只是返回list，list是不能作为模型输入的，因此在PyTorch中需要用另一个类来封装list，那就是：torch.utils.data.DataLoader。torch.utils.data.DataLoader类可以将list类型的输入数据封装成Tensor数据格式，以备模型使用。注意，这里是对图像和标签分别封装成一个Tensor。这里要提到另一个很重要的类：torch.utils.data.Dataset，这是一个抽象类，在pytorch中所有和数据相关的类都要继承这个类来实现。比如前面说的torchvision.datasets.ImageFolder类是这样的，以及这里的torch.util.data.DataLoader类也是这样的。所以当你的数据不是按照一个类别一个文件夹这种方式存储时，你就要自定义一个类来读取数据，自定义的这个类必须继承自torch.utils.data.Dataset这个基类，最后同样用torch.utils.data.DataLoader封装成Tensor。

```
dataloders = {x: torch.utils.data.DataLoader(image_datasets[x],     
                                            batch_size=4, 
                                            shuffle=True,
                                            num_workers=4) 
                                            for x in ['train', 'val']}
```

生成dataloaders后再有一步就可以作为模型的输入了，那就是将Tensor数据类型封装成Variable数据类型，来看下面这段代码。dataloaders是一个字典，dataloders[‘train’]存的就是训练的数据，这个for循环就是从dataloders[‘train’]中读取batch_size个数据，batch_size在前面生成dataloaders的时候就设置了。因此这个data里面包含图像数据（inputs）这个Tensor和标签（labels）这个Tensor。然后用torch.autograd.Variable将Tensor封装成模型真正可以用的Variable数据类型。 

为什么要封装成Variable呢？在pytorch中，torch.tensor和torch.autograd.Variable是两种比较重要的数据结构，Variable可以看成是tensor的一种包装，其不仅包含了tensor的内容，还包含了梯度等信息，因此在神经网络中常常用Variable数据结构。那么怎么从一个Variable类型中取出tensor呢？也很简单，比如下面封装后的inputs是一个Variable，那么inputs.data就是对应的tensor。

```
for data in dataloders['train']:
   inputs, labels = data

   if use_gpu:
       inputs = Variable(inputs.cuda())
       labels = Variable(labels.cuda())
   else:
       inputs, labels = Variable(inputs), Variable(labels)
```

封装好了数据后，就可以作为模型的输入了。所以要先导入你的模型。在PyTorch中已经默认为大家准备了一些常用的网络结构，比如分类中的VGG，ResNet，DenseNet等等，可以用torchvision.models模块来导入。比如用torchvision.models.resnet18(pretrained=True)来导入ResNet18网络，同时指明导入的是已经预训练过的网络。因为预训练网络一般是在1000类的ImageNet数据集上进行的，所以要迁移到你自己数据集的2分类，需要替换最后的全连接层为你所需要的输出。因此下面这三行代码进行的就是用models模块导入resnet18网络，然后获取全连接层的输入channel个数，用这个channel个数和你要做的分类类别数（这里是2）替换原来模型中的全连接层。这样网络结果也准备好。

```
model = models.resnet18(pretrained=True)
num_ftrs = model.fc.in_features
model.fc = nn.Linear(num_ftrs, 2)
```

但是只有网络结构和数据还不足以让代码运行起来，还需要定义损失函数。在PyTorch中采用torch.nn模块来定义网络的所有层，比如卷积、降采样、损失层等等，这里采用交叉熵函数，因此可以这样定义：
`criterion = nn.CrossEntropyLoss()`
然后你还需要定义优化函数，比如最常见的随机梯度下降，在PyTorch中是通过torch.optim模块来实现的。另外这里虽然写的是SGD，但是因为有momentum，所以是Adam的优化方式。这个类的输入包括需要优化的参数：model.parameters()，学习率，还有Adam相关的momentum参数。现在很多优化方式的默认定义形式就是这样的。
`optimizer = optim.SGD(model.parameters(), lr=0.001, momentum=0.9)`
然后一般还会定义学习率的变化策略，这里采用的是torch.optim.lr_scheduler模块的StepLR类，表示每隔step_size个epoch就将学习率降为原来的gamma倍。
`scheduler = lr_scheduler.StepLR(optimizer, step_size=7, gamma=0.1)`
**准备工作终于做完了，要开始训练了。**

首先训练开始的时候需要先更新下学习率，这是因为我们前面制定了学习率的变化策略，所以在每个epoch开始时都要更新下：
`scheduler.step()`
然后设置模型状态为训练状态：
`model.train(True)`
然后先将网络中的所有梯度置0：
`optimizer.zero_grad()`
然后就是网络的前向传播了：
`outputs = model(inputs)`
然后将输出的outputs和原来导入的labels作为loss函数的输入就可以得到损失了：
`loss = criterion(outputs, labels)`
输出的outputs也是torch.autograd.Variable格式，得到输出后（网络的全连接层的输出）还希望能到到模型预测该样本属于哪个类别的信息，这里采用torch.max。torch.max()的第一个输入是tensor格式，所以用outputs.data而不是outputs作为输入；第二个参数1是代表dim的意思，也就是取每一行的最大值，其实就是我们常见的取概率最大的那个index；第三个参数loss也是torch.autograd.Variable格式。
` _, preds = torch.max(outputs.data, 1)`
计算得到loss后就要回传损失。要注意的是这是在训练的时候才会有的操作，测试时候只有forward过程。
`loss.backward()`
回传损失过程中会计算梯度，然后需要根据这些梯度更新参数，optimizer.step()就是用来更新参数的。optimizer.step()后，你就可以从optimizer.param_groups[0][‘params’]里面看到各个层的梯度和权值信息。
`optimizer.step()`
**这样一个batch数据的训练就结束了！当你不断重复这样的训练过程，最终就可以达到你想要的结果了。**

另外如果你有gpu可用，那么包括你的数据和模型都可以在gpu上操作，这在PyTorch中也非常简单。判断你是否有gpu可以用可以通过下面这行代码，如果有，则use_gpu是true。
`use_gpu = torch.cuda.is_available()`
**完整代码请移步**：[Github](https://github.com/miraclewkf/ImageClassification-PyTorch/blob/master/level1/train.py)



