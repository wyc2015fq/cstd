
# caffe︱ImageData层、DummyData层作为原始数据导入的应用 - 素质云笔记-Recorder... - CSDN博客

2017年02月12日 13:25:10[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：7270



# Part1:caffe的ImageData层
ImageData是一个图像输入层，该层的好处是，直接输入原始图像信息就可以导入分析。
在案例中利用ImageData层进行数据转化，得到了一批数据。
但是笔者现在还有几个问题，
这个ImageData只能显示一个batch的图像信息，不能在同一案例循环使用的是吧？
L.ImageData之后的数据，可以直接用于框架的输入数据吗？（待尝试）
.
一、官方文档中的使用
在案例[《Fine-tuning a Pretrained Network for Style Recognition》](http://nbviewer.jupyter.org/github/BVLC/caffe/blob/master/examples/02-fine-tuning.ipynb)官方文档中，用来作为数据导入的方法：
transform_param = dict(mirror=train,crop_size=227,mean_file=caffe_root +'data/ilsvrc12/imagenet_mean.binaryproto')  
L.ImageData(transform_param=transform_param,source=source,batch_size=50, new_height=256, new_width=256, ntop=2)其中transform_param包含了一些参数：
1、mean_file，数据均值文件存放地
2、source，train_txt的txt文件，其中不论是训练集还是测试集最好有标签，没有标签可以随便标啊，有了这个是为了避免程序出错，每个文档的格式最好都如下（/caffe/data/flickr_style）：
/caffe/data/flickr_style/images/8483553476_79bae22144.jpg 1/caffe/data/flickr_style/images/11801337354_7bf2aa44b4.jpg 03、new_height、new_width（可有可无，但是有了之后就会对图片进行resize）如果new_height or
new_width 不等于0，则会resize图像,最好new_height、new_width、crop_size  三个都标同一数值。
同时crop_size，new_height，new_width 一定要和论文中给出的一致（经验来源于博客：[Caffe C++API 提取任意一张图片的特征系列一—-ImageData](http://blog.csdn.net/sunshine_in_moon/article/details/50125899)）
4、★crop_size
修剪上限，超过227的就会进行修剪，一般height和width与crop_size一致就对了，从原图随机crop的尺寸，crop就是截图，利用crop_size这种方式可以剪裁中心关注点和边角特征
其中crop_size 在训练集与验证集中也不尽相同，如果我们输入的图片尺寸大于crop_size，那么图片会被裁剪。当 phase模式为 TRAIN 时，裁剪是随机进行裁剪，而当为TEST 模式时，其裁剪方式则只是裁剪图像的中间区域。
caffe中提供了过采样的方法（oversample），详见/caffe/python/caffe/io.py，裁剪的是图片中央、4个角以及镜像共10张图片。（该部分见博客：[caffe中使用crop_size剪裁训练图片](http://blog.csdn.net/u010417185/article/details/52651761)）
5、mirror:false，1表示开启镜像，0表示关闭，也可用ture和false来表示，是否要对输入图像采用随机水平镜像，mirror可以产生镜像,弥补小数据集的不足
6、batch_size，就是每一次载入的数量,多了就放不下了
7、shuffle（可有可无，最好打乱） 每一个 epoch是否打乱数据.
该层输入上述参数后，返回：图像修整、载入过的图像特征。
同时，另外一点：mirror+crop_size 代表着对 images 进行水平镜像处理或者随机裁剪处理，这两个操作都可以看做是简单的数据增强操作。（来源于：Caffe官方教程中译本_CaffeCN社区翻译(caffecn.cn)）
.
.
二、用ImageData进行原始图像数据导入
对于ImageData使用方式，我总结的不知道对不对：
1、设置transform_param参数
其中包括：均值文件目录、是否修整mirror、crop_size修整上限
2、ImageData引擎
设置batch_size，height*width图像尺寸，按照框架要求，source代表数据集txt文件
3、扔入框架构建caffenet的框架文件
style_net(train=False, subset='train')不知道这个步骤中，L.ImageData之后的数据，可以直接用于框架的输入数据吗？（待尝试）
4、构建预测引擎
caffe.Net(style_net(train=False, subset='train'),weights, caffe.TEST)5、前馈
untrained_style_net.forward()6、拿出图像
style_data_batch= untrained_style_net.blobs['data'].data.copy()ImageData层源码解读博客:
[caffe源码之ImageDataLayer解析](http://blog.csdn.net/paranoid_cnn/article/details/53172030)
[caffe层解读系列——Data以及ImageData层用法](http://blog.csdn.net/shuzfan/article/details/53185008)
.
.
Part2:caffe中DummyData层的使用
DummyData:虚拟数据，可以用这一层模拟预测过程.在官方案例中，借用DummyData层来较快完成预测任务。使用流程：
利用DummyData设置备选图框，给新来的图像留个坑
dummy_data=L.DummyData(shape=dict(dim=[1, 3, 227, 227]))不同框架size不一致
构建框架文件deploy.prototxt，用caffenet
caffenet(data=dummy_data, train=False)设置预测引擎
net=caffe.Net(imagenet_net_filename, weights, caffe.TEST)其中需要加载框架权值单张预测
disp_imagenet_preds(net, image)其中image是图像的信息，可以是之前ImageData层，也可以是其他转化过的图像特征信息。

