# 决策树（七）--Boost及源码分析 - 工作笔记 - CSDN博客





2016年08月08日 11:30:39[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5786








原文：http://blog.csdn.net/zhaocj/article/details/50536385

一、原理

AdaBoost（**Ada**ptive **Boost**ing，自适应提升）算法是由来自AT&T实验室的Freund和Schapire于1995年首次提出，该算法解决了早期Boosting算法的一些实际执行难题，而且该算法可以作为一种从一系列弱分类器中产生一个强分类器的通用方法。正由于AdaBoost算法的优异性能，Freund和Schapire因此获得了2003年度的哥德尔奖（Gödel Prize，该奖是在理论计算机科学领域中最负盛名的奖项之一）。

假设我们有一个集合{(*x*1, *y*1),(*x*2,*y*2), …,
 (*xN*,*yN*)}，每一个数据项*xi*是一个表示事物特征的矢量，*yi*是一个与其相对应的分类*yi*∈{-1,
 1}，即*xi*要么属于-1，要么属于1。AdaBoost算法通过*M*次迭代得到了一个弱分类器集合{*k*1,*k*2,…,
*kM*}，对于每一个数据项*xi*来说，每个弱分类器都会给出一个分类结果来，即*km*(*xi*)∈{-1,
 1}。这*M*个弱分类器通过某种线性组合（式1所示）就得到了一个强分类器*Cm*，这样我们就可以通过*Cm*来判断一个新的数据项*xk*是属于-1，还是1。这就是一个训练的过程。

在进行了第*m*-1次迭代后，我们可以把这*m*-1个弱分类器进行线性组合，所得到的强分类器为：

![](https://img-blog.csdn.net/20160118141336342)（1）

式中，*α*为*k*的权值，并且*m *> 1。当进行第*m*次迭代时，AdaBoost就通过增加一个弱分类器的方式扩展成另一个的强分类器：

![](https://img-blog.csdn.net/20160118141435952)（2）

只有使*Cm*的分类结果强于*Cm*-1，算法迭代才有意义。那么如何能够使*Cm*的性能优于*Cm*-1呢？这就由增加的那个第*m*个弱分类器*km*及其它的权值*αm*来决定。我们用所有数据项*xi*的指数损失的总和来定义*Cm*的误差*E*，从而判断*km*和*αm*是否为最优，即：

![](https://img-blog.csdn.net/20160118141541782)（3）

令 ![](https://img-blog.csdn.net/20160118141647199) ，*wi*(*m*)表示在第*m*-1次迭代后，对训练数据项*xi*所分配的权重，而在第1次迭代时，所使用的权重为1，即*wi*(1)
 = 1。那么，上式可以重写为：

![](https://img-blog.csdn.net/20160118141744565)（4）

我们可以把上式拆分成两项：

![](https://img-blog.csdn.net/20160118141837166)（5）

式中，*yi*=*km*(*xi*)的意思是对*xi*来说，第*m*次迭代得到的弱分类器*km*的分类结果与其实际的分类*y*相同，要么都等于-1，要么都等于1，即分类正确，所以它们的乘积一定等于1，即*yi km*(*xi*)= 1；而*yi*≠*km*(*xi*)的意思是对*xi*来说，第*m*次迭代得到的弱分类器*km*的分类结果与其实际的分类*y*不相同，其中一个如果等于-1，那么另一个一定等于1，即分类错误，所以它们的乘积一定等于-1，即*yi km*(*xi*)= -1。所以式5中的前一项表示的是对所有*y*等于*km*的那些数据项*xi*的误差求和；而第二项表示的是对所有*y*不等于*km*的那些数据项*xi*的误差求和。我们又可以把式5写出如下形式：

![](https://img-blog.csdn.net/20160118141933958)（6）

从上式我们可以看出，如果*αm*一定的话，强分类器*Cm*的误差大小完全取决于第二项中的![](https://img-blog.csdn.net/20160118142024039) 的大小，也就是取决于该次迭代中被分类错误的那些权值![](https://img-blog.csdn.net/20160118142148926)
 的大小。
为了使误差*E*最小，我们需要对式5进行求导：

![](https://img-blog.csdn.net/20160118142243908)（7）

令上式等于0，则权值*αm*为：

![](https://img-blog.csdn.net/20160118142334736)（8）

令![](https://img-blog.csdn.net/20160118142438339) 表示误差率，其值为：

![](https://img-blog.csdn.net/20160118142523392)（9）

则式8为：

![](https://img-blog.csdn.net/20160118142606951)（10）

经过以上分析，我们导出了AdaBoost算法：在每次迭代中，我们选择使![](https://img-blog.csdn.net/20160118142647515) 为最小的分类器*km*，并得到误差率 （式9），应用该误差率 我们又得到了权值*αm*（式10），则最终强分类器就由*Cm*-1提升为*Cm*=*Cm*-1
 +*αmkm*。而在每次迭代后，得到的每个训练样本数据的权值*wi*(*m*+1)为：

![](https://img-blog.csdn.net/20160118142821505)（11）

需要说明的是，权值仍然可以按照前面介绍的![](https://img-blog.csdn.net/20160118142913061) 方法计算，但式11更简单，在应用迭代时效率更高。

基于以上的分析，我们给出AdaBoost的计算步骤：

1、设有*n*个样本*x*1, …,*xn*，它们所希望得到的输出（即分类）为*y*1, …,*yn*，*y*∈{-1,
 1}；

2、初始化每个样本的权值 ![](https://img-blog.csdn.net/20160118143023140)，它们都为![](https://img-blog.csdn.net/20160118143129141) ；
3、进行迭代：*m* = 1, …, *M*

①找到使误差率![](https://img-blog.csdn.net/20160118142438339) 最小的弱分类器*km*(*x*)，并得到![](https://img-blog.csdn.net/20160118142438339)（式9）；
②计算*km*(*x*)的权值*αm*（式10）；

③得到新的强分类器*Cm*(*x*)（式2）；

④更新每个样本的权值*wi*(*m*+1)（式11）；

⑤对权值*wi*(*m*+1)进行归一化处理，使∑*iwi*(*m*+1)=
 1；

4、得到最终的强分类器

![](https://img-blog.csdn.net/20160118143338975)（12）

从式12可以看出，强分类器是由权值*αm*和弱分类器*km*(*x*)决定的。权值*αm*由式10计算得到，而弱分类器可以用上一篇文章介绍的决策树的方法得到，那种最简单的、每个中间节点只能被分叉为左、右两个分支（stumps）的决策树就足以完成对弱分类器的设计。也就是说，每一个弱分类器就是一个决策树，而该决策树是由加权后的样本构建而成，由于每次迭代的权值*wi*(*m*)不同，所以每次迭代所构建的决策树也是不同的。当要应用AdaBoost预测样本*x*时，只要把该样本带入不同的决策树（即弱分类器）进行预测即可，预测的结果就是*km*(*x*)，然后应用式12把不同的决策树的预测结果进行加权和，最后判断其符号。OpenCV就是采用的这种方法。当用决策树的形式的时候，权值*αm*的计算公式为：

![](https://img-blog.csdn.net/20160118143431724)（13）

而每个训练样本数据的权值*wi*(*m*+1)则为：

![](https://img-blog.csdn.net/20160118143509308)（14）

AdaBoost算法可分为DiscreteAdaboost，Real AdaBoost，LogitBoost和Gentle AdaBoost。前面我们给出的是Discrete Adaboost算法的原理和计算步骤。虽然这四种算法的基本结构相似的，但彼此之间还是有一些差异。Discrete AdaBoost的每个弱分类的输出结果是1或-1，在组成强分类器的过程中，这种离散形式的弱分类器就略显粗糙。而Real Adaboost的每个弱分类器输出是该样本属于某一类的概率，即弱分类器*km*是通过一个对数函数将0-1的概率值映射到实数域，sign(*km*)给出的是一个分类，而|*km*|给出的就是一个置信度的度量。Real
 Adaboost的迭代过程为：

①基于每个样本的权值*wi*(*m*)，拟合一个分类概率估计*pm*(*x*)=*P*(*y*=1|*x*)∈[0,1]，它表示样本属于分类结果为1的概率；

②得到该次迭代的弱分类器*km*(*x*)：

![](https://img-blog.csdn.net/20160118143607190)（15）

③更新权值*wi*(*m*+1)：

![](https://img-blog.csdn.net/20160118143649133)（16）

④归一化权值使∑*iwi*(*m*+1)= 1。

则最终的强分类器*C*为：

![](https://img-blog.csdn.net/20160118143736580)（17）

LogitBoost算法是逻辑回归技术在AdaBoost的应用，弱分类器的选取并不是基于分类结果*yi*，而是基于加权最小二乘法。设迭代之前强分类器*C*0(*x*) = 0，每个训练样本数据的概率估计*p*0(*xi*)
 = 0.5，则LogitBoost算法的迭代过程为：

①计算工作响应*zi*(*m*)：

![](https://img-blog.csdn.net/20160118143823219)（18）

式中

![](https://img-blog.csdn.net/20160118143903831)（19）

②计算权值*wi*(*m*)：

![](https://img-blog.csdn.net/20160118143952517)（20）

③应用权值*wi*(*m*)，基于从*zi*(*m*)到*xi*的加权最小二乘法回归方法，拟合弱分类器*km*(*x*)；

④更新*pm*(*xi*)：

![](https://img-blog.csdn.net/20160118144042755)（21）

⑤更新强分类器

![](https://img-blog.csdn.net/20160118144124498)（22）

最终的强分类器为式17。

Gentle AdaBoost算法与LogitBoost算法相似，但参数的选择上更简单。弱分类器*km*(*x*)是由基于权值*wi*(*m*)的从*yi*到*xi*的加权最小二乘法的回归拟合得到。每次迭代得到强分类器*Cm*(*x*)和权值分别*wi*(*m*+1)为：

![](https://img-blog.csdn.net/20160118144225910)（23）

![](https://img-blog.csdn.net/20160118144300666)（24）

这里的权值仍然需要进行归一化处理。强分类器仍然为式17。

OpenCV实现了上述四种类型的AdaBoost，并且弱分类器都是采用CART决策树的方法，其中DiscreteAdaboost和Real AdaBoost应用的是分类树，因为它们的分类结果是类的形式，而LogitBoost和Gentle AdaBoost应用的是回归树，因为它们的分类结果是数值的形式。



二、源码分析



下面我们就给出OpenCV的源码分析。



我们先看构建Boosting的参数：





**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- CvBoostParams::CvBoostParams()  
- {  
-     boost_type = CvBoost::REAL;  
-     weak_count = 100;  
-     weight_trim_rate = 0.95;  
-     cv_folds = 0;  
-     max_depth = 1;  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
CvBoostParams::CvBoostParams()
{
    boost_type = CvBoost::REAL;
    weak_count = 100;
    weight_trim_rate = 0.95;
    cv_folds = 0;
    max_depth = 1;
}
```



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- CvBoostParams::CvBoostParams( int _boost_type, int _weak_count,  
- double _weight_trim_rate, int _max_depth,  
- bool _use_surrogates, constfloat* _priors )  
- {  
-     boost_type = _boost_type;  
-     weak_count = _weak_count;  
-     weight_trim_rate = _weight_trim_rate;  
-     split_criteria = CvBoost::DEFAULT;    //分叉准则，即用什么方法计算决策树节点的纯度
- /*****************************
- CvBoost::DEFAULT为特定的Boosting算法选择默认系数
- CvBoost::GINI使用基尼指数，这是Real AdaBoost的默认方法，也可以被用于Discrete Adaboost
- CvBoost::MISCLASS使用错误分类率，这是Discrete Adaboost的默认方法，也可以被用于Real AdaBoost
- CvBoost::SQERR使用最小二乘准则，这是LogitBoost和Gentle AdaBoost的默认及唯一方法
- ******************************/
-     cv_folds = 0;    //表示构建决策树时，不执行剪枝操作
-     max_depth = _max_depth;  
-     use_surrogates = _use_surrogates;  
-     priors = _priors;  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
CvBoostParams::CvBoostParams( int _boost_type, int _weak_count,
                                        double _weight_trim_rate, int _max_depth,
                                        bool _use_surrogates, const float* _priors )
{
    boost_type = _boost_type;
    weak_count = _weak_count;
    weight_trim_rate = _weight_trim_rate;
    split_criteria = CvBoost::DEFAULT;    //分叉准则，即用什么方法计算决策树节点的纯度
/*****************************
CvBoost::DEFAULT为特定的Boosting算法选择默认系数
CvBoost::GINI使用基尼指数，这是Real AdaBoost的默认方法，也可以被用于Discrete Adaboost
CvBoost::MISCLASS使用错误分类率，这是Discrete Adaboost的默认方法，也可以被用于Real AdaBoost
CvBoost::SQERR使用最小二乘准则，这是LogitBoost和Gentle AdaBoost的默认及唯一方法
******************************/
    cv_folds = 0;    //表示构建决策树时，不执行剪枝操作
    max_depth = _max_depth;
    use_surrogates = _use_surrogates;
    priors = _priors;
}
```







其中参数的含义为：

boost_type表示Boosting算法的类型，可以是CvBoost::DISCRETE、CvBoost::REAL、CvBoost::LOGIT或CvBoost::GENTLE这4类中的任意一个，OpenCV推荐使用GentleAdaBoost或Real AdaBoost算法

_weak_count表示弱分类器的数量，即迭代的次数

_weight_trim_rate表示裁剪率，在0～1之间，默认值为0.95，在迭代过程中，那些归一化后的样本权值*wi*(*m*)小于该裁剪率的样本将不进入下次迭代

_max_depth表示构建决策树的最大深度

_use_surrogates表示在构建决策树时，是否使用替代分叉属性

_priors表示样本的先验概率



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- CvBoost类的两个构造函数：  
- CvBoost::CvBoost( const Mat& _train_data, int _tflag,  
- const Mat& _responses, const Mat& _var_idx,  
- const Mat& _sample_idx, const Mat& _var_type,  
- const Mat& _missing_mask,  
-                CvBoostParams _params )  
- //_train_data训练的样本数据
- //_tflag训练数据的特征属性类型，如果为CV_ROW_SAMPLE，表示样本是以行的形式储存的，即_train_data矩阵的每一行为一个样本（或特征矢量）；如果为CV_COL_SAMPLE，表示样本是以列的形式储存的
- //_responses分类的结果，即分类的响应值
- //_var_idx标识感兴趣的特征属性，即真正用于训练的那些特征属性，该值的形式与_sample_idx变量相似
- //_sample_idx标识感兴趣的样本，即真正用于训练的样本，该值必须是一维矩阵的形式，即矢量的形式，并且类型必须是8UC1、8SU1或者32SC1。如果为8UC1或8SU1，则该值的含义是用掩码的形式表示对应的样本，即0表示不感兴趣的样本，其他数为感兴趣的样本，因此矢量的元素数量必须与训练样本数据_train_data的样本数一致；如果为32SC1，则该值的含义是那些感兴趣的样本的索引，而不感兴趣的样本的索引不在该矢量中出现，因此该矢量的元素数量可以小于或等于_train_data的样本数
- //_var_type特征属性的形式，是类的形式还是数值的形式，用掩码的形式表现对应特征属性的形式，0表示为数值的形式，1表示为类的形式。该值必须是一维矩阵，并且元素的数量必须是真正用于训练的那些特征属性的数量加1，多余的一个元素表示的是响应值的形式，即是分类树还是回归树
- //_missing_mask缺失的特征属性，用掩码的形式表现对应的特征属性，0表示没有缺失，而且必须与_train_data的矩阵尺寸大小一致
- //_params为构建Boosting的参数
- {  
- // CvSeq* weak，用于保存决策树，即最终的强分类器
-     weak = 0;  
- // CvDTreeTrainData* data，所训练的样本数据
-     data = 0;  
- // const char* default_model_name，该机器学习的模型名称
-     default_model_name = "my_boost_tree";  
- //所有变量的类型都为CvMat*
- // active_vars表示那些对决策树的分叉起到作用的特征属性的索引值，该索引值是只相对于参与训练决策树的那些特征属性
- // active_vars_abs表示那些对决策树的分叉起到作用的特征属性的索引值，但与active_vars变量不同，该索引值是针对所有的特征属性
- // orig_response训练样本的原始响应值，即样本的分类，为-1或1
- // sum_response弱分类器线性组合后的样本输出分类，即强分类器的分类结果
- // weak_eval：弱分类器的分类评估结果，即弱分类器的响应值
- // subsample_mask训练样本集的掩码集，即每一个样本都有一个掩码，当该值为0时，该样本被去掉，训练的时候不使用该样本
- //weights每个训练样本数据的权值，即wi(m)
- // subtree_weights每个决策树所应用的样本权值，它的值虽然也是wi(m)，但样本的顺序与weights不同，即subtree_weights针对的是具体决策树
-     active_vars = active_vars_abs = orig_response = sum_response = weak_eval =  
-         subsample_mask = weights = subtree_weights = 0;  
- //AdaBoost训练
-     train( _train_data, _tflag, _responses, _var_idx, _sample_idx,  
-           _var_type, _missing_mask, _params );  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
CvBoost类的两个构造函数：
CvBoost::CvBoost( const Mat& _train_data, int _tflag,
               const Mat& _responses, const Mat& _var_idx,
               const Mat& _sample_idx, const Mat& _var_type,
               const Mat& _missing_mask,
               CvBoostParams _params )
//_train_data训练的样本数据
//_tflag训练数据的特征属性类型，如果为CV_ROW_SAMPLE，表示样本是以行的形式储存的，即_train_data矩阵的每一行为一个样本（或特征矢量）；如果为CV_COL_SAMPLE，表示样本是以列的形式储存的
//_responses分类的结果，即分类的响应值
//_var_idx标识感兴趣的特征属性，即真正用于训练的那些特征属性，该值的形式与_sample_idx变量相似
//_sample_idx标识感兴趣的样本，即真正用于训练的样本，该值必须是一维矩阵的形式，即矢量的形式，并且类型必须是8UC1、8SU1或者32SC1。如果为8UC1或8SU1，则该值的含义是用掩码的形式表示对应的样本，即0表示不感兴趣的样本，其他数为感兴趣的样本，因此矢量的元素数量必须与训练样本数据_train_data的样本数一致；如果为32SC1，则该值的含义是那些感兴趣的样本的索引，而不感兴趣的样本的索引不在该矢量中出现，因此该矢量的元素数量可以小于或等于_train_data的样本数
//_var_type特征属性的形式，是类的形式还是数值的形式，用掩码的形式表现对应特征属性的形式，0表示为数值的形式，1表示为类的形式。该值必须是一维矩阵，并且元素的数量必须是真正用于训练的那些特征属性的数量加1，多余的一个元素表示的是响应值的形式，即是分类树还是回归树
//_missing_mask缺失的特征属性，用掩码的形式表现对应的特征属性，0表示没有缺失，而且必须与_train_data的矩阵尺寸大小一致
//_params为构建Boosting的参数
{
    // CvSeq* weak，用于保存决策树，即最终的强分类器
    weak = 0;
    // CvDTreeTrainData* data，所训练的样本数据
    data = 0;
    // const char* default_model_name，该机器学习的模型名称
    default_model_name = "my_boost_tree";
    //所有变量的类型都为CvMat*
    // active_vars表示那些对决策树的分叉起到作用的特征属性的索引值，该索引值是只相对于参与训练决策树的那些特征属性
    // active_vars_abs表示那些对决策树的分叉起到作用的特征属性的索引值，但与active_vars变量不同，该索引值是针对所有的特征属性
    // orig_response训练样本的原始响应值，即样本的分类，为-1或1
    // sum_response弱分类器线性组合后的样本输出分类，即强分类器的分类结果
    // weak_eval：弱分类器的分类评估结果，即弱分类器的响应值
    // subsample_mask训练样本集的掩码集，即每一个样本都有一个掩码，当该值为0时，该样本被去掉，训练的时候不使用该样本
    //weights每个训练样本数据的权值，即wi(m)
    // subtree_weights每个决策树所应用的样本权值，它的值虽然也是wi(m)，但样本的顺序与weights不同，即subtree_weights针对的是具体决策树
    active_vars = active_vars_abs = orig_response = sum_response = weak_eval =
        subsample_mask = weights = subtree_weights = 0;
    //AdaBoost训练
    train( _train_data, _tflag, _responses, _var_idx, _sample_idx,
          _var_type, _missing_mask, _params );
}
```



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- CvBoost::CvBoost()  
- {  
-     data = 0;  
-     weak = 0;  
-     default_model_name = "my_boost_tree";  
- 
-     active_vars = active_vars_abs = orig_response = sum_response = weak_eval =  
-         subsample_mask = weights = subtree_weights = 0;  
-     have_active_cat_vars = have_subsample = false;  
- 
-     clear();    //清空一些参数
- }  




```cpp
CvBoost::CvBoost()
{
    data = 0;
    weak = 0;
    default_model_name = "my_boost_tree";

    active_vars = active_vars_abs = orig_response = sum_response = weak_eval =
        subsample_mask = weights = subtree_weights = 0;
    have_active_cat_vars = have_subsample = false;

    clear();    //清空一些参数
}
```
两个train函数：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- bool CvBoost::train( CvMLData* _data,    //训练样本数据
-              CvBoostParams _params,    //构建Boosting的参数
- bool update )  
- //update表示是更新分类器，还是重新创建分类器，默认是false，表示重新创建
- {  
- bool result = false;  
- 
-     CV_FUNCNAME( "CvBoost::train" );  
- 
-     __BEGIN__;  
- //从_data变量中提取各类数据
- const CvMat* values = _data->get_values();  
- const CvMat* response = _data->get_responses();  
- const CvMat* missing = _data->get_missing();  
- const CvMat* var_types = _data->get_var_types();  
- const CvMat* train_sidx = _data->get_train_sample_idx();  
- const CvMat* var_idx = _data->get_var_idx();  
- //调用另一个train函数
-     CV_CALL( result = train( values, CV_ROW_SAMPLE, response, var_idx,  
-         train_sidx, var_types, missing, _params, update ) );  
- 
-     __END__;  
- 
- return result;  
- }  
- 
- bool
- CvBoost::train( const CvMat* _train_data, int _tflag,  
- const CvMat* _responses, const CvMat* _var_idx,  
- const CvMat* _sample_idx, const CvMat* _var_type,  
- const CvMat* _missing_mask,  
-               CvBoostParams _params, bool _update )  
- {  
- bool ok = false;    //算法训练正确与否的标识变量
- //定义一段内存空间
-     CvMemStorage* storage = 0;  
- 
-     CV_FUNCNAME( "CvBoost::train" );  
- 
-     __BEGIN__;  
- 
- int i;  
- //调用CvBoost::set_params()函数，设置并判断Boosting参数的正确性
-     set_params( _params );  
- //释放active_vars和active_vars_abs矩阵变量
-     cvReleaseMat( &active_vars );  
-     cvReleaseMat( &active_vars_abs );  
- 
- if( !_update || !data )    //需要重新建立分类器，或者没有训练样本数据
-     {  
- //调用CvBoost::clear()函数，清空一些参数
-         clear();  
- //实例化CvDTreeTrainData类，并通过set_data函数设置决策树的训练样本数据，倒数第二个参数表示是否共享决策树，最后一个参数表示是否可以添加分类标签，该参数实现了把决策树中用于交叉验证的子集空间用于存储每个决策树的样本权值的目的
-         data = new CvDTreeTrainData( _train_data, _tflag, _responses, _var_idx,  
-             _sample_idx, _var_type, _missing_mask, _params, true, true );  
- //确保Boosting只能用于两类问题
- if( data->get_num_classes() != 2 )  
-             CV_ERROR( CV_StsNotImplemented,  
- "Boosted trees can only be used for 2-class classification." );  
- //创建一块内存空间，用于存储决策树weak
-         CV_CALL( storage = cvCreateMemStorage() );  
- //为AdaBoost创建一个序列
-         weak = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvBoostTree*), storage );  
-         storage = 0;  
-     }  
- else//否则，为训练样本数据赋值
-     {  
-         data->set_data( _train_data, _tflag, _responses, _var_idx,  
-             _sample_idx, _var_type, _missing_mask, _params, true, true, true );  
-     }  
- //如果AdaBoost算法的类型为Logit AdaBoost或Gentle AdaBoost，则生成一个样本响应值的副本
- if ( (_params.boost_type == LOGIT) || (_params.boost_type == GENTLE) )  
-         data->do_responses_copy();  
- //这里是第一次调用CvBoost::update_weights()函数，因此它的作用是初始化权值，该函数在后面有详细的介绍
-     update_weights( 0 );  
- //执行AdaBoost算法，进行M次迭代
- for( i = 0; i < params.weak_count; i++ )  
-     {  
- //定义一个弱分类器，它是用决策树的方法得到的
-         CvBoostTree* tree = new CvBoostTree;  
- //用决策树的方法训练得到一个弱分类器，如果训练失败，则退出迭代循环，CvBoostTree类中的train函数在后面给出详细的介绍，其中参数subsample_mask表示样本数据的掩码，如果样本数据相应位的值为1，则表示该样本可用，如果为0，则不可用
- if( !tree->train( data, subsample_mask, this ) )  
-         {  
- delete tree;    //删除该弱分类器
- break;    //退出循环
-         }  
- //cvCheckArr( get_weak_response());    //检查该弱分类器的输出响应
- //把得到的弱分类器tree添加进强分类器weak队列中
-         cvSeqPush( weak, &tree );  
- //更新权值，以备下次迭代循环之用
-         update_weights( tree );  
- //裁剪去掉那些权值wi(m+1)太小的训练样本数据
-         trim_weights();  
- //计算样本中没有被置1的数量，如果为零，则说明下次迭代将没有训练样本
- if( cvCountNonZero(subsample_mask) == 0 )  
- break;    //退出迭代
-     }  
- 
- if(weak->total > 0)    //得到了若干个弱分类器
-     {  
- //计算active_vars和condensed_idx变量，这两个变量在预测时要用到，该函数的详细介绍在后面
-         get_active_vars(); // recompute active_vars* maps and condensed_idx's in the splits.
-         data->is_classifier = true;    //重新赋值
-         data->free_train_data();    //释放一些矩阵变量
-         ok = true;    //标识变量
-     }  
- else//没有得到弱分类器
-         clear();    //清空一些参数
- 
-     __END__;  
- //得到了Boosting分类器，返回true，否则返回false
- return ok;      
- }  




```cpp
bool CvBoost::train( CvMLData* _data,    //训练样本数据
             CvBoostParams _params,    //构建Boosting的参数
             bool update )
//update表示是更新分类器，还是重新创建分类器，默认是false，表示重新创建
{
    bool result = false;

    CV_FUNCNAME( "CvBoost::train" );

    __BEGIN__;
    //从_data变量中提取各类数据
    const CvMat* values = _data->get_values();
    const CvMat* response = _data->get_responses();
    const CvMat* missing = _data->get_missing();
    const CvMat* var_types = _data->get_var_types();
    const CvMat* train_sidx = _data->get_train_sample_idx();
    const CvMat* var_idx = _data->get_var_idx();
    //调用另一个train函数
    CV_CALL( result = train( values, CV_ROW_SAMPLE, response, var_idx,
        train_sidx, var_types, missing, _params, update ) );

    __END__;

    return result;
}

bool
CvBoost::train( const CvMat* _train_data, int _tflag,
              const CvMat* _responses, const CvMat* _var_idx,
              const CvMat* _sample_idx, const CvMat* _var_type,
              const CvMat* _missing_mask,
              CvBoostParams _params, bool _update )
{
    bool ok = false;    //算法训练正确与否的标识变量
    //定义一段内存空间
    CvMemStorage* storage = 0;

    CV_FUNCNAME( "CvBoost::train" );

    __BEGIN__;

    int i;
    //调用CvBoost::set_params()函数，设置并判断Boosting参数的正确性
    set_params( _params );
    //释放active_vars和active_vars_abs矩阵变量
    cvReleaseMat( &active_vars );
    cvReleaseMat( &active_vars_abs );
  
    if( !_update || !data )    //需要重新建立分类器，或者没有训练样本数据
    {
        //调用CvBoost::clear()函数，清空一些参数
        clear();
        //实例化CvDTreeTrainData类，并通过set_data函数设置决策树的训练样本数据，倒数第二个参数表示是否共享决策树，最后一个参数表示是否可以添加分类标签，该参数实现了把决策树中用于交叉验证的子集空间用于存储每个决策树的样本权值的目的
        data = new CvDTreeTrainData( _train_data, _tflag, _responses, _var_idx,
            _sample_idx, _var_type, _missing_mask, _params, true, true );
        //确保Boosting只能用于两类问题
        if( data->get_num_classes() != 2 )
            CV_ERROR( CV_StsNotImplemented,
            "Boosted trees can only be used for 2-class classification." );
        //创建一块内存空间，用于存储决策树weak
        CV_CALL( storage = cvCreateMemStorage() );
        //为AdaBoost创建一个序列
        weak = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvBoostTree*), storage );
        storage = 0;
    }
    else    //否则，为训练样本数据赋值
    {
        data->set_data( _train_data, _tflag, _responses, _var_idx,
            _sample_idx, _var_type, _missing_mask, _params, true, true, true );
    }
    //如果AdaBoost算法的类型为Logit AdaBoost或Gentle AdaBoost，则生成一个样本响应值的副本
    if ( (_params.boost_type == LOGIT) || (_params.boost_type == GENTLE) )
        data->do_responses_copy();
    //这里是第一次调用CvBoost::update_weights()函数，因此它的作用是初始化权值，该函数在后面有详细的介绍
    update_weights( 0 );
    //执行AdaBoost算法，进行M次迭代
    for( i = 0; i < params.weak_count; i++ )
    {
        //定义一个弱分类器，它是用决策树的方法得到的
        CvBoostTree* tree = new CvBoostTree;
        //用决策树的方法训练得到一个弱分类器，如果训练失败，则退出迭代循环，CvBoostTree类中的train函数在后面给出详细的介绍，其中参数subsample_mask表示样本数据的掩码，如果样本数据相应位的值为1，则表示该样本可用，如果为0，则不可用
        if( !tree->train( data, subsample_mask, this ) )
        {
            delete tree;    //删除该弱分类器
            break;    //退出循环
        }
        //cvCheckArr( get_weak_response());    //检查该弱分类器的输出响应
        //把得到的弱分类器tree添加进强分类器weak队列中
        cvSeqPush( weak, &tree );
        //更新权值，以备下次迭代循环之用
        update_weights( tree );
        //裁剪去掉那些权值wi(m+1)太小的训练样本数据
        trim_weights();
        //计算样本中没有被置1的数量，如果为零，则说明下次迭代将没有训练样本
        if( cvCountNonZero(subsample_mask) == 0 )
            break;    //退出迭代
    }

    if(weak->total > 0)    //得到了若干个弱分类器
    {
        //计算active_vars和condensed_idx变量，这两个变量在预测时要用到，该函数的详细介绍在后面
        get_active_vars(); // recompute active_vars* maps and condensed_idx's in the splits.
        data->is_classifier = true;    //重新赋值
        data->free_train_data();    //释放一些矩阵变量
        ok = true;    //标识变量
    }
    else    //没有得到弱分类器
        clear();    //清空一些参数

    __END__;
    //得到了Boosting分类器，返回true，否则返回false
    return ok;    
}
```
更新权值函数，当要初始化权值时，参数tree为0，表示此时还没有弱分类器：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- void
- CvBoost::update_weights( CvBoostTree* tree ) {  
- double initial_weights[2] = { 1, 1 };    //该变量只有在初始化权值时用到
-   update_weights_impl( tree, initial_weights );  
- }  




```cpp
void
CvBoost::update_weights( CvBoostTree* tree ) {
  double initial_weights[2] = { 1, 1 };    //该变量只有在初始化权值时用到
  update_weights_impl( tree, initial_weights );
}
```
更新权值的核心函数：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- void
- CvBoost::update_weights_impl( CvBoostTree* tree, double initial_weights[2] )  
- {  
-     CV_FUNCNAME( "CvBoost::update_weights_impl" );  
- 
-     __BEGIN__;  
- //n表示训练样本的数量
- int i, n = data->sample_count;  
- double sumw = 0.;    //表示权值的和，权值归一化时要用
- int step = 0;  
- float* fdata = 0;  
- int *sample_idx_buf;  
- constint* sample_idx = 0;  
-     cv::AutoBuffer<uchar> inn_buf;  
- //根据AdaBoost类型的不同，定义不同大小的缓存，Discrete和Real时为0，LogitBoost和Gentle时为所有训练样本的数据长度，存储着样本响应值的副本
- size_t _buf_size = (params.boost_type == LOGIT) || (params.boost_type == GENTLE) ? (size_t)(data->sample_count)*sizeof(int) : 0;  
- if( !tree )    //初始化权值，因为此时还没有弱分类器
-         _buf_size += n*sizeof(int);    //增加_buf_size的大小，增加的长度为所有训练样本的数据长度，存储着样本响应值
- else//训练样本的迭代过程中
-     {  
- //如果发生了样本裁剪，则_buf_size的增加长度为没有被裁剪掉的样本数据长度，即此次迭代中真正用到的那些样本的数据长度，存储的内容为该次迭代的训练样本数据的响应值
- if( have_subsample )  
-             _buf_size += data->get_length_subbuf()*(sizeof(float)+sizeof(uchar));  
-     }  
- //为inn_buf分配_buf_size大小的内存空间
-     inn_buf.allocate(_buf_size);  
- // cur_buf_pos指向inn_buf的空间的首地址
-     uchar* cur_buf_pos = (uchar*)inn_buf;  
- //如果是LogitBoost和Gentle类型
- if ( (params.boost_type == LOGIT) || (params.boost_type == GENTLE) )  
-     {  
- //得到步长
-         step = CV_IS_MAT_CONT(data->responses_copy->type) ?  
-             1 : data->responses_copy->step / CV_ELEM_SIZE(data->responses_copy->type);  
-         fdata = data->responses_copy->data.fl;    //指向样本响应值的副本
-         sample_idx_buf = (int*)cur_buf_pos;    //指向inn_buf空间
- // cur_buf_pos指向此次迭代训练的样本数据的首地址
-         cur_buf_pos = (uchar*)(sample_idx_buf + data->sample_count);  
- //得到训练样本序列索引
-         sample_idx = data->get_sample_indices( data->data_root, sample_idx_buf );  
-     }  
- // dtree_data_buf指向训练数据的内存首地址，训练数据是以矩阵的形式进行存储的
-     CvMat* dtree_data_buf = data->buf;  
- // length_buf_row表示当前训练样本的数量，因为并不是所有的样本都用于本次迭代，有一些样本被剪切掉了
- size_t length_buf_row = data->get_length_subbuf();  
- //进入if的条件是在第一次迭代之前，即还没有弱分类器的时候，它的作用是初始化一些变量和参数
- if( !tree ) // before training the first tree, initialize weights and other parameters
-     {   
- // class_labels_buf表示分类结果，即响应值
- int* class_labels_buf = (int*)cur_buf_pos;  
- // cur_buf_pos指向另一块内存空间
-         cur_buf_pos = (uchar*)(class_labels_buf + n);  
- //得到训练样本的响应值，也就是样本的分类结果
- constint* class_labels = data->get_class_labels(data->data_root, class_labels_buf);  
- // in case of logitboost and gentle adaboost each weak tree is a regression tree,
- // so we need to convert class labels to floating-point values
- //由于LogitBoost和Gentle需要浮点型的数据，构建它们的弱分类器的决策树是回归树，所以需要重新定义一些变量，因为这些变量在初始化时都是整型的
- //w0用于初始化权值wi(1)
- double w0 = 1./n;  
- // p[2] = { 1, 1 }，表示两类问题的先验概率
- double p[2] = { initial_weights[0], initial_weights[1] };  
- //释放、重新分配一些矩阵
-         cvReleaseMat( &orig_response );  
-         cvReleaseMat( &sum_response );  
-         cvReleaseMat( &weak_eval );  
-         cvReleaseMat( &subsample_mask );  
-         cvReleaseMat( &weights );  
-         cvReleaseMat( &subtree_weights );  
- //重新定义一些矩阵
-         CV_CALL( orig_response = cvCreateMat( 1, n, CV_32S ));  
-         CV_CALL( weak_eval = cvCreateMat( 1, n, CV_64F ));  
-         CV_CALL( subsample_mask = cvCreateMat( 1, n, CV_8U ));  
-         CV_CALL( weights = cvCreateMat( 1, n, CV_64F ));  
-         CV_CALL( subtree_weights = cvCreateMat( 1, n + 2, CV_64F ));  
- //如果在设置Boosting参数的时候设置了_priors值，并且AdaBoost算法为Discrete Adaboost或Real AdaBoost，则表示样本属性有先验概率，即have_priors为true，则需要根据训练样本的数量计算先验概率，具体内容见上一篇关于决策树的介绍
- if( data->have_priors )  
-         {  
- // compute weight scale for each class from their prior probabilities
- //根据_priors值计算两类问题的两个分类结果的先验概率
- int c1 = 0;  
- //样本的分类class_labels，即响应值，只能为0或者为1 
- for( i = 0; i < n; i++ )  
-                 c1 += class_labels[i];    //统计响应值为1的样本数量
- //响应值为0的先验概率
-             p[0] = data->priors->data.db[0]*(c1 < n ? 1./(n - c1) : 0.);  
- //响应值为1的先验概率
-             p[1] = data->priors->data.db[1]*(c1 > 0 ? 1./c1 : 0.);  
- //归一化p[0]和p[1]
-             p[0] /= p[0] + p[1];  
-             p[1] = 1. - p[0];  
-         }  
- 
- if (data->is_buf_16u)    //训练样本数量小于65536个，即可以用一个unsigned short数据类型表示该数量
-         {  
- // data->get_cv_labels函数访问的就是labels指向的空间，表示全体样本索引与构建决策树的样本索引的映射关系
-             unsigned short* labels = (unsigned short*)(dtree_data_buf->data.s + data->data_root->buf_idx*length_buf_row +  
-                 data->data_root->offset + (data->work_var_count-1)*data->sample_count);  
- //遍历所有样本数据
- for( i = 0; i < n; i++ )  
-             {  
- // save original categorical responses {0,1}, convert them to {-1,1}
- //原始数据的响应值为0或1，我们要把它转换为-1或1的形式
- // 0 × 2 – 1 = -1；1 × 2 - 1 = 1
-                 orig_response->data.i[i] = class_labels[i]*2 - 1;  
- // make all the samples active at start.
- // later, in trim_weights() deactivate/reactive again some, if need
- //第一次开始训练的时候，所有样本数据都应该被使用，所以样本对应的掩码都为1，表示可以使用，而在训练迭代的过程中，也许会有一些样本通过trim_weights()函数被裁剪掉，这时该样本的掩码就要为0
-                 subsample_mask->data.ptr[i] = (uchar)1;  
- // make all the initial weights the same.
- //初始化每个样本数据的权值，即步骤2。class_labels[i]不是为0就是为1，而在没有先验分类概率的情况下p[0]和p[1]都为1，即前面原理分析中步骤2的每个样本数据的权值为1/n；而在有先验分类概率的情况下，每个样本数据的权值还要根据响应值用先验概率p[0]或p[1]再乘以1/n
-                 weights->data.db[i] = w0*p[class_labels[i]];  
- // set the labels to find (from within weak tree learning proc)
- // the particular sample weight, and where to store the response.
- //顺序存储样本的索引值，即得到一种映射关系
-                 labels[i] = (unsigned short)i;  
-             }  
-         }  
- else//样本数据大于65536个，则需要用一个int数据类型表示
-         {  
- int* labels = dtree_data_buf->data.i + data->data_root->buf_idx*length_buf_row +  
-                 data->data_root->offset + (data->work_var_count-1)*data->sample_count;  
- 
- for( i = 0; i < n; i++ )  
-             {  
- // save original categorical responses {0,1}, convert them to {-1,1}
-                 orig_response->data.i[i] = class_labels[i]*2 - 1;  
- // make all the samples active at start.
- // later, in trim_weights() deactivate/reactive again some, if need
-                 subsample_mask->data.ptr[i] = (uchar)1;  
- // make all the initial weights the same.
-                 weights->data.db[i] = w0*p[class_labels[i]];  
- // set the labels to find (from within weak tree learning proc)
- // the particular sample weight, and where to store the response.
-                 labels[i] = i;  
-             }  
-         }  
- 
- if( params.boost_type == LOGIT )    //LogitBoost类型
-         {  
-             CV_CALL( sum_response = cvCreateMat( 1, n, CV_64F ));    //定义矩阵
- //遍历所有样本，初始化变量
- for( i = 0; i < n; i++ )  
-             {  
-                 sum_response->data.db[i] = 0;    //清零
- //如果样本的响应值为1，则该样本对应的响应值副本为2；如果响应值不为1，则为-2
-                 fdata[sample_idx[i]*step] = orig_response->data.i[i] > 0 ? 2.f : -2.f;  
-             }  
- 
- // in case of logitboost each weak tree is a regression tree.
- // the target function values are recalculated for each of the trees
-             data->is_classifier = false;    // LogitBoost类型应用的是回归树
-         }  
- elseif( params.boost_type == GENTLE )    // Gentle AdaBoost类型
-         {  
- //遍历所有样本，初始化变量
- for( i = 0; i < n; i++ )  
- //样本的响应值副本的值与原始响应值一致
-                 fdata[sample_idx[i]*step] = (float)orig_response->data.i[i];  
- 
-             data->is_classifier = false;    // Gentle AdaBoost类型应用的是回归树
-         }  
-     }  
- else//真正进入循环迭代，更新权值的过程
-     {  
- // at this moment, for all the samples that participated in the training of the most
- // recent weak classifier we know the responses. For other samples we need to compute them
- //在上一次迭代中，如果利用的裁剪后的样本数据，说明有一些样本被裁剪掉而没有它们的弱分类器（即决策树）的响应值，但在后更新权值时，是需要所有的训练样本数据，因此这里我们通过构建好的决策树来预测这些裁剪掉的样本，从而得到它们的响应值
- if( have_subsample )    //有训练样本被掩码裁剪掉
-         {  
- // values表示需要预测的样本数据
- float* values = (float*)cur_buf_pos;  
- // values空间的长度为本次迭代训练样本的实际长度
-             cur_buf_pos = (uchar*)(values + data->get_length_subbuf());  
-             uchar* missing = cur_buf_pos;    //missing表示样本缺失特征属性的掩码
- // cur_buf_pos再次指向一段内存空间
-             cur_buf_pos = missing + data->get_length_subbuf() * (size_t)CV_ELEM_SIZE(data->buf->type);  
- 
-             CvMat _sample, _mask;  
- 
- // invert the subsample mask
- //保留的样本掩码为1，裁剪掉的为0，现在反转掩码，即保留的为0，裁剪掉的为1
-             cvXorS( subsample_mask, cvScalar(1.), subsample_mask );  
- //得到裁剪掉的样本数据
-             data->get_vectors( subsample_mask, values, missing, 0 );  
- //定义矩阵
-             _sample = cvMat( 1, data->var_count, CV_32F );    //表示裁剪掉的样本
-             _mask = cvMat( 1, data->var_count, CV_8U );    //表示对应样本所缺失的特征
- 
- // run tree through all the non-processed samples
- //遍历所有样本，但只处理那些被裁剪掉的样本
- for( i = 0; i < n; i++ )  
- if( subsample_mask->data.ptr[i] )    //被裁剪掉的样本数据
-                 {  
-                     _sample.data.fl = values;    //赋值
-                     _mask.data.ptr = missing;    //赋值
-                     values += _sample.cols;    //指向下一个样本
-                     missing += _mask.cols;    //指向下一个样本
- //通过预测得到被裁剪掉的样本的弱分类器的响应值
-                     weak_eval->data.db[i] = tree->predict( &_sample, &_mask, true )->value;  
-                 }  
-         }  
- 
- // now update weights and other parameters for each type of boosting
- //根据不同类型更新权值
- if( params.boost_type == DISCRETE )    //Discrete Adaboost类型
-         {  
- // Discrete AdaBoost:
- //   weak_eval[i] (=f(x_i)) is in {-1,1}
- //   err = sum(w_i*(f(x_i) != y_i))/sum(w_i)
- //   C = log((1-err)/err)
- //   w_i *= exp(C*(f(x_i) != y_i))
- 
- double C, err = 0.;  
- double scale[] = { 1., 0. };  
- //遍历所有样本数据
- for( i = 0; i < n; i++ )  
-             {  
- //得到上次迭代后每个训练样本数据的权值，即wi(m)
- double w = weights->data.db[i];  
- //权值之和，即式9中的分母部分
-                 sumw += w;  
- //分类器分类错误的那些训练样本数据的权值之和，即式9中的分子部分
- // weak_eval表示该次迭代的弱分类器的分类结果，orig_response为实际的分类结果，weak_eval->data.db[i] != orig_response->data.i[i]的作用是判断分类是否正确，正确为0，错误为1
-                 err += w*(weak_eval->data.db[i] != orig_response->data.i[i]);  
-             }  
- //得到误差率，即式9
- if( sumw != 0 )  
-                 err /= sumw;  
- //log_ratio(x)为内嵌函数，该函数的输出为ln[x/(1-x)]
- //C和err的值为式13，C表示该次迭代得到的弱分类器的权值，即αm
-             C = err = -log_ratio( err );  
- // scale[1]为式10中括号内的部分，该值肯定大于1，而此时scale[0]仍然为1
-             scale[1] = exp(err);  
- 
-             sumw = 0;  
- //再次遍历训练样本数据，更新样本权值wi(m+1)
- for( i = 0; i < n; i++ )  
-             {  
- //分类正确为scale[0]，分类错误为scale[1]，即式14
- double w = weights->data.db[i]*  
-                     scale[weak_eval->data.db[i] != orig_response->data.i[i]];  
-                 sumw += w;    //权值之和，后面归一化要用
-                 weights->data.db[i] = w;    //赋值
-             }  
- //为弱分类器的权值赋值，即该决策树tree的每节点值都乘以C，也就是得到了式12的αmkm(x)
-             tree->scale( C );  
-         }  
- elseif( params.boost_type == REAL )    // Real AdaBoost类型
-         {  
- // Real AdaBoost:
- //   weak_eval[i] = f(x_i) = 0.5*log(p(x_i)/(1-p(x_i))), p(x_i)=P(y=1|x_i)
- //   w_i *= exp(-y_i*f(x_i))
- //weak_eval变量在计算前为弱分类器km(x)，但在遍历循环后被赋予新的含义，为式16中的e指数中的指数部分
- for( i = 0; i < n; i++ )  
-                 weak_eval->data.db[i] *= -orig_response->data.i[i];  
- //计算式16的e指数
-             cvExp( weak_eval, weak_eval );  
- //遍历训练样本数据，更新权值wi(m)
- for( i = 0; i < n; i++ )  
-             {  
- //式16，得到权值wi(m+1)
- double w = weights->data.db[i]*weak_eval->data.db[i];  
-                 sumw += w;    //权值之和，后面归一化要用
-                 weights->data.db[i] = w;    //赋值
-             }  
-         }  
- elseif( params.boost_type == LOGIT )     //LogitBoost类型
-         {  
- // LogitBoost:
- //   weak_eval[i] = f(x_i) in [-z_max,z_max]
- //   sum_response = F(x_i).
- //   F(x_i) += 0.5*f(x_i)
- //   p(x_i) = exp(F(x_i))/(exp(F(x_i)) + exp(-F(x_i))=1/(1+exp(-2*F(x_i)))
- //   reuse weak_eval: weak_eval[i] <- p(x_i)
- //   w_i = p(x_i)*1(1 - p(x_i))
- //   z_i = ((y_i+1)/2 - p(x_i))/(p(x_i)*(1 - p(x_i)))
- //   store z_i to the data->data_root as the new target responses
- 
- constdouble lb_weight_thresh = FLT_EPSILON;  
- constdouble lb_z_max = 10.;  
- /*float* responses_buf = data->get_resp_float_buf();
-             const float* responses = 0;
-             data->get_ord_responses(data->data_root, responses_buf, &responses);*/
- 
- /*if( weak->total == 7 )
-                 putchar('*');*/
- 
- for( i = 0; i < n; i++ )  
-             {  
- //式22
- double s = sum_response->data.db[i] + 0.5*weak_eval->data.db[i];  
-                 sum_response->data.db[i] = s;    //赋值
-                 weak_eval->data.db[i] = -2*s;    //该变量式21要用
-             }  
- //计算e指数
-             cvExp( weak_eval, weak_eval );  
- //遍历所有训练样本数据
- for( i = 0; i < n; i++ )  
-             {  
- //式21，得到更新后的pm(xi)
- double p = 1./(1. + weak_eval->data.db[i]);  
- //w为权值wi(m)，式20
- double w = p*(1 - p), z;  
- //确保w是具有一定意义的正数
-                 w = MAX( w, lb_weight_thresh );  
-                 weights->data.db[i] = w;    //权值赋值
-                 sumw += w;    //权值之和，后面归一化要用
- //计算工作响应zi(m)
- if( orig_response->data.i[i] > 0 )    //样本数据属于y = 1的一类
-                 {  
-                     z = 1./p;  
-                     fdata[sample_idx[i]*step] = (float)MIN(z, lb_z_max);  
-                 }  
- else//样本数据属于y = -1的一类
-                 {  
-                     z = 1./(1-p);  
-                     fdata[sample_idx[i]*step] = (float)-MIN(z, lb_z_max);  
-                 }  
-             }  
-         }  
- else//Gentle AdaBoost类型
-         {  
- // Gentle AdaBoost:
- //   weak_eval[i] = f(x_i) in [-1,1]
- //   w_i *= exp(-y_i*f(x_i))
- //确保为Gentle AdaBoost类型
-             assert( params.boost_type == GENTLE );  
- //计算式24中e指数中的指数部分
- for( i = 0; i < n; i++ )  
-                 weak_eval->data.db[i] *= -orig_response->data.i[i];  
- //计算式24中的e指数
-             cvExp( weak_eval, weak_eval );  
- 
- for( i = 0; i < n; i++ )  
-             {  
- //式24，更新权值
- double w = weights->data.db[i] * weak_eval->data.db[i];  
-                 weights->data.db[i] = w;    //赋值
-                 sumw += w;    //权值之和，后面归一化要用
-             }  
-         }  
-     }  
- 
- // renormalize weights
- //归一化权值wi(m+1)
- if( sumw > FLT_EPSILON )  
-     {  
-         sumw = 1./sumw;  
- for( i = 0; i < n; ++i )  
-             weights->data.db[i] *= sumw;  
-     }  
- 
-     __END__;  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
void
CvBoost::update_weights_impl( CvBoostTree* tree, double initial_weights[2] )
{
    CV_FUNCNAME( "CvBoost::update_weights_impl" );

    __BEGIN__;
    //n表示训练样本的数量
    int i, n = data->sample_count;
    double sumw = 0.;    //表示权值的和，权值归一化时要用
    int step = 0;
    float* fdata = 0;
    int *sample_idx_buf;
    const int* sample_idx = 0;
    cv::AutoBuffer<uchar> inn_buf;
    //根据AdaBoost类型的不同，定义不同大小的缓存，Discrete和Real时为0，LogitBoost和Gentle时为所有训练样本的数据长度，存储着样本响应值的副本
    size_t _buf_size = (params.boost_type == LOGIT) || (params.boost_type == GENTLE) ? (size_t)(data->sample_count)*sizeof(int) : 0;
    if( !tree )    //初始化权值，因为此时还没有弱分类器
        _buf_size += n*sizeof(int);    //增加_buf_size的大小，增加的长度为所有训练样本的数据长度，存储着样本响应值
    else    //训练样本的迭代过程中
    {
        //如果发生了样本裁剪，则_buf_size的增加长度为没有被裁剪掉的样本数据长度，即此次迭代中真正用到的那些样本的数据长度，存储的内容为该次迭代的训练样本数据的响应值
        if( have_subsample )
            _buf_size += data->get_length_subbuf()*(sizeof(float)+sizeof(uchar));
    }
    //为inn_buf分配_buf_size大小的内存空间
    inn_buf.allocate(_buf_size);
    // cur_buf_pos指向inn_buf的空间的首地址
    uchar* cur_buf_pos = (uchar*)inn_buf;
    //如果是LogitBoost和Gentle类型
    if ( (params.boost_type == LOGIT) || (params.boost_type == GENTLE) )
    {
        //得到步长
        step = CV_IS_MAT_CONT(data->responses_copy->type) ?
            1 : data->responses_copy->step / CV_ELEM_SIZE(data->responses_copy->type);
        fdata = data->responses_copy->data.fl;    //指向样本响应值的副本
        sample_idx_buf = (int*)cur_buf_pos;    //指向inn_buf空间
        // cur_buf_pos指向此次迭代训练的样本数据的首地址
        cur_buf_pos = (uchar*)(sample_idx_buf + data->sample_count);
        //得到训练样本序列索引
        sample_idx = data->get_sample_indices( data->data_root, sample_idx_buf );
    }
    // dtree_data_buf指向训练数据的内存首地址，训练数据是以矩阵的形式进行存储的
    CvMat* dtree_data_buf = data->buf;
    // length_buf_row表示当前训练样本的数量，因为并不是所有的样本都用于本次迭代，有一些样本被剪切掉了
    size_t length_buf_row = data->get_length_subbuf();
    //进入if的条件是在第一次迭代之前，即还没有弱分类器的时候，它的作用是初始化一些变量和参数
    if( !tree ) // before training the first tree, initialize weights and other parameters
    { 
        // class_labels_buf表示分类结果，即响应值
        int* class_labels_buf = (int*)cur_buf_pos;
        // cur_buf_pos指向另一块内存空间
        cur_buf_pos = (uchar*)(class_labels_buf + n);
        //得到训练样本的响应值，也就是样本的分类结果
        const int* class_labels = data->get_class_labels(data->data_root, class_labels_buf);
        // in case of logitboost and gentle adaboost each weak tree is a regression tree,
        // so we need to convert class labels to floating-point values
        //由于LogitBoost和Gentle需要浮点型的数据，构建它们的弱分类器的决策树是回归树，所以需要重新定义一些变量，因为这些变量在初始化时都是整型的
        //w0用于初始化权值wi(1)
        double w0 = 1./n;
        // p[2] = { 1, 1 }，表示两类问题的先验概率
        double p[2] = { initial_weights[0], initial_weights[1] };
        //释放、重新分配一些矩阵
        cvReleaseMat( &orig_response );
        cvReleaseMat( &sum_response );
        cvReleaseMat( &weak_eval );
        cvReleaseMat( &subsample_mask );
        cvReleaseMat( &weights );
        cvReleaseMat( &subtree_weights );
        //重新定义一些矩阵
        CV_CALL( orig_response = cvCreateMat( 1, n, CV_32S ));
        CV_CALL( weak_eval = cvCreateMat( 1, n, CV_64F ));
        CV_CALL( subsample_mask = cvCreateMat( 1, n, CV_8U ));
        CV_CALL( weights = cvCreateMat( 1, n, CV_64F ));
        CV_CALL( subtree_weights = cvCreateMat( 1, n + 2, CV_64F ));
        //如果在设置Boosting参数的时候设置了_priors值，并且AdaBoost算法为Discrete Adaboost或Real AdaBoost，则表示样本属性有先验概率，即have_priors为true，则需要根据训练样本的数量计算先验概率，具体内容见上一篇关于决策树的介绍
        if( data->have_priors )
        {
            // compute weight scale for each class from their prior probabilities
            //根据_priors值计算两类问题的两个分类结果的先验概率
            int c1 = 0;
            //样本的分类class_labels，即响应值，只能为0或者为1 
            for( i = 0; i < n; i++ )
                c1 += class_labels[i];    //统计响应值为1的样本数量
            //响应值为0的先验概率
            p[0] = data->priors->data.db[0]*(c1 < n ? 1./(n - c1) : 0.);
            //响应值为1的先验概率
            p[1] = data->priors->data.db[1]*(c1 > 0 ? 1./c1 : 0.);
            //归一化p[0]和p[1]
            p[0] /= p[0] + p[1];
            p[1] = 1. - p[0];
        }

        if (data->is_buf_16u)    //训练样本数量小于65536个，即可以用一个unsigned short数据类型表示该数量
        {
            // data->get_cv_labels函数访问的就是labels指向的空间，表示全体样本索引与构建决策树的样本索引的映射关系
            unsigned short* labels = (unsigned short*)(dtree_data_buf->data.s + data->data_root->buf_idx*length_buf_row +
                data->data_root->offset + (data->work_var_count-1)*data->sample_count);
            //遍历所有样本数据
            for( i = 0; i < n; i++ )
            {
                // save original categorical responses {0,1}, convert them to {-1,1}
                //原始数据的响应值为0或1，我们要把它转换为-1或1的形式
                // 0 × 2 – 1 = -1；1 × 2 - 1 = 1
                orig_response->data.i[i] = class_labels[i]*2 - 1;
                // make all the samples active at start.
                // later, in trim_weights() deactivate/reactive again some, if need
                //第一次开始训练的时候，所有样本数据都应该被使用，所以样本对应的掩码都为1，表示可以使用，而在训练迭代的过程中，也许会有一些样本通过trim_weights()函数被裁剪掉，这时该样本的掩码就要为0
                subsample_mask->data.ptr[i] = (uchar)1;
                // make all the initial weights the same.
                //初始化每个样本数据的权值，即步骤2。class_labels[i]不是为0就是为1，而在没有先验分类概率的情况下p[0]和p[1]都为1，即前面原理分析中步骤2的每个样本数据的权值为1/n；而在有先验分类概率的情况下，每个样本数据的权值还要根据响应值用先验概率p[0]或p[1]再乘以1/n
                weights->data.db[i] = w0*p[class_labels[i]];
                // set the labels to find (from within weak tree learning proc)
                // the particular sample weight, and where to store the response.
                //顺序存储样本的索引值，即得到一种映射关系
                labels[i] = (unsigned short)i;
            }
        }
        else    //样本数据大于65536个，则需要用一个int数据类型表示
        {
            int* labels = dtree_data_buf->data.i + data->data_root->buf_idx*length_buf_row +
                data->data_root->offset + (data->work_var_count-1)*data->sample_count;

            for( i = 0; i < n; i++ )
            {
                // save original categorical responses {0,1}, convert them to {-1,1}
                orig_response->data.i[i] = class_labels[i]*2 - 1;
                // make all the samples active at start.
                // later, in trim_weights() deactivate/reactive again some, if need
                subsample_mask->data.ptr[i] = (uchar)1;
                // make all the initial weights the same.
                weights->data.db[i] = w0*p[class_labels[i]];
                // set the labels to find (from within weak tree learning proc)
                // the particular sample weight, and where to store the response.
                labels[i] = i;
            }
        }

        if( params.boost_type == LOGIT )    //LogitBoost类型
        {
            CV_CALL( sum_response = cvCreateMat( 1, n, CV_64F ));    //定义矩阵
            //遍历所有样本，初始化变量
            for( i = 0; i < n; i++ )
            {
                sum_response->data.db[i] = 0;    //清零
                //如果样本的响应值为1，则该样本对应的响应值副本为2；如果响应值不为1，则为-2
                fdata[sample_idx[i]*step] = orig_response->data.i[i] > 0 ? 2.f : -2.f;
            }

            // in case of logitboost each weak tree is a regression tree.
            // the target function values are recalculated for each of the trees
            data->is_classifier = false;    // LogitBoost类型应用的是回归树
        }
        else if( params.boost_type == GENTLE )    // Gentle AdaBoost类型
        {
            //遍历所有样本，初始化变量
            for( i = 0; i < n; i++ )
                //样本的响应值副本的值与原始响应值一致
                fdata[sample_idx[i]*step] = (float)orig_response->data.i[i];

            data->is_classifier = false;    // Gentle AdaBoost类型应用的是回归树
        }
    }
    else    //真正进入循环迭代，更新权值的过程
    {
        // at this moment, for all the samples that participated in the training of the most
        // recent weak classifier we know the responses. For other samples we need to compute them
        //在上一次迭代中，如果利用的裁剪后的样本数据，说明有一些样本被裁剪掉而没有它们的弱分类器（即决策树）的响应值，但在后更新权值时，是需要所有的训练样本数据，因此这里我们通过构建好的决策树来预测这些裁剪掉的样本，从而得到它们的响应值
        if( have_subsample )    //有训练样本被掩码裁剪掉
        {
            // values表示需要预测的样本数据
            float* values = (float*)cur_buf_pos;
            // values空间的长度为本次迭代训练样本的实际长度
            cur_buf_pos = (uchar*)(values + data->get_length_subbuf());
            uchar* missing = cur_buf_pos;    //missing表示样本缺失特征属性的掩码
            // cur_buf_pos再次指向一段内存空间
            cur_buf_pos = missing + data->get_length_subbuf() * (size_t)CV_ELEM_SIZE(data->buf->type);

            CvMat _sample, _mask;

            // invert the subsample mask
            //保留的样本掩码为1，裁剪掉的为0，现在反转掩码，即保留的为0，裁剪掉的为1
            cvXorS( subsample_mask, cvScalar(1.), subsample_mask );
            //得到裁剪掉的样本数据
            data->get_vectors( subsample_mask, values, missing, 0 );
            //定义矩阵
            _sample = cvMat( 1, data->var_count, CV_32F );    //表示裁剪掉的样本
            _mask = cvMat( 1, data->var_count, CV_8U );    //表示对应样本所缺失的特征

            // run tree through all the non-processed samples
            //遍历所有样本，但只处理那些被裁剪掉的样本
            for( i = 0; i < n; i++ )
                if( subsample_mask->data.ptr[i] )    //被裁剪掉的样本数据
                {
                    _sample.data.fl = values;    //赋值
                    _mask.data.ptr = missing;    //赋值
                    values += _sample.cols;    //指向下一个样本
                    missing += _mask.cols;    //指向下一个样本
                    //通过预测得到被裁剪掉的样本的弱分类器的响应值
                    weak_eval->data.db[i] = tree->predict( &_sample, &_mask, true )->value;
                }
        }

        // now update weights and other parameters for each type of boosting
        //根据不同类型更新权值
        if( params.boost_type == DISCRETE )    //Discrete Adaboost类型
        {
            // Discrete AdaBoost:
            //   weak_eval[i] (=f(x_i)) is in {-1,1}
            //   err = sum(w_i*(f(x_i) != y_i))/sum(w_i)
            //   C = log((1-err)/err)
            //   w_i *= exp(C*(f(x_i) != y_i))

            double C, err = 0.;
            double scale[] = { 1., 0. };
            //遍历所有样本数据
            for( i = 0; i < n; i++ )
            {
                //得到上次迭代后每个训练样本数据的权值，即wi(m)
                double w = weights->data.db[i];
                //权值之和，即式9中的分母部分
                sumw += w;
                //分类器分类错误的那些训练样本数据的权值之和，即式9中的分子部分
                // weak_eval表示该次迭代的弱分类器的分类结果，orig_response为实际的分类结果，weak_eval->data.db[i] != orig_response->data.i[i]的作用是判断分类是否正确，正确为0，错误为1
                err += w*(weak_eval->data.db[i] != orig_response->data.i[i]);
            }
            //得到误差率，即式9
            if( sumw != 0 )
                err /= sumw;
            //log_ratio(x)为内嵌函数，该函数的输出为ln[x/(1-x)]
            //C和err的值为式13，C表示该次迭代得到的弱分类器的权值，即αm
            C = err = -log_ratio( err );
            // scale[1]为式10中括号内的部分，该值肯定大于1，而此时scale[0]仍然为1
            scale[1] = exp(err);

            sumw = 0;
            //再次遍历训练样本数据，更新样本权值wi(m+1)
            for( i = 0; i < n; i++ )
            {
                //分类正确为scale[0]，分类错误为scale[1]，即式14
                double w = weights->data.db[i]*
                    scale[weak_eval->data.db[i] != orig_response->data.i[i]];
                sumw += w;    //权值之和，后面归一化要用
                weights->data.db[i] = w;    //赋值
            }
            //为弱分类器的权值赋值，即该决策树tree的每节点值都乘以C，也就是得到了式12的αmkm(x)
            tree->scale( C );
        }
        else if( params.boost_type == REAL )    // Real AdaBoost类型
        {
            // Real AdaBoost:
            //   weak_eval[i] = f(x_i) = 0.5*log(p(x_i)/(1-p(x_i))), p(x_i)=P(y=1|x_i)
            //   w_i *= exp(-y_i*f(x_i))
            //weak_eval变量在计算前为弱分类器km(x)，但在遍历循环后被赋予新的含义，为式16中的e指数中的指数部分
            for( i = 0; i < n; i++ )
                weak_eval->data.db[i] *= -orig_response->data.i[i];
            //计算式16的e指数
            cvExp( weak_eval, weak_eval );
            //遍历训练样本数据，更新权值wi(m)
            for( i = 0; i < n; i++ )
            {
                //式16，得到权值wi(m+1)
                double w = weights->data.db[i]*weak_eval->data.db[i];
                sumw += w;    //权值之和，后面归一化要用
                weights->data.db[i] = w;    //赋值
            }
        }
        else if( params.boost_type == LOGIT )     //LogitBoost类型
        {
            // LogitBoost:
            //   weak_eval[i] = f(x_i) in [-z_max,z_max]
            //   sum_response = F(x_i).
            //   F(x_i) += 0.5*f(x_i)
            //   p(x_i) = exp(F(x_i))/(exp(F(x_i)) + exp(-F(x_i))=1/(1+exp(-2*F(x_i)))
            //   reuse weak_eval: weak_eval[i] <- p(x_i)
            //   w_i = p(x_i)*1(1 - p(x_i))
            //   z_i = ((y_i+1)/2 - p(x_i))/(p(x_i)*(1 - p(x_i)))
            //   store z_i to the data->data_root as the new target responses

            const double lb_weight_thresh = FLT_EPSILON;
            const double lb_z_max = 10.;
            /*float* responses_buf = data->get_resp_float_buf();
            const float* responses = 0;
            data->get_ord_responses(data->data_root, responses_buf, &responses);*/

            /*if( weak->total == 7 )
                putchar('*');*/

            for( i = 0; i < n; i++ )
            {
                //式22
                double s = sum_response->data.db[i] + 0.5*weak_eval->data.db[i];
                sum_response->data.db[i] = s;    //赋值
                weak_eval->data.db[i] = -2*s;    //该变量式21要用
            }
            //计算e指数
            cvExp( weak_eval, weak_eval );
            //遍历所有训练样本数据
            for( i = 0; i < n; i++ )
            {
                //式21，得到更新后的pm(xi)
                double p = 1./(1. + weak_eval->data.db[i]);
                //w为权值wi(m)，式20
                double w = p*(1 - p), z;
                //确保w是具有一定意义的正数
                w = MAX( w, lb_weight_thresh );
                weights->data.db[i] = w;    //权值赋值
                sumw += w;    //权值之和，后面归一化要用
                //计算工作响应zi(m)
                if( orig_response->data.i[i] > 0 )    //样本数据属于y = 1的一类
                {
                    z = 1./p;
                    fdata[sample_idx[i]*step] = (float)MIN(z, lb_z_max);
                }
                else    //样本数据属于y = -1的一类
                {
                    z = 1./(1-p);
                    fdata[sample_idx[i]*step] = (float)-MIN(z, lb_z_max);
                }
            }
        }
        else    //Gentle AdaBoost类型
        {
            // Gentle AdaBoost:
            //   weak_eval[i] = f(x_i) in [-1,1]
            //   w_i *= exp(-y_i*f(x_i))
            //确保为Gentle AdaBoost类型
            assert( params.boost_type == GENTLE );
            //计算式24中e指数中的指数部分
            for( i = 0; i < n; i++ )
                weak_eval->data.db[i] *= -orig_response->data.i[i];
            //计算式24中的e指数
            cvExp( weak_eval, weak_eval );

            for( i = 0; i < n; i++ )
            {
                //式24，更新权值
                double w = weights->data.db[i] * weak_eval->data.db[i];
                weights->data.db[i] = w;    //赋值
                sumw += w;    //权值之和，后面归一化要用
            }
        }
    }

    // renormalize weights
    //归一化权值wi(m+1)
    if( sumw > FLT_EPSILON )
    {
        sumw = 1./sumw;
        for( i = 0; i < n; ++i )
            weights->data.db[i] *= sumw;
    }

    __END__;
}
```
裁剪去掉那些权值过小的样本数据：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- void
- CvBoost::trim_weights()  
- {  
- //CV_FUNCNAME( "CvBoost::trim_weights" );
- 
-     __BEGIN__;  
- //count为训练样本总数，nz_count表示裁剪以后保留下来的样本总数
- int i, count = data->sample_count, nz_count = 0;  
- double sum, threshold;  
- //裁剪率weight_trim_rate要在0和1之间
- if( params.weight_trim_rate <= 0. || params.weight_trim_rate >= 1. )  
-         EXIT;  
- 
- // use weak_eval as temporary buffer for sorted weights
- //样本权值weights暂时保存到weak_eval中，以便用于对其进行排序
-     cvCopy( weights, weak_eval );  
- //对样本权值按由小到大的顺序进行排序
-     icvSort_64f( weak_eval->data.db, count, 0 );  
- 
- // as weight trimming occurs immediately after updating the weights,
- // where they are renormalized, we assume that the weight sum = 1.
-     sum = 1. - params.weight_trim_rate;  
- //由小到大遍历所有训练样本，找到裁剪率所对应的那个权值
- for( i = 0; i < count; i++ )  
-     {  
- double w = weak_eval->data.db[i];  
- if( sum <= 0 )  
- break;  
-         sum -= w;  
-     }  
- //得到阈值
-     threshold = i < count ? weak_eval->data.db[i] : DBL_MAX;  
- //再次遍历所有训练样本（这一次不是按照大小顺序遍历），确定哪些样本被裁剪掉
- for( i = 0; i < count; i++ )  
-     {  
- double w = weights->data.db[i];    //得到权值
- //权值与阈值比较，权值大则f为1，反之f为0
- int f = w >= threshold;  
- //把权值大于阈值的样本所对应的掩码为置1，反之清0
-         subsample_mask->data.ptr[i] = (uchar)f;  
-         nz_count += f;    //计数
-     }  
- // nz_count < count表示有裁剪掉的样本，此时have_subsample为1，反之为0
-     have_subsample = nz_count < count;  
- 
-     __END__;  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
void
CvBoost::trim_weights()
{
    //CV_FUNCNAME( "CvBoost::trim_weights" );

    __BEGIN__;
    //count为训练样本总数，nz_count表示裁剪以后保留下来的样本总数
    int i, count = data->sample_count, nz_count = 0;
    double sum, threshold;
    //裁剪率weight_trim_rate要在0和1之间
    if( params.weight_trim_rate <= 0. || params.weight_trim_rate >= 1. )
        EXIT;

    // use weak_eval as temporary buffer for sorted weights
    //样本权值weights暂时保存到weak_eval中，以便用于对其进行排序
    cvCopy( weights, weak_eval );
    //对样本权值按由小到大的顺序进行排序
    icvSort_64f( weak_eval->data.db, count, 0 );

    // as weight trimming occurs immediately after updating the weights,
    // where they are renormalized, we assume that the weight sum = 1.
    sum = 1. - params.weight_trim_rate;
    //由小到大遍历所有训练样本，找到裁剪率所对应的那个权值
    for( i = 0; i < count; i++ )
    {
        double w = weak_eval->data.db[i];
        if( sum <= 0 )
            break;
        sum -= w;
    }
    //得到阈值
    threshold = i < count ? weak_eval->data.db[i] : DBL_MAX;
    //再次遍历所有训练样本（这一次不是按照大小顺序遍历），确定哪些样本被裁剪掉
    for( i = 0; i < count; i++ )
    {
        double w = weights->data.db[i];    //得到权值
        //权值与阈值比较，权值大则f为1，反之f为0
        int f = w >= threshold;
        //把权值大于阈值的样本所对应的掩码为置1，反之清0
        subsample_mask->data.ptr[i] = (uchar)f;
        nz_count += f;    //计数
    }
    // nz_count < count表示有裁剪掉的样本，此时have_subsample为1，反之为0
    have_subsample = nz_count < count;

    __END__;
}
```
计算active_vars和active_vars_abs值，这两个值的含义都是记录那些对决策树的分叉起到作用的特征属性的索引值，active_vars存储的索引值是只相对于参与训练决策树的那些特征属性，而active_vars_abs存储的索引值是针对所有的特征属性，包括那些被掩码掉的特征属性



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- const CvMat*  
- CvBoost::get_active_vars( bool absolute_idx )  
- {  
-     CvMat* mask = 0;  
-     CvMat* inv_map = 0;  
-     CvMat* result = 0;  
- 
-     CV_FUNCNAME( "CvBoost::get_active_vars" );  
- 
-     __BEGIN__;  
- 
- if( !weak )    //确保已得到分类器
-         CV_ERROR( CV_StsError, "The boosted tree ensemble has not been trained yet" );  
- //还没有为active_vars和active_vars_abs变量赋值
- if( !active_vars || !active_vars_abs )  
-     {  
-         CvSeqReader reader;  
- int i, j, nactive_vars;  
-         CvBoostTree* wtree;  
- const CvDTreeNode* node;  
- 
-         assert(!active_vars && !active_vars_abs);  
-         mask = cvCreateMat( 1, data->var_count, CV_8U );  
-         inv_map = cvCreateMat( 1, data->var_count, CV_32S );  
-         cvZero( mask );    //mask矩阵清零
-         cvSet( inv_map, cvScalar(-1) );    //inv_map矩阵赋值为-1
- 
- // first pass: compute the mask of used variables
- //把强分类器序列weak保存到reader变量中
-         cvStartReadSeq( weak, &reader );  
- //遍历所有的弱分类器
- for( i = 0; i < weak->total; i++ )  
-         {  
-             CV_READ_SEQ_ELEM(wtree, reader);    //得到一个弱分类器，即决策树
- 
-             node = wtree->get_root();    //得到决策树的根节点
-             assert( node != 0 );    //确保根节点不为零，即该决策树存在
- for(;;)    //遍历决策树的各个节点
-             {  
- const CvDTreeNode* parent;    //定义父节点
- for(;;)    沿着左分子向叶节点遍历  
-                 {  
-                     CvDTreeSplit* split = node->split;    //分叉属性
- //遍历该节点的所有分叉属性，包括最佳分叉属性和替代分叉属性
- for( ; split != 0; split = split->next )  
-                         mask->data.ptr[split->var_idx] = 1;    //分叉属性相应位置1
- if( !node->left )    //到达了叶节点，则退出for死循环
- break;  
-                     node = node->left;    //下一个左分支
-                 }  
- //从叶节点沿着右分支向父节点遍历
- for( parent = node->parent; parent && parent->right == node;  
-                     node = parent, parent = parent->parent )  
-                     ;  
- 
- if( !parent )    //到达了根节点，则退出for死循环
- break;  
- 
-                 node = parent->right;    //下一个右分支
-             }  
-         }  
- 
-         nactive_vars = cvCountNonZero(mask);    //统计mask中非0元素的数量
- 
- //if ( nactive_vars > 0 )
-         {  
- //定义两个矩阵
-             active_vars = cvCreateMat( 1, nactive_vars, CV_32S );  
-             active_vars_abs = cvCreateMat( 1, nactive_vars, CV_32S );  
- 
-             have_active_cat_vars = false;    //标识赋值
- //遍历所有特征属性
- for( i = j = 0; i < data->var_count; i++ )  
-             {  
- if( mask->data.ptr[i] )  
-                 {  
- //赋值为特征属性的索引
-                     active_vars->data.i[j] = i;  
-                     active_vars_abs->data.i[j] = data->var_idx ? data->var_idx->data.i[i] : i;  
- // inv_map存储的形式与active_vars相反，即反映射
-                     inv_map->data.i[i] = j;  
- //如果这些特征属性中只要有一个特征属性是类的形式，则have_active_cat_vars变量就为true
- if( data->var_type->data.i[i] >= 0 )  
-                         have_active_cat_vars = true;  
-                     j++;    //计数
-                 }  
-             }  
- 
- 
- // second pass: now compute the condensed indices
-             cvStartReadSeq( weak, &reader );  
- for( i = 0; i < weak->total; i++ )    //再次遍历所有的弱分类器
-             {  
-                 CV_READ_SEQ_ELEM(wtree, reader);  
-                 node = wtree->get_root();  
- for(;;)    //再次遍历决策树
-                 {  
- const CvDTreeNode* parent;  
- for(;;)  
-                     {  
-                         CvDTreeSplit* split = node->split;  
- for( ; split != 0; split = split->next )  
-                         {  
- // condensed_idx的值为该分叉属性在active_vars的索引，该索引值也是样本数据中特征属性分布排序的索引
-                             split->condensed_idx = inv_map->data.i[split->var_idx];  
-                             assert( split->condensed_idx >= 0 );  
-                         }  
- 
- if( !node->left )  
- break;  
-                         node = node->left;  
-                     }  
- 
- for( parent = node->parent; parent && parent->right == node;  
-                         node = parent, parent = parent->parent )  
-                         ;  
- 
- if( !parent )  
- break;  
- 
-                     node = parent->right;  
-                 }  
-             }  
-         }  
-     }  
- //行参absolute_idx为true，则该函数返回active_vars_abs，否则返回active_vars
-     result = absolute_idx ? active_vars_abs : active_vars;  
- 
-     __END__;  
- 
-     cvReleaseMat( &mask );  
-     cvReleaseMat( &inv_map );  
- 
- return result;  
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
const CvMat*
CvBoost::get_active_vars( bool absolute_idx )
{
    CvMat* mask = 0;
    CvMat* inv_map = 0;
    CvMat* result = 0;

    CV_FUNCNAME( "CvBoost::get_active_vars" );

    __BEGIN__;

    if( !weak )    //确保已得到分类器
        CV_ERROR( CV_StsError, "The boosted tree ensemble has not been trained yet" );
    //还没有为active_vars和active_vars_abs变量赋值
    if( !active_vars || !active_vars_abs )
    {
        CvSeqReader reader;
        int i, j, nactive_vars;
        CvBoostTree* wtree;
        const CvDTreeNode* node;

        assert(!active_vars && !active_vars_abs);
        mask = cvCreateMat( 1, data->var_count, CV_8U );
        inv_map = cvCreateMat( 1, data->var_count, CV_32S );
        cvZero( mask );    //mask矩阵清零
        cvSet( inv_map, cvScalar(-1) );    //inv_map矩阵赋值为-1

        // first pass: compute the mask of used variables
        //把强分类器序列weak保存到reader变量中
        cvStartReadSeq( weak, &reader );
        //遍历所有的弱分类器
        for( i = 0; i < weak->total; i++ )
        {
            CV_READ_SEQ_ELEM(wtree, reader);    //得到一个弱分类器，即决策树

            node = wtree->get_root();    //得到决策树的根节点
            assert( node != 0 );    //确保根节点不为零，即该决策树存在
            for(;;)    //遍历决策树的各个节点
            {
                const CvDTreeNode* parent;    //定义父节点
                for(;;)    沿着左分子向叶节点遍历
                {
                    CvDTreeSplit* split = node->split;    //分叉属性
                    //遍历该节点的所有分叉属性，包括最佳分叉属性和替代分叉属性
                    for( ; split != 0; split = split->next )
                        mask->data.ptr[split->var_idx] = 1;    //分叉属性相应位置1
                    if( !node->left )    //到达了叶节点，则退出for死循环
                        break;
                    node = node->left;    //下一个左分支
                }
                //从叶节点沿着右分支向父节点遍历
                for( parent = node->parent; parent && parent->right == node;
                    node = parent, parent = parent->parent )
                    ;

                if( !parent )    //到达了根节点，则退出for死循环
                    break;

                node = parent->right;    //下一个右分支
            }
        }

        nactive_vars = cvCountNonZero(mask);    //统计mask中非0元素的数量

        //if ( nactive_vars > 0 )
        {
            //定义两个矩阵
            active_vars = cvCreateMat( 1, nactive_vars, CV_32S );
            active_vars_abs = cvCreateMat( 1, nactive_vars, CV_32S );

            have_active_cat_vars = false;    //标识赋值
            //遍历所有特征属性
            for( i = j = 0; i < data->var_count; i++ )
            {
                if( mask->data.ptr[i] )
                {
                    //赋值为特征属性的索引
                    active_vars->data.i[j] = i;
                    active_vars_abs->data.i[j] = data->var_idx ? data->var_idx->data.i[i] : i;
                    // inv_map存储的形式与active_vars相反，即反映射
                    inv_map->data.i[i] = j;
                    //如果这些特征属性中只要有一个特征属性是类的形式，则have_active_cat_vars变量就为true
                    if( data->var_type->data.i[i] >= 0 )
                        have_active_cat_vars = true;
                    j++;    //计数
                }
            }


            // second pass: now compute the condensed indices
            cvStartReadSeq( weak, &reader );
            for( i = 0; i < weak->total; i++ )    //再次遍历所有的弱分类器
            {
                CV_READ_SEQ_ELEM(wtree, reader);
                node = wtree->get_root();
                for(;;)    //再次遍历决策树
                {
                    const CvDTreeNode* parent;
                    for(;;)
                    {
                        CvDTreeSplit* split = node->split;
                        for( ; split != 0; split = split->next )
                        {
                            // condensed_idx的值为该分叉属性在active_vars的索引，该索引值也是样本数据中特征属性分布排序的索引
                            split->condensed_idx = inv_map->data.i[split->var_idx];
                            assert( split->condensed_idx >= 0 );
                        }

                        if( !node->left )
                            break;
                        node = node->left;
                    }

                    for( parent = node->parent; parent && parent->right == node;
                        node = parent, parent = parent->parent )
                        ;

                    if( !parent )
                        break;

                    node = parent->right;
                }
            }
        }
    }
    //行参absolute_idx为true，则该函数返回active_vars_abs，否则返回active_vars
    result = absolute_idx ? active_vars_abs : active_vars;

    __END__;

    cvReleaseMat( &mask );
    cvReleaseMat( &inv_map );

    return result;
}
```
CvBoostTree类中的一个train函数：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- bool
- CvBoostTree::train( CvDTreeTrainData* _train_data,  
- const CvMat* _subsample_idx, CvBoost* _ensemble )  
- {  
-     clear();  
-     ensemble = _ensemble;  
-     data = _train_data;    //样本数据
-     data->shared = true;  
- return do_train( _subsample_idx );  
- }  




```cpp
bool
CvBoostTree::train( CvDTreeTrainData* _train_data,
                    const CvMat* _subsample_idx, CvBoost* _ensemble )
{
    clear();
    ensemble = _ensemble;
    data = _train_data;    //样本数据
    data->shared = true;
    return do_train( _subsample_idx );
}
```
从上面的train函数可以看出，它主要是调用do_train函数，而CvBoostTree类没有do_train函数，因此train函数是调用CvBoostTree类的父类CvDTree中的do_train函数：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- bool CvDTree::do_train( const CvMat* _subsample_idx )  
- {  
- bool result = false;  
- 
-     CV_FUNCNAME( "CvDTree::do_train" );  
- 
-     __BEGIN__;  
- //得到训练样本数据
-     root = data->subsample_data( _subsample_idx );  
- 
-     CV_CALL( try_split_node(root));  
- 
- if( root->split )  
-     {  
-         CV_Assert( root->left );  
-         CV_Assert( root->right );  
- //用于弱分类器的决策树不需要剪枝，所以这里不会调用prune_cv函数
- if( data->params.cv_folds > 0 )  
-             CV_CALL( prune_cv() );  
- 
- if( !data->shared )  
-             data->free_train_data();  
- 
-         result = true;  
-     }  
- 
-     __END__;  
- 
- return result;  
- }  




```cpp
bool CvDTree::do_train( const CvMat* _subsample_idx )
{
    bool result = false;

    CV_FUNCNAME( "CvDTree::do_train" );

    __BEGIN__;
    //得到训练样本数据
    root = data->subsample_data( _subsample_idx );

    CV_CALL( try_split_node(root));

    if( root->split )
    {
        CV_Assert( root->left );
        CV_Assert( root->right );
        //用于弱分类器的决策树不需要剪枝，所以这里不会调用prune_cv函数
        if( data->params.cv_folds > 0 )
            CV_CALL( prune_cv() );

        if( !data->shared )
            data->free_train_data();

        result = true;
    }

    __END__;

    return result;
}
```
递归调用try_split_node函数，完成决策树的构造：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- void
- CvBoostTree::try_split_node( CvDTreeNode* node )  
- {  
- //调用CvDTree::try_split_node函数
-     CvDTree::try_split_node( node );  
- //在构建决策树的过程中，一旦某个节点再也不能分叉，则进入下面的if语句，完成该节点内所有样本的弱分类器（即响应值）的赋值
- if( !node->left )   
-     {  
- // if the node has not been split,
- // store the responses for the corresponding training samples
- //指向弱分类器的响应值
- double* weak_eval = ensemble->get_weak_response()->data.db;  
- //开辟一块大小为该节点样本数据长的内存
-         cv::AutoBuffer<int> inn_buf(node->sample_count);  
- //labels表示该节点node内的样本在全体训练样本集中的索引
- constint* labels = data->get_cv_labels( node, (int*)inn_buf );  
- int i, count = node->sample_count;    //count表示该节点的样本数
- //该节点的值，当该节点有多个样本是，如果是分类树，节点的值等于拥有最大数量的那个分类的响应值；如果是回归树，节点的值等于所有样本的平均响应值；当该节点只有一个样本时，节点的值就是该样本的响应值
- double value = node->value;   
- //遍历该节点的所有样本，为该节点内的样本所对应的弱分类器的响应值赋值
- for( i = 0; i < count; i++ )  
-             weak_eval[labels[i]] = value;  
-     }  
- }  




```cpp
void
CvBoostTree::try_split_node( CvDTreeNode* node )
{
    //调用CvDTree::try_split_node函数
    CvDTree::try_split_node( node );
    //在构建决策树的过程中，一旦某个节点再也不能分叉，则进入下面的if语句，完成该节点内所有样本的弱分类器（即响应值）的赋值
    if( !node->left ) 
    {
        // if the node has not been split,
        // store the responses for the corresponding training samples
        //指向弱分类器的响应值
        double* weak_eval = ensemble->get_weak_response()->data.db;
        //开辟一块大小为该节点样本数据长的内存
        cv::AutoBuffer<int> inn_buf(node->sample_count);
        //labels表示该节点node内的样本在全体训练样本集中的索引
        const int* labels = data->get_cv_labels( node, (int*)inn_buf );
        int i, count = node->sample_count;    //count表示该节点的样本数
        //该节点的值，当该节点有多个样本是，如果是分类树，节点的值等于拥有最大数量的那个分类的响应值；如果是回归树，节点的值等于所有样本的平均响应值；当该节点只有一个样本时，节点的值就是该样本的响应值
        double value = node->value; 
        //遍历该节点的所有样本，为该节点内的样本所对应的弱分类器的响应值赋值
        for( i = 0; i < count; i++ )
            weak_eval[labels[i]] = value;
    }
}
```
下面我们介绍预测函数predict：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- float
- CvBoost::predict( const CvMat* _sample, const CvMat* _missing,  
-                   CvMat* weak_responses, CvSlice slice,  
- bool raw_mode, bool return_sum ) const
- //_sample表示要预测的样本数据
- //_missing表示预测样本中所缺失的特征属性，该变量为掩码的形式
- // weak_responses表示弱分类器所对应的响应，因此它的元素个数必须与弱分类器的数量一致，默认值0，表示不使用该变量
- //slice表示用于预测的弱分类器的连续子集，默认值为CV_WHOLE_SEQ，表示使用所有的弱分类器
- // raw_mode与CvDTree::predict函数的第三个参数的含义相同，默认为false，具体请看上一篇文章
- // return_sum表示该函数返回值的形式，默认为false，表示返回式12或式17的值，如果该值为true，则返回式12或式17中括号内的值，即不进行符号判断
- {  
- float value = -FLT_MAX;    //初始一个很大的负值
- 
-     CvSeqReader reader;  
- double sum = 0;  
- int wstep = 0;  
- constfloat* sample_data;  
- 
- if( !weak )    //还没有强分类器
-         CV_Error( CV_StsError, "The boosted tree ensemble has not been trained yet" );  
- //判断输入参数_sample的正确性
- if( !CV_IS_MAT(_sample) || CV_MAT_TYPE(_sample->type) != CV_32FC1 ||  
-         (_sample->cols != 1 && _sample->rows != 1) ||  
-         (_sample->cols + _sample->rows - 1 != data->var_all && !raw_mode) ||  
-         (active_vars && _sample->cols + _sample->rows - 1 != active_vars->cols && raw_mode) )  
-             CV_Error( CV_StsBadArg,  
- "the input sample must be 1d floating-point vector with the same "
- "number of elements as the total number of variables or "
- "as the number of variables used for training" );  
- 
- if( _missing )    //判断输入参数_missing的正确性
-     {  
- if( !CV_IS_MAT(_missing) || !CV_IS_MASK_ARR(_missing) ||  
-             !CV_ARE_SIZES_EQ(_missing, _sample) )  
-             CV_Error( CV_StsBadArg,  
- "the missing data mask must be 8-bit vector of the same size as input sample" );  
-     }  
- // weak_count为根据输入参数slice而得到的弱分类器的数量
- int i, weak_count = cvSliceLength( slice, weak );  
- //确保weak_count正确
- if( weak_count >= weak->total )  
-     {  
-         weak_count = weak->total;  
-         slice.start_index = 0;  
-     }  
- 
- if( weak_responses )    //确保输入参数weak_responses的正确性
-     {  
- if( !CV_IS_MAT(weak_responses) ||  
-             CV_MAT_TYPE(weak_responses->type) != CV_32FC1 ||  
-             (weak_responses->cols != 1 && weak_responses->rows != 1) ||  
-             weak_responses->cols + weak_responses->rows - 1 != weak_count )  
-             CV_Error( CV_StsBadArg,  
- "The output matrix of weak classifier responses must be valid "
- "floating-point vector of the same number of components as the length of input slice" );  
-         wstep = CV_IS_MAT_CONT(weak_responses->type) ? 1 : weak_responses->step/sizeof(float);  
-     }  
- //得到active_vars变量的行，即启作用的特征属性的数量
- int var_count = active_vars->cols;  
- constint* vtype = data->var_type->data.i;    //指向全体特征属性
- constint* cmap = data->cat_map->data.i;    //指向特征属性为类形式的映射
- constint* cofs = data->cat_ofs->data.i;    //表示特征属性为类形式的各个属性的偏移量
- 
-     cv::Mat sample = _sample;    //预测样本
-     cv::Mat missing;    //缺失特征属性的掩码
- if(!_missing)  
-         missing = _missing;    //赋值
- 
- // if need, preprocess the input vector
- //对预测样本的特征属性做规范化处理，具体内容请看我的上一篇关于决策树的文章
- if( !raw_mode )  
-     {  
- int sstep, mstep = 0;  
- constfloat* src_sample;  
- const uchar* src_mask = 0;  
- float* dst_sample;  
-         uchar* dst_mask;  
- constint* vidx = active_vars->data.i;  
- constint* vidx_abs = active_vars_abs->data.i;  
- bool have_mask = _missing != 0;  
- 
-         sample = cv::Mat(1, var_count, CV_32FC1);  
-         missing = cv::Mat(1, var_count, CV_8UC1);  
- 
-         dst_sample = sample.ptr<float>();  
-         dst_mask = missing.ptr<uchar>();  
- 
-         src_sample = _sample->data.fl;  
-         sstep = CV_IS_MAT_CONT(_sample->type) ? 1 : _sample->step/sizeof(src_sample[0]);  
- 
- if( _missing )  
-         {  
-             src_mask = _missing->data.ptr;  
-             mstep = CV_IS_MAT_CONT(_missing->type) ? 1 : _missing->step;  
-         }  
- 
- for( i = 0; i < var_count; i++ )  
-         {  
- int idx = vidx[i], idx_abs = vidx_abs[i];  
- float val = src_sample[idx_abs*sstep];  
- int ci = vtype[idx];  
-             uchar m = src_mask ? src_mask[idx_abs*mstep] : (uchar)0;  
- 
- if( ci >= 0 )  
-             {  
- int a = cofs[ci], b = (ci+1 >= data->cat_ofs->cols) ? data->cat_map->cols : cofs[ci+1],  
-                     c = a;  
- int ival = cvRound(val);  
- if ( (ival != val) && (!m) )  
-                     CV_Error( CV_StsBadArg,  
- "one of input categorical variable is not an integer" );  
- 
- while( a < b )  
-                 {  
-                     c = (a + b) >> 1;  
- if( ival < cmap[c] )  
-                         b = c;  
- elseif( ival > cmap[c] )  
-                         a = c+1;  
- else
- break;  
-                 }  
- 
- if( c < 0 || ival != cmap[c] )  
-                 {  
-                     m = 1;  
-                     have_mask = true;  
-                 }  
- else
-                 {  
-                     val = (float)(c - cofs[ci]);  
-                 }  
-             }  
- 
-             dst_sample[i] = val;  
-             dst_mask[i] = m;  
-         }  
- 
- if( !have_mask )  
-             missing.release();  
-     }  
- else
-     {  
- if( !CV_IS_MAT_CONT(_sample->type & (_missing ? _missing->type : -1)) )  
-             CV_Error( CV_StsBadArg, "In raw mode the input vectors must be continuous" );  
-     }  
- 
-     cvStartReadSeq( weak, &reader );    //读取各个弱分类器
-     cvSetSeqReaderPos( &reader, slice.start_index );    //指定起始地址
- 
-     sample_data = sample.ptr<float>();    //预测样本数据的指针
- // !have_active_cat_vars表示启作用的特征属性中没有任何一个是类的形式，missing.empty()预测样本没有缺失的特征属性，!weak_responses表示该输入参数不存在
- if( !have_active_cat_vars && missing.empty() && !weak_responses )  
-     {  
- //遍历所有的弱分类器
- for( i = 0; i < weak_count; i++ )  
-         {  
-             CvBoostTree* wtree;    //表示代表该弱分类器的决策树
- const CvDTreeNode* node;  
-             CV_READ_SEQ_ELEM( wtree, reader );    //提取当前的决策树
- 
-             node = wtree->get_root();    //得到决策树的根节点
- //遍历该决策树，直到到达了叶节点
- while( node->left )  
-             {  
-                 CvDTreeSplit* split = node->split;   //节点分叉
- //得到该节点分叉属性在样本中特征属性排序的索引
- int vi = split->condensed_idx;  
- float val = sample_data[vi];    //得到该特征属性所对应的样本值
- //与分叉值比较，确定方向信息
- int dir = val <= split->ord.c ? -1 : 1;  
- if( split->inversed )    //方向需要反转的处理
-                     dir = -dir;  
- //由方向信息确定下一步是向左分支遍历还是向右分支遍历
-                 node = dir < 0 ? node->left : node->right;  
-             }  
- //累加各个弱分类器响应值，即式12或式17括号内的部分
-             sum += node->value;   
-         }  
-     }  
- else//其他情况的处理
-     {  
- constint* avars = active_vars->data.i;    //指向active_vars变量
- //如果有缺失的特征属性，则m指向掩码变量missing，否则m为空
- const uchar* m = !missing.empty() ? missing.ptr<uchar>() : 0;  
- 
- // full-featured version
- //遍历所有的弱分类器
- for( i = 0; i < weak_count; i++ )  
-         {  
-             CvBoostTree* wtree;    //弱分类器，即决策树
- const CvDTreeNode* node;  
-             CV_READ_SEQ_ELEM( wtree, reader );    //提取当前决策树
- 
-             node = wtree->get_root();    //根节点
- //遍历该决策树，直到到达了叶节点
- while( node->left )  
-             {  
- const CvDTreeSplit* split = node->split;    //得到该节点的分叉
- int dir = 0;  
- //按照从最佳分叉属性到替代分叉属性的顺序遍历分叉属性，直到得到了节点的方向信息为止，
- for( ; !dir && split != 0; split = split->next )  
-                 {  
- //得到该节点分叉属性在样本中特征属性排序的索引
- int vi = split->condensed_idx;  
- int ci = vtype[avars[vi]];    //得到分叉属性的特征属性类型
- float val = sample_data[vi];    //得到该特征属性所对应的样本值
- //如果该特征属性是缺失的特征属性，则进入下次循环
- if( m && m[vi] )  
- continue;  
- if( ci < 0 ) // ordered 特征属性是数值的形式
- //与分叉值比较，确定方向信息
-                         dir = val <= split->ord.c ? -1 : 1;  
- else// categorical 特征属性是类的形式
-                     {  
- int c = cvRound(val);    //取整
- //对应找到split->subset中所对应的位，从而得到方向信息
-                         dir = CV_DTREE_CAT_DIR(c, split->subset);  
-                     }  
- if( split->inversed )    //方向需要反转的处理
-                         dir = -dir;  
-                 }  
- 
- if( !dir )    //如果方向信息还是没有得到
-                 {  
- //该节点的哪个分支的样本数量多，方向dir就指向哪
- int diff = node->right->sample_count - node->left->sample_count;  
-                     dir = diff < 0 ? -1 : 1;  
-                 }  
- //由方向信息确定下一步是向左分支遍历还是向右分支遍历
-                 node = dir < 0 ? node->left : node->right;  
-             }  
- //如果定义了输入参数weak_responses，则输出弱分类器的值
- if( weak_responses )  
-                 weak_responses->data.fl[i*wstep] = (float)node->value;  
- //累加各个弱分类器响应值，即式12或式17括号内的部分
-             sum += node->value;  
-         }  
-     }  
- 
- if( return_sum )  
-         value = (float)sum;    //直接返回式12或式17中括号内的值
- else
-     {  
- int cls_idx = sum >= 0;    //符号判断，即式12或式17的值
- if( raw_mode )  
-             value = (float)cls_idx;    //得到原始的数据
- else//得到其所代表的数据
-             value = (float)cmap[cofs[vtype[data->var_count]] + cls_idx];  
-     }  
- 
- return value;    //返回
- }  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
float
CvBoost::predict( const CvMat* _sample, const CvMat* _missing,
                  CvMat* weak_responses, CvSlice slice,
                  bool raw_mode, bool return_sum ) const
//_sample表示要预测的样本数据
//_missing表示预测样本中所缺失的特征属性，该变量为掩码的形式
// weak_responses表示弱分类器所对应的响应，因此它的元素个数必须与弱分类器的数量一致，默认值0，表示不使用该变量
//slice表示用于预测的弱分类器的连续子集，默认值为CV_WHOLE_SEQ，表示使用所有的弱分类器
// raw_mode与CvDTree::predict函数的第三个参数的含义相同，默认为false，具体请看上一篇文章
// return_sum表示该函数返回值的形式，默认为false，表示返回式12或式17的值，如果该值为true，则返回式12或式17中括号内的值，即不进行符号判断
{
    float value = -FLT_MAX;    //初始一个很大的负值

    CvSeqReader reader;
    double sum = 0;
    int wstep = 0;
    const float* sample_data;

    if( !weak )    //还没有强分类器
        CV_Error( CV_StsError, "The boosted tree ensemble has not been trained yet" );
    //判断输入参数_sample的正确性
    if( !CV_IS_MAT(_sample) || CV_MAT_TYPE(_sample->type) != CV_32FC1 ||
        (_sample->cols != 1 && _sample->rows != 1) ||
        (_sample->cols + _sample->rows - 1 != data->var_all && !raw_mode) ||
        (active_vars && _sample->cols + _sample->rows - 1 != active_vars->cols && raw_mode) )
            CV_Error( CV_StsBadArg,
        "the input sample must be 1d floating-point vector with the same "
        "number of elements as the total number of variables or "
        "as the number of variables used for training" );

    if( _missing )    //判断输入参数_missing的正确性
    {
        if( !CV_IS_MAT(_missing) || !CV_IS_MASK_ARR(_missing) ||
            !CV_ARE_SIZES_EQ(_missing, _sample) )
            CV_Error( CV_StsBadArg,
            "the missing data mask must be 8-bit vector of the same size as input sample" );
    }
    // weak_count为根据输入参数slice而得到的弱分类器的数量
    int i, weak_count = cvSliceLength( slice, weak );
    //确保weak_count正确
    if( weak_count >= weak->total )
    {
        weak_count = weak->total;
        slice.start_index = 0;
    }

    if( weak_responses )    //确保输入参数weak_responses的正确性
    {
        if( !CV_IS_MAT(weak_responses) ||
            CV_MAT_TYPE(weak_responses->type) != CV_32FC1 ||
            (weak_responses->cols != 1 && weak_responses->rows != 1) ||
            weak_responses->cols + weak_responses->rows - 1 != weak_count )
            CV_Error( CV_StsBadArg,
            "The output matrix of weak classifier responses must be valid "
            "floating-point vector of the same number of components as the length of input slice" );
        wstep = CV_IS_MAT_CONT(weak_responses->type) ? 1 : weak_responses->step/sizeof(float);
    }
    //得到active_vars变量的行，即启作用的特征属性的数量
    int var_count = active_vars->cols;
    const int* vtype = data->var_type->data.i;    //指向全体特征属性
    const int* cmap = data->cat_map->data.i;    //指向特征属性为类形式的映射
    const int* cofs = data->cat_ofs->data.i;    //表示特征属性为类形式的各个属性的偏移量

    cv::Mat sample = _sample;    //预测样本
    cv::Mat missing;    //缺失特征属性的掩码
    if(!_missing)
        missing = _missing;    //赋值

    // if need, preprocess the input vector
    //对预测样本的特征属性做规范化处理，具体内容请看我的上一篇关于决策树的文章
    if( !raw_mode )
    {
        int sstep, mstep = 0;
        const float* src_sample;
        const uchar* src_mask = 0;
        float* dst_sample;
        uchar* dst_mask;
        const int* vidx = active_vars->data.i;
        const int* vidx_abs = active_vars_abs->data.i;
        bool have_mask = _missing != 0;

        sample = cv::Mat(1, var_count, CV_32FC1);
        missing = cv::Mat(1, var_count, CV_8UC1);

        dst_sample = sample.ptr<float>();
        dst_mask = missing.ptr<uchar>();

        src_sample = _sample->data.fl;
        sstep = CV_IS_MAT_CONT(_sample->type) ? 1 : _sample->step/sizeof(src_sample[0]);

        if( _missing )
        {
            src_mask = _missing->data.ptr;
            mstep = CV_IS_MAT_CONT(_missing->type) ? 1 : _missing->step;
        }

        for( i = 0; i < var_count; i++ )
        {
            int idx = vidx[i], idx_abs = vidx_abs[i];
            float val = src_sample[idx_abs*sstep];
            int ci = vtype[idx];
            uchar m = src_mask ? src_mask[idx_abs*mstep] : (uchar)0;

            if( ci >= 0 )
            {
                int a = cofs[ci], b = (ci+1 >= data->cat_ofs->cols) ? data->cat_map->cols : cofs[ci+1],
                    c = a;
                int ival = cvRound(val);
                if ( (ival != val) && (!m) )
                    CV_Error( CV_StsBadArg,
                        "one of input categorical variable is not an integer" );

                while( a < b )
                {
                    c = (a + b) >> 1;
                    if( ival < cmap[c] )
                        b = c;
                    else if( ival > cmap[c] )
                        a = c+1;
                    else
                        break;
                }

                if( c < 0 || ival != cmap[c] )
                {
                    m = 1;
                    have_mask = true;
                }
                else
                {
                    val = (float)(c - cofs[ci]);
                }
            }

            dst_sample[i] = val;
            dst_mask[i] = m;
        }

        if( !have_mask )
            missing.release();
    }
    else
    {
        if( !CV_IS_MAT_CONT(_sample->type & (_missing ? _missing->type : -1)) )
            CV_Error( CV_StsBadArg, "In raw mode the input vectors must be continuous" );
    }

    cvStartReadSeq( weak, &reader );    //读取各个弱分类器
    cvSetSeqReaderPos( &reader, slice.start_index );    //指定起始地址

    sample_data = sample.ptr<float>();    //预测样本数据的指针
    // !have_active_cat_vars表示启作用的特征属性中没有任何一个是类的形式，missing.empty()预测样本没有缺失的特征属性，!weak_responses表示该输入参数不存在
    if( !have_active_cat_vars && missing.empty() && !weak_responses )
    {
        //遍历所有的弱分类器
        for( i = 0; i < weak_count; i++ )
        {
            CvBoostTree* wtree;    //表示代表该弱分类器的决策树
            const CvDTreeNode* node;
            CV_READ_SEQ_ELEM( wtree, reader );    //提取当前的决策树

            node = wtree->get_root();    //得到决策树的根节点
            //遍历该决策树，直到到达了叶节点
            while( node->left )
            {
                CvDTreeSplit* split = node->split;   //节点分叉
                //得到该节点分叉属性在样本中特征属性排序的索引
                int vi = split->condensed_idx;
                float val = sample_data[vi];    //得到该特征属性所对应的样本值
                //与分叉值比较，确定方向信息
                int dir = val <= split->ord.c ? -1 : 1;
                if( split->inversed )    //方向需要反转的处理
                    dir = -dir;
                //由方向信息确定下一步是向左分支遍历还是向右分支遍历
                node = dir < 0 ? node->left : node->right;
            }
            //累加各个弱分类器响应值，即式12或式17括号内的部分
            sum += node->value; 
        }
    }
    else    //其他情况的处理
    {
        const int* avars = active_vars->data.i;    //指向active_vars变量
        //如果有缺失的特征属性，则m指向掩码变量missing，否则m为空
        const uchar* m = !missing.empty() ? missing.ptr<uchar>() : 0;

        // full-featured version
        //遍历所有的弱分类器
        for( i = 0; i < weak_count; i++ )
        {
            CvBoostTree* wtree;    //弱分类器，即决策树
            const CvDTreeNode* node;
            CV_READ_SEQ_ELEM( wtree, reader );    //提取当前决策树

            node = wtree->get_root();    //根节点
            //遍历该决策树，直到到达了叶节点
            while( node->left )
            {
                const CvDTreeSplit* split = node->split;    //得到该节点的分叉
                int dir = 0;
                //按照从最佳分叉属性到替代分叉属性的顺序遍历分叉属性，直到得到了节点的方向信息为止，
                for( ; !dir && split != 0; split = split->next )
                {
                    //得到该节点分叉属性在样本中特征属性排序的索引
                    int vi = split->condensed_idx;
                    int ci = vtype[avars[vi]];    //得到分叉属性的特征属性类型
                    float val = sample_data[vi];    //得到该特征属性所对应的样本值
                    //如果该特征属性是缺失的特征属性，则进入下次循环
                    if( m && m[vi] )
                        continue;
                    if( ci < 0 ) // ordered 特征属性是数值的形式
                        //与分叉值比较，确定方向信息
                        dir = val <= split->ord.c ? -1 : 1;
                    else // categorical 特征属性是类的形式
                    {
                        int c = cvRound(val);    //取整
                        //对应找到split->subset中所对应的位，从而得到方向信息
                        dir = CV_DTREE_CAT_DIR(c, split->subset);
                    }
                    if( split->inversed )    //方向需要反转的处理
                        dir = -dir;
                }

                if( !dir )    //如果方向信息还是没有得到
                {
                    //该节点的哪个分支的样本数量多，方向dir就指向哪
                    int diff = node->right->sample_count - node->left->sample_count;
                    dir = diff < 0 ? -1 : 1;
                }
                //由方向信息确定下一步是向左分支遍历还是向右分支遍历
                node = dir < 0 ? node->left : node->right;
            }
            //如果定义了输入参数weak_responses，则输出弱分类器的值
            if( weak_responses )
                weak_responses->data.fl[i*wstep] = (float)node->value;
            //累加各个弱分类器响应值，即式12或式17括号内的部分
            sum += node->value;
        }
    }

    if( return_sum )
        value = (float)sum;    //直接返回式12或式17中括号内的值
    else
    {
        int cls_idx = sum >= 0;    //符号判断，即式12或式17的值
        if( raw_mode )
            value = (float)cls_idx;    //得到原始的数据
        else    //得到其所代表的数据
            value = (float)cmap[cofs[vtype[data->var_count]] + cls_idx];
    }

    return value;    //返回
}
```

OpenCV是用决策树来得到弱分类器的，因此实现弱分类器的类CvBoostTree继承于构建决策树的类CvDTree，前面介绍过的CvBoostTree类内的train函数和try_split_node函数都是虚函数，它们是针对用于弱分类器设计的决策树的特殊性而重写的函数，这样的虚函数还有许多，如find_surrogate_split_ord、find_split_ord_class、calc_node_value等。对这些函数就不再详细阐述，这里主要把与CvDTree类的不同的地方说明一下，关于CvDTree类的介绍请看我的上一篇关于决策树的介绍。

1、构建弱分类器的每个样本都有权值，即*wi*(*m*)，在决策树中被称为是先验概率；

2、用于衡量分类树纯度的不仅有基尼指数，还有错误分类率这种方法；

3、弱分类器只是一个用于研究两类问题的决策树，所有要相对简单一点；

4、不同的弱分类器所应用的训练样本的数量可能会不同。



三、应用实例
![](https://img-blog.csdn.net/20160118145034770)

下面我们就给出一个具体的应用实例。

如上图所示，红色“+”的采样点的坐标为：(40,55)，(35,35)，(55,15)，(45,25)，(10,10)，(15,15)，(40,10)，(30,15)，(30,50)，(100,20)，(45,65)，(20,35)，(80,20)，(90,5)，(95,35)，(80,65)，(15,55)，(25,65)，(85,35)，(85,55)，(95,70)，(105,50)，(115,65)，(110,25)，(120,45)，(15,45)；蓝色“×”的采样点的坐标为：(55,30)，(60,65)，(95,60)，(25,40)，(75,45)，(105,35)，(65,10)，(50,50)，(40,35)，(70,55)，(80,30)，(95,45)，(60,20)，(70,30)，(65,45)，(85,40)。则我们用AdaBoost算法判断新的坐标(55,25)是属于红色标注的区域还是蓝色标注的区域，程序为：



**[cpp]**[view plain](http://blog.csdn.net/zhaocj/article/details/50536385#)[copy](http://blog.csdn.net/zhaocj/article/details/50536385#)

[print](http://blog.csdn.net/zhaocj/article/details/50536385#)[?](http://blog.csdn.net/zhaocj/article/details/50536385#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- #include "opencv2/core/core.hpp"
- #include "opencv2/highgui/highgui.hpp"
- #include "opencv2/imgproc/imgproc.hpp"
- #include "opencv2/ml/ml.hpp"
- 
- #include <iostream>
- usingnamespace cv;  
- usingnamespace std;  
- 
- int main( int argc, char** argv )  
- {     
- //训练样本
- float trainingData[42][2]={ {40, 55},{35, 35},{55, 15},{45, 25},{10, 10},{15, 15},{40, 10},  
-                             {30, 15},{30, 50},{100, 20},{45, 65},{20, 35},{80, 20},{90, 5},  
-                             {95, 35},{80, 65},{15, 55},{25, 65},{85, 35},{85, 55},{95, 70},  
-                             {105, 50},{115, 65},{110, 25},{120, 45},{15, 45},  
-                             {55, 30},{60, 65},{95, 60},{25, 40},{75, 45},{105, 35},{65, 10},  
-                             {50, 50},{40, 35},{70, 55},{80, 30},{95, 45},{60, 20},{70, 30},  
-                             {65, 45},{85, 40}   };  
-     Mat trainingDataMat(42, 2, CV_32FC1, trainingData);   
- //训练样本的响应值
- float responses[42] = {'R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R',  
- 'R','R','R','R','R','R','R','R','R','R',  
- 'B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B' };  
-     Mat responsesMat(42, 1, CV_32FC1, responses);  
- 
- float priors[2] = {1, 1};    //先验概率
- 
-     CvBoostParams params( CvBoost::REAL, // boost_type  
-                           10, // weak_count  
-                           0.95, // weight_trim_rate  
-                           15, // max_depth  
- false, // use_surrogates  
-                           priors // priors 
-                           );    
- 
-     CvBoost boost;  
-     boost.train (   trainingDataMat,   
-                     CV_ROW_SAMPLE,   
-                     responsesMat,  
-                     Mat(),    
-                     Mat(),  
-                     Mat(),  
-                     Mat(),    
-                     params  
-                     );    
- //预测样本
- float myData[2] = {55, 25};  
-     Mat myDataMat(2, 1, CV_32FC1, myData);  
- double r = boost.predict( myDataMat );  
- 
-     cout<<endl<<"result:  "<<(char)r<<endl;  
- 
- return 0;  
-  }  




```cpp
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml/ml.hpp"

#include <iostream>
using namespace cv;
using namespace std;

int main( int argc, char** argv )
{	
	//训练样本
	float trainingData[42][2]={	{40, 55},{35, 35},{55, 15},{45, 25},{10, 10},{15, 15},{40, 10},
							{30, 15},{30, 50},{100, 20},{45, 65},{20, 35},{80, 20},{90, 5},
							{95, 35},{80, 65},{15, 55},{25, 65},{85, 35},{85, 55},{95, 70},
							{105, 50},{115, 65},{110, 25},{120, 45},{15, 45},
							{55, 30},{60, 65},{95, 60},{25, 40},{75, 45},{105, 35},{65, 10},
							{50, 50},{40, 35},{70, 55},{80, 30},{95, 45},{60, 20},{70, 30},
							{65, 45},{85, 40}	};
	Mat trainingDataMat(42, 2, CV_32FC1, trainingData); 
	//训练样本的响应值
	float responses[42] = {'R','R','R','R','R','R','R','R','R','R','R','R','R','R','R','R',
							'R','R','R','R','R','R','R','R','R','R',
						'B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B'	};
	Mat responsesMat(42, 1, CV_32FC1, responses);

	float priors[2] = {1, 1};    //先验概率

	CvBoostParams params( CvBoost::REAL, // boost_type  
                          10, // weak_count  
                          0.95, // weight_trim_rate  
                          15, // max_depth  
                          false, // use_surrogates  
                          priors // priors 
                          );  

	CvBoost boost;
	boost.train (	trainingDataMat, 
					CV_ROW_SAMPLE, 
					responsesMat,
					Mat(),  
					Mat(),
					Mat(),
					Mat(),  
					params
				    );  
	//预测样本
	float myData[2] = {55, 25};
	Mat myDataMat(2, 1, CV_32FC1, myData);
	double r = boost.predict( myDataMat );

	cout<<endl<<"result:  "<<(char)r<<endl;

	return 0;
 }
```

输出的结果为：

result:  R
















































