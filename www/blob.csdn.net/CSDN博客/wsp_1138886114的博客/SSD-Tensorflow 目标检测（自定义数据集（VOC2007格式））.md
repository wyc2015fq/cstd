# SSD-Tensorflow 目标检测（自定义数据集（VOC2007格式）） - wsp_1138886114的博客 - CSDN博客





2018年12月16日 12:08:50[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1166标签：[SSD-Tensorflow 目标检测（自定义数据集（VOC2007格](https://so.csdn.net/so/search/s.do?q=SSD-Tensorflow 目标检测（自定义数据集（VOC2007格&t=blog)
个人分类：[深度学习](https://blog.csdn.net/wsp_1138886114/article/category/7729523)












### 文章目录
- [一、准备](#_1)
- [二、生成.tfrecords文件](#tfrecords_61)
- [三、训练模型修改](#_288)
- [四、训练](#_357)
- [五、测试验证](#_416)
- [六、报错误及解决方案：](#_449)




### 一、准备
- 
搭建SSD框架，[下载解压](https://github.com/balancap/SSD-Tensorflow)即可

- 
下载pascalvoc数据，自己的数据根据voc格式改写（图片的名称，不用拘泥于6位数字，其他命名也可以）[数据集下载点击](https://blog.csdn.net/wsp_1138886114/article/details/84993500)

解压后不要混合在一个文件夹下

VOCtrainval用来训练，VOCtest用来测试。

VOCtrainval 中JPEGImage文件夹中仅是训练和验证的图片，Main文件夹中仅是trainval.txt,  train.txt,   val.txt

VOCtest中JPEGImage文件夹中仅是测试图片，Main文件夹中仅是test.txt

自己的文件根据以上文件格式放置图片即可。- 
自己的数据根据voc格式改写（图片的名称，不用拘泥于6位数字，其他命名也可以）
[文件重命名点击](https://blog.csdn.net/wsp_1138886114/article/details/85028499)

- 
标记自己的数据 ，这个过程枯燥，需要耐心。[详情请点击](https://blog.csdn.net/wsp_1138886114/article/details/85017498),

- 
生成txt文件，train.txt, trainval.txt, test.txt, val.txt（注意文件路径）


```python
import os
import random

saveBasePath = r"./VOC2007/ImageSets"              # txt文件保存目录
total_xml = os.listdir(r'./VOC2007/Annotations')   # 获取标注文件（file_name.xml）

# 划分数据集为（训练，验证，测试集 = 49%,20%,30%）
trainval_percent = 0.7
train_percent = 0.7
tv = int(len(total_xml) * trainval_percent)       # 70%训练-验证集的文件数目
tr = int(tv * train_percent)                      # 70%训练集的文件数目

# 打乱训练文件（洗牌）
trainval = random.sample(range(len(total_xml)), tv)
train = random.sample(trainval, tr)

print("train and val size", tv)
print("train size", tr)
ftrainval = open(os.path.join(saveBasePath, 'Main/trainval.txt'), 'w')
ftest = open(os.path.join(saveBasePath, 'Main/test.txt'), 'w')
ftrain = open(os.path.join(saveBasePath, 'Main/train.txt'), 'w')
fval = open(os.path.join(saveBasePath, 'Main/val.txt'), 'w')

for i in range(len(total_xml)):                # 遍历所有 file_name.xml 文件
    name = total_xml[i][:-4] + '\n'            # 获取 file_name
    if i in trainval:
        ftrainval.write(name)
        if i in train:
            ftrain.write(name)
        else:
            fval.write(name)
    else:
        ftest.write(name)

ftrainval.close()
ftrain.close()
fval.close()
ftest.close()
```
- 将train.txt, trainval.txt, test.txt, val.txt放置训练验证集文件目录下
`VOCtrainval_06-Nov-2007\VOCdevkit\VOC2007\ImageSets\Main\`
- 将test.txt放置测试集文件目录下：
`VOCtest_06-Nov-2007\VOCdevkit\VOC2007\ImageSets\Main\`

### 二、生成.tfrecords文件
- 将训练类别修改为和自己一样的

在此目录文件下： `SSD-Tensorflow/datasets/pascalvoc_common.py`

根据实际情况进行修改
```python
# 注释原始的标签，添加自己的标签
VOC_LABELS = {   
    'none': (0, 'Background'),   
    'aeroplane': (1, 'Vehicle'),   
    'bicycle': (2, 'Vehicle'),   
    'bird': (3, 'Animal'),   
    'boat': (4, 'Vehicle'),   
     ... ...
    'Person': (15, 'Person'),   
    'pottedplant': (16, 'Indoor'),   
    'sheep': (17, 'Animal'),   
    'sofa': (18, 'Indoor'),   
    'train': (19, 'Vehicle'),   
    'tvmonitor': (20, 'Indoor'),   
}
```
- 将图像数据转换为tfrecods格式
`SSD-Tensorflow/datasets/pascalvoc_to_tfrecords.py`。

更改文件的 **83** 行:`image_data = tf.gfile.FastGFile(filename, 'rb').read()`；

如果你的图片不是`.jpg`格式，修改图片类型；

更改文件的 **67** 行，`SAMPLES_PER_FILES = 500(自定义)`意为：几个.xml转为一个tfrecords，如下图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216120455542.PNG)- 生成.tfrecords文件

打开`tf_convert_data.py`文件，依次点击：`run、Edit Configuration`，在`Parameters`中填入以下内容，再运行`tf_convert_data.py`文件，在面板中得到成功信息，可以在tfrecords_文件夹下看到生成的.tfrecords文件；

```python
--dataset_name=pascalvoc
--dataset_dir=./VOC2007/
--output_name=voc_2007_train
--output_dir=./tfrecords_
```

**或者**在SSD-Tensorflow 文件夹下创建`tf_conver_data.sh` 运行。

```
#!/bin/bash  
# 这是一个shell脚本，用于将pascal VOC数据集转换tfrecords数据

DATASET_DIR=./VOC2007/            # VOC数据保存的文件夹（VOC的目录格式未改变）  
OUTPUT_DIR=./tfrecords_           # 保存tfrecords数据的文件夹  

python ./tf_convert_data.py\
--dataset_name=pascalvoc\
--dataset_dir=${DATASET_DIR}\
--output_name=voc_2007_train\
--output_dir=${OUTPUT_DIR}
```

**或者**直接使用如下代码

```python
"""
特别注意: path地址是否正确、要在主目录下提前创建“tfrecords_”文件夹
"""

import os
import sys
import random
import numpy as np
import tensorflow as tf
import xml.etree.ElementTree as ET  # 操作xml文件

# 我的标签定义只有两类，要根据自己的图片而定
VOC_LABELS = {
    'none': (0, 'Background'),
    'aiaitie': (1, 'Product')
}

# 图片和标签存放的文件夹.
DIRECTORY_ANNOTATIONS = 'Annotations/'
DIRECTORY_IMAGES = 'JPEGImages/'


RANDOM_SEED = 4242     # 随机种子.
SAMPLES_PER_FILES = 3  # 每个.tfrecords文件包含几个.xml样本


# 生成整数型，浮点型和字符串型的属性
def int64_feature(value):
    if not isinstance(value, list):
        value = [value]
    return tf.train.Feature(int64_list=tf.train.Int64List(value=value))

def float_feature(value):
    if not isinstance(value, list):
        value = [value]
    return tf.train.Feature(float_list=tf.train.FloatList(value=value))

def bytes_feature(value):
    if not isinstance(value, list):
        value = [value]
    return tf.train.Feature(bytes_list=tf.train.BytesList(value=value))

# 图片处理
def _process_image(directory, name):
    # Read the image file.
    filename = directory + DIRECTORY_IMAGES + name + '.jpg'
    image_data = tf.gfile.FastGFile(filename, 'rb').read()

    # Read the XML annotation file.
    filename = os.path.join(directory, DIRECTORY_ANNOTATIONS, name + '.xml')
    tree = ET.parse(filename)
    root = tree.getroot()

    # Image shape.
    size = root.find('size')
    shape = [int(size.find('height').text),
             int(size.find('width').text),
             int(size.find('depth').text)]
    # Find annotations.
    bboxes = []
    labels = []
    labels_text = []
    difficult = []
    truncated = []
    for obj in root.findall('object'):
        label = obj.find('name').text
        labels.append(int(VOC_LABELS[label][0]))
        labels_text.append(label.encode('ascii'))  # 变为ascii格式

        if obj.find('difficult'):
            difficult.append(int(obj.find('difficult').text))
        else:
            difficult.append(0)
        if obj.find('truncated'):
            truncated.append(int(obj.find('truncated').text))
        else:
            truncated.append(0)

        bbox = obj.find('bndbox')
        a = float(bbox.find('ymin').text) / shape[0]
        b = float(bbox.find('xmin').text) / shape[1]
        a1 = float(bbox.find('ymax').text) / shape[0]
        b1 = float(bbox.find('xmax').text) / shape[1]
        a_e = a1 - a
        b_e = b1 - b
        if abs(a_e) < 1 and abs(b_e) < 1:
            bboxes.append((a, b, a1, b1))

    return image_data, shape, bboxes, labels, labels_text, difficult, truncated

# 转化样例
def _convert_to_example(image_data, labels, labels_text, bboxes, shape,
                        difficult, truncated):
    xmin = []
    ymin = []
    xmax = []
    ymax = []
    for b in bboxes:
        assert len(b) == 4
        # pylint: disable=expression-not-assigned
        [l.append(point) for l, point in zip([ymin, xmin, ymax, xmax], b)]
        # pylint: enable=expression-not-assigned

    image_format = b'JPEG'
    example = tf.train.Example(features=tf.train.Features(feature={
        'image/height': int64_feature(shape[0]),
        'image/width': int64_feature(shape[1]),
        'image/channels': int64_feature(shape[2]),
        'image/shape': int64_feature(shape),
        'image/object/bbox/xmin': float_feature(xmin),
        'image/object/bbox/xmax': float_feature(xmax),
        'image/object/bbox/ymin': float_feature(ymin),
        'image/object/bbox/ymax': float_feature(ymax),
        'image/object/bbox/label': int64_feature(labels),
        'image/object/bbox/label_text': bytes_feature(labels_text),
        'image/object/bbox/difficult': int64_feature(difficult),
        'image/object/bbox/truncated': int64_feature(truncated),
        'image/format': bytes_feature(image_format),
        'image/encoded': bytes_feature(image_data)}))
    return example

# 增加到tfrecord
def _add_to_tfrecord(dataset_dir, name, tfrecord_writer):
    image_data, shape, bboxes, labels, labels_text, difficult, truncated = \
        _process_image(dataset_dir, name)
    example = _convert_to_example(image_data, labels, labels_text,
                                  bboxes, shape, difficult, truncated)
    tfrecord_writer.write(example.SerializeToString())


# name为转化文件的前缀
def _get_output_filename(output_dir, name, idx):
    return '%s/%s_%03d.tfrecord' % (output_dir, name, idx)


def run(dataset_dir, output_dir, name='voc_train', shuffling=False):
    if not tf.gfile.Exists(dataset_dir):
        tf.gfile.MakeDirs(dataset_dir)

    path = os.path.join(dataset_dir, DIRECTORY_ANNOTATIONS)
    filenames = sorted(os.listdir(path))  # 排序
    if shuffling:
        random.seed(RANDOM_SEED)
        random.shuffle(filenames)

    i = 0
    fidx = 0
    while i < len(filenames):            # Open new TFRecord file.
        tf_filename = _get_output_filename(output_dir, name, fidx)
        with tf.python_io.TFRecordWriter(tf_filename) as tfrecord_writer:
            j = 0
            while i < len(filenames) and j < SAMPLES_PER_FILES:
                sys.stdout.write(' Converting image %d/%d \n' % (i + 1, len(filenames)))  # 终端打印，类似print
                sys.stdout.flush()  # 缓冲

                filename = filenames[i]
                img_name = filename[:-4]
                _add_to_tfrecord(dataset_dir, img_name, tfrecord_writer)
                i += 1
                j += 1
            fidx += 1

    print('\nFinished converting the Pascal VOC dataset!')


# 原数据集路径，输出路径以及输出文件名，要根据自己实际做改动
dataset_dir = "C:/Users/Admin/Desktop/"
output_dir = "./tfrecords_"
name = "voc_train"

def main(_):
    run(dataset_dir, output_dir, name)

if __name__ == '__main__':
    tf.app.run()
```

### 三、训练模型修改
- `datasets/pascalvoc_2007.py`修改训练数据shape：

根据自己训练数据修改：`NUM_CLASSES = 类别数`；

```python
TRAIN_STATISTICS = {
    'none': (0, 0),
    'aeroplane': (238, 306),   #238图片数， 306目标总数
    'bicycle': (243, 353),
    'bird': (330, 486),
    'boat': (181, 290),
    ... ...
    'sheep': (96, 257),
    'sofa': (229, 248),
    'train': (261, 297),
    'tvmonitor': (256, 324),
    'total': (5011, 12608),   #5011 为训练的图片书，12608为目标总数
}
TEST_STATISTICS = {
    'none': (0, 0),
    'aeroplane': (1, 1),
    'bicycle': (1, 1),
    'bird': (1, 1),
     ... ...
    'sheep': (1, 1),
    'sofa': (1, 1),
    'train': (1, 1),
    'tvmonitor': (1, 1),
    'total': (20, 20),
}
SPLITS_TO_SIZES = {
    'train': 5011,        # 训练数据量
    'test': 4952,         # 测试数据量
}
SPLITS_TO_STATISTICS = {
    'train': TRAIN_STATISTICS,
    'test': TEST_STATISTICS,
}
NUM_CLASSES = 20         # 类别，根据自己数据的实际类别修改（不包含背景）
```
- `nets/ssd_vgg_300.py` 修改类别个数，根据自己训练类别数修改**96** 和**97**行：等于类别数+1；

```python
img_shape=(300, 300),
        num_classes=21,         #根据自己的数据修改为类别+1 
        no_annotation_label=21, #根据自己的数据修改为类别+1
```
- `eval_ssd_network.py`修改类别个数，修改**66**行的类别个数：等于类别数+1；

```python
tf.app.flags.DEFINE_integer(
    'num_classes', 21, 'Number of classes to use in the dataset.')
```
- `train_ssd_network.py`

修改 **27**行的数据格式，改为’NHWC’；

修改**135**行的类别个数：等于类别数+1；

修改 **56**行—**66**行是关于模型运行保存的参数；

修改 **154**行的最大训练步数，将None（训练会无限进行）改为比如50000。
```python
tf.app.flags.DEFINE_integer(
    'log_every_n_steps', 10,
    'The frequency with which logs are print.')
tf.app.flags.DEFINE_integer(
    'save_summaries_secs', 600,
    'The frequency with which summaries are saved, in seconds.')
tf.app.flags.DEFINE_integer(
    'save_interval_secs', 600,
    'The frequency with which the model is saved, in seconds.')
tf.app.flags.DEFINE_float(
    'gpu_memory_fraction', 0.9, 'GPU memory fraction to use.')
```

### 四、训练

**方案1** 从vgg开始训练其中某些层的参数：
- ssd_300_vgg中的300是指把图片归一化为 300*300，所以如果要用ssd_512_vgg来fine-tune的话，就只需要重新训练受图片分辨率影响的层即可。

```
# 通过加载预训练好的vgg16模型，进行训练  
# 通过 checkpoint_exclude_scopes 指定哪些层的参数不需要从vgg16模型里面加载进来  
# 通过 trainable_scopes          指定哪些层的参数是需要训练的，未指定的参数保持不变,若注释掉此命令，所有的参数均需要训练

DATASET_DIR=./tfrecords_/                  # 数据存放路径
TRAIN_DIR=./train_model/                   # 训练生成模型的存放路径  
CHECKPOINT_PATH=./checkpoints/vgg_16.ckpt  # 加载预训练模型的路径  
  
python ../train_ssd_network.py \  
	--train_dir=${TRAIN_DIR} \              # 训练生成模型的存放路径  
	--dataset_dir=${DATASET_DIR} \          # 数据存放路径  
	--dataset_name=pascalvoc_2007 \         # 数据名的前缀  
	--dataset_split_name=train \  
	--model_name=ssd_300_vgg \              # 加载的模型的名字  
	--checkpoint_path=${CHECKPOINT_PATH} \  # 所加载模型的路径  
	--checkpoint_model_scope=vgg_16 \       # 所加载模型里面的作用域名  
	--checkpoint_exclude_scopes=ssd_300_vgg/conv6,ssd_300_vgg/conv7,ssd_300_vgg/block8,ssd_300_vgg/block9,ssd_300_vgg/block10,ssd_300_vgg/block11,ssd_300_vgg/block4_box,ssd_300_vgg/block7_box,ssd_300_vgg/block8_box,ssd_300_vgg/block9_box,ssd_300_vgg/block10_box,ssd_300_vgg/block11_box \  
	--trainable_scopes=ssd_300_vgg/conv6,ssd_300_vgg/conv7,ssd_300_vgg/block8,ssd_300_vgg/block9,ssd_300_vgg/block10,ssd_300_vgg/block11,ssd_300_vgg/block4_box,ssd_300_vgg/block7_box,ssd_300_vgg/block8_box,ssd_300_vgg/block9_box,ssd_300_vgg/block10_box,ssd_300_vgg/block11_box \  
	--save_summaries_secs=60 \  # 每60s保存一下日志  
	--save_interval_secs=600 \  # 每600s保存一下模型  
	--weight_decay=0.0005 \     # 正则化的权值衰减的系数  
	--optimizer=adam \          # 选取的最优化函数  
	--learning_rate=0.001 \     # 学习率  
	--learning_rate_decay_factor=0.94 \     # 学习率的衰减因子  
	--batch_size=24 \                       # 可以小一点，不然可能会报错（显存不够用）
	--gpu_memory_fraction=0.9               # 指定占用gpu内存的百分比
```

**方案2**：从头开始训练自己的模型

```
#注释掉如下参数：
#CHECKPOINT_PATH=./checkpoints/vgg_16.ckpt  不提供初始化模型,让模型自己随机初始化权重，从头训练
#--checkpoint_path=${CHECKPOINT_PATH}
#--checkpoint_path=${CHECKPOINT_PATH}
#--checkpoint_model_scope=ssd_512_vgg
#--checkpoint_exclude_scopes=ssd_300_vgg/block10...
#--trainable_scopes=ssd_300_vgg/conv6...

#/bin/bash
DATASET_DIR=./tfrecords_/                  # 数据存放路径
TRAIN_DIR=./train_model/                   # 训练生成模型的存放路径

CUDA_VISIBLE_DEVICES=0 python ./train_ssd_network.py \
	 --train_dir=${TRAIN_DIR} \
	 --dataset_dir=${DATASET_DIR} \
	 --dataset_name=pascalvoc_2007 \
	 --dataset_split_name=train \
	 --model_name=ssd_300_vgg \
	 --save_summaries_secs=600 \
	 --save_interval_secs=600 \
	 --optimizer=adam \
	 --learning_rate_decay_factor=0.94 \
	 --batch_size=32 \
	 --gpu_memory_fraction=0.9
```

### 五、测试验证
- 生成 `.tfrecords`文件。将测试图片转换为tfrecords

```
#!/bin/bash
DATASET_DIR=./VOC2007/test_images/        # 测试图片目录（存放测试的图片）
OUTPUT_DIR=./tfrecords_/tfrecords/        # 测试图片的 .tfrecords文件    
python ./tf_convert_data.py \
       --dataset_name=pascalvoc \
       --dataset_dir=${DATASET_DIR} \
       --output_name=voc_2007_test \
       --output_dir=${OUTPUT_DIR}
```
- 运行测试

```
#!/bin/bash
DATASET_DIR=./tfrecords_/tfrecords/
EVAL_DIR=./ssd_eval_log/
CHECKPOINT_PATH=./train_model/model.ckpt-5000

python ./eval_ssd_network.py \
       --eval_dir=${EVAL_DIR} \
       --dataset_dir=${DATASET_DIR} \
       --dataset_name=pascalvoc_2007 \
       --dataset_split_name=test \
       --model_name=ssd_300_vgg \
       --checkpoint_path=${CHECKPOINT_PATH} \
       --batch_size=1
```
- 使用 `notebooksssd_notebook.ipynb`来查看模型标注的图片。[详情请点击](https://blog.csdn.net/wsp_1138886114/article/details/84993500)

修改`ckpt_filename = "路径/自己训练的权重文件"`

修改自己图片所在的路径，或者将需要测试的图片放入
### 六、报错误及解决方案：
- 错误1`ZeroDivisionError: float division by zero`,详情如下：

```python
>> Converting image 117/504Traceback (most recent call last): #第117张标注文件有问题
  File "D:/AI_target_detection/SSD-Tensorflow/tf_convert_data.py", line 59, in <module>
    tf.app.run()
  File "C:\Anaconda3\envs\AI_tensorflow_GPU\lib\site-packages\tensorflow\python\platform\app.py", line 125, in run
    _sys.exit(main(argv))
  File "D:/AI_target_detection/SSD-Tensorflow/tf_convert_data.py", line 54, in main
    pascalvoc_to_tfrecords.run(FLAGS.dataset_dir, FLAGS.output_dir, FLAGS.output_name)
  File "D:\AI_target_detection\SSD-Tensorflow\datasets\pascalvoc_to_tfrecords.py", line 223, in run
    _add_to_tfrecord(dataset_dir, img_name, tfrecord_writer)
  File "D:\AI_target_detection\SSD-Tensorflow\datasets\pascalvoc_to_tfrecords.py", line 182, in _add_to_tfrecord
    _process_image(dataset_dir, name)
  File "D:\AI_target_detection\SSD-Tensorflow\datasets\pascalvoc_to_tfrecords.py", line 121, in _process_image
    bboxes.append((max(float(bbox.find('ymin').text) / shape[0], 0.1),
ZeroDivisionError: float division by zero
```
- 错误2 `All bounding box coordinates must be in [0.0, 1.0]`

原因及解决方法：标注数据集时鼠标多点了一下，没有任何标注，和标注框超出图片范围。
- 找到那一张图片以及其标注，删除标注及其文件，或重新标注
- 找到`pascalvoc_to_tfrecords.py`114-119行

```python
将：
        bboxes.append((float(bbox.find('ymin').text) / shape[0],
                       float(bbox.find('xmin').text) / shape[1],
                       float(bbox.find('ymax').text) / shape[0],
                       float(bbox.find('xmax').text) / shape[1]
                       ))
修改为：
        bboxes.append((max(float(bbox.find('ymin').text) / shape[0], 0.0),
                       max(float(bbox.find('xmin').text) / shape[1], 0.0),
                       min(float(bbox.find('ymax').text) / shape[0], 1.0),
                       min(float(bbox.find('xmax').text) / shape[1], 1.0)
                       ))
```









