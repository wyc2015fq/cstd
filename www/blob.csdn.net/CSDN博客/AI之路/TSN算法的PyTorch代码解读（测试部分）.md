# TSN算法的PyTorch代码解读（测试部分） - AI之路 - CSDN博客





2018年01月14日 19:13:26[AI之路](https://me.csdn.net/u014380165)阅读数：3541








**这篇博客介绍TSN算法的PyTorch代码的测试部分，建议先看训练部分的代码解读：[TSN算法的PyTorch代码解读（训练部分）](http://blog.csdn.net/u014380165/article/details/79058147)，test_moels.py是测试模型的入口。**

前面模块导入和命令行参数配置方面和训练代码类似，不细讲。

```
import argparse
import time

import numpy as np
import torch.nn.parallel
import torch.optim
from sklearn.metrics import confusion_matrix

from dataset import TSNDataSet
from models import TSN
from transforms import *
from ops import ConsensusModule

# options
parser = argparse.ArgumentParser(
    description="Standard video-level testing")
parser.add_argument('dataset', type=str, choices=['ucf101', 'hmdb51', 'kinetics'])
parser.add_argument('modality', type=str, choices=['RGB', 'Flow', 'RGBDiff'])
parser.add_argument('test_list', type=str)
parser.add_argument('weights', type=str)
parser.add_argument('--arch', type=str, default="resnet101")
parser.add_argument('--save_scores', type=str, default=None)
parser.add_argument('--test_segments', type=int, default=25)
parser.add_argument('--max_num', type=int, default=-1)
parser.add_argument('--test_crops', type=int, default=10)
parser.add_argument('--input_size', type=int, default=224)
parser.add_argument('--crop_fusion_type', type=str, default='avg',
                    choices=['avg', 'max', 'topk'])
parser.add_argument('--k', type=int, default=3)
parser.add_argument('--dropout', type=float, default=0.7)
parser.add_argument('-j', '--workers', default=4, type=int, metavar='N',
                    help='number of data loading workers (default: 4)')
parser.add_argument('--gpus', nargs='+', type=int, default=None)
parser.add_argument('--flow_prefix', type=str, default='')

args = parser.parse_args()
```

接下来先是根据数据集来确定类别数。**然后通过models.py脚本中的TSN类来导入网络结构**。另外如果想查看得到的网络net的各层信息，可以通过net.state_dict()来查看。checkpoint = torch.load(args.weights)是导入预训练的模型，在PyTorch中，导入模型都是采用torch.load()接口实现，输入args.weights就是.pth文件，也就是预训练模型。`base_dict = {'.'.join(k.split('.')[1:]): v for k,v in list(checkpoint.state_dict().items())}`就是读取预训练模型的层和具体参数并存到base_dict这个字典中。net.load_state_dict(base_dict)就是通过调用torch.nn.Module类的load_state_dict方法，达到用预训练模型初始化net网络的过程。需要注意的是load_state_dict方法还有一个输入：strict，如果该参数为True，就表示网络结构的层信息要和预训练模型的层信息严格相等，反之亦然，该参数默认是True。那么什么时候会用到False呢？就是当你只想用预训练网络初始化你的网络的部分层参数或者说你的预训练网络的层信息和你要被初始化的网络的层信息不完全一致，那样就只会初始化层信息相同的层。

```
if args.dataset == 'ucf101':
    num_class = 101
elif args.dataset == 'hmdb51':
    num_class = 51
elif args.dataset == 'kinetics':
    num_class = 400
else:
    raise ValueError('Unknown dataset '+args.dataset)

net = TSN(num_class, 1, args.modality,
          base_model=args.arch,
          consensus_type=args.crop_fusion_type,
          dropout=args.dropout)

checkpoint = torch.load(args.weights)

base_dict = {'.'.join(k.split('.')[1:]): v for k,v in list(checkpoint.state_dict().items())}
net.load_state_dict(base_dict)
```

**接下来关于args.test_crops的条件语句是用来对数据做不同的crop操作：简单crop操作和重复采样的crop操作**。如果args.test_crops等于1，就先resize到指定尺寸（比如从400resize到256），然后再做center crop操作，最后得到的是net.input_size的尺寸（比如224），注意这里一张图片做完这些crop操作后输出还是一张图片。如果args.test_crops等于10，那么就调用该项目下的transforms.py脚本中的GroupOverSample类进行重复采样的crop操作，最终一张图像得到10张crop的结果，后面会详细介绍GroupOverSample这个类。接下来的数据读取部分和训练时候类似，需要注意的是：1、num_segments的参数默认是25，比训练时候要多的多。2、test_mode=True，所以在调用TSNDataSet类的`__getitem__`方法时和训练时候有些差别。

```python
if args.test_crops == 1:
    cropping = torchvision.transforms.Compose([
        GroupScale(net.scale_size),
        GroupCenterCrop(net.input_size),
    ])
elif args.test_crops == 10:
    cropping = torchvision.transforms.Compose([
        GroupOverSample(net.input_size, net.scale_size)
    ])
else:
    raise ValueError("Only 1 and 10 crops are supported while we got {}".format(args.test_crops))

data_loader = torch.utils.data.DataLoader(
        TSNDataSet("", args.test_list, num_segments=args.test_segments,
                   new_length=1 if args.modality == "RGB" else 5,
                   modality=args.modality,
                   image_tmpl="img_{:05d}.jpg" if args.modality in ['RGB', 'RGBDiff'] else args.flow_prefix+"{}_{:05d}.jpg",
                   test_mode=True,
                   transform=torchvision.transforms.Compose([
                       cropping,
                       Stack(roll=args.arch == 'BNInception'),
                       ToTorchFormatTensor(div=args.arch != 'BNInception'),
                       GroupNormalize(net.input_mean, net.input_std),
                   ])),
        batch_size=1, shuffle=False,
        num_workers=args.workers * 2, pin_memory=True)
```

**transforms.py脚本中的GroupOverSample类**。首先`__init__`中的GroupScale类也是在transforms.py中定义的，其实是对输入的n张图像都做torchvision.transforms.Scale操作，也就是resize到指定尺寸。GroupMultiScaleCrop.fill_fix_offset返回的offsets是一个长度为5的列表，每个值都是一个tuple，其中前4个是四个点坐标，最后一个是中心点坐标，目的是以这5个点为左上角坐标时可以在原图的四个角和中心部分crop出指定尺寸的图，后面有例子介绍。crop = img.crop((o_w, o_h, o_w + crop_w, o_h + crop_h))是按照crop_w*crop_h的大小去crop原图像，这里采用的是224*224。flip_crop = crop.copy().transpose(Image.FLIP_LEFT_RIGHT)是对crop得到的图像做左右翻转。最后把未翻转的和翻转后的列表合并，这样一张输入图像就可以得到10张输出了（5张crop，5张crop加翻转）。举个例子，假设image_w=340，image_h=256，crop_w=224，crop_h=224，那么offsets就是[(0,0),(116,0),(0,32),(116,32),(58,16)]，因此第一个crop的结果就是原图上左上角坐标为(0,0)，右下角坐标为(224,224)的图，这也就是原图的左上角部分图；第二个crop的结果就是原图上左上角坐标为(116,0)，右下角坐标为(340,224)的图，这也就是原图的右上角部分图，其他依次类推分别是原图的左下角部分图和右下角部分图，最后一个是原图正中央crop出来的224*224图。这就是论文中说的corner crop，而且是4个corner和1个center。

```python
class GroupOverSample(object):
    def __init__(self, crop_size, scale_size=None):
        self.crop_size = crop_size if not isinstance(crop_size, int) else (crop_size, crop_size)

        if scale_size is not None:
            self.scale_worker = GroupScale(scale_size)
        else:
            self.scale_worker = None

    def __call__(self, img_group):

        if self.scale_worker is not None:
            img_group = self.scale_worker(img_group)

        image_w, image_h = img_group[0].size
        crop_w, crop_h = self.crop_size

        offsets = GroupMultiScaleCrop.fill_fix_offset(False, image_w, image_h, crop_w, crop_h)
        oversample_group = list()
        for o_w, o_h in offsets:
            normal_group = list()
            flip_group = list()
            for i, img in enumerate(img_group):
                crop = img.crop((o_w, o_h, o_w + crop_w, o_h + crop_h))
                normal_group.append(crop)
                flip_crop = crop.copy().transpose(Image.FLIP_LEFT_RIGHT)

                if img.mode == 'L' and i % 2 == 0:
                    flip_group.append(ImageOps.invert(flip_crop))
                else:
                    flip_group.append(flip_crop)

            oversample_group.extend(normal_group)
            oversample_group.extend(flip_group)
        return oversample_group
```

接下来是设置GPU模式、设置模型为验证模式、初始化数据等。

```
if args.gpus is not None:
    devices = [args.gpus[i] for i in range(args.workers)]
else:
    devices = list(range(args.workers))

net = torch.nn.DataParallel(net.cuda(devices[0]), device_ids=devices)
net.eval()

data_gen = enumerate(data_loader)

total_num = len(data_loader.dataset)
output = []
```

开始循环读取数据，每执行一次循环表示读取一个video的数据。在循环中主要是调用eval_video函数来测试。预测结果和真实标签的结果都保存在output列表中。

```
proc_start_time = time.time()
max_num = args.max_num if args.max_num > 0 else len(data_loader.dataset)

for i, (data, label) in data_gen:
    if i >= max_num:
        break
    rst = eval_video((i, data, label))
    output.append(rst[1:])
    cnt_time = time.time() - proc_start_time
    print('video {} done, total {}/{}, average {} sec/video'.format(i, i+1,
                                                                    total_num,
                                                                    float(cnt_time) / (i+1)))
```

**eval_video函数是测试的主体**，当准备好了测试数据和模型后就通过这个函数进行预测。输入video_data是一个tuple：(i, data, label)。data.view(-1, length, data.size(2), data.size(3))是将原本输入为(1,3*args.test_crops*args.test_segments,224,224)变换到(args.test_crops*args.test_segments,3,224,224)，相当于batch size为args.test_crops*args.test_segments。然后用torch.autograd.Variable接口封装成Variable类型数据并作为模型的输入。net(input_var)得到的结果是Variable，如果要读取Tensor内容，需读取data变量，cpu()表示存储到cpu，numpy()表示Tensor转为numpy array，copy()表示拷贝。rst.reshape((num_crop, args.test_segments, num_class))表示将输入维数（二维）变化到指定维数（三维），mean(axis=0)表示对num_crop维度取均值，也就是原来对某帧图像的10张crop或clip图像做预测，最后是取这10张预测结果的均值作为该帧图像的结果。最后再执行一个reshape操作。最后返回的是3个值，分别表示video的index，预测结果和video的真实标签。

```python
def eval_video(video_data):
    i, data, label = video_data
    num_crop = args.test_crops

    if args.modality == 'RGB':
        length = 3
    elif args.modality == 'Flow':
        length = 10
    elif args.modality == 'RGBDiff':
        length = 18
    else:
        raise ValueError("Unknown modality "+args.modality)

    input_var = torch.autograd.Variable(data.view(-1, length, data.size(2), data.size(3)),
                                        volatile=True)
    rst = net(input_var).data.cpu().numpy().copy()
    return i, rst.reshape((num_crop, args.test_segments, num_class)).mean(axis=0).reshape(
        (args.test_segments, 1, num_class)
    ), label[0]
```

**接下来要计算video-level的预测结果**，这里从np.mean(x[0], axis=0)可以看出对args.test_segments帧图像的结果采取的也是均值方法来计算video-level的预测结果，然后通过np.argmax将概率最大的那个类别作为该video的预测类别。video_labels则是真实类别。cf = confusion_matrix(video_labels, video_pred).astype(float)是调用了混淆矩阵生成结果（numpy array），举个例子，y_true=[2,0,2,2,0,1]，y_pred=[0,0,2,2,0,2]，那么confusion_matrix(y_true, y_pred)的结果就是array([[2,0,0],[0,0,1],[1,0,2]])，每行表示真实类别，每列表示预测类别。因此cls_cnt = cf.sum(axis=1)表示每个真实类别有多少个video，cls_hit = np.diag(cf)就是将cf的对角线数据取出，表示每个类别的video中各预测对了多少个，因此cls_acc = cls_hit / cls_cnt就是每个类别的video预测准确率。np.mean(cls_acc)就是各类别的平均准确率。最后的if args.save_scores is not None:语句只是用来将预测结果保存成文件。

```
video_pred = [np.argmax(np.mean(x[0], axis=0)) for x in output]

video_labels = [x[1] for x in output]

cf = confusion_matrix(video_labels, video_pred).astype(float)

cls_cnt = cf.sum(axis=1)
cls_hit = np.diag(cf)

cls_acc = cls_hit / cls_cnt

print(cls_acc)

print('Accuracy {:.02f}%'.format(np.mean(cls_acc) * 100))

if args.save_scores is not None:

    # reorder before saving
    name_list = [x.strip().split()[0] for x in open(args.test_list)]

    order_dict = {e:i for i, e in enumerate(sorted(name_list))}

    reorder_output = [None] * len(output)
    reorder_label = [None] * len(output)

    for i in range(len(output)):
        idx = order_dict[name_list[i]]
        reorder_output[idx] = output[i]
        reorder_label[idx] = video_labels[i]

    np.savez(args.save_scores, scores=reorder_output, labels=reorder_label)
```



