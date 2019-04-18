# 最新论文阅读（24）--Log-DenseNet: How to Sparsify a DenseNet - wydbyxr的博客 - CSDN博客
2018年06月09日 19:01:23[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：231
# Log-DenseNet: How to Sparsify a DenseNet
```
- 2017年10月  
- Log-densenet  
- 微软研究院；卡耐基梅隆大学
```
　　在语义分割和目标识别上，优于densenet；速度上是L*log(L)，而densenet是L*L。根本的改变就是使用固定数目的skip-connect。
### 原始的densenet
![这里写图片描述](https://img-blog.csdn.net/20180609190029286?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 改进后的densenet，即log-densenet
![这里写图片描述](https://img-blog.csdn.net/20180609190055492?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018060919011215?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
