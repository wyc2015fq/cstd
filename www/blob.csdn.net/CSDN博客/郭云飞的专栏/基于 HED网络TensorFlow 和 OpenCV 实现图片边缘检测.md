
# 基于 HED网络TensorFlow 和 OpenCV 实现图片边缘检测 - 郭云飞的专栏 - CSDN博客


2017年09月30日 18:25:35[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：671


**传统的边缘检测：**
OpenCV 里面的两个函数，cv2.Canny() 和 cv2.findContours()：
![](https://img-blog.csdn.net/20170930182700627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
看上去很容易就能实现出来，但是真实情况是：复杂背景时根本不好使！
![](https://img-blog.csdn.net/20170930183212497?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**canny**算法的检测效果，依赖于几个阈值参数，这些阈值参数的选择，通常都是人为设置的经验值。在改进的过程中，引入额外的步骤后，通常又会引入一些新的阈值参数，同样，也是依赖于调试结果设置的经验值。整体来看，这些阈值参数的个数，不能特别的多，因为一旦太多了，就很难依赖经验值进行设置，另外，虽然有这些阈值参数，但是最终的参数只是一组或少数几组固定的组合，所以算法的鲁棒性又会打折扣，很容易遇到边缘检测效果不理想的场景。

**HED(Holistically-Nested Edge Detection) 网络：**
边缘检测这种需求，在图像处理领域里面，通常叫做 Edge Detection 或 Contour Detection，按照这个思路，找到了 Holistically-Nested Edge Detection 网络模型。HED 网络模型是在 VGG16 网络结构的基础上设计出来的，所以有必要先看看 VGG16。
![](https://img-blog.csdn.net/20170930183555005?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上图是 VGG16 的原理图，为了方便从 VGG16 过渡到 HED，我们先把 VGG16 变成下面这种示意图：
![](https://img-blog.csdn.net/20170930183719654?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
好，接下来抛出HED网络结构：
![](https://img-blog.csdn.net/20170930183945662?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
HED 网络是一种多尺度多融合(multi-scale and multi-level feature learning)的网络结构，所谓的多尺度，就是如上图所示，把 VGG16 的每一组的最后一个卷积层(绿色部分)的输出取出来，因为每一组得到的 image 的长宽尺寸是不一样的，所以这里还需要用转置卷积(transposed convolution)/反卷积(deconv)对每一组得到的 image 再做一遍运算，从效果上看，相当于把第二至五组得到的 image 的长宽尺寸分别扩大 2 至 16 倍，这样在每个尺度(VGG16
 的每一组就是一个尺度)上得到的 image，都是相同的大小了。
基于 TensorFlow 编写的 HED 网络Git地址：

```python
https://github.com/s9xie/hed
```
**HED分割出边缘后，用opencv优化：**虽然用神经网络技术，已经得到了一个比 canny 算法更好的边缘检测效果，但是，神经网络也并不是万能的，干扰是仍然存在的，所以，第二个步骤中的数学模型算法，仍然是需要的，只不过因为第一个步骤中的边缘检测有了大幅度改善，所以第二个步骤中的算法，得到了适当的简化，而且算法整体的适应性也更强了。
这部分的算法如下图所示：
![](https://img-blog.csdn.net/20170930185441516?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
按照编号顺序，几个关键步骤做了下面这些事情：

用 HED 网络检测边缘，可以看到，这里得到的边缘线还是存在一些干扰的
在前一步得到的图像上，使用 HoughLinesP 函数检测线段(蓝色线段)
把前一步得到的线段延长成直线(绿色直线)
在第二步中检测到的线段，有一些是很接近的，或者有些短线段是可以连接成一条更长的线段的，所以可以采用一些策略把它们合并到一起，这个时候，就要借助第三步中得到的直线。定义一种策略判断两条直线是否相等，当遇到相等的两条直线时，把这两条直线各自对应的线段再合并或连接成一条线段。这一步完成后，后面的步骤就只需要蓝色的线段而不需要绿色的直线了
根据第四步得到的线段，计算它们之间的交叉点，临近的交叉点也可以合并，同时，把每一个交叉点和产生这个交叉点的线段也要关联在一起(每一个蓝色的点，都有一组红色的线段和它关联)
对于第五步得到的所有交叉点，每次取出其中的 4 个，判断这 4 个点组成的四边形是否是一个合理的矩形(有透视变换效果的矩形)，除了常规的判断策略，比如角度、边长的比值之外，还有一个判断条件就是每条边是否可以和第五步中得到的对应的点的关联线段重合，如果不能重合，则这个四边形就不太可能是我们期望检测出来的矩形
经过第六步的过滤后，如果得到了多个四边形，可以再使用一个简单的过滤策略，比如排序找出周长或面积最大的矩形
















