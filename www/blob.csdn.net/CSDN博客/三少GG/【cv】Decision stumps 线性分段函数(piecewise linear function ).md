# 【cv】Decision stumps||线性分段函数(piecewise linear function )|| - 三少GG - CSDN博客
2011年06月01日 11:45:00[三少GG](https://me.csdn.net/scut1135)阅读数：2300
1.   Decision stumps的概念
[http://www.answers.com/topic/decision-stump](http://www.answers.com/topic/decision-stump)
A **decision stump** is a [machine learning](http://www.answers.com/topic/machine-learning) model consisting of a one-level[decision tree](http://www.answers.com/topic/decision-tree-learning).[[1]](http://www.answers.com/topic/decision-stump#cite_note-IL92-0)
 That is, it is a decision tree with one internal node (the root) which is immediately connected to the terminal nodes. A decision stump makes a prediction based on the value of just a single input feature. Sometimes they are also called**1-rules**.[[2]](http://www.answers.com/topic/decision-stump#cite_note-1)
[](http://en.wikipedia.org/wiki/File:Decision_stump.svg)
Depending on the type of the input feature, several variations are possible. For nominal features, one may build a stump which contains a leaf for each possible feature value[[3]](http://www.answers.com/topic/decision-stump#cite_note-2)[[4]](http://www.answers.com/topic/decision-stump#cite_note-3)
 or a stump with the two leaves, one of which corresponds to some chosen category, and the other leaf to all the other categories.[[5]](http://www.answers.com/topic/decision-stump#cite_note-weka-4)
 For binary features these two schemes are identical. A missing value may be treated as a yet another category.[[5]](http://www.answers.com/topic/decision-stump#cite_note-weka-4)
For continuous features, usually, some threshold feature value is selected, and the stump contains two leaves — for values below and above the threshold. However, rarely, multiple thresholds may be chosen and the stump therefore contains three or more leaves.
Decision stumps are often[[6]](http://www.answers.com/topic/decision-stump#cite_note-5) used as components (called "weak learners" or "base
 learners") in [machine learning ensemble](http://www.answers.com/topic/machine-learning-ensemble) techniques such as[bagging](http://www.answers.com/topic/bootstrap-aggregating-1) and
[boosting](http://www.answers.com/topic/boosting). For example, a state-of-the-art
[Viola-Jones](http://www.answers.com/topic/robust-real-time-object-detection) face detection algorithm employs[AdaBoost](http://www.answers.com/topic/adaboost) with decision stumps as weak learners.[[7]](http://www.answers.com/topic/decision-stump#cite_note-6)
The term "decision stump" has been coined in a 1992 [ICML](http://www.answers.com/topic/international-conference-on-machine-learning) paper by Wayne Iba and Pat Langley.[[1]](http://www.answers.com/topic/decision-stump#cite_note-IL92-0)[[8]](http://www.answers.com/topic/decision-stump#cite_note-7)
## References
- ^ [**a**](http://www.answers.com/topic/decision-stump#cite_ref-IL92_0-0)[**b**](http://www.answers.com/topic/decision-stump#cite_ref-IL92_0-1) Wayne Iba and Pat Langley. (1992).[Induction
 of One-Level Decision Trees.](http://lyonesse.stanford.edu/~langley/papers/stump.ml92.pdf) Proceedings of the Ninth International Conference on Machine Learning.
- **[^](http://www.answers.com/topic/decision-stump#cite_ref-1)**Robert C. Holte (1993).
["Very Simple Classification Rules Perform Well on Most Commonly Used Datasets"](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.67.2711&rep=rep1&type=pdf).[http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.67.2711&rep=rep1&type=pdf](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.67.2711&rep=rep1&type=pdf).
- **[^](http://www.answers.com/topic/decision-stump#cite_ref-2)**Loper, Edward L.; Bird, Steven; Klein, Ewan (2009).
[*Natural language processing with Python*](http://nltk.googlecode.com/svn/trunk/doc/book/ch06.html). Sebastopol, CA:[O'Reilly](http://www.answers.com/topic/o-reilly-media).
[ISBN](http://www.answers.com/topic/international-standard-book-number)[0-596-51649-5](http://en.wikipedia.org/wiki/Special:BookSources/0-596-51649-5).[http://nltk.googlecode.com/svn/trunk/doc/book/ch06.html](http://nltk.googlecode.com/svn/trunk/doc/book/ch06.html).
- **[^](http://www.answers.com/topic/decision-stump#cite_ref-3)** This classifier is implemented in[Weka](http://www.answers.com/topic/weka-machine-learning)
 under the name `OneR` (for "1-rule").
- ^ [**a**](http://www.answers.com/topic/decision-stump#cite_ref-weka_4-0)[**b**](http://www.answers.com/topic/decision-stump#cite_ref-weka_4-1) This is what has been implemented in[Weka](http://www.answers.com/topic/weka-machine-learning)'s
`DecisionStump` classifier.
- **[^](http://www.answers.com/topic/decision-stump#cite_ref-5)** Lev Reyzin and Robert E. Schapire. (2006).[How
 Boosting the Margin Can Also Boost Classifier Complexity.](http://www.cs.princeton.edu/~schapire/papers/boost_complexity.pdf) ICML 2006. Page 7.
- **[^](http://www.answers.com/topic/decision-stump#cite_ref-6)** Paul Viola and Michael J. Jones. (2004).[Robust
 Real-Time Face Detection.](http://www.face-rec.org/algorithms/Boosting-Ensemble/16981346.pdf) International Journal of Computer Vision, 2004.
- **[^](http://www.answers.com/topic/decision-stump#cite_ref-7)** Jonathan Oliver and[David
 Hand](http://www.answers.com/topic/david-hand-statistician). (1994). *Averaging Over Decision Stumps*. ECML 1994.[doi](http://www.answers.com/topic/digital-object-identifier):[10.1007/3-540-57868-4_61](http://dx.doi.org/10.1007%2F3-540-57868-4_61)
Quote: "These simple rules are in effect severely pruned decision trees and have been termed*decision stumps* [cites Iba and Langley]".
/////////////////////////////////////
[Class Project Discussion](http://gogoshen.org/ml/Lists/General%20Discussion/AllItems.aspx): What is a decision stump? -- a decision stump...
What is a decision stump? -- a decision stump is a decision tree with only one node, and only one branch coming out of the node.
Text: This is best illustrated using Weka Explorer on the nominal-Weather dataset. Attached are 2 "Weka Explorer Result files:"
(1) The OneR (i.e., One-Rule) is essentially a single node decision tree. The node has as many output branches as there are values for the attribute assigned to the node.
(2) The Decision-Stump is a more limited single node decision tree. In contrast to One-R, the single node can only have one output branch.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
2.  **Dicision Tree**
[http://superangevil.wordpress.com/2009/11/12/nltk6_4/](http://superangevil.wordpress.com/2009/11/12/nltk6_4/)
**(1) Dicision Treedecision tree**就是一个简单的*流程图*，它从**root** node开始，自顶向下，根据每个decision node对当前输入的决策选择不同的分枝，重复迭代，一直到**leaf** node.
总体来说，决策树学习着眼于从一组无次序、无规则的事例中推理出决策树形式的分类规则 — 从根结点到叶结点的一条路径就对应一条合取规则；整棵决策树就对应一组析取表达式规则.
看我们的gender classifier的决策树：
**decision stump**是一棵单点的decision tree, 它基于单个feature对输入进行分类，对每个可能的feature, 它都包含一个leaf，用来指明该feature的class label.
为了建立一个decision stump, 我们必须首先决定用哪一个feature:
最简单的方法是为每个可能的feature都建一个decision stump, 看哪个feature在training set上的accuracy分数高就用哪个（当然还有很多其他方法）;一旦我们选定了feature, 就可以将每个leaf都标为频率最高的label.
Decision Stump的选择算法如下：
>     首先为分类任务选择全局最好的decision stump
    然后在training set上检测每个leaf node的accuracy
         对于accuracy不好的leaf nodes，在该叶结点路径的训练语料库子集上重新训练获得新的decision stump替换原有decision stump
3. Piecewise linear function
[http://en.wikipedia.org/wiki/Piecewise_linear_function](http://en.wikipedia.org/wiki/Piecewise_linear_function)
