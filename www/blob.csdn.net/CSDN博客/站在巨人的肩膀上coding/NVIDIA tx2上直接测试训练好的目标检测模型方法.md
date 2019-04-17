# NVIDIA tx2上直接测试训练好的目标检测模型方法 - 站在巨人的肩膀上coding - CSDN博客





2018年01月12日 19:14:10[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：1442








在tx2上无法快速训练模型,所以,一般只是直接测试训练好的模型(原论文提供的训练得到的最终模型).方法如下:

cd ~/py-faster-rcnn

1.测试ZF网络



```
time ./tools/test_net.py \
--gpu 0 \
--def models/pascal_voc/ZF/faster_rcnn_end2end/test.prototxt \
--net data/faster_rcnn_models/ZF_faster_rcnn_final.caffemode \
--imdb voc_2007_test \
--cfg experiments/cfgs/faster_rcnn_end2end.yml
```

(注意上面的路径需要换成你自己的,这里是我的模型及网络文件的存储位置，把上述文件放到一个.sh脚本文件中)

２.测试VGG16



```
time ./tools/test_net.py \
--gpu 0 \
--def models/pascal_voc/VGG16/faster_rcnn_end2end/test.prototxt \
--net data/faster_rcnn_models/VGG16_faster_rcnn_final.caffemode \
--imdb voc_2007_test \
--cfg experiments/cfgs/faster_rcnn_end2end.yml
```






