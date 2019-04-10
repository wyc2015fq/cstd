# 在caffe 中实现Generative Adversarial Nets（一）

2016年11月01日 21:30:50 [seven_first](https://me.csdn.net/seven_first) 阅读数：12727



# 目录



- 目录
  - - 一Generative Adversarial Nets
      - [GAN简介](https://blog.csdn.net/seven_first/article/details/52999758#1-gan%E7%AE%80%E4%BB%8B)
      - [GAN训练过程](https://blog.csdn.net/seven_first/article/details/52999758#2-gan%E8%AE%AD%E7%BB%83%E8%BF%87%E7%A8%8B)
    - 二Deep Convolutional GANs DCGAN
      - [DCGAN 网络结构](https://blog.csdn.net/seven_first/article/details/52999758#1-dcgan-%E7%BD%91%E7%BB%9C%E7%BB%93%E6%9E%84)
      - [DCGAN caffe prototxt](https://blog.csdn.net/seven_first/article/details/52999758#2-dcgan-caffe-prototxt)
      - [生成结果](https://blog.csdn.net/seven_first/article/details/52999758#3%E7%94%9F%E6%88%90%E7%BB%93%E6%9E%9C)



### 一、Generative Adversarial Nets

#### 1. GAN简介

对抗生成网络（GAN）同时训练两个模型：能够得到数据分布的生成模型（generative model G）和能判够区别数据是生成的还是真实的判别模型 （discriminative model D）。训练过程使得G生成的数据尽可能真实，同时又使得D尽可能能够区分生成的数据和真实的数据，最终G生成数据足以以假乱真，而D输出数据的概率均为0.5 。 参考论文：Bengio大神的 [Generative Adversarial Networks](https://arxiv.org/pdf/1406.2661v1.pdf)

#### 2. GAN训练过程

![这里写图片描述](https://img-blog.csdn.net/20161101194945846)
注意：这里的loss在更新D梯度是上升方向，在caffe具体实现时，为了使得D模型梯度更新为梯度的下降方向，loss等价改为: loss=−[log(D(x(i))+log(1−D(G(z(i))))]loss=−[log⁡(D(x(i))+log⁡(1−D(G(z(i))))].

### 二、Deep Convolutional GANs (DCGAN)

#### 1. DCGAN 网络结构

DCGAN 用卷积神经（CNN）代替GAN中用多层感知器（MLP）实现D和G，后生成图片效果显著提升。参考论文[Unsupervised Representation Learning with Deep Convolutional Generative Adversarial Neworks](https://arxiv.org/pdf/1511.06434v2.pdf)

- DCGAN网络结构图 
  ![这里写图片描述](https://img-blog.csdn.net/20161101202626052)
- DCGAN结构内容 
  ![这里写图片描述](https://img-blog.csdn.net/20161101202856014)

#### 2. DCGAN caffe prototxt

1.train.prototxt

```
#  Create on: 2016/10/22 ShanghaiTech
#  Author:    Yingying Zhang
name: "gan_newface"
layer {
  name: "images"
  type: "ImageData"
  top: "face_images"
  top: "label"
  include {
    phase: TRAIN
  }
  transform_param {
    mirror: true
    mean_value: [127.5]
    scale: 0.00784314
  }
  image_data_param {
    source: "data/face_data.txt"
    root_folder: "data/"
    batch_size: 128
    new_height: 64
    new_width: 64
    is_color: true
    shuffle: true
  }
}
layer {
  name: "rand_vec"
  type: "RandVec"
  top: "rand_vec"
  rand_vec_param {
    batch_size: 128
    dim: 100
    lower: -1.0
    upper: 1.0
    repeat: 3
  }
}
layer {
  name: "silence"
  type: "Silence"
  bottom: "label"
}
#----------- generative -----------
layer {
  name: "ip1"
  type: "InnerProduct"
  bottom: "rand_vec"
  top: "ip1"
  param {
    name: "ip_w_g"
    lr_mult: 1
  }
  param {
    name: "ip_b_g"
    lr_mult: 2
  }
  inner_product_param{
    num_output: 16384
    gen_mode: true
    weight_filler {
      type: "gaussian"
      std: 0.02
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
layer {
  name: "ip1_reshape"
  type: "Reshape"
  bottom: "ip1"
  top: "ip1_reshape"
  reshape_param {
    shape {
      dim: 0
      dim: 1024
      dim: 4
      dim: 4
    }
  }
}
layer {
  name: "batch_norm_g1"
  type: "BatchNorm"
  bottom: "ip1_reshape"
  top: "ip1_reshape"
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  batch_norm_param {
    use_global_stats: false
  }
}
layer {
  name: "scale_batch_g1"
  type: "Scale"
  bottom: "ip1_reshape"
  top: "ip1_reshape"
  param {
    name: "gen_s1"
    lr_mult: 1
    decay_mult: 1
  }
  param {
    name: "gen_b1"
    lr_mult: 1
    decay_mult: 1
  }
  scale_param {
    bias_term: true
    gen_mode: true
  }
}
layer {
  name: "relu_g1"
  type: "ReLU"
  bottom: "ip1_reshape"
  top: "ip1_reshape"
}
layer {
  name: "gen_conv2"
  type: "Deconvolution"
  bottom: "ip1_reshape"
  top: "gen_conv2"
  param {
    name: "gen_w_2"
    lr_mult: 1
  }
  param {
    name: "gen_b_2"
    lr_mult: 2
  }
  convolution_param {
    num_output: 512
    pad: 2
    kernel_size: 5
    stride: 2
    gen_mode: true
    shape_offset: [1, 1]
    weight_filler {
      type: "gaussian"
      std: 0.02
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
layer {
  name: "batch_norm_g2"
  type: "BatchNorm"
  bottom: "gen_conv2"
  top: "gen_conv2"
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  batch_norm_param {
    use_global_stats: false
  }
}
layer {
  name: "scale_batch_g2"
  type: "Scale"
  bottom: "gen_conv2"
  top: "gen_conv2"
  param {
    name: "gen_s2"
    lr_mult: 1
    decay_mult: 1
  }
  param {
    name: "gen_b2"
    lr_mult: 1
    decay_mult: 1
  }
  scale_param {
    gen_mode: true
    bias_term: true
  }
}
layer {
  name: "relu_g2"
  type: "ReLU"
  bottom: "gen_conv2"
  top: "gen_conv2"
}
layer {
  name: "gen_conv3"
  type: "Deconvolution"
  bottom: "gen_conv2"
  top: "gen_conv3"
  param {
    name: "gen_w_3"
    lr_mult: 1
  }
  param {
    name: "gen_b_3"
    lr_mult: 2
  }
  convolution_param {
    num_output: 256
    pad: 2
    kernel_size: 5
    stride: 2
    gen_mode: true
    shape_offset: [1, 1]
    weight_filler {
      type: "gaussian"
      std: 0.02
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
layer {
  name: "batch_norm_g3"
  type: "BatchNorm"
  bottom: "gen_conv3"
  top: "gen_conv3"
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  batch_norm_param {
    use_global_stats: false
  }
}
layer {
  name: "scale_batch_g3"
  type: "Scale"
  bottom: "gen_conv3"
  top: "gen_conv3"
  param {
    name: "gen_s3"
    lr_mult: 1
    decay_mult: 1
  }
  param {
    name: "gen_b3"
    lr_mult: 1
    decay_mult: 1
  }
  scale_param {
    gen_mode: true
    bias_term: true
  }
}
layer {
  name: "relu_g3"
  type: "ReLU"
  bottom: "gen_conv3"
  top: "gen_conv3"
}
layer {
  name: "gen_conv4"
  type: "Deconvolution"
  bottom: "gen_conv3"
  top: "gen_conv4"
  param {
    name: "gen_w_4"
    lr_mult: 1
  }
  param {
    name: "gen_b_4"
    lr_mult: 2
  }
  convolution_param {
    num_output: 128
    pad: 2
    kernel_size: 5
    stride: 2
    gen_mode: true
    shape_offset: [1, 1]
    weight_filler {
      type: "gaussian"
      std: 0.02
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
layer {
  name: "batch_norm_g4"
  type: "BatchNorm"
  bottom: "gen_conv4"
  top: "gen_conv4"
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  batch_norm_param {
    use_global_stats: false
  }
}
layer {
  name: "scale_batch_g4"
  type: "Scale"
  bottom: "gen_conv4"
  top: "gen_conv4"
  param {
    name: "gen_s4"
    lr_mult: 1
    decay_mult: 1
  }
  param {
    name: "gen_b4"
    lr_mult: 1
    decay_mult: 1
  }
  scale_param {
    gen_mode: true
    bias_term: true
  }
}
layer {
  name: "relu_g4"
  type: "ReLU"
  bottom: "gen_conv4"
  top: "gen_conv4"
}
layer {
  name: "gen_conv5"
  type: "Deconvolution"
  bottom: "gen_conv4"
  top: "gen_conv5"
  param {
    name: "gen_w_5"
    lr_mult: 1
  }
  param {
    name: "gen_b_5"
    lr_mult: 2
  }
  convolution_param {
    num_output: 3
    pad: 2
    kernel_size: 5
    stride: 2
    gen_mode: true
    shape_offset: [1, 1]
    weight_filler {
      type: "gaussian"
      std: 0.02
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
layer {
  name: "tanh_g5"
  type: "TanH"
  bottom: "gen_conv5"
  top: "gen_conv5"
}
#-----------   gan gate  ------------
layer {
  name: "gan_gate"
  type: "GANGate"
  bottom: "face_images"
  bottom: "gen_conv5"
  top: "dis_input"
}
#----------- discrimitive -----------
layer {
  name: "dis_conv_d1"
  type: "Convolution"
  bottom: "dis_input"
  top: "dis_conv_d1"
  param {
    name: "dis_w_1"
    lr_mult: 1
  }
  param {
    name: "dis_b_1"
    lr_mult: 2
  }
  convolution_param {
    num_output: 128
    pad: 2
    kernel_size: 5
    stride: 2
    dis_mode: true
    weight_filler {
      type: "gaussian"
      std: 0.02
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
layer {
  name: "batch_norm_d1"
  type: "BatchNorm"
  bottom: "dis_conv_d1"
  top: "dis_conv_d1"
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  batch_norm_param {
    use_global_stats: false
  }
}
layer {
  name: "scale_batch_d1"
  type: "Scale"
  bottom: "dis_conv_d1"
  top: "dis_conv_d1"
  param {
    name: "dis_s1"
    lr_mult: 1
    decay_mult: 1
  }
  param {
    name: "dis_b1"
    lr_mult: 1
    decay_mult: 1
  }
  scale_param {
    dis_mode: true
    bias_term: true
  }
}
layer {
  name: "relu_d1"
  type: "ReLU"
  bottom: "dis_conv_d1"
  top: "dis_conv_d1"
  relu_param {
    negative_slope: 0.2
  }
}
layer {
  name: "dis_conv_d2"
  type: "Convolution"
  bottom: "dis_conv_d1"
  top: "dis_conv_d2"
  param {
    name: "dis_w_2"
    lr_mult: 1
  }
  param {
    name: "dis_b_2"
    lr_mult: 2
  }
  convolution_param {
    num_output: 256
    pad: 2
    kernel_size: 5
    stride: 2
    dis_mode: true
    weight_filler {
      type: "gaussian"
      std: 0.02
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
layer {
  name: "batch_norm_d2"
  type: "BatchNorm"
  bottom: "dis_conv_d2"
  top: "dis_conv_d2"
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  batch_norm_param {
    use_global_stats: false
  }
}
layer {
  name: "scale_batch_d2"
  type: "Scale"
  bottom: "dis_conv_d2"
  top: "dis_conv_d2"
  param {
    name: "dis_s2"
    lr_mult: 1
    decay_mult: 1
  }
  param {
    name: "dis_b2"
    lr_mult: 1
    decay_mult: 1
  }
  scale_param {
    dis_mode: true
    bias_term: true
  }
}
layer {
  name: "relu_d2"
  type: "ReLU"
  bottom: "dis_conv_d2"
  top: "dis_conv_d2"
  relu_param {
    negative_slope: 0.2
  }
}
layer {
  name: "dis_conv_d3"
  type: "Convolution"
  bottom: "dis_conv_d2"
  top: "dis_conv_d3"
  param {
    name: "dis_w_3"
    lr_mult: 1
  }
  param {
    name: "dis_b_3"
    lr_mult: 2
  }
  convolution_param {
    num_output: 512
    pad: 2
    kernel_size: 5
    stride: 2
    dis_mode: true
    weight_filler {
      type: "gaussian"
      std: 0.02
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
layer {
  name: "batch_norm_d3"
  type: "BatchNorm"
  bottom: "dis_conv_d3"
  top: "dis_conv_d3"
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  batch_norm_param {
    use_global_stats: false
  }
}
layer {
  name: "scale_batch_d3"
  type: "Scale"
  bottom: "dis_conv_d3"
  top: "dis_conv_d3"
  param {
    name: "dis_s3"
    lr_mult: 1
    decay_mult: 1
  }
  param {
    name: "dis_b3"
    lr_mult: 1
    decay_mult: 1
  }
  scale_param {
    dis_mode: true
    bias_term: true
  }
}
layer {
  name: "relu_d3"
  type: "ReLU"
  bottom: "dis_conv_d3"
  top: "dis_conv_d3"
  relu_param {
    negative_slope: 0.2
  }
}
layer {
  name: "dis_conv_d4"
  type: "Convolution"
  bottom: "dis_conv_d3"
  top: "dis_conv_d4"
  param {
    name: "dis_w_4"
    lr_mult: 1
  }
  param {
    name: "dis_b_4"
    lr_mult: 2
  }
  convolution_param {
    num_output: 1024
    pad: 2
    kernel_size: 5
    stride: 2
    dis_mode: true
    weight_filler {
      type: "gaussian"
      std: 0.02
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
layer {
  name: "batch_norm_d4"
  type: "BatchNorm"
  bottom: "dis_conv_d4"
  top: "dis_conv_d4"
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  batch_norm_param {
    use_global_stats: false
  }
}
layer {
  name: "scale_batch_d4"
  type: "Scale"
  bottom: "dis_conv_d4"
  top: "dis_conv_d4"
  param {
    name: "dis_s4"
    lr_mult: 1
    decay_mult: 1
  }
  param {
    name: "dis_b4"
    lr_mult: 1
    decay_mult: 1
  }
  scale_param {
    dis_mode: true
    bias_term: true
  }
}
layer {
  name: "relu_d4"
  type: "ReLU"
  bottom: "dis_conv_d4"
  top: "dis_conv_d4"
  relu_param {
    negative_slope: 0.2
  }
}
layer {
  name: "score"
  type: "InnerProduct"
  bottom: "dis_conv_d4"
  top: "score"
  param {
    name: "score_w"
    lr_mult: 1
  }
  param {
    name: "score_b"
    lr_mult: 2
  }
  inner_product_param{
    num_output: 1
    dis_mode: true
    weight_filler {
      type: "gaussian"
      std: 0.0002
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
layer {
  name: "sigmoid"
  type: "Sigmoid"
  bottom: "score"
  top: "score"
}
layer {
  name: "gan_loss"
  type: "GANDGLoss"
  bottom: "score"
  top: "gan_loss"
  gan_loss_param {
    dis_iter: 1
    gen_iter: 2
  }
}123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131132133134135136137138139140141142143144145146147148149150151152153154155156157158159160161162163164165166167168169170171172173174175176177178179180181182183184185186187188189190191192193194195196197198199200201202203204205206207208209210211212213214215216217218219220221222223224225226227228229230231232233234235236237238239240241242243244245246247248249250251252253254255256257258259260261262263264265266267268269270271272273274275276277278279280281282283284285286287288289290291292293294295296297298299300301302303304305306307308309310311312313314315316317318319320321322323324325326327328329330331332333334335336337338339340341342343344345346347348349350351352353354355356357358359360361362363364365366367368369370371372373374375376377378379380381382383384385386387388389390391392393394395396397398399400401402403404405406407408409410411412413414415416417418419420421422423424425426427428429430431432433434435436437438439440441442443444445446447448449450451452453454455456457458459460461462463464465466467468469470471472473474475476477478479480481482483484485486487488489490491492493494495496497498499500501502503504505506507508509510511512513514515516517518519520521522523524525526527528529530531532533534535536537538539540541542543544545546547548549550551552553554555556557558559560561562563564565566567568569570571572573574575576577578579580581582583584585586587588589590591592593594595596597598599600601602603604605606607608609610611612613614615616617618619620621622623624625626627628629630631632633634635636637638639640641642643644645646647648649650651652653654655656657658659660661662663664665666667668669670671672673674675676677678679680681682683684685686687688689690691692693694695696697698699700701702703704705706707708709710711712713714715716717718719720721722723724725726727728729730731732733734735736737738739740741742743744745746747748749750751752753754755756757758759760761762763764765
```

2.solver.prototxt

```
#  Create on: 2016/10/22 ShanghaiTech
#  Author:    Yingying Zhang

net: "gan_configs/train.prototxt"
debug_info: false
display: 10
solver_type: ADAM
average_loss: 100
base_lr: 2e-4
lr_policy: "fixed"
max_iter: 15000
momentum: 0.5
snapshot: 100
gan_solver: true
snapshot_prefix: "models/gan_"
12345678910111213141516
```

#### 3.生成结果

![这里写图片描述](https://img-blog.csdn.net/20161101212746054)

未完待续！！！[第二部分为caffe具体源码！](http://blog.csdn.net/seven_first/article/details/53100325)





