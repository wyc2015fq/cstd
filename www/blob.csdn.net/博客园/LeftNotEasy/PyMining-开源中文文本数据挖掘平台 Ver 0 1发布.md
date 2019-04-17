# PyMining-开源中文文本数据挖掘平台 Ver 0.1发布 - LeftNotEasy - 博客园







# [PyMining-开源中文文本数据挖掘平台 Ver 0.1发布](https://www.cnblogs.com/LeftNotEasy/archive/2011/05/15/pymining-second-edition.html)





**项目首页：**

[http://code.google.com/p/python-data-mining-platform/](http://code.google.com/p/python-data-mining-platform/) （可能需FQ）

目前已经在该googlecode中加入了Tutorial等内容，可以在wiki中查看



**项目介绍（复制自项目首页的说明）：**

这是一个能够根据源数据（比如说用csv格式表示的矩阵，或者中文文档）使用多种多样的算法去得到结果的一个平台。  

算法能够通过xml配置文件去一个一个的运行，比如在开始的时候，我们可以先运行一下主成分分析算法去做特种选择，然后我们再运行随机森林算法来做分类。  

目前算法主要是针对那些单机能够完成的任务，该架构良好的扩展性能够让你在很短的时间内完成自己想要的算法，并且用于工程之中（相信我，肯定比Weka更快更好）。**该项目的另一个特色是能够很好的支持中文文本的分类、聚类**等操作。  

只需要写下下面的程序，就能够得到神奇的结果（对文本进行特征选择、得到naive bayes分类模型，并且预测）： 


   1:#load config   2: config = Configuration.FromFile("conf/test.xml")   3: PyMining.Init(config, "__global__")   4:   5:#get matrix from source text   6: matCreater = ClassifierMatrix(config, "__matrix__")   7: [trainx, trainy] = matCreater.CreateTrainMatrix("data/train.txt")   8:   9:#get chi square filter  10: chiFilter = ChiSquareFilter(config, "__filter__")  11: chiFilter.TrainFilter(trainx, trainy)  12:  13:#runs naive-bayes model to get model  14: nbModel = TwcNaiveBayes(config, "twc_naive_bayes")  15: nbModel.Train(trainx, trainy)  16:  17:#using the model to predict an unseen doc to target class  18: [testx, testy] = matCreater.CreatePredictMatrix("data/test.txt")  19: [testx, testy] = chiFilter.MatrixFilter(testx, testy)  20: retY = nbModel.TestMatrix(testx, testy)


**目前的版本：**

Ver 0.1（第二个开发版）



**Features：**

**    上一版的Feature：**
- 能够支持中文文本输入，并且对其进行分词等操作，作为分类的源数据 

- 带有卡方检测(chi square test)的特征词选择器(feature selector) 

- 参数的调整(parameter tuning)支持通过xml配置文件进行 



**     新增Feature：**
- 加入了K-Means算法，能够对文本进行聚类 
- 加入了基于补集的朴素贝叶斯算法，大大提升了分类的准确率，目前该算法在搜狗实验室文本分类数据中，对20000篇、8分类左右的数据的预测准确率在90%左右 
- 加入了Sogou实验室文本分类数据的导入器，可以进行更多的实验



**获取PyMining：**

     于[http://code.google.com/p/python-data-mining-platform/downloads/detail?name=pymining_0_1.zip&can=2&q=#makechanges](http://code.google.com/p/python-data-mining-platform/downloads/detail?name=pymining_0_1.zip&can=2&q=#makechanges)，可获取目前最新的ver 0.1版（可能需要FQ）

     不用FQ的版本：[http://files.cnblogs.com/LeftNotEasy/pymining_0_1.zip](http://files.cnblogs.com/LeftNotEasy/pymining_0_1.zip)












