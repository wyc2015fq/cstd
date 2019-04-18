# 使用Caffe预测遇到的问题 - wishchinYang的专栏 - CSDN博客
2015年04月07日 14:06:55[wishchin](https://me.csdn.net/wishchin)阅读数：3800
1. 在使用网络预测图像时，
prediction = net.predict( [input_image] )
出现： net.image_dims[0] 不是整数情况，
(2).甚至以为np.zeros()出现错误！最后发现
原因：net.image_dims 
Out[25]: '/home/wishchin/caffe-master/python/caffe/imagenet/ilsvrc_2012_mean.npy'
(3).出错原因：mean_file= caffe_root+"python/caffe/imagenet/ilsvrc_2012_mean.npy"
        引用了错误的文件
因此在初始化Net的时候不引用MeanFIle就可以：
net = caffe.Classifier(Model_File,PreTrained,mean_file,channel_swap=(2,1,0 )  , input_scale=255);
修改为：
net = caffe.Classifier(Model_File,PreTrained);
可以运行！
2.Caffe出现了IndexError: tuple index out of range问题；
prediction = net.predict( input_image  )#prediction = net.predict( ImageFile )
File "/home/wishchin/caffe-master/python/caffe/classifier.py", line 69, in predict
    inputs[0].shape[2] ),
IndexError: tuple index out of range
好怪异的事情： caffe函数自身有bug啊！再一次载入运行竟然出现了输入输出不能 对应的问题，坑死！
input_image=caffe.io.load_image(ImageFile );
prediction = net.predict( input_image  )
第二次运行出现错误............
  (2).原因找出来了：prediction = net.predict( input_image  )
       忘记 了填入列表，应该修改为：
      prediction = net.predict( [input_image ]  )
3.
