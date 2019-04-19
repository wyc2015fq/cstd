# win10上安装Caffe，配置matlab、python - sinat_31425585的博客 - CSDN博客
2017年03月03日 13:17:12[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：235
 花了三天，终于在win10上将caffe配置成功，现在mark一下，做个笔记。
在win10上安装caffe，可以参考[谢小小XH的博客](http://blog.csdn.net/xierhacker/article/details/51834563)，[阿尔法旺旺](http://blog.csdn.net/yingwei13mei/article/details/53844087)。
在测试mnist数据集的时候出现了如下错误：
![](https://img-blog.csdn.net/20170303131045163?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzE0MjU1ODU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
错误的原因是，在.\examples\mnist路径下的lenet_solver.prototxt以及lenet_train_test.prototxt中设置路径没有注意/ 和 \的区别，将路径中的\换成即可：
net: "examples/mnist/lenet_train_test.prototxt"



