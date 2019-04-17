# 解读flow-guided feature aggregation for video object detection - 家家的专栏 - CSDN博客





2017年11月30日 20:03:49[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：838








**文章主要贡献点：**

**Flow-guided feature aggregation**, an end-to-end framework for **video object detection.**

Improve the per-frame features by aggregation of nearby features along the motion path, and thus improve the video recognition accuracy.

  Or improve the per-frame feature learning by **temporal **aggregation

**数据库**：**ImageNet VID dataset**

              3862 video snippet from the traning set

              555 snippets from the validation set

              Fully annotated

              30 object categories (a subset of the categories in the ImageNet DET dataset),

相关工作：




本文工作：

      1. the **feature extraction network** is applied on individual frames to produce the per-frame feature maps

       2. To enhance the features at a reference frame, an **optical flow network ** [flownet] estimates the**motions between the nearby frames an the reference frame**. 

       3. The feature maps from nearby frames are warped to the reference maps, as well as its own feature maps on the reference frame, are**aggregated** according to an        adaptive weighting network. 

        4. The resulting aggregated feature maps are then fed to the detection network to produce the detection result on the reference frame.

         System: **Feature extraction + flow estimation + feature aggregation + detection**


















