# SSD-Tensorflow 目标检测—基于检测的图像分类 - wsp_1138886114的博客 - CSDN博客





2018年12月30日 22:21:19[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：969











### 文章目录
- [一、准备](#_4)
- [二、加载模型预测分类](#_139)
- [三、生成 .tfrecords文件，批量测试](#_tfrecords_158)



各位好，今天主要讲解基于目标检测的图像分类，之前在做图像分类的模型时，分类准确度始终是个问题。特别是在公司做项目时，要对图像内容进行识别，图像样式近200多种，由于图像数据是经过扫描+压缩过的图像，图像质量可想而知，在对其进行识别时，困难十分大，故要对其分类，在针对不同模板类分别识别，提高准确率。所以想到做基于检测的图像分类，还好，想到了这个方法，前期的数据处理把我累的。不过，分类准确率直线上升。今天为大家带来VOC2007数据集的分类。


关于[数据集的标注](https://blog.csdn.net/wsp_1138886114/article/details/85017498)，[模型训练](https://blog.csdn.net/wsp_1138886114/article/details/85031094)。我这里不在论述。直接进行预测分类

### 一、准备

我这里为方便观察，修改了**SSD-Tensorflow/notebooks/visualization.py**预测代码。

修改后代码如下：

```python
# Copyright 2017 Paul Balanca. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================
import cv2,sys
import random

import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import matplotlib.cm as mpcm


# =========================================================================== #
# Some colormaps.
# =========================================================================== #
def colors_subselect(colors, num_classes=21):
    dt = len(colors) // num_classes
    sub_colors = []
    for i in range(num_classes):
        color = colors[i*dt]
        if isinstance(color[0], float):
            sub_colors.append([int(c * 255) for c in color])
        else:
            sub_colors.append([c for c in color])
    return sub_colors

colors_plasma = colors_subselect(mpcm.plasma.colors, num_classes=21)
colors_tableau = [(255, 255, 255), (31, 119, 180), (174, 199, 232), (255, 127, 14), (255, 187, 120),
                  (44, 160, 44), (152, 223, 138), (214, 39, 40), (255, 152, 150),
                  (148, 103, 189), (197, 176, 213), (140, 86, 75), (196, 156, 148),
                  (227, 119, 194), (247, 182, 210), (127, 127, 127), (199, 199, 199),
                  (188, 189, 34), (219, 219, 141), (23, 190, 207), (158, 218, 229)]


# =========================================================================== #
# OpenCV drawing.
# =========================================================================== #
def draw_lines(img, lines, color=[255, 0, 0], thickness=2):
    """Draw a collection of lines on an image.
    """
    for line in lines:
        for x1, y1, x2, y2 in line:
            cv2.line(img, (x1, y1), (x2, y2), color, thickness)


def draw_rectangle(img, p1, p2, color=[255, 0, 0], thickness=2):
    cv2.rectangle(img, p1[::-1], p2[::-1], color, thickness)


def draw_bbox(img, bbox, shape, label, color=[255, 0, 0], thickness=2):
    p1 = (int(bbox[0] * shape[0]), int(bbox[1] * shape[1]))
    p2 = (int(bbox[2] * shape[0]), int(bbox[3] * shape[1]))
    cv2.rectangle(img, p1[::-1], p2[::-1], color, thickness)
    p1 = (p1[0]+15, p1[1])
    cv2.putText(img, str(label), p1[::-1], cv2.FONT_HERSHEY_DUPLEX, 0.5, color, 1)


def bboxes_draw_on_img(img, classes, scores, bboxes, colors, thickness=2):
    shape = img.shape
    for i in range(bboxes.shape[0]):
        bbox = bboxes[i]
        color = colors[classes[i]]
        # Draw bounding box...
        p1 = (int(bbox[0] * shape[0]), int(bbox[1] * shape[1]))
        p2 = (int(bbox[2] * shape[0]), int(bbox[3] * shape[1]))
        cv2.rectangle(img, p1[::-1], p2[::-1], color, thickness)
        # Draw text...
        s = '%s/%.3f' % (classes[i], scores[i])
        p1 = (p1[0]-5, p1[1])
        cv2.putText(img, s, p1[::-1], cv2.FONT_HERSHEY_DUPLEX, 0.4, color, 1)


# =========================================================================== #
# Matplotlib show...
# =========================================================================== #
def plt_bboxes(img, classes, scores, bboxes, figsize=(10,10), linewidth=1.5):
    """Visualize bounding boxes. Largely inspired by SSD-MXNET!
    """
    # ===================adde-start ======================
    def num2class(n):
        sys.path.append('../')
        from datasets import pascalvoc_2007 as pas
        x=pas.pascalvoc_common.VOC_LABELS.items()
        for name,item in x:
            if n in item:
                return name
    # ===================added-end======================
    fig = plt.figure(figsize=figsize)
    plt.imshow(img)
    height = img.shape[0]
    width = img.shape[1]
    colors = dict()
    class_names = []        # 用来储存类别名（一张图有可能不只一个类别名）
    for i in range(classes.shape[0]):
        cls_id = int(classes[i])
        if cls_id >= 0:
            score = scores[i]
            if cls_id not in colors:
                colors[cls_id] = (random.random(), random.random(), random.random())
            ymin = int(bboxes[i, 0] * height)
            xmin = int(bboxes[i, 1] * width)
            ymax = int(bboxes[i, 2] * height)
            xmax = int(bboxes[i, 3] * width)
            rect = plt.Rectangle((xmin, ymin), xmax - xmin,
                                 ymax - ymin, fill=False,
                                 edgecolor=colors[cls_id],
                                 linewidth=linewidth)
            plt.gca().add_patch(rect)
            #================adde-start ==============
            class_name = num2class(cls_id)
            class_names.append(class_name)
            #================added-end=================
            plt.gca().text(xmin, ymin - 2,
                           '{:s} | {:.3f}'.format(class_name, score),
                           bbox=dict(facecolor=colors[cls_id], alpha=0.5),
                           fontsize=12, color='white')
    plt.show()
    return class_names
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181231105315525.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

### 二、加载模型预测分类

在`SSD-Tensorflow\notebooks`目录下打开jupyter notebook(注意运行环境)，打开ssd_notebook.ipynb文件，修改最后一个cell 代码如下：

依次运行所有cell即可。

```python
# Test on some demo image and visualize output.
import shutil
path = '../demo/'
image_names = sorted(os.listdir(path))          # 获取测试文件夹所有图片

for it  in image_names:
    img = mpimg.imread(path + it)
    print("path + it",path + it)
    rclasses, rscores, rbboxes =  process_image(img)
    class_names = visualization.plt_bboxes(img, rclasses, rscores, rbboxes)
    if  "person" in class_names:             # 将有人的图片复制到目标目录
        shutil.copy(path + it,"D:/person/person_00"+it[-8:])
    else:
        pass
```

### 三、生成 .tfrecords文件，批量测试

关于数据集的下载，[详情请点击](https://blog.csdn.net/wsp_1138886114/article/details/84993500)

在工程目录下下创建文件夹（可省略）用来放置测试文件。文件格式与训练集相同。
- 打开 `tf_convert_data.py`文件，

依次点击：run , Edit Configuration，在面板中依次执行如下：

Name:tf_convert_data

Script path :`D:\XXX\SSD-Tensorflow\tf_convert_data.py`最好是绝对路径

在Parameters中填入以下内容： 设置好点击Apply,OK。执行。
```python
--dataset_name=pascalvoc
--dataset_dir=./VOC2007_test/
--output_name=voc_2007_test
--output_dir=./tfrecords_test
```

![效果图](https://img-blog.csdnimg.cn/20181231191123901.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)




