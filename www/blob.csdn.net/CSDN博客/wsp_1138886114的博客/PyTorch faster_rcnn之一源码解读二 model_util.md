# PyTorch faster_rcnn之一源码解读二 model_util - wsp_1138886114的博客 - CSDN博客





2019年03月22日 13:33:32[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：502








> 
整个model/util文件夹下主要将了三个Creator函数：
- `AnchorTargetCreator()` : 用每张图中bbox的真实标签为所有任务分配ground truth！【RPN网络】
- `ProposalCreator()` : 输入上一张图的所有bbox,label的ground trurh。输出的2000个roi作为ProposalTargetCreator的输入【RPN网络】
- `ProposalTargetCreator()` : 为2000个rois赋予ground truth！（挑出128个赋予ground truth！）【RoIHead网络】

关于理论详情：[https://www.cnblogs.com/king-lps/p/8981222.html](https://www.cnblogs.com/king-lps/p/8981222.html)

整个model/util文件夹下的模块流程图：这个主要帮助我们理解数据流的流动过程。该网络的处理流程：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190328131609926.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
#### **model/utils/bbox_tools.py 文件**
- 这个文件下有如下函数：

`loc2bbox()` 将源bbox+偏移和比例 == 解码 ==>目标bbox。正好是下面的逆过程
`bbox2loc()` 将源bbox和目标bbox  == 编码 ==>为“loc(源bbox相对于bbox的offset和scales)”
`bbox_iou()` 两个bbox相交的面积与相并的面积比IOU。给定两组bbox（N,4 与 K,4），输出数组shape为（N,K）
`generate_anchor_base()` 通过枚举纵横比和比例生成anchor(锚)基窗口 （9个base anchor）
下图所示是以（0，0）为中心：

**`loc2bbox(src_bbox,loc)`传入参数**；**计算公式**；**返回值**三者关系如下：
- 
传入参数
- `src bbox（array）`bounding box的坐标: $p_{ymin}, p_{xmin}, p_{ymax}, p_{xmax}$
`loc(array)`scales and offsets(比例和偏移)：$t_y, t_x, t_h, t_w$ ;
- `bounding box中心`$(y, x) = p_y, p_x$
`bounding box 大小`$p_h, p_w$
- 
计算公式：$\bf \hat{g}$ 表示近似目标边框；$\bf p$ 表示proposal边框
- $\hat{g}_y = p_h t_y + p_y$
- $\hat{g}_x = p_w t_x + p_x$
- $\hat{g}_h = p_h \exp(t_h)$
- $\hat{g}_w = p_w \exp(t_w)$

- 
返回值
- 解码的bounding box坐标，数据shape=[R,4]；数据为[$\hat{g}_{ymin}, \hat{g}_{xmin}, \hat{g}_{ymax}, \hat{g}_{xmax}$]
- loc（array）：具有偏移量和比例的数组。同输入。
- `bounding box's center`：$\hat{g}_y, \hat{g}_x$
`bounding box's size` : $\hat{g}_h, \hat{g}_w$


**`bbox2loc(src_bbox, dst_bbox)`传入参数**；**计算公式**；**返回值**三者关系如下：
- 
传入参数
- 
src_bbox (array)一个图像坐标，数据shape=[R,4]；坐标：$p_{ymin}, p_{xmin}, p_{ymax}, p_{xmax}$

dst_bbox (array)一个图像坐标，数据shape=[R,4]；坐标：$g_{ymin}, g_{xmin}, g_{ymax}, g_{xmax}$

- 
bounding box’s center:$(y, x) = p_y, p_x$

bounding box’s size : $p_h, p_w$

- 
target bounding box’s center : $g_y, g_x$

target bounding box’s size : $g_h, g_w$

- 
计算公式：真正目标边框 $\bf g$ 与 proposal边框$\bf p$之间的offsets and scales : $t_y, t_x, t_h, t_w$
- $t_y = \frac{(g_y - p_y)} {p_h}$
- $t_x = \frac{(g_x - p_x)} {p_w}$
- $t_h = \log(\frac{g_h} {p_h})$
- $t_w = \log(\frac{g_w} {p_w})$

- 
返回值:
- `loc`-Bounding box offsets and scales（array）：数据shape=[R,4]；坐标：$t_y, t_x, t_h, t_w$。


**`bbox_iou(bbox_a, bbox_b)`传入参数**；**计算公式**；**返回值**三者关系如下：
- 
传入参数
- `bbox_a (array：numpy.ndarray)`数据shape=[N,4]
- `bbox_b (array：cupy.ndarray)`数据shape=[K,4]

- 
计算公式：（交并公式）$\frac{IOU_A \cap IOU_B}{IOU_A \cup  IOU_B}$

- 
返回值:

返回一个 array : 数据shape =（n，k）。

这个数组是一个IoUs元素的索引：这个索引介于 第n 个在box_a的bounding box 和 第k个在box_b的bounding box。
```python
import numpy as np
import numpy as xp
import six
from six import __init__


def loc2bbox(src_bbox, loc):
    """ 将边界框偏移和比例==解码==>bbox边界框
        边界框偏移 + 比例由函数：bbox2loc 计算而得。此函数表示解码为二维图像坐标中的坐标。
    """

    if src_bbox.shape[0] == 0:
        return xp.zeros((0, 4), dtype=loc.dtype)

    src_bbox = src_bbox.astype(src_bbox.dtype, copy=False)

    src_height = src_bbox[:, 2] - src_bbox[:, 0]   # Ph = ymax-ymin
    src_width = src_bbox[:, 3] - src_bbox[:, 1]    # Pw = xmax-xmin
    src_ctr_y = src_bbox[:, 0] + 0.5 * src_height  # Py = y0+0.5h
    src_ctr_x = src_bbox[:, 1] + 0.5 * src_width   # Px = x0+0.5w,计算出中心点坐标 

    dy = loc[:, 0::4]     # python [start:stop:step]
    dx = loc[:, 1::4]
    dh = loc[:, 2::4]
    dw = loc[:, 3::4]
	
	""" RCNN中提出的边框回归：寻找原始proposal与近似目标框G之间的映射关系
	    回归后的目标框（Gx,Gy,Gh,Gw）
	"""
    ctr_y = dy * src_height[:, xp.newaxis] + src_ctr_y[:, xp.newaxis] # #ctr_y为Gy
    ctr_x = dx * src_width[:, xp.newaxis] + src_ctr_x[:, xp.newaxis] # ctr_x为Gx
    h = xp.exp(dh) * src_height[:, xp.newaxis]   # h为Gh
    w = xp.exp(dw) * src_width[:, xp.newaxis]    # w为Gw

	# 以下代码：由中心点转换为左上角和右下角坐标
    dst_bbox = xp.zeros(loc.shape, dtype=loc.dtype)
    dst_bbox[:, 0::4] = ctr_y - 0.5 * h
    dst_bbox[:, 1::4] = ctr_x - 0.5 * w
    dst_bbox[:, 2::4] = ctr_y + 0.5 * h
    dst_bbox[:, 3::4] = ctr_x + 0.5 * w
    return dst_bbox  


def bbox2loc(src_bbox, dst_bbox):  #  已知源框和目标框求出其位置偏差
    """将源和目标边界框编码为“loc(偏移与比例)”.
       给定边界框，此函数计算偏移量和比例将源边界框与目标边界框匹配。
    """
    height = src_bbox[:, 2] - src_bbox[:, 0]
    width = src_bbox[:, 3] - src_bbox[:, 1]
    ctr_y = src_bbox[:, 0] + 0.5 * height
    ctr_x = src_bbox[:, 1] + 0.5 * width           # 计算出源框中心点坐标

    base_height = dst_bbox[:, 2] - dst_bbox[:, 0]
    base_width = dst_bbox[:, 3] - dst_bbox[:, 1]
    base_ctr_y = dst_bbox[:, 0] + 0.5 * base_height
    base_ctr_x = dst_bbox[:, 1] + 0.5 * base_width # 计算出目标框中心点坐标

    eps = xp.finfo(height.dtype).eps               # 求出最小的正数
    height = xp.maximum(height, eps)               # #将height,width与其比较保证全部是非负
    width = xp.maximum(width, eps)

    dy = (base_ctr_y - ctr_y) / height
    dx = (base_ctr_x - ctr_x) / width
    dh = xp.log(base_height / height)
    dw = xp.log(base_width / width)               #  根据上面的公式二计算dx，dy，dh，dw

    loc = xp.vstack((dy, dx, dh, dw)).transpose()  # np.vstack按照行的顺序把数组给堆叠起来
    return loc


def bbox_iou(bbox_a, bbox_b):                      # 求两个bbox相交的交并比
    """两个bbox相交的面积与相并的面积比
    """
    if bbox_a.shape[1] != 4 or bbox_b.shape[1] != 4: # 确保bbox第二维为bbox的四个坐标
        raise IndexError

    # tl = 交叉框 top left 坐标； br = 交叉框bottom right坐标。
    tl = xp.maximum(bbox_a[:, None, :2], bbox_b[:, :2])
    br = xp.minimum(bbox_a[:, None, 2:], bbox_b[:, 2:])

    area_i = xp.prod(br - tl, axis=2) * (tl < br).all(axis=2)
    area_a = xp.prod(bbox_a[:, 2:] - bbox_a[:, :2], axis=1)
    area_b = xp.prod(bbox_b[:, 2:] - bbox_b[:, :2], axis=1)
    return area_i / (area_a[:, None] + area_b - area_i) # 计算IOU


def __test():
    pass


if __name__ == '__main__':
    __test()


def generate_anchor_base(base_size=16, ratios=[0.5, 1, 2],
                         anchor_scales=[8, 16, 32]):
    """通过枚举纵横比和比例生成anchor(锚)基窗口。
       具体：按给定的纵横比==生成==>被缩放和被修改的锚。
    """
    py = base_size / 2.
    px = base_size / 2.

    anchor_base = np.zeros((len(ratios) * len(anchor_scales), 4),
                           dtype=np.float32)
    for i in six.moves.range(len(ratios)):
        for j in six.moves.range(len(anchor_scales)):
            h = base_size * anchor_scales[j] * np.sqrt(ratios[i])
            w = base_size * anchor_scales[j] * np.sqrt(1. / ratios[i])

            index = i * len(anchor_scales) + j
            anchor_base[index, 0] = py - h / 2.
            anchor_base[index, 1] = px - w / 2.
            anchor_base[index, 2] = py + h / 2.
            anchor_base[index, 3] = px + w / 2. # #计算出anchor_base画的9个框的左下角和右上角的4个anchor坐标值
    return anchor_base
```

**`generate_anchor_base()`传入参数**；**计算公式**；**返回值**三者关系如下：函数代码如上：
- 
生成anchor(锚)基窗口详情：
`R = len(ratios比率) * len(anchor_scalesr锚定标度)` 生成anchors。（anchors总个数）
`i * len(anchor_scales) + j` 这个anchor ：相对应的由`ratios[i]` 和 `anchor_scales[j]`生成- 
传入参数

base_size=16 ： 参考窗口的宽度和高度

ratios=[0.5, 1, 2] ： 锚的宽高比（可以有多个比率）：宽度与高度的比率

anchor_scales=[8, 16, 32]：锚定区域：anchor_scales中元素的平方与参考窗口的原始区域的乘积。- 
返回值

numpy.ndarray：数据shape=(R, 4)

每个元素都是一组 bounding box(边界框)的坐标，对应 $y_{min}, x_{min}, y_{max}, x_{max}$
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019032814465427.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

该函数返回值为anchor_base，形状9*4，9个anchor的左下右上坐标：
(-37.2548,-82.5097)	(53.2548,98.5097)

(-82.5097,-173.019)	(98.5097,189.019)

(-173.019,-354.039)	(189.019,370.039)

(-56,-56)	(72,72)

(-120,-120)	(136,136)

(-248,-248)	(264,264)

(-82.5097,-37.2548)	(98.5097,53.2548)

(-173.019,-82.5097)	(189.019,98.5097)

(-354.039,-173.019)	(370.039,189.019)
#### **model/utils/creator_tool.py文件**

##### `class AnchorTargetCreator(object)`:
- **作用**： 将图像事实 bounding boxes 指定给anchors用于训练RPN。
- 初始化参数:
`n_sample (int)`: 要生成的区域数.
`pos_iou_thresh (float)`: IOU高于此阈值的锚定将被指定为正.
`neg_iou_thresh (float)`: IOU低于此阈值的锚定将被指定为负.
`pos_ratio (float)`: 采样区域中正区域的比率。
输入: （将ground truth(图上实况)监控分配给锚的抽样子集。最初生成的20000个anchor坐标、此一张图中所有的bbox的真实坐标。）
- 输入参数:
`bbox (array)`: bounding boxes的坐标。数据shape=(R, 4)。R是bounding boxes的数量
`anchor (array)`: anchors的坐标。     数据shape=(S, 4)。S是anchors的数量
`img_size`=(H, W)- 返回值:
`loc(array)`：通过偏移和缩放来匹配anchors到图面真值边界框。数据shape=(S, 4)。S是anchors的数量
`label(array)`：anchors(锚)的标签（1=正，0=负，-1=忽略）。数据shape=(S,)- 过程：
**样本**：训练要用的anchor(与对应框iou值最大或者最小的各128个框的坐标和256个label（0或者1））将图像的事实 bounding boxes（边界框）指定给anchors 形成带标签的**样本**去训练faster_RCNN中的RPN网络。anchor变成真正的ROIS需要进行位置修正。使用 `model.utils.bbox_tools.bbox2loc()`的编码方案计算anchors(锚)与ground truth(图面真实值)匹配的偏移量和比例。
输出：size为（20000，1）的正负label（其中只有128个为1，128个为0，其余都为-1）、 size为（20000，4）的回归目标（所有anchor的坐标都有）

##### `class ProposalTargetCreator(object):`
- **作用**：将(ground truth)地面实况 bounding boxes 分配给给定的ROI。
- 初始化参数:
`n_sample (int)`: 采样区域的数量。
`pos_ratio（float）`：标记为前景的区域的分数。
`pos-iou-thresh（float）`：将ROI视为前景的IOU阈值。
`neg-iou-thresh-hi（float）`：如果IoU在设定参数[,)范围内，则将roi视为背景。
`neg-iou-thresh-lo（float）`：见上文。- 输入参数：
`roi (array)`: 采样的rois。数据shape=(R, 4)
`bbox (array)`: 图面真实bounding boxes坐标.数据shape=(R, 4).
`label (array)`: 图面真实bounding boxes labels. 数据shape=(R,)， R[0, L - 1], 其中L前景类别数- 返回值 (array, array, array)：
`sample_roi`：采样的rois。数据shape=(S, 4);S=128
`gt_roi_loc`：将采样的ROI与图面真值边界框匹配的**偏移和缩放**。数据shape=(S, 4);S=128
``gt_roi_label：分配给采样ROI的标签. 数据shape=(S,) 范围[0, L]，值为0的标签是背景;S=128

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190328144852622.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)``class ProposalCreator:- 
输入参数：``loc (array): 预测anchors的偏移量比例。数据shape=(R, 4)
``score (array): 预测anchors的前景概率。数据shape=(R,).
``anchor (array): anchors坐标。数据shape=(R, 4).
``img_size (tuple of ints：H,W): 包含缩放后的图像大小.
``scale (float): 图像缩放比例.- 
返回值``roi:proposal boxes坐标(array),数据shape=(S, 4):

S在测试时间小于n_test_post_nms，在训练时间小于n_train_post_nms。

S取决于预测边界框的大小和NMS丢弃的边界框的数量。

在看代码前，我们先来看一下代码流程，主要是逻辑转换有点多。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019031813350820.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

第二个AnchorTargetCreator()函数逻辑流程图。函数方法调用流程图也如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190317211531826.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)















AnchorTargetCreator

_get_inside_index

_create_label

bbox2loc

_unmap

_calc_ious

bbox_iou
```python

```
import numpy as np
import cupy as cp

from model.utils.bbox_tools import bbox2loc, bbox_iou, loc2bbox
from model.utils.nms import non_maximum_suppression


classProposalTargetCreator(object):"""将(ground truth)地面实况(bounding boxes)边界框分配给给定的ROI。
    """def__init__(self,
                 n_sample=128,
                 pos_ratio=0.25, pos_iou_thresh=0.5,
                 neg_iou_thresh_hi=0.5, neg_iou_thresh_lo=0.0):
        self.n_sample = n_sample
        self.pos_ratio = pos_ratio
        self.pos_iou_thresh = pos_iou_thresh
        self.neg_iou_thresh_hi = neg_iou_thresh_hi
        self.neg_iou_thresh_lo = neg_iou_thresh_lo  # NOTE:default 0.1 in py-faster-rcnndef__call__(self, roi, bbox, label,
                 loc_normalize_mean=(0.,0.,0.,0.),
                 loc_normalize_std=(0.1,0.1,0.2,0.2)):"""为抽样方案分配(ground truth)地面实况

        此函数对来自：roi和bbox组合的n_sample ROI进行总计采样。.
        ROI分配有ground truth类标签以及边界框偏移和比例，以匹配(ground truth)地面真值边界框
        pos_ratio*n_sample ROI作为前场采样.
        model.utils.bbox_tools.bbox2loc:计算边界框的偏移和比例

        S是采样ROI的总数 = n_sample
        L是可能包括背景的对象类别数
        
        输入参数与返回值 请查看上面该类的解析。
        """
        n_bbox, _ = bbox.shape

        roi = np.concatenate((roi, bbox), axis=0)

        pos_roi_per_image = np.round(self.n_sample * self.pos_ratio)
        iou = bbox_iou(roi, bbox)
        gt_assignment = iou.argmax(axis=1)
        max_iou = iou.max(axis=1)# Offset range of classes from [0, n_fg_class - 1] to [1, n_fg_class].
        gt_roi_label = label[gt_assignment]+1# 值为0的标签是背景# 筛选出大于前景IoU阈值的RoI（感兴趣区域）
        pos_index = np.where(max_iou >= self.pos_iou_thresh)[0]
        pos_roi_per_this_image =int(min(pos_roi_per_image, pos_index.size))if pos_index.size >0:
            pos_index = np.random.choice(
                pos_index, size=pos_roi_per_this_image, replace=False)# Select background RoIs as those within [neg_iou_thresh_lo, neg_iou_thresh_hi).
        neg_index = np.where((max_iou < self.neg_iou_thresh_hi)&(max_iou >= self.neg_iou_thresh_lo))[0]
        neg_roi_per_this_image = self.n_sample - pos_roi_per_this_image
        neg_roi_per_this_image =int(min(neg_roi_per_this_image,neg_index.size))if neg_index.size >0:
            neg_index = np.random.choice(
                neg_index, size=neg_roi_per_this_image, replace=False)# The indices that we're selecting (both positive and negative).
        keep_index = np.append(pos_index, neg_index)
        gt_roi_label = gt_roi_label[keep_index]
        gt_roi_label[pos_roi_per_this_image:]=0# negative labels --> 0
        sample_roi = roi[keep_index]# 计算偏移量和比例，使采样ROI与GTs匹配
        gt_roi_loc = bbox2loc(sample_roi, bbox[gt_assignment[keep_index]])
        gt_roi_loc =((gt_roi_loc - np.array(loc_normalize_mean, np.float32))/ np.array(loc_normalize_std, np.float32))return sample_roi, gt_roi_loc, gt_roi_label


classAnchorTargetCreator(object):"""将图像的事实 bounding boxes（边界框）指定给anchors
       将图像的事实 bounding boxes（边界框）指定给anchors 去训练faster_RCNN中的RPN网络
    """def__init__(self,
                 n_sample=256,
                 pos_iou_thresh=0.7, neg_iou_thresh=0.3,
                 pos_ratio=0.5):
        self.n_sample = n_sample
        self.pos_iou_thresh = pos_iou_thresh
        self.neg_iou_thresh = neg_iou_thresh
        self.pos_ratio = pos_ratio

    def__call__(self, bbox, anchor, img_size):"""将图像的ground truth(图上实况)监控分配给锚的抽样子集。
           数据类型：input arrays = output arrays
		   
		   输入参数与返回值 请查看上面该类的解析。
        """

        img_H, img_W = img_size

        n_anchor =len(anchor)# 一般对应20000个左右anchor
        inside_index = _get_inside_index(anchor, img_H, img_W)# 将那些超出图片范围的anchor全部去掉
        anchor = anchor[inside_index]# 只保留位于图片内部anchor的序号
        argmax_ious, label = self._create_label(
            inside_index, anchor, bbox)# 筛选出符合条件的正例128个负例128并给它们附上相应的label

        loc = bbox2loc(anchor, bbox[argmax_ious])# 计算边界框回归目标

        label = _unmap(label, n_anchor, inside_index, fill=-1)#映射到原始锚点集
        loc = _unmap(loc, n_anchor, inside_index, fill=0)return loc, label

    def_create_label(self, inside_index, anchor, bbox):# label：(正样本为1, 负样本为0, 背景(不关心的忽略值)为-1，）
        label = np.empty((len(inside_index),), dtype=np.int32)# inside_index所有在图片范围内的anchor序号
        label.fill(-1)

        argmax_ious, max_ious, gt_argmax_ious = \
            self._calc_ious(anchor, bbox, inside_index)# assign negative labels first so that positive labels can clobber them
        label[max_ious < self.neg_iou_thresh]=0# anchor与对应的框求得的iou值:若小于负样本阈值，则label=0

        label[gt_argmax_ious]=1# 把与每个bbox求得iou值最大的anchor的label设为1
        label[max_ious >= self.pos_iou_thresh]=1# 正标签：大于设定阈值的IOU#==================正样本抽样处理===================
        n_pos =int(self.pos_ratio * self.n_sample)# 按照比例计算出正样本数量，pos_ratio=0.5，n_sample=256(再次抽样正标签)
        pos_index = np.where(label ==1)[0]# 得到所有正样本的索引iflen(pos_index)> n_pos:# 若选取的正样本数多于预设定数，则随机抛弃，抛弃样本的label设为-1
            disable_index = np.random.choice(
                pos_index, size=(len(pos_index)- n_pos), replace=False)
            label[disable_index]=-1# ==================负样本抽样处理===================
        n_neg = self.n_sample - np.sum(label ==1)
        neg_index = np.where(label ==0)[0]iflen(neg_index)> n_neg:
            disable_index = np.random.choice(
                neg_index, size=(len(neg_index)- n_neg), replace=False)
            label[disable_index]=-1return argmax_ious, label

    def_calc_ious(self, anchor, bbox, inside_index):
        ious = bbox_iou(anchor, bbox)# 计算anchor与bbox的IOU
        argmax_ious = ious.argmax(axis=1)# 获取改行最大iou值的索引
        max_ious = ious[np.arange(len(inside_index)), argmax_ious]
        gt_argmax_ious = ious.argmax(axis=0)
        gt_max_ious = ious[gt_argmax_ious, np.arange(ious.shape[1])]
        gt_argmax_ious = np.where(ious == gt_max_ious)[0]return argmax_ious, max_ious, gt_argmax_ious


def_unmap(data, count, index, fill=0):# Unmap a subset of item (data) back to the original set of items (of# size count)iflen(data.shape)==1:
        ret = np.empty((count,), dtype=data.dtype)
        ret.fill(fill)
        ret[index]= data
    else:
        ret = np.empty((count,)+ data.shape[1:], dtype=data.dtype)
        ret.fill(fill)
        ret[index,:]= data
    return ret


def_get_inside_index(anchor, H, W):# Calc indicies of anchors which are located completely inside of the image# whose size is speficied.
    index_inside = np.where((anchor[:,0]>=0)&(anchor[:,1]>=0)&(anchor[:,2]<= H)&(anchor[:,3]<= W))[0]return index_inside


classProposalCreator:"""建议区域是通过调用此对象生成的（ 这部分操作不需要反向传播，因此可用numpy/tensor实现）
     
    此对象的__call__方法通过将估计的边界框偏移应用到一组锚来输出对象检测(proposals)建议
    此类接受参数以控制传递给NMS的边界框的数量,和经过NMS后的边界框的数量。（若参数=负，保留所有bbox或经过NMS返回的bbox）
   
    Args:
        force_cpu_nms (bool): 若（强制cpu_nms）=True,则始终在cpu模式下使用nms,否则，根据输入类型选择NMS模式。
    """def__init__(self,
                 parent_model,
                 nms_thresh=0.7,# NMS时使用的阈值
                 n_train_pre_nms=12000,# 训练模式下：通过NMS之前要保留的得分最高的边界框数。
                 n_train_post_nms=2000,# 训练模式下：通过NMS后要保留的得分最高的边界框数。
                 n_test_pre_nms=6000,# 测试模式下：传递给nms之前要保留的得分最高的边界框数。
                 n_test_post_nms=300,# 测试模式下：传递给nms后要保留的得分最高的边界框数
                 min_size=16# 根据边界框的大小确定丢弃边界框的阈值.):
        self.parent_model = parent_model
        self.nms_thresh = nms_thresh
        self.n_train_pre_nms = n_train_pre_nms
        self.n_train_post_nms = n_train_post_nms
        self.n_test_pre_nms = n_test_pre_nms
        self.n_test_post_nms = n_test_post_nms
        self.min_size = min_size

    def__call__(self, loc, score,
                 anchor, img_size, scale=1.):""" loc和score是经过region_proposal_network中经过1x1卷积分类和回归得到的
        
        Inputs :loc，score，anchor当用同一索引索引索引时，指的是同一个锚
        On notations：R是anchor(锚)的总数 = 图像(feature map)的高X宽X每个像素的锚定基数。（H/16）x(W/16)x9(大概20000)
		输入参数与返回值 请查看上面该类的解析。
        """# NOTE: when test, remember faster_rcnn.eval() and to set self.traing = Falseif self.parent_model.training:# 若是训练模式
            n_pre_nms = self.n_train_pre_nms        # 12000
            n_post_nms = self.n_train_post_nms      # 经过NMS后有2000个else:
            n_pre_nms = self.n_test_pre_nms         # 6000
            n_post_nms = self.n_test_post_nms       # 经过NMS后有300个

        roi = loc2bbox(anchor, loc)# 将bbox转换为近似groudtruth的anchor(即rois)# Clip predicted boxes to image.
        roi[:,slice(0,4,2)]= np.clip(
            roi[:,slice(0,4,2)],0, img_size[0])
        roi[:,slice(1,4,2)]= np.clip(
            roi[:,slice(1,4,2)],0, img_size[1])# Remove predicted boxes with either height or width < threshold.
        min_size = self.min_size * scale
        hs = roi[:,2]- roi[:,0]# #rois的宽
        ws = roi[:,3]- roi[:,1]
        keep = np.where((hs >= min_size)&(ws >= min_size))[0]# 确保rois的长宽大于最小阈值
        roi = roi[keep,:]
        score = score[keep]# Take top pre_nms_topN (e.g. 6000).
        order = score.ravel().argsort()[::-1]#  将score拉伸并逆序（从高到低）排序if n_pre_nms >0:
            order = order[:n_pre_nms]
        roi = roi[order,:]# Apply nms (e.g. threshold = 0.7).# Take after_nms_topN (e.g. 300).# unNOTE: somthing is wrong here!# TODO: remove cuda.to_gpu# 调用非极大值抑制函数，将重复的抑制掉，就可以将筛选后ROIS进行返回。#     经过NMS处理后Train数据集得到2000个框，Test数据集得到300个框
        keep = non_maximum_suppression(
            cp.ascontiguousarray(cp.asarray(roi)),
            thresh=self.nms_thresh)if n_post_nms >0:
            keep = keep[:n_post_nms]
        roi = roi[keep]return roi     # 取出最终的2000或300个roismodel/utils/roi_cupy.py文件
```python

```
kernel_forward ='''
    extern "C"
    __global__ void roi_forward(const float* const bottom_data,const float* const bottom_rois,
                float* top_data, int* argmax_data,
                const double spatial_scale,const int channels,const int height, 
                const int width, const int pooled_height, 
                const int pooled_width,const int NN
    ){
        
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx>=NN)
        return;
    const int pw = idx % pooled_width;
    const int ph = (idx / pooled_width) % pooled_height;
    const int c = (idx / pooled_width / pooled_height) % channels;
    int num = idx / pooled_width / pooled_height / channels;
    const int roi_batch_ind = bottom_rois[num * 5 + 0];
    const int roi_start_w = round(bottom_rois[num * 5 + 1] * spatial_scale);
    const int roi_start_h = round(bottom_rois[num * 5 + 2] * spatial_scale);
    const int roi_end_w = round(bottom_rois[num * 5 + 3] * spatial_scale);
    const int roi_end_h = round(bottom_rois[num * 5 + 4] * spatial_scale);
    // Force malformed ROIs to be 1x1
    const int roi_width = max(roi_end_w - roi_start_w + 1, 1);
    const int roi_height = max(roi_end_h - roi_start_h + 1, 1);
    const float bin_size_h = static_cast<float>(roi_height)
                    / static_cast<float>(pooled_height);
    const float bin_size_w = static_cast<float>(roi_width)
                    / static_cast<float>(pooled_width);

    int hstart = static_cast<int>(floor(static_cast<float>(ph)
                                    * bin_size_h));
        int wstart = static_cast<int>(floor(static_cast<float>(pw)
                                    * bin_size_w));
    int hend = static_cast<int>(ceil(static_cast<float>(ph + 1)
                                * bin_size_h));
        int wend = static_cast<int>(ceil(static_cast<float>(pw + 1)
                                * bin_size_w));

    // Add roi offsets and clip to input boundaries
    hstart = min(max(hstart + roi_start_h, 0), height);
    hend = min(max(hend + roi_start_h, 0), height);
    wstart = min(max(wstart + roi_start_w, 0), width);
    wend = min(max(wend + roi_start_w, 0), width);
    bool is_empty = (hend <= hstart) || (wend <= wstart);

    // Define an empty pooling region to be zero
    float maxval = is_empty ? 0 : -1E+37;
    // If nothing is pooled, argmax=-1 causes nothing to be backprop'd
    int maxidx = -1;
    const int data_offset = (roi_batch_ind * channels + c) * height * width;
    for (int h = hstart; h < hend; ++h) {
        for (int w = wstart; w < wend; ++w) {
            int bottom_index = h * width + w;
            if (bottom_data[data_offset + bottom_index] > maxval) {
                maxval = bottom_data[data_offset + bottom_index];
                maxidx = bottom_index;
            }
        }
    }
    top_data[idx]=maxval;
    argmax_data[idx]=maxidx;
    }
'''
kernel_backward ='''
    extern "C"
    __global__ void roi_backward(const float* const top_diff,
         const int* const argmax_data,const float* const bottom_rois,
         float* bottom_diff, const int num_rois,
         const double spatial_scale, int channels,
         int height, int width, int pooled_height,
          int pooled_width,const int NN)
    {

    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    ////Importtan >= instead of >
    if(idx>=NN)
        return;
    int w = idx % width;
    int h = (idx / width) % height;
    int c = (idx/ (width * height)) % channels;
    int num = idx / (width * height * channels);

    float gradient = 0;
    // Accumulate gradient over all ROIs that pooled this element
    for (int roi_n = 0; roi_n < num_rois; ++roi_n) {
        // Skip if ROI's batch index doesn't match num
        if (num != static_cast<int>(bottom_rois[roi_n * 5])) {
            continue;
        }

        int roi_start_w = round(bottom_rois[roi_n * 5 + 1]
                                * spatial_scale);
        int roi_start_h = round(bottom_rois[roi_n * 5 + 2]
                                * spatial_scale);
        int roi_end_w = round(bottom_rois[roi_n * 5 + 3]
                                * spatial_scale);
        int roi_end_h = round(bottom_rois[roi_n * 5 + 4]
                                * spatial_scale);

        // Skip if ROI doesn't include (h, w)
        const bool in_roi = (w >= roi_start_w && w <= roi_end_w &&
                                h >= roi_start_h && h <= roi_end_h);
        if (!in_roi) {
            continue;
        }

        int offset = (roi_n * channels + c) * pooled_height
                        * pooled_width;

        // Compute feasible set of pooled units that could have pooled
        // this bottom unit

        // Force malformed ROIs to be 1x1
        int roi_width = max(roi_end_w - roi_start_w + 1, 1);
        int roi_height = max(roi_end_h - roi_start_h + 1, 1);

        float bin_size_h = static_cast<float>(roi_height)
                        / static_cast<float>(pooled_height);
        float bin_size_w = static_cast<float>(roi_width)
                        / static_cast<float>(pooled_width);

        int phstart = floor(static_cast<float>(h - roi_start_h)
                            / bin_size_h);
        int phend = ceil(static_cast<float>(h - roi_start_h + 1)
                            / bin_size_h);
        int pwstart = floor(static_cast<float>(w - roi_start_w)
                            / bin_size_w);
        int pwend = ceil(static_cast<float>(w - roi_start_w + 1)
                            / bin_size_w);

        phstart = min(max(phstart, 0), pooled_height);
        phend = min(max(phend, 0), pooled_height);
        pwstart = min(max(pwstart, 0), pooled_width);
        pwend = min(max(pwend, 0), pooled_width);
        for (int ph = phstart; ph < phend; ++ph) {
            for (int pw = pwstart; pw < pwend; ++pw) {
                int index_ = ph * pooled_width + pw + offset;
                if (argmax_data[index_] == (h * width + w)) {
                    gradient += top_diff[index_];
                }
            }
        }
    }
    bottom_diff[idx] = gradient;
    }
'''rpn_loss与roi_loss的异同：

两者都是分类与回归的多目标损失。所以Faster-RCNN共有4个子损失函数。- 


对于 rpn_loss中的分类是2分类，是256个样本参与，正负样本各一半，分类预测值是rpn网络的11卷积输出，分类真实标签是AnchorTargetCreator生成的ground truth。 rpn_loss中的回归样本数是所有20000个（严格讲是20000个bbox中所有完整出现在原图中的bbox）bbox来参与，回归预测值是rpn网络的另一个11卷积输出，回归目标是AnchorTargetCreator生成的ground truth。- 


对于roi_loss中的分类是21分类，是128个样本参与，正负样本1：3。分类预测值是Roi_head网络的FC21输出，分类真实标签是ProposalTargetCreator生成的ground truth。roi_loss中的回归样本数是128个，回归预测值是Roi_head网络的FC84输出，回归目标是ProposalTargetCreator生成的ground truth。

















