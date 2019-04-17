# KLT光流法及其特征点检测方法 - tostq的专栏 - CSDN博客





2015年10月15日 20:28:27[tostq](https://me.csdn.net/tostq)阅读数：8376








KLT角点检测方法最初是用于满足Lucas-Kanade光流法选择合适特征点的需求，Lucas-Kanade光流法是通过先在前后两帧图像里分别建立一个固定大小窗口，然后找到让两个窗口间像素强度差的平方和最小的位移。然后将窗口内像素的移动近似为这样的位移向量，然后实际上，一方面像素移动并不会那么简单，另一方面窗口内像素并不都是同样的移动方式，因为这样的近似必然会带来误差。而现在的问题就是如何去选择合适的窗口，或者特征点，从而获得最为精确的跟踪。KLT角点检测方法就是为了选择一个适合跟踪的特征点，它认为一个好的特征点的定义应当就是能被好的跟踪。

KLT角点检测思路：

（1）像素点的光强度函数可以由其泰勒展开式表示，g指强度梯度，d指像素位移

![](https://img-blog.csdn.net/20151015202940710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）选择d使窗口内偏差能量最小，即使其导数为0

![](https://img-blog.csdn.net/20151015203021884?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20151015203107600?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20151015203132445?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


令

![](https://img-blog.csdn.net/20151015203236276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)     及 ![](https://img-blog.csdn.net/20151015203251895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

故![](https://img-blog.csdn.net/20151015203310641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

由上式，我们就可以估计出位移向量d，显然这个估计是建立在窗口内的像素的是平滑的，即其强度是一致的，而实际上这会在窗口内一些变化较大（如角点）的位置，造成其位移估计d偏差较大。然而可以通过迭代的方式，不断的应用新的d值，而图像块每次迭代都可以采用双线性插值（获得子像素精确度）得到新位置。

（3）下面最为重要的是这个角点的选择问题，这个角点是根据G矩阵的两个特征值来选择的，一方面两个特征值不能太小，排除噪声影响，另一方面两个特征值不能差别太大，说明这是角点。这里提出了下面的公式：

![](https://img-blog.csdn.net/20151015203346869?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里的阈值是为区分目标前景同背景的，在实际操作中，可以选择图像上大约一致亮度区域的特征值为阈值下限，而将角点或高纹理区域的特征值视为上限，一般情况下我们取上下限的一半左右。

下图左指图像各区域最小特征值的直方图，右指选择阈值为10情况下所选择区域。

![](https://img-blog.csdn.net/20151015203443706?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20151015203503361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（4）特征点的选择：首先将最小特征值降序排列，优先选择最小特征值大的。为了防止窗口重叠，如果发生选择的特征点在先前选择特征点的窗口内，就删除。通过上图，我们可以发现一个现象，在窗口内，如图中红色所框，角点附近，白色区域要远比黑色区域大。

这个现象虽然对于跟踪没有影响，但会影响对于兴趣点（角点）的定位，其主要因为是像素在明亮区域的变化要比在黑暗区域的变化要大。

（5）通过设置实际的能量偏差函数的阈值来排除，被阻挡的点，如下图所示，12、17等点其能量偏差函数太大，排除在特征点序列外。

![](https://img-blog.csdn.net/20151015203553094?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


总结：

KLT角点检测方法，实际上同Harris方法一样，都是计算梯度矩阵的特征值，都认为特征值都较大且接近的点是我们要寻找特征点，然而KLT不同的是，一个它计算的是窗口内所以点梯度矩阵的权值和，而Harris是一个像素点。二个就是它需要分别计算最小特征值大小，将比较其大小来选择特征点，而Harris并不需要具体计算特征值，而通过矩阵迹与行列式的比值来获得特征值比的。



