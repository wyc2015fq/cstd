# 最新论文阅读（22）--Learning Activation Functions to Improve Deep Neural Networks - wydbyxr的博客 - CSDN博客
2018年06月09日 18:55:32[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：55
个人分类：[深度学习论文阅读](https://blog.csdn.net/wydbyxr/article/category/7151097)
# Learning Activation Functions to Improve Deep Neural Networks
```
- ICLR2015  
- 新型激活函数APL 
- 加州大学欧文分校；Adobe
```
　　提出新的激活函数adaptive piecewise linear (APL)，其实就是“RELU+分段函数”，分段的参数是学习到的。
### 公式
![这里写图片描述](https://img-blog.csdn.net/20180609185511930?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### APL激活函数
　　1）输入为N*H*W，令它们为maps-1，每张为map-1 
　　2）每张map-1的每个点会通过K种不同的分段函数（故APL会有2KHW个参数），则一张map-1会得到K张map-2。再将这K张中间map-2进行element-sum，得到1张map-3。 
　　3）经过2），我们会得到N张map-3，对这N张map-3卷积 
![这里写图片描述](https://img-blog.csdn.net/20180609185450480?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
