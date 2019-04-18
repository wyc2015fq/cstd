# MXNet如何用mxnet.image.ImageIter直接导入图像 - AI之路 - CSDN博客





2017年07月09日 21:07:45[AI之路](https://me.csdn.net/u014380165)阅读数：4602标签：[脚本																[MXNet																[源码](https://so.csdn.net/so/search/s.do?q=源码&t=blog)
个人分类：[MXNet](https://blog.csdn.net/u014380165/article/category/6996511)

所属专栏：[MXNet使用及源码解读](https://blog.csdn.net/column/details/19485.html)](https://so.csdn.net/so/search/s.do?q=MXNet&t=blog)




**MXNet的图像数据导入模块主要有mxnet.io.ImageRecordIter和mxnet.image.ImageIter两个类，前者主要用来读取.rec格式的数据，后者既可以读.rec格式文件，也可以读原图像数据。**这里先介绍mxnet.image.ImageIter这个类，image是个package，脚本image.py可以在~/mxnet/python/mxnet/image.py找到，里面是各个函数和类的具体实现细节。另外，这个包的各个函数的介绍可以看官网地址：[http://mxnet.io/api/python/io.html#api-reference](http://mxnet.io/api/python/io.html#api-reference)。

下文提到的~/mxnet就是你从mxnet的官方git上拉取下来的项目的文件名。git地址：[https://github.com/dmlc/mxnet](https://github.com/dmlc/mxnet)。

这里先提另外一个函数：mxnet.image.imdecode(buf, **kwargs) # 将图像编码成NDArray格式，我们知道在MXNet框架中，数据存储为NDArray格式，图像数据也是如此，因此mxnet.image中的很多函数的输入输出都是NDArray格式。

**mxnet.image.ImageIter是一个非常重要的类。在MXNet中，当你要读入图像数据时，可以用im2rec.py生成lst和rec文件，然后用mxnet.io.ImageRecordIter类来读取rec文件或者用这个mxnet.image.ImageIter类来读取rec文件，但是这个函数和前者相比还能直接读取图像文件，这样就可以不用生成占内存的rec文件了，只需要原图像文件和lst文件即可。**另外，在mxnet.io.ImageRecordIter中对于数据的预处理操作都是固定的，不好修改，但是mxnet.image.ImageIter却可以非常灵活地添加各种预处理操作。接下来看看这个类。

```
mxnet.image.ImageIter(batch_size, data_shape, label_width=1, path_imgrec=None, path_imglist=None,
 path_root=None, path_imgidx=None, shuffle=False, part_index=0, num_parts=1, aug_list=None, imglist=None,
  data_name='data', label_name='softmax_label', **kwargs)
```

```
参数：
  ● batch_size (int) – Number of examples per batch.
  ● data_shape (tuple) – Data shape in (channels, height, width) format. For now, only RGB image with 3 channels is supported.
  ● label_width (int, optional) – Number of labels per example. The default label width is 1.
  ● path_imgrec (str) – Path to image record file (.rec). Created with tools/im2rec.py or bin/im2rec.
  ● path_imglist (str) – Path to image list (.lst). Created with tools/im2rec.py or with custom script. Format: Tab separated record of index, one or more labels and relative_path_from_root.
  ● imglist (list) – A list of images with the label(s). Each item is a list [imagelabel: float or list of float, imgpath].
  ● path_root (str) – Root folder of image files.
  ● path_imgidx (str) – Path to image index file. Needed for partition and shuffling when using .rec source.
  ● shuffle (bool) – Whether to shuffle all images at the start of each iteration or not. Can be slow for HDD.
  ● part_index (int) – Partition index.
  ● num_parts (int) – Total number of partitions.
  ● data_name (str) – Data name for provided symbols.
  ● label_name (str) – Label name for provided symbols.
  ● kwargs – More arguments for creating augmenter. See mx.image.CreateAugmenter.
```

**官方文档中一个ImageIter类的使用例子：**

```
data_iter = mx.image.ImageIter(batch_size=4, data_shape=(3, 227, 227),
                              path_imgrec="./data/caltech.rec",
                              path_imgidx="./data/caltech.idx" )

# data_iter的类型是mxnet.image.ImageIter
#reset()函数的作用是：resents the iterator to the beginning of the data
data_iter.reset()

#batch的类型是mxnet.io.DataBatch，因为next()方法的返回值就是DataBatch
batch = data_iter.next()

#data是一个NDArray，表示第一个batch中的数据，因为这里的batch_size大小是4，所以data的size是4*3*227*227
data = batch.data[0]

#这个for循环就是读取这个batch中的每张图像并显示
for i in range(4):
    plt.subplot(1,4,i+1)
    plt.imshow(data[i].asnumpy().astype(np.uint8).transpose((1,2,0)))
plt.show()
```

**在mxnet.image.ImageIter中要灵活添加预处理可以通过mxnet.image.CreateAugmenter()函数，**这个函数完整的定义是这样的：
`mxnet.image.CreateAugmenter(data_shape, resize=0, rand_crop=False, rand_resize=False, rand_mirror=False, mean=None, std=None, brightness=0, contrast=0, saturation=0, pca_noise=0, inter_method=2)`
这个函数具体的内容可以参看：~/mxnet/python/mxnet/image.py

**具体要怎么做呢？就是修改data.py！**路径是:~/mxnet/example/image-classification/common/data.py。我在另一篇博文：[MXNet的data.py源码详解](http://blog.csdn.net/u014380165/article/details/73928835)里面已经解读过这个脚本，因为这个脚本“掌握”着数据进来的入口，所以需要仔细研读。 

找到data.py脚本中的get_rec_iter()函数，可以看到get_rec.iter()里面有这一部分：

```
train = mx.io.ImageRecordIter(
        path_imgrec         = args.data_train,
        label_width         = 1,
        mean_r              = rgb_mean[0],
        mean_g              = rgb_mean[1],
        mean_b              = rgb_mean[2],
        data_name           = 'data',
        label_name          = 'softmax_label',
        data_shape          = image_shape,
        batch_size          = args.batch_size,
        rand_crop           = args.random_crop,
        max_random_scale    = args.max_random_scale,
        pad                 = args.pad_size,
        fill_value          = 127,
        min_random_scale    = args.min_random_scale,
        max_aspect_ratio    = args.max_random_aspect_ratio,
        random_h            = args.max_random_h,
        random_s            = args.max_random_s,
        random_l            = args.max_random_l,
        max_rotate_angle    = args.max_random_rotate_angle,
        max_shear_ratio     = args.max_random_shear_ratio,
        rand_mirror         = args.random_mirror,
        preprocess_threads  = args.data_nthreads,
        shuffle             = True,
        num_parts           = nworker,
        part_index          = rank)
```

**这段代码就是从rec文件读取数据的过程。现在我们不用mx.io.ImageRecordIter()，而是改用mx.image.ImageIter()，修改如下:**

```
train = mx.image.ImageIter(
        batch_size            = args.batch_size,
        data_shape          = (3,224,224),
        label_width           = 1,
        path_imglist          = args.data_train,
        path_root              = args.image_train,
        part_index            = rank,
        shuffle                  = True,
        data_name           = 'data',
        label_name           = 'softmax_label',
        aug_list                 = mx.image.CreateAugmenter((3,224,224),resize=224,rand_crop=True,rand_mirror=True,mean=True))
```

**这里的path_imglist参数和path_root参数是这个类特有的，分别表示.lst文件和图像的路径，这个.lst文件就是你在生成.rec文件时候要用到的，因此这个类只是不需要.rec文件，但是.lst文件还是需要的，只是一个列表文件，大大节省了存储空间，也方便以后对数据的增删改变，因为只要重新生成.lst文件即可，而不需要花时间生成占空间的.rec文件。另外因为原本的data.py脚本中没有args.image_train，所以你需要自己添加，就按其他args的一样来添加即可，这样就可以导入这个参数了。**

val部分可以类似修改，这里最重要的就是最后一个参数aug_list，表示所有预处理的列表，不过在val中一般不会有类似crop，mirror等操作。为什么会用到aug_list这个参数呢？来自于image.py脚本中ImageIter类的**init**()函数的这几行代码：

```
if aug_list is None:
     self.auglist = CreateAugmenter(data_shape, **kwargs)
else:
     self.auglist = aug_list
```

**什么意思呢？就是如果aug_list这个参数没有赋值（默认是None），那么就不对图像做预处理；如果这个参数有值，那么就调用CreateAugmenter()函数生成预处理列表。**

**因此讲了这么多，最后这个类的具体实现才是重点。数据处理的内容还有很多，接下来我会写一篇博文详细介绍image.py脚本，尤其是CreateAugmenter()函数，让大家对MXNet中的数据预处理有更深入的认识。**](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)




