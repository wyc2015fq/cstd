# OpenCV学习笔记（二十四）——初探机器学习库ml

2011年11月14日 10:30:47

yang_xian521

阅读数：19741

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

这部分内容应该是OpenCV高深精髓的一部分。给OpenCV插上了翅膀，可以使它实现更强大的功能，而不是简单的做一些基本的图像处理。文采太差，就不描绘machine learning的强大了。直接剖析之。

大部分的分类器和识别的算法都封装在了c++的类中。有些类有一些共同的基础，都被定义在CvStatModel类中了。

那就先介绍这个统计模型类CvStatModel，它是机器学习统计模型的基础类，其包括了构造函数和析构函数，清除内存重置模型状态的clear函数（功能类似析构函数，但可以重用，在析构函数里也调用该函数），模型保存  / 加载XML文件的save / load函数，读写文件和模型的函数write /  read，训练模型的函数train，预测样本结果predict函数。

普通的贝叶斯分类器CvNormalBayesClassifier，有train和predict函数

k近邻算法CvKNearest，有train、find_nearest、get_max_k、get_var_count、get_sample_count、is_regression函数

支持向量机SVM相关的有类CvParamGrid用来表示统计参数范围的对数格子，类CvSVMParams、类CvSVM

决策树类CvDTreeSplit、CvDTreeNode、CvDTreeParams、CvDTreeTrainData、CvDTree

Boosting算法相关类CvBoostParams、CvBoostTree、CvBoost

Gradient Boosted Trees（GBT）算法相关类CvGBTreesParams、CvGBTrees

随机森林相关算法类CvRTparams、CvRTrees

随机树算法的扩展类CvERTrees

期望最大EM算法相关类CvEMParams、CvEM

神经网络算法相关类CvANN_MLP_TrainParams、CvANN_MLP

OpenCV果然够强大，几乎囊括了目前比较流行的全部机器学习方面的经典算法。以上这些牛叉的算法我都好膜拜啊，得下苦功专研了。

记得哪位牛人说过algorithm is king，data is queen。有了算法还需要对应的数据类CvMLData、CvTrainTestSplit

这部分内容真是博大精深啊，作为初学者，我一直不太敢写这方面的博客，很多算法只知道个皮毛，没有实现过，更不知道其中的奥妙，这里只能先做个介绍，待我日后慢慢丰富内容