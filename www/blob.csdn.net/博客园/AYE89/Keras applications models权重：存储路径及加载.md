
# Keras.applications.models权重：存储路径及加载 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[Keras.applications.models权重：存储路径及加载](https://www.cnblogs.com/eniac1946/p/7808439.html)
|Posted on|2017-11-09 11:14|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7808439)|[收藏](#)
|网络中断原因导致keras加载vgg16等模型权重失败，
|直接解决方法是：删掉下载文件，再重新下载
|Windows-weights路径|：
|C:\Users\你的用户名\.keras\models
|Linux-weights路径|：
|.keras/models/
|注意： linux中 带点号的文件都被隐藏了，需要查看hidden文件才能显示
|Keras-Github-教程
|[https://github.com/fchollet/deep-learning-models](https://github.com/fchollet/deep-learning-models)
|原始权重下载地址
|[https://github.com/fchollet/deep-learning-models/releases](https://github.com/fchollet/deep-learning-models/releases)
|tf模型权重百度云下载地址
|[http://pan.baidu.com/s/1dE9giOD](http://pan.baidu.com/s/1dE9giOD)
|VGG16:
|WEIGHTS_PATH =
|‘|[https://github.com/fchollet/deep-learning-models/releases/download/v0.1/vgg16_weights_tf_dim_ordering_tf_kernels.h5](https://github.com/fchollet/deep-learning-models/releases/download/v0.1/vgg16_weights_tf_dim_ordering_tf_kernels.h5)|’
|WEIGHTS_PATH_NO_TOP =
|‘|[https://github.com/fchollet/deep-learning-models/releases/download/v0.1/vgg16_weights_tf_dim_ordering_tf_kernels_notop.h5](https://github.com/fchollet/deep-learning-models/releases/download/v0.1/vgg16_weights_tf_dim_ordering_tf_kernels_notop.h5)|’
|VGG19:
|TF_WEIGHTS_PATH =
|‘|[https://github.com/fchollet/deep-learning-models/releases/download/v0.4/xception_weights_tf_dim_ordering_tf_kernels.h5](https://github.com/fchollet/deep-learning-models/releases/download/v0.4/xception_weights_tf_dim_ordering_tf_kernels.h5)|’
|TF_WEIGHTS_PATH_NO_TOP =
|‘|[https://github.com/fchollet/deep-learning-models/releases/download/v0.4/xception_weights_tf_dim_ordering_tf_kernels_notop.h5](https://github.com/fchollet/deep-learning-models/releases/download/v0.4/xception_weights_tf_dim_ordering_tf_kernels_notop.h5)|’
|RESNET50:
|WEIGHTS_PATH =
|‘|[https://github.com/fchollet/deep-learning-models/releases/download/v0.2/resnet50_weights_tf_dim_ordering_tf_kernels.h5](https://github.com/fchollet/deep-learning-models/releases/download/v0.2/resnet50_weights_tf_dim_ordering_tf_kernels.h5)|’
|WEIGHTS_PATH_NO_TOP =
|‘|[https://github.com/fchollet/deep-learning-models/releases/download/v0.2/resnet50_weights_tf_dim_ordering_tf_kernels_notop.h5](https://github.com/fchollet/deep-learning-models/releases/download/v0.2/resnet50_weights_tf_dim_ordering_tf_kernels_notop.h5)|’
|INCEPTIONS_V3:
|WEIGHTS_PATH =
|‘|[https://github.com/fchollet/deep-learning-models/releases/download/v0.5/inception_v3_weights_tf_dim_ordering_tf_kernels.h5](https://github.com/fchollet/deep-learning-models/releases/download/v0.5/inception_v3_weights_tf_dim_ordering_tf_kernels.h5)|’
|WEIGHTS_PATH_NO_TOP =
|‘|[https://github.com/fchollet/deep-learning-models/releases/download/v0.5/inception_v3_weights_tf_dim_ordering_tf_kernels_notop.h5](https://github.com/fchollet/deep-learning-models/releases/download/v0.5/inception_v3_weights_tf_dim_ordering_tf_kernels_notop.h5)|’
|XCEPTION:
|TF_WEIGHTS_PATH =
|‘|[https://github.com/fchollet/deep-learning-models/releases/download/v0.4/xception_weights_tf_dim_ordering_tf_kernels.h5](https://github.com/fchollet/deep-learning-models/releases/download/v0.4/xception_weights_tf_dim_ordering_tf_kernels.h5)|’
|TF_WEIGHTS_PATH_NO_TOP =
|‘|[https://github.com/fchollet/deep-learning-models/releases/download/v0.4/xception_weights_tf_dim_ordering_tf_kernels_notop.h5](https://github.com/fchollet/deep-learning-models/releases/download/v0.4/xception_weights_tf_dim_ordering_tf_kernels_notop.h5)|’







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
