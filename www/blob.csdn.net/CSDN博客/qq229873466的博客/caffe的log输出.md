# caffe的log输出 - qq229873466的博客 - CSDN博客

2017年03月01日 10:55:17[qq229873466](https://me.csdn.net/qq229873466)阅读数：783


在::google::InitGoogleLogging(argv[0]);之前设置

FLAGS_alsologtostderr = true;
FLAGS_colorlogtostderr = true;
FLAGS_log_dir = "DIR";

等属性。

