# 【IM】关于支持向量机分类的理解 - 专注于数据挖掘算法研究和应用 - CSDN博客
2018年10月10日 16:20:02[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：82
支持向量机分类的理解核心就是核方法以及二次规划最优求解。
SVM相关博文：
[https://blog.csdn.net/fjssharpsword/article/details/79965283](https://blog.csdn.net/fjssharpsword/article/details/79965283)
[https://blog.csdn.net/fjssharpsword/article/details/79064687](https://blog.csdn.net/fjssharpsword/article/details/79064687)
[https://blog.csdn.net/fjssharpsword/article/details/73801978](https://blog.csdn.net/fjssharpsword/article/details/73801978)
[https://blog.csdn.net/fjssharpsword/article/details/53787022](https://blog.csdn.net/fjssharpsword/article/details/53787022)
[https://blog.csdn.net/fjssharpsword/article/details/53784474](https://blog.csdn.net/fjssharpsword/article/details/53784474)
![](https://img-blog.csdn.net/20181010161902991?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20181010161919581?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
SVM在目标函数优化以及求解方法已有很多探索。
