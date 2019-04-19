# MTCNN（Multi-task convolutional neural networks）人脸对齐 - 数据之美的博客 - CSDN博客
2017年03月24日 19:56:54[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：4506
该MTCNN算法出自深圳先进技术研究院，乔宇老师组，是今年2016的ECCV。（至少我知道的今年已经一篇cvpr，一篇eccv了）。
进入正题
理论基础：
![](https://img-blog.csdn.net/20160927151542441)
正如上图所示，该MTCNN由3个网络结构组成（P-Net,R-Net,O-Net）。
Proposal Network (P-Net)：该网络结构主要获得了人脸区域的候选窗口和边界框的回归向量。并用该边界框做回归，对候选窗口进行校准，然后通过非极大值抑制（NMS）来合并高度重叠的候选框。
Refine Network (R-Net)：该网络结构还是通过边界框回归和NMS来去掉那些false-positive区域。
只是由于该网络结构和P-Net网络结构有差异，多了一个全连接层，所以会取得更好的抑制false-positive的作用。
Output Network (O-Net)：该层比R-Net层又多了一层卷基层，所以处理的结果会更加精细。作用和R-Net层作用一样。但是该层对人脸区域进行了更多的监督，同时还会输出5个地标（landmark）。
详细的网络结构如下图所示：
![](https://img-blog.csdn.net/20160927151644359)
prototxt的更加详细的网络结构如下：分别为det1,det2,det3。
det1.prototxt结构：
![](https://img-blog.csdn.net/20160927154450043)
det2.prototxt结构：
![](https://img-blog.csdn.net/20160927154512715)
det3.prototxt结构：
![](https://img-blog.csdn.net/20160927154529184)
训练：
MTCNN特征描述子主要包含3个部分，人脸/非人脸分类器，边界框回归，地标定位。
人脸分类：
![](https://img-blog.csdn.net/20160927151710209)
上式为人脸分类的交叉熵损失函数，其中，pi为是人脸的概率，yidet为背景的真实标签。
边界框回归：
![](https://img-blog.csdn.net/20160927151729296)
上式为通过欧氏距离计算的回归损失。其中，带尖的y为通过网络预测得到，不带尖的y为实际的真实的背景坐标。其中，y为一个（左上角x，左上角y，长，宽）组成的四元组。
地标定位：
![](https://img-blog.csdn.net/20160927151747812)
和边界回归一样，还是计算网络预测的地标位置和实际真实地标的欧式距离，并最小化该距离。其中，，带尖的y为通过网络预测得到，不带尖的y为实际的真实的地标坐标。由于一共5个点，每个点2个坐标，所以，y属于十元组。
多个输入源的训练：
![](https://img-blog.csdn.net/20160927151808203)
整个的训练学习过程就是最小化上面的这个函数，其中，N为训练样本数量，aj表示任务的重要性，bj为样本标签，Lj为上面的损失函数。
在训练过程中，为了取得更好的效果，作者每次只后向传播前70%样本的梯度，这样来保证传递的都是有效的数字。有点类似latent SVM，只是作者在实现上更加体现了深度学习的端到端。
在训练过程中，y尖和y的交并集IoU（Intersection-over-Union）比例：
0-0.3：非人脸
0.65-1.00：人脸
0.4-0.65：Part人脸
0.3-0.4：地标
训练样本的比例，负样本:正样本:part样本:地标=3:1:1:2
安装步骤：
caffe-windows的安装：
[http://blog.csdn.net/qq_14845119/article/details/52415090](http://blog.csdn.net/qq_14845119/article/details/52415090)
Pdollartoolbox的安装：
Pdollartoolbox由UCSD的Piotr
 Dollar编写，侧重物体识别（Object
 Recognition）检测相关的特征提取和分类算法。这个工具箱属于专而精的类型，主要就是Dollar的几篇物体检测的论文的相关算法，如果做物体识别相关的研究，应该是很好用的。同时它的图像操作或矩阵操作函数也可以作为Matlab图像处理工具箱的补充，功能主要包括几个模块：
* channels模块，图像特征提取，包括HOG等，Dollar的研究工作提出了一种Channel
 Feature的特征[2]，因此这个channels主要包括了提取这一特征需要的一些基本算法梯度、卷及等基本算法
* classify模块，一些快速的分类相关算法，包括random ferns, RBF functions, PCA等
* detector模块，与Channel Feature特征对应的检测算法1 
* filters模块，一些常规的图像滤波器
* images模块，一些常规的图像、视频操作，有一些很实用的函数
* matlab模块，一些常规的Matlab函数，包括矩阵计算、显示、变量操作等，很实用
* videos模块，一些常规的视频操作函数等
下载链接：[https://github.com/pdollar/toolbox](https://github.com/pdollar/toolbox)
下载到Toolbox后，将其解压到任意目录下，如E:\MATLAB\MATLAB Production Server\toolbox
在Matlab命令行中输入
addpath(genpath(‘toolbox-masterROOT’));savepath;
将解压目录加入Matlab路径。其中toolbox-masterROOT为解压目录路径，如解压到E:\
 MATLAB\MATLAB Production Server\toolbox时，则命令为
addpath(genpath(‘E:\ MATLAB\MATLAB ProductionServer\toolbox’)); savepath;
这样Piotr’s Image & VideoMatlab Toolbox就安装好了。
path里面加入caffe的库目录，例如本人的path加入如下的路径
![](https://img-blog.csdn.net/20160927151915930)
打开demo.m，修改其中的caffe_path，pdollar_toolbox_path，caffe_model_path
。同时由于本人电脑没有GPU，因此对其做如下修改。
![](https://img-blog.csdn.net/20160927151938032)
实验结果：
运行时间1.2S，按照检测出18个脸算，平均一个66MS，运行版本为release版本。从实验结果来看，不管是检测还是对齐都是空前的好。以我的经验来看，face++的对齐是最好的，剩下的开源的里面这篇MTCNN算是最好的了，然后才是SDM。
![](https://img-blog.csdn.net/20160927152000594)
从下面的作者在FDDB+WIDERFACE+AFLW上验证的正确性来看，基本95%的准确度。可见该MTCNN的性能和效率都是很给力的。
![](https://img-blog.csdn.net/20160927152031407)
从实验结果可以看出，上图的第二行的第二个对齐的出了问题，因此，本人对其程序进行了微小改动，实际运行效果如下图，时间和效果上都有了提升。
![](https://img-blog.csdn.net/20161013193753550)
![](https://img-blog.csdn.net/20161013193200182)
下载链接：[http://download.csdn.net/detail/qq_14845119/9653138](http://download.csdn.net/detail/qq_14845119/9653138)
组后感谢公司大牛的帮忙，c语言版本终于改出来了。老实说，真心不容易，走了好多坑。贴个效果图，纪念那些苦了笑了的时光。
![](https://img-blog.csdn.net/20161016182651828)
References:
[1] [https://kpzhang93.github.io/MTCNN_face_detection_alignment/index.html](https://kpzhang93.github.io/MTCNN_face_detection_alignment/index.html)
[2] [https://github.com/kpzhang93/MTCNN_face_detection_alignment/tree/master/code/codes](https://github.com/kpzhang93/MTCNN_face_detection_alignment/tree/master/code/codes)
[3] ZhangK, Zhang Z, Li Z, et al. Joint Face Detection and Alignment using Multi-taskCascaded Convolutional Networks[J]. arXiv preprint arXiv:1604.02878,
 2016.
