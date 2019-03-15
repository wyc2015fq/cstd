# 照片美妆---基于Haar特征的Adaboost级联人脸检测分类器

2017年05月11日 11:27:12

 

Trent1985

 

阅读数：1031

 

标签： [人脸检测](http://so.csdn.net/so/search/s.do?q=%E4%BA%BA%E8%84%B8%E6%A3%80%E6%B5%8B&t=blog) 更多

个人分类： [照片美妆](https://blog.csdn.net/Trent1985/article/category/6887373)



本文转载自张雨石http://blog.csdn.net/stdcoutzyx/article/details/34842233



基于Haar特征的Adaboost级联人脸检测分类器

基于Haar特征的Adaboost级联人脸检测分类器，简称haar分类器。通过这个[算法](http://lib.csdn.net/base/datastructure)的名字，我们可以看到这个算法其实包含了几个关键点：Haar特征、Adaboost、级联。理解了这三个词对该算法基本就掌握了。

# 1        算法要点

Haar分类器 = Haar-like特征 + 积分图方法 + AdaBoost +级联；

Haar分类器算法的要点如下：

a)        使用Haar-like特征做检测。

b)       使用积分图（IntegralImage）对Haar-like特征求值进行加速。

c)        使用AdaBoost算法训练区分人脸和非人脸的强分类器。

d)       使用筛选式级联把分类器级联到一起，提高准确率。

# 2        历史

在2001年，Viola和Jones两位大牛发表了经典的《Rapid Object Detectionusing a Boosted Cascade of Simple Features》和《Robust Real-Time Face Detection》，在AdaBoost算法的基础上，使用Haar-like小波特征和积分图方法进行人脸检测，他俩不是最早使用提出小波特征的，但是他们设计了针对人脸检测更有效的特征，并对AdaBoost训练出的强分类器进行级联。这可以说是人脸检测史上里程碑式的一笔了，也因此当时提出的这个算法被称为Viola-Jones检测器。又过了一段时间，RainerLienhart和Jochen Maydt两位大牛将这个检测器进行了扩展，最终形成了[OpenCV](http://lib.csdn.net/base/opencv)现在的Haar分类器。

AdaBoost是Freund和Schapire在1995年提出的算法，是对传统Boosting算法的一大提升。Boosting算法的核心思想，是将弱学习方法提升成强学习算法，也就是“三个臭皮匠顶一个诸葛亮”

# 3        Haar特征

什么是特征，特征就是分类器的输入。把它放在下面的情景中来描述，假设在人脸检测时我们需要有这么一个子窗口在待检测的图片窗口中不断的移位滑动，子窗口每到一个位置，就会计算出该区域的特征，然后用我们训练好的级联分类器对该特征进行筛选，一旦该特征通过了所有强分类器的筛选，则判定该区域为人脸。

那么这个特征如何表示呢？好了，这就是大牛们干的好事了。后人称这他们搞出来的这些东西叫Haar-Like特征。

Viola大牛在[1]中提出的haar特征如下：

![img](https://img-blog.csdn.net/20140626141448140)



Rainer大牛改进了这些特征，提出了更多的haar特征。如下图所示：

![img](https://img-blog.csdn.net/20140626141451687)



这些所谓的特征不就是一堆堆带条纹的矩形么，到底是干什么用的？我这样给出解释，将上面的任意一个矩形放到人脸区域上，然后，将白色区域的像素和减去黑色区域的像素和，得到的值我们暂且称之为人脸特征值，如果你把这个矩形放到一个非人脸区域，那么计算出的特征值应该和人脸特征值是不一样的，而且越不一样越好，所以这些方块的目的就是把人脸特征量化，以区分人脸和非人脸。

# 4        Adaboost算法

本节旨在介绍AdaBoost在Haar分类器中的应用，所以只是描述了它在Haar分类器中的特性，而实际上AdaBoost是一种具有一般性的分类器提升算法，它使用的分类器并不局限某一特定算法。

[1]中给出的Adaboost算法流程如下图。

![img](https://img-blog.csdn.net/20140626141528625)

![img](https://img-blog.csdn.net/20140626141533343)



由adaboost在haar特征上构建分类器的流程可知，adaboost算法就是构建多个简单的分类器，每个简单的分类器都建立在之前分类器的基础上（对之前分类器分错了的样例提高其权重），然后将这些分类器加权，得到一个强大的分类器。

Adaboost的每一步训练出的分类器，如下图所示。其中，f表示特征的值，theta表示阈值，p则表示不等式的方向。这样的一个分类器就是基于一个特征的弱分类器。

![img](https://img-blog.csdn.net/20140626141645515)



 

更进一步，adaboost的一般算法框架如下。可以看到，Discrete Adaboost和GentleAdaboost在分类器的计算上和权重的更新上是有差别的。还有一种是RealAdaboost，即分类器输出的是一个概率而不只是+1与-1。[3]中就比较了这三种Adaboost的变种的效果。

![img](https://img-blog.csdn.net/20140626141705234)

![img](https://img-blog.csdn.net/20140626141708875)





# 5        级联

什么是级联分类器？级联分类器就是如下图所示的一种退化了的决策树。为什么说是退化了的决策树呢？是因为一般决策树中，判断后的两个分支都会有新的分支出现，而级联分类器中，图像被拒绝后就直接被抛弃，不会再有判断了。

![img](https://img-blog.csdn.net/20140626141749796)



级联强分类器的策略是，将若干个强分类器由简单到复杂排列，希望经过训练使每个强分类器都有较高检测率，而误识率可以放低，比如几乎99%的人脸可以通过，但50%的非人脸也可以通过，这样如果有20个强分类器级联，那么他们的总识别率为0.99^20约等于98%，错误接受率也仅为0.5^20约等于0.0001%。这样的效果就可以满足现实的需要了。文献[1]中给出了一种由简单到复杂设计级联分类器的方法，那就是添加特征法，对于第一个分类器，只用少数几个特征，之后的每个分类器都在上一个的基础上添加特征，直到满足该级的要求。

训练级联分类器的目的就是为了检测的时候，更加准确，这涉及到Haar分类器的另一个体系，检测体系，检测体系是以现实中的一幅大图片作为输入，然后对图片中进行多区域，多尺度的检测，所谓多区域，是要对图片划分多块，对每个块进行检测，由于训练的时候用的照片一般都是20*20左右的小图片，所以对于大的人脸，还需要进行多尺度的检测，多尺度检测机制一般有两种策略，一种是不改变搜索窗口的大小，而不断缩放图片，这种方法显然需要对每个缩放后的图片进行区域特征值的运算，效率不高，而另一种方法，是不断初始化搜索窗口size为训练时的图片大小，不断扩大搜索窗口，进行搜索，解决了第一种方法的弱势。

# 6        积分图

积分图是用来加速计算haar特征的方法。Haar特征的计算所需要的基本数据就是区域内像素的和。而仅仅对于24*24的图片来说，它的不同类型的haar特征数目就有11W个（参考[3]）。为了快速的计算出这些特征的值，就有了积分图表示图像的方法。

什么是积分图？对于与图像边界平行的haar特征来说，积分图就是与图像大小一样的一个二维数组。该数组中，(x,y)位置的值是原始图像中从(0,0)到(x,y)处的像素值的和。对于45度偏向的haar特征来说，也类似。如下图所示：

![img](https://img-blog.csdn.net/20140626141817218)



对于a、c图所示的haar特征。计算公式如下：

![img](https://img-blog.csdn.net/20140626141902250)

![img](https://img-blog.csdn.net/20140626141905687)

![img](https://img-blog.csdn.net/20140626141908890)









其中，SAT即为积分图中的值，RecSum即为(x,y)处的长宽为(w,h)的区域的和。有了RecSum后，就可以计算haar特征了。

相似的，b，d图所示的haar特征计算公式如下：

![img](https://img-blog.csdn.net/20140626141956859)

![img](https://img-blog.csdn.net/20140626142000703)

![img](https://img-blog.csdn.net/20140626142004796)





意义类似，不一一解释了。

积分图的好处在于，只对图片进行一次累计计算，就可以很方便的计算出haar特征值了。

# 7        总结

基于Haar特征的Adaboost级联分类器，在人脸的识别效果上并没有比其他算法高，其亮点在于检测速度。而速度的提升，有如下几方面的因素。第一：使用的特征简单，haar特征只需计算像素的和就可以了。第二：即便是如此简单的特征，还添加了积分图进行加速。第三，级联分类器的设定，使得大量的没有人脸的子窗口被抛弃。



# 参考文献

[1]. Viola P, Jones M. Rapid object detection using a boosted cascade of simple features[C]//Computer Vision and Pattern Recognition, 2001. CVPR 2001. Proceedings of the 2001 IEEE Computer Society Conference on. IEEE, 2001, 1: I-511-I-518 vol. 1.
[2]. Lienhart R, Maydt J. An extended set of haar-like features for rapid object detection[C]//Image Processing. 2002. Proceedings. 2002 International Conference on. IEEE, 2002, 1: I-900-I-903 vol. 1.



[3]. Lienhart R, Kuranov A, Pisarevsky V. Empirical analysis of detection cascades of boosted classifiers for rapid object detection[M]//Pattern Recognition. Springer Berlin Heidelberg, 2003: 297-304.

[4]. <http://blog.sina.com.cn/s/blog_74a459380101fcx7.html>

[5]. <http://www.codeproject.com/Articles/441226/Haar-feature-Object-Detection-in-Csharp>