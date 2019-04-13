
# ChainerCV︱堪比Opencv--深度学习工具库（Faster R-CNN、SSD 和 SegNet） - 素质云笔记-Recorder... - CSDN博客

2017年09月03日 21:30:49[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2379



> Preferred Networks 通过其研究博客发布了深度学习计算机视觉实用库 ChainerCV，它基于

> Chainer，能够简化计算机视觉的训练和应用过程，并含有用于计算机视觉研究的必备工具集。 ChainerCV

> 提供了目标检测和语义分割模型（Faster R-CNN、SSD 和 SegNet）的实现。 ChainerCV

> 具有大量的已训练权重，可在运行时自动从网络上下载，因此用户无需担心下载或者记住已训练权重的文件位置。同样，ChainerCV

> 提供了简单统一的界面，从而为解决相同任务的不同模型执行推理。例如，Faster R-CNN 和 SSD

> 都具备一个被称作「预测」的方法，它可以接收图像并返回物体检测的结果。

> 参考机器之心：
> [资源 | ChainerCV：基于Chainer的深度学习计算机视觉实用库（内含工具集）](https://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650729880&idx=3&sn=e612c8e2d74f31f7eb078110b8882296&chksm=871b29e6b06ca0f0980a04a4820fd79053b77c23def30d6c2d770d4269d36312328731508bdf&mpshare=1&scene=1&srcid=08167UOgcg1sDUv8XWPwjWXR#rd)

> GitHub 地址：
> [https://github.com/chainer/chainercv](https://github.com/chainer/chainercv)

> 文档：
> [http://chainercv.readthedocs.io/en/stable/](http://chainercv.readthedocs.io/en/stable/)

> ChainerCV 包含了一组指导计算机视觉研究的工具：

> 为一般视觉数据集（如 PASCAL VOC 任务的目标识别数据集）而构建的数据集加载器

> 能用于数据预处理/数据增强的转换

> 可视化

> 一般度量方法的评估代码
![这里写图片描述](https://img-blog.csdn.net/20170903211701206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> .

> 一、基本介绍

> 1、安装

> pip
> install
> chainercv
> 2、一个DEMO——SSD物体检测的案例

> 20分类，voc07数据集训练，VGG16为基本

> from
> chainercv.links import FasterRCNNVGG16, SSD300
\# You can use Faster R-CNN instead
> of
> SSD.
\# model = FasterRCNNVGG16(pretrained_model=
> 'voc07'
> )
model = SSD300(pretrained_model=
> 'voc0712'
> )

\# `bboxes`
> is
> a list
> of
> numpy arrays containing coordinates
> of
> boundnig boxes
\# around objects. `labels`
> and
> `scores` are
> class
> ids
> and
> confidence scores
> for
> \# the boxes.
bboxes, labels, scores = model.predict(imgs)  \# imgs
> is
> a list
> of
> image
> 3、转化数据到指定训练格式

> from
> chainer.datasets
> import
> get_mnist
> from
> chainer.datasets
> import
> TransformDataset
> from
> chainercv.transforms
> import
> random_rotate
dataset, _ = get_mnist(ndim=
> 3
> )
> def
> transform
> (in_data)
> :
> \# in_data is values returned by __getitem__ method of MNIST dataset.
> img, label = in_data
    img -=
> 0.5
> \# rescale to [-0.5, 0.5]
> img = random_rotate(img)
> return
> img, label
dataset = TransformDataset(dataset, transform)
img, label = dataset[
> 0
> ]
> 4、库中拥有的数据集

> TransformDataset

> CamVid

> CUB

> OnlineProducts

> PASCAL VOC

> .

> 5、如何使用GPU

> 下载cupy库，
> pip install cupy
> ， 当然预先需要配置好CUDNN

> chainer
> .cuda
> .get
> _device_from_id(设备标号)
> .use
> ()
model
> .to
> _gpu()
> \# Copy the model to the GPU
> .

> 二、用chainer实现图像分割SegNet

> 码云链接：
> [https://gitee.com/mattzheng/chainercv_SegNet/tree/master](https://gitee.com/mattzheng/chainercv_SegNet/tree/master)

> github链接：
> [https://github.com/mattzheng/chainer_SegNet](https://github.com/mattzheng/chainer_SegNet)

> 相关code都放在上面了.

> 1、 图像分割SegNet

> 相关参考

> github链接：
> [https://github.com/chainer/chainercv](https://github.com/chainer/chainercv)

> 官方文档链接：
> [http://chainercv.readthedocs.io/en/stable/index.html](http://chainercv.readthedocs.io/en/stable/index.html)

> 预训练模型下载页面：
> [https://github.com/yuyu2172/share-weights/releases/](https://github.com/yuyu2172/share-weights/releases/)

> SegNet参考的caffe实现及预训练模型：
> [http://mi.eng.cam.ac.uk/projects/segnet/tutorial.html](http://mi.eng.cam.ac.uk/projects/segnet/tutorial.html)

> 由chainercv自带的预训练模型

> 采用的数据集为：camvid

> 数据集类别camvid_label_names：’Sky’,  ‘Building’,  ‘Pole’,  ‘Road’,  ‘Pavement’,  ‘Tree’,  ‘SignSymbol’,  ‘Fence’,  ‘Car’,  ‘Pedestrian’,  ‘Bicyclist’

> 不同类别的颜色camvid_label_colors：(128, 128, 128),  (128, 0, 0),  (192, 192, 128),  (128, 64, 128),  (60, 40, 222),  (128, 128, 0),  (192, 128, 128),  (64, 64, 128),  (64, 0, 128),  (64, 64, 0),  (0, 128, 192)

> 其中需要注意的是：

> (1)’pip install chainercv’好像没有load进去vis_semantic_segmentation模块，所以我的做法是从github中加到：/usr/local/lib/python3.5/dist-packages/chainercv/visualizations目录下（github该模块链接：
> [https://github.com/chainer/chainercv/tree/master/chainercv/visualizations](https://github.com/chainer/chainercv/tree/master/chainercv/visualizations)
> ）

> (2)读图的时候，注意最好使用chainercv自带的读入函数utils.read_image

> code展示：

> import
> argparse
> import
> matplotlib.pyplot
> as
> plot
> import
> chainer
> from
> chainercv.datasets
> import
> camvid_label_colors
> from
> chainercv.datasets
> import
> camvid_label_names
> from
> chainercv.links
> import
> SegNetBasic
> from
> chainercv
> import
> utils
> from
> chainercv.visualizations
> import
> vis_image
> from
> chainercv.visualizations.vis_semantic_segmentation
> import
> vis_semantic_segmentation
> \# pre-model + read picture
> img = utils.read_image(
> '../../iamge.jpg'
> , color=
> True
> )
model = SegNetBasic(n_class =
> 11
> ,pretrained_model  =
> 'camvid'
> )
labels = model.predict([img])
> \# plot
> label = labels[
> 0
> ]
> \# (332, 500)
> fig = plot.figure()
ax1 = fig.add_subplot(
> 1
> ,
> 2
> ,
> 1
> )
vis_image(img, ax=ax1)
ax2 = fig.add_subplot(
> 1
> ,
> 2
> ,
> 2
> )
vis_semantic_segmentation(label, camvid_label_names, camvid_label_colors, ax=ax2)
plot.show()
> SegNetBasic函数解析（来源
> [文档](http://chainercv.readthedocs.io/en/stable/reference/links/segnet.html#segnetbasic)
> ）：

> camvid数据是类别较少的分类。

> SegNetBasic(n_class=None, pretrained_model=None, initialW=None)函数，有三个参数，n_class是预训练模型的参数，pretrained_model是预训练模型位置

> predict(imgs),imgs输入array的list

> .

> 2、自己训练segnet模型

> 其他的，如果你要自己训练segnet模型，请参考
> [该页面](https://github.com/chainer/chainercv/tree/master/examples/segnet)

> First, move to this directory (i.e., examples/segnet) and run:

> python
> train
> .
> py
> [
> -
> -
> gpu
> <
> gpu
> >
> ]
> 一个使用的demo：

> wget https://raw
> .githubusercontent
> .com
> /alexgkendall/SegNet-Tutorial/master/CamVid/test/
> 0001
> TP_008550
> .png
> python demo
> .py
> [--gpu <gpu>] [--pretrained_model <model_path>]
> 0001
> TP_008550
> .png
> 模型评估的函数：

> python
> eval_camvid
> .
> py
> [
> -
> -
> gpu
> <
> gpu
> >
> ]
> [
> -
> -
> pretrained_model
> <
> model_path
> >
> ]
> [
> -
> -
> batchsize
> <
> batchsize
> >
> ]
> 这里有一个已经训练好的模型，可以做个案例，预训练模型的下载链接为：
> [https://www.dropbox.com/s/exas66necaqbxyw/model_iteration-16000](https://www.dropbox.com/s/exas66necaqbxyw/model_iteration-16000)

> 评估的结果展示：
![这里写图片描述](https://img-blog.csdn.net/20170927112043613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 官方自带的一个预训练模型后的模型使用demo：

> import
> argparse
> import
> matplotlib.pyplot
> as
> plot
> import
> chainer
> from
> chainercv.datasets
> import
> camvid_label_colors
> from
> chainercv.datasets
> import
> camvid_label_names
> from
> chainercv.links
> import
> SegNetBasic
> from
> chainercv
> import
> utils
> from
> chainercv.visualizations
> import
> vis_image
> from
> chainercv.visualizations
> import
> vis_semantic_segmentation
> def
> main
> ()
> :
> chainer.config.train =
> False
> parser = argparse.ArgumentParser()
    parser.add_argument(
> '--gpu'
> , type=int, default=-
> 1
> )
    parser.add_argument(
> '--pretrained_model'
> , default=
> 'camvid'
> )
    parser.add_argument(
> 'image'
> )
    args = parser.parse_args()
    model = SegNetBasic(
        n_class=len(camvid_label_names),
        pretrained_model=args.pretrained_model)
> if
> args.gpu >=
> 0
> :
        chainer.cuda.get_device_from_id(args.gpu).use()
        model.to_gpu()
    img = utils.read_image(args.image, color=
> True
> )
    labels = model.predict([img])
    label = labels[
> 0
> ]
    fig = plot.figure()
    ax1 = fig.add_subplot(
> 1
> ,
> 2
> ,
> 1
> )
    vis_image(img, ax=ax1)
    ax2 = fig.add_subplot(
> 1
> ,
> 2
> ,
> 2
> )
    vis_semantic_segmentation(
        label, camvid_label_names, camvid_label_colors, ax=ax2)
    plot.show()
> if
> __name__ ==
> '__main__'
> :
    main()

