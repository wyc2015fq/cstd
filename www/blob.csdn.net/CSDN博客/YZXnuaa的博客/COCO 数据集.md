# COCO 数据集 - YZXnuaa的博客 - CSDN博客
2018年06月01日 13:23:47[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：579
微软发布的COCO数据库, 除了图片以外还提供物体检测, 分割(segmentation)和对图像的语义文本描述信息.
COCO数据库的网址是:
- MS COCO API - [http://mscoco.org/](http://mscoco.org/)
- Github网址 - [https://github.com/pdollar/coco](https://github.com/pdollar/coco)
- 关于API更多的细节在网站: [http://mscoco.org/dataset/#download](http://mscoco.org/dataset/#download)
数据库提供 Matlab, Python 和 Lua 的 API 接口. 其中 matlab 和 python 的 API 接口可以提供完整的图像标签数据的加载, parsing 和可视化.此外,网站还提供了数据相关的文章, 教程等.
在使用 COCO 数据库提供的 API 和 demo 时, 需要首先下载 COCO 的图像和标签数据.
- 安装:- 首先解压数据文件:- 图像数据下载到 `coco/images/` 文件夹中
- 标签数据下载到 `coco/` 文件夹中.
- matlab, 在 matlab 的默认路径中添加 `coco/MatlabApi`
- Python. 打开终端,将路径切换到 `coco/PythonAPI`下,输入 `make`
- COCO数据集的标注信息
COCO的数据标注信息包括:
- 类别标志
- 类别数量区分
- 像素级的分割
```python
```python
import sys
sys.path.append('E:/xinlib')
from data import cocox
import zipfile
```
```
查看 `coco/images/` 文件夹下的数据：
```python
```python
image_names = cocox.get_image_names()
image_names
```
```
```
['E:/Data/coco/images/test2017.zip',
 'E:/Data/coco/images/train2017.zip',
 'E:/Data/coco/images/unlabeled2017.zip',
 'E:/Data/coco/images/val2017.zip']
```
查看 `coco/` 文件夹的文件：
```python
```python
import os
dataDir = cocox.root
```
```
```python
```python
os.listdir(dataDir)
```
```
```
['annotations',
 'annotations_trainval2017.zip',
 'cocoapi',
 'images',
 'image_info_test2017.zip',
 'image_info_unlabeled2017.zip',
 'stuff_annotations_trainval2017.zip']
```
我们只需要获取 annotations 的信息（这里都是以 `.zip` 结尾）：
```python
```python
annDir = [z_name for z_name in os.listdir(dataDir) if z_name.endswith('.zip')]
annDir
```
```
```
['annotations_trainval2017.zip',
 'image_info_test2017.zip',
 'image_info_unlabeled2017.zip',
 'stuff_annotations_trainval2017.zip']
```
解压 annotations 的文件：
```python
```python
for ann_name in annDir:
    z = zipfile.ZipFile(dataDir + '/' + ann_name)
    # 全部解压
    z.extractall(dataDir)
```
```
```python
```python
# 封装为函数
cocox.unzip_annotations()
```
```
```python
```python
# 删除标签的压缩文件
cocox.del_annotations()
```
```
由于图片数据比较大，我就不解压了，不过可以通过 `MXNet + zipfile` 来直接获取图片信息。
# 获取图片数据
我以 `test2017.zip` 为例：
```python
```python
image_names
```
```
```
['E:/Data/coco/images/test2017.zip',
 'E:/Data/coco/images/train2017.zip',
 'E:/Data/coco/images/unlabeled2017.zip',
 'E:/Data/coco/images/val2017.zip']
```
```python
```python
z = zipfile.ZipFile(image_names[0])
```
```
```python
```python
# 测试集的图片名称列表
z.namelist()
```
```
```
['test2017/',
 'test2017/000000259564.jpg',
 'test2017/000000344475.jpg',
 ...]
```
我们可以看出，第一个是目录名，之后的才是图片。下面我们来看看第一张图片：
```python
```python
from mxnet import image
```
```
```python
```python
r = z.read(z.namelist()[1])    # bytes
data = image.imdecode(r)       # 转换为 NDArray 数组，可以做数值运算
data
```
```
```
[[[ 87  94  78]
  [ 85  94  77]
  [ 87  96  79]
  ..., 
  [108  63  44]
  [252 244 233]
  [253 253 253]]
 [[ 86  95  76]
  [ 88  97  78]
  [ 85  94  75]
  ..., 
  [ 55  14   0]
  [150  94  81]
  [252 245 216]]
 [[ 90  99  78]
  [ 89  98  77]
  [ 89  98  77]
  ..., 
  [ 63  37  12]
  [ 90  30   6]
  [149  83  61]]
 ..., 
 [[ 86 104  82]
  [ 89 102  82]
  [ 84 102  80]
  ..., 
  [ 50  62  40]
  [ 50  61  45]
  [ 51  58  50]]
 [[ 89 101  77]
  [ 87  96  75]
  [ 89 104  83]
  ..., 
  [ 54  63  42]
  [ 49  53  39]
  [ 53  54  48]]
 [[ 96 100  77]
  [ 94  97  76]
  [ 88 103  82]
  ..., 
  [ 44  58  32]
  [ 45  57  37]
  [ 49  57  42]]]
<NDArray 480x640x3 @cpu(0)>
```
```python
```python
x = data.asnumpy()   # 转换为 array
```
```
```python
```python
# 显示图片
%pylab inline 
plt.imshow(x)
```
```
![output_21_3.png-125.1kB](http://static.zybuluo.com/xinet/me14jz54c45sd5lgudgzf8oe/output_21_3.png)
为此，我们可以将其封装为一个迭代器：`cocox.data_iter(dataType)`
# 获取标签信息（利用官方给定教程）
- 
安装 python API：
```bash
pip install -U pycocotools
```
Windows （一般需要安装 visual studio）下有许多的坑：[Windows 10 编译 Pycocotools 踩坑记](https://www.jianshu.com/p/de455d653301)
```python
```python
%pylab inline
from pycocotools.coco import COCO
import numpy as np
import skimage.io as io
import matplotlib.pyplot as plt
import pylab
pylab.rcParams['figure.figsize'] = (8.0, 10.0)
```
```
这里有一个坑 (由 PIL 引发) `import skimage.io as io` 在 Windows 下可能会报错，我的解决办法是：
- 
先卸载 Pillow，然后重新安装即可。
- 
插曲：PIL(Python Imaging Library)是Python一个强大方便的图像处理库，名气也比较大。Pillow 是 PIL 的一个派生分支，但如今已经发展成为比 PIL 本身更具活力的图像处理库。
```python
```python
dataDir = cocox.root
dataType = 'val2017'
annFile = '{}/annotations/instances_{}.json'.format(dataDir, dataType)
```
```
```python
```python
# initialize COCO api for instance annotations
coco=COCO(annFile)
```
```
```
loading annotations into memory...
Done (t=0.93s)
creating index...
index created!
```
```python
```python
COCO??
```
```
`COCO` 是一个类：
```
Constructor of Microsoft COCO helper class for reading and visualizing annotations.
:param annotation_file (str): location of annotation file
:param image_folder (str): location to the folder that hosts images.
```
## display COCO categories and supercategories
```python
```python
cats = coco.loadCats(coco.getCatIds())
nms = [cat['name'] for cat in cats]
print('COCO categories: \n{}\n'.format(' '.join(nms)))
nms = set([cat['supercategory'] for cat in cats])
print('COCO supercategories: \n{}'.format(' '.join(nms)))
```
```
```
COCO categories: 
person bicycle car motorcycle airplane bus train truck boat traffic light fire hydrant stop sign parking meter bench bird cat dog horse sheep cow elephant bear zebra giraffe backpack umbrella handbag tie suitcase frisbee skis snowboard sports ball kite baseball bat baseball glove skateboard surfboard tennis racket bottle wine glass cup fork knife spoon bowl banana apple sandwich orange broccoli carrot hot dog pizza donut cake chair couch potted plant bed dining table toilet tv laptop mouse remote keyboard cell phone microwave oven toaster sink refrigerator book clock vase scissors teddy bear hair drier toothbrush
COCO supercategories: 
appliance sports person indoor vehicle food electronic furniture animal outdoor accessory kitchen
```
```python
```python
# get all images containing given categories, select one at random
catIds = coco.getCatIds(catNms=['person', 'dog', 'skateboard'])
imgIds = coco.getImgIds(catIds=catIds)
imgIds = coco.getImgIds(imgIds=[335328])
img = coco.loadImgs(imgIds[np.random.randint(0, len(imgIds))])[0]
```
```
```python
```python
img
```
```
```
{'license': 4,
 'file_name': '000000335328.jpg',
 'coco_url': 'http://images.cocodataset.org/val2017/000000335328.jpg',
 'height': 640,
 'width': 512,
 'date_captured': '2013-11-20 19:29:37',
 'flickr_url': 'http://farm3.staticflickr.com/2079/2128089396_ddd988a59a_z.jpg',
 'id': 335328}
```
官方给的这个代码需要将图片数据集解压：
```python
# load and display image
# use url to load image
# I = io.imread(img['coco_url'])
I = io.imread('%s/images/%s/%s' % (dataDir, dataType, img['file_name']))
plt.axis('off')
plt.imshow(I)
plt.show()
```
我们可以使用 `zipfile` 模块直接读取图片，而无须解压：
```python
```python
image_names[-1]
```
```
`'E:/Data/coco/images/val2017.zip'`
```python
```python
val_z = zipfile.ZipFile(image_names[-1])
I = image.imdecode(val_z.read('%s/%s' % (dataType, img['file_name']))).asnumpy()
plt.axis('off')
plt.imshow(I)
plt.show()
```
```
![output_36_0.png-493.1kB](http://static.zybuluo.com/xinet/dkgt9ylzcvffjg2r46agphx2/output_36_0.png)
## load and display instance annotations
```python
```python
plt.imshow(I)
plt.axis('off')
annIds = coco.getAnnIds(imgIds=img['id'], catIds=catIds, iscrowd=None)
anns = coco.loadAnns(annIds)
coco.showAnns(anns)
```
```
## initialize COCO api for person keypoints annotations
```python
```python
annFile = '{}/annotations/person_keypoints_{}.json'.format(dataDir, dataType)
coco_kps = COCO(annFile)
```
```
```
loading annotations into memory...
Done (t=0.43s)
creating index...
index created!
```
## load and display keypoints annotations
```python
```python
plt.imshow(I)
plt.axis('off')
ax = plt.gca()
annIds = coco_kps.getAnnIds(imgIds=img['id'], catIds=catIds, iscrowd=None)
anns = coco_kps.loadAnns(annIds)
coco_kps.showAnns(anns)
```
```
![output_42_0.png-491kB](http://static.zybuluo.com/xinet/18h0q946r5xjw8c8dmaakb52/output_42_0.png)
## initialize COCO api for caption annotations
```python
```python
annFile = '{}/annotations/captions_{}.json'.format(dataDir, dataType)
coco_caps = COCO(annFile)
```
```
```
loading annotations into memory...
Done (t=0.06s)
creating index...
index created!
```
## load and display caption annotations
```python
```python
annIds = coco_caps.getAnnIds(imgIds=img['id'])
anns = coco_caps.loadAnns(annIds)
coco_caps.showAnns(anns)
plt.imshow(I)
plt.axis('off')
plt.show()
```
```
```
A couple of people riding waves on top of boards.
a couple of people that are surfing in water
A man and a young child in wet suits surfing in the ocean.
a man and small child standing on a surf board  and riding some waves
A young boy on a surfboard being taught to surf.
```
![output_46_1.png-493.1kB](http://static.zybuluo.com/xinet/7xnqxod6mtap1k63nl5eyk0p/output_46_1.png)
