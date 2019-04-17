# ubutnu16.04+caffe+cuda8.0+NVIDIA TX2环境下，Faster RCNN 训练中的一些问题及解决办法 - 站在巨人的肩膀上coding - CSDN博客





2018年01月11日 16:04:50[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：1974








之前实验室购买了NVIDIA-jetson-TX2系类的嵌入式开发板，就迫不及待的在上面配好了caffe 和py-faster rcnn。当时没配散热器，无法在上面训练测试，只是跑了demo，demo跑起来还是很顺畅的，基本没修改代码，也没有所谓的numpy版本问题。不知道具体在嵌入式环境下的准确率效果怎样。今天买的散热器到了，装好后来训练。

好，废话不多说，先贴一张在nvidia jetson tx2上的demo效果图和检测时间：

![](https://img-blog.csdn.net/20180111161646728?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTE9WRTEwNTUyNTk0MTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



检测时间（使用的是作者提供的ZF模型）：

![](https://img-blog.csdn.net/20180111161758510?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTE9WRTEwNTUyNTk0MTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**************************************我是萌萌哒的分割线************************************

今天使用Faster RCNN训练自己的数据的时候，出现了一些因为boost或者是numpy版本不兼容导致的问题，经过各种查资料和求助大神，总算是顺利把网络跑起来了。下面内容都是今天亲测出现的问题并与其对应的解决方案，和大家一起分享，也便于我以后查看。


训练方法：在配置好Faster RCNN之后，准备好自己的数据，修改网络的配置文件和相应的训练脚本满，使用end to end 的训练方法，在$py-faster-rcnn的根目录下执行：./experiments/scripts/faster_rcnn_end2end.sh 0 ZF pascal_voc 。以下都是执行该脚本后出现的问题。



 特别提醒：这里推荐解决完一个问题后立马运行测试，或许你没有接下来的问题，不用修改代码。

下面的错误是在用自己的数据集的话容易出的错误，我用VOC2007训练时候只出现了问题2，其他问题都没有出现。



**问题１**
`TypeError: slice indices must be integers or None or have an __index__ method`

这里还是因为numpy版本的原因，最好的解决办法还是换numpy版本（我换numpy版本还是包这些错，无效。使用如下方法）

修改 /home/lzx/py-faster-rcnn/lib/rpn/proposal_target_layer.py，转到123行：

```
for ind in inds:
        cls = clss[ind]
        start = 4 * cls
        end = start + 4
        bbox_targets[ind, start:end] = bbox_target_data[ind, 1:]
        bbox_inside_weights[ind, start:end] = cfg.TRAIN.BBOX_INSIDE_WEIGHTS
    return bbox_targets, bbox_inside_weights
```


这里的ind，start，end都是 numpy.int 类型，这种类型的数据不能作为索引，所以必须对其进行强制类型转换，转换成为int类型，转化结果如下：

```
for ind in inds:
```
` ind = int(ind)`
```
cls = clss[ind]
```
`start = int(4 * cls)`
```
end = int(start + 4)
        bbox_targets[ind, start:end] = bbox_target_data[ind, 1:]
        bbox_inside_weights[ind, start:end] = cfg.TRAIN.BBOX_INSIDE_WEIGHTS
    return bbox_targets, bbox_inside_weights
```


以上内容是我刚刚在训练自己的datasets时候出现的一些问题，参考了一些其他博客的解决方法。

在这里需要提醒的是，我重装numpy，升级或者降级都无法解决问题，可以先按博客修改，修改好了没问题就不用改变numpy的版本。大部分还是因为Faster RCNN 发布的时候使用的一些库现在都升级了，所以需要对代码中一些细节进行修改。


**问题２**
`AttributeError: 'module' object has no attribute ‘text_format'`
解决方法：在/home/xxx/py-faster-rcnn/lib/fast_rcnn/train.py的头文件导入部分加上 ：import google.protobuf.text_format




**问题３**
`TypeError: 'numpy.float64' object cannot be interpreted as an index `


修改的位置如下（xxx为你的用户名）：这里我的是nvidia


1) /home/nvidia/py-faster-rcnn/lib/roi_data_layer/minibatch.py

```
查看第26行：fg_rois_per_image = np.round(cfg.TRAIN.FG_FRACTION * rois_per_image)
改为：fg_rois_per_image = np.round(cfg.TRAIN.FG_FRACTION * rois_per_image).astype(np.int)
```


2) /home/nvidia/py-faster-rcnn/lib/datasets/ds_utils.py

```
查看第12行：hashes = np.round(boxes * scale).dot(v)
改为：hashes = np.round(boxes * scale).dot(v).astype(np.int)
```


3) /home/nvidia/py-faster-rcnn/lib/fast_rcnn/test.py

```matlab
查看第129行： hashes = np.round(blobs['rois'] * cfg.DEDUP_BOXES).dot(v)
改为： hashes = np.round(blobs['rois'] * cfg.DEDUP_BOXES).dot(v).astype(np.int)
```


4) /home/nvidia/py-faster-rcnn/lib/rpn/proposal_target_layer.py

```
查看第60行：fg_rois_per_image = np.round(cfg.TRAIN.FG_FRACTION * rois_per_image)
改为：fg_rois_per_image = np.round(cfg.TRAIN.FG_FRACTION * rois_per_image).astype(np.int)
```

最后再贴一张训练时候的情况：

![](https://img-blog.csdn.net/20180111162022516?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTE9WRTEwNTUyNTk0MTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

当然，速度很慢，实际训练我也不是在这个上面训练，实际用实验室的服务器进行训练，在这里只是来测试下网络的map等结果。

总结：很多博客都说这里是因为numpy版本不兼容导致的问题，最好的解决办法是卸载你的numpy，安装numpy1.11.0。但是我实际使用发现，不管是pip升级/降级numpy还是先卸载完全后，再重新安装，都会出现上面的问题。demo也跑不起来，而且报ImportError: No
 module named numpy.core.multiarray的错误，但实际上直接import的时候是通过的！！！！很坑爹，最后还是妥协了，不改变原来numpy的版本，而是改变代码中的内容。

如果想修改numpy的版本的话，可以执行：sudo pip install -U numpy==1.11.0，更换numpy的版本




