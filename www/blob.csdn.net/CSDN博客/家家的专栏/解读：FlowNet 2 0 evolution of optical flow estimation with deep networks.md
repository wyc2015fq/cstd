# 解读：FlowNet 2.0 evolution of optical flow estimation with deep networks - 家家的专栏 - CSDN博客





2017年11月30日 22:19:51[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：431








贡献点:

1. focus on the **training data** and show that the schedule of presenting data during training is very important. 

2. develop **a stacked architecture** that includes warping of the second image with intermediate optical flow. 

3. we elaborate on small displacements by introducing a sub-network specializing on small motions. 




实验结果：

FlowNet 2.0 is only marginally **slower **than the original FlowNet but decreases the
**estimation error by more than 50%**.









