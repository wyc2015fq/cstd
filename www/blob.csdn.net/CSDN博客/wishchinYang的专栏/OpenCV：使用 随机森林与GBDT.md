# OpenCV：使用 随机森林与GBDT - wishchinYang的专栏 - CSDN博客
2015年06月16日 16:33:25[wishchin](https://me.csdn.net/wishchin)阅读数：2285
        随机森林顾名思义，是用随机的方式建立一个森林。简单来说，随机森林就是由多棵CART（Classification And Regression Tree）构成的。对于每棵树，它们使用的训练集是从总的训练集中**有放回采样**出来的，这意味着，总的训练集中的有些样本可能多次出现在一棵树的训练集中，也可能从未出现在一棵树的训练集中。在训练每棵树的节点时，使用的特征是从所有特征中按照一定比例随机地无放回的抽取的，根据Leo Breiman的建议，假设总的特征数量为M，这个比例可以是sqrt(M),1/2sqrt(M),2sqrt(M)。
       随机性来源：特征选择的随机性、自举重采样的随机性。
       随机森林在多类识别上有足够的优点：随机森林可以作为天然的多类分类器，通过其学习特性进行bag弱分类器，不需要SVM类似的顶层多类训练；随机森林在训练的同时使用**随机采样**和**特征选择**，因此并不需要特别的剪枝，可以防止过拟合；并由于随机森林使用决策树，在本质上等同于布尔逻辑，理论上可以表达任意复杂的布尔函数，有强大的可扩充特性，因此随机森林在处理多样本问题上看似是无边界的，并容易实现在线学习方式-通过**在线分裂叶子节点**或者**增加生成树**等方式。
        其他描述可参考：[http://www.cnblogs.com/hrlnw/p/3850459.html](http://www.cnblogs.com/hrlnw/p/3850459.html)
        或者此描述：[https://www.cnblogs.com/maybe2030/p/4585705.htm](https://www.cnblogs.com/maybe2030/p/4585705.html)l
        随机森林在生成过程中，使用了模型平均，致力于降低集合**模型的方差**，能够获取到内部生成误差的一种**无偏估计**/It generates an internal unbiased estimate of the generalization error as the forest building progresses。且理论证明，随机森林的弱分类器的个数越多，分类器逐渐逼近理论模型的准确度极限。
        在   [**do we need hundreds of classifiers to solve real world classification problem**](http://www.jmlr.org/papers/v15/delgado14a.html) 论文中，对不使用DNN的传统分类器，十几个族的各个分类器在多个数据集上进行评测，得到了排名第一的效果。
**一、OpenCV的RTrees:**
1. RTrees参数，参考链接：[http://blog.csdn.net/pi9nc/article/details/12197731](http://blog.csdn.net/pi9nc/article/details/12197731)
|*CvRTParams*|定义R.T.训练用参数，CvDTreeParams的扩展子类，但并不用到CvDTreeParams（单一决策树）所需的所有参数。比如说，R.T.通常不需要剪枝，因此剪枝参数就不被用到。***max_depth***：单棵树所可能达到的最大深度***min_sample_count***：  树节点持续分裂的最小样本数量，也就是说，小于这个数节点就不持续分裂，变成叶子。***regression_accuracy***： 回归树的终止条件，如果所有节点的精度都达到要求就停止***use_surrogates*** ：是否使用代理分裂。通常都是false，在有缺损数据或计算变量重要性的场合为true，比如，变量是色彩，而图片中有一部分区域因为光照是全黑的***max_categories***：将所有可能取值聚类到有限类，以保证计算速度。树会以次优分裂（suboptimal split）的形式生长。只对2种取值以上的树有意义***priors*** ： 优先级设置，设定某些你尤其关心的类或值，使训练过程更关注它们的分类或回归精度。通常不设置***calc_var_importance*** ： 设置是否需要获取变量的重要值，一般设置true***nactive_vars***： 树的每个节点随机选择变量的数量，根据这些变量寻找最佳分裂。如果设置0值，则自动取变量总和的平方根***max_num_of_trees_in_the_forest：***  R.T.中可能存在的树的最大数量***forest_accuracy***： 准确率（作为终止条件）**termcrit_type**：终止条件设置			  — **CV_TERMCRIT_ITER**  以树的数目为终止条件，max_num_of_trees_in_the_forest生效			  –** CV_TERMCRIT_EPS**  以准确率为终止条件，forest_accuracy生效			  — **CV_TERMCRIT_ITER | CV_TERMCRIT_EPS**  两者同时作为终止条件|
|----|----|
|*CvRTrees::train*|训练R.T.***return bool*** ： 训练是否成功***train_data** ： *训练数据：样本（一个样本由固定数量的多个变量定义），以Mat的形式存储，以列或行排列，必须是CV_32FC1格式（Float格式！）***tflag***  trainData的排列结构			  — **CV_ROW_SAMPLE** 行排列			  — **CV_COL_SAMPLE**  列排列***responses***  ：训练数据：样本的值（输出），以一维Mat的形式存储，对应trainData，必须是CV_32FC1或CV_32SC1格式。对于分类问题，responses是类标签；对于回归问题，responses是需要逼近的函数取值***var_idx***  ：定义感兴趣的变量，变量中的某些，传null表示全部***sample_idx***  ：定义感兴趣的样本，样本中的某些，传null表示全部***var_type***  ：定义responses的类型			  — **CV_VAR_CATEGORICAL** 分类标签			  — **CV_VAR_ORDERED**（**CV_VAR_NUMERICAL**）数值，用于回归问题***missing_mask***  ：定义缺失数据，和train_data一样大的8位Mat***params** ：* CvRTParams定义的训练参数|
|*CvRTrees::train*|训练R.T.（简短版的train函数）***return bool***  训练是否成功***data  ***训练数据：CvMLData格式，可从外部.csv格式的文件读入，内部以Mat形式存储，也是类似的value / responses / missing mask。***params***CvRTParams定义的训练参数|
|*CvRTrees:predict*|对一组输入样本进行预测（分类或回归）***return double***  预测结果***sample***  输入样本，格式同CvRTrees::train的train_data***missing_mask***  定义缺失数据|
**2.   训练过程**
***Training***
       RF属于Bagging类模型，因此大体训练过程和Bagging类似，关键在于样本的随机选取避免模型的overfitting问题。RF中的每棵决策树是分开训练的，彼此之间并无关联。对于每棵决策树，训练之前形成一个样本子集，在这个子集中有些样本可能出现多次，而另一些可能一次都没出现。接下去，就是循序决策树训练算法的，针对这个样本子集的单棵决策树训练。
单棵决策树的生成大致遵循以下过程：
1）随机生成样本子集；
2）分裂当前节点为左右节点，比较所有可选分裂，选取最优者；
3）重复2）直至达到最大节点深度，或当前节点分类精度达到要求。
*这一过程是贪婪的。*
当然对于不同的应用场合，训练过程中，会有细节上的差别，比如样本子集的生成过程、以及最优分割的定义。   
**二.  使用OpenCV随机森林**
1. 所使用的代码：
```cpp
//训练过程——阈值、迭代次数、输出响应
//训练参数存储为XML newrtrees.xml
//注意事项： 此代码的内存错误我并没有解决，而使用GBDTree！已解决！
//注意事项： CvRTrees* forest = new CvRTrees; 使用是错误的，无法完成初始化，应该修改为：CvRTrees* forest = new CvRTrees();
int  CPalmRecog::rTreesTrain(LabeledFeatrureArrayI   &trainData)
{
	int NumSample = trainData.size();
	int NUMBER_OF_TRAINING_SAMPLES = NumSample;
	int ATTRIBUTES_PER_SAMPLE      = FeatureLenG;
	//定义训练数据与标签矩阵  
	cv::Mat training_data =
		cv::Mat(NUMBER_OF_TRAINING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
	cv::Mat training_classifications =
		cv::Mat(NUMBER_OF_TRAINING_SAMPLES, 1, CV_32FC1);
	//Copy 数据和标签 到训练数据！
	for (int i = 0; i< NUMBER_OF_TRAINING_SAMPLES; i++)   {
		for (int idx = 0; idx < ATTRIBUTES_PER_SAMPLE; ++idx){
			//转化为char类型
			training_data.at<float>(i,idx) = trainData[i].second[idx];
		}
		training_classifications.at<float>(i, 0) = trainData[i].first;//类别标签！
	}
	//1. RF参数！
	//正好15类//
	//float priors[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	CvRTParams Params(
		15, 10, 0.01f, 
		true,//false,
		15, 0, true,
		4,
		1000, 0.01f,
		//CV_TERMCRIT_ITER);//最大类别数设置为8//设置为64颗树如何？
		CV_TERMCRIT_EPS );
	//2. 随机森林训练 .SVM SVMS;// CvSVM SVMs;
	CvRTrees* forest = new CvRTrees();//CvRTrees* forest = new CvRTrees;
	CvMat* var_importance = 0;
	forest->train(
		training_data,//已经填充数据
		CV_ROW_SAMPLE,//CV_COL_SAMPLE,//CV_ROW_SAMPLE,
		training_classifications,//已经初始化，应该没有什么问题！！！
		cv::Mat(), cv::Mat(), cv::Mat(), cv::Mat(),
		Params);
	//3. Test，不需要吗？
	//4. 保存参数到XML文件
	forest->save("Out/rtrees.xml");
	return 1;
}
```
2.注意事项
对于网上copy的代码！
比如此行代码：
```cpp
CvRTrees* forest = new CvRTrees();//CvRTrees* forest = new CvRTrees;
```
使用GBTrees和RTrees的存储空间不太一样，使用RTrees得到89MB的森林XML文件，而GBTrees只获得了433KB的文件，精度上应该有些差别。
梯度提升树使用了**残差学习**的方法，每一层学习的是上一层的函数，类似于反馈模型，使用了较少的参数，因此泛化性能也相应地降低。
3.测试结果
```cpp
全部测试，错误率如下：
	Class (digit 0) false postives  0 (0%)
	Class (digit 1) false postives  9 (1.07527%)
	Class (digit 2) false postives  1 (0.119474%)
	Class (digit 3) false postives  0 (0%)
	Class (digit 4) false postives  0 (0%)
	Class (digit 5) false postives  0 (0%)
	Class (digit 6) false postives  0 (0%)
	Class (digit 7) false postives  0 (0%)
	Class (digit 8) false postives  0 (0%)
	Class (digit 9) false postives  1 (0.119474%)
	Class (digit 10) false postives  1 (0.119474%)
	Class (digit 11) false postives  0 (0%)
	Class (digit 12) false postives  0 (0%)
	Class (digit 13) false postives  0 (0%)
	Class (digit 14) false postives  0 (0%)
```
错误率：12/836 *100% = 1.435%
**二、GBDT**
参考文章：[机器学习中的算法——决策树模型组合之随机森林与GBDT](http://www.36dsj.com/archives/21036)
        原始的Boost算法是在算法开始的时候，为每一个样本赋上一个权重值，初始的时候，大家都是一样重要的。在每一步训练中得到的模型，会使得数据点的估计有对有错，我们就在每一步结束后，增加分错的点的权重，减少分对的点的权重，这样使得某些点如果老是被分错，那么就会被“严重关注”，也就被赋上一个很高的权重。然后等进行了N次迭代（由用户指定），将会得到N个简单的分类器（basic learner），然后我们将它们组合起来（比如说可以对它们进行加权、或者让它们进行投票等），得到一个最终的模型。
        而Gradient Boost与传统的Boost的区别是，每一次的计算是为了减少上一次的残差(residual)，而为了消除残差，我们可以在残差减少的梯度(Gradient)方向上建立一个新的模型。所以说，在Gradient Boost中，每个新的模型的简历是为了使得之前模型的残差往梯度方向减少，与传统Boost对正确、错误的样本进行加权有着很大的区别。
       OpenCV可以直接使用GBDT，方法与随机森林相同，在相同的准确率下生成的模型要小很多，同时这意味着泛化能力变差。
