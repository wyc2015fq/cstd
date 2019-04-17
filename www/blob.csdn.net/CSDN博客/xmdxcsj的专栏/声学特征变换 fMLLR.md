# 声学特征变换 fMLLR - xmdxcsj的专栏 - CSDN博客





2017年11月12日 16:29:24[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2716








## 含义

![这里写图片描述](https://img-blog.csdn.net/20171112162836203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

当测试数据$Y$和模型$\Lambda_x$不匹配的时候，可以通过变换的方式进行匹配[1]： 

- model-space 也就是$\Lambda_x$转化为$\Lambda_y$

- feature-space 也就是$Y$转化为$X$
其中model-space的变换又可以分为两种： 

- unconstrained: 均值和方差无关 

- constrained: 均值和方差变换是相同的形式
对于constrained model-space transformations，虽然出发点是对模型的均值和方差做转换，但是公式推导[2]最后的形式可以看成对输入的特征做线性变化，所以Constrained Maximum Likelihood Linear Regression (CMLLR)也称为feature-space MLLR (fMLLR)。 

fMLLR主要用于SAT（speaker adaptive training）训练，基本思想是训练得到的转化矩阵，使得adaptation数据在当前模型获得最大似然值。

## kaldi使用

steps/train_sat.sh

```
#1.获得trans
#  假设特征40维，每个speaker对应一个40*41维的矩阵
gmm-est-fmllr
#使用trans
transform-feats --utt2spk=ark:$sdata/JOB/utt2spk ark,s,cs:$dir/trans.JOB ark:- ark:- |
#查看trans矩阵
copy-matrix ark:trans.1 ark,t:trans.1.txt
```

## 参考

[1].A Maximum-Likelihood Approach to Stochastic Matching for Robust Speech Recognition 

[2].[Maximum likelihood linear transformations for HMM-based speech recognition](https://pdfs.semanticscholar.org/2109/f8f91301abec8497286160cd6b0f2e65ed05.pdf)





