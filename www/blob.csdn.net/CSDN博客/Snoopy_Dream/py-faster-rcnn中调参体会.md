# py-faster-rcnn中调参体会 - Snoopy_Dream - CSDN博客





2018年05月02日 23:14:10[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：1607








**config.py的参数有**

**目录**

[AnchorTargetCreator (RPN开头)：](#AnchorTargetCreator%C2%A0(RPN%E5%BC%80%E5%A4%B4)%EF%BC%9A)

[ProposalCreator(RPN开头)：](#ProposalCreator(RPN%E5%BC%80%E5%A4%B4)%EF%BC%9A)

[ProposalTargetCreator：](#ProposalTargetCreator%EF%BC%9A)

### **`AnchorTargetCreator`**(RPN开头)**：**

`AnchorTargetCreator`将 20000 多个候选的 anchor 选出 256 个 anchor 进行**分类和回归位置**。

```python
RPN_POSTIVE_OVERLAP =0.7,

RPN_NEGATIVE_OVERLAP =0.3

RPN_FG_FRACTION =0.5
```

选择过程如下：

对于每个 anchor, gt_label 要么为 1（前景），要么为 0（背景），对于每一个  `gt_bbox`，**选择和它重叠度（IoU）最高的一个 anchor **作为**正样本**，对于剩下的 anchor，从中**选择和任意一个**`**gt_bbox**`**重叠度超过 0.7 的 anchor**，作为正样本，正样本的数目不超过 128 个。随机选择和`gt_bbox`重叠度小于 0.3 的 anchor 作为**负样本**。负样本和正样本的总数为 256。

### `**ProposalCreator**`**(RPN开头)：**

RPN_NMS_THRESH = 0.7，如果是多目标。我觉得这里的0.7应该变小一些，这样后面选概率最大的2000个的时候，才不会在同一个目标处选太多，从上万个 anchor 中，选择一定数目（2000 或者 300），调整大小和位置，生成 RoIs，用以 Fast R-CNN 训练或者测试。（20000anchor----2000or300的ROIs-----用作RoIHead的训练样本）

```python
TRAIN.RPN_NMS_THRESH = 0.7

RPN_PRE_NMS_TOP =12000 在nms抑制之前选的12000个anchor

RPN_POST_NMS_TOP =2000在nms抑制之后的2000个roi

FG_THRESH = 0.5

BG_THRESH_HI = 0.5

BG_THRESH_LO = 0.1
```

具体过程：
- 对于每张图片，利用它的 feature map， 计算 (H/16)× (W/16)×9（大概 20000）个 anchor 属于前景的概率，以及对应的位置参数。（**找前景和anchor的位置参数）**
- 选取概率较大的 12000 个 anchor（选概率大的前景）
- 利用回归（这里用的AnchorTargetCreator算出的回归的参数）的位置参数，修正这 12000 个 anchor 的位置，得到 RoIs**（****利用gt_loc回归参数修正12000个ROIS）**
- 利用非极大值（(Non-maximum suppression, NMS）抑制，选出**概率最大的 2000 个 RoIs**

### **`ProposalTargetCreator`**（不是RPN开头）**：**

负责在训练 **RoIHead**/Fast R-CNN 的时候，从 RoIs 选择一部分 (比如 128 个) 用以训练。同时给定训练目标, 返回（`sample_RoI`, `gt_RoI_loc`, `gt_RoI_label`）

```
BATCH_SIZE = 128

BBOX_THRESH=0.5

FG_FRACTION = 0.25
```

步骤：

RoIs 和 gt_bboxes 的 IoU 大于 0.5 的，选择一些（比如 32 个）

选择 RoIs 和 gt_bboxes 的 IoU 小于等于 0（或者 0.1）的选择一些（比如 128-32=96 个）作为负样本

为了便于训练，对选择出的 **128 个 RoIs**，还对他们的`gt_roi_loc` 进行标准化处理（减去均值除以标准差）





