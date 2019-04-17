# faster rcnn中损失函数（二）—— Smoooh L1 Loss的讲解 - Snoopy_Dream - CSDN博客





2018年11月08日 11:18:40[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：237










1. 使用Smoooh L1 Loss的原因

对于边框的预测是一个回归问题。通常可以选择**平方损失函数（L2损失）f(x)=x^2。**但这个**损失对于比较大的误差的惩罚很高**。

我们可以采用稍微缓和一点**绝对损失函数（L1损失）f(x)=|x|**，它是随着误差线性增长，而不是平方增长。但**这个函数在0点处导数不存在**，因此可能会影响收敛。

一个通常的解决办法是，**分段函数**，在**0点附近使用平方函数使得它更加平滑**。它被称之为**平滑L1损失函数**。它通过一个参数**sigma**来控制平滑的区域：比如**sigma=3 下面的 if |x|<1/3**2的时候，更接近于0的地方。**

![](https://img-blog.csdnimg.cn/20181108101316779.png)

![](https://img-blog.csdnimg.cn/20181108112239449.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



2.

SmoothL1LossLayer 计算一张图片的损失函数，对应于下图的加号右边部分

![](https://img-blog.csdnimg.cn/20181108105638545.png)

            i是mini-batch的anchor的索引。

![](https://img-blog.csdnimg.cn/20181108112151651.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

有物体时pi*为1，否则为0

所以上图中，回归损失中，Pi*Leg(ti, ti*)表明只有fg才有回归损失。

![](https://img-blog.csdnimg.cn/20181108111111993.png)

**ti是一个向量，预测坐标 = (tx,ty,tw,th)**

ti*是一个向量，是gt包围盒的坐标

![](https://img-blog.csdnimg.cn/20181108105644167.png)

R表示的下一个图那样的分段函数。

**bottom[0]**预测坐标，对应于下图的**ti，即rpn_bbox_pred层返回的预测的与初始anchor的偏移。**

**bottom[1]**target坐标，对应于下图的**ti*，即anchor_target_layer类型的rpn_data层返回的，rpn_bbox_target，gt与初始每个anchor的偏移。**

**bottom[2]inside，有物体(fg)时为1，否则为0，对应于上图的pi***

bottom[3]**outside**，没有前景（fg）也没有后景（bg）的为0，其他为**1/（bg+fg）=Ncls**，对应于加号右边的系数部分（但其实这个地方我本人还是不懂，因为论文上说的系数都是一些固定的值，如入=10。初始代码一直在更新，估计又换了别的方法。不论如何，在现在的代码中outside是乘以了后面的结果）



Lreg的公式就是下图，另 **x=ti - ti*，**

**其中|x|<1 /sigma*sigma :  (x*x)/(sigma*sigma/2)**

**                                  other:   |x|-0.5/(sigma**2)**

![](https://img-blog.csdnimg.cn/20181108105627603.png)

![](https://img-blog.csdnimg.cn/20181127174430136.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

我们可以从下面代码中学习到**分段函数，如何用矩阵来进行运算,以及loss如何相加（轴）：**

摘自：pytorch的net_utils.py中

```python
def _smooth_l1_loss(bbox_pred, bbox_targets, bbox_inside_weights, bbox_outside_weights, sigma=1.0, dim=[1]):
    
    sigma_2 = sigma ** 2
    box_diff = bbox_pred - bbox_targets### B,9*4,H,W      B,9*4,H,W都是variable
    in_box_diff = bbox_inside_weights * box_diff# 内部正样本的diff
    abs_in_box_diff = torch.abs(in_box_diff)
    #detach返回的Variable和原始的Variable公用同一个data tensor。
     #返回一个新的Variable，从当前图中分离下来的。requires_grad=False，
    smoothL1_sign = (abs_in_box_diff < 1. / sigma_2).detach().float()
    
    in_loss_box = torch.pow(in_box_diff, 2) * (sigma_2 / 2.) * smoothL1_sign \
                  + (abs_in_box_diff - (0.5 / sigma_2)) * (1. - smoothL1_sign)
    out_loss_box = bbox_outside_weights * in_loss_box#bbox_outside_weights 正负样本都是1/256 背景为0
    #注意学习loss的相加，按照轴
    loss_box = out_loss_box
    for i in sorted(dim, reverse=True):
      loss_box = loss_box.sum(i)
    loss_box = loss_box.mean()
    return loss_box
```

## `detach`能用来干啥

如果我们有两个网络 A,B

, 两个关系是这样的 y=A(x),z=B(y) 现在我们想用 z.backward() 来为 B 网络的参数来求梯度，但是又不想求 A

网络参数的梯度。我们可以这样：

```
# y=A(x), z=B(y) 求B中参数的梯度，不求A中参数的梯度
# 第一种方法
y = A(x)
z = B(y.detach())
z.backward()

# 第二种方法
y = A(x)
y.detach_()
z = B(y)
z.backward()
```

在这种情况下，`detach 和 detach_` 都可以用。但是如果 你也想用 y

来对 A 进行 BP 呢？那就只能用第一种方法了。因为 第二种方法 已经将 A 模型的输出 给 detach（分离）了。





