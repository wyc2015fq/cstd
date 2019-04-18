# NN模型设置--反卷积层的参数设置 - wydbyxr的博客 - CSDN博客
2018年12月06日 10:53:50[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：109
  将反卷积层的学习率设为0，upsample的方式就是默认的双线性插值，当然也可以设置学习率不为0，让反卷积层自己学习参数（听说：学习率不为0，能让网络的效果有略微提升）。
  转置卷积层的stride最好别设置为和kernal相等，最好小一些，比如1/2；
  如果遇到了上图的马赛克现象，就将反卷积层的weight_filler设为双线性插值；而不要设为高斯。例：
```
layer {
  name: ""deconv2_1""
  type: ""Deconvolution""
  bottom: ""conv2_d""
  top: ""deconv2_1""
  param {
    lr_mult: 0
  }
  convolution_param { 
    num_output: 32
    bias_term: false
    kernel_size: 4
    stride: 2
    weight_filler {      
    type: ""bilinear""
    }
#    weight_filler {      
#    type: ""gaussian""      
#    std: 0.01    
#    }
#    bias_filler { 
#    type: ""constant"" 
#    value: 0 
#    }
  }
}
```
