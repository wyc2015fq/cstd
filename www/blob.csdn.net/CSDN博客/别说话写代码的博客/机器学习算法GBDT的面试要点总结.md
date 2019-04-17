# 机器学习算法GBDT的面试要点总结 - 别说话写代码的博客 - CSDN博客





2018年09月10日 19:15:01[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：88标签：[机器学习GDBT																[GDBT面试题](https://so.csdn.net/so/search/s.do?q=GDBT面试题&t=blog)](https://so.csdn.net/so/search/s.do?q=机器学习GDBT&t=blog)
个人分类：[其他](https://blog.csdn.net/qq_21997625/article/category/7018578)








转自：[https://www.cnblogs.com/ModifyRong/p/7744987.html](https://www.cnblogs.com/ModifyRong/p/7744987.html)

**1.简介**

    gbdt全称梯度下降树，在传统机器学习算法里面是对真实分布拟合的最好的几种算法之一，在前几年深度学习还没有大行其道之前，gbdt在各种竞赛是大放异彩。原因大概有几个，一是效果确实挺不错。二是即可以用于分类也可以用于回归。三是可以筛选特征。这三点实在是太吸引人了，导致在面试的时候大家也非常喜欢问这个算法。 gbdt的面试考核点，大致有下面几个:
- gbdt 的算法的流程？
- gbdt 如何选择特征 ？
- gbdt 如何构建特征 ？
- gbdt 如何用于分类？
- gbdt 通过什么方式减少误差 ？
- gbdt的效果相比于传统的LR，SVM效果为什么好一些 ？
- gbdt 如何加速训练？
- gbdt的参数有哪些，如何调参 ？
- gbdt 实战当中遇到的一些问题 ？
- gbdt的优缺点 ？

**2. 正式介绍**

    首先gbdt 是通过采用加法模型（即基函数的线性组合），以及不断减小训练过程产生的残差来达到将数据分类或者回归的算法。
- **gbdt的训练过程**

        我们通过一张图片，[图片来源](https://www.zybuluo.com/vivounicorn/note/446479#4210-adam)来说明gbdt的训练过程: 

![](https://images2017.cnblogs.com/blog/666027/201710/666027-20171030203845058-619624621.png)

                      图 1：GBDT 的训练过程

        gbdt通过多轮迭代,每轮迭代产生一个弱分类器，每个分类器在上一轮分类器的**残差**基础上进行训练。对弱分类器的要求一般是足够简单，并且是低方差和高偏差的。因为训练的过程是通过降低偏差来不断提高最终分类器的精度，（此处是可以证明的）。

        弱分类器一般会选择为CART TREE（也就是分类回归树）。由于上述高偏差和简单的要求 每个分类回归树的深度不会很深。最终的总分类器 是将每轮训练得到的弱分类器加权求和得到的（也就是加法模型）。

        模型最终可以描述为：

![](https://img-blog.csdn.net/20180910191650905?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        模型一共训练M轮，每轮产生一个弱分类器 T(x;θm)T(x;θm)。弱分类器的**损失函数**

![](https://img-blog.csdn.net/20180910191705786?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018091019172232?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)为当前的模型，gbdt 通过经验风险极小化来确定下一个弱分类器的参数。具体到损失函数本身的选择也就是L的选择，有平方损失函数，0-1损失函数，对数损失函数等等。如果我们选择平方损失函数，那么这个差值其实就是我们平常所说的残差。
- - 但是其实我们真正关注的，1.是希望损失函数能够不断的减小，2.是希望损失函数能够尽可能快的减小。所以如何尽可能快的减小呢？

- - 让损失函数沿着梯度方向的下降。这个就是gbdt 的 gb的核心了。 利用损失函数的负梯度在当前模型的值作为回归问题提升树算法中的残差的近似值去拟合一个回归树。gbdt 每轮迭代的时候，都去拟合损失函数在当前模型下的负梯度。

- - 这样每轮训练的时候都能够让损失函数尽可能快的减小，尽快的收敛达到局部最优解或者全局最优解。

- **gbdt如何选择特征？**

        gbdt选择特征的细节其实是想问你CART Tree生成的过程。这里有一个前提，gbdt的弱分类器默认选择的是CART TREE。其实也可以选择其他弱分类器的，选择的前提是低方差和高偏差。框架服从boosting 框架即可。

        下面我们具体来说CART TREE(是一种二叉树) 如何生成。CART TREE 生成的过程其实就是一个选择特征的过程。假设我们目前总共有 M 个特征。第一步我们需要从中选择出一个特征 j，做为二叉树的第一个节点。然后对特征 j 的值选择一个切分点 m. 一个 样本的特征j的值 如果小于m，则分为一类，如果大于m,则分为另外一类。如此便构建了CART 树的一个节点。其他节点的生成过程和这个是一样的。现在的问题是在每轮迭代的时候，如何选择这个特征 j,以及如何选择特征 j 的切分点 m:
- - 原始的gbdt的做法非常的暴力，首先遍历每个特征，然后对每个特征遍历它所有可能的切分点，找到最优特征 m 的最优切分点 j。

- - 如何衡量我们找到的特征 m和切分点 j 是最优的呢？ 我们用定义一个函数 FindLossAndSplit 来展示一下求解过程：




```python
def findLossAndSplit(x,y):
    # 我们用 x 来表示训练数据
    # 我们用 y 来表示训练数据的label
    # x[i]表示训练数据的第i个特征
    # x_i 表示第i个训练样本

    # minLoss 表示最小的损失
    minLoss = Integet.max_value
    # feature 表示是训练的数据第几纬度的特征
    feature = 0
    # split 表示切分点的个数
    split = 0

    # M 表示 样本x的特征个数
    for j in range(0,M):
        # 该维特征下，特征值的每个切分点，这里具体的切分方式可以自己定义
        for c in range(0,x[j]):
            L = 0
            # 第一类
            R1 = {x|x[j] <= c}
            # 第二类
            R2 = {x|x[j] > c}
            # 属于第一类样本的y值的平均值
            y1 = ave{y|x 属于 R1}
            # 属于第二类样本的y值的平均值
            y2 = ave{y| x 属于 R2}
            # 遍历所有的样本，找到 loss funtion 的值
            for x_1 in all x
                if x_1 属于 R1： 
                    L += (y_1 - y1)^2 
                else:
                    L += (y_1 - y2)^2
            if L < minLoss:
               minLoss = L
               feature  = i
               split = c
    return minLoss,feature ,split
```


- - 如果对这段代码不是很了解的，可以先去看看李航第五章中对CART TREE 算法的叙述。在这里，我们先遍历训练样本的所有的特征，对于特征 j，我们遍历特征 j 所有特征值的切分点 c。找到可以让下面这个式子最小的特征 j 以及切分点c.
- - ![](https://images2017.cnblogs.com/blog/666027/201710/666027-20171031134114980-1949308807.png)


- **gbdt 如何构建特征 ?**

        其实说gbdt 能够构建特征并非很准确，gbdt 本身是不能产生特征的，但是我们可以利用gbdt去产生特征的组合。在CTR预估中，工业界一般会采用[逻辑回归](http://www.cnblogs.com/ModifyRong/p/7739955.html#3825035)去进行处理,在我的上一篇博文当中已经说过，逻辑回归本身是适合处理线性可分的数据，如果我们想让逻辑回归处理非线性的数据，其中一种方式便是组合不同特征，增强逻辑回归对非线性分布的拟合能力。

        长久以来，我们都是通过人工的先验知识或者实验来获得有效的组合特征，但是很多时候，使用人工经验知识来组合特征过于耗费人力，造成了机器学习当中一个很奇特的现象：有多少人工就有多少智能。关键是这样通过人工去组合特征并不一定能够提升模型的效果。所以我们的从业者或者学界一直都有一个趋势便是通过算法自动，高效的寻找到有效的特征组合。Facebook 在2014年 发表的一篇论文便是这种尝试下的产物，利用gbdt去产生有效的特征组合，以便用于逻辑回归的训练，提升模型最终的效果。
- - ![](https://images2017.cnblogs.com/blog/666027/201710/666027-20171031154748527-1827071972.png)


             图 2：用GBDT 构造特征

        如图 2所示，我们 使用 GBDT 生成了两棵树，两颗树一共有五个叶子节点。我们将样本 X 输入到两颗树当中去，样本X 落在了第一棵树的第二个叶子节点，第二颗树的第一个叶子节点，于是我们便可以依次构建一个五纬的特征向量，每一个纬度代表了一个叶子节点，样本落在这个叶子节点上面的话那么值为1，没有落在该叶子节点的话，那么值为 0.

        于是对于该样本，我们可以得到一个向量[0,1,0,1,0] 作为该样本的组合特征，和原来的特征一起输入到逻辑回归当中进行训练。实验证明这样会得到比较显著的效果提升。
- **GBDT 如何用于分类 ？**

        首先明确一点，gbdt 无论用于分类还是回归一直都是使用的CART 回归树。不会因为我们所选择的任务是分类任务就选用分类树，这里面的核心是因为gbdt 每轮的训练是在上一轮的训练的残差基础之上进行训练的。这里的残差就是当前模型的负梯度值 。这个要求每轮迭代的时候，弱分类器的输出的结果相减是有意义的。残差相减是有意义的。

        如果选用的弱分类器是分类树，类别相减是没有意义的。上一轮输出的是样本 x 属于 A类，本一轮训练输出的是样本 x 属于 B类。 A 和 B 很多时候甚至都没有比较的意义，A 类- B类是没有意义的。

        我们具体到分类这个任务上面来，我们假设样本 X 总共有 K类。来了一个样本 x，我们需要使用gbdt来判断 x 属于样本的哪一类。
- - ![](https://images2017.cnblogs.com/blog/666027/201711/666027-20171103112239482-1095616989.png)
图三 gbdt 多分类算法流程



        第一步 我们在训练的时候，是针对样本 X 每个可能的类都训练一个分类回归树。举例说明，目前样本有三类，也就是 K = 3。样本 x 属于 第二类。那么针对该样本 x 的分类结果，其实我们可以用一个 三维向量 [0,1,0] 来表示。0表示样本不属于该类，1表示样本属于该类。由于样本已经属于第二类了，所以第二类对应的向量维度为1，其他位置为0。

        针对样本有 三类的情况，我们实质上是在每轮的训练的时候是同时训练三颗树。第一颗树针对样本x的第一类，输入为（x,0）。第二颗树输入针对 样本x 的第二类，输入为（x,1）。第三颗树针对样本x 的第三类，输入为（x，0）

        在这里每颗树的训练过程其实就是就是我们之前已经提到过的CATR TREE 的生成过程。在此处我们参照之前的生成树的程序 即可以就解出三颗树，以及三颗树对x 类别的预测值f1(x),f2(x),f3(x)。那么在此类训练中，我们仿照多分类的逻辑回归 ，使用softmax 来产生概率，则属于类别 1 的概率

![](https://img-blog.csdn.net/20180910191920158?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        并且我们我们可以针对类别1 求出 残差![](https://img-blog.csdn.net/20180910192150557?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70);类别2 求出残差![](https://img-blog.csdn.net/20180910192133313?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70);类别3 求出残差![](https://img-blog.csdn.net/20180910192111601?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        然后开始第二轮训练 针对第一类 输入为![](https://img-blog.csdn.net/20180910192225324?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70), 针对第二类输入为![](https://img-blog.csdn.net/20180910192242877?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70), 针对 第三类输入为 ![](https://img-blog.csdn.net/20180910192303556?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70).继续训练出三颗树。一直迭代M轮。每轮构建 3颗树。

        所以当K =3。我们其实应该有三个式子

![](https://img-blog.csdn.net/20180910191936201?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        当训练完毕以后，新来一个样本 x1 ，我们需要预测该样本的类别的时候，便可以有这三个式子产生三个值，f1(x),f2(x),f3(x)。样本属于 某个类别c的概率为 

![](https://img-blog.csdn.net/20180910191949773?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- **GBDT 多分类举例说明**

        上面的理论阐述可能仍旧过于难懂，我们下面将拿Iris 数据集中的六个数据作为例子，来展示gbdt 多分类的过程。
- - |样本编号|花萼长度(cm)|花萼宽度(cm)|花瓣长度(cm)|花瓣宽度|花的种类|
|----|----|----|----|----|----|
|1|5.1|3.5|1.4|0.2|山鸢尾|
|2|4.9|3.0|1.4|0.2|山鸢尾|
|3|7.0|3.2|4.7|1.4|杂色鸢尾|
|4|6.4|3.2|4.5|1.5|杂色鸢尾|
|5|6.3|3.3|6.0|2.5|维吉尼亚鸢尾|
|6|5.8|2.7|5.1|1.9|维吉尼亚鸢尾|

		图四 Iris 数据集


        这是一个有6个样本的三分类问题。我们需要根据这个花的花萼长度，花萼宽度，花瓣长度，花瓣宽度来判断这个花属于山鸢尾，杂色鸢尾，还是维吉尼亚鸢尾。具体应用到gbdt多分类算法上面。我们用一个三维向量来标志样本的label。[1,0,0] 表示样本属于山鸢尾，[0,1,0] 表示样本属于杂色鸢尾，[0,0,1] 表示属于维吉尼亚鸢尾。

        gbdt 的多分类是针对每个类都独立训练一个 CART Tree。所以这里，我们将针对山鸢尾类别训练一个 CART Tree 1。杂色鸢尾训练一个 CART Tree 2 。维吉尼亚鸢尾训练一个CART Tree 3，这三个树相互独立。

![](https://img-blog.csdn.net/20180910192416873?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        下面我们来看 CART Tree1 是如何生成的，其他树 CART Tree2 , CART Tree 3的生成方式是一样的。CART Tree的生成过程是从这四个特征中找一个特征做为CART Tree1 的节点。比如花萼长度做为节点。6个样本当中花萼长度 大于5.1 cm的就是 A类，小于等于 5.1 cm 的是B类。生成的过程其实非常简单，问题 1.是哪个特征最合适？ 2.是这个特征的什么特征值作为切分点？ 即使我们已经确定了花萼长度做为节点。花萼长度本身也有很多值。在这里我们的方式是遍历所有的可能性，找到一个最好的特征和它对应的最优特征值可以让当前式子的值最小。
- - - ![](https://images2017.cnblogs.com/blog/666027/201710/666027-20171031134114980-1949308807.png)



我们以第一个特征的第一个特征值为例。R1 为所有样本中花萼长度小于 5.1 cm 的样本集合，R2 为所有样本当中花萼长度大于等于 5.1cm 的样本集合。所以 R1={2}，R2={1,3,4,5,6}.

![](https://images2017.cnblogs.com/blog/666027/201711/666027-20171107095824388-509509244.jpg)

图 5 节点分裂示意图

        y1 为 R1 所有样本的label 的均值 1/1=1。y2 为 R2 所有样本的label 的均值 (1+0+0+0+0)/5=0.2。

![](https://img-blog.csdn.net/20180910192539476?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://images2017.cnblogs.com/blog/666027/201711/666027-20171107095955325-625057114.jpg)

图 6 第一个特征的第二个特侦值的节点分裂情况        

![](https://img-blog.csdn.net/20180910192600375?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://images2017.cnblogs.com/blog/666027/201711/666027-20171107095601544-1325744377.jpg)

图 7 所有情况说明  

        这样我们可以遍历所有特征的所有特征值，找到让这个式子最小的特征以及其对应的特征值，一共有24种情况,4个特征*每个特征有6个特征值。在这里我们算出来让这个式子最小的特征花萼长度,特征值为5.1 cm。这个时候损失函数最小为 0.8。

        于是我们的预测函数此时也可以得到:

![](https://img-blog.csdn.net/20180910192626484?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        下面我们用代码来实现整个找特征的过程，大家可以自己再对照代码看看。

```python
# 定义训练数据
train_data = [[5.1,3.5,1.4,0.2],[4.9,3.0,1.4,0.2],[7.0,3.2,4.7,1.4],[6.4,3.2,4.5,1.5],[6.3,3.3,6.0,2.5],[5.8,2.7,5.1,1.9]]

# 定义label
label_data = [[1,0,0],[1,0,0],[0,1,0],[0,1,0],[0,0,1],[0,0,1]]
# index 表示的第几类
def findBestLossAndSplit(train_data,label_data,index):
        sample_numbers = len(label_data)
        feature_numbers = len(train_data[0])
        current_label = []

        # define the minLoss
        minLoss = 10000000

        # feature represents the dimensions of the feature
        feature = 0

        # split represents the detail split value
        split = 0

        # get current label
        for label_index in range(0,len(label_data)):
            current_label.append(label_data[label_index][index])

        # trans all features
        for feature_index in range(0,feature_numbers):
            ## current feature value
            current_value = []

            for sample_index in range(0,sample_numbers):
                current_value.append(train_data[sample_index][feature_index])
            L = 0
            ## different split value
            print current_value
            for index in range(0,len(current_value)):
                R1 = []
                R2 = []
                y1 = 0
                y2 = 0

                for index_1 in range(0,len(current_value)):
                    if current_value[index_1] < current_value[index]:
                        R1.append(index_1)
                    else:
                        R2.append(index_1)

                ## calculate the samples for first class
                sum_y = 0
                for index_R1 in R1:
                    sum_y += current_label[index_R1]
                if len(R1) != 0:
                    y1 = float(sum_y) / float(len(R1))
                else:
                    y1 = 0

                ## calculate the samples for second class
                sum_y = 0
                for index_R2 in R2:
                    sum_y += current_label[index_R2]
                if len(R2) != 0:
                    y2 = float(sum_y) / float(len(R2))
                else:
                    y2 = 0

                ## trans all samples to find minium loss and best split
                for index_2 in range(0,len(current_value)):
                    if index_2 in R1:
                        L += float((current_label[index_2]-y1))*float((current_label[index_2]-y1))
                    else:
                        L += float((current_label[index_2]-y2))*float((current_label[index_2]-y2))

                if L < minLoss:
                    feature = feature_index
                    split = current_value[index]
                    minLoss = L
                    print "minLoss"
                    print minLoss
                    print "split"
                    print split
                    print "feature"
                    print feature
        return minLoss,split,feature

findBestLossAndSplit(train_data,label_data,0)
```
- **3 总结**

        目前，我们总结了 gbdt 的算法的流程，gbdt如何选择特征，如何产生特征的组合，以及gbdt 如何用于分类，这个目前可以认为是gbdt 最经常问到的四个部分。至于剩余的问题，因为篇幅的问题，我们准备再开一个篇幅来进行总结。




