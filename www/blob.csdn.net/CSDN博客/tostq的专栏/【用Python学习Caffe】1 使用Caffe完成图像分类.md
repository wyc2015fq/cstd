# 【用Python学习Caffe】1. 使用Caffe完成图像分类 - tostq的专栏 - CSDN博客





2017年06月22日 22:01:47[tostq](https://me.csdn.net/tostq)阅读数：6981
所属专栏：[用Python学习Caffe](https://blog.csdn.net/column/details/16089.html)









## 1. 使用Caffe完成图像分类

本节将以著名的图像分类深度学习网络AlexNet为例子，通过Python Caffe来进行图像分类。虽然不同的网络的结构是不样的，但其大体的过程都是一致的，因此大家可以通过这个例子，熟悉如何利用Caffe进行图像分类。

关于AlexNet的原理，可以参见其论文：Krizhevsky A, Sutskever I, Hinton G E. **Imagenet classification with deep convolutional neural networks[C]**. In Proc. Advances in Neural Information Processing Systems (NIPS). 2012: 1097-1105.

基于深度学习的图像分类网络，一般是将原始图像经过深度网络后，最后将得到一组向量，这组向量的长度等于分类种类的数目，向量中每个值对应于该图像在相应的分类种类下的概率。因此通常的情况将向量最大值对应的序号视为最可能的分类。

### 1.1 准备文件
- `deploy.prototxt`： 网络结构配置文件
- `bvlc_alexnet.caffemodel`： 网络权重文件
- `synset_words.txt`： 分类名称
- 测试图像

> 
本文的AlexNet是在`ilsvrc12`下进行训练的，`bvlc_alexnet.caffemodel`是在该数据库下训练得到的权重文件，而`synset_words.txt`也是该数据库下的分类种类名称，该文件的每一行代表一种类，每一种类可能会有多个相似的名称，当然近似名称并不重要，但要知道每一行代表一类，行号则代表其在网络最终输出向量的序号，举个例子，比如说，如果最终输出向量的第50个值最大，则代表该图像的分类为`synset_words.txt`第50行表示的分类。


### 1.2 加载网络

```
caffe_root = '../../'
    # 网络参数（权重）文件
    caffemodel = caffe_root + 'models/bvlc_alexnet/bvlc_alexnet.caffemodel'
    # 网络实施结构配置文件
    deploy = caffe_root + 'models/bvlc_alexnet/deploy.prototxt'
    net = caffe.Net(deploy,  # 定义模型结构
                    caffemodel,  # 包含了模型的训练权值
                    caffe.TEST)  # 使用测试模式(不执行dropout)
```

### 1.3 测试图像预处理

预处理主要包含两个部分：
- 减去均值
- 调整大小

```
# 加载ImageNet图像均值 (随着Caffe一起发布的)
    mu = np.load(caffe_root + 'python/caffe/imagenet/ilsvrc_2012_mean.npy')
    mu = mu.mean(1).mean(1)  # 对所有像素值取平均以此获取BGR的均值像素值

    # 图像预处理
    transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
    transformer.set_transpose('data', (2,0,1))
    transformer.set_mean('data', mu)
    transformer.set_raw_scale('data', 255)
    transformer.set_channel_swap('data', (2,1,0))
```

### 1.4 运行网络
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
    print('classification time: %f s' % (end - start))
```

### 1.5 查看分类结果

用于图像分类的网络的最后一层一般是一个名为’prob’的SoftMax网络，这个名为’prob’层的输出即为反应该图像在各分类下的概率向量。而prob层的输出Blob也名为prob
    layer {
      name: "prob"
      type: "Softmax"
      bottom: "ip2"
      top: "prob" # 输出Blob
    }

因此我们可以通过`net.blobs['prob']`确定最终的输出结果。找到该向量的最大值即可确定最可能的分类，当然对于图像可能存在多个目标的情况下，可以提取最大概率的多个分类。

```
# 查看目标检测结果
    # 加载分类名称文件
    labels = np.loadtxt(synset_words, str, delimiter='\t')
    # 得到分类网络的最终结果
    category = net.blobs['prob'].data[0].argmax() # 最大概率的分类
    # 得到分类名称
    class_str = labels[int(category)].split(',')
    class_name = class_str[0]
    # 在图像中标记分类名称
    cv2.putText(im, class_name, (0, im.shape[0]), cv2.cv.CV_FONT_HERSHEY_SIMPLEX, 1, (55, 255, 155), 2)
```

### 1.6 分类结果展示

![](http://i.imgur.com/vctQzrU.png)

### 1.7 具体代码下载

GitHub仓库[Caffe-Python-Tutorial](https://github.com/tostq/Caffe-Python-Tutorial)中的[classification.py](https://github.com/tostq/Caffe-Python-Tutorial/blob/master/classification.py)



