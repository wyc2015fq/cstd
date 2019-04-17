# faster rcnn中损失函数（三）——理解faster-rcnn中计算rpn_loss_cls&rpn_loss_box的过程 - Snoopy_Dream - CSDN博客





2018年11月13日 12:01:13[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：1912








首先感想来源与pytorch的rpn.py。

我们都知道，rpn通过制作lable和targe_ shift来构造rpn loss的计算。那具体是怎么构造的呢?

首先rpn_loss_cls计算：

我们应该首先想到的是： rpn_loss_cls = F**.cross_entropy**(rpn_cls_score, rpn_label)

**维度分析：**cross_entropy要求输入是**Variable**预测的是2D，label是1D。

所以可以根据默认规定的初始的数据格式**b 2*9 h w进行推导。**

rpn_cls_score:**b 2*9 h w _> b*9*h*w，2   **#二分类

然后去除掉不感兴趣的区域：

rpn_cls_score： **(b*9*h*w - 标签-1的 ，2)**    #二分类

rpn_label：(b*9*h*w** - 标签-1****的**，)

```python
#return outputs [label ,target ,inside-weight ,outside_weight]
rpn_data = self.RPN_anchor_target((rpn_cls_score.data, gt_boxes, im_info, num_boxes))
rpn_cls_score = rpn_cls_score_reshape.permute(0, 2, 3, 1).contiguous().view(batch_size, -1, 2)# b 9h*w 2
rpn_label = rpn_data[0].view(batch_size, -1)#B 1 9*H W._>b,9*h*w
```

**数据本身分析：**

label包括:1 0 -1

首先需要做的是去除-1,即不感兴趣的目标

```python
#～@@！通过的ne去除掉-1，返回非0的索引
rpn_keep = Variable(rpn_label.view(-1).ne(-1).nonzero().view(-1))#nonzero返回b*9h*w行1列，所以需要view变成一维
rpn_cls_score = torch.index_select(rpn_cls_score.view(-1,2), 0, rpn_keep)#从rpn_cls_score（b*9h*w，2)从第0轴按照rpn_keep索引找
rpn_label = torch.index_select(rpn_label.view(-1), 0, rpn_keep.data)#rpn_data上文就是tensor，不是Variable
rpn_label = Variable(rpn_label.long())
```

**注意：**rpn_score是 **Variable **而rpn_label**刚开始是tensor，因为anchor_target_layer和prosal_layer.py不需要反向传播，了解他们的输入输出这一点很简单，他们本身就是生成rpn_lable 等，做的事情是制定选出的规则，并没有对选出的东西进行计算，所以无需反向传播，所以里面的forward的input都是Tensor，输入的时候都需要 **Variable**.data, 运算完后的输出再用Variable( Tensor.long())转换回来。**

分析：

1. rpn_label.view(-1).ne(-1).nonzero().view(-1)

ne(-1)返回 是-1就返回0，不是-1，返回1

nonzero返回不是0的索引，n行1列的(n,1)

综上，返回不是-1的所有索引，列成1维数组(n,)。

2.** torch.index_select**(rpn_cls_score.view(-1,2), **0,** rpn_keep)#从 **0维，按照**rpn_keep索引，找rpn_cls_score.view(-1,2)

**得到rpn_loss_cls**
`self.rpn_loss_cls = F.cross_entropy(rpn_cls_score, rpn_label)#  (b*9*h*w，2)   (b*9*h*w，) `




