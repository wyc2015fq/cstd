# python版本及ML库 - wishchinYang的专栏 - CSDN博客
2013年10月27日 21:10:53[wishchin](https://me.csdn.net/wishchin)阅读数：1558
**一：关于Python版本的选择问题**
          关于Python的选择问题：要看学术界能不能把科学库迁移到Python3.
1：**多个版本共用**：
最近发现SciPy的最高版本是3.2，只能是退而求其次，不使用最新版本了！！！
把注册表修改一下，就可以多个版本共用了！
Windows Registry Editor Version 5.00
[HKEY_LOCAL_MACHINE\SOFTWARE\Python]
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore]
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\3.2]
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\3.2\Help]
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\3.2\Help\Main Python Documentation]
@="D:\\PCL_X64\\Python32\\Doc\\python324.chm"
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\3.2\InstallPath]
@="D:\\PCL_X64\\Python32\\"
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\3.2\InstallPath\InstallGroup]
@="Python 3.2"
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\3.2\Modules]
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\3.2\PythonPath]
@="D:\\PCL_X64\\Python32\\Lib;D:\\PCL_X64\\Python32\\DLLs"
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\2.7]
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\2.7\Help]
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\2.7\Help\Main Python Documentation]
@="C:\\Python27\\Doc\\python275.chm"
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\3.2\InstallPath]
@="C:\\Python27\\"
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\3.2\InstallPath\InstallGroup]
@="Python 2.7"
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\2.7\Modules]
[HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\3.2\PythonPath]
@="C:\\Python27\\Lib;C:\\Python27\\DLLs"
关于找不到路径问题，上述方法可以解决了！直接修改注册表
2：在使用SciPy时缺少包的问题，对于WIndows系统，可以参考一下页面：
[python包：http://www.lfd.uci.edu/~gohlke/pythonlibs/](http://www.lfd.uci.edu/~gohlke/pythonlibs/)
看来windows下还是很给力了！
3：Scikit学习库的下载地址：[https://pypi.python.org/pypi/scikit-learn/](https://pypi.python.org/pypi/scikit-learn/)
4：python症取代R语言成为数据科学的编程语言
[http://readwrite.com/2013/11/25/python-displacing-r-as-the-programming-language-for-data-science#awesm=~op7vaD2VWwJVdM](http://readwrite.com/2013/11/25/python-displacing-r-as-the-programming-language-for-data-science#awesm=~op7vaD2VWwJVdM)
5：**Tips**：再Python3没有完全普遍开始之前，最好使用Python2.7.
**二：Python的机器学习库**
原文链接：[http://qxde01.blog.163.com/blog/static/67335744201368101922991/](http://qxde01.blog.163.com/blog/static/67335744201368101922991/)
首选 Scikit-Learn.....
**1.       scikit-learn**
scikit-learn 是一个基于SciPy和Numpy的开源机器学习模块，包括分类、回归、聚类系列算法，主要算法有SVM、逻辑回归、朴素贝叶斯、Kmeans、DBSCAN等，目前由INRI 资助，偶尔Google也资助一点。
项目主页：
[https://pypi.python.org/pypi/scikit-learn/](https://pypi.python.org/pypi/scikit-learn/)
[http://scikit-learn.org/](http://scikit-learn.org/)
[https://github.com/scikit-learn/scikit-learn](https://github.com/scikit-learn/scikit-learn)
2.      **NLTK**
NLTK(NaturalLanguage Toolkit)是Python的自然语言处理模块，包括一系列的字符处理和语言统计模型。NLTK常用于学术研究和教学，应用的领域有语言学、认知科学、人工智能、信息检索、机器学习等。NLTK提供超过50个语料库和词典资源，文本处理库包括分类、分词、词干提取、解析、语义推理。可稳定运行在Windows, Mac OS X和Linux平台上. 
项目主页：
[http://sourceforge.net/projects/nltk/](http://sourceforge.net/projects/nltk/)
[https://pypi.python.org/pypi/nltk/](https://pypi.python.org/pypi/nltk/)
[http://nltk.org/](http://nltk.org/)
3.      **Mlpy**
Mlpy是基于NumPy/SciPy的Python机器学习模块，它是Cython的扩展应用。包含的机器学习算法有：
l 回归
[leastsquares](http://en.wikipedia.org/wiki/Least_squares), [ridgeregression](http://en.wikipedia.org/wiki/Ridge_regression),
 leastangle regression, [elastic net](http://en.wikipedia.org/wiki/Elastic_net_regularization), kernel ridge regression, [supportvector
 machines](http://en.wikipedia.org/wiki/Support_vector_machines) (SVM), partialleast squares (PLS)
l  分类
linear discriminant analysis (LDA), Basic perceptron, Elastic Net, logistic regression, (Kernel) Support VectorMachines (SVM), Diagonal Linear Discriminant Analysis (DLDA), Golub Classifier,Parzen-based, (kernel) Fisher Discriminant
 Classifier, k-nearest neighbor,Iterative RELIEF, Classification Tree, Maximum Likelihood Classifier
l  聚类
hierarchical clustering, Memory-saving HierarchicalClustering, [k-means](http://en.wikipedia.org/wiki/K-means)
l  维度约减
(Kernel) [Fisher discriminant analysis](http://en.wikipedia.org/wiki/Fisher_discriminant_analysis) (FDA), Spectral
 Regression Discriminant Analysis(SRDA), (kernel) [Principal component analysis](http://en.wikipedia.org/wiki/Principal_component_analysis) (PCA)
项目主页：
[http://sourceforge.net/projects/mlpy](http://sourceforge.net/projects/mlpy)
[https://mlpy.fbk.eu/](https://mlpy.fbk.eu/)
4.  **Shogun**
Shogun是一个开源的大规模机器学习工具箱。目前Shogun的机器学习功能分为几个部分：feature表示，feature预处理， 核函数表示,核函数标准化，距离表示，分类器表示，聚类方法，分布， 性能评价方法，回归方法，结构化输出学习器。
SHOGUN 的核心由C++实现，提供 Matlab、 R、 Octave、 Python接口。主要应用在linux平台上。
项目主页：
[http://www.shogun-toolbox.org/](http://www.shogun-toolbox.org/)
5.   MDP
**TheModular toolkit for Data Processing (MDP)** ，用于数据处理的模块化工具包，一个Python数据处理框架。
从用户的观点，MDP是能够被整合到数据处理序列和更复杂的前馈网络结构的一批监督学习和非监督学习算法和其他数据处理单元。计算依照速度和内存需求而高效的执行。从科学开发者的观点，MDP是一个模块框架，它能够被容易地扩展。新算法的实现是容易且直观的。新实现的单元然后被自动地与程序库的其余部件进行整合。MDP在神经科学的理论研究背景下被编写，但是它已经被设计为在使用可训练数据处理算法的任何情况中都是有用的。其站在用户一边的简单性，各种不同的随时可用的算法，及应用单元的可重用性，使得它也是一个有用的教学工具。
项目主页：
[http://mdp-toolkit.sourceforge.net/](http://mdp-toolkit.sourceforge.net/)
[https://pypi.python.org/pypi/MDP/](https://pypi.python.org/pypi/MDP/)
6.      **PyBrain**
PyBrain(Python-BasedReinforcement Learning, Artificial Intelligence and Neural Network)是Python的一个机器学习模块，它的目标是为机器学习任务提供灵活、易应、强大的机器学习算法。（这名字很霸气）
PyBrain正如其名，包括神经网络、强化学习(及二者结合)、无监督学习、进化算法。因为目前的许多问题需要处理连续态和行为空间，必须使用函数逼近(如神经网络)以应对高维数据。PyBrain以神经网络为核心，所有的训练方法都以神经网络为一个实例。
项目主页：
[http://www.pybrain.org/](http://www.pybrain.org/)
[https://github.com/pybrain/pybrain/](https://github.com/pybrain/pybrain/)
7.      **BigML**
BigML 使得机器学习为数据驱动决策和预测变得容易，BigML使用容易理解的交互式操作创建优雅的预测模型。BigML使用BigML.io,捆绑Python。
项目主页：
[https://bigml.com/](https://bigml.com/)
[https://pypi.python.org/pypi/bigml](https://pypi.python.org/pypi/bigml)
[http://bigml.readthedocs.org/](http://bigml.readthedocs.org/)
8.      **PyML**
PyML是一个Python机器学习工具包， 为各分类和回归方法提供灵活的架构。它主要提供特征选择、模型选择、组合分类器、分类评估等功能。
项目主页：
[http://cmgm.stanford.edu/~asab/pyml/tutorial/](http://cmgm.stanford.edu/~asab/pyml/tutorial/)
[http://pyml.sourceforge.net/](http://pyml.sourceforge.net/)
9.      **Milk**
Milk是Python的一个机器学习工具箱，其重点是提供监督分类法与几种有效的分类分析：SVMs(基于libsvm)，K-NN，随机森林经济和决策树。它还可以进行特征选择。这些分类可以在许多方面相结合，形成不同的分类系统。
对于无监督学习，它提供K-means和affinitypropagation聚类算法。
项目主页：
[https://pypi.python.org/pypi/milk/](https://pypi.python.org/pypi/milk/)
[http://luispedro.org/software/milk](http://luispedro.org/software/milk)
10.  **PyMVPA**
PyMVPA(MultivariatePattern Analysis in Python)是为大数据集提供统计学习分析的Python工具包，它提供了一个灵活可扩展的框架。它提供的功能有分类、回归、特征选择、数据导入导出、可视化等
项目主页：
[http://www.pymvpa.org/](http://www.pymvpa.org/)
[https://github.com/PyMVPA/PyMVPA](https://github.com/PyMVPA/PyMVPA)
11.  **Pattern**
Pattern是Python的web挖掘模块，它绑定了  Google、Twitter 、WikipediaAPI，提供网络爬虫、HTML解析功能，文本分析包括浅层规则解析、WordNet接口、句法与语义分析、TF-IDF、LSA等，还提供聚类、分类和图网络可视化的功能。
项目主页：
[http://www.clips.ua.ac.be/pages/pattern](http://www.clips.ua.ac.be/pages/pattern)
[https://pypi.python.org/pypi/Pattern](https://pypi.python.org/pypi/Pattern)
12.  **pyrallel**
Pyrallel(ParallelData Analytics in Python)基于分布式计算模式的机器学习和半交互式的试验项目，可在小型集群上运行，适用范围：
l  focus on small to medium dataset that fits in memory on a small (10+nodes) to medium cluster (100+ nodes).
l  focus on small to medium data (with data locality when possible).
l  focus on CPU bound tasks (e.g. training Random Forests) while tryingto limit disk / network access to a minimum.
l  do not focus on HA / Fault Tolerance (yet).
l  do not try to invent new set of high level programming abstractions(yet): use a low level programming model (IPython.parallel) to finely controlthe cluster elements and messages transfered and help identify what are thepractical
 underlying constraints in distributed machine learning setting.
项目主页：
https://pypi.python.org/pypi/pyrallel
[http://github.com/pydata/pyrallel](http://github.com/pydata/pyrallel)
13.  **Monte**
Monte (machine learning in pure Python)是一个纯Python机器学习库。它可以迅速构建神经网络、条件随机场、逻辑回归等模型，使用inline-C优化，极易使用和扩展。
项目主页：
[https://pypi.python.org/pypi/Monte](https://pypi.python.org/pypi/Monte)
[http://montepython.sourceforge.net](http://montepython.sourceforge.net/)
14.  **Orange**
Orange 是一个基于组件的数据挖掘和机器学习软件套装，它的功能即友好，又很强大，快速而又多功能的可视化编程前端，以便浏览数据分析和可视化，基绑定了 Python以进行脚本开发。它包含了完整的一系列的组件以进行数据预处理，并提供了数据帐目，过渡，建模，模式评估和勘探的功能。其由C++ 和 Python开发，它的图形库是由跨平台的Qt框架开发。
项目主页：
[https://pypi.python.org/pypi/Orange/](https://pypi.python.org/pypi/Orange/)
[http://orange.biolab.si/](http://orange.biolab.si/)
15.  **Theano**
Theano 是一个 Python 库，用来定义、优化和模拟数学表达式计算，用于高效的解决多维数组的计算问题。Theano的特点：
> 
l  紧密集成Numpy
l  高效的数据密集型GPU计算
l  高效的符号微分运算
l  高速和稳定的优化
l  动态生成c代码
l  广泛的单元测试和自我验证
自2007年以来，Theano已被广泛应用于科学运算。theano使得构建深度学习模型更加容易，可以快速实现下列模型：
l LogisticRegression 
l Multilayerperceptron 
l DeepConvolutional Network 
l AutoEncoders, Denoising Autoencoders 
l StackedDenoising Auto-Encoders 
l RestrictedBoltzmann Machines 
l Deep BeliefNetworks 
l HMCSampling
l Contractiveauto-encoders 
Theano，一位希腊美女，Croton最有权势的Milo的女儿，后来成为了毕达哥拉斯的老婆。
项目主页：
[http://deeplearning.net/tutorial/](http://deeplearning.net/tutorial/)
[https://pypi.python.org/pypi/Theano](https://pypi.python.org/pypi/Theano)
16.     **Pylearn2**
Pylearn2建立在theano上，部分依赖scikit-learn上，目前Pylearn2正处于开发中，将可以处理向量、图像、视频等数据，提供MLP、RBM、SDA等深度学习模型。Pylearn2的目标是：
- Researchers add features as they need them. We avoid getting bogged down by too much top-down planning in advance.
- A machine learning toolbox for easy scientific experimentation.
- All models/algorithms published by the LISA lab should have reference implementations in Pylearn2.
- Pylearn2 may wrap other libraries such as scikits.learn when this is practical
- Pylearn2 differs from scikits.learn in that Pylearn2 aims to provide great flexibility and make it possible for a researcher to do almost anything, while scikits.learn aims to work as a “black box” that can produce good results
 even if the user does not understand the implementation
- Dataset interface for vector, images, video, ...
- Small framework for all what is needed for one normal MLP/RBM/SDA/Convolution experiments.
- Easy reuse of sub-component of Pylearn2.
- Using one sub-component of the library does not force you to use / learn to use all of the other sub-components if you choose not to.
- Support cross-platform serialization of learned models.
- Remain approachable enough to be used in the classroom (IFT6266 at the University of Montreal).
项目主页：
[http://deeplearning.net/software/pylearn2/](http://deeplearning.net/software/pylearn2/)
[https://github.com/lisa-lab/pylearn2](https://github.com/lisa-lab/pylearn2)
还有其他的一些Python的机器学习库，如：
pmll([https://github.com/pavlov99/pmll](https://github.com/pavlov99/pmll))
[pymining](https://pypi.python.org/pypi/pymining/0.1)([https://github.com/bartdag/pymining](https://github.com/bartdag/pymining))
ease (https://github.com/edx/ease)
textmining([http://www.christianpeccei.com/textmining/](http://www.christianpeccei.com/textmining/))
更多的机器学习库可通过[https://pypi.python.org/pypi](https://pypi.python.org/pypi)查找。
