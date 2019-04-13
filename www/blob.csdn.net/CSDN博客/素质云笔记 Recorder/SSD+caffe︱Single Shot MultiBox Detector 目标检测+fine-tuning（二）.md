
# SSD+caffe︱Single Shot MultiBox Detector 目标检测+fine-tuning（二） - 素质云笔记-Recorder... - CSDN博客

2017年03月29日 19:05:03[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5688



> 承接上一篇SSD介绍：
> [SSD+caffe︱Single Shot MultiBox Detector 目标检测（一）](http://blog.csdn.net/sinat_26917383/article/details/67639189)

> 如果自己要训练SSD模型呢，关键的就是LMDB格式生成，从官方教程
> [weiliu89/caffe](https://github.com/weiliu89/caffe/tree/ssd)
> 来看，寥寥几行code，但是前面的数据整理真实要了老命。

> 教程其实就是一种fine-tuning，在VGG基础上进行训练SSD框架。

> 公众号“素质云笔记”定期更新博客内容：
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> 一般SSD的数据来源，看到好多人都在用这款框架进行数据标注：
> [BBox-Label-Tool](https://github.com/puzzledqs/BBox-Label-Tool)
> 也有人在这基础上进行简单修改，详见博客：
> [SSD框架训练自己的数据集](http://www.cnblogs.com/objectDetect/p/5780006.html)
> ，或者用opencv重写：
> [深度学习ssd配置并在VGG模型上训练自己的数据](https://my.oschina.net/u/1046919/blog/777470)

> 还有一些自己开发的小工具：
> [图像标注VS2013项目](http://download.csdn.net/detail/sinat_30071459/9573982)

> .

> 0 官方教程训练过程：

> （1）下载VGGNet.caffemodel，
> [fully convolutional reduced (atrous) VGGNet](https://gist.github.com/weiliu89/2ed6e13bfd5b57cf81d6)

> （2）下载VOC2007 and VOC2012两个数据集，放在/caffe/data目录下

> （3）创建LMDB文件：

> ./
> data
> /
> VOC0712
> /create_list.sh
> ./
> data
> /
> VOC0712
> /create_data.sh
> （4）训练

> python examples/ssd/ssd_pascal
> .py
> 以上是官方教程常规训练步骤，并不难。但是自己训练时候的数据处理，比较麻烦。

> .

> 1 数据集准备与形成

> 1.1 所需准备文件介绍

> 其中训练所需的文件夹包括三大类：
![这里写图片描述](https://img-blog.csdn.net/20170329181900917?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 笔者自己改写了一份纯python版本的，并上传到github：
> [https://github.com/mattzheng/umdfaces2VOC2007/](https://github.com/mattzheng/umdfaces2VOC2007/)

> 该份代码可以直接把下面三个文件夹填满：Annotations、JPEGImages、ImageSets

> 另外的ImageSets简单的用Python来随机抽样即可。

> Annotations

> *
> .xml   （所有的标注的物体信息文件）

> 其中.py代码会生成以下首行内容，是没有关系的

> <?xml version="1.0" encoding="utf-8"?>
> JPEGImages

> *
> .jpg    （所有的图片集）

> ImageSets

> 保存格式为文件名，这里不带扩展名

> 00001
00002
> 训练验证集： trainval.txt，整个数据集的50%

> 测试集：     test.txt，整个数据集的50%

> 训练集：     train.txt，大概是trainval的50%

> 验证集：     val.txt，大概是trainval的50%

> 1.2 训练文件介绍

> （1） create_list.sh:用于生成三类文件列表

> - test_name_size：测试集图像大小

> - test：测试集图像-标签一一对应

> - trainval：训练集图像-标签一一对应

> 这是将数据集做成方便之后生成lmdb的路径信息文件，注意三点：

> 1、root_dir路径，文件夹名称的上一级，譬如：/home/caffe-master/ssd/data

> 2、文件夹名称，VOC2007,本来默认这边填了两个文件夹。

> 3、get_image_size.cpp

> 工具位置，在41行左右。笔者在自己改了路径之后，一直找不到于是就自己定义了绝对路径。一般在：/home/caffe-master/ssd、build/tools/get_image_size

> 其余输出文件的示例：

> 图像-标签一一对应：

> Images/
> 000112.
> png Labels/
> 000112.
> xml
> Images/
> 001365.
> png Labels/
> 001365.
> xml
> 同时注意名称的一致。

> 000800 1241 376
006206 1242 375
> test_name_size里面，000800就是图片名称，1241*376就是图片尺寸，高 长。

> （2）labelmap_voc.prototxt：

> 目标检测的标签文件

> 该文件主要记录需要训练识别的n种对象的信息，第一类是background，是不变的，对于Pascal VOC数据集来说，需要识别20种对象，所以后面还有20个label。与传统的机器学习不太一样的是，以前需要输入正负样本，在caffe种，不管是faster rcnn还是ssd，都是只用对训练图像将识别对象的坐标信息标注即可，除了标注信息以外的部分都会当做background去处理。

> 所以，此时只能填一个：

> item
> {
> name
> :
> "none_of_the_above"
> label:
> 0
> display_name:
> "background"
> }
> item
> {
> name
> :
> "face"
> label:
> 1
> display_name:
> "face"
> }
> 这里为什么要填face，这里是有讲究的。要跟xml里面的类别一直，github里面类别写的是脸，所以必须写”face”。另外一个案例label可见：
> [SSD框架训练自己的数据集](http://www.cnblogs.com/objectDetect/p/5780006.html)

> （3）create_data.sh：

> 生成LMDB文件，以上的create_list准备好之后即可。

> 最麻烦还是路径填写。

> （4）ssd_pascal.py

> 可见：
> [Ubuntu上用caffe的SSD方法训练umdfaces数据集](http://blog.csdn.net/u013738531/article/details/61934587)

> 另外一个案例文件ssd_pascal.py可见：
> [SSD框架训练自己的数据集](http://www.cnblogs.com/objectDetect/p/5780006.html)

> 主要还是路径设置问题。

> .

> 2 数据集生成方案

> 训练的数据集三种方案：

> 第一：保持原来的文件目录结构及文件名不变， 只替换里面的数据，通用。

> 第二：重新新建一个与之前类似的目录结构，改成自己命名的文件夹，第二种方法，有一定的风险性，需要修改程序里涉及数据路径的代码。

> 第三：txt格式数据变成LMDB，参考github：
> [https://github.com/jinfagang/kitti-ssd](https://github.com/jinfagang/kitti-ssd)

> 第三种是直接用txt格式保存，就可以生成LMDB的办法，我觉得这个比较好，但是笔者最终没有实验成功…譬如：

> class_index xmin ymin xmax ymax
> 第一种方案里面的一些小技巧：

> （1）还有将txt变成xml格式的小工具：见博客
> [将数据集做成VOC2007格式用于Faster-RCNN训练](http://blog.csdn.net/sinat_30071459/article/details/50723212)

> （2）将BBox-Label-Tool下的txt格式保存的bounding box信息转换成VOC数据格式下以xml方式表示:，见博客：
> [SSD框架训练自己的数据集](http://www.cnblogs.com/objectDetect/p/5780006.html)

> （3）将csv变成xml文件类型

> [luuuyi/umdfaces2VOC2007](https://github.com/luuuyi/umdfaces2VOC2007)

> （4）自己改写的一个txt变为xml的code，见本人的github：
> [https://github.com/mattzheng/forSSD_txt2xml](https://github.com/mattzheng/forSSD_txt2xml)

> 2.1 训练流程

> 本节参考：
> [SSD框架训练自己的数据集](http://www.cnblogs.com/objectDetect/p/5780006.html)

> - 第一步：create_list.sh：生成test_name_size.txt、test.txt、trainval.txt

> - 第二步：修改labelmap_voc.prototxt，标签文件，同时需要注意标签文件必须多一个background的标签，作为编号0

> - 第三步：create_data.sh：生成LMDB

> - 第四步：修改ssd_pascal.py：参考：
> [Ubuntu上用caffe的SSD方法训练umdfaces数据集](http://blog.csdn.net/u013738531/article/details/61934587)

> 第五步训练：

> python examples/ssd/ssd_pascal.py

> 2.2 训练脚本内容存储

> 会在相应的路径下生成jobs文件夹，其中包含了这一次训练的脚本文件，并且会记录caffe执行只一次训练的日志信息。

> 参考：
> [Ubuntu上用caffe的SSD方法训练Pascal VOC数据集](http://jishu.y5y.com.cn/u013738531/article/details/58637760)

> jobs文件夹下有：

> .log文件：运行记录，时间，迭代次数等信息；

> .caffemodel文件，权重文件

> deploy.prototxt、train_val.prototxt等

> .

> 2.3 一些实验数据

> 来源于：
> [Ubuntu上用caffe的SSD方法训练Pascal VOC数据集](http://jishu.y5y.com.cn/u013738531/article/details/58637760)

> 计划训练的迭代次数是12w次，但是在K80上只开了一个核来进行计算，差不多一天可以迭代1w+次吧，跑了6天将近7w次，打断来测试，在K80上，检测单张人脸图片，分辨率在300X300左右，速度为40ms左右，也就是说帧率可以达到25fps，速度还是很快的。至于准确度，在log文件里面，每1w次迭代之后会计算一个mAP，第6w次的时候计算了一下mAP（作者自定义过，跟mAP比较像）为0.965：
![这里写图片描述](https://img-blog.csdn.net/20170504153010499?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170504153010499?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> .

> [
](https://img-blog.csdn.net/20170504153010499?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 参考：

> [深度学习ssd配置并在VGG模型上训练自己的数据](https://my.oschina.net/u/1046919/blog/777470)

> [SSD 安装、训练](http://www.voidcn.com/blog/lilai619/article/p-6352088.html)

> [Ubuntu上用caffe的SSD方法训练umdfaces数据集](http://blog.csdn.net/u013738531/article/details/61934587)

> 重点参考：

> [使用faster rcnn训练umdfaces数据集](http://blog.csdn.net/u013738531/article/details/54316435)

> [将数据集做成VOC2007格式用于Faster-RCNN训练](http://blog.csdn.net/sinat_30071459/article/details/50723212)

> [Caffe-SSD 训练自己的数据集教程](http://www.jianshu.com/p/ebebfcd274e6)

> [SSD框架训练自己的数据集](http://www.cnblogs.com/objectDetect/p/5780006.html)

> .

> 报错一：

> cudasucess
> (
> 10
> vs0)
> 解决：修改pythonssd_pascal.py文件gpus=’0,1,2,3’,如果有一块GPU,则删除123，有两块则删除23

> .

> 延伸一：大致的运算效率

> K80-单GPU-一天可以迭代1w+次

> 6天将近7w次

> 每1w次迭代之后会计算一个mAP

> （数据来源：
> [Ubuntu上用caffe的SSD方法训练umdfaces数据集](http://blog.csdn.net/u013738531/article/details/61934587)
> ）

> TX.P，笔者自己训练，大概的运算效率是5千/小时（运行官方教程时）

> .

> 延伸二：SSD额外的功能——视频检测 + 实时摄像头物体检测

> 参考来源：
> [SSD:Single Shot MultiBox Detector的安装配置和运行](http://www.itdadao.com/articles/c15a586080p0.html)

> - 电脑前置摄像头

> python examples/ssd/ssd_pascal_webcam
> .py
> 视频检测

> python examples/ssd/ssd_pascal_video
> .py

