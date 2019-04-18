# 【超分辨率】Laplacian Pyramid Networks（LapSRN） - YZXnuaa的博客 - CSDN博客
2018年03月19日 16:53:26[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：333
期刊论文： 
[https://arxiv.org/abs/1710.01992](https://arxiv.org/abs/1710.01992)
会议论文CVPR2017： 
[http://vllab.ucmerced.edu/wlai24/LapSRN/papers/cvpr17_LapSRN.pdf](http://vllab.ucmerced.edu/wlai24/LapSRN/papers/cvpr17_LapSRN.pdf)
项目主页：[http://vllab.ucmerced.edu/wlai24/LapSRN/](http://vllab.ucmerced.edu/wlai24/LapSRN/)
看了眼作者，Jia-Bin Huang是传统算法领域（呃，自从深度学习出来后，我就将之前的算法都算传统方法了）的超分辨率学习的专家大牛。 
然而有意思的是，这篇论文结合了传统算法laplacian pyramid 和 CNN网络，这也给我们这些研究者一些思路，可以通过结合经典算法和深度学习作为insight。打个比方，最近KaiMing大神指导的一篇文章：《Non-local Neural Networks 》则将非局部算法融入到深度学习中。 
值得惋惜的是，好多人上来就直接研究深度学习，对自己领域的传统算法知之甚少。
LapSRN有两个版本（CVPR版本和期刊版本）。废话不多说，直接进入主题。下面是对**期刊论文的LapSRN**做简要理解。
## **Insight**
- 本文是通过将低分辨率图像直接作为输入到网络中，通过**逐级放大**，在减少计算量的同时，也有效的提高了精度
- 提出了一种鲁棒的loss function, **robust Charbonnier loss function**.
- 对各个金字塔的level之间和每个level之内，通过**recursive进行参数共享**
![这里写图片描述](https://img-blog.csdn.net/20171201211337485?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h3YW5fbWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## **Detail:**
**网络结构：**
**一、Network architecture**
**1. Feature extraction branch**
通过stack convolution来获取非线性特征映射 
**2. Image reconstruction branch**
在每一个pyramid level，最后加上deconv来提升图像的2x分辨率
**参数共享**
本文网络在两个地方进行参数共享，减少了参数量 
**1. 在各个pyramid level之间参数共享**， 称之为Recursive block
因为laplacian pyramid是在x2的基础上得到x4，由于各个level中的结构相似性，因此在各个level，参数得以共享 
形式如下： 
![这里写图片描述](https://img-blog.csdn.net/20171201214849336?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h3YW5fbWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**2. 每个pyramid level之中参数共享**
inspired DRCN and DRRN，作者在每个pyramid level中进行参数共享，如下图 
![这里写图片描述](https://img-blog.csdn.net/20171201214914989?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h3YW5fbWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**整个网络深度为：depth = (D x R + 1) x L + 2 也因此会出现了两个超参数 D，R**
**二、Local residual learning**
为了解决梯度弥散问题，作者在各个recursive block上采取了local residual learning。在本文中，作者探究了三种方法来stabilize训练过程： 
**1. No skip connection**
该种方法在之后的实验中称之为： LapSRN（NS） 
**2. Distinct-source skip connection**
该种方法在之后的实验中称之为： LapSRN（DS） 
**3. Shared-source skip connection**
该种方法在之后的实验中称之为： LapSRN（SS） 
![这里写图片描述](https://img-blog.csdn.net/20171202141549791?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h3YW5fbWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**实验证明LapSRN(ss)更work一些**
**三、Loss function**
本文认为相同的LR patch 可能有多种corresponding HR patches，而L2范数并不能capture the underlying multi-modal distributions of HR patches. 因此L2范数重建出的图像往往过平滑
本文提出了一种抗噪性强的loss functions： 
![这里写图片描述](https://img-blog.csdn.net/20171202142256941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h3YW5fbWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## **Experiments:**
**D,R关系， D为每个pyramid中的conv数，R为循环次数**
![这里写图片描述](https://img-blog.csdn.net/20171202143800975?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h3YW5fbWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**各个超分辨率算法参数细节：**
![这里写图片描述](https://img-blog.csdn.net/20171201215150917?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h3YW5fbWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**各个超分辨率算法实验结果：**
![这里写图片描述](https://img-blog.csdn.net/20171201215228121?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h3YW5fbWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
