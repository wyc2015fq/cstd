# kaldi feature extraction - xmdxcsj的专栏 - CSDN博客





2017年02月25日 17:37:53[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2014








支持提取mfcc/plp特征，和HTK特征的主要差别在于默认参数值的设定。使用选项-htk-compat=true以及合适的参数，可以得到htk特征。

## 全局CMVN

### compute-cmvn-stats

假设帧数为i，特征维度为j，cmvn计算得到两个矩阵： 


$(A_1, A_2,...,A_j,A_{j+1})$


$(B_1, B_2,...,B_j)$

第$A_{j+1}$表示总的帧数N 

假设特征为$f_{ij}$，其中变量的计算公式如下： 


$A_j=\sum_{i} f_{ij}$


$B_j=\sum_{i} f_{ij}^2$
### apply-cmvn

假设均值和方差分别是$M_j$和$V_j$，特征$f_j$经过CMVN归一化以后变成： 


$f'_{j}=\frac{f_{j}-M_j}{V_j}$

其中$M_J$表示均值，$V_j$表示标准差： 


$M_j=\frac{A_j}{N}$


$V_j=\sqrt{\frac{B_j}{N}-M_j^2}$

方差的计算展开可以得到： 


$V_j=\sqrt{\frac{\sum_i (f_{ij} - M_j)^2}{N}}=\sqrt{\frac{\sum_i f_{ij}^2}{N}-M_j^2}$




