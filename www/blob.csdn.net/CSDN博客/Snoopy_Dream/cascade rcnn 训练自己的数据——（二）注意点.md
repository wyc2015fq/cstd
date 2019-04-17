# cascade rcnn 训练自己的数据——（二）注意点 - Snoopy_Dream - CSDN博客





2018年07月04日 15:42:53[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：1474
所属专栏：[cascade rcnn目标检测](https://blog.csdn.net/column/details/25672.html)









写在题前：首先要看懂文件夹是什么意思，15s代表这有15个anchor（train.prototxt中最开始就是）800代表这short_size 


![](https://img-blog.csdn.net/20180704153923177?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


1.在train.prototxt中还可以加一下的参数，默认只有short 和 long size，但是长短边也好，resize_ratio也好，必须是32的倍数。

![](https://img-blog.csdn.net/20180704152417504?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

  把所有的81改成2（我这里是汽车检测，所以是2）

  把img_width和img_height全部变成你的short_size


![](https://img-blog.csdn.net/20180704152851817?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


2.记得在deploy.prototxt中也要做相对与train的修改

3.在做检测的时候，检测代码在另一个博客当中。记得也要修改short_size 和 long_size.


4.因为cascade训练第一步需要找iou>0.5的，如果你的目标太小，可能需要考虑增加anchor。【下图中前三个是我加的】

![](https://img-blog.csdn.net/20180704153150987?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


再对下面的层做相应修改就ok了。

detection_data_param、label_fpn2、rpn_fpn2_loss、rpn_fpn2_accuracy、proposals_fpn2

**rpn_fpn2_cls、rpn_fpn2_bbox【后两个需要注意】**




