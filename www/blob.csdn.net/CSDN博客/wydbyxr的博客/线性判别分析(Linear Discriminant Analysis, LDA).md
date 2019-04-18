# 线性判别分析(Linear Discriminant Analysis, LDA) - wydbyxr的博客 - CSDN博客
2018年10月22日 13:44:29[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：43
个人分类：[机器学习具体算法](https://blog.csdn.net/wydbyxr/article/category/7945743)
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# 线性判别分析(Linear Discriminant Analysis, LDA)
  是一种有监督的降维方法。与PCA齐名。
## 公式
![在这里插入图片描述](https://img-blog.csdn.net/20181022134402697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
  直观理解：将两类样本投影到一条直线上，使得投影后的类间散布矩阵与类内散布矩阵的比值最大。
