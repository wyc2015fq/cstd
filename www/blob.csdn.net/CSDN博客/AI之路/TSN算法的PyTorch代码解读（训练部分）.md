# TSN算法的PyTorch代码解读（训练部分） - AI之路 - CSDN博客





2018年01月14日 18:31:04[AI之路](https://me.csdn.net/u014380165)阅读数：12770








这篇博客来读一读TSN算法的PyTorch代码，总体而言代码风格还是不错的，多读读优秀的代码对自身的提升还是有帮助的，另外因为代码内容较多，所以分训练和测试两篇介绍，这篇介绍训练代码，介绍顺序为代码运行顺序。TSN算法的介绍可以参考博客[TSN(Temporal Segment Networks)算法笔记](http://blog.csdn.net/u014380165/article/details/79029309)。 

论文：Temporal Segment Networks: Towards Good Practices for Deep Action Recognition 

代码地址：[https://github.com/yjxiong/tsn-pytorch](https://github.com/yjxiong/tsn-pytorch)
**项目结构：**

main.py是训练脚本 

test_models.py是测试脚本 

opts.py是参数配置脚本 

dataset.py是数据读取脚本 

models.py是网络结构构建脚本 

transforms.py是数据预处理相关的脚本 

tf_model_zoo文件夹关于导入模型结构的脚本
**main.py是训练模型的入口。**

首先是导入模块，其中比较重要的是导入模型：from models import TSN，导入配置的参数：from opts import parser。

```python
import argparse
import os
import time
import shutil
import torch
import torchvision
import torch.nn.parallel
import torch.backends.cudnn as cudnn
import torch.optim
from torch.nn.utils import clip_grad_norm

from dataset import TSNDataSet
from models import TSN
from transforms import *
from opts import parser

best_prec1 = 0
```

**main函数主要包含导入模型、数据准备、训练三个部分，接下来将按顺序介绍**。parser是在opts.py中定义的关于读取命令行参数的对象，然后通过from opts import parser导入的。`model = TSN(num_class, args.num_segments, args.modality,...，partial_bn=not args.no_partialbn)`这一行是导入模型操作，TSN类的定义在models.py脚本中。输入包含分类的类别数：num_class；args.num_segments表示把一个video分成多少份，对应论文中的K，默认K=3；采用哪种输入：args.modality，比如RGB表示常规图像，Flow表示optical flow等；采用哪种模型：args.arch，比如resnet101，BNInception等；不同输入snippet的融合方式：args.consensus_type，比如avg等；dropout参数：args.dropout。

```python
def main():
    global args, best_prec1
    args = parser.parse_args()

    if args.dataset == 'ucf101':
        num_class = 101
    elif args.dataset == 'hmdb51':
        num_class = 51
    elif args.dataset == 'kinetics':
        num_class = 400
    else:
        raise ValueError('Unknown dataset '+args.dataset)

    model = TSN(num_class, args.num_segments, args.modality,
                base_model=args.arch,
                consensus_type=args.consensus_type, dropout=args.dropout, partial_bn=not args.no_partialbn)
```

**TSN类（定义在models.py中）的初始化操作：`__init__`**，这里只列出主要的代码。new_length和输入数据类型相关。这里主要调用了该类的两个方法来完成初始化操作，一个是self._prepare_base_model(base_model)，通过调用TSN类的_prepare_base_model方法来导入模型。另一个是feature_dim = self._prepare_tsn(num_class)，通过调用TSN类的_prepare_tsn方法来得到。另外如果你的输入数据是optical flow或RGBDiff，那么还会对网络结构做修改，分别调用_construct_flow_model方法和_construct_diff_model方法来实现的，主要差别在第一个卷积层，因为该层的输入channel依据不同的输入类型而变化。接下来依次介绍这些方法。

```
class TSN(nn.Module):
    def __init__(self, num_class, num_segments, modality,
                 base_model='resnet101', new_length=None,
                 consensus_type='avg', before_softmax=True,
                 dropout=0.8,
                 crop_num=1, partial_bn=True):
        super(TSN, self).__init__()

        if new_length is None:
            self.new_length = 1 if modality == "RGB" else 5
        else:
            self.new_length = new_length

        self._prepare_base_model(base_model)

        feature_dim = self._prepare_tsn(num_class)

        if self.modality == 'Flow':
            print("Converting the ImageNet model to a flow init model")
            self.base_model = self._construct_flow_model(self.base_model)
            print("Done. Flow model ready...")
        elif self.modality == 'RGBDiff':
            print("Converting the ImageNet model to RGB+Diff init model")
            self.base_model = self._construct_diff_model(self.base_model)
            print("Done. RGBDiff model ready.")

        self.consensus = ConsensusModule(consensus_type)

        if not self.before_softmax:
            self.softmax = nn.Softmax()

        self._enable_pbn = partial_bn
        if partial_bn:
            self.partialBN(True)
```

**_prepare_base_model方法**的部分代码（以base_model为‘BNInception为例’）如下。getattr模块的使用：getattr(tf_model_zoo, base_model)()类似tf_model_zoo.BNInception()，因为要根据base_model的不同指定值来导入不同的网络，所以用getattr模块。导入模型之后就是一些常规的配置信息了。

```
elif base_model == 'BNInception':
            import tf_model_zoo
            self.base_model = getattr(tf_model_zoo, base_model)()
            self.base_model.last_layer_name = 'fc'
            self.input_size = 224
            self.input_mean = [104, 117, 128]
            self.input_std = [1]

            if self.modality == 'Flow':
                self.input_mean = [128]
            elif self.modality == 'RGBDiff':
                self.input_mean = self.input_mean * (1 + self.new_length)
```

**BNInception类**，定义在tf_model_zoo文件夹下的bninception文件夹下的pytorch_load.py中。前面当运行self.base_model = getattr(tf_model_zoo, base_model)()，且base_model是‘BNInception’的时候就会调用这个BNInception类的初始化函数`__init__`。manifest = yaml.load(open(model_path))是读进配置好的网络结构（.yml格式），返回的manifest是长度为3的字典，和.yml文件内容对应。其中manifest[‘layers’]是关于网络层的详细定义，其中的每个值表示一个层，每个层也是一个字典，包含数据流关系、名称和结构参数等信息。然后get_basic_layer函数是用来根据这些参数得到具体的网络层并保存相关信息。setattr(self, id, module)是将得到的层写入self的指定属性中，就是搭建层的过程。这样循环完所有层的配置信息后，就搭建好了整个网络。 

构建好了网络结构后，另外比较重要的是：`self.load_state_dict(torch.utils.model_zoo.load_url(weight_url))`这一行，可以分解一下，里面的torch.utils.model_zoo.load_url(weight_url)是通过提供的.pth文件的url地址来下载指定的.pth文件，在PyTorch中.pth文件就是模型的参数文件，如果你已经有合适的模型了且不想下载，那么可以通过torch.load(‘the/path/of/.pth’)导入，因为torch.utils.model_zoo.load_url方法最后返回的时候也是用torch.load接口封装成字典输出。self.load_state_dict()则是将导入的模型参数赋值到self中。因此不想下载的话可以用`checkpoint=torch.load('the/path/of/.pth')`和`self.load_state_dict(checkpoint)`两行代替`self.load_state_dict(torch.utils.model_zoo.load_url(weight_url))`。

```
class BNInception(nn.Module):
    def __init__(self, model_path='tf_model_zoo/bninception/bn_inception.yaml', num_classes=101,
                       weight_url='https://yjxiong.blob.core.windows.net/models/bn_inception-9f5701afb96c8044.pth'):
        super(BNInception, self).__init__()

        manifest = yaml.load(open(model_path))

        layers = manifest['layers']

        self._channel_dict = dict()

        self._op_list = list()
        for l in layers:
            out_var, op, in_var = parse_expr(l['expr'])
            if op != 'Concat':
                id, out_name, module, out_channel, in_name = get_basic_layer(l,
                                                                3 if len(self._channel_dict) == 0 else self._channel_dict[in_var[0]],
                                                                             conv_bias=True)

                self._channel_dict[out_name] = out_channel
                setattr(self, id, module)
                self._op_list.append((id, op, out_name, in_name))
            else:
                self._op_list.append((id, op, out_var[0], in_var))
                channel = sum([self._channel_dict[x] for x in in_var])
                self._channel_dict[out_var[0]] = channel

        self.load_state_dict(torch.utils.model_zoo.load_url(weight_url))
```

**_prepare_tsn方法**。feature_dim是网络最后一层的输入feature map的channel数。接下来如果有dropout层，那么添加一个dropout层后连一个全连接层，否则就直接连一个全连接层。setattr是torch.nn.Module类的一个方法，用来为输入的某个属性赋值，一般可以用来修改网络结构，以`setattr(self.base_model, self.base_model.last_layer_name, nn.Dropout(p=self.dropout))`为例，输入包含3个值，分别是基础网络，要赋值的属性名，要赋的值，一般而言setattr的用法都是这样。因此当这个setattr语句运行结束后，self.base_model.last_layer_name这一层就是nn.Dropout(p=self.dropout)。 

最后对全连接层的参数（weight）做一个0均值且指定标准差的初始化操作，参数（bias）初始化为0。getattr同样是torch.nn.Module类的一个方法，与为属性赋值方法setattr相比，getattr是获得属性值，一般可以用来获取网络结构相关的信息，以getattr(self.base_model, self.base_model.last_layer_name)为例，输入包含2个值，分别是基础网络和要获取值的属性名。

```
def _prepare_tsn(self, num_class):
        feature_dim = getattr(self.base_model, self.base_model.last_layer_name).in_features
        if self.dropout == 0:
            setattr(self.base_model, self.base_model.last_layer_name, nn.Linear(feature_dim, num_class))
            self.new_fc = None
        else:
            setattr(self.base_model, self.base_model.last_layer_name, nn.Dropout(p=self.dropout))
            self.new_fc = nn.Linear(feature_dim, num_class)

        std = 0.001
        if self.new_fc is None:
            normal(getattr(self.base_model, self.base_model.last_layer_name).weight, 0, std)
            constant(getattr(self.base_model, self.base_model.last_layer_name).bias, 0)
        else:
            normal(self.new_fc.weight, 0, std)
            constant(self.new_fc.bias, 0)
        return feature_dim
```

前面提到如果输入不是RGB，那么就要修改网络结构，这里以models.py脚本中TSN类的_construct_flow_model方法介绍对于optical flow类型的输入需要修改哪些网络结构。conv_layer是第一个卷积层的内容，params 包含weight和bias，kernel_size就是(64,3,7,7)，因为对于optical flow的输入，self.new_length设置为5，所以new_kernel_size是(63,10,7,7)。new_kernels是修改channel后的卷积核参数，主要是将原来的卷积核参数复制到新的卷积核。然后通过nn.Conv2d来重新构建卷积层。new_conv.weight.data = new_kernels是赋值过程。

```
def _construct_flow_model(self, base_model):
        # modify the convolution layers
        # Torch models are usually defined in a hierarchical way.
        # nn.modules.children() return all sub modules in a DFS manner
        modules = list(self.base_model.modules())
        first_conv_idx = list(filter(lambda x: isinstance(modules[x], nn.Conv2d), list(range(len(modules)))))[0]
        conv_layer = modules[first_conv_idx]
        container = modules[first_conv_idx - 1]

        # modify parameters, assume the first blob contains the convolution kernels
        params = [x.clone() for x in conv_layer.parameters()]
        kernel_size = params[0].size()
        new_kernel_size = kernel_size[:1] + (2 * self.new_length, ) + kernel_size[2:]
        new_kernels = params[0].data.mean(dim=1, keepdim=True).expand(new_kernel_size).contiguous()

        new_conv = nn.Conv2d(2 * self.new_length, conv_layer.out_channels,
                             conv_layer.kernel_size, conv_layer.stride, conv_layer.padding,
                             bias=True if len(params) == 2 else False)
        new_conv.weight.data = new_kernels
        if len(params) == 2:
            new_conv.bias.data = params[1].data # add bias if neccessary
        layer_name = list(container.state_dict().keys())[0][:-7] # remove .weight suffix to get the layer name

        # replace the first convlution layer
        setattr(container, layer_name, new_conv)
        return base_model
```

接着main函数的思路，前面这几行都是在TSN类中定义的变量或者方法，model = torch.nn.DataParallel(model, device_ids=args.gpus).cuda()是设置多GPU训练模型。args.resume这个参数主要是用来设置是否从断点处继续训练，比如原来训练模型训到一半停止了，希望继续从保存的最新epoch开始训练，因此args.resume要么是默认的None，要么就是你保存的模型文件（.pth）的路径。其中checkpoint = torch.load(args.resume)是用来导入已训练好的模型。model.load_state_dict(checkpoint[‘state_dict’])是完成导入模型的参数初始化model这个网络的过程，load_state_dict是torch.nn.Module类中重要的方法之一。

```
crop_size = model.crop_size
    scale_size = model.scale_size
    input_mean = model.input_mean
    input_std = model.input_std
    policies = model.get_optim_policies()
    train_augmentation = model.get_augmentation()

    model = torch.nn.DataParallel(model, device_ids=args.gpus).cuda()

    if args.resume:
        if os.path.isfile(args.resume):
            print(("=> loading checkpoint '{}'".format(args.resume)))
            checkpoint = torch.load(args.resume)
            args.start_epoch = checkpoint['epoch']
            best_prec1 = checkpoint['best_prec1']
            model.load_state_dict(checkpoint['state_dict'])
            print(("=> loaded checkpoint '{}' (epoch {})"
                  .format(args.evaluate, checkpoint['epoch'])))
        else:
            print(("=> no checkpoint found at '{}'".format(args.resume)))

    cudnn.benchmark = True
```

**介绍完第一部分模型导入后，接下来是main函数中的第二部分：数据导入**。首先是自定义的TSNDataSet类用来处理最原始的数据，返回的是torch.utils.data.Dataset类型，一般而言在PyTorch中自定义的数据读取类都要继承torch.utils.data.Dataset这个基类，比如此处的TSNDataSet类，然后通过重写初始化函数`__init__`和`__getitem__`方法来读取数据。torch.utils.data.Dataset类型的数据并不能作为模型的输入，还要通过torch.utils.data.DataLoader类进一步封装，这是因为数据读取类TSNDataSet返回两个值，第一个值是Tensor类型的数据，第二个值是int型的标签，而torch.utils.data.DataLoader类是将batch size个数据和标签分别封装成一个Tensor，从而组成一个长度为2的list。对于torch.utils.data.DataLoader类而言，最重要的输入就是TSNDataSet类的初始化结果，其他如batch size和shuffle参数是常用的。通过这两个类读取和封装数据，后续再转为Variable就能作为模型的输入了。

```python
# Data loading code
    if args.modality != 'RGBDiff':
        normalize = GroupNormalize(input_mean, input_std)
    else:
        normalize = IdentityTransform()

    if args.modality == 'RGB':
        data_length = 1
    elif args.modality in ['Flow', 'RGBDiff']:
        data_length = 5

    train_loader = torch.utils.data.DataLoader(
        TSNDataSet("", args.train_list, num_segments=args.num_segments,
                   new_length=data_length,
                   modality=args.modality,
                   image_tmpl="img_{:05d}.jpg" if args.modality in ["RGB", "RGBDiff"] else args.flow_prefix+"{}_{:05d}.jpg",
                   transform=torchvision.transforms.Compose([
                       train_augmentation,
                       Stack(roll=args.arch == 'BNInception'),
                       ToTorchFormatTensor(div=args.arch != 'BNInception'),
                       normalize,
                   ])),
        batch_size=args.batch_size, shuffle=True,
        num_workers=args.workers, pin_memory=True)

    val_loader = torch.utils.data.DataLoader(
        TSNDataSet("", args.val_list, num_segments=args.num_segments,
                   new_length=data_length,
                   modality=args.modality,
                   image_tmpl="img_{:05d}.jpg" if args.modality in ["RGB", "RGBDiff"] else args.flow_prefix+"{}_{:05d}.jpg",
                   random_shift=False,
                   transform=torchvision.transforms.Compose([
                       GroupScale(int(scale_size)),
                       GroupCenterCrop(crop_size),
                       Stack(roll=args.arch == 'BNInception'),
                       ToTorchFormatTensor(div=args.arch != 'BNInception'),
                       normalize,
                   ])),
        batch_size=args.batch_size, shuffle=False,
        num_workers=args.workers, pin_memory=True)
```

自定义数据读取相关类的时候需要继承torch.utils.data.Dataset这个基类。在TSNDataSet类的初始化函数`__init__`中最重要的是self._parse_list()，也就是调用了该类的_parse_list()方法。在该方法中，self.list_file就是训练或测试的列表文件（.txt文件），里面包含三列内容，用空格键分隔，第一列是video名，第二列是video的帧数，第三列是video的标签。VideoRecord这个类只是提供了一些简单的封装，用来返回关于数据的一些信息（比如帧路径、该视频包含多少帧、帧标签）。因此最后self.video_list的内容就是一个长度为训练数据数量的列表，列表中的每个值都是VideoRecord对象，该对象包含一个列表和3个属性，列表长度为3，分别是帧路径、该视频包含多少帧、帧标签，同样这三者也是三个属性的值。

```python
class TSNDataSet(data.Dataset):
    def __init__(self, root_path, list_file,
                 num_segments=3, new_length=1, modality='RGB',
                 image_tmpl='img_{:05d}.jpg', transform=None,
                 force_grayscale=False, random_shift=True, test_mode=False):

        self.root_path = root_path
        self.list_file = list_file
        self.num_segments = num_segments
        self.new_length = new_length
        self.modality = modality
        self.image_tmpl = image_tmpl
        self.transform = transform
        self.random_shift = random_shift
        self.test_mode = test_mode

        if self.modality == 'RGBDiff':
            self.new_length += 1# Diff needs one more image to calculate diff

        self._parse_list()

def _parse_list(self):
        self.video_list = [VideoRecord(x.strip().split(' ')) for x in open(self.list_file)]
```

**介绍完第二部分数据读取后，接下来就是main函数的第三部分：训练模型**。这里包括定义损失函数、优化函数、一些超参数设置等，然后训练模型并在指定epoch验证和保存模型。adjust_learning_rate(optimizer, epoch, args.lr_steps)是设置学习率变化策略，args.lr_steps是一个列表，里面的值表示到达多少个epoch的时候要改变学习率，在adjust_learning_rate函数中，默认是修改学习率的时候修改成当前的0.1倍。train(train_loader, model, criterion, optimizer, epoch)就是训练模型，输入包含训练数据、模型、损失函数、优化函数和要训练多少个epoch。最后的if语句是当训练epoch到达指定值的时候就进行一次模型验证和模型保存，args.eval_freq这个参数就是用来控制保存的epoch值。prec1 = validate(val_loader, model, criterion, (epoch + 1) * len(train_loader))就是用训练好的模型验证测试数据集。最后的save_checkpoint函数就是保存模型参数（model）和其他一些信息，这里我对源代码做了修改，希望有助于理解，该函数中主要就是调用torch.save(mode, save_path)来保存模型。模型训练函数train和模型验证函数validate函数是重点，后面详细介绍。

```python
# define loss function (criterion) and optimizer
    if args.loss_type == 'nll':
        criterion = torch.nn.CrossEntropyLoss().cuda()
    else:
        raise ValueError("Unknown loss type")

    for group in policies:
        print(('group: {} has {} params, lr_mult: {}, decay_mult: {}'.format(
            group['name'], len(group['params']), group['lr_mult'], group['decay_mult'])))

    optimizer = torch.optim.SGD(policies,
                                args.lr,
                                momentum=args.momentum,
                                weight_decay=args.weight_decay)

    if args.evaluate:
        validate(val_loader, model, criterion, 0)
        return

    for epoch in range(args.start_epoch, args.epochs):
        adjust_learning_rate(optimizer, epoch, args.lr_steps)

        # train for one epoch
        train(train_loader, model, criterion, optimizer, epoch)

        # evaluate on validation set
        if (epoch + 1) % args.eval_freq == 0 or epoch == args.epochs - 1:
            prec1 = validate(val_loader, model, criterion, (epoch + 1) * len(train_loader))

            # remember best prec@1 and save checkpoint
            is_best = prec1 > best_prec1
            best_prec1 = max(prec1, best_prec1)
            save_checkpoint(epoch=epoch + 1, arch=args.arch, state_dict=model, is_best=is_best)

def save_checkpoint(epoch, arch, model, is_best):
    filename = os.path.join(args.snapshot_pref, '_'.join((args.modality.lower(), 'arch:{}', 'epoch:{}', 'checkpoint.pth')).format(arch, epoch))
    torch.save(model, filename)
    if is_best:
        best_name = os.path.join(args.snapshot_pref, '_'.join((args.modality.lower(), 'arch:{}', 'epoch:{}', 'model_best.pth')).format(arch, epoch))
        shutil.copyfile(filename, best_name)
```

**train函数是模型训练的入口**。首先一些变量的更新采用自定义的AverageMeter类来管理，后面会介绍该类的定义。然后model.train()是设置为训练模式。 for i, (input, target) in enumerate(train_loader) 是数据迭代读取的循环函数，具体而言，当执行enumerate(train_loader)的时候，是先调用DataLoader类的`__iter__`方法，该方法里面再调用DataLoaderIter类的初始化操作`__init__`。而当执行for循环操作时，调用DataLoaderIter类的`__next__`方法，在该方法中通过self.collate_fn接口读取self.dataset数据时就会调用TSNDataSet类的`__getitem__`方法，从而完成数据的迭代读取。读取到数据后就将数据从Tensor转换成Variable格式，然后执行模型的前向计算：output = model(input_var)，得到的output就是batch size*class维度的Variable；损失函数计算： loss = criterion(output, target_var)；准确率计算： prec1, prec5 = accuracy(output.data, target, topk=(1,5))；模型参数更新等等。其中loss.backward()是损失回传， optimizer.step()是模型参数更新。

```
def train(train_loader, model, criterion, optimizer, epoch):
    batch_time = AverageMeter()
    data_time = AverageMeter()
    losses = AverageMeter()
    top1 = AverageMeter()
    top5 = AverageMeter()

    if args.no_partialbn:
        model.module.partialBN(False)
    else:
        model.module.partialBN(True)

    # switch to train mode
    model.train()

    end = time.time()
    for i, (input, target) in enumerate(train_loader):
        # measure data loading time
        data_time.update(time.time() - end)

        target = target.cuda(async=True)
        input_var = torch.autograd.Variable(input)
        target_var = torch.autograd.Variable(target)

        # compute output
        output = model(input_var)
        loss = criterion(output, target_var)

        # measure accuracy and record loss
        prec1, prec5 = accuracy(output.data, target, topk=(1,5))
        losses.update(loss.data[0], input.size(0))
        top1.update(prec1[0], input.size(0))
        top5.update(prec5[0], input.size(0))

        # compute gradient and do SGD step
        optimizer.zero_grad()

        loss.backward()

        if args.clip_gradient is not None:
            total_norm = clip_grad_norm(model.parameters(), args.clip_gradient)
            if total_norm > args.clip_gradient:
                print("clipping gradient: {} with coef {}".format(total_norm, args.clip_gradient / total_norm))

        optimizer.step()

        # measure elapsed time
        batch_time.update(time.time() - end)
        end = time.time()

        if i % args.print_freq == 0:
            print(('Epoch: [{0}][{1}/{2}], lr: {lr:.5f}\t'
                  'Time {batch_time.val:.3f} ({batch_time.avg:.3f})\t'
                  'Data {data_time.val:.3f} ({data_time.avg:.3f})\t'
                  'Loss {loss.val:.4f} ({loss.avg:.4f})\t'
                  'Prec@1 {top1.val:.3f} ({top1.avg:.3f})\t'
                  'Prec@5 {top5.val:.3f} ({top5.avg:.3f})'.format(
                   epoch, i, len(train_loader), batch_time=batch_time,
                   data_time=data_time, loss=losses, top1=top1, top5=top5, lr=optimizer.param_groups[-1]['lr'])))
```

**前面提到在train函数中采用自定义的AverageMeter类来管理一些变量的更新**。在初始化的时候就调用的重置方法reset。当调用该类对象的update方法的时候就会进行变量更新，当要读取某个变量的时候，可以通过对象.属性的方式来读取，比如在train函数中的top1.val读取top1准确率。

```python
class AverageMeter(object):
    """Computes and stores the average and current value"""
    def __init__(self):
        self.reset()

    def reset(self):
        self.val = 0
        self.avg = 0
        self.sum = 0
        self.count = 0

    def update(self, val, n=1):
        self.val = val
        self.sum += val * n
        self.count += n
        self.avg = self.sum / self.count
```

**前面提到在运行for i, (input, target) in enumerate(train_loader)的时候最终会调用TSNDataSet类的`__getitem__`方法，该方法就是用来返回具体数据的**。前面介绍过TSNDataSet类的初始化函数`__init__`，在那里面都是一些初始化或定义操作，真正的数据读取操作是在`__getitem__`方法中。在`__getitem__`方法中，record = self.video_list[index]得到的record就是一帧图像的信息，index是随机的，这个和前面数据读取中的shuffle参数对应。在训练的时候，self.test_mode是False，所以执行if语句，另外self.random_shift默认是True，所以最后执行的是segment_indices = self._sample_indices(record)。在测试的时候，会设置self.test_mode为True，这样的话就会执行segment_indices = self._get_test_indices(record)。最后再通过get方法返回。接下来分别介绍这三个方法。

```
def __getitem__(self, index):
        record = self.video_list[index]

        if not self.test_mode:
            segment_indices = self._sample_indices(record) if self.random_shift else self._get_val_indices(record)
        else:
            segment_indices = self._get_test_indices(record)
        return self.get(record, segment_indices)
```

**在TSNDataSet类的_sample_indices方法中**，average_duration表示某个视频分成self.num_segments份的时候每一份包含多少帧图像，因此只要该视频的总帧数大于等于self.num_segments，就会执行if average_duration > 0这个条件，在该条件语句下offsets的计算分成两部分，np.multiply(list(range(self.num_segments)), average_duration)相当于确定了self.num_segments个片段的区间，randint(average_duration, size=self.num_segments)则是生成了self.num_segments个范围在0到average_duration的数值，二者相加就相当于在这self.num_segments个片段中分别随机选择了一帧图像。因此在`__getitem__`方法中返回的segment_indices就是一个长度为self.num_segments的列表，表示帧的index。

```python
def _sample_indices(self, record):
        """
        :param record: VideoRecord
        :return: list
        """
        average_duration = (record.num_frames - self.new_length + 1) // self.num_segments
        if average_duration > 0:
            offsets = np.multiply(list(range(self.num_segments)), average_duration) + randint(average_duration, size=self.num_segments)
        elif record.num_frames > self.num_segments:
            offsets = np.sort(randint(record.num_frames - self.new_length + 1, size=self.num_segments))
        else:
            offsets = np.zeros((self.num_segments,))
        return offsets + 1
```

**在TSNDataSet类的_get_test_indices方法中**，就是将输入video按照相等帧数距离分成self.num_segments份，最终返回的offsets就是长度为self.num_segments的numpy array，表示从输入video中取哪些帧作为模型的输入。该方法是模型测试的时候才会调用。

```python
def _get_test_indices(self, record):
        tick = (record.num_frames - self.new_length + 1) / float(self.num_segments)
        offsets = np.array([int(tick / 2.0 + tick * x) for x in range(self.num_segments)])
        return offsets + 1
```

**在TSNDataSet类的get方法中**，先通过seg_imgs = self._load_image(record.path, p)来读取图像数据。_load_image方法中主要就是采用PIL库的Image模块来读取图像数据，该方法比较固定，一般作为当前类的一个方法比较合适，另外区分RGB和Flow数据读取的原因主要是图像名称不同。对于RGB或RGBDiff数据，返回的seg_imgs是一个长度为1的列表，对于Flow数据，返回的seg_imgs是一个长度为2的列表，然后将读取到的图像数据合并到images这个列表中。另外对于RGB而言，self.new_length是1，这样images的长度就是indices的长度；对于Flow而言，self.new_length是5，这样images的长度就是indices的长度乘以(5*2)。process_data = self.transform(images)将list类型的images封装成了Tensor，在训练的时候：对于RGB输入，这个Tensor的尺寸是(3*self.num_segments,224,224)，其中3表示3通道彩色；对于Flow输入，这个Tensor的尺寸是(self.num_segments*2*self.new_length,224,224)，其中第一维默认是30(3*2*5)。因此，最后get方法返回的是一个Tensor的数据和一个int的标签。

```python
def get(self, record, indices):
        images = list()
        for seg_ind in indices:
            p = int(seg_ind)
            for i in range(self.new_length):
                seg_imgs = self._load_image(record.path, p)
                images.extend(seg_imgs)
                if p < record.num_frames:
                    p += 1

        process_data = self.transform(images)
        return process_data, record.label

def _load_image(self, directory, idx):
        if self.modality == 'RGB' or self.modality == 'RGBDiff':
            return [Image.open(os.path.join(directory, self.image_tmpl.format(idx))).convert('RGB')]
        elif self.modality == 'Flow':
            x_img = Image.open(os.path.join(directory, self.image_tmpl.format('x', idx))).convert('L')
            y_img = Image.open(os.path.join(directory, self.image_tmpl.format('y', idx))).convert('L')

            return [x_img, y_img]
```

**验证函数validate**基本上和训练函数train类似，主要有几个不同点。先是model.eval()将模型设置为evaluate mode，其次没有optimizer.zero_grad()、loss.backward()、optimizer.step()等损失回传或梯度更新操作。

```
def validate(val_loader, model, criterion, iter, logger=None):
    batch_time = AverageMeter()
    losses = AverageMeter()
    top1 = AverageMeter()
    top5 = AverageMeter()

    # switch to evaluate mode
    model.eval()

    end = time.time()
    for i, (input, target) in enumerate(val_loader):
        target = target.cuda(async=True)
        input_var = torch.autograd.Variable(input, volatile=True)
        target_var = torch.autograd.Variable(target, volatile=True)

        # compute output
        output = model(input_var)
        loss = criterion(output, target_var)

        # measure accuracy and record loss
        prec1, prec5 = accuracy(output.data, target, topk=(1,5))

        losses.update(loss.data[0], input.size(0))
        top1.update(prec1[0], input.size(0))
        top5.update(prec5[0], input.size(0))

        # measure elapsed time
        batch_time.update(time.time() - end)
        end = time.time()

        if i % args.print_freq == 0:
            print(('Test: [{0}/{1}]\t'
                  'Time {batch_time.val:.3f} ({batch_time.avg:.3f})\t'
                  'Loss {loss.val:.4f} ({loss.avg:.4f})\t'
                  'Prec@1 {top1.val:.3f} ({top1.avg:.3f})\t'
                  'Prec@5 {top5.val:.3f} ({top5.avg:.3f})'.format(
                   i, len(val_loader), batch_time=batch_time, loss=losses,
                   top1=top1, top5=top5)))

    print(('Testing Results: Prec@1 {top1.avg:.3f} Prec@5 {top5.avg:.3f} Loss {loss.avg:.5f}'
          .format(top1=top1, top5=top5, loss=losses)))

    return top1.avg
```

**准确率计算函数**。输入output是模型预测的结果，尺寸为batch size*num class；target是真实标签，长度为batch size。这二者都是Tensor类型，具体而言前者是Float Tensor，后者是Long Tensor。batch_size = target.size(0)是读取batch size值。 _, pred = output.topk(maxk, 1, True, True)这里调用了PyTorch中Tensor的topk方法，第一个输入maxk表示你要计算的是top maxk的结果；第二个输入1表示dim，即按行计算（dim=1）；第三个输入True完整的是largest=True，表示返回的是top maxk个最大值；第四个输入True完整的是sorted=True，表示返回排序的结果，主要是因为后面要基于这个top maxk的结果计算top 1。target.view(1, -1).expand_as(pred)先将target的尺寸规范到1*batch size，然后将维度扩充为pred相同的维度，也就是maxk*batch size，比如5*batch size，然后调用eq方法计算两个Tensor矩阵相同元素情况，得到的correct是同等维度的ByteTensor矩阵，1值表示相等，0值表示不相等。correct_k = correct[:k].view(-1).float().sum(0)通过k值来决定是计算top k的准确率，sum(0)表示按照dim 0维度计算和，最后都添加到res列表中并返回。

```python
def accuracy(output, target, topk=(1,)):
    """Computes the precision@k for the specified values of k"""
    maxk = max(topk)
    batch_size = target.size(0)

    _, pred = output.topk(maxk, 1, True, True)
    pred = pred.t()
    correct = pred.eq(target.view(1, -1).expand_as(pred))

    res = []
    for k in topk:
        correct_k = correct[:k].view(-1).float().sum(0)
        res.append(correct_k.mul_(100.0 / batch_size))
    return res
```





