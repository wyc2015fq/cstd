# Fine-tuning on our database with Caffe - 家家的专栏 - CSDN博客





2014年09月22日 11:12:11[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3090
个人分类：[菜鸟从零开始学习Deep learning](https://blog.csdn.net/yihaizhiyan/article/category/2388401)









Fin-tuning....

●**Rename**the layers you need tochange.



●**Call**CopyTrainedLayerFrom(pretrained_file)

●**Train**as normal



layer{ 

   layer {

~~name: “~~~~fc8”  ~~

  name: “fc8-t”

~~  type: “innerproduct”  ~~

  type: “innerproduct”

~~  num_output: 1000~~

  num_output: 397

  …  …} 

      }




摘自：https://docs.google.com/presentation/d/1lzyXMRQFlOYE2Jy0lCNaqltpcCIKuRzKJxQ7vCuPRc8/edit#slide=id.g3c48e0e3ae4e944415






