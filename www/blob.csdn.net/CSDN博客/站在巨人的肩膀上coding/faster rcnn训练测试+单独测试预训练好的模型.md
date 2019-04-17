# faster rcnn训练测试+单独测试预训练好的模型 - 站在巨人的肩膀上coding - CSDN博客





2018年01月12日 19:01:19[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：4201









Faster R-CNN是一种object detection算法，这里记录下Faster R-CNN的Caffe实现。 

git地址：[https://github.com/rbgirshick/py-faster-rcnn](https://github.com/rbgirshick/py-faster-rcnn)

**这里主要讲两个方面内容，一方面是从无到有如何成功运行demo（假设你的Caffe已经可以用），另一方面是如何自己训练模型，可以用自己的数据，也可以是本文的VOC数据集。**

**一、运行demo**

直接按照git上的readme进行安装和编译即可，由于git上的说明较为简单，所以接下来详细介绍：

**1、Clone the faster r-cnn**

注意参数–recursive（递归），要不caffe-fast-rcnn文件夹就是空的，看看git的目录就知道为什么了。另外假设你下载下来的文件名字叫py-faster-rcnn，接下来所有的操作都是在这个文件夹下面进行，所以请记住这个名字。

```php
git clone --recursive https://github.com/rbgirshick/py-faster-rcnn.git
```
- 1

**2、拷贝Makefile.config**

```
cd $FRCN_ROOT/caffe-fast-rcnn
cp Makefile.config.example Makefile.config
```
- 1
- 2

这里要修改Makefile.config两处地方： 

**2.1、将makefile.config里面的以下两行：**

```
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib
```
- 1
- 2

换成这两行：其实就是加上了hdf5的路径

```
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial/
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib /usr/lib/x86_64-linux-gnu/hdf5/serial/
```
- 1
- 2

否则在编译caffe的时候遇到这样的错误

![这里写图片描述](https://img-blog.csdn.net/20170524194151218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**2.2、将makefile.config里面的 WITH_PYTHON_LAYER :=1前面的注释去掉**，这是因为faster R-CNN是要Python接口的，所以这一项要有，不能注释。


否则会在运行demo.py的时候遇到这样的错误：

![这里写图片描述](https://img-blog.csdn.net/20170524194218538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**3、build the Cython modules**

```
cd py-faster-rcnn/lib
make
```
- 1
- 2

**4、build Caffe and pycaffe**

```
cd py-faster-rcnn/caffe-fast-rcnn
make -j32 && make pycaffe
```
- 1
- 2

前面一个是编译Caffe，后面一个是编译python接口，&&表示在前面运行成功的前提下执行后面

**5、下载Faster R-CNN的预训练模型**

```
cd py-faster-rcnn
./data/scripts/fetch_faster_rcnn_models.sh
```
- 1
- 2

**6、运行demo.py**

```
cd py-faster-rcnn
./tools/demo.py
```
- 1
- 2

**二、自己训练模型**

**1、先下载数据**

假设用VOC数据集来训练，如何下载请参考另一篇博文：[YOLO算法的Caffe实现](http://blog.csdn.net/u014380165/article/details/72553074)。下载好了以后把该文件夹放在/py-faster-rcnn/data/目录下，并改名为VOCdevkit2007，为什么一定要改成这个名字是因为psacal_voc.py里面默认dataset的名字是这样命名，所以你也可以修改pascal_voc.py内容，但是相比之下改文件夹名字更简单。这里面主要包含三个文件夹：Annotations文件夹里面是.xml文件，放的是对应图片的坐标和尺寸信息。JOEGImage文件夹里面放的就是图片。ImageSets文件夹下的Main文件夹放的train.txt，text.txt主要是训练和测试的图像名称及路径。

**2、下载预训练模型**

因为faster R-CNN的训练是在一个已训练的模型上fine-tuning的，所以需要先下载一个预训练模型。下载地址：[https://pan.baidu.com/s/1hsxx8OW](https://pan.baidu.com/s/1hsxx8OW)，下载的压缩文件叫imagenet_models.rar，下载好了以后把它解压到/py-faster-rcnn/data/ 下面，这个imagenet_models文件夹下面包含3个caffemodel：

![这里写图片描述](https://img-blog.csdn.net/20170524194355112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**3、修改文件**

这一步主要是当你需要针对自己的数据集进行训练时候，你需要自己制作数据并且修改文件中的类别信息。**如果你还是用pascal VOC数据集来训练，那么这一步可以跳过。**一方面是制作数据：自己制作的数据替换原来VOCdevkit2007文件夹下面的三个文件夹：Annotations，JOEGImage和ImageSets。另一方面是修改文件：主要是solver.prototxt，train.prototxt，text.prototxt等，这些文件都在/py-faster-rcnn/models/下面，根据不同数据集和不同模型以及不同训练方式分门别类地放在不同目录下。以/py-faster-rcnn/models/pascal_voc/ZF/faster_rcnn_end2end/train.prototxt的修改为例，如下图的数据层，这里因为pascal
 voc数据集包含20个类别，然后模型中需要用到这20个类别和一个背景类，所以最后一个参数num_classes是21。

![这里写图片描述](https://img-blog.csdn.net/20170524194425821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

还有如下的全连接层：前面一个是classification的输出，21（20+1）个类别，后面一个是regression的输出，84（21*4）个坐标。所以如果你实际的物体类别是5个，那么相应的这些类别要改成6（加上背景），坐标的话就是24。

![这里写图片描述](https://img-blog.csdn.net/20170524194438394?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**4、训练模型**

首先看下py-faster-rcnn/experiments/scripts/文件夹下面主要包含三个脚本，分别是faster RCNN的交替训练版本，faster RCNN的端到端训练版本，fast RCNN训练版本。

![这里写图片描述](https://img-blog.csdn.net/20170524194538271?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

以faster RCNN交替训练版本为例，进到py-faster-rcnn文件夹，然后运行 
`./experiments/scripts/faster_rcnn_alt_opt.sh 0 ZF pascal_voc`

这个命令的意思是运行faster_rcnn_alt_opt.sh这个脚本，后面的0表示GPU的ID，ZF表示网络的名字，pascal_voc表示dataset，这三个参数都是在这个.sh脚本的开头定义好了，如下截图：

![这里写图片描述](https://img-blog.csdn.net/20170524194621943?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**看一下faster_rcnn_alt_opt.sh里面的主要内容，**如下图：上面一半是train用，下面一半是test用。在train中指明了训练的.py脚本，GPU的id，网络的名字，预训练的模型，数据等。

![这里写图片描述](https://img-blog.csdn.net/20170524194636462?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**错误：**运行`./experiments/scripts/faster_rcnn_alt_opt.sh 0 ZF pascal_voc`出错：

![这里写图片描述](https://img-blog.csdn.net/20170524194647725?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**解决：**很明显是找不到text_format，所以在/py-faster-rcnn/tools/下面找到对应的train_****.py。比如你是用faster rcnn的交替训练，那么打开train_faster_rcnn_alt_opt.py，在开头添加一句：`import google.protobuf.text_format`。然后再运行
```
./experiments/scripts/faster_rcnn_alt_opt.sh 0
 ZF pascal_voc
```
 就可以成功运行了，如下图：

![这里写图片描述](https://img-blog.csdn.net/20170524194659178?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**5、测试模型**

如第4步中的第二个图，在训练结束后会在训练好的模型上测试结果，如下图：用test_net.py脚本来测试，def表示测试的网络结构test.prototxt，net表示已经训练好的Faster RCNN模型，imdb表示测试数据，cfg表示配置文件。

![这里写图片描述](https://img-blog.csdn.net/20170709222417861?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

也可以直接在命令行中直接运行如下命令（在项目的根目录下运行）：
`time ./tools/test_net.py --gpu 0 --def models/pascal_voc/VGG16/faster_rcnn_end2end/test.prototxt --net output/faster_rcnn_end2end/voc_2007_trainval/vgg16_faster_rcnn_iter_70000.caffemodel --imdb voc_2007_test --cfg experimets/cfgs/faster_rcnn_end2end.yml`- 1

如下图表示在依次测试测试集中的图像，4952表示测试集中的图像数量，0.136表示测试一张图像所用的时间。毕竟是VGG网络，测试时间比较长。

![这里写图片描述](https://img-blog.csdn.net/20170709222539288?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

测试时候遇到过类似这样的错误，本来测试的结果应该写到相应的txt文件中，但是这个错误表示找不到测试结果的txt文件。

![这里写图片描述](https://img-blog.csdn.net/20170709222610822?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

解决的办法一方面可以手动在data目录下新建一个文件目录~/data/VOCdevkit2007/results/VOC2017/Main/ 

或者在~/lib/datasets/pascal_voc.py脚本中添加这两行：表示如果目录不存在，则新建目录。

![这里写图片描述](https://img-blog.csdn.net/20170709222638890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后的测试结果如下，AP表示评价指标mAP，这里列出了每个类别的mAP值以及最后的均值

![这里写图片描述](https://img-blog.csdn.net/20170709222655964?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

并且会在你的数据集目录下生成一个annotations_cache文件夹（比如在我这边就是~/py-faster-rcnn/data/VOCdevkit2007/annotations_cache/annots.pkl），里面有个annots.pkl文件详细记录了每个测试图像的检测结果信息。





