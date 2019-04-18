# 机器学习知识点(二十六)概率图模型条件随机场CRF - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月16日 17:21:37[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2762
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









1、先看概率图模型谱系，有其是MFR和CRF的关系：

![](https://img-blog.csdn.net/20170516171400008?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这个图源自经典的文章《An Introduction to Conditional Random Fields》

地址：http://homepages.inf.ed.ac.uk/csutton/publications/crftut-fnt.pdf

能静心阅读和理解就好，这了先保存，还是需要借助课题来专题研究，否则只能是浮于定义认识。




2、再看看关于概率图模型的公式：

http://web.donga.ac.kr/yjko/usefulthings/MEMM&CRF.pdf 


这总结太专业了。

设X与Y是随机变量，P(Y|X)是给定条件X的条件下Y的条件概率分布，若随机变量Y构成一个由无向图G=(V,E)表示的马尔科夫随机场，则称条件概率分布P(X|Y)为条件随机场。





3、CRF应用于中文分词，有开源代码可以学习

http://www.hankcs.com/nlp/hanlp.html


https://github.com/hankcs/HanLP


可以看看里面CFR分词是什么实现的。






