# 高斯（exp）kernel 中缩放参数r的确定  度量矩阵到kernel矩阵的转换 - 家家的专栏 - CSDN博客





2014年05月08日 16:18:27[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1011
个人分类：[乐在科研中																[数学 Matlab 基本知识](https://blog.csdn.net/yihaizhiyan/article/category/749753)](https://blog.csdn.net/yihaizhiyan/article/category/742190)








function [kernel] = getkernel(dismatrix)

%根据sigma确定kernel中的gama参数



tmp = mean(mean(dismatrix))*0.5;

param.sigma = sqrt(tmp);

param.gamma     = 1/(2*param.sigma^2);



K = exp(-dismatrix*param.gamma);




end




