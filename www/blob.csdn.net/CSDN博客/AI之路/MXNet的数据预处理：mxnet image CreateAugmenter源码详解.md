# MXNet的数据预处理：mxnet.image.CreateAugmenter源码详解 - AI之路 - CSDN博客





2017年07月09日 22:12:43[AI之路](https://me.csdn.net/u014380165)阅读数：3038
所属专栏：[MXNet使用及源码解读](https://blog.csdn.net/column/details/19485.html)









**这篇博文要介绍的mxnet.image.CreateAugmenter是承接前面一篇博文：MXNet如何用mxnet.image.ImageIter直接导入图像，在那篇博文中介绍了如何用ImageIter类导入图像，如下代码（修改后的data.py脚本）：**

```
train = mx.image.ImageIter(
        batch_size          = args.batch_size,
        data_shape          = (3,224,224),
        label_width         = 1,
        path_imglist        = args.data_train,
        path_root           = args.image_train,
        part_index          = rank,
        shuffle             = True,
        data_name           = 'data',
        label_name          = 'softmax_label',
        aug_list            = mx.image.CreateAugmenter((3,224,224),resize=224,rand_crop=True,rand_mirror=True,mean=True))
```

**可以看出在ImageIter类中主要通过调用image.py脚本中的CreateAugmenter()函数来实现数据预处理，这个函数的细节在image.py脚本中，脚本image.py可以在~/mxnet/python/mxnet/image.py找到。接下来主要看看这个脚本中的这个重要的函数：CreateAugmenter()，重点可以看代码中的注释。**

**CreateAugmenter()函数：**

```python
def CreateAugmenter(data_shape, resize=0, rand_crop=False, rand_resize=False, rand_mirror=False,mean=None, 
std=None, brightness=0, contrast=0, saturation=0,
                    pca_noise=0, inter_method=2):
    """Creates an augmenter list."""
    auglist = []


# resize这个参数很重要，一般都要做resize，如果你的resize参数设置为224，你的原图像是350*300，那么最后resize的大小就是
# (350*300/224)*224。这里ResizeAug()函数调用resize_short()函数，resize_short()函数调用OpenCV的imresize()函数完成resize
# ，interp参数为2表示采用双三次插值做resize，可以参考：http://docs.opencv.org/master/da/d54/group__imgproc__transform.html。
    if resize > 0:
        auglist.append(ResizeAug(resize, inter_method))

    crop_size = (data_shape[2], data_shape[1])

# 如果rand_resize参数是true，那么会调用RandomSizedCropAug()函数，输入是size，min_area，retio，interp，
# 这个函数既做resize又做crop，因此这边才会写成if elif的语句。RandomSizedCropAug()函数调用random_size_crop()函数，
# 这个函数会先生成随机的坐标点和长宽值，然后调用fixed_crop()函数做crop。
#这里还有一个语句是assert rand_crop，python的assert语句是用来声明其布尔值必须为真，如果表达式为假，就会触发异常。
# 也就是说要调用RandomSizedCropAug()函数的前提是rand_crop是True。
    if rand_resize:
        assert rand_crop
        auglist.append(RandomSizedCropAug(crop_size, 0.3, (3.0 / 4.0, 4.0 / 3.0), inter_method))

#如果rand_crop参数是true，表示随机裁剪，randomCropAug()函数的输入之一是crop_size，
# 这个crop_size就是CreateAugmenter()函数的输入data_shape的图像大小，然后randomCropAug()函数调用random_crop()函数，
# random_crop()函数会先生成新的长宽值和坐标点，然后以此调用fixed_crop()函数做crop，
# 最后返回crop后的图像和坐标即长宽值，因为生成中心坐标点的时候是随机的，所以还是random crop。
    elif rand_crop:
        auglist.append(RandomCropAug(crop_size, inter_method))

# 如果前面两个if条件都不满足，就调用CenterCropAug()函数做crop，这个函数的输入也包括了crop_size，也就是你的输入data_shape，
# 所以这个参数是很有用的。CenterCropAug()函数调用center_crop()函数，这个函数的输入输出都是NDArray格式。
# center_crop()函数和random_crop()函数的区别在于前者坐标点的生成不是随机的，而是和原图像一样，
# 然后再将坐标点和新的长宽作为fixed_crop()函数的输入。
    else:
        auglist.append(CenterCropAug(crop_size, inter_method))
#可以看出不管你是否要做crop，只要你给定了data_shape参数，就默认要将输入图像做crop操作。
# 因此如果你不想在test的时候做crop，可以在这修改源码。

# 随机镜像处理，参数是0.5，HorizontalFlipAug()函数调用nd.flip()函数做水平翻转
    if rand_mirror:
        auglist.append(HorizontalFlipAug(0.5))


# CastAug()函数主要是将数据格式转化为float32
    auglist.append(CastAug())


# 这三个参数分别是亮度，对比度，饱和度。当你对这三个参数设置了值，
# 就会调用ColorJitterAug()函数对其相应的亮度或对比度或饱和度做改变
    if brightness or contrast or saturation:
        auglist.append(ColorJitterAug(brightness, contrast, saturation))


# 这个部分主要是添加pca噪声的，具体可以看LightingAug()函数
    if pca_noise > 0:
        eigval = np.array([55.46, 4.794, 1.148])
        eigvec = np.array([[-0.5675, 0.7192, 0.4009],
                           [-0.5808, -0.0045, -0.8140],
                           [-0.5836, -0.6948, 0.4203]])
        auglist.append(LightingAug(pca_noise, eigval, eigvec))


# mean这个参数主要是和归一化相关。这里的assert语句前面已经介绍过了。mean参数默认是None，这种情况下是不会进入下面的if elif条件函数的。
# 如果想进行均值操作，可以设置mean为True，那么就会进入第一个if条件，如果你设置为其他值，就会进入elif条件，
# 这个时候如果你的mean不符合要求，比如isinstance函数用来判断类型，就会触发异常。
    if mean is True:
        mean = np.array([123.68, 116.28, 103.53])
    elif mean is not None:
        assert isinstance(mean, np.ndarray) and mean.shape[0] in [1, 3]

# std与mean同理
    if std is True:
        std = np.array([58.395, 57.12, 57.375])
    elif std is not None:
        assert isinstance(std, np.ndarray) and std.shape[0] in [1, 3]

# 这里需要mean和std同时都设置正确才能进行预处理，如果你只设置了mean，没有设置std，那么还是没有启动归一化的预处理。
# 这里主要调用ColorNormalizeAug()函数，这个函数调用color_normalize()函数，这个函数的实现很简单，
# 就是将原图像的像素值减去均值mean，然后除以标准差std得到返回值。
    if mean is not None and std is not None:
        auglist.append(ColorNormalizeAug(mean, std))

# 最后返回预处理的列表
    return auglist
```



