# kaldi 1d-CNN源码 - xmdxcsj的专栏 - CSDN博客





2017年03月15日 21:33:43[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1551








## CNN

> 
nnet/nnet-convolutional-component.h


假设CNN相关参数如下

```
###输入相关参数
num_splice=3                                            #语音数据的左右context，此处左右一帧
path_stride=5                                           #一帧特征的维度
input_dim=path_stride*num_splice=15                     #输入特征的维度

###滤波器相关参数
num_filters=2                                           #滤波器个数
patch_dim=2                                             #滤波器在一帧特征上面的维度
filter_dim=num_splice*patch_dim=6                       #滤波器在3帧特征上的维度
patch_step=1                                            #patch移动的步长

###输出相关参数
num_patch=1 + (patch_stride - patch_dim) / patch_step=4 #每个滤波器在特征上面做卷积的次数（即输出个数）
output_dim=num_patch*num_filters=8                      #cnn的输出维度

###多帧并行的参数
frame_num=2                                             #一次计算特征的个数
```

### PropagateFnc

首先将特征按照splice重新组织成vectorized_feature_patches，然后跟对应的filters做卷积 
![这里写图片描述](https://img-blog.csdn.net/20170315213153352?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170315213208654?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### BackpropagateFnc

计算对输入的梯度（向后继续传播） 
![这里写图片描述](https://img-blog.csdn.net/20170315213234733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### Update

计算对参数的梯度（用于参数更新） 
![这里写图片描述](https://img-blog.csdn.net/20170315213259915?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## Max-Pooling

在后向传递梯度的时候只在最大值对应的边传递，其他值的梯度为0.






