# 深度学习AI美颜系列----AI美甲算法揭秘(WANNA NAILS)

2018年08月23日 13:30:10

 

Trent1985

 

阅读数：1150

更多

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/81979255

本文将给大家分享WANNA NAILS App中漂亮的美甲效果是如何实现的，对其背后的图像算法技术做一个详解。

WANNA NAILS是一款专门做美甲效果的app，效果举例如下：

![img](https://img-blog.csdn.net/20180823132105368?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

WANNA NAILS可以实现图片和视频的美甲效果，非常逼真，同时结合美甲产品进行商业变现。

本文针对美甲的效果来给大家进行图像算法分析与实现；

美甲的流程如下：

![img](https://img-blog.csdn.net/20180823132226163?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们按照流程来分析：

1.输入手指照片，这个只需要大家手指指甲露出来拍照即可；

2.指甲区域图像分割

这一步的实现得益于目前深度学习在图像分割中的精彩表现。按照以往传统图像分割算法的效果来看，是很难达到准确性的要求的，这里推荐使用DenseNet、PSPNet、UNet网络等来进行指甲区域的分割训练，至于SegNet/FCN等个人感觉精确度不够，大家可以尝试一下，数据集的准备形式如下所示：

![img](https://img-blog.csdn.net/20180823132318611?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

数据集在10000张以上，输入原图，输出指甲区域Mask，这里本人以UNet为例，UNet的网络结构如下所示：

![img](https://img-blog.csdn.net/20180823132415195?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这是一个全卷积神经网络，输入和输出都是图像，没有全连接层，较浅的高分辨率层用来解决像素定位的问题，较深的层用来解决像素分类的问题；左边进行卷积和下采样，同时保留当前结果，右边进行上采样时将上采样结果和左边对应结果进行融合，以此来提高分割效果；这个网络中左右是不对称的，后来改进的Unet基本上在图像分辨率上呈现出对称的样式，这一步的精确度是关键，它将决定最后美甲的效果。

  对于图像分割的代码及详细说明，可以参考本人博客：[UNet图像分割](https://blog.csdn.net/Trent1985/article/details/80661230)

3.连通域判断

这一步我们需要对指甲分割区域的Mask来做一个连通域判断，得到每个指甲的联通区域。

美甲的效果是多种多样的，有单色的也有多色的，对于单色，我们不需要进行连通域计算，直接跳过此步，对Mask中的指甲区域进行美甲上色即可；对于多色的，我们需要按照不同的连通域来分别上色，因此，此处要进行连通域计算，得到每个指甲连通域。

连通域计算方法有很多，比如回溯法，Two-Pass法等等，这里本人以Two-Pass 4邻域为例：

TwoPass 算法对二值图像进行两次扫描：

①当前像素点若是前景（非零），判断其已扫描过的邻接节点（上邻像素，左邻像素）有无已标记的。若仅有一个含有标记值，则将该标记赋予该位置，若两个都含有标记值，将最小标记赋予当前值，并将这两个标记值合并（Union），归为同一类；若无，新增标记赋予当前值，即label++。

②对上述含有标记的像素，查找其集合的根节点(Find)，用根节点对当前值赋值。

如下图所示：

![img](https://img-blog.csdn.net/20180823132640254?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

代码如下(代码来自“**风吹夏天**”)：

```cpp
#define MAXLABEL 500
uchar parent[MAXLABEL] = {0};
 
int Find(uchar x, uchar parent[]) 
{
    int i = x;
    while (0 != parent[i])
        i = parent[i];
    return i;
}
 
void Union(uchar big, uchar small, uchar parent[]) 
{
    uchar i = big;
    uchar j = small;
    while (0 != parent[i])
        i = parent[i];
    while (0 != parent[j])
        j = parent[j];
    if (i != j)
        parent[i] = j;
}
 
Mat Label(Mat &I)
{
    /// first pass
    int label = 0;
 
    Mat dst = Mat::zeros(I.size(), I.type());
    for (int nY = 0; nY < I.rows; nY++)
    {
        for (int nX = 0; nX < I.cols;nX++)
        {
            if (I.at<uchar>(nY,nX) != 0)
            {
                uchar left = nX - 1<0?0:dst.at<uchar>(nY, nX - 1);
                uchar up = nY - 1<0?0:dst.at<uchar>(nY - 1, nX);
 
                if (left != 0 || up != 0)
                {
                    if (left != 0 && up != 0)
                    {
                        dst.at<uchar>(nY, nX) = min(left, up);
                        if(left < up)
                            Union(up,left,parent);
                        else if(up<left)
                            Union(left, up, parent);
                    }
                    else
                        dst.at<uchar>(nY, nX) = max(left, up);
                }
                else
                {
                    dst.at<uchar>(nY, nX) = ++label;
 
                }
            }
        }
    }
 
    /// second pass 
    for (int nY = 0; nY < I.rows; nY++)
    {
        for (int nX = 0; nX < I.cols; nX++)
        {
            if (I.at<uchar>(nY, nX) == 1)
                dst.at<uchar>(nY, nX) = Find(dst.at<uchar>(nY, nX), parent);
        }
    }
    return dst;
}
```

我们获取指甲连通域如下：

![img](https://img-blog.csdn.net/20180823132807128?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4.单色/多色选择

这一步主要是进行单一颜色美甲和多色美甲选择，一般而言，可以让用户对每个指甲连通域进行颜色选择，这样就可以得到用户想要的美甲颜色了。

也就是说此处主要是记录不同指甲区域对应不同颜色的列表，为美甲做准备；

5.美甲

美甲这一步主要实现指甲上色，上色的方式有多种多样，可以使用HSL颜色空间进行颜色替换，不过这里本人使用更简单的一种方法：

①假设用户选取颜色为D(r,g,b)，指甲区域为S；

②对指甲区域做去色处理，得到图像M；

③对M进行亮度对比度调整，提高亮度，增强对比度，得到图N；

②以S为底图，以N为混合图，进行一次正片叠底图层混合，得到效果图A；

Photoshop的正片叠底算法如下：

A = S * N / 255

③将A与S按照alpha(alpha为经验值，这里推荐80%)进行Blend，即可达到美甲的效果B；

B=A*alpha + S*(1.0-alpha)

这样我们得到的效果图如下：

![img](https://img-blog.csdn.net/2018082313291456?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

至此，美甲算法讲解完了，对于实时处理，只要算法优化的足够快，完全没有问题。

本文主要揭秘了WANNA NAILS美甲的图像算法实现，当然本人所有算法逻辑仅仅是研究与尝试的结果，并非WANNA NAILS App原版算法，仅供大家参考！