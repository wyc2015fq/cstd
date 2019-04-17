# LOCOCO角点检测方法（一种快速精确的Harris及KLT）介绍 - tostq的专栏 - CSDN博客





2015年10月19日 11:06:12[tostq](https://me.csdn.net/tostq)阅读数：2803








虽然Harris及KLT（Kanade-Lucas-Tomasi）等方法被广泛等应用于角特征检测领域，但是由于这些算子计算复杂，很难达到实时运行，所以其应用也受到了限制。由此一种新的实时快速算法LOCOCO角点检测方法（2011年）被提出，它重新设计了Harris及KLT算法来减少其计算复杂度，并提出了一种子像素精确度的角点位置确定。

Harris算法及KLT的方法非常类似，都是属于对图像像素强度梯度进行操作。其方法都是首先通过卷积一阶高斯梯度核来获得图像梯度，然后计算每个像素梯度矩阵及角响应（这里Harris同KLT唯一不同之外，即Harris是计算梯度矩阵特征值比，而KLT是确定梯度矩阵最小特征值，不过其目的都是为了获得具有两个较大特征值的点）最后，通过快速排序获得每个像素点角响应排序，从中依次挑选最大值并在序列中排除其邻域内其它候选点（即NMS）。

而LOCOCO方法将在其每一步骤里都减少计算复杂度，即在计算高斯梯度、获得角度响应及非极大值抑制（NMS）方面上进行改进。其主要利用高斯梯度及角响应的计算复杂度可以通过积分图像来减少，而在NMS上，则通过图像块来代替一个个像素点来进行排序。



**一、Harris及KLT方法**

为了方便理解，这里先补充下Harris及KLT方法关于其角响应程度计算方法的不同之处（如果需要详细了解其方法内容，可以参考我的博客）：

![](https://img-blog.csdn.net/20151019110815032?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


Harris：

![](https://img-blog.csdn.net/20151019110839810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20151019110857409?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


KLT：

![](https://img-blog.csdn.net/20151019110912824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**二、LOCOCO角点检测方法**

因为Harris及KLT等角点检测方法的类似性，而LOCOCO方法实际上是对这两个方法中相同步骤的一种改进，所以它可以针对这两种方法被分成两部分：SLC-Harris（子像素精度低复杂度的Harris），SLC-KLT（子像素精度低复杂度的KLT）。

其改进原理：

（1）一阶高斯梯度模板可以由一个盒形核所近似（其可以通过结合积分图像被快速计算）。

（2）角响应程度的计算会造成旁瓣窗口效应（这是由于窗口会在相邻像素间重叠），这里我们也可以利用积分图像来减少。

（3）快速排序用于在NMS前分析响应程度，然后这个顺序并没有那么重要，因为NMS总是能找到局部最大值，而无论我们从那个像素开始。另外，NMS窗口的比较结果可以被相邻像素共享，因此计算复杂可以通过有效的NMS来减少，一旦最大值点被提取出来，它们就可以根据其角响应程度进行排序了。

**A）积分图像的概念**

对于一幅灰度的图像，积分图像中的任意一点(x,y)的值是指从图像的左上角到这个点的所构成的矩形区域内所有的点的灰度值之和

![](https://img-blog.csdn.net/20151019111116394?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


一个递归的快速算法：

![](https://img-blog.csdn.net/20151019111131326?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


一个矩形窗口内的像素和可以被这样计算：

![](https://img-blog.csdn.net/20151019111151551?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**B）通过盒函数及积分图像计算图像梯度**

盒函数一阶偏导模板（b）同高斯函数一阶偏导模板（a）

![](https://img-blog.csdn.net/20151019111308793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


通过卷积一阶偏导核函数获得图像像素点在x及y方向的梯度，而高斯核需要被离散化，其需要一定长度的FIR滤波模板来近似，则其窗口大小至少为4Ϭ，我们可以通过迭代滤波器的方法，将高斯梯度滤波器近似为IIR滤波器（长度减少），而LOCOCO的方法计算量更少。

其思路是这样的，通过积分图像，一个区域的像素和只需要由积分图像区域四个角点的值将可以确定（前面我们可以看到），通过SURF里得到的启示，我们可以通过盒函数核来近似代替高斯核，而从盒函数模板中，我们可以看到中间区域的系数是一致的，这样可以大大减少计算量。下图是复杂度比较，这里的N是指高斯滤波器的长度，

![](https://img-blog.csdn.net/20151019111329058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**C）角响应度**

角响应度的计算（上面我们提到的Harris及KLT的角响应度的计算）是算法里计算度最大的部分，其主要是通过每点的窗口内所有像素的梯度平方和乘积的带权值和，最后得到了梯度矩阵。如果当窗口权值为1的话，因为每个点都需要计算，那么窗口内的和的计算肯定会出现重叠，而如果在积分图像的情况下，直接计算积分图像的各个梯度，将会让计算量大大减少：

![](https://img-blog.csdn.net/20151019111354606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


显然，要计算某个点的窗口内梯度矩阵，只需要通过其窗口四个点的简单加减运算就可以获得了。有个梯度矩阵G，之后获得角响应度就可以根据其计算方法，选择是SLC-Harris，还是SLC-KLT方法了。

**D）NMS（非极大值抑制）**

NMS主要分为这么几步：第一，通过快速排序将全图所有角响应点以降序排成一列，然后，顺次挑选最大值，并把最大值点窗口内的非最值点从候选列中删除。最后，特征点之间都会有一个最小距离。如果单纯是这样实施的话，我们需要先排序每个点角响应值，然后在每点的窗口![](https://img-blog.csdn.net/20151019111542878?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)内所有点进行比较，这个计算量非常复杂。

而对于LOCOCO算法来说，它可以减少这个计算量，它基于这样的概念，一旦一个最大值被找到，就表明我们可以将这个点所有距离在d以内的邻域点排除在外。

E-NMS通过图像块（而不是一个个的像素点）来实施NMS。其先将图像分成![](https://img-blog.csdn.net/20151019111603786?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)大小图像块，然后将每个块内的最大值找到，最后，对于每个块内的最大值，对比较其邻域点（如下图点框内所示），这里面的黑点是图像分块后的最大值，最后我们对这些局部最大值点进行排序比较（如图下a，b，c三点所示）

![](https://img-blog.csdn.net/20151019111619667?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


它们的复杂度比较：

![](https://img-blog.csdn.net/20151019111638221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里的图像大小为b*h，而经过分块后的局部特征值数为F

**E）子像素精度的特征点位置确定：**

主要是在角响应度R上考虑，通过R在x,y方向梯度的变化，来估计其在子像素位置的精确度，这个方法sift方法也用到过，不过其是DOG的响应度，而不是角响应度。

![](https://img-blog.csdn.net/20151019111731136?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20151019111749206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**参考文献**

Robust Low Complexity Corner Detector  【我上传的资源里面有】






