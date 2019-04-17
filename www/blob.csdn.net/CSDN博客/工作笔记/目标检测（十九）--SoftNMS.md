# 目标检测（十九）--SoftNMS - 工作笔记 - CSDN博客





2017年09月13日 11:14:05[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：11080
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

转自：[http://blog.csdn.net/shuzfan/article/details/71036040](http://blog.csdn.net/shuzfan/article/details/71036040)

一篇讲通过改进NMS来提高检测效果的论文。

文章链接： [《Improving Object Detection With One Line of Code》](http://cn.arxiv.org/abs/1704.04503)

Github链接： [https://github.com/bharatsingh430/soft-nms](https://github.com/bharatsingh430/soft-nms)

## **Motivation**

绝大部分目标检测方法，最后都要用到 [NMS-非极大值抑制](http://blog.csdn.net/shuzfan/article/details/52711706)进行后处理。 通常的做法是将检测框按得分排序，然后保留得分最高的框，同时删除与该框重叠面积大于一定比例的其它框。

这种贪心式方法存在如下图所示的问题： 红色框和绿色框是当前的检测结果，二者的得分分别是0.95和0.80。如果按照传统的NMS进行处理，首先选中得分最高的红色框，然后绿色框就会因为与之重叠面积过大而被删掉。

另一方面，NMS的阈值也不太容易确定，设小了会出现下图的情况（绿色框因为和红色框重叠面积较大而被删掉），设置过高又容易增大误检。

![float](https://img-blog.csdn.net/20170430162558050?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h1emZhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**思路：不要粗鲁地删除所有IOU大于阈值的框，而是降低其置信度。**

## **Method**

先直接上伪代码，如下图：如文章题目而言，就是用一行代码来替换掉原来的NMS。**按照下图整个处理一遍之后，指定一个置信度阈值，然后最后得分大于该阈值的检测框得以保留**

![float](https://img-blog.csdn.net/20170430181251698?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h1emZhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

原来的NMS可以描述如下：将IOU大于阈值的窗口的得分全部置为0。

![float](https://img-blog.csdn.net/20170430184812363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h1emZhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

文章的改进有两种形式，一种是**线性加权**的：

![float](https://img-blog.csdn.net/20170430185157861?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h1emZhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

一种是**高斯加权**的：

![float](https://img-blog.csdn.net/20170430185212127?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h1emZhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

分析上面的两种改进形式，思想都是：**M为当前得分最高框，b i   为待处理框，b i   和M的IOU越大，b i   的得分s i   就下降的越厉害。**

具体地，下面是作者给出的代码：(当然不止一行T_T)

```python
def cpu_soft_nms(np.ndarray[float, ndim=2] boxes, float sigma=0.5, float Nt=0.3, float threshold=0.001, unsigned int method=0):
    cdef unsigned int N = boxes.shape[0]
    cdef float iw, ih, box_area
    cdef float ua
    cdef int pos = 0
    cdef float maxscore = 0
    cdef int maxpos = 0
    cdef float x1,x2,y1,y2,tx1,tx2,ty1,ty2,ts,area,weight,ov

    for i in range(N):
        maxscore = boxes[i, 4]
        maxpos = i

        tx1 = boxes[i,0]
        ty1 = boxes[i,1]
        tx2 = boxes[i,2]
        ty2 = boxes[i,3]
        ts = boxes[i,4]

        pos = i + 1
    # get max box
        while pos < N:
            if maxscore < boxes[pos, 4]:
                maxscore = boxes[pos, 4]
                maxpos = pos
            pos = pos + 1

    # add max box as a detection 
        boxes[i,0] = boxes[maxpos,0]
        boxes[i,1] = boxes[maxpos,1]
        boxes[i,2] = boxes[maxpos,2]
        boxes[i,3] = boxes[maxpos,3]
        boxes[i,4] = boxes[maxpos,4]

    # swap ith box with position of max box
        boxes[maxpos,0] = tx1
        boxes[maxpos,1] = ty1
        boxes[maxpos,2] = tx2
        boxes[maxpos,3] = ty2
        boxes[maxpos,4] = ts

        tx1 = boxes[i,0]
        ty1 = boxes[i,1]
        tx2 = boxes[i,2]
        ty2 = boxes[i,3]
        ts = boxes[i,4]

        pos = i + 1
    # NMS iterations, note that N changes if detection boxes fall below threshold
        while pos < N:
            x1 = boxes[pos, 0]
            y1 = boxes[pos, 1]
            x2 = boxes[pos, 2]
            y2 = boxes[pos, 3]
            s = boxes[pos, 4]

            area = (x2 - x1 + 1) * (y2 - y1 + 1)
            iw = (min(tx2, x2) - max(tx1, x1) + 1)
            if iw > 0:
                ih = (min(ty2, y2) - max(ty1, y1) + 1)
                if ih > 0:
                    ua = float((tx2 - tx1 + 1) * (ty2 - ty1 + 1) + area - iw * ih)
                    ov = iw * ih / ua #iou between max box and detection box

                    if method == 1: # linear
                        if ov > Nt: 
                            weight = 1 - ov
                        else:
                            weight = 1
                    elif method == 2: # gaussian
                        weight = np.exp(-(ov * ov)/sigma)
                    else: # original NMS
                        if ov > Nt: 
                            weight = 0
                        else:
                            weight = 1

                    boxes[pos, 4] = weight*boxes[pos, 4]

            # if box score falls below threshold, discard the box by swapping with last box
            # update N
                    if boxes[pos, 4] < threshold:
                        boxes[pos,0] = boxes[N-1, 0]
                        boxes[pos,1] = boxes[N-1, 1]
                        boxes[pos,2] = boxes[N-1, 2]
                        boxes[pos,3] = boxes[N-1, 3]
                        boxes[pos,4] = boxes[N-1, 4]
                        N = N - 1
                        pos = pos - 1

            pos = pos + 1

    keep = [i for i in range(N)]
    return keep
```

![](http://static.blog.csdn.net/images/save_snippets.png)

## **Experiments**

下图可以看出，基本可以获得平均1%的提升，且不增加额外的训练和计算负担。

![float](https://img-blog.csdn.net/20170430193152979?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h1emZhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

高斯方差以及NMS的IOU阈值的敏感性测试：

![float](https://img-blog.csdn.net/20170430194501090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h1emZhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



