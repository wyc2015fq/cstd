# MXNet如何生成.lst文件和.rec文件 - AI之路 - CSDN博客





2017年10月19日 08:07:47[AI之路](https://me.csdn.net/u014380165)阅读数：5506
所属专栏：[MXNet使用及源码解读](https://blog.csdn.net/column/details/19485.html)









MXNet框架用于做图像相关的项目时，读取图像主要有两种方式：**第一种是读.rec格式的文件**，类似Caffe框架中LMDB，优点是.rec文件比较稳定，移植到别的电脑上也能复现，缺点是占空间（.rec文件的大小基本上和图像的存储大小差不多），而且增删数据不大灵活。**第二种是.lst和图像结合的方式**，首先在前面生成.rec文件的过程中也会生成.lst文件，这个.lst文件就是图像路径和标签的对应列表，也就是说通过维护这个列表来控制你训练集和测试集的变化，优点是灵活且不占空间，缺点是如果图像格式不符合要求的话容易出错而且如果列表中的某些图像路径对应的图像文件夹中图像被删除，就寻找不到，另外如果你不是从固态硬盘上读取图像的话，速度会很慢。

**1、.rec**
**主要分两步：生成.lst和生成.rec**

**1.1、生成.lst**
**需要准备的就是你的图像。**假设你的图像数据放在/home/image文件夹下，一共有10个类别，那么在/home/image文件夹下应该有10个子文件夹，每个子文件夹放属于这个类的图像文件，你可以用英文名命名这些子文件夹来表达类别，这个都无所谓，即便用1到10这10个数字来分别命名这10个子文件夹也没什么，只不过用英文名会方便你记忆这个文件夹包含的图像是属于哪个类别的。另外假设你要将生成的.lst文件放在/home/lst文件夹下，你的mxnet项目的路径是~/incubator-mxnet，那么运行下面的命令就可以生成.lst文件：
`python ~/incubator-mxnet/tools/im2rec.py --list True --recursive True --train-ratio 0.9 /home/lst/data  /home/image`
–list参数必须要是True，说明你是要生成.lst文件，–recursive参数必须为True，表示要将所有图像路径写进成.lst文件，–train-ratio参数表示将train和val以多少比例划分，默认为1，表示都是train的数据。

这样在/home/lst文件夹下就会生成data_train.lst和data_val.lst两个文件。

![这里写图片描述](https://img-blog.csdn.net/20171019080344199?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.lst文件样例：第一列是index，第二列是label，第三列是图像路径

![这里写图片描述](https://img-blog.csdn.net/20171019080400369?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**当然有时候可能你的数据图像不是按照一个类别放在一个文件夹这种方式，那么就要考虑修改这个脚本来生成相同格式的.lst文件才能用于后续生成.rec文件。**

**1.2、生成.rec**
**需要准备的就是第一步生成的.lst文件和你的图像。**

假设你要将生成的.rec文件放在.lst文件相同的/home/lst文件夹下（一般都会这样操作），那么运行下面的命令就可以生成.rec文件：`python ~/incubator-mxnet/tools/im2rec.py --num-thread 4 /home/lst /home/image`
**这里倒数第二个参数：/home/lst是你的.lst文件所放的路径，可以不用指明.lst文件名称，因为代码会自动搜索/home/lst文件夹下所有以.lst结尾的文件。最后一个参数：/home/image是你的图像所放的路径。**–num-thread 4 这个参数是表示用4个线程来执行，当你数据量较大的时候，生成.rec的过程会比较慢，所以这样可以加速。

运行成功后，在/home/rec文件夹下就生成了data_train.rec和data_val.rec文件，然后就可以用**mxnet.io.ImageRecordIter**类来导入.rec文件了。

![这里写图片描述](https://img-blog.csdn.net/20171019080544821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

另外还会生成两个.idx文件，可忽略。

**2、.lst + 图像**

这里.lst文件的生成和前面1.1部分的一样。然后用**mxnet.image.ImageIter**类来导入.lst和图像数据。可以参考博客：[MXNet如何用mxnet.image.ImageIter直接导入图像](http://blog.csdn.net/u014380165/article/details/74906061)




