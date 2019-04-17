# 【用Python学习Caffe】2. 使用Caffe完成图像目标检测 - tostq的专栏 - CSDN博客





2017年06月22日 22:08:21[tostq](https://me.csdn.net/tostq)阅读数：6239
所属专栏：[用Python学习Caffe](https://blog.csdn.net/column/details/16089.html)









## 2. 使用Caffe完成图像目标检测

本节将以一个快速的图像目标检测网络SSD作为例子，通过Python Caffe来进行图像目标检测。

> 
必须安装windows-ssd版本的Caffe，或者自行在caffe项目中添加SSD的新增相关源代码.


图像目标检测网络同图像分类网络的大体原理及结构很相似，不过原始图像再经过深度网络后，并不是得到一组反映不同分类种类下概率的向量，而得到若干组位置信息，其反映不同目标在图像中的位置及相应分类等信息。但与分类网络的总体实施结构是一致的。

关于SSD的原理，可以参见其论文：Liu W, Anguelov D, Erhan D, et al. **SSD : Single shot multibox detector[C]**. In Proc. European Conference on Computer Vision (ECCV). 2016: 21-37.

### 2.1 准备文件
- `deploy.prototxt`： 网络结构配置文件
- `VGG_VOC0712_SSD_300x300_iter_60000.caffemodel`： 网络权重文件
- `labelmap_voc.prototxt`： 数据集分类名称
- 测试图像

> 
本文的SSD是在`VOC0712`数据集下进行训练的，`labelmap_voc.prototxt`也是该数据库下的各目标的名称，该文件对于目标检测网络的训练任务是必须的，在下节中，我们将重点介绍如何生成LMDB数据库及Labelmap文件。


### 2.2 加载网络

加载网络的方法，目标检测网络同目标分类网络都是一致的。
    caffe_root = '../../'
    # 网络参数（权重）文件
    caffemodel = caffe_root + 'models/SSD_300x300/VGG_VOC0712_SSD_300x300_iter_60000.caffemodel'
    # 网络实施结构配置文件
    deploy = caffe_root + 'models/SSD_300x300/deploy.prototxt'
    labels_file = caffe_root + 'data/VOC0712/labelmap_voc.prototxt'

    # 网络实施分类
    net = caffe.Net(deploy,  # 定义模型结构
                    caffemodel,  # 包含了模型的训练权值
                    caffe.TEST)  # 使用测试模式(不执行dropout)

### 2.3 测试图像预处理

预处理主要包含两个部分：
- 减去均值
- 调整大小
    # 加载ImageNet图像均值 (随着Caffe一起发布的)
    mu = np.load(caffe_root + 'python/caffe/imagenet/ilsvrc_2012_mean.npy')
    mu = mu.mean(1).mean(1)  # 对所有像素值取平均以此获取BGR的均值像素值

    # 图像预处理
    transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
    transformer.set_transpose('data', (2,0,1))
    transformer.set_mean('data', mu)
    transformer.set_raw_scale('data', 255)
    transformer.set_channel_swap('data', (2,1,0))

### 2.4 运行网络
- 导入输入数据
- 通过forward()运行结果

```
# 加载图像
    im = caffe.io.load_image(img)
    # 导入输入图像
    net.blobs['data'].data[...] = transformer.preprocess('data', im)

    start = time.clock()
    # 执行测试
    net.forward()
    end = time.clock()
    print('detection time: %f s' % (end - start))
```

### 2.5 查看目标检测结果

SSD网络的最后一层名为`'detection_out'`，该层输出Blob结构`'detection_out'`中包含了多组元组结构，每个元组结构包含7个参数，其中第2参数表示分类类别序号，第3个参数表示概率置信度，第4~7个参数分别表示目标区域左上及右下的坐标，而元组的个数表明该图像中可能的目标个数。

> 
当然可能不同网络模型的结构不一样，可能会有不同的设置，但至少对于SSD是这样设置的。

    # 查看目标检测结果
    # 打开labelmap_voc.prototxt文件
    file = open(labels_file, 'r')
    labelmap = caffe_pb2.LabelMap()
    text_format.Merge(str(file.read()), labelmap)
    # 得到网络的最终输出结果
    loc = net.blobs['detection_out'].data[0][0]
    confidence_threshold = 0.5
    for l in range(len(loc)):
      if loc[l][2] >= confidence_threshold:
        # 目标区域位置信息
        xmin = int(loc[l][3] * im.shape[1])
        ymin = int(loc[l][4] * im.shape[0])
        xmax = int(loc[l][5] * im.shape[1])
        ymax = int(loc[l][6] * im.shape[0])
        # 画出目标区域
        cv2.rectangle(im, (xmin, ymin), (xmax, ymax), (55 / 255.0, 255 / 255.0, 155 / 255.0), 2)
        # 确定分类类别
        class_name = labelmap.item[int(loc[l][1])].display_name
        cv2.putText(im, class_name, (xmin, ymax), cv2.cv.CV_FONT_HERSHEY_SIMPLEX, 1, (55, 255, 155), 2)

### 2.6 目标检测结果展示

![](http://i.imgur.com/NKyYqnv.png)

### 2.7 具体代码下载

GitHub仓库[Caffe-Python-Tutorial](https://github.com/tostq/Caffe-Python-Tutorial)中的[detection.py](https://github.com/tostq/Caffe-Python-Tutorial/blob/master/detection.py)

项目地址：[https://github.com/tostq/Caffe-Python-Tutorial](https://github.com/tostq/Caffe-Python-Tutorial)



