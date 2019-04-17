# faster rcnn pytorch 复现系列（二）：generate_anchors源码解析 - Snoopy_Dream - CSDN博客





2018年10月31日 11:38:25[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：305
所属专栏：[faster-rcnn.pytorch](https://blog.csdn.net/column/details/25674.html)









**目录**[​](#%E2%80%8B)

[1. 总函数](#)[generate_anchors](#)

[2. 函数分功能写，首先是ratios的实现，其次是scale的实现](#2.%20%E5%87%BD%E6%95%B0%E5%88%86%E5%8A%9F%E8%83%BD%E5%86%99%EF%BC%8C%E9%A6%96%E5%85%88%E6%98%AFratios%E7%9A%84%E5%AE%9E%E7%8E%B0%EF%BC%8C%E5%85%B6%E6%AC%A1%E6%98%AFscale%E7%9A%84%E5%AE%9E%E7%8E%B0)

[3. anchor2WHXY函数+WsHsXsYs2anchors函数[s表示复数]](#3.%20anchor2WHXY%E5%87%BD%E6%95%B0%2BWsHsXsYs2anchors%E5%87%BD%E6%95%B0%5Bs%E8%A1%A8%E7%A4%BA%E5%A4%8D%E6%95%B0%5D)

[4.  _ratio_enum(anchor,ratios)](#3.%E6%97%A0%E8%AE%BA%E6%98%AFratio%E8%BF%98%E6%98%AFscale%E7%9A%84%E5%AE%9E%E7%8E%B0%EF%BC%8C%E6%9C%AC%E8%BA%AB%E7%9A%84%E4%B8%AD%E5%BF%83%E7%82%B9%E6%B2%A1%E6%9C%89%E5%8F%98%EF%BC%8C%E5%8F%AA%E6%98%AF%E9%95%BF%E5%AE%BD%E6%AF%94%E5%8F%91%E7%94%9F%E4%BA%86%E5%8F%98%E5%8C%96%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0%C2%A0)

[5 ._scale_enum(anchor,scales)](#5.%E5%AF%B9%E4%B8%8A%E4%B8%80%E6%AD%A5%E5%BE%97%E5%88%B0%E7%9A%84ratio_anchors%E4%B8%AD%E7%9A%84%E4%B8%89%E7%A7%8D%E5%AE%BD%E9%AB%98%E6%AF%94%E7%9A%84anchor%EF%BC%8C%E5%86%8D%E5%88%86%E5%88%AB%E8%BF%9B%E8%A1%8C%E4%B8%89%E7%A7%8Dscale%E7%9A%84%E5%8F%98%E6%8D%A2)

[6. 最终获得的anchors的坐标：](#6.%E6%9C%80%E7%BB%88%E7%BB%8F%E8%BF%87%E5%AE%BD%E9%AB%98%E6%AF%94%E5%92%8Cscale%E5%8F%98%E6%8D%A2%E5%BE%97%E5%88%B0%E7%9A%849%E7%A7%8D%E5%B0%BA%E5%AF%B8%E7%9A%84anchors%E7%9A%84%E5%9D%90%E6%A0%87%E5%A6%82%E4%B8%8B%EF%BC%9A)

**学习到的变成知识点：**

函数按照功能分开写，**每一个函数只实现一个功能，如果需要叠加，交给循环或者np.vstack**

其中ratios函数的功能：一次只实现3种ratios的变化 _ratio_enum

scale的功能：一次只实现3种scale的变化，_scale_enum

要得到3*3anchors，交给stack处理



首先，添加print，然后直接运行py文件，生成anchor结果

**大意理解：**

**由一个anchor得到9个anchors**

[0 0 15 15]



[[ -84.  -40.   99.   55.]

 [-176.  -88.  191.  103.]

 [-360. -184.  375.  199.]

 [ -56.  -56.   71.   71.]

 [-120. -120.  135.  135.]

 [-248. -248.  263.  263.]

 [ -36.  -80.   51.   95.]

 [ -80. -168.   95.  183.]

 [-168. -344.  183.  359.]]



![](https://img-blog.csdnimg.cn/20181031112514552.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

### ![](https://img-blog.csdnimg.cn/20181031102948533.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

### 1. 总函数 generate_anchors

  输入包括：特征图对应于原图的大小，ratios长宽比，scales放大倍数

```python
def generate_anchors(base_size=16, ratios=[0.5, 1, 2],
                     scales=2**np.arange(3, 6)):
    """
    Generate anchor (reference) windows by enumerating aspect ratios X
    scales wrt a reference (0, 0, 15, 15) window.
    """
    base_anchor = np.array([1, 1, base_size, base_size]) - 1
    print ("base anchors",base_anchor)

    ratio_anchors = _ratio_enum(base_anchor, ratios)
    print ("anchors after ratio",ratio_anchors)
    """
    对ratios后处理得到的每一行，分别进行3次scale变化，循环三次
    """
    anchors = np.vstack([_scale_enum(ratio_anchors[i, :], scales)
                         for i in xrange(ratio_anchors.shape[0])])
    print ("achors after ration and scale",anchors)
    return anchors
```

### 2. 函数分功能写，首先是ratios的实现，其次是scale的实现

  其中ratios函数的功能：一次实现3种ratios的变化

  scale的功能：一次实现3中scale的变化，其余交给循环处理x

### 3. anchor2WHXY函数+WsHsXsYs2anchors函数[s表示复数]

  无论是ratio还是scale的实现，本身的中心点没有变，只是长宽比发生了变化 ，所以需要相互转换。

  下面两个scales和ratios的函数的实现，都是先将anchor2whxy然后WsHsXsYs2anchors 。

```python
def _whctrs(anchor):
    """
    Return width, height, x center, and y center for an anchor (window).
    本身负责的只是一个行向量的转变,x0 y0 x1 y1 to  xc yc w h
    input:anchor [x0,y0,x1,y1]
    """
    w = anchor[2] - anchor[0] + 1
    h = anchor[3] - anchor[1] + 1
    x_ctr = anchor[0] + 0.5 * (w - 1)
    y_ctr = anchor[1] + 0.5 * (h - 1)
    return w, h, x_ctr, y_ctr

def _mkanchors(ws, hs, x_ctr, y_ctr):
    """
    input:xc yc w_after h_after
    out:anchors 注意这里是anchors复数
    method:首先需要将w改成列向量，然后水平拼接
    
             x0  y0 x1 y1
    state1
    state2
    state3
    ...
    """
    #(x_ctr, y_ctr) 7.5  7.5  
    ws = ws[:, np.newaxis]#[[23],[16],[11]]  col 
    hs = hs[:, np.newaxis]#[[12],[16],[22]]
    anchors = np.hstack((x_ctr - 0.5 * (ws - 1),
                         y_ctr - 0.5 * (hs - 1),
                         x_ctr + 0.5 * (ws - 1),
                         y_ctr + 0.5 * (hs - 1)))
    return anchors
```

### 4.  _ratio_enum(anchor,ratios) 

  输入:一个anchor,return都是三个anchor。

```python
def _ratio_enum(anchor, ratios):
    """
    input: 
          anchor[np.array]  [0,0,15,15] 
          ratios[list]   [0.5,1,2]
    output:
          anchors
    method:
          1.x0 y0 x1 y1 to xc yc w h 
          2.compute w_after and h_after
          3.xc yc w h to x0 y0 x1 y1
    """
    
    w, h, x_ctr, y_ctr = _whctrs(anchor)#
    size = w * h   #size:16*16=256
    size_ratios = size / ratios  #256/ratios[0.5,1,2]=[512,256,128]
    #round()方法返回x的四舍五入的数字，sqrt()方法返回数字x的平方根
    ws = np.round(np.sqrt(size_ratios)) #ws:[23 16 11]
    hs = np.round(ws * ratios)    #hs:[12 16 22],ws和hs一一对应。as:23&12
    #给定一组宽高向量，输出各个预测窗口，也就是将（宽，高，中心点横坐标，中心点纵坐标）的形式，转成
    #四个坐标值的形式
    anchors = _mkanchors(ws, hs, x_ctr, y_ctr)  
    return anchors
```

高比变换之后anchors的坐标

```python
ratio_anchors = _ratio_enum(base_anchor, ratios)
       x0     y0      x1     y1
'''[[ -3.5,   2. ,  18.5,  13. ],
    [  0. ,   0. ,  15. ,  15. ],
    [  2.5,  -3. ,  12.5,  18. ]]'''
```

### 5 ._scale_enum(anchor,scales)

 对上一步得到的ratio_anchors中的三种宽高比的anchor，再分别进行三种scale的变换

 输入:一个anchor,return都是三个anchor。

```python
def _scale_enum(anchor, scales):
    """
    输入一个anchr行向量，一次得到3种变化
    """

    w, h, x_ctr, y_ctr = _whctrs(anchor)
    ws = w * scales
    hs = h * scales
    anchors = _mkanchors(ws, hs, x_ctr, y_ctr)
    return anchors
```

### 6. 最终获得的anchors的坐标：

```python
anchors = np.vstack([_scale_enum(ratio_anchors[i, :], scales)
                         for i in xrange(ratio_anchors.shape[0])])
'''
[[ -84.  -40.   99.   55.]
 [-176.  -88.  191.  103.]
 [-360. -184.  375.  199.]
 [ -56.  -56.   71.   71.]
 [-120. -120.  135.  135.]
 [-248. -248.  263.  263.]
 [ -36.  -80.   51.   95.]
 [ -80. -168.   95.  183.]
 [-168. -344.  183.  359.]]
```



