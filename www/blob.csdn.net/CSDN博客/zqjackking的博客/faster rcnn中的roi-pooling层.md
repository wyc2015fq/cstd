# faster rcnn中的roi-pooling层 - zqjackking的博客 - CSDN博客





2017年03月01日 11:46:09[zqjackking](https://me.csdn.net/zqjackking)阅读数：9573








```
本文就faster rcnn中的ZF网络来对roi-pooling层做一个解读
 首先看网络结构，
 layer {
```

name: “roi_pool_conv5” 

  type: “ROIPooling” 

  bottom: “conv5” 

  bottom: “rois” 

  top: “roi_pool_conv5” 

  roi_pooling_param { 

    pooled_w: 6 

    pooled_h: 6 

    spatial_scale: 0.0625 # 1/16 

  } 

} 

其bottom[0]就是conv5出来的feature map（长和宽分别是原始图片的1/16），bottom[1]是rois(是一个blob)。 

其中，top的形状是（bottom[1]->num(), channels_, pooled_height_,pooled_width_）。 

其中，channels是bottom[0]的channel,pool_height_和pooled_width_都是固定的，等于6。 

下面来看rio-pooling层是怎么实现的。代码见roi_pooling_layer.cpp中的Forward_cpu部分。 

      首先确定top的长度和宽度。我们可以将bottom[1]看成一个二维的矩阵。矩阵的行数是num_rois = bottom[1]->num()，矩阵的列数是5。（For each ROI R = [batch_index x1 y1 x2 y2]: max pool over R），这每行的5个参数代表的意思是，batch_index（也就是bottom_rois[0]）确定每次在bottom[0]中的第一个维度的偏移量。其它四个参数确定在feature map中的点的坐标。现详述如下： 

       在feature map中，首先确定2个坐标（x1/16,y1/16）,(x2/16,y2/16)。这两个点（左上，右下）就在feature map上确定了一个区域。 

       然后将这个区域36等分（6x6）,划分成了36个相同大小的子区域。然后找出每个子区域的对应的feature map的最大值，即为top中的相应的点的值。（top的高度和宽度都是6） 

       对于bottom[0]的每一个channel，也都是这样找的。（注意，每次找完一个channel），batch_data和top_data都会偏移offset(0,1)。所以top的第二个维度就等于bottom[0]的channel。 

       对于top的第一个维度应该怎么解释呢？前面已经说了，在最外面的for循环中， 

      for (int n = 0; n < num_rois; ++n) { 

      const Dtype* batch_data = bottom_data + bottom[0]->offset(roi_batch_ind);  

      ………… 

      bottom_rois += bottom[1]->offset(1); 

      } 

      每次都会有个rio_batch_ind确定在bottom[0]中的偏移量，这个rio_batch_ind就是bottom[0]这个二维矩阵的每一行第一列的值。很显然，bottom[0]不是每个num都会起作用的，我们会根据bottom[1]中的每一行的第一个值来确定要选择bottom[0]中的那个num。所以最后的top第一个维度等于bottom[1]->num。



