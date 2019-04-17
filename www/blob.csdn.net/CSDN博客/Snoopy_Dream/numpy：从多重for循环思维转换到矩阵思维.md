# numpy：从多重for循环思维转换到矩阵思维 - Snoopy_Dream - CSDN博客





2018年11月05日 22:07:19[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：102
所属专栏：[faster-rcnn.pytorch](https://blog.csdn.net/column/details/25674.html)









例子是bbox_overlaps计算anchors和gts的iou。

**输入：**    anchors: (N, 4) ndarray of float

                gt_boxes: (K, 4) ndarray of float

**输出：****overlaps: (N, K) ndarray of overlap between boxes and query_boxes**

**首先想到**for i in range(N)

               for j  in range(K)

                overlaps[i][i] = ****

**矩阵思维：创建新的维度**

#anchors(N, 4) _>boxes(N, K, 4)

#gt_boxes: (K, 4) _> query_boxes(N, K, 4)

![](https://img-blog.csdnimg.cn/20181106113101548.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

```python
def bbox_overlaps(anchors, gt_boxes):
    """
    anchors: (N, 4) ndarray of float
    gt_boxes: (K, 4) ndarray of float

    overlaps: (N, K) ndarray of overlap between boxes and query_boxes
    """
    N = anchors.size(0)
    K = gt_boxes.size(0)
    #gt_boxes_area (1, K)
    gt_boxes_area = ((gt_boxes[:,2] - gt_boxes[:,0] + 1) *
                (gt_boxes[:,3] - gt_boxes[:,1] + 1)).view(1, K)
    #anchors_area (N, 1)
    anchors_area = ((anchors[:,2] - anchors[:,0] + 1) *
                (anchors[:,3] - anchors[:,1] + 1)).view(N, 1)
    #anchors(N, 4) _>boxes(N, K, 4)
    boxes = anchors.view(N, 1, 4).expand(N, K, 4)
    #gt_boxes: (K, 4) _> query_boxes(N, K, 4)
    query_boxes = gt_boxes.view(1, K, 4).expand(N, K, 4)

    iw = (torch.min(boxes[:,:,2], query_boxes[:,:,2]) -
        torch.max(boxes[:,:,0], query_boxes[:,:,0]) + 1)
    iw[iw < 0] = 0

    ih = (torch.min(boxes[:,:,3], query_boxes[:,:,3]) -
        torch.max(boxes[:,:,1], query_boxes[:,:,1]) + 1)
    ih[ih < 0] = 0
    #broad (1, K)+(N, 1)=  (N, K)     -  (N, K)
    ua = anchors_area + gt_boxes_area - (iw * ih)
    overlaps = iw * ih / ua

    return overlaps
```



