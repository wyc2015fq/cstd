# ROI Pooling层解析_代码原理详细解释和存在目的 - zj360202的专栏 - CSDN博客





2017年12月19日 18:59:40[zj360202](https://me.csdn.net/zj360202)阅读数：6621









ROIs Pooling顾名思义，是pooling层的一种，而且是针对ROIs的pooling；


整个 ROI 的过程，就是将这些 proposal 抠出来的过程，得到大小统一的 feature map。




```python
什么是ROI呢？
ROI是Region of interest的简写，指的是faster rcnn结构中，经过rpn层后，产生的proposal对应的box框。
```




所以ROI就是指矩形框，往往经过rpn后输出的不止一个矩形框，所以这里我们是对多个ROI进行Pooling。

## ROI Pooling的输入


输入有两部分组成： 

1. data：指的是进入RPN层之前的那个Conv层的Feature Map，通常我们称之为“share_conv”； 

2. rois：指的是RPN层的输出，一堆矩形框，形状为1x5x1x1（4个坐标+索引index），其中值得注意的是：坐标的参考系不是针对feature map这张图的，而是针对原图的（神经网络最开始的输入）

## ROI Pooling的输出


输出是batch个vector，其中batch的值等于roi的个数，vector的大小为channelxwxh；ROI Pooling的过程就是将一个个大小不同的box矩形框，都映射成大小为wxh的矩形框；

![](http://images2017.cnblogs.com/blog/821593/201711/821593-20171122221026040-928530123.jpg)





如图所示，我们先把roi中的坐标映射到feature map上，映射规则比较简单，就是把各个坐标除以输入图片与feature map的大小的比值，得到了feature map上的box坐标后，我们使用pooling得到输出；由于输入的图片大小不一，所以这里我们使用的spp pooling，spp pooling在pooling的过程中需要计算pooling后的结果对应的两个像素点反映社到feature map上所占的范围，然后在那个范围中进行取max或者取average。

## Caffe ROI Pooling的源码解析

1. LayerSetUp



```python
template <typename Dtype>
void ROIPoolingLayer<Dtype>::LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  ROIPoolingParameter roi_pool_param = this->layer_param_.roi_pooling_param();
  //经过Pooling后的feature map的高
  pooled_height_ = roi_pool_param.pooled_h();
  //经过Pooling后的feature map的宽
  pooled_width_ = roi_pool_param.pooled_w();
  //输入图片与feature map之前的比值，这个feature map指roi pooling层的输入
  spatial_scale_ = roi_pool_param.spatial_scale();
}
```



2. Reshape



```python
template <typename Dtype>
void ROIPoolingLayer<Dtype>::Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  //输入的feature map的channel数
  channels_ = bottom[0]->channels();
  //输入的feature map的高
  height_ = bottom[0]->height();
  //输入的feature map的宽
  width_ = bottom[0]->width();
  //设置输出的形状NCHW，N=ROI的个数，C=channels_，H=pooled_height_，W=pooled_width_
  top[0]->Reshape(bottom[1]->num(), channels_, pooled_height_,
      pooled_width_);
  //max_idx_的形状与top一致
  max_idx_.Reshape(bottom[1]->num(), channels_, pooled_height_,
      pooled_width_);
}
```



3. Forward



```python
template <typename Dtype>
void ROIPoolingLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  //输入有两部分组成，data和rois
  const Dtype* bottom_data = bottom[0]->cpu_data();
  const Dtype* bottom_rois = bottom[1]->cpu_data();
  // Number of ROIs
  int num_rois = bottom[1]->num();
  int batch_size = bottom[0]->num();
  int top_count = top[0]->count();
  Dtype* top_data = top[0]->mutable_cpu_data();
  caffe_set(top_count, Dtype(-FLT_MAX), top_data);
  int* argmax_data = max_idx_.mutable_cpu_data();
  caffe_set(top_count, -1, argmax_data);

  // For each ROI R = [batch_index x1 y1 x2 y2]: max pool over R
  for (int n = 0; n < num_rois; ++n) {
    int roi_batch_ind = bottom_rois[0];
    //把原图的坐标映射到feature map上面
    //通过roi候选框的x1,y1,x2,y2乘以缩放比得到当前层对应关系，fast-rcnn是1/16，因为是conv-5，进行进行四次pooling
    int roi_start_w = round(bottom_rois[1] * spatial_scale_);
    int roi_start_h = round(bottom_rois[2] * spatial_scale_);
    int roi_end_w = round(bottom_rois[3] * spatial_scale_);
    int roi_end_h = round(bottom_rois[4] * spatial_scale_);
    //计算每个roi在feature map上面的大小
    int roi_height = max(roi_end_h - roi_start_h + 1, 1);
    int roi_width = max(roi_end_w - roi_start_w + 1, 1);
    //pooling之后的feature map的一个值对应于pooling之前的feature map上的大小
    //注：由于roi的大小不一致，所以每次都需要计算一次
    //通过feature map的h,w除以输出结果的h,w得到缩放比
    const Dtype bin_size_h = static_cast<Dtype>(roi_height)
                             / static_cast<Dtype>(pooled_height_);
    const Dtype bin_size_w = static_cast<Dtype>(roi_width)
                             / static_cast<Dtype>(pooled_width_);
    //找到对应的roi的feature map，如果input data的batch size为1
    //那么roi_batch_ind=0
    const Dtype* batch_data = bottom_data + bottom[0]->offset(roi_batch_ind);
    //pooling的过程是针对每一个channel的，所以需要循环遍历
    for (int c = 0; c < channels_; ++c) {
      //计算output的每一个值，所以需要遍历一遍output，然后求出所有值
      for (int ph = 0; ph < pooled_height_; ++ph) {
        for (int pw = 0; pw < pooled_width_; ++pw) {
          // Compute pooling region for this output unit:
          //  start (included) = floor(ph * roi_height / pooled_height_)
          //  end (excluded) = ceil((ph + 1) * roi_height / pooled_height_)
          // 计算output上的一点对应于input上面区域的大小[hstart, wstart, hend, wend]
          // 通过宽高缩放比对每一个输出点进行缩放映射
          int hstart = static_cast<int>(floor(static_cast<Dtype>(ph)
                                              * bin_size_h));
          int hend = static_cast<int>(ceil(static_cast<Dtype>(ph + 1)   //下一个像素的对应映射值
                                           * bin_size_h));
          int wstart = static_cast<int>(floor(static_cast<Dtype>(pw)
                                              * bin_size_w));
          int wend = static_cast<int>(ceil(static_cast<Dtype>(pw + 1)
                                           * bin_size_w));
          //将映射后的区域平动到对应的位置[hstart, wstart, hend, wend]
          //当前值加上此框x1在conv5的映射开始坐标，和输入featrue map的高比，小的输出，
          //对外面一层Min不太理解，感觉应该是hend-hstart和height_比较
          //此处得到的结果是相对于原图缩放一定比例(fast_rcnn是1/16)从左上角原点开始计算的绝对值坐标
          hstart = min(max(hstart + roi_start_h, 0), height_);
          hend = min(max(hend + roi_start_h, 0), height_);
          wstart = min(max(wstart + roi_start_w, 0), width_);
          wend = min(max(wend + roi_start_w, 0), width_);
          //如果映射后的矩形框不符合
          bool is_empty = (hend <= hstart) || (wend <= wstart);
          //pool_index指的是此时计算的output的值对应于output的位置
          const int pool_index = ph * pooled_width_ + pw;
          //如果矩形不符合，此处output的值设为0，此处的对应于输入区域的最大值为-1
          if (is_empty) {
            top_data[pool_index] = 0;
            argmax_data[pool_index] = -1;
          }
          //遍历output的值对应于input的区域块
          //
          for (int h = hstart; h < hend; ++h) {
            for (int w = wstart; w < wend; ++w) {
             // 对应于input上的位置
              const int index = h * width_ + w;
              //计算区域块的最大值，保存在output对应的位置上
              //假设输入featrue map是20x20，输出是4x4的，那就是输出一个点对应输入25个点，缩放后就是输出的点在这25个中保留最大的那个，其他不要
              //同时记录最大值的索引
              if (batch_data[index] > top_data[pool_index]) {
                top_data[pool_index] = batch_data[index];
                argmax_data[pool_index] = index;
              }
            }
          }
        }
      }
      // Increment all data pointers by one channel
      batch_data += bottom[0]->offset(0, 1);
      top_data += top[0]->offset(0, 1);
      argmax_data += max_idx_.offset(0, 1);
    }
    // Increment ROI data pointer
    bottom_rois += bottom[1]->offset(1);
  }
}
```







