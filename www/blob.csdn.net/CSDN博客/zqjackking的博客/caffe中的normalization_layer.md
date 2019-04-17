# caffe中的normalization_layer - zqjackking的博客 - CSDN博客





2017年04月09日 23:02:28[zqjackking](https://me.csdn.net/zqjackking)阅读数：6866








caffe-ssd里面有normalization的实现，包括.hpp，.cpp，.cu。其实现的是L2-normalization 

L2正则化的公式是： 
![这里写图片描述](https://img-blog.csdn.net/20170409213729503?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

现在来看caffe的代码实现。 

首先是caffe.proto，这里面定义了normalization_parameter 

message NormalizeParameter { 

  optional bool across_spatial = 1 [default = true]; 

  // Initial value of scale. Default is 1.0 for all 

  optional FillerParameter scale_filler = 2; 

  // Whether or not scale parameters are shared across channels. 

  optional bool channel_shared = 3 [default = true]; 

  // Epsilon for not dividing by zero while normalizing variance 

  optional float eps = 4 [default = 1e-10]; 

} 

这里面有两个很重要的参数，across_spatial和channel_shared。 

accross_spatial决定了normalization的范围，如果为true的话（默认），则对每个num(channel*height*width)整体进行normalization，也就是上面xi的平方加和的个数是channel*height*width；如果是false的话，就表明normalization不是accross_spatial的，上面加和的个数是channel，也就是说，spatial中的每个像素点（height*width个数）分别进行normalization，这就大大减小了normalization的范围. 

至于channel_shared。在上面的归一化完了之后，要将top_data乘以一个scale（这个scale是normalization_layer的唯一的参数），如果channel_shared为true（默认），那么top_data的所有channel都乘以同一个数，如果channel_shared为false，那么top_data的channel乘的数是不一样的。 

下面看forward_cpu。
```bash
for (int n = 0; n < num; ++n) {
    caffe_sqr<Dtype>(dim, bottom_data, buffer_data);
    if (across_spatial_) {
      // add eps to avoid overflow
      norm_data[n] = pow(caffe_cpu_asum<Dtype>(dim, buffer_data)+eps_,
                         Dtype(0.5));
      caffe_cpu_scale<Dtype>(dim, Dtype(1.0 / norm_data[n]), bottom_data,
                             top_data);
    } else {
      caffe_cpu_gemv<Dtype>(CblasTrans, channels, spatial_dim, Dtype(1),
                            buffer_data, sum_channel_multiplier, Dtype(1),
                            norm_data);
      // compute norm
      caffe_powx<Dtype>(spatial_dim, norm_data, Dtype(0.5), norm_data);
      // scale the layer
      caffe_cpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, channels, spatial_dim,
                            1, Dtype(1), sum_channel_multiplier, norm_data,
                            Dtype(0), buffer_data);
      caffe_div<Dtype>(dim, bottom_data, buffer_data, top_data);
      norm_data += spatial_dim;
    }
    // scale the output
    if (channel_shared_) {   //defalut为true
      caffe_scal<Dtype>(dim, scale[0], top_data);  //都乘以一个scale[0]
    } else {
      caffe_cpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, channels, spatial_dim,
                            1, Dtype(1), scale, sum_spatial_multiplier,
                            Dtype(0),
                            buffer_data);
      caffe_mul<Dtype>(dim, top_data, buffer_data, top_data);
    }
    bottom_data += dim;
    top_data += dim;
  }
```

注意，这里的循环以及最后的bottom_data += dim; top_data += dim;这表明任何情况下normalization_layer都是对每一个num进行归一化的，而不是对所有的num进行归一化。 

首先是  caffe_sqr(dim, bottom_data, buffer_data);   这是将bottom_data的第一个num进行element_wise的平方，并保存在buffer_data中。 

然后进入第一个if（也就是across_spatial为true，此时norm_data形状为（num,1,1,1））
```bash
if (across_spatial_) {
// add eps to avoid overflow
norm_data[n] = pow(caffe_cpu_asum<Dtype>(dim, buffer_data)+eps_,
                   Dtype(0.5));
caffe_cpu_scale<Dtype>(dim, Dtype(1.0 / norm_data[n]), bottom_data,
                       top_data);
 }
```

第一句算出了buffer-data所有元素的和（加上eps_防止overflow），然后开根号 

第二句是将bottom_data的所有元素乘上norm_data[n]的导数。至此across_spatial为true下的normalization就完成了。 

下面是第一个else，也就是across_spatial为false的情况 

else { 

      caffe_cpu_gemv(CblasTrans, channels, spatial_dim, Dtype(1), 

                            buffer_data, sum_channel_multiplier, Dtype(1), 

                            norm_data); 

      // compute norm 

      caffe_powx(spatial_dim, norm_data, Dtype(0.5), norm_data); 

      // scale the layer 

      caffe_cpu_gemm(CblasNoTrans, CblasNoTrans, channels, spatial_dim, 

                            1, Dtype(1), sum_channel_multiplier, norm_data, 

                            Dtype(0), buffer_data); 

      caffe_div(dim, bottom_data, buffer_data, top_data); 

      norm_data += spatial_dim; 

    } 

这段代码要完成的事情我们上面已经介绍了，很简单，只不过实现上比across_spatial为true要复杂些。我们看到上面总共有4个操作，现分别介绍。（注意，此时norm_data为（num,1,height,width））
第一个操作： 

norm_data=1*buffer_data*sum_channel_multiplier+1*norm_data 

(后面那个1改成0也行，因为norm_data没有进行任何初始化，所以其原始元素为0） 

其中，buffer_data形状为channel*spatial_dim(计算时要转置，因为第一个参数是CblasTrans） 

         sum_channel_multiplier形状为 channel*1(其元素都是1） 

         norm_data(参与运算的）的形状为spatial_dim*1 

这里的norm_data相当于在channel维度上对元素进行了求平方和，然后结果存在norm_data中（存了spatial_dim也就是height*width个）, 

第二个操作： 

caffe_powx(spatial_dim, norm_data, Dtype(0.5), norm_data); 

对上面得到的norm_data开根号 

第三个操作： 

buffer_data=sum_channel_multiplier*norm_data 

其中，buffer_data为channels*spatial_dim 

          sum_channel_multiplier为channel*1（所有元素为1） 

          norm_data位1*spatial_dim 

这里是将norm_data扩展成channel个，得到了buffer_data，便于后面的element_wise的相除 

第4个操作： 

caffe_div(dim, bottom_data, buffer_data, top_data); 

top_data=bottom_data/buffer_data   （element_wise） 

最后，别忘了norm_data会加上一个spatial_dim。这样好就行下一个num的计算。 

以上就完成了across_spatial为false的normalization。 

接下来是第二个if(跟上面的if是平行而不是嵌入的关系，即是在上面的normalization完成后进行的）
```
if (channel_shared_) {   //defalut为true
      caffe_scal<Dtype>(dim, scale[0], top_data);  //都乘以一个scale[0]
    }
```

如果channel_shared为ture，则将整个top_data乘以一个scale[0]

```bash
else {
 caffe_cpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, channels, spatial_dim,
                       1, Dtype(1), scale, sum_spatial_multiplier,
                       Dtype(0),
                       buffer_data);
 caffe_mul<Dtype>(dim, top_data, buffer_data, top_data);
}
```

完成的操作是： 

buffer_data= scale*sum_spatial_multiplier 

其中  buffer_data： channel*spatial_dim 

        scale : channel 

        sum_spatial_multiplier: 1*spatial_dim （元素都是1） 

 其实就是将scale复制spatial_dim遍，做成一张mask，以便后面element_wise的相乘 

 caffe_mul(dim, top_data, buffer_data, top_data); 

 最后是bottom_data += dim;top_data += dim;整个上面提到过的，进入下一个num的归一化。







