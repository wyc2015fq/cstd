# PyTorch faster_rcnn之一源码解读一 - wsp_1138886114的博客 - CSDN博客





2019年03月10日 11:14:14[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：141











### 文章目录
- [数据预处理](#_1)
- [1. data/dataset.py文件](#1_datadatasetpy_29)
- [2. data/util.py文件](#2_datautilpy_145)
- [3. data/voc_dataset.py](#3_datavoc_datasetpy_303)




### 数据预处理

dataset.py文件下的方法关系如下


































dataset.py

class Dataset

class Testdataset

voc_dataset.VOCBboxDataset

class Transform

voc_dataset.VOCBboxDataset

class Transform

read_label

read_image

progress

util.resize_bbox

util.random_flip

util.flip_bbox

caffe_pretrain

pytorch_pretrain

caffe_normalize

pytorch_normalze


##### 1. data/dataset.py文件
- 函数**inverse_normalize**实现对caffe与torchvision版本的去正则化。因为可以利用caffe版本的vgg预训练权重，也可利用torchvision版本的预训练权重。只不过后者结果略微逊色于前者。
- 函数**pytorch_normalze**实现对pytorch模型输入图像的标准化：由【0，255】的RGB转为【0，1】的RGB再正则化为【-1，1】的RGB。
- 函数**caffe_normalze**实现对caffe模型输入图像的标准化：由【0，255】的RGB转为【0，1】的RGB再正则化为【-125，125】的BGR。
- 函数**preprocess**实现对图像的预处理：由**read_image**函数读入的图像为CHW的【0，255】格式，这里首先除以255， 再按照论文长边不超1000，短边不超600。按此比例缩放。然后调用pytorch_normalze或者caffe_normalze对图像进行正则化。
- 类**Transform**实现了预处理，定义了__call__方法，在__call__方法中利用函数**preprocess**对图像预处理，并将bbox按照图像缩放的尺度等比例缩放。然后随机对图像与bbox同时进行水平翻转。
- 类**Dataset**实现对训练集样本的生成， 即trainval。__getitem__方法利用VOCBboxDataset类来生成一张训练图片，并调用**Trandform**类处理。返回处理后的图像，bbox，label，scale。
- 类**TestDataset**实现对测试机样本的生成，即test。__getitem__方法利用VOCBboxDataset类来生成一张测试图片，不同于训练的是调用**preprocess**函数处理。也即没有对bbox进行相应resize，而是返回处理前的图像尺寸。

```python
"""
# 如果采用caffe预训练模型（caffe_normalize之前有减均值预处理，现在还原回去。）
# pytorch_normalze中标准化为channel=（channel-mean）/std,转换为[-1,1]，现在乘以标准差加上均值还原回去，转换为0-255
"""
def inverse_normalize(img): 
	"""
	img维度[[B,G,R],H,W]
	反归一化（pytorch采用RGB[0-1]图片-->caffe采用BGR[0-255]图片）
	"""
    if opt.caffe_pretrain:   
        img = img + (np.array([122.7717, 115.9465, 102.9801]).reshape(3, 1, 1))
        return img[::-1, :, :]
    return (img * 0.225 + 0.45).clip(min=0, max=1) * 255


def pytorch_normalze(img): 
    """
    采用pytorch预训练模型对图片预处理，输入img为[0-1]
    https://github.com/pytorch/vision/issues/223
    return appr -1~1 RGB
    """
    normalize = tvtsf.Normalize(mean=[0.485, 0.456, 0.406],
                                std=[0.229, 0.224, 0.225])
    img = normalize(t.from_numpy(img))
    return img.numpy()


def caffe_normalize(img): 
    """
    采用caffe预训练模型时对输入图像标准化，输入img为[0-1]
    return appr -125-125 BGR
    """
    img = img[[2, 1, 0], :, :]  # RGB-BGR
    img = img * 255
    mean = np.array([122.7717, 115.9465, 102.9801]).reshape(3, 1, 1)
    img = (img - mean).astype(np.float32, copy=True)
    return img


def preprocess(img, min_size=600, max_size=1000): # 输入的img为0-255,长边不超1000，短边不超600。按此比例缩放
    C, H, W = img.shape
    scale1 = min_size / min(H, W)
    scale2 = max_size / max(H, W)
    scale = min(scale1, scale2)
    img = img / 255.
    img = sktsf.resize(img, (C, H * scale, W * scale), mode='reflect',anti_aliasing=False)
    # both the longer and shorter should be less than
    # max_size and min_size
    if opt.caffe_pretrain:
        normalize = caffe_normalize
    else:
        normalize = pytorch_normalze
    return normalize(img)


class Transform(object):
    def __init__(self, min_size=600, max_size=1000):
        self.min_size = min_size
        self.max_size = max_size

    def __call__(self, in_data):
        img, bbox, label = in_data
        _, H, W = img.shape
        img = preprocess(img, self.min_size, self.max_size) # 调用preprocess函数：图像等比例缩放
        _, o_H, o_W = img.shape
        scale = o_H / H
        bbox = util.resize_bbox(bbox, (H, W), (o_H, o_W)) # bbox按照与原图等比例缩放

        img, params = util.random_flip(
            img, x_random=True, return_param=True) #将图片进行随机水平翻转
        bbox = util.flip_bbox(
            bbox, (o_H, o_W), x_flip=params['x_flip'])  # 同样将bbox进行与对应图片水平翻转
        return img, bbox, label, scale


class Dataset:                                           # 训练集样本的生成
    def __init__(self, opt):
        self.opt = opt
        self.db = VOCBboxDataset(opt.voc_data_dir)       # 实例化类
        self.tsf = Transform(opt.min_size, opt.max_size) # #实例化类

    def __getitem__(self, idx):
        ori_img, bbox, label, difficult = self.db.get_example(idx)

        img, bbox, label, scale = self.tsf((ori_img, bbox, label))
        # TODO: check whose stride is negative to fix this instead copy all
        # some of the strides of a given numpy array are negative.
        return img.copy(), bbox.copy(), label.copy(), scale

    def __len__(self):
        return len(self.db)


class TestDataset:   # 测试集样本的生成
    def __init__(self, opt, split='test', use_difficult=True):
        self.opt = opt
        self.db = VOCBboxDataset(opt.voc_data_dir, split=split, use_difficult=use_difficult)

    def __getitem__(self, idx):
        ori_img, bbox, label, difficult = self.db.get_example(idx)
        img = preprocess(ori_img)
        return img, ori_img.shape[1:], bbox, label, difficult

    def __len__(self):
        return len(self.db)
```

##### 2. data/util.py文件
- 函数read_image首先用PIL将图像读入为RGB格式或单通道格式彩图，然后分别转为C*H*W与1*H*W格式。图像范围【0，255】。
- 函数resize_bbox将形状为(R,4)的bbox按照输入与输出的height、weight进行resize。
- 函数flip_bbox将根据是否翻转实现对输入bbox的横向与纵向翻转。
- 函数crop_bbox将bbox适应于图像的裁剪区域。
- 函数translate_bbox根据输入的偏移量，进行水平或竖直偏移。
- 函数random_flip将图片（CHW格式）随机水平或竖直反转：

```python
def read_image(path, dtype=np.float32, color=True):
    """
    图像为CHW格式(通道，高，宽)[0,255]；若color=True,通道顺序RGB，否则返回灰度图
    """

    f = Image.open(path)
    try:
        if color:
            img = f.convert('RGB')
        else:
            img = f.convert('P')
        img = np.asarray(img, dtype=dtype)
    finally:
        if hasattr(f, 'close'):
            f.close()

    if img.ndim == 2:
        return img[np.newaxis]          # reshape (H, W) -> (1, H, W)
    else:
        return img.transpose((2, 0, 1)) # transpose (H, W, C) -> (C, H, W)


def resize_bbox(bbox, in_size, out_size):
    """
    边界框根据给定的图像形状重新缩放
    """
    bbox = bbox.copy()
    y_scale = float(out_size[0]) / in_size[0]
    x_scale = float(out_size[1]) / in_size[1]
    bbox[:, 0] = y_scale * bbox[:, 0]
    bbox[:, 2] = y_scale * bbox[:, 2]
    bbox[:, 1] = x_scale * bbox[:, 1]
    bbox[:, 3] = x_scale * bbox[:, 3]
    return bbox


def flip_bbox(bbox, size, y_flip=False, x_flip=False):
    """
    边框共四个边：沿着y轴镜像（使用边框的第二个边和第四个边）
                沿着x轴镜像（使用边框的第一个边和第三个边）
    """
    H, W = size
    bbox = bbox.copy()
    if y_flip:
        y_max = H - bbox[:, 0]
        y_min = H - bbox[:, 2]
        bbox[:, 0] = y_min
        bbox[:, 2] = y_max
    if x_flip:
        x_max = W - bbox[:, 1]
        x_min = W - bbox[:, 3]
        bbox[:, 1] = x_min
        bbox[:, 3] = x_max
    return bbox


def crop_bbox(
        bbox, y_slice=None, x_slice=None,
        allow_outside_center=True, return_param=False):
    """  转换bounding box(边界框)以适应图像的裁剪区域
    将边界框的坐标转换为：func:data.util.translate_bbox 格式
    此外，截断边界框以适应裁剪区域。如果边界框不与裁剪区域重叠，此边界框将被删除。
    """

    t, b = _slice_to_bounds(y_slice)
    l, r = _slice_to_bounds(x_slice)
    crop_bb = np.array((t, l, b, r))

    if allow_outside_center:   （中心在裁剪区域之外的bbox（边界框）会被移除）
        mask = np.ones(bbox.shape[0], dtype=bool)
    else:
        center = (bbox[:, :2] + bbox[:, 2:]) / 2.0
        mask = np.logical_and(crop_bb[:2] <= center, center < crop_bb[2:]) \
            .all(axis=1)

    bbox = bbox.copy()
    bbox[:, :2] = np.maximum(bbox[:, :2], crop_bb[:2])
    bbox[:, 2:] = np.minimum(bbox[:, 2:], crop_bb[2:])
    bbox[:, :2] -= crop_bb[:2]
    bbox[:, 2:] -= crop_bb[:2]

    mask = np.logical_and(mask, (bbox[:, :2] < bbox[:, 2:]).all(axis=1))
    bbox = bbox[mask]

    if return_param:
        return bbox, {'index': np.flatnonzero(mask)}
    else:
        return bbox


def _slice_to_bounds(slice_):
    if slice_ is None:
        return 0, np.inf

    if slice_.start is None:
        l = 0
    else:
        l = slice_.start

    if slice_.stop is None:
        u = np.inf
    else:
        u = slice_.stop

    return l, u


def translate_bbox(bbox, y_offset=0, x_offset=0):
    """转换 bounding boxes.
    主要与图像变换（如填充和裁剪）一起使用。
    将图像的左上角从坐标：math:（0，0）转换为：math:（y，x）=（y offset，x offset）。
	参数：
	y_offset：沿Y轴的偏移。
    x_offset：沿X轴的偏移。
    """
    out_bbox = bbox.copy()
    out_bbox[:, :2] += (y_offset, x_offset)
    out_bbox[:, 2:] += (y_offset, x_offset)
    return out_bbox


def random_flip(img, y_random=False, x_random=False,
                return_param=False, copy=False):
    """ 沿垂直或水平方向随机翻转图像（用于数据增广）
    
	参数：img（numpy.ndarray）格式：CHW(用于翻转时的切片参考)。
	     return_param：是否返回翻转的bool参数
    """
    y_flip, x_flip = False, False
    if y_random:
        y_flip = random.choice([True, False])
    if x_random:
        x_flip = random.choice([True, False])

    if y_flip:
        img = img[:, ::-1, :]
    if x_flip:
        img = img[:, :, ::-1]

    if copy:
        img = img.copy()

    if return_param:
        return img, {'y_flip': y_flip, 'x_flip': x_flip}
    else:
        return img
```

##### 3. data/voc_dataset.py
- 
VOC2007包含{‘train’, ‘val’, ‘trainval’, ‘test’}，共20类，加背景21类。四个集合图片数分别为2501， 2510，5011，4952（trainval=train+val）。VOC2012无test集。

训练时使用trainval数据集，测试时使用test数据集。

- 
类 VOCBboxDataset继承自Object基类，实例化该类时只需提供VOC数据集路径即可。

类 VOCBboxDataset的方法只有一个，即返回第i张图片的信息（图像、bbox、label、difficult）


```python
import os
import xml.etree.ElementTree as ET
import numpy as np
from .util import read_image


class VOCBboxDataset:
    """ 数据集：训练数据根目/voc/vocdevkit/voc2007/
       split（'train','val','train val','test'）其中test拆分仅适用于2007数据集
    """

    def __init__(self, data_dir, split='trainval',
                 use_difficult=False, return_difficult=False,):
        id_list_file = os.path.join(
            data_dir, 'ImageSets/Main/{0}.txt'.format(split))

        self.ids = [id_.strip() for id_ in open(id_list_file)] #ids：路径样本集合
        self.data_dir = data_dir
        self.use_difficult = use_difficult
        self.return_difficult = return_difficult
        self.label_names = VOC_BBOX_LABEL_NAMES  # 类别数：20类

    def __len__(self):
        return len(self.ids)       # trainval.txt有5011个，test.txt有210个

    def get_example(self, i):
        """返回第i个示例。
        返回彩色图像和边框。图像为CHW格式。其中通道C=RGB。
        Returns: 图像和bounding boxes的元组
        """
        id_ = self.ids[i]
        anno = ET.parse(
            os.path.join(self.data_dir, 'Annotations', id_ + '.xml')) #读入.xml标签文件
        bbox = list()
        label = list()
        difficult = list()
        # 解析xml文件
        for obj in anno.findall('object'):
            if not self.use_difficult and int(obj.find('difficult').text) == 1:
                continue

            difficult.append(int(obj.find('difficult').text))
            bndbox_anno = obj.find('bndbox')
            bbox.append([
                int(bndbox_anno.find(tag).text) - 1
                for tag in ('ymin', 'xmin', 'ymax', 'xmax')])
            name = obj.find('name').text.lower().strip()
            label.append(VOC_BBOX_LABEL_NAMES.index(name))
        bbox = np.stack(bbox).astype(np.float32)
        label = np.stack(label).astype(np.int32)
        difficult = np.array(difficult, dtype=np.bool).astype(np.uint8)  #  PyTorch 不支持 np.bool，转换为uint8

        # 根据图片编号在/JPEGImages里面取图片
        img_file = os.path.join(self.data_dir, 'JPEGImages', id_ + '.jpg')
        img = read_image(img_file, color=True)   # 如果color=True，则read_image函数转换为RGB图

        # if self.return_difficult:
        #     return img, bbox, label, difficult
        return img, bbox, label, difficult

    __getitem__ = get_example


VOC_BBOX_LABEL_NAMES = (
    'aeroplane',
    'bicycle',
    'bird',
    'boat',
    'bottle',
    'bus',
    'car',
    'cat',
    'chair',
    'cow',
    'diningtable',
    'dog',
    'horse',
    'motorbike',
    'person',
    'pottedplant',
    'sheep',
    'sofa',
    'train',
    'tvmonitor')
```



