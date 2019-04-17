# coursera-斯坦福-机器学习-吴恩达-第6周笔记-算法改进and机器学习系统设计 - 博客堂 - CSDN博客





2017年12月14日 19:52:59[最小森林](https://me.csdn.net/u012052268)阅读数：2964
所属专栏：[斯坦福-吴恩达《机器学习》之路](https://blog.csdn.net/column/details/18191.html)









# coursera-斯坦福-机器学习-吴恩达-第6周笔记-算法改进and机器学习系统设计



- [coursera-斯坦福-机器学习-吴恩达-第6周笔记-算法改进and机器学习系统设计](#coursera-斯坦福-机器学习-吴恩达-第6周笔记-算法改进and机器学习系统设计)- [1算法改进](#1算法改进)- [1评价算法](#11评价算法)- [11模型诊断](#111模型诊断)
- [12模型选择validation](#112模型选择validation)

- [2偏置与方差](#12偏置与方差)- [21诊断偏差与方差](#121诊断偏差与方差)
- [22正则化方差与偏差](#122正则化方差与偏差)
- [23学习曲线Learning Curves](#123学习曲线learning-curves)
- [24决定保留谁](#124决定保留谁)

- [3复习](#13复习)- [31quiz](#131quiz)
- [31编程](#131编程)


- [2机器学习系统设计](#2机器学习系统设计)- [1 误差分析](#21-误差分析)
- [2 处理倾斜数据](#22-处理倾斜数据)
- [3 使用大的数据集](#23-使用大的数据集)
- [4 quiz](#24-quiz)






## 1算法改进

### 1.1评价算法

#### 1.1.1模型诊断

对于正则化线性回归，其代价函数为: 
$min_\theta\ \dfrac{1}{2m}\  \sum_{i=1}^m (h_\theta(x^{(i)}) - y^{(i)})^2 + \lambda\ \sum_{j=1}^n \theta_j^2$

当面对测试集，你的算法效果不佳时，你一般会怎么做？ 

- 获得更多的训练样本？ 

- 尝试更少的特征？ 

- 尝试获取附加的特征？ 

- 尝试增加多项式的特征？ 

- 尝试增加λ? 

- 尝试减小λ?
具体的情况要具体分析，方法不能乱用 

机器学习（算法）诊断(Diagnostic)是一种测试方法，使你能对一种学习算法进行深入的认识，知道什么能运行，什么不能运行，并且能指导你如何最大限度的提高学习算法的性能。

诊断测试虽然需要一些时间来实现，但是这样做可以更有效的利用你的时间。

![](http://oqy7bjehk.bkt.clouddn.com/17-12-4/48049398.jpg)

通常的解决办法是：

将数据集分成训练集和测试集，  

将训练集训练出的参数用测试集数据测试性能。 

通常情况下，训练集包含70％的数据，测试集是剩下的30％。
那么使用这两套新步骤是： 

1. 学习Θ并使用训练集最小化Jtrain（Θ） 

2. 计算测试集错误Jtest（Θ）
The test set error计算为：
- linear regression 
$J_{test}(\Theta) = \dfrac{1}{2m_{test}} \sum_{i=1}^{m_{test}}(h_\Theta(x^{(i)}_{test}) - y^{(i)}_{test})^2$
- classification 
$J_{test}(\theta) = -\frac{1}{2m_{test}}\sum_{i=1}^{m_{test}}y^{(i)}_{test}log(h_{\theta}(x^{(i)}_{test})) + (1-y^{(i)}_{test})log(1-h_{\theta}(x^{(i)}_{test}))$

#### 1.1.2模型选择，validation

只是因为学习算法很好地适合训练集，这并不意味着这是一个很好的假设。它可能会过于合适，因此您对测试集的预测会很差。假设您在训练参数的数据集上测量的假设的误差将低于任何其他数据集上的误差。

给定许多具有不同多项式度的模型，我们可以用系统的方法来确定“最佳”函数。为了选择你的假设的模型，你可以测试每个多项式的程度，看看错误的结果。比如在多项式回归时，我们该怎么选择次数作为我们的假设模型呢？  
![image](https://img-blog.csdn.net/20170518155931255?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjcwMDgwNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们可以把数据集分为三类，训练集，交叉验证集和测试集，  

- 训练集：60％ 

- 交叉验证集：20％ 

- 测试集：20％
用交叉验证集来作为评判选择的标准，选择合适的模型，而测试集则是作为算法性能的评判。  
![image](https://img-blog.csdn.net/20170518160647375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjcwMDgwNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
现在我们可以使用以下方法为三个不同的集合计算三个单独的错误值： 

1. 使用每个多项式的训练集来优化Θ中的参数。 

2. 使用交叉验证集找出具有最小误差的多项式度d（确定合适的模型）。 

3. 使用带有Jtest的测试集（Θ（d））估计泛化误差，（d =具有较低误差的多项式的theta）;（评价模型）
为啥要设置 “交叉验证集”呢？答案：

一般的数据集都是分为trainning set, cross validation set, test set. 

当然, 也有只分为training set和test set的分法 

对第二种分法来说，取得min(Err(test_set))的model作为最佳model，但是我们并不能评价选出来的这个model的性能，如果就将Err(test_set)的值当作这个model的评价的话，这是不公正的，因为这个model本来就是最满足test_set的model
相反，第一种方法取得min(Err(cv_set))的model作为最佳model，对其进行评价的时候，使用剩下的test_set对其进行评价 而不是使用Err(cv_set))的值

### 1.2偏置与方差

#### 1.2.1诊断偏差与方差

![image](https://img-blog.csdn.net/20170518163134691?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjcwMDgwNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上面的图分别表示了高偏差，刚好，高方差
在本节中，我们考察多项式d的程度与假设的不合适或过拟合之间的关系。
- 我们需要区分是否偏差bias 或方差variance 是造成不良预测的问题。
- 高偏差是不足的，高方差是过度拟合。理想的情况下，我们需要找到这两者之间的中庸之道。

![image](https://img-blog.csdn.net/20170518221048066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjcwMDgwNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

随着我们增加多项式的阶数d，训练误差将趋于减小。

与此同时，交叉验证误差会随着我们将d增加到一个点而降低，然后随d的增加而增加，形成一个凸曲线。

![](http://oqy7bjehk.bkt.clouddn.com/17-12-6/16935432.jpg)
- 高偏差（underfitting）：Jtrain（Θ）和JCV（Θ）都很高。而且，JCV（Θ）≈Jtrain（Θ）。
- 高方差（overfitting）：Jtrain（Θ）将是低的，JCV（Θ）将比Jtrain（Θ）大得多。总结如图，记住图的左边与右边。

#### 1.2.2正则化方差与偏差

算法正则化可以有效地防止过拟合, 但正则化跟算法的偏差和方差 又有什么关系呢？

在这段视频中 探讨偏差和方差的问题, 讨论一下两者之间 是如何相互影响的。 以及和算法的正则化之间的相互关系  

根据λ大小画出的拟合曲线如下 

![image](https://img-blog.csdn.net/20170518223443303?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjcwMDgwNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在上图中，我们看到随着λ的增加，我们的拟合变得更加低下。（右边欠拟合）

另一方面，当λ接近0时，我们倾向于过度拟合数据。（左边过拟合）

那么我们如何选择我们的参数λ来使其“恰到好处”呢？ 

为了选择模型和正则化项λ，我们需要：
- 创建一个lambda表（即λ∈{0,0.01,0.02,0.04,0.08,0.16,0.32,0.64,1.28,2.56,5.12,10.24}）;
- 创建一个不同程度的模型或任何其他变体。
- 通过λs迭代，并为每个λ遍历所有的模型学习一些Θ。
- 使用JCV（Θ）上学习的Θ（用λ计算）来计算交叉验证误差，无需正则化或λ= 0。
- 选择交叉验证集合中产生最低错误的最佳组合。
- 使用最佳组合Θ和λ，将其应用于Jtest（Θ），以查看它是否具有良好的问题概括性。

#### 1.2.3学习曲线Learning Curves

绘制学习曲线非常有用 。也许你想检查你的学习算法 运行是否一切正常 ，或者你希望改进算法的表现或效果  

。

那么学习曲线 就是一种很好的工具， 使用学习曲线 来判断某一个学习算法 是否处于偏差 方差问题 或是二者皆有 。下面绘制：

在少数几个数据点（如1,2或3）上训练一个算法将很容易产生0个错误，因为我们总是可以找到一个接近这些点数的二次曲线。因此： 

- 随着训练集变大，二次函数的误差增加。 

- 经过一定的m或训练集大小后，误差值将平稳。
根据样本的大小与误差的关系我们可以画出一般的学习曲线模样 

![image](https://img-blog.csdn.net/20170518223947055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjcwMDgwNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下面在两种情况分析：高偏差和高方差 

1. 高偏差

![image](https://img-blog.csdn.net/20170518224030919?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjcwMDgwNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从图片上看增大样本的方法对高偏差的模型并不能起到一定作用
- 高方差 
![image](https://img-blog.csdn.net/20170518224341002?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjcwMDgwNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
而模型处于高方差的情况下，增大样本可能会起到效果。 

#### 1.2.4决定保留谁

对开头提出的各种措施，我们看看他们适合于什么样的模型  

- 获取更多培训示例：高度差异 

- 尝试更小的功能集：高差异 

- 增加特征：高偏见 

- 添加多项式特征：高偏差 

- 降低λ：高偏差 

- 增加λ：高方差。
诊断神经网络：
- 一个参数较少的神经网络容易出现underfitting。这也是计算待价更低。
- 具有更多参数的大型神经网络容易过度拟合overfitting。这在计算上也是昂贵的。在这种情况下，您可以使用正则化（增加λ）来解决过度拟合问题。

使用单个隐藏层是一个很好的默认开始。您可以使用交叉验证集在许多隐藏层上训练您的神经网络。然后您可以选择性能最好的一个。

模型复杂性的影响：
- 低阶多项式（低模型复杂度）具有高偏差和低方差。在这种情况下，模型不太一致。
- 高阶多项式（高模型复杂度）非常适合训练数据，测试数据极其糟糕。这些对训练数据的偏倚低，但差异很大。
- 实际上，我们希望选择一个介于两者之间的模型，这个模型可以很好地推广，但是也能很好地适合数据。

### 1.3复习

#### 1.3.1quiz
- You train a learning algorithm, and find that it has unacceptably high error on the test set. You plot the learning curve, and obtain the figure below. Is the algorithm suffering from high bias, high variance, or neither?答案：A

你训练一个学习算法，发现它在测试集上有很高的误差。您绘制学习曲线，并获得下图。该算法是否受到高偏差，高方差或两者都没有？

![](http://oqy7bjehk.bkt.clouddn.com/17-12-6/15416050.jpg)
- Suppose you have implemented regularized logistic regression to classify what object is in an image (i.e., to do object recognition). However, when you test your hypothesis on a new set of images, you find that it makes unacceptably large  errors with its predictions on the new images. However, your hypothesis performs well (has low error) on the training set. Which of the following are promising steps to take? Check all that apply.（过拟合，高方差）答案CD！！！！

假设你已经实现了规则化逻辑回归来分类图像中的对象（即做对象识别）。 然而，当你在一组新的图像上测试你的假设时，你会发现它对新图像的预测会产生无法接受的大错误。 但是，您的假设在训练集上表现良好（误差较低）。 以下哪一项是有希望的步骤？ 检查所有适用。

![](http://oqy7bjehk.bkt.clouddn.com/17-12-6/28172314.jpg)

3.假设您已经实施正规化的逻辑回归来预测客户将在网上购物网站上购买什么项目。 然而，当你在一组新的客户上测试你的假设时，你会发现它在预测中产生了无法接受的大错误。 此外，假设在训练集上表现不佳。 以下哪一个可能是有希望的步骤？ 检查所有适用。

Suppose you have implemented regularized logistic regression to predict what items customers will purchase on a web shopping site. However, when you test your hypothesis on a new set of customers, you find that it makes unacceptably large errors in its predictions. Furthermore, the hypothesis performs poorly on the training set. Which of the following might be promising steps to take? Check all that apply.（欠拟合，高偏差）

![](http://oqy7bjehk.bkt.clouddn.com/17-12-6/43771731.jpg)
- Which of the following statements are true? Check all that apply.

It is okay to use data from the test set to choose the regularization parameter λ, but not the model parameters (θ).答案BC 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-6/28442637.jpg)- Which of the following statements are true? Check all that apply.答案BCD

(A) If a neural network has much lower training error than test error, then adding more layers will help bring the test error down because we can fit the test set better.

(B) If a learning algorithm is suffering from high bias, only adding more training examples may not improve the test error significantly.

(C)When debugging learning algorithms, it is useful to plot a learning curve to understand if there is a high bias or high variance problem.

(D)A model with more parameters is more prone to overfitting and typically has higher variance.

#### 1.3.1编程

在上半场练习中，您将实施正则化的线性回归，利用水库水位的变化来预测大坝出水量。 下半场，您将通过调试学习算法的一些诊断，并检查偏差和方差的影响。
- 完成文件linearRegCostFunction.m中的代码。 你的任务是写一个函数来计算正则化的线性回归成本函数。 如果可能的话，尝试向量化你的代码，避免写循环。

```
J = 1/2/m* sum((X*theta - y) .^ 2) + lambda/2/m * sum(theta(2:end) .^ 2);

grad = 1/m* (X'*(X*theta - y));
grad(2:end) = grad(2:end) + lambda/m*theta(2:end);
```
- 填写learningCurve.m，以便返回训练集和交叉验证集的错误向量。

```matlab
for i=1:m
    theta = trainLinearReg(X(1:i, :), y(1:i), lambda);
    error_train(i) = linearRegCostFunction(X(1:i, :), y(1:i), theta, 0);
    error_val(i) = linearRegCostFunction(Xval, yval, theta, 0);
end
```
- 完成polyFeatures.m中的代码，以便函数将大小为m×1的原始训练集X映射到其更高的权力。

```matlab
for i = 1:p
    X_poly(:,i) = X.^i;
end
```
- 完成validationCurve.m中的代码。具体来说，应该使用trainLinearReg函数来训练使用不同λ值的模型，并计算训练误差和交叉验证误差。

```matlab
for i=1:size(lambda_vec, 1)
    theta = trainLinearReg(X, y, lambda_vec(i));
    error_train(i) = linearRegCostFunction(X, y, theta, 0);
    error_val(i) = linearRegCostFunction(Xval, yval, theta, 0);
end
```

## 2机器学习系统设计

这一章对应PPT lecture11

给定一个电子邮件数据集，我们可以为每个电子邮件构建一个向量。这个向量中的每个条目代表一个单词（词袋模型）。该矢量通常包含10,000到50,000个条目，通过查找我们数据集中最常用的单词来收集。如果在电子邮件中找到一个单词，我们将分配它的相应条目1，否则如果没有找到，条目将是0.一旦我们已经准备好了所有的x向量，我们将训练我们的算法，最后，我们可以用它来分类电子邮件是否是垃圾邮件。

那么你怎么能花时间来提高这个分类器的准确度呢？ 

- Step1.使用快速但不完美的算法实现； 

- Step2.画出学习曲线，分析偏差、方差，判断是否需要更多的数据、增加特征量….; 

- Step3.误差分析:人工检测错误、发现系统短处，来增加特征量以改进系统。
那么你怎么能花时间来提高这个分类器的准确度呢？ 

- 收集大量的数据（例如“蜜罐”项目，但并不总是有效） 

- 开发复杂的功能（例如：在垃圾电子邮件中使用电子邮件标题数据） 

- 开发算法以不同的方式处理您的输入（识别垃圾邮件中的拼写错误）。
### 2.1 误差分析

一般我们提升模型的步骤为： 

- Step1.使用快速但不完美的算法实现； 

- Step2.画出学习曲线，分析偏差、方差，判断是否需要更多的数据、增加特征量….; 

- Step3.误差分析:人工检测错误、发现系统短处，来增加特征量以改进系统。
因此，我们应该尝试新的东西，为我们的错误率得到一个数值，并根据我们的结果决定是否要保留新的特征。（使用误差度量值 来判断是否添加新的特征）

### 2.2 处理“倾斜”数据

有些时候，一些数据很不平衡，比如正向的占99%、负向的占1%（倾斜数据shewed data）。这样只用准确度这个指标就不能很好的衡量算法的好坏，所以我们引入了信度precision和召回率recall。举个例子：

判断癌症的分类器，建立逻辑回归模型hθ(x)，y=1表示有癌症，y=0则没有。假设你的算法在测试集上只有1%的错误，可实际上，测试集中只有0.5%的病人患有癌症，因此我们可以通过下面的方法来提高正确率。  
![image](https://img-blog.csdn.net/20170519214455499?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjcwMDgwNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这个“算法”很搞笑：不管是谁来看病，统统判断为没有癌症。这样准确度就提高了，但是病人一时开心，以后会耽误治疗哈哈哈哈哈哈。
从上面的例子我们可以知道正确率不足以表现一个算法的优劣（在某些正例或反例及其少的数据集中），因此我们引入了Precision/Recall。 构造方法：

![image](https://img-blog.csdn.net/20170519215310718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjcwMDgwNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- Precision（准确率、信度） 

在我们预测y=1的数据中，真正得癌症的比重，也就是你预测的可信度。 

$\frac{True pos}{predicted pos} = \frac{True pos}{True pos+ false pos}$
- Recal（召回率）  

在真正得癌症的数据中，我们预测癌症所占的比重。 

$\frac{True pos}{actual pos} = \frac{True pos}{True pos+ false neg}$

在大多数的时候我们想找到两个评价指标的平衡。比如： 

- 假设考虑到一个正常人如果误判为癌症，将会承受不必要的心理和生理压力，所以我们要有很大把握才预测一个病人患癌症(y=1)。那么一种方式就是提高阙值(threshold)，不妨设我们将阙值提高到0.7，即：

```bash
Predict 1 if: hθ(x)≥0.7 
Predict 0 if: hθ(x)<0.7
```

在这种情况下，我们将会有较高的precision，但是recall将会变低。
- 假设考虑到一个已经患癌症的病人如果误判为没有患癌症，那么病人可能将因不能及时治疗而失去宝贵生命，所以我们想要避免错过癌症患者的一种方式就是降低阙值，假设降低到0.3, 即

```bash
Predict 1 if: hθ(x)≥0.3 
Predict 0 if: hθ(x)<0.3
```

在这种情况下，将得到较高的recall，但是precision将会下降。 

为了将precision和Recal转变为一个单一数值，我们引入了F1值：$F = 2\frac{PR}{P+R}$

总结： 

衡量一个算法应该用一下值综合考虑：
- Accuracy = (true positives + true negatives) / (total examples)
- Precision = (true positives) / (true positives + false positives)
- Recall = (true positives) / (true positives + false negatives)
- F 1 score = (2 * precision * recall) / (precision + recall)

### 2.3 使用大的数据集

但事实证明 在一定条件下 ， 得到大量的数据并在 某种类型的学习算法中进行训练。 可以是一种 获得 一个具有良好性能的学习算法 有效的方法。

像这样的结果 引起了一种 在机器学习中 的普遍共识： “取得成功的人不是拥有最好算法的人 而是拥有最多数据的人” 

那么这种说法 在什么时候是真 什么时候是假呢？ 
- 特征x包含足够的信息来准确地预测y。 （例如，验证这种情况的一种方式是，如果只有在给定x的情况下，领域上的专家才能自信地预测y）。
- 我们训练一个具有大量参数（能够学习/表示相当复杂的函数）的学习算法。

### 2.4 quiz

机器学习系统的测试题:
- You are working on a spam classification system using regularized logistic regression. “Spam” is a positive class (y = 1) and “not spam” is the negative class (y = 0). You have trained your classifier and there are m = 1000 examples in the cross-validation set. The chart of predicted class vs. actual class is: 

您正在使用正则化逻辑回归来处理垃圾邮件分类系统。 “垃圾邮件”是一个积极的类（y = 1），“不垃圾邮件”是否定类（y = 0）。 您已经对分类器进行了训练，并且在交叉验证集中有m = 1000个示例。 预测班级与实际班级的关系图如下：
|·|Actual Class: 1|Actual Class: 0|
|----|----|----|
|Predicted Class: 1|85|890|
|Predicted Class: 0|15|10|

计算公式如下： 

- Accuracy = (true positives + true negatives) / (total examples) 

- Precision = (true positives) / (true positives + false positives) 

- Recall = (true positives) / (true positives + false negatives) 

- F1 score = (2 * precision * recall) / (precision + recall)
What is the classifier’s accuracy (as a value from 0 to 1)? 

答案：0.095
- 第 2 个问题 

Suppose a massive dataset is available for training a learning algorithm. Training on a lot of data is likely to give good performance when two of the following conditions hold true.

Which are the two?
- The features x contain sufficient information to predict y accurately. (For example, one way to verify this is if a human expert on the domain can confidently predict y when given only x).
- 
We train a learning algorithm with a large number of parameters (that is able to learn/represent fairly complex functions).
- Suppose you have trained a logistic regression classifier which is outputing hθ(x).


Currently, you predict 1 if hθ(x)≥threshold, and predict 0 if hθ(x)《threshold, where currently the threshold is set to 0.5.

Suppose you increase the threshold to 0.9. Which of the following are true? Check all that apply.

假设你已经训练了输出hθ（x）的逻辑回归分类器。 

目前，如果hθ（x）≥threshold，则预测1;如果hθ（x）《threshold，则预测为0，其中当前阈值设置为0.5。假设将阈值增加到0.9。 以下哪一项是正确的？ 检查所有适用。

答案：高信度，低召回。
- Suppose you are working on a spam classifier, where spam

emails are positive examples (y=1) and non-spam emails are

negative examples (y=0). You have a training set of emails

in which 99% of the emails are non-spam and the other 1% is

spam. Which of the following statements are true? Check all

that apply.

假设您正在使用垃圾邮件分类器，其中垃圾邮件是正面示例（y = 1），非垃圾邮件是负面示例（y = 0）。 您有一组电子邮件，其中99％的电子邮件是非垃圾邮件，另外1％是垃圾邮件。 以下哪一项是正确的？ 检查所有适用。
- If you always predict non-spam (output y=0), your classifier will have an accuracy of 99%.
- If you always predict non-spam (output y=0), your classifier will have a recall of 0%.
- If you always predict spam (output y=1), your classifier will have a recall of 100% and precision of 1%.
- 如果你总是预测非垃圾邮件（输出y = 0），你的分类器将有99％的准确性。
- 如果你总是预测非垃圾邮件（输出y = 0），你的分类器将有一个0％的召回。
- 
如果您总是预测垃圾邮件（输出y = 1），您的分类器将具有100％的回忆率和1％的精度。
- Which of the following statements are true? Check all that apply.

- 
Using a very large training set makes it unlikely for model to overfit the training data.

- On skewed datasets (e.g., when there are more positive examples than negative examples), accuracy is not a good measure of performance and you should instead use F1 score based on the precision and recall.
- 使用非常大的训练集使模型不太可能过度训练数据。
- 在偏斜的数据集上（例如，当比正面的例子更多的时候，正确的例子），准确性不是一个好的衡量指标，你应该使用基于精度和回忆的F1分数。





















