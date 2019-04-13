
# VGG16 - 机器学习的小学生 - CSDN博客


2018年03月03日 10:46:06[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：1782


**Models from the BMVC-2014 paper “Return of the Devil in the Details: Delving Deep into Convolutional Nets”**
The models are trained on the ILSVRC-2012 dataset. The details can be found on the project page or in the following BMVC-2014 paper:
> Return of the Devil in the Details: Delving Deep into Convolutional

> Nets K. Chatfield, K. Simonyan, A. Vedaldi, A. Zisserman British

> Machine Vision Conference, 2014 (arXiv ref. cs1405.3531)

> Please cite the paper if you use the models.

> Models:

> [VGG_CNN_S](https://gist.github.com/ksimonyan/fd8800eeb36e276cd6f9#file-readme-md)
> : 13.1% top-5 error on ILSVRC-2012-val

> [VGG_CNN_M](https://gist.github.com/ksimonyan/f194575702fae63b2829#file-readme-md)
> : 13.7% top-5 error on ILSVRC-2012-val

> [VGG_CNN_M_2048](https://gist.github.com/ksimonyan/78047f3591446d1d7b91#file-readme-md)
> : 13.5% top-5 error on ILSVRC-2012-val

> [VGG_CNN_M_1024](https://gist.github.com/ksimonyan/f0f3d010e6d5f0100274#file-readme-md)
> : 13.7% top-5 error on ILSVRC-2012-val

> [VGG_CNN_M_128](https://gist.github.com/ksimonyan/976847408258292576a1#file-readme-md)
> : 15.6% top-5 error on ILSVRC-2012-val

> [VGG_CNN_F](https://gist.github.com/ksimonyan/a32c9063ec8e1118221a#file-readme-md)
> : 16.7% top-5 error on ILSVRC-2012-val

> Models used by the VGG team in ILSVRC-2014

> The models are the improved versions of the models used by the VGG team in the ILSVRC-2014 competition. The details can be found on the project page or in the following arXiv paper:

> Very Deep Convolutional Networks for Large-Scale Image Recognition K.

> Simonyan, A. Zisserman arXiv:1409.1556

> Please cite the paper if you use the models.

> Models:

> [16-layer](https://gist.github.com/ksimonyan/211839e770f7b538e2d8#file-readme-md)
> : 7.5% top-5 error on ILSVRC-2012-val, 7.4% top-5 error on ILSVRC-2012-test

> [19-layer](https://gist.github.com/ksimonyan/3785162f95cd2d5fee77#file-readme-md)
> : 7.5% top-5 error on ILSVRC-2012-val, 7.3% top-5 error on ILSVRC-2012-test

> In the paper, the models are denoted as configurations D and E, trained with scale jittering. The combination of the two models achieves 7.1% top-5 error on ILSVRC-2012-val, and 7.0% top-5 error on ILSVRC-2012-test.

> VGG16 for face recognition: caffemodel,

> [http://www.robots.ox.ac.uk/~vgg/software/vgg_face/](http://www.robots.ox.ac.uk/~vgg/software/vgg_face/)

> 注释
> ：

> gist.github.com 被墙无法访问解决办法

> windows下 打开C:\Windows\System32\drivers\etc\hosts文件

> 编辑器打开，在最后行添加192.30.253.118 gist.github.com

> 保存。

> 从头开始训练，将参数的“gaussian”初始方式 修改为“xavier”, 否则训练的时候可能不会收敛。参数的初始化方式很重要。

> train.prototxt

> layer {
  name:
> "data"
> type:
> "Python"
> top:
> "Features"
> top:
> "Headposes"
> python_param {
    module:
> "python_read_data_for_softmax"
> layer:
> "AllDataLayer"
> param_str:
> "{\'phase\': \'train\', \'dataset_name\': \'lfw\', \'data_type\': \'image\',\'batch_size\': 48,\'cross_id\':0}"
> }
}
layer {  
  bottom:
> "Features"
> top:
> "conv1_1"
> name:
> "conv1_1"
> type:
> "Convolution"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  convolution_param {  
    num_output:
> 64
> pad:
> 1
> kernel_size:
> 3
> weight_filler {  
      type:
> "xavier"
> std:
> 0.01
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0
> }  
  }  
}  
layer {  
  bottom:
> "conv1_1"
> top:
> "conv1_1"
> name:
> "relu1_1"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "conv1_1"
> top:
> "conv1_2"
> name:
> "conv1_2"
> type:
> "Convolution"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  convolution_param {  
    num_output:
> 64
> pad:
> 1
> kernel_size:
> 3
> weight_filler {  
      type:
> "xavier"
> std:
> 0.01
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0
> }  
  }  
}  
layer {  
  bottom:
> "conv1_2"
> top:
> "conv1_2"
> name:
> "relu1_2"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "conv1_2"
> top:
> "pool1"
> name:
> "pool1"
> type:
> "Pooling"
> pooling_param {  
    pool: MAX  
    kernel_size:
> 2
> stride:
> 2
> }  
}  
layer {  
  bottom:
> "pool1"
> top:
> "conv2_1"
> name:
> "conv2_1"
> type:
> "Convolution"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  convolution_param {  
    num_output:
> 128
> pad:
> 1
> kernel_size:
> 3
> weight_filler {  
      type:
> "xavier"
> std:
> 0.01
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0
> }  
  }  
}  
layer {  
  bottom:
> "conv2_1"
> top:
> "conv2_1"
> name:
> "relu2_1"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "conv2_1"
> top:
> "conv2_2"
> name:
> "conv2_2"
> type:
> "Convolution"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  convolution_param {  
    num_output:
> 128
> pad:
> 1
> kernel_size:
> 3
> weight_filler {  
      type:
> "xavier"
> std:
> 0.01
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0
> }  
  }  
}  
layer {  
  bottom:
> "conv2_2"
> top:
> "conv2_2"
> name:
> "relu2_2"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "conv2_2"
> top:
> "pool2"
> name:
> "pool2"
> type:
> "Pooling"
> pooling_param {  
    pool: MAX  
    kernel_size:
> 2
> stride:
> 2
> }  
}  
layer {  
  bottom:
> "pool2"
> top:
> "conv3_1"
> name:
> "conv3_1"
> type:
> "Convolution"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  convolution_param {  
    num_output:
> 256
> pad:
> 1
> kernel_size:
> 3
> weight_filler {  
      type:
> "xavier"
> std:
> 0.01
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0
> }  
  }  
}  
layer {  
  bottom:
> "conv3_1"
> top:
> "conv3_1"
> name:
> "relu3_1"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "conv3_1"
> top:
> "conv3_2"
> name:
> "conv3_2"
> type:
> "Convolution"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  convolution_param {  
    num_output:
> 256
> pad:
> 1
> kernel_size:
> 3
> weight_filler {  
      type:
> "xavier"
> std:
> 0.01
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0
> }  
  }  
}  
layer {  
  bottom:
> "conv3_2"
> top:
> "conv3_2"
> name:
> "relu3_2"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "conv3_2"
> top:
> "conv3_3"
> name:
> "conv3_3"
> type:
> "Convolution"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  convolution_param {  
    num_output:
> 256
> pad:
> 1
> kernel_size:
> 3
> weight_filler {  
      type:
> "xavier"
> std:
> 0.01
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0
> }  
  }  
}  
layer {  
  bottom:
> "conv3_3"
> top:
> "conv3_3"
> name:
> "relu3_3"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "conv3_3"
> top:
> "pool3"
> name:
> "pool3"
> type:
> "Pooling"
> pooling_param {  
    pool: MAX  
    kernel_size:
> 2
> stride:
> 2
> }  
}  
layer {  
  bottom:
> "pool3"
> top:
> "conv4_1"
> name:
> "conv4_1"
> type:
> "Convolution"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  convolution_param {  
    num_output:
> 512
> pad:
> 1
> kernel_size:
> 3
> weight_filler {  
      type:
> "xavier"
> std:
> 0.01
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0
> }  
  }  
}  
layer {  
  bottom:
> "conv4_1"
> top:
> "conv4_1"
> name:
> "relu4_1"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "conv4_1"
> top:
> "conv4_2"
> name:
> "conv4_2"
> type:
> "Convolution"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  convolution_param {  
    num_output:
> 512
> pad:
> 1
> kernel_size:
> 3
> weight_filler {  
      type:
> "xavier"
> std:
> 0.01
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0
> }  
  }  
}  
layer {  
  bottom:
> "conv4_2"
> top:
> "conv4_2"
> name:
> "relu4_2"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "conv4_2"
> top:
> "conv4_3"
> name:
> "conv4_3"
> type:
> "Convolution"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  convolution_param {  
    num_output:
> 512
> pad:
> 1
> kernel_size:
> 3
> weight_filler {  
      type:
> "xavier"
> std:
> 0.01
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0
> }  
  }  
}  
layer {  
  bottom:
> "conv4_3"
> top:
> "conv4_3"
> name:
> "relu4_3"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "conv4_3"
> top:
> "pool4"
> name:
> "pool4"
> type:
> "Pooling"
> pooling_param {  
    pool: MAX  
    kernel_size:
> 2
> stride:
> 2
> }  
}  
layer {  
  bottom:
> "pool4"
> top:
> "conv5_1"
> name:
> "conv5_1"
> type:
> "Convolution"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  convolution_param {  
    num_output:
> 512
> pad:
> 1
> kernel_size:
> 3
> weight_filler {  
      type:
> "xavier"
> std:
> 0.01
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0
> }  
  }  
}  
layer {  
  bottom:
> "conv5_1"
> top:
> "conv5_1"
> name:
> "relu5_1"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "conv5_1"
> top:
> "conv5_2"
> name:
> "conv5_2"
> type:
> "Convolution"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  convolution_param {  
    num_output:
> 512
> pad:
> 1
> kernel_size:
> 3
> weight_filler {  
      type:
> "xavier"
> std:
> 0.01
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0
> }  
  }  
}  
layer {  
  bottom:
> "conv5_2"
> top:
> "conv5_2"
> name:
> "relu5_2"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "conv5_2"
> top:
> "conv5_3"
> name:
> "conv5_3"
> type:
> "Convolution"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  convolution_param {  
    num_output:
> 512
> pad:
> 1
> kernel_size:
> 3
> weight_filler {  
      type:
> "xavier"
> std:
> 0.01
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0
> }  
  }  
}  
layer {  
  bottom:
> "conv5_3"
> top:
> "conv5_3"
> name:
> "relu5_3"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "conv5_3"
> top:
> "pool5"
> name:
> "pool5"
> type:
> "Pooling"
> pooling_param {  
    pool: MAX  
    kernel_size:
> 2
> stride:
> 2
> }  
}  
layer {  
  bottom:
> "pool5"
> top:
> "fc6"
> name:
> "fc6"
> type:
> "InnerProduct"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  inner_product_param {  
    num_output:
> 4096
> weight_filler {  
      type:
> "xavier"
> std:
> 0.005
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0.1
> }  
  }  
}  
layer {  
  bottom:
> "fc6"
> top:
> "fc6"
> name:
> "relu6"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "fc6"
> top:
> "fc6"
> name:
> "drop6"
> type:
> "Dropout"
> dropout_param {  
    dropout_ratio:
> 0.5
> }  
}  
layer {  
  bottom:
> "fc6"
> top:
> "fc7"
> name:
> "fc7"
> type:
> "InnerProduct"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  inner_product_param {  
    num_output:
> 4096
> weight_filler {  
      type:
> "xavier"
> std:
> 0.005
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0.1
> }  
  }  
}  
layer {  
  bottom:
> "fc7"
> top:
> "fc7"
> name:
> "relu7"
> type:
> "ReLU"
> }  
layer {  
  bottom:
> "fc7"
> top:
> "fc7"
> name:
> "drop7"
> type:
> "Dropout"
> dropout_param {  
    dropout_ratio:
> 0.5
> }  
}  
layer {  
  bottom:
> "fc7"
> top:
> "fc8"
> name:
> "fc8"
> type:
> "InnerProduct"
> param {  
    lr_mult:
> 1
> decay_mult:
> 1
> }  
  param {  
    lr_mult:
> 2
> decay_mult:
> 0
> }  
  inner_product_param {  
    num_output:
> 5
> weight_filler {  
      type:
> "xavier"
> std:
> 0.005
> }  
    bias_filler {  
      type:
> "constant"
> value:
> 0.1
> }  
  }  
}  
layer {  
  bottom:
> "fc8"
> bottom:
> "Headposes"
> top:
> "headposeloss1"
> name:
> "headposeloss1"
> type:
> "SoftmaxWithLoss"
> }
> solver.prototxt:

> train_net:
> "ZOO_VGG16/train2.prototxt"
> base_lr:
> 0.01
> display:
> 20
> max_iter:
> 100000
> lr_policy:
> "step"
> gamma:
> 0.1
> momentum:
> 0.9
> weight_decay:
> 0.0005
> stepsize:
> 50000
> snapshot:
> 5000
> snapshot_prefix:
> "snapshots_pointing04_vgg16_klLoss/cross1/vgg_"
> random_seed:
> 831486
> iter_size:
> 1
> 参考文献：

> [https://github.com/NVIDIA/DIGITS/issues/159\#issuecomment-247707549](https://github.com/NVIDIA/DIGITS/issues/159#issuecomment-247707549)

> [https://github.com/NVIDIA/DIGITS/issues/535](https://github.com/NVIDIA/DIGITS/issues/535)

> [https://stackoverflow.com/questions/42652903/caffe-net-not-converging-when-replacing-alexnet-with-vgg16-but-everything-else?rq=1](https://stackoverflow.com/questions/42652903/caffe-net-not-converging-when-replacing-alexnet-with-vgg16-but-everything-else?rq=1)

> [https://github.com/BVLC/caffe/wiki/Model-Zoo](https://github.com/BVLC/caffe/wiki/Model-Zoo)

> [https://www.cnblogs.com/ryans/p/8196151.html](https://www.cnblogs.com/ryans/p/8196151.html)


