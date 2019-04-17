# Caffe LOG(INFO) 打开 - 家家的专栏 - CSDN博客





2015年02月12日 12:12:20[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：5623








问题：

      LOG(ERROR) 可以正常打印信息。

      LOG(INFO) 不起作用。

 解决办法：

添加此句： FLAGS_alsologtostderr =1 




