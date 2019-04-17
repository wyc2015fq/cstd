# PyTorch基础系列（二）——cnn简单训练6个流程 - Snoopy_Dream - CSDN博客





2018年11月09日 12:06:41[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：126
所属专栏：[faster-rcnn.pytorch](https://blog.csdn.net/column/details/25674.html)









**目录**

[零、 函数总体说明](#%E9%9B%B6%E3%80%81%20%E5%87%BD%E6%95%B0%E6%80%BB%E4%BD%93%E8%AF%B4%E6%98%8E)

[一、数据导入](#%E4%B8%80%20%E3%80%81%E6%95%B0%E6%8D%AE%E5%AF%BC%E5%85%A5)

[1. torch.utils.data.Dataset基类 or torchvision.datasets.ImageFolder 将路径和标签变列表](#1.%20%E5%9F%BA%E7%B1%BBDataset%20or%20ImageFolder%20%E5%B0%86%E8%B7%AF%E5%BE%84%E5%92%8C%E6%A0%87%E7%AD%BE%E5%8F%98%E5%88%97%E8%A1%A8)

[2. torch.utils.data.DataLoader对图像和标签列表分别封装成一个Tensor](#2.%20torch.utils.data.DataLoader%E5%AF%B9%E5%9B%BE%E5%83%8F%E5%92%8C%E6%A0%87%E7%AD%BE%E5%88%97%E8%A1%A8%E5%88%86%E5%88%AB%E5%B0%81%E8%A3%85%E6%88%90%E4%B8%80%E4%B8%AATensor)

[3. 将Tensor数据类型封装成Variable数据类型。](#3.%20%E5%B0%86Tensor%E6%95%B0%E6%8D%AE%E7%B1%BB%E5%9E%8B%E5%B0%81%E8%A3%85%E6%88%90Variable%E6%95%B0%E6%8D%AE%E7%B1%BB%E5%9E%8B%E3%80%82)

[补充：Sample](#%E8%A1%A5%E5%85%85%EF%BC%9ASample)

[二、导入你的模型](#%E4%BA%8C%C2%A0%20%E3%80%81%E5%AF%BC%E5%85%A5%E4%BD%A0%E7%9A%84%E6%A8%A1%E5%9E%8B)

[三、定义损失函数criterion = nn.CrossEntropyLoss()](#%E4%B8%89%E3%80%81%E5%AE%9A%E4%B9%89%E6%8D%9F%E5%A4%B1%E5%87%BD%E6%95%B0)

[四、定义优化函数optimizer = optim.SGD(model.parameters(), lr=0.001, momentum=0.9)](#%E5%9B%9B%20%E3%80%81%E5%AE%9A%E4%B9%89%E4%BC%98%E5%8C%96%E5%87%BD%E6%95%B0)

[五、定义学习率的变化策略scheduler = lr_scheduler.StepLR(optimizer, step_size=7, gamma=0.1)](#%E4%BA%94%20%E3%80%81%20%E5%AE%9A%E4%B9%89%E5%AD%A6%E4%B9%A0%E7%8E%87%E7%9A%84%E5%8F%98%E5%8C%96%E7%AD%96%E7%95%A5)

[六、开始训练](#%E5%85%AD%20%E3%80%81%20%E5%BC%80%E5%A7%8B%E8%AE%AD%E7%BB%83)

[1. 更新下学习率 scheduler.step()【制定了学习率的变化策略的原因】](#1.%20%E6%9B%B4%E6%96%B0%E4%B8%8B%E5%AD%A6%E4%B9%A0%E7%8E%87%20scheduler.step()%E3%80%90%E5%88%B6%E5%AE%9A%E4%BA%86%E5%AD%A6%E4%B9%A0%E7%8E%87%E7%9A%84%E5%8F%98%E5%8C%96%E7%AD%96%E7%95%A5%E7%9A%84%E5%8E%9F%E5%9B%A0%E3%80%91)

[2. 设置模型状态为训练状态 model.train(True)](#2.%20%E8%AE%BE%E7%BD%AE%E6%A8%A1%E5%9E%8B%E7%8A%B6%E6%80%81%E4%B8%BA%E8%AE%AD%E7%BB%83%E7%8A%B6%E6%80%81%20model.train(True))

[3. 所有梯度置0 model.zero_grad()](#3.%20%E6%89%80%E6%9C%89%E6%A2%AF%E5%BA%A6%E7%BD%AE0%20optimizer.zero_grad())

[4. 网络的前向传播model(inputs)](#4.%20%E7%BD%91%E7%BB%9C%E7%9A%84%E5%89%8D%E5%90%91%E4%BC%A0%E6%92%ADmodel(inputs))

[5. 得到损失criterion(outputs, labels)](#5.%20%E5%BE%97%E5%88%B0%E6%8D%9F%E5%A4%B1criterion(outputs%2C%20labels))

[6. torch.max预测该样本属于哪个类别的信息](#6.%20torch.max%E9%A2%84%E6%B5%8B%E8%AF%A5%E6%A0%B7%E6%9C%AC%E5%B1%9E%E4%BA%8E%E5%93%AA%E4%B8%AA%E7%B1%BB%E5%88%AB%E7%9A%84%E4%BF%A1%E6%81%AF)

[7. optimizer.zero_grad()](#7.%20optimizer.zero_grad())

[8. loss.backward()回传损失，过程中会计算梯度](#7.%20loss.backward()%E5%9B%9E%E4%BC%A0%E6%8D%9F%E5%A4%B1%EF%BC%8C%E8%BF%87%E7%A8%8B%E4%B8%AD%E4%BC%9A%E8%AE%A1%E7%AE%97%E6%A2%AF%E5%BA%A6)

[9. 根据这些梯度更新参数 optimizer.step()](#8.%20%E6%A0%B9%E6%8D%AE%E8%BF%99%E4%BA%9B%E6%A2%AF%E5%BA%A6%E6%9B%B4%E6%96%B0%E5%8F%82%E6%95%B0%20optimizer.step())

[0. 判断你是否有gpu可以用use_gpu = torch.cuda.is_available()](#0.%20%E5%88%A4%E6%96%AD%E4%BD%A0%E6%98%AF%E5%90%A6%E6%9C%89gpu%E5%8F%AF%E4%BB%A5%E7%94%A8use_gpu%20%3D%20torch.cuda.is_available())

## 零、 函数总体说明
- from torch.utils.data import **Dataset***所有数据读取的基类,自定义数据读取会用到，返回一个****列表。***然后用torch.utils.data.DataLoader封装成Tensor。最后变variable。
- from **torchvision.dataset** import** ImageFolde**r*：ImageFolder（root,transform,loader读片读取的方法）**读取 root/dog/xxx.png,返回一个****列表**,**列表中的每个值都是一个tuple，**每个tuple包含的是图像路径和标签信息**。所以需要转换为*DataLoader将其转换*tensor**，在后期转成*Variable数据类型*作为网络输入。**注意：**list是不能作为模型输入的，因此在PyTorch中需要用另一个类来封装list，那是：torch.utils.data.DataLoader*
- from torch.utils.data import** DataLoader ***DataLoader(**dataset** or ImageFolder**,batchsize,shuffle)将Dataset可以batch、shuffle、多线程读取，**torch.utils.data.DataLoader类可以将list类型的输入数据封装成Tensor数据格式，以备模型使用。**,返回****两个**Tensor，** data = inputs, labels ***
- from torchvision import transfrom *预处理模块*

## 一、数据导入

这里采用官方写好的torchvision.datasets.ImageFolder接口实现数据导入。这个接口需要你提供图像所在的文件夹，就是下面的data_dir=‘/data’这句，然后对于一个分类问题，这里data_dir目录下一般包括两个文件夹：train和val，每个文件件下面包含N个子文件夹，N是你的分类类别数，且每个子文件夹里存放的就是这个类别的图像。这样torchvision.datasets.ImageFolder就会返回一个列表（比如下面代码中的image_datasets[‘train’]或者image_datasets[‘val]），列表中的每个值都是一个tuple，每个tuple包含图像和标签信息。

data_dir = '/data'

**[自定义数据读取+多gpu运算](https://blog.csdn.net/u014380165/article/details/78634829)**

### **1. **torch.utils.data.**Dataset基类 or **torchvision.datasets**.ImageFolde**r 将路径和标签变列表

根据训练集和验证集预处理的不同和路径的不同，设置一个字典。

image_datasets = *{**x***: ImageFolder**(**

                    os.path.join(data_dir,** x**)**,**

                    data_transforms[**x])，#**data_transforms**是一个字典**
***for x in ['train', 'val']***}

变成一行，看for生成的字典：

*image_datasets = {**x**: ImageFolder****(****os.path.join**(**data_dir,** x****)****,**data_transforms[**x**]****),******for x in ['train', 'val']**}*

data_transforms = {

    'train': transforms.Compose([ * #torchvision.transforms.Compose叠加transforms操作*

        transforms.RandomSizedCrop(224),#*输入对象都是**PIL Image**，也就是用python的PIL库读进来的图像内容*

        transforms.RandomHorizontalFlip(),#*输入对象都是**PIL Image**，也就是用python的PIL库读进来的图像内容*

        transforms.ToTensor()*,#生成Tensor*

        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])#*输入对象 ****Tensor***

    ]),

    'val': transforms.Compose([

        transforms.Scale(256),*#transforms.Scale(256)其实就是resize操作，目前已经被transforms.Resize类取代了。*

        transforms.CenterCrop(224),

        transforms.ToTensor(),

        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])

    ]),

}

torch.utils.data.**Dataset**

```python
class TensorDataset(Dataset): """Dataset wrapping data and target tensors.

    Each sample will be retrieved by indexing both tensors along the first
    dimension.

    Arguments:
        data_tensor (Tensor): contains sample data.
        target_tensor (Tensor): contains sample targets (labels).
    """ 
    def __init__(self, data_tensor, target_tensor): 
        assert data_tensor.size(0) == target_tensor.size(0) 
        self.data_tensor = data_tensor 
        self.target_tensor = target_tensor 
    def __getitem__(self, index): 
        return self.data_tensor[index], self.target_tensor[index] 
    def __len__(self): return self.data_tensor.size(0)
```

### 2. torch.utils.data.DataLoader***对图像和标签列表分别封装成一个Tensor***

前面torchvision.datasets.ImageFolder只是返回list，list是不能作为模型输入的，因此在PyTorch中需要用另一个类来封装list，那就是：torch.utils.data.DataLoader。**torch.utils.data.DataLoader类可以将list类型的输入数据封装成Tensor数据格式，以备模型使用。*注意，这里是对图像和标签分别封装成一个Tensor。***这里要提到另一个很重要的类：torch.utils.data.Dataset，这是一个抽象类，在pytorch中所有和数据相关的类都要继承这个类来实现。比如前面说的torchvision.datasets.ImageFolder类是这样的，以及这里的torch.util.data.DataLoader类也是这样的。自定义一个类读取数据，自定义的这个类必须继承自torch.utils.data.Dataset这个基类，最后同样用torch.utils.data.DataLoader封装成Tensor。

dataloders = {x: torch.utils.data.DataLoader(image_datasets[x], *#**加入ImageFolder输出的图片的名字***

                                            batch_size=4,

                                            shuffle=True,

                                            num_workers=4)#

                                            for x in ['train', 'val']}

**class torch.utils.data.DataLoader**(dataset, batch_size=1, shuffle=False, sampler=None, batch_sampler=None, num_workers=0, collate_fn=, pin_memory=False, drop_last=False, timeout=0, worker_init_fn=None)

参数:
- dataset (Dataset): 加载数据的数据集
-  batch_size (int, optional): 每批加载多少个样本
-  shuffle (bool, optional): 设置为“真”时,在每个epoch对数据打乱.（默认：False）
-  sampler (Sampler, optional): 定义从数据集中提取样本的策略,返回一个样本
-  batch_sampler (Sampler, optional): like sampler, but returns a batch of indices at a time 返回一批样本. 与atch_size, shuffle, sampler和 drop_last互斥.
-  num_workers (int, optional): 用于加载数据的子进程数。0表示数据将在主进程中加载​​。（默认：0）
-  collate_fn (callable, optional): 合并样本列表以形成一个 mini-batch.  # callable可调用对象
-  pin_memory (bool, optional): 如果为 True, 数据加载器会将张量复制到 CUDA 固定内存中,然后再返回它们.
-  drop_last (bool, optional): 设定为 True 如果数据集大小不能被批量大小整除的时候, 将丢掉最后一个不完整的batch,(默认：False).
-  timeout (numeric, optional): 如果为正值，则为从工作人员收集批次的超时值。应始终是非负的。（默认：0）
-  worker_init_fn (callable, optional): If not None, this will be called on each worker subprocess with the worker id (an int in ``[0, num_workers - 1]``) as input, after seeding and before data loading. (default: None)．


### 3. 将***Tensor数据***类型封装成***Variable数据类型***。

**来看下面这段代码。**dataloaders是一个字典，dataloders[‘train’]存的就是训练的数据，下面这个for循环就是从dataloders[‘train’]中读取batch_size个数据，batch_size在前面生成dataloaders的时候就设置了。因此这个*data里面包含图像数据（inputs）这个Tensor和标签（labels）这个Tensor*。然后用torch.autograd.Variable将Tensor封装成模型真正可以用的Variable数据类型。


for data in dataloders['train']:***#dataloders迭代器***
***  inputs, labels = data**#每次取出batch_size个数据***

   if use_gpu:

       inputs = Variable(inputs.cuda())***#两个Variable***

       labels = Variable(labels.cuda())#

   else:

       inputs, labels = Variable(inputs), Variable(labels)

### 补充：Sample

class torch.utils.data.sampler.Sampler(data_source)

参数: data_source (Dataset) – dataset to sample from

作用: 创建一个采样器, class torch.utils.data.sampler.Sampler是所有的Sampler的基类, 其中,iter(self)函数来获取一个迭代器,对数据集中元素的索引进行迭代,len(self)方法返回迭代器中包含元素的长度.  

```python
class RandomSampler(Sampler): """Samples elements randomly, without replacement.

    Arguments:
        data_source (Dataset): dataset to sample from
    """ def __init__(self, data_source): self.data_source = data_source def __iter__(self): return iter(torch.randperm(len(self.data_source)).long()) def __len__(self): return len(self.data_source)
```

```python
class sampler(Sampler):
  def __init__(self, train_size, batch_size):
"""
返回一个所有的rand_num_view排序的迭代器，至于batchsize放到dataloader里面，这里面存着一个batch接着一个batch.

（1）batchsize：每批数据量的大小。DL通常用SGD的优化算法进行训练，也就是一次（1 个iteration）一起训练batchsize个样本，计算它们的平均损失函数值，来更新参数。

（2）iteration：1个iteration即迭代一次，也就是用batchsize个样本训练一次。

"""
    self.num_data = train_size # 训练大小
    self.num_per_batch = int(train_size / batch_size) #iteration 总数据/批处理大小
    self.batch_size = batch_size
    self.range = torch.arange(0,batch_size).view(1, batch_size).long()
    self.leftover_flag = False
    if train_size % batch_size:
      self.leftover = torch.arange(self.num_per_batch*batch_size, train_size).long()
      self.leftover_flag = True
  def __iter__(self):
   #torch.randperm(n, out=None) → LongTensor 给定参数n，返回一个从0 到n -1 的随机整数排列。
    rand_num = torch.randperm(self.num_per_batch).view(-1,1) * self.batch_size
    self.rand_num = rand_num.expand(self.num_per_batch, self.batch_size) + self.range

    self.rand_num_view = self.rand_num.view(-1)

    if self.leftover_flag:
      self.rand_num_view = torch.cat((self.rand_num_view, self.leftover),0)

    return iter(self.rand_num_view)#返回一个所有的rand_num_view排序的迭代器

  def __len__(self):
    return self.num_data
```



## 二、导入你的模型

封装好了数据后，就可以作为模型的输入了。所以要先导入你的模型。在PyTorch中已经默认为大家准备了一些常用的网络结构，比如分类中的VGG，ResNet，DenseNet等等，可以用*torchvision.models*模块来导入。比如用torchvision.models.resnet18(*pretrained=True)*来导入ResNet18网络，同时指明导入的是*已经预训练过的网络*。因为预训练网络一般是在1000类的ImageNet数据集上进行的，所以要迁移到你自己数据集的2分类，需要替换最后的全连接层为你所需要的输出。因此下面这三行代码进行的就是用models模块导入resnet18网络，然后获取全连接层的输入channel个数，用这个channel个数和你要做的分类类别数（这里是2）替换原来模型中的全连接层。这样网络结果也准备好。

*model = models.resnet18(pretrained=True)num_ftrs = model**.fc.in_features#获取全连接层的输入channel个数**model.fc = nn.Linear(num_ftrs, 2)*

## 三、定义损失函数*criterion = nn.CrossEntropyLoss()*

在PyTorch中采用*torch.nn*模块来定义网络的所有层，比如卷积、降采样、损失层等等，这里采用交叉熵函数，因此可以这样定义：

*criterion = nn.CrossEntropyLoss()*

## 四、定义优化函数*optimizer = optim.SGD(model.parameters(), lr=0.001, momentum=0.9)*

在PyTorch中是通过*torch.optim*模块来实现的。另外这里虽然写的是SGD，但是因为有momentum，所以是Adam的优化方式。这个类的输入包括需要优化的参数：model.parameters()，学习率，还有Adam相关的momentum参数。现在很多优化方式的默认定义形式就是这样的。

*optimizer = optim.SGD(model.parameters(), lr=0.001, momentum=0.9)*



## 五、定义学习率的变化策略*scheduler = lr_scheduler.StepLR(optimizer, step_size=7, gamma=0.1)*

这里采用的是*torch.optim.lr_scheduler*模块的StepLR类，表示每隔step_size个epoch就将学习率降为原来的gamma倍。

*scheduler = lr_scheduler.StepLR(optimizer, step_size=7, gamma=0.1)*



## 六、开始训练

### 1. 更新下学习率 *scheduler**.step()【*制定了学习率的变化策略的原因*】*

这是因为我们前面制定了学习率的变化策略，所以在每个epoch开始时都要更新下：

*scheduler**.step()*

### 2. 设置模型状态为训练状态 *model.train(True)*

*model.train(True)*

### 3. 所有梯度置0 model.zero_grad()

# Zero the gradients before running the backward pass.

    model.zero_grad()

model.zero_grad()

### 4. 网络的前向传播*model(inputs)*

*outputs = model(inputs)*

### *5. *得到损失*criterion(outputs, labels)*

然后将输出的outputs和原来导入的labels作为loss函数的输入就可以得到损失了：

### *loss = criterion(outputs, labels)*

### *6. torch.max*预测该样本属于哪个类别的信息

输出的outputs也是torch.autograd.Variable格式，得到输出后（网络的全连接层的输出）还希望能到到模型预测该样本属于哪个类别的信息，这里采用torch.max。torch.max()的第一个输入是tensor格式，所以用outputs.data而不是outputs作为输入；第二个参数1是代表dim的意思，也就是取每一行的最大值，其实就是我们常见的取概率最大的那个index；第三个参数loss也是torch.autograd.Variable格式。

* _, preds = torch.max(**outputs.data**, 1)*

`torch.max()`返回的是两个`Variable`，第一个`Variable`存的是最大值，第二个存的是其对应的位置索引index。这里我们想要得到的是索引，所以后面用`[1]`。

### 7.梯度置0 *optimizer**.zero_grad()*

# Before the backward pass, use the optimizer object to zero all of the

# gradients for the variables it will update (which are the learnable weights

# of the model)

根据pytorch中的backward()函数的计算，当网络参量进行反馈时，梯度是被积累的而不是被替换掉；但是在每一个batch时毫无疑问并不需要将两个batch的梯度混合起来累积，因此这里就需要每个batch设置一遍zero_grad 了。

### 8. *loss.backward()*回传损失，***过程中会计算梯度***

计算得到loss后就要回传损失。要注意的是这是在训练的时候才会有的操作，测试时候只有forward过程。

*loss.backward()*

### *9. ****根据这些梯度更新参数 optimizer.step()***

***回传损失过程中会计算梯度，然后需要根据这些梯度更新参数，optimizer.step()就是用来更新参数的。optimizer.step()后，你就可以从optimizer.param_groups[0][‘params’]里面看到各个层的梯度和权值信息。***

*optimizer.step()*

这样一个batch数据的训练就结束了！当你不断重复这样的训练过程，最终就可以达到你想要的结果了。

### 0. 判断你是否有gpu可以用*use_gpu = torch.cuda.is_available()*

另外如果你有gpu可用，那么包括你的数据和模型都可以在gpu上操作，这在PyTorch中也非常简单。判断你是否有gpu可以用可以通过下面这行代码，如果有，则use_gpu是true。

*use_gpu = torch.cuda.is_available()*





完整代码请移步：[Github](https://github.com/miraclewkf/ImageClassification-PyTorch/blob/master/level1/train.py)






整理自：https://blog.csdn.net/u014380165/article/details/78525273  




