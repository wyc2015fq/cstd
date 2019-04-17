# 目标检测中IOU和NMS的python实现 - 别说话写代码的博客 - CSDN博客





2018年12月30日 20:00:04[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：106








IOU：两个框的交并比

```python
import numpy as np
def compute_iou(box1, box2, wh=False):
    """
    compute the iou of two boxes.
    Args:
        box1, box2: [xmin, ymin, xmax, ymax] (wh=False) or [xcenter, ycenter, w, h] (wh=True)
        wh: the format of coordinate.
    Return:
        iou: iou of box1 and box2.
    """
    if wh == False:
        xmin1, ymin1, xmax1, ymax1 = box1
        xmin2, ymin2, xmax2, ymax2 = box2
    else:
        xmin1, ymin1 = int(box1[0]-box1[2]/2.0), int(box1[1]-box1[3]/2.0)
        xmax1, ymax1 = int(box1[0]+box1[2]/2.0), int(box1[1]+box1[3]/2.0)
        xmin2, ymin2 = int(box2[0]-box2[2]/2.0), int(box2[1]-box2[3]/2.0)
        xmax2, ymax2 = int(box2[0]+box2[2]/2.0), int(box2[1]+box2[3]/2.0)

    ## 获取矩形框交集对应的左上角和右下角的坐标（intersection）
    xx1 = np.max([xmin1, xmin2])
    yy1 = np.max([ymin1, ymin2])
    xx2 = np.min([xmax1, xmax2])
    yy2 = np.min([ymax1, ymax2])
    
    if xx1>xx2 or yy1>yy2:   #两个框不相交
        return 0
    ## 计算两个矩形框面积
    area1 = (xmax1-xmin1) * (ymax1-ymin1) 
    area2 = (xmax2-xmin2) * (ymax2-ymin2)

    inter_area = (np.max([0, xx2-xx1])) * (np.max([0, yy2-yy1]))　#计算交集面积
    iou = inter_area / (area1+area2-inter_area+1e-6)　＃计算交并比

    return iou
```

NMS：非极大值抑制(这里转自：[https://blog.csdn.net/sinat_34474705/article/details/80045294](https://blog.csdn.net/sinat_34474705/article/details/80045294)）

```
for object in all objects:
    (1) 获取当前目标类别下所有bbx的信息
    (2) 将bbx按照confidence从高到低排序,并记录当前confidence最大的bbx
    (3) 计算最大confidence对应的bbx与剩下所有的bbx的IOU,移除所有大于IOU阈值的bbx
    (4) 对剩下的bbx，循环执行(2)和(3)直到所有的bbx均满足要求（即不能再移除bbx）
```

NMS是对所有的类别分别执行的。举个栗子，假设最后预测出的矩形框有2类（分别为cup, pen）,在NMS之前，每个类别可能都会有不只一个bbx被预测出来，这个时候我们需要对这两个类别分别执行一次NMS过程。

我们用python编写NMS代码，假设对于一张图片，所有的bbx信息已经保存在一个字典中，保存形式如下：

```python
predicts_dict: {"cup": [[x1_1, y1_1, x2_1, y2_1, scores1], [x1_2, y1_2, x2_2, y2_2, scores2], ...], "pen": [[x1_1, y1_1, x2_1, y2_1, scores1], [x1_2, y1_2, x2_2, y2_2, scores2], ...]}.
```

即目标的位置和置信度用列表储存，每个列表中的一个子列表代表一个bbx信息。详细的代码如下：

```python
def non_max_suppress(predicts_dict, threshold=0.2):
    """
    implement non-maximum supression on predict bounding boxes.
    Args:
        predicts_dict: {"stick": [[x1, y1, x2, y2, scores1], [...]]}.
        threshhold: iou threshold
    Return:
        predicts_dict processed by non-maximum suppression
    """
    for object_name, bbox in predicts_dict.items():   #对每一个类别的目标分别进行NMS
        bbox_array = np.array(bbox, dtype=np.float)

        ## 获取当前目标类别下所有矩形框（bounding box,下面简称bbx）的坐标和confidence,并计算所有bbx的面积
        x1, y1, x2, y2, scores = bbox_array[:,0], bbox_array[:,1], bbox_array[:,2], bbox_array[:,3], bbox_array[:,4]
        areas = (x2-x1+1) * (y2-y1+1)
        #print "areas shape = ", areas.shape

        ## 对当前类别下所有的bbx的confidence进行从高到低排序（order保存索引信息）
        order = scores.argsort()[::-1]
        print "order = ", order
        keep = [] #用来存放最终保留的bbx的索引信息

        ## 依次从按confidence从高到低遍历bbx，移除所有与该矩形框的IOU值大于threshold的矩形框
        while order.size > 0:
            i = order[0]
            keep.append(i) #保留当前最大confidence对应的bbx索引

            ## 获取所有与当前bbx的交集对应的左上角和右下角坐标，并计算IOU（注意这里是同时计算一个bbx与其他所有bbx的IOU）
            xx1 = np.maximum(x1[i], x1[order[1:]])　#当order.size=1时，下面的计算结果都为np.array([]),不影响最终结果
            yy1 = np.maximum(y1[i], y1[order[1:]])
            xx2 = np.minimum(x2[i], x2[order[1:]])
            yy2 = np.minimum(y2[i], y2[order[1:]])
            inter = np.maximum(0.0, xx2-xx1+1) * np.maximum(0.0, yy2-yy1+1)
            iou = inter/(areas[i]+areas[order[1:]]-inter)
            print "iou =", iou

            print np.where(iou<=threshold) #输出没有被移除的bbx索引（相对于iou向量的索引）
            indexs = np.where(iou<=threshold)[0] + 1 #获取保留下来的索引(因为没有计算与自身的IOU，所以索引相差１，需要加上)
            print "indexs = ", type(indexs)
            order = order[indexs] #更新保留下来的索引
            print "order = ", order
        bbox = bbox_array[keep]
        predicts_dict[object_name] = bbox.tolist()
        predicts_dict = predicts_dict
    return predicts_dict
```



```
import numpy as np

class Bounding_box:
    def __init__(self, x1, y1, x2, y2, score):
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2
        self.score = score

def get_iou(boxa, boxb):
    max_x = max(boxa.x1, boxb.x1)
    max_y = max(boxa.y1, boxb.y1)
    min_x = min(boxa.x2, boxb.x2)
    min_y = min(boxa.y2, boxb.y2)
    if min_x <= max_x or min_y <= max_y:
        return 0
    area_i = (min_x - max_x) * (min_y - max_y)
    area_a = (boxa.x2 - boxa.x1) * (boxa.y2 - boxa.y1)
    area_b = (boxb.x2 - boxb.x1) * (boxb.y2 - boxb.y1)
    area_u = area_a + area_b - area_i
    return float(area_i) / float(area_u)

def NMS(box_lists, k):
    box_lists = sorted(box_lists, key=lambda x: x.score, reverse=True)
    NMS_lists = [box_lists[0]]
    temp_lists = []
    for i in range(k):
        for j in range(1, len(box_lists)):
            iou = get_iou(NMS_lists[i], box_lists[j])
            if iou < 0.7:
                temp_lists.append(box_lists[j])
        if len(temp_lists) == 0:
            return NMS_lists
        box_lists = temp_lists
        temp_lists = []
        NMS_lists.append(box_lists[0])
    return NMS_lists

box1 = Bounding_box(13, 22, 268, 367, 0.124648176)
box2 = Bounding_box(18, 27, 294, 400, 0.35818103)
box3 = Bounding_box(234, 123, 466, 678, 0.13638769)
box_lists = [box1, box2, box3]
NMS_list = NMS(box_lists, 2)
print NMS_list
print NMS_list[0].x1
```

参考：[https://blog.csdn.net/sinat_34474705/article/details/80045294](https://blog.csdn.net/sinat_34474705/article/details/80045294)



