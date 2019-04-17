# 多类分类(Multi-label classification)性能评价之宏平均(macro-average)与微平均(micro-average) - lijunweiyhn的博客 - CSDN博客





2016年12月15日 13:30:18[_Junwei](https://me.csdn.net/lijunweiyhn)阅读数：1173








http://blog.csdn.net/u010551621/article/details/46907575






通常，我们在评价classifier的性能时使用的是accuracy

考虑在多类分类的背景下

accuracy = (分类正确的样本个数) / (分类的所有样本个数)

这样做其实看上去也挺不错的，不过可能会出现一个很严重的问题：例如某一个不透明的袋子里面装了1000台手机，其中有600台iphone6, 300台galaxy s6, 50台华为mate7,50台mx4(当然，这些信息分类器是不知道的。。。)。如果分类器只是简单的把所有的手机都预测为iphone6, 那么通过上面的公式计算的准确率accuracy为0.6，看起来还不错;可是三星，华为和小米的全部预测错了。如果再给一个袋子，里面装着600台galaxy
 s6, 300台mx4, 50台华为mate7,50台iphone，那这个分类器立马就爆炸了，连回家带孩子的要求都达不到

所以，仅仅用accuracy来衡量一个分类器的性能是很不科学的

为此，引入了宏平均(micro-average)的概念

介绍宏平均之前，还要介绍几个概念：

(1)precision(准确率，查准率)

(2)recall(召回率，查全率)

(3)F-measure

文字性的概念我就不水，我从符号上来表示一下吧：

多类分类问题中，分类结果一般有4种情况:


- 属于类C的样本被正确分类到类C，记这一类样本数为 TP
- 不属于类C的样本被错误分类到类C，记这一类样本数为 FN
- 属于类别C的样本被错误分类到类C的其他类，记这一类样本数为 TN
- 不属于类别C的样本被正确分类到了类别C的其他类，记这一类样本数为 FP                     



从字面意思上来说，某一类别C的准确率(查准率)可以理解为预测该类样本的准确性：

precision = TP / (TP + FN) 

这里很好理解， TP+FN实际上就是分类器在分类时，预测为类别C的样本个数

而某一个类别C的召回率(查全率)可以理解为，你预测正确的类别C的样本对于样本集中类别C的样本的覆盖程度，如果完全覆盖，那么recall就是100%

recall = TP / (TP + TN)

这里的TP+TN表示样本集中，真实类标为C的样本的个数

单一的用recall或者precision来评价分类器也是不合理的，譬如文章开头说到的那个简单的classifier

我们通过公式可以惊奇的发现在两个样本集上的recall都是100%，这是因为所有的iphone6样本，分类器都可以把它预测对，但是准确率就大不相同了；在第一个数据集上的准确率precision为0.6，但是第二个样本集上的准确率只有可怜的0.05；对于第二个样本集的结果，我可以说classifier很好，因为recall很高，也可以说classifier不好，因为precision很低。不过很明显，这些说法都是不合理的；综合考虑两种指标才是直观的比较可靠的指标。

现在就引入F-measure，计算公式如下

F- measure = (A^2 + 1) * precision * recall / (A ^ 2 * precision + recall)

（不知道怎么写公式，就直接用文字写的，感觉有点挫）

如果A取1，那么F-measure实际上就是precison和recall的调和平均值2*precision*recall / (precision + recall)， 此时的F-measure就称为F1值

把这些概念说了， 就回到刚才的宏平均的计算

把所有类的F1值取一个算术平均就得到了Macro-average

微平均Micro-average=(TP + FP) / (TP + TN + FP + FN)

分母就是输入分类器的预测样本个数，分子就是预测正确的样本个数（无论类别）

微平均实际上就是在文章开头说的accuracy




写到这里就差不多了，最后以一个例子来收尾把

考虑现在输入分类器的样本有10个，他们属于类别A B C

假设这10个样本的真实类标为（有序）和分类器预测的类标分别是： 

真实：A A A C B C A B B C


预测：A A C B A C A C B C

precision(A) = 3(正确预测为A类的样本个数为3) / 4(预测为A类的样本数为4) = 0.75  recall(A) = 3 / 4(真实A类样本有4个) = 0.75  

precision(B) = 1 / 2 = 0.5   recall(B) = 1 / 3 = 0.3333

precision(C) = 2 / 3 = 0.6667  recall(C) = 2 / 3 = 0.6667

F值计算出来之后，取算术平均就是Macro-average

Micro-average = 6(预测正确的样本个数) / 10 = 0.6




宏平均比微平均更合理，但也不是说微平均一无是处，具体使用哪种评测机制，还是要取决于数据集中样本分布




