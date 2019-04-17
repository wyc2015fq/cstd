# 语义分割(semantic segmentation)--DeepLabV3之ASPP(Atrous Spatial Pyramid Pooling)代码详解 - 别说话写代码的博客 - CSDN博客





2019年02月12日 12:51:34[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：865标签：[语义分割																[semantic segmentation																[DeepLabV3																[ASPP](https://so.csdn.net/so/search/s.do?q=ASPP&t=blog)
个人分类：[语义分割Semantic Segmentation](https://blog.csdn.net/qq_21997625/article/category/8661707)





ASPP： Atrous Spatial Pyramid Pooling

![](https://img-blog.csdnimg.cn/20190212123514332.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

deeplabv2中的aspp如上图所示，在特征顶部映射图使用了四中不同采样率的空洞卷积。这表明以不同尺度采样时有效的，在Deeolabv3中向ASPP中添加了BN层。不同采样率的空洞卷积可以有效捕获多尺度信息，但会发现随着采样率的增加，滤波器有效权重（权重有效的应用在特征区域，而不是填充0）逐渐变小。如下图

![](https://img-blog.csdnimg.cn/20190212123514328.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

当我们以不同采样率的3*3卷积核应用在65*65的特征映射上，当采样率接近特征映射大小时，3*3滤波器不是捕获全图像上下文，而是退化为简单的1*1滤波器，只有滤波器中心起作用。

为了克服这个问题，我们考虑使用图片级特征。具体来说，我们在模型最后的特征映射上应用全局平均，将结果经过1x1的卷积，再双线性上采样得到所需的空间维度。最终我们改进的ASPP包括：
- 
1.一个1x1卷积和三个3x3的采样率为rates={6,12,18}的空洞卷积，滤波器数量为256，包含BN层。针对output_stride=16的情况。如下图(a)部分Atrous Spatial Pyramid Pooling

- 
2.图像级特征，即将特征做全局平均池化，经过卷积再融合。如下图(b)部分Image Pooling


![](https://img-blog.csdnimg.cn/20190212123514353.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

也就是说当output_stride=8时，加倍了采样率。所有特征通过1x1级联到一起，生成最终的分数。代码(pytorch)为：

```python
class ASPP(nn.Module):
    def __init__(self, in_channel=512, depth=256):
        super(ASPP,self).__init__()
        # global average pooling : init nn.AdaptiveAvgPool2d ;also forward torch.mean(,,keep_dim=True)
        self.mean = nn.AdaptiveAvgPool2d((1, 1))
        self.conv = nn.Conv2d(in_channel, depth, 1, 1)
        # k=1 s=1 no pad
        self.atrous_block1 = nn.Conv2d(in_channel, depth, 1, 1)
        self.atrous_block6 = nn.Conv2d(in_channel, depth, 3, 1, padding=6, dilation=6)
        self.atrous_block12 = nn.Conv2d(in_channel, depth, 3, 1, padding=12, dilation=12)
        self.atrous_block18 = nn.Conv2d(in_channel, depth, 3, 1, padding=18, dilation=18)

        self.conv_1x1_output = nn.Conv2d(depth * 5, depth, 1, 1)

    def forward(self, x):
        size = x.shape[2:]

        image_features = self.mean(x)
        image_features = self.conv(image_features)
        image_features = F.upsample(image_features, size=size, mode='bilinear')

        atrous_block1 = self.atrous_block1(x)

        atrous_block6 = self.atrous_block6(x)

        atrous_block12 = self.atrous_block12(x)

        atrous_block18 = self.atrous_block18(x)

        net = self.conv_1x1_output(torch.cat([image_features, atrous_block1, atrous_block6,
                                              atrous_block12, atrous_block18], dim=1))
        return net
```

本文内容参考：

[https://blog.csdn.net/u011974639/article/details/79144773](https://blog.csdn.net/u011974639/article/details/79144773)](https://so.csdn.net/so/search/s.do?q=DeepLabV3&t=blog)](https://so.csdn.net/so/search/s.do?q=semantic segmentation&t=blog)](https://so.csdn.net/so/search/s.do?q=语义分割&t=blog)




