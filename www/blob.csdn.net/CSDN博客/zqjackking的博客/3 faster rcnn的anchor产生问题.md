# 3 faster rcnn的anchor产生问题 - zqjackking的博客 - CSDN博客





2017年03月02日 22:26:46[zqjackking](https://me.csdn.net/zqjackking)阅读数：2968








这一部分是在rpn文件夹下面。 

首先看generate_anchors.py。这个模块直接返回anchor。 

anchor=[[ -83.,  -39.,  100.,   56.], 

       [-175.,  -87.,  192.,  104.], 

       [-359., -183.,  376.,  200.], 

       [ -55.,  -55.,   72.,   72.], 

       [-119., -119.,  136.,  136.], 

       [-247., -247.,  264.,  264.], 

       [ -35.,  -79.,   52.,   96.], 

       [ -79., -167.,   96.,  184.], 

       [-167., -343.,  184.,  360.]] 

这是一个9*4的矩阵。 

假设输入图片为1000*600，那么经过前5层卷积网络之后，feature map变为60*40(除以16） 

按照论文里面的叙述，feature map上每个点都在原始图上对应9个anchor，这9个anchor的大小有3种（按面积算，分别为128*128，256*256，512*512），纵横比有3种（1：1，1：2，2：1），而且每个anchor都以此点为中心。问题是，上面的anchor对应的是feature map上（0，0）对应的9个框。那么feature map上其他地方对应的框怎么找呢。 

      接下来就要看anchor_target_layer.py了。 

      关键是找到偏移量。
```
shift_x = np.arange(0, width) * self._feat_stride   #[0,16,32,.....,(width-1)*16]
shift_y = np.arange(0, height) * self._feat_stride
shift_x, shift_y = np.meshgrid(shift_x, shift_y)     #见onenote。shift_x,shift_y都是宽为width，高为height的矩阵
shifts = np.vstack((shift_x.ravel(), shift_y.ravel(),
                    shift_x.ravel(), shift_y.ravel())).transpose()
```

这里举个例子。

```
x=np.arange(0,3)
y=np.arange(0,4)
z=np.meshgrid(x,y)
print z[0]
print z[1]
m=z[0].ravel()  #   [0 1 2 0 1 2 0 1 2 0 1 2]
n=z[1].ravel()   #   [0 0 0 1 1 1 2 2 2 3 3 3]
z1=np.vstack((z[0].ravel(),z[1].ravel(),z[0].ravel(),z[1].ravel())).transpose()*16
print z1
```

打印出来的z1的值为： 

[[ 0  0  0  0] 

 [16  0 16  0] 

 [32  0 32  0] 

 [ 0 16  0 16] 

 [16 16 16 16] 

 [32 16 32 16] 

 [ 0 32  0 32] 

 [16 32 16 32] 

 [32 32 32 32] 

 [ 0 48  0 48] 

 [16 48 16 48] 

 [32 48 32 48]] 

 这是一个(height*width,4)的矩阵。在这里，height=4，width=3.这个正好是每个anchor应该偏移的量。 

 接下来，
```
K = shifts.shape[0]  #height*width
all_anchors = (self._anchors.reshape((1, A, 4)) +
               shifts.reshape((1, K, 4)).transpose((1, 0, 2)))   #形状为(K,A,4)
```

这样就能实现把最开始的anchors加上偏移量了。 

接着上面的例子。

经过上面的这个语句，all_anchors就是一个（height*width,A,4）的矩阵了。（其中，A=9） 

每个（A,4）的二维矩阵就表示feature map上那个点在原图对应的9个框。





