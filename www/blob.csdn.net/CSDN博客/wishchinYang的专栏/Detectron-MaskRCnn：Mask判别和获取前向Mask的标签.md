# Detectron-MaskRCnn：Mask判别和获取前向Mask的标签 - wishchinYang的专栏 - CSDN博客
2018年03月23日 13:19:39[wishchin](https://me.csdn.net/wishchin)阅读数：442
        对于FCN-SceneParse网络，最后卷积生成N个类别的maps,每个Map都得到图像所有点的单类概率。MaskRCNN的结构与FCN不相同。
       参考三个文章：
       Detectron总结1：[Blob的生成 和 generate propos](https://blog.csdn.net/u013548568/article/details/79468365)al  
       Detectron总结2：[前向生成数据标签](https://blog.csdn.net/u013548568/article/details/79468365)的过程 
       Detectron总结3：参数设置，直接打开cfg文件，找到train进行设置
       Detectron总结4：[Detectron测试流程](https://blog.csdn.net/u013548568/article/details/79505996)（含Mask和KeyPoint）
![](https://img-blog.csdn.net/20180323131913615)
获取多个Mask：
       对于预测的二值掩膜输出，我们对每个像素点应用sigmoid函数，整体损失定义为平均二值交叉损失熵。 **引入预测个输出的机制，允许每个类都生成独立的掩膜，避免类间竞争。这样做解耦了掩膜和种类预测**。不像是FCN的方法，在每个像素点上应用softmax函数，整体采用的多任务交叉熵，这样会导致类间竞争，最终导致分割效果差。
      代码段：
获取每一个图像Mask
```python
def getMaskImg( mask,box,image ):
    imageMask = image.copy();
    for i in range(mask.shape[0]):
        for j in range(mask.shape[1]):
            if( mask[i,j]==0 ):
                #imageMask[i,j] = image[i,j]
                #mask[i,j] =254;
                imageMask[i,j,0]=0;
                imageMask[i,j,1]=0;
                imageMask[i,j,2]=0;
    
    x1 = int(box[0]);
    x2 = int(box[2]);
    y1 = int(box[1]);
    y2 = int(box[3]);
    imageMask = imageMask[y1:y2,x1:x2];
                
    return imageMask;
```
![](https://img-blog.csdn.net/20180403144926192)
![](https://img-blog.csdn.net/20180403145127831)![](https://img-blog.csdn.net/20180403145153197)![](https://img-blog.csdn.net/20180403145215431)![](https://img-blog.csdn.net/20180403145232551)

