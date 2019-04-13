
# Deep Learning（深度学习）学习笔记整理系列之（八） - zouxy09的专栏 - CSDN博客


2013年04月10日 11:42:50[zouxy09](https://me.csdn.net/zouxy09)阅读数：183474


**Deep Learning（深度学习）学习笔记整理系列**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
**作者：Zouxy**
**version 1.0 2013-04-08**
**声明：**
1）该Deep Learning的学习系列是整理自网上很大牛和机器学习专家所无私奉献的资料的。具体引用的资料请看参考文献。具体的版本声明也参考原文献。
2）本文仅供学术交流，非商用。所以每一部分具体的参考资料并没有详细对应。如果某部分不小心侵犯了大家的利益，还望海涵，并联系博主删除。
3）本人才疏学浅，整理总结的时候难免出错，还望各位前辈不吝指正，谢谢。
4）阅读本文需要机器学习、计算机视觉、神经网络等等基础（如果没有也没关系了，没有就看看，能不能看懂，呵呵）。
5）此属于第一版本，若有错误，还需继续修正与增删。还望大家多多指点。大家都共享一点点，一起为祖国科研的推进添砖加瓦（呵呵，好高尚的目标啊）。请联系：zouxy09@qq.com
**目录：**
[一、概述](http://blog.csdn.net/zouxy09/article/details/8775360)
[二、背景](http://blog.csdn.net/zouxy09/article/details/8775360)
[三、人脑视觉机理](http://blog.csdn.net/zouxy09/article/details/8775360)
[四、关于特征](http://blog.csdn.net/zouxy09/article/details/8775488)
4.1、特征表示的粒度
4.2、初级（浅层）特征表示
4.3、结构性特征表示
4.4、需要有多少个特征？
[五、Deep Learning的基本思想](http://blog.csdn.net/zouxy09/article/details/8775518)
[六、浅层学习（Shallow Learning）和深度学习（Deep Learning）](http://blog.csdn.net/zouxy09/article/details/8775518)
[七、Deep learning与Neural Network](http://blog.csdn.net/zouxy09/article/details/8775518)
[八、Deep learning训练过程](http://blog.csdn.net/zouxy09/article/details/8775518)
8.1、传统神经网络的训练方法
8.2、deep learning训练过程
[九、Deep Learning的常用模型或者方法](http://blog.csdn.net/zouxy09/article/details/8775524)
9.1、AutoEncoder自动编码器
9.2、Sparse Coding稀疏编码
9.3、Restricted Boltzmann Machine(RBM)限制波尔兹曼机
9.4、Deep BeliefNetworks深信度网络
9.5、Convolutional Neural Networks卷积神经网络
[十、总结与展望](http://blog.csdn.net/zouxy09/article/details/8782018)
[十一、参考文献和Deep Learning学习资源](http://blog.csdn.net/zouxy09/article/details/8782018)
接上
**十、总结与展望**
**1）Deep learning总结**
深度学习是关于自动学习要建模的数据的潜在（隐含）分布的多层（复杂）表达的算法。换句话来说，深度学习算法自动的提取分类需要的低层次或者高层次特征。高层次特征，一是指该特征可以分级（层次）地依赖其他特征，例如：对于机器视觉，深度学习算法从原始图像去学习得到它的一个低层次表达，例如边缘检测器，小波滤波器等，然后在这些低层次表达的基础上再建立表达，例如这些低层次表达的线性或者非线性组合，然后重复这个过程，最后得到一个高层次的表达。
Deep learning能够得到更好地表示数据的feature，同时由于模型的层次、参数很多，capacity足够，因此，模型有能力表示大规模数据，所以对于图像、语音这种特征不明显（需要手工设计且很多没有直观物理含义）的问题，能够在大规模训练数据上取得更好的效果。此外，从模式识别特征和分类器的角度，deep learning框架将feature和分类器结合到一个框架中，用数据去学习feature，在使用中减少了手工设计feature的巨大工作量（这是目前工业界工程师付出努力最多的方面），因此，不仅仅效果可以更好，而且，使用起来也有很多方便之处，因此，是十分值得关注的一套框架，每个做ML的人都应该关注了解一下。
当然，deep learning本身也不是完美的，也不是解决世间任何ML问题的利器，不应该被放大到一个无所不能的程度。
**2）Deep learning未来**
深度学习目前仍有大量工作需要研究。目前的关注点还是从机器学习的领域借鉴一些可以在深度学习使用的方法，特别是降维领域。例如：目前一个工作就是稀疏编码，通过压缩感知理论对高维数据进行降维，使得非常少的元素的向量就可以精确的代表原来的高维信号。另一个例子就是半监督流行学习，通过测量训练样本的相似性，将高维数据的这种相似性投影到低维空间。另外一个比较鼓舞人心的方向就是evolutionary programming approaches（遗传编程方法），它可以通过最小化工程能量去进行概念性自适应学习和改变核心架构。
Deep learning还有很多核心的问题需要解决：
（1）对于一个特定的框架，对于多少维的输入它可以表现得较优（如果是图像，可能是上百万维）？
（2）对捕捉短时或者长时间的时间依赖，哪种架构才是有效的？
（3）如何对于一个给定的深度学习架构，融合多种感知的信息？
（4）有什么正确的机理可以去增强一个给定的深度学习架构，以改进其鲁棒性和对扭曲和数据丢失的不变性？
（5）模型方面是否有其他更为有效且有理论依据的深度模型学习算法？
探索新的特征提取模型是值得深入研究的内容。此外有效的可并行训练算法也是值得研究的一个方向。当前基于最小批处理的随机梯度优化算法很难在多计算机中进行并行训练。通常办法是利用图形处理单元加速学习过程。然而单个机器GPU对大规模数据识别或相似任务数据集并不适用。在深度学习应用拓展方面，如何合理充分利用深度学习在增强传统学习算法的性能仍是目前各领域的研究重点。
**十一、参考文献和Deep Learning学习资源**（持续更新……）
先是机器学习领域大牛的微博：@余凯_西二旗民工；@老师木；@梁斌penny；@张栋_机器学习；@邓侃；@大数据皮东；@djvu9……
（1）Deep Learning
http://deeplearning.net/
（2）Deep Learning Methods for Vision
http://cs.nyu.edu/~fergus/tutorials/deep_learning_cvpr12/
（3）Neural Network for Recognition of Handwritten Digits[Project]
http://www.codeproject.com/Articles/16650/Neural-Network-for-Recognition-of-Handwritten-Digi
（4）Training a deep autoencoder or a classifier on MNIST digits
http://www.cs.toronto.edu/~hinton/MatlabForSciencePaper.html
（5）Ersatz：deep neural networks in the cloud
http://www.ersatz1.com/
（6）Deep Learning
http://www.cs.nyu.edu/~yann/research/deep/
（7）Invited talk "A Tutorial on Deep Learning" by Dr. Kai Yu (余凯)
http://vipl.ict.ac.cn/News/academic-report-tutorial-deep-learning-dr-kai-yu
（8）CNN - Convolutional neural network class
http://www.mathworks.cn/matlabcentral/fileexchange/24291
（9）Yann LeCun's Publications
http://yann.lecun.com/exdb/publis/index.html\#lecun-98
（10） LeNet-5, convolutional neural networks
http://yann.lecun.com/exdb/lenet/index.html
（11） Deep Learning 大牛Geoffrey E. Hinton's HomePage
http://www.cs.toronto.edu/~hinton/
（12）Sparse coding simulation software[Project]
http://redwood.berkeley.edu/bruno/sparsenet/
（13）Andrew Ng's homepage
http://robotics.stanford.edu/~ang/
（14）stanford deep learning tutorial
http://deeplearning.stanford.edu/wiki/index.php/UFLDL_Tutorial
（15）「深度神经网络」（deep neural network）具体是怎样工作的
http://www.zhihu.com/question/19833708?group_id=15019075\#1657279
（16）A shallow understanding on deep learning
http://blog.sina.com.cn/s/blog_6ae183910101dw2z.html
（17）Bengio's Learning Deep Architectures for AI
http://www.iro.umontreal.ca/~bengioy/papers/ftml_book.pdf
（18）andrew ng's talk video:
http://techtalks.tv/talks/machine-learning-and-ai-via-brain-simulations/57862/
（19）cvpr 2012 tutorial：
http://cs.nyu.edu/~fergus/tutorials/deep_learning_cvpr12/tutorial_p2_nnets_ranzato_short.pdf
（20）Andrew ng清华报告听后感
http://blog.sina.com.cn/s/blog_593af2a70101bqyo.html
（21）Kai Yu：CVPR12 Tutorial on Deep Learning Sparse Coding
（22）Honglak Lee：Deep Learning Methods for Vision
（23）Andrew Ng ：Machine Learning and AI via Brain simulations
（24）Deep Learning 【2,3】
http://blog.sina.com.cn/s/blog_46d0a3930101gs5h.html
（25）deep learning这件小事……
http://blog.sina.com.cn/s/blog_67fcf49e0101etab.html
（26）Yoshua Bengio, U. Montreal：Learning Deep Architectures
（27）Kai Yu：A Tutorial on Deep Learning
（28）Marc'Aurelio Ranzato：NEURAL NETS FOR VISION
（29）Unsupervised feature learning and deep learning
http://blog.csdn.net/abcjennifer/article/details/7804962
（30）机器学习前沿热点–Deep Learning
http://elevencitys.com/?p=1854
（31）机器学习——深度学习(Deep Learning)
http://blog.csdn.net/abcjennifer/article/details/7826917
（32）卷积神经网络
http://wenku.baidu.com/view/cd16fb8302d276a200292e22.html
（33）浅谈Deep Learning的基本思想和方法
http://blog.csdn.net/xianlingmao/article/details/8478562
（34）深度神经网络
http://blog.csdn.net/txdb/article/details/6766373
（35）Google的猫脸识别:人工智能的新突破
http://www.36kr.com/p/122132.html
（36）余凯，深度学习-机器学习的新浪潮，Technical News程序天下事
http://blog.csdn.net/datoubo/article/details/8577366
（37）Geoffrey Hinton：UCLTutorial on: Deep Belief Nets
（38）Learning Deep Boltzmann Machines
http://web.mit.edu/~rsalakhu/www/DBM.html
（39）Efficient Sparse Coding Algorithm
http://blog.sina.com.cn/s/blog_62af19190100gux1.html
（40）Itamar Arel, Derek C. Rose, and Thomas P. Karnowski： Deep Machine Learning—A New Frontier in Artificial Intelligence Research
（41）Francis Quintal Lauzon：An introduction to deep learning
（42）Tutorial on Deep Learning and Applications
（43）Boltzmann神经网络模型与学习算法
http://wenku.baidu.com/view/490dcf748e9951e79b892785.html
（44）Deep Learning 和 Knowledge Graph 引爆大数据革命
http://blog.sina.com.cn/s/blog_46d0a3930101fswl.html
（45）……


