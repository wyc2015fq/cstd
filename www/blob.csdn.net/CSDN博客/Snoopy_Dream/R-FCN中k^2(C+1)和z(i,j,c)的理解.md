# R-FCN中k^2(C+1)和z(i,j,c)的理解 - Snoopy_Dream - CSDN博客





2018年05月18日 20:51:23[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：99标签：[R-FCN																[k^2(C+1)](https://so.csdn.net/so/search/s.do?q=k^2(C+1)&t=blog)](https://so.csdn.net/so/search/s.do?q=R-FCN&t=blog)
个人分类：[RFCN](https://blog.csdn.net/e01528/article/category/7674855)








 关于R-FCN论文的讲解，网上有很多资料，在这里重缀了。这里只对其中的核心图做讲解。

    假设c+1类中有一类是人脸，下图中k=3，对应的可以找到**人脸的左上部**对应的特征向量的。相当于把c+1类又细分了，每一类里面有**k*k个**位置细分类。





![](https://img-blog.csdn.net/20180518204223197?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
 解释上图，其中**feature maps**对应 下面网络图 中 **con_new_1[1024*w*h]**（res5c是2048*w*h，这一层**1024***1*1的全卷积降维操作）

**    position-sensitive score maps**（以下简称ps层），为什么是**k^2(c+1)**维度呢?一般不都是c+1维吗?k^2的加入才是position-sensitive的关键，因为一般的conv+pool具体平移不变性，对于检测任务位置的定位不敏感，所以需要改变。

    每个feature map，**z(i,j,c)**是第i+k(j-1)个立体块上的第c个map（1<= i,j <=3）。(i,j)决定了9种位置的某一种位置，假设为左上角位置（i=j=1），c决定了哪一类，假设为person类。在z(i,j,c)这个feature map上的某一个像素的位置是（x,y），像素值是value，则value表示的是原图对应的(x,y)这个位置上可能是人（c=‘person’）且是人的左上部位（i=j=1）的概率值。


    这里**k^2**共有k*k =** 9个颜色（成为9个bin）**（示意图中k=3，实际中默认k=7），每个颜色的立体块（W*H*(C+1)）表示的是不同位置存在目标的概率值（第一块黄色表示的是左上角位置，最后一块淡蓝色表示的是右下角位置）。共有k^2*(C+1)个feature map。每个feature map，**z(i,j,c)**是第i共有k*k = 9个颜色，每个颜色的立体块（W*H*(C+1)）表示的是**不同位置存在目标的概率值**（第一块黄色表示的是左上角位置，最后一块淡蓝色表示的是右下角位置）。共有k^2*(C+1)个feature map。

![](https://img-blog.csdn.net/20180518165935539?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







