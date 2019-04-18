# 深度学习基础--卷积--反卷积(deconvolution) - wydbyxr的博客 - CSDN博客
2018年11月15日 09:47:48[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：96
# 反卷积(deconvolution)
  这个概念很混乱，没有统一的定义，在不同的地方出现，意义却不一样。
  上采样的卷积层有很多名字：
> 
全卷积（full convolution），
网络内上采样（ in-network upsampling），
微步幅卷积（fractionally-strided convolution），
反向卷积（backwards convolution），
去卷积（deconvolution），
上卷积（upconvolution），
转置卷积（transposed convolution）。
  用「去卷积」这个术语是非常不推荐的，因为这是一个过载的术语：在数学运算或计算机视觉中的其他应用有着完全不同的含义。
## Deconvolution的应用
  1）unsupervised learning，其实就是covolutional sparse coding
  这里的deconv只是观念上和传统的conv反向，传统的conv是从图片生成feature map，而deconv是用unsupervised的方法找到一组kernel和feature map，让它们重建图片。
  2）CNN可视化
  通过deconv将CNN中conv得到的feature map还原到像素空间，以观察特定的feature map对哪些pattern的图片敏感，这里的deconv其实不是conv的可逆运算，只是conv的transpose，所以tensorflow里一般取名叫transpose_conv。
  3）upsampling
  在pixel-wise prediction比如image segmentation以及image generation中，由于需要做原始图片尺寸空间的预测，而卷积由于stride往往会降低图片size， 所以往往需要通过upsampling的方法来还原到原始图片尺寸，deconv就充当了一个upsampling的角色。
  例如：FCN网络（编码-解码模型）中的upsampling，GAN中的Generative图片生成。
## 具体的运算
  和卷积过程完全相反。通俗的说，先处理input（扩大），再做正常卷积。
  反卷积相当于卷积的输入与输出层进行交换，
  i,o表示卷积过程的输入输出大小，s,p,k分别表示卷积核的步进，pad的大小，卷积核的大小
  i’,o’表示反卷积过程的输入输出大小，s’,p’,k’分别表示卷积核的步进，pad的大小，卷积核的大小(反卷积过程的卷积核是卷积过程卷积核的转置）
  反卷积过程即为将s,p,k变成s’,p’,k’再进行卷积
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114102919452.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## 反卷积的具体过程
  1）将k，s，p的值转换为k’,s’,p’。
  K,s,p→K’=k,s’=1,p’=k-p-1(另外每个点之间pad s-1个0）
  2）利用转置后的卷积核进行卷积
  参考资料：[https://github.com/vdumoulin/conv_arithmetic](https://github.com/vdumoulin/conv_arithmetic)
### 例子
  对3* 3的图片进行反卷积，k=3，s=2，p=1
  相当于对3*3的图片进行k’=3，s’=1，p’=1（另外每个点之间pad s-1个0）的卷积，
  如下图所示，下面的蓝色是输入，上面的绿色是输出，在输入上移动的灰色的是卷积核。
  O’=s(i’-1)+k-2p
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114102904924.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
