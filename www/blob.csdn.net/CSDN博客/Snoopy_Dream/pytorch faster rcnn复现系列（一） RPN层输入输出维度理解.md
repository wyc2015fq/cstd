# pytorch faster rcnn复现系列（一） RPN层输入输出维度理解 - Snoopy_Dream - CSDN博客





置顶2018年11月01日 12:28:41[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：808
所属专栏：[faster-rcnn.pytorch](https://blog.csdn.net/column/details/25674.html)









**目录**

[1.  1*1 FC层](#1.%C2%A0%201*1%20FC%E5%B1%82)

[2.  anchor_target_layer_>rpn_data                              ​](#%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%20%E2%80%8B)

[3. 二分类 softmaxloss](#3.%20%E4%BA%8C%E5%88%86%E7%B1%BB%20softmaxloss)

[4. 回归损失 rpn_loss_bbox](#4.%20%E5%9B%9E%E5%BD%92%E6%8D%9F%E5%A4%B1%20rpn_loss_bbox)

[5. softmax 二分类分类概率](#5.%20softmax%20%E4%BA%8C%E5%88%86%E7%B1%BB%E5%88%86%E7%B1%BB%E6%A6%82%E7%8E%87)

[6. prosal_layer_>rpn_rois 2000个rpn_rois](#6.%20prosal_layer_%3Erpn_rois)

[7. proposal_target_layer_>roi_data 128个rois](#7.%20proposal_target_layer_%3Eroi_data)



想要复现faster rcnn，理解rpn是必备的，而明白其输入输出又是复现的第一步。

以FCN中rpn的caffe模型来讲解rpn的过程。

首先是数据读入操作，假设输出的data是1*3*600*1000，im_info是1*3[h w  imscale]，gt_boxes是1*5[ymin xmin ymax xmax，class]，后面的所有维度都是以这个假设为前提。

![这里写图片描述](https://vipkshttps12.wiz.cn/ks/note/view/c71bc2bc-5b36-465e-9921-a72e170d7cfa/2cc53107-2d49-45b1-ab42-6742ad607643/index_files/0.8821269870458401.png)

![](https://img-blog.csdnimg.cn/20181101120355755.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

[faster rcnn中 softmax，softmax loss和cross entropy的讲解](https://blog.csdn.net/e01528/article/details/83795970)

总体理解：

![](https://img-blog.csdnimg.cn/20181101155218739.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

### 1.  1*1 FC层

 1个共享卷积层之后，紧跟着RPN结构。最开始是一个512*3*3的conv+relu。

下面紧跟着一个18*1*1，一个36*1*1的全连接功能的层。

### 2.  anchor_target_layer_>rpn_data

 先说cls这边。rpn_data层（anchor_target_layer类型的层），输入 ，输出分别为以下

```
bottom: 'rpn_cls_score'#[1,2*k,H,W] 类似与fc层的输出
  bottom: 'gt_boxes'
  bottom: 'im_info'#原图c h w，因为坐标回归的是原图坐标的xy偏移，需要用hw 过滤不在图片内部的
  bottom: 'data'
```

```
K表示9个anchor，3*3，HW表示的是rpn第一层卷积的特征图的宽高。
  top: 'rpn_labels' # [1,1,k*H,W] 生成rpn的标签，前景为1，背景为0，然后给softmaxLoss层，
  top: 'rpn_bbox_targets'#[1,4*K,H,W] smooth_l1_loss输入
  top: 'rpn_bbox_inside_weights'#[1,4*K,H,W]smooth_l1_loss输入
  top: 'rpn_bbox_outside_weights'#[1,4*K,H,W] smooth_l1_loss输入
```

   将** 20000** 多个候选的 **anchor 选出 256 **个 anchor 进行分类和回归位置。选择过程如下：
- 
对于每一个 ground truth bounding box (`gt_bbox`)，**选择和它重叠度（IoU）最高的一个 anchor 作为正样本**

- 
对于剩下的 anchor，**从中选择和任意一个`gt_bbox`重叠度超过 0.7 的 ancho**r，作为正样本，正样本的数目**不超过 128 个**。

- 
**随****机选择和`gt_bbox`重叠度小于 0.3 的 anchor 作为负样**本。负样本和正样本的总数为 256。例如：正样本100负样本156


对于每个 anchor, gt_label 要么为 1（前景），要么为 0（背景），而 gt_loc 则是由 4 个位置参数 (tx,ty,tw,th) 组成，这样比直接回归座标更好。

### ![](https://img-blog.csdnimg.cn/20181101163141903.png)

### 3. 二分类 softmaxloss

求二分类的loss的时候，softmaxloss，输入是rpn_lables【自己生成的rpn标签】+rpn_cls_scole【预测的】_>reshape【二分类，每一个特征图上的点都是一个二分类，reshape的作用是将格式转换成和rpn_lables的一样】

### 4. 回归损失 rpn_loss_bbox

输入rpn_data 生成的rpn的bbox相关信息，与fc层rpn_bbox_pred【预测的】一起作为输入，进行损失计算。

两个输入的维度都是[1,4*k,H,W]，不需要reshape操作。

### 5. softmax 二分类分类概率

输入**[1,2,k*H,W]**：rpn_cls_scole【预测的】_>rpn_cls_prob_reshape后的结果

输出**[1,2,k*H,W]**：得到所有anchor的二分类的概率。

### 6. prosal_layer_>2000个rpn_rois

首先需要softmax的结果，进行reshape**[1,2,k*H,W]_>[1,2*k,H,W],这样与其他的输入是类似的。**

**输出：**  rpn_rois **[1,5]**

```
bottom: 'rpn_cls_prob_reshape'#[1,18,H,W]每个框经过softmax对应的概率
  bottom: 'rpn_bbox_pred'#[1,36,H,W]预测的框运算loss，反向传播，越来越准
  bottom: 'im_info'
  top: 'rpn_rois' #[1,5]
```
- 
对于每张图片，利用它的 feature map， 计算 (H/16)× (W/16)×9（大概 20000）个 anchor 属于前景的概率，以及对应的位置参数【softmax二分类概率】

- 
rpn_cls_prob_reshape始终连存储的是每个anchor 的偏移。

- 
选取概率较大的 12000 个 anchor

- 
利用**回归的位置参数**，**修正这 12000 个 anchor 的位置**，得到 rpn_rois

- 
利用非极大值（(Non-maximum suppression, NMS）抑制，选出概率最大的 2000 个 rpn_rois


### 7. proposal_target_layer_> roi_data 128 个RoIs

```
bottom: 'rpn_rois'[1,5]
  bottom: 'gt_boxes'[1,4]
```

```
top: 'rois'[1,5,1,1]
  top: 'labels'[1,1,1,1]
  top: 'bbox_targets'[1,8,1,1]存储的是anchors和最高重叠的gt的（
   x*-xa）/wa,(y*-ya)/ha,log(w*/wa),log(h*/hg),*为gt。
  top: 'bbox_inside_weights'[1,8,1,1]被抽中的正类为1，其他为0；
  top: 'bbox_outside_weights[1,8,1,1]外部权重，目前负例的外部权重=正例的外部权重=np.ones((1, 4)) * 1.0 /
```

RPN 会产生大约 2000 个 RoIs，这 2000 个 RoIs 不是都拿去训练，而是利用`ProposalTargetCreator` 选择 128 个 RoIs 用以训练。选择的规则如下：
- 
RoIs 和 gt_bboxes 的 IoU 大于 0.5 的，选择一些（比如 32 个）

- 
选择 RoIs 和 gt_bboxes 的 IoU 小于等于 0（或者 0.1）的选择一些（比如 128-32=96 个）作为负样本


为了便于训练，对选择出的 128 个 RoIs，还对他们的`gt_roi_loc` 进行标准化处理（减去均值除以标准差）

对于分类问题, 直接利用交叉熵损失. 而对于位置的回归损失, 一样采用 Smooth_L1Loss, 只不过只对正样本计算损失. 而且是只对正样本中的这个类别 4 个参数计算损失。举例来说:
- 
一个 RoI 在经过 FC 84 后会输出一个 84 维的 loc 向量. 如果这个 RoI 是负样本, 则这 84 维向量不参与计算 L1_Loss

- 
如果这个 RoI 是正样本, 属于 label K, 那么它的第 K×4, K×4+1 ，K×4+2， K×4+3 这 4 个数参与计算损失，其余的不参与计算损失。




其中prosal layer里的输入错了，rpn_bbox_pred是回归的偏移。不是预测的框。

**RPN网络**，RPN网络以一个3*3的卷积核，pad=1，stride=1的512个卷积核的卷积层开始，输入是res4f层的输出，res4f层的输出即conv4_x最后的输出。该rpn_conv/3*3层的输出是1*512*38*63。

![这里写图片描述](https://vipkshttps12.wiz.cn/ks/note/view/c71bc2bc-5b36-465e-9921-a72e170d7cfa/2cc53107-2d49-45b1-ab42-6742ad607643/index_files/0.5594255968660047.png)

然后是分类层和回归层，分类层采用1*1的卷积核，pad=0，stride=1的18（2(back ground/fore ground)*9(anchors)）个卷积核的卷积层，分类层的输出是**1*18*38*63**。回归采用1*1的卷积核，pad=0，stride=1的36（4*9(anchors)）个卷积核的卷积层，回归层的输出是**1*36*38*63**。

![这里写图片描述](https://vipkshttps12.wiz.cn/ks/note/view/c71bc2bc-5b36-465e-9921-a72e170d7cfa/2cc53107-2d49-45b1-ab42-6742ad607643/index_files/0.4506533571898399.png)

**Reshape层**对分类层的结果做了一次维度调整，从1*18*38*63变成**1*2*342*63**，后面的342*63就代表该层所有anchor的数量。

![这里写图片描述](https://vipkshttps12.wiz.cn/ks/note/view/c71bc2bc-5b36-465e-9921-a72e170d7cfa/2cc53107-2d49-45b1-ab42-6742ad607643/index_files/0.9472662681470656.png)

anchor_target_layer

input：最开始读取的数据得到rpn的label和target。

输出：rpn_labels为**1*1*342*63**，rpn_bbox_targets为**1*36*38*63**，rpn_bbox_inside_weights为**1*36*38*63**，rpn_bbox_outside_weights为1***36*38*63**。

![这里写图片描述](https://vipkshttps12.wiz.cn/ks/note/view/c71bc2bc-5b36-465e-9921-a72e170d7cfa/2cc53107-2d49-45b1-ab42-6742ad607643/index_files/0.26110536286798536.png)

**损失函数**如下：**分类的损失采用SoftmaxWithLoss**，输入是reshape后的预测的类别score(1*2*342*63)和真实的label(1*1*342*63)。**回归的损失采用SmoothL1Loss**，输入是rpn_bbox_pred(1*36*38*63)即所有anchor的坐标相关的预测，rpn_bbox_targets(1*36*38*63)，rpn_bbox_inside_weights(1*36*38*63)，rpn_bbox_outside_weights(1*36*38*63)。

![这里写图片描述](https://vipkshttps12.wiz.cn/ks/note/view/c71bc2bc-5b36-465e-9921-a72e170d7cfa/2cc53107-2d49-45b1-ab42-6742ad607643/index_files/0.40046035060744867.png)

然后是ROI Proposal，先用一个softmax层算出概率（1*2*342*63），然后再reshape到1*18*38*63。

![这里写图片描述](https://vipkshttps12.wiz.cn/ks/note/view/c71bc2bc-5b36-465e-9921-a72e170d7cfa/2cc53107-2d49-45b1-ab42-6742ad607643/index_files/0.7197532201343646.png)

然后是生成proposal，维度是1*5。

![这里写图片描述](https://vipkshttps12.wiz.cn/ks/note/view/c71bc2bc-5b36-465e-9921-a72e170d7cfa/2cc53107-2d49-45b1-ab42-6742ad607643/index_files/0.3580359294351778.png)

这一层生成rois（1*5*1*1），labels（1*1*1*1），bbox_targets（1*8*1*1），bbox_inside_weights 

（1*8*1*1），bbox_outside_weights（1*8*1*1）。

![这里写图片描述](https://vipkshttps12.wiz.cn/ks/note/view/c71bc2bc-5b36-465e-9921-a72e170d7cfa/2cc53107-2d49-45b1-ab42-6742ad607643/index_files/0.26862338447818.png)

**至此RPN网络结束。**











