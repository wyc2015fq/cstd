# 最新论文阅读（6）--Jointly Attentive Spatial-Temporal Pooling Networks for Video-based Person - wydbyxr的博客 - CSDN博客
2018年06月06日 21:47:42[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：382
所属专栏：[深度学习--最新论文](https://blog.csdn.net/column/details/23683.html)
# Jointly Attentive Spatial-Temporal Pooling Networks for Video-based Person
```
- 2017年9月   
- 用于行人重检验的，ASTPN
- 华中科大；IBM；西北大学【美】
```
　　joint Spatial and Temporal Attention Pooling Network。 
　　spatial pooling就是spatial pyramid pooling，它有不同尺度的spatial bins (8*8, 4*4, 2*2 and 1*1)； 
　　Attentive temporal pooling分为Row-wise Max Pooling和column-wise Max Pooling。 
　　网络结构是CNN+RNN，孪生网络。得到的两个特征向量会在欧式空间中计算距离，该距离就是最终结果。
![这里写图片描述](https://img-blog.csdn.net/20180606214656451?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180606214709186?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180606214723794?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
