# 最新论文阅读（8）--DiracNets: Training Very Deep Neural Networks Without Skip-Connections - wydbyxr的博客 - CSDN博客
2018年06月07日 20:11:51[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：203
所属专栏：[深度学习--最新论文](https://blog.csdn.net/column/details/23683.html)
# DiracNets: Training Very Deep Neural Networks Without Skip-Connections
```
- 2017年6月   
- 提出NCReLU和Dirac参数化，它适用于广泛的网络架构
- ParisTech(巴黎高科，一个小学校)
```
　　提出新的权重参数化（weight parameterization）方法，即Dirac参数化，它适用于广泛的网络架构。 
提出NCReLU，类似CReLU。 
　　论文中，实验了没有使用跳层连接的Diracnet（VGG-like），发现它在34层的时候就好于resnet101。 
　　Dirac参数化： 
![这里写图片描述](https://img-blog.csdn.net/20180607201051989?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180607201102334?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
　　NCReLU： 
![这里写图片描述](https://img-blog.csdn.net/2018060720112896?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
