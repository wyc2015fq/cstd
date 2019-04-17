# kernel 矩阵的规则化 - 家家的专栏 - CSDN博客





2013年11月07日 10:22:04[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1233











K_norm = K./repmat(sqrt(sum(K.^2)),[size(K,1) 1]);

其中K是kernel矩阵，



