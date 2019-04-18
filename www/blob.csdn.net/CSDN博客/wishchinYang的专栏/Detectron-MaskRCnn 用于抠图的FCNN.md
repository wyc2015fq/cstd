# Detectron-MaskRCnn: 用于抠图的FCNN - wishchinYang的专栏 - CSDN博客
2018年03月04日 23:50:38[wishchin](https://me.csdn.net/wishchin)阅读数：1773
       市面上暂时还没有找到可以在消费机显卡上实时运行的MaskRCnn，TensorFlow即使是C++版本训练在coco数据集上的模型也是慢的要死，最后不堪忍受，只能放弃。
       经历了一些列fuckingDog的复杂配置之后，终于配置成功了。测试一把
       Detectron地址：[https://github.com/facebookresearch/Detectron](https://github.com/facebookresearch/Detectron)
**一、预 安装环境**
        #查看python    ubuntu16.04 默认安装了2.7 和3.5
        whereis python
        #安装pip
        sudo *apt-get* install python-*pip*
        #根据Caffe2的介绍，安装其他库
        使用cmake make之时，选择blas为openblas
*        sudo apt-get install libopenblas-dev*
         省去重新安装eigen3.3Up 
*...................................................*
*       #安装caffe2成功后安装detection，根据detection Github上的介绍*
*...................................................*
***二、InferImage***
把Detectron导入Eclipse ，把infer_sample.py复制到根目录;
在debug选项里面添加参数：
--cfg configs/12_2017_baselines/e2e_mask_rcnn_R-101-FPN_2x.yaml 
--output-dir out/  --image-ext  jpg --wts /home/wishchin/build/detectron/model/model_final.pkl demo
结果输出到out文件夹
![](https://img-blog.csdn.net/20180304233939881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20180304233958942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对于自己的数据，检测结果还不错！
![](https://img-blog.csdn.net/20180307211010772)
时间消耗在1秒左右
INFO infer_simple.py: 145: Inference time: 0.906s
INFO infer_simple.py: 147:  | im_detect_bbox: 0.765s
INFO infer_simple.py: 147:  | misc_mask: 0.091s
INFO infer_simple.py: 147:  | im_detect_mask: 0.041s
INFO infer_simple.py: 147:  | misc_bbox: 0.008s
![](https://img-blog.csdn.net/20180304234819048?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
NFO infer_simple.py: 111: Processing image/nordic/livingroom (40).jpg -> image/nordicOut/livingroom (40).jpg.pdf
INFO infer_simple.py: 119: Inference time: 0.846s
INFO infer_simple.py: 121:  | im_detect_bbox: 0.783s
INFO infer_simple.py: 121:  | misc_mask: 0.017s
INFO infer_simple.py: 121:  | im_detect_mask: 0.035s
INFO infer_simple.py: 121:  | misc_bbox: 0.010s
INFO infer_simple.py: 124:  \ Note: inference on the first image will be slower than the rest (caches and auto-tuning need to warm up)
INFO infer_simple.py: 111: Processing image/nordic/livingroom (32).jpg -> image/nordicOut/livingroom (32).jpg.pdf
INFO infer_simple.py: 119: Inference time: 0.366s
INFO infer_simple.py: 121:  | im_detect_bbox: 0.320s
INFO infer_simple.py: 121:  | misc_mask: 0.015s
INFO infer_simple.py: 121:  | im_detect_mask: 0.020s
INFO infer_simple.py: 121:  | misc_bbox: 0.011s
INFO infer_simple.py: 111: Processing image/nordic/livingroom (57).jpg -> image/nordicOut/livingroom (57).jpg.pdf
INFO infer_simple.py: 119: Inference time: 0.514s
INFO infer_simple.py: 121:  | im_detect_bbox: 0.445s
INFO infer_simple.py: 121:  | misc_mask: 0.017s
INFO infer_simple.py: 121:  | im_detect_mask: 0.042s
INFO infer_simple.py: 121:  | misc_bbox: 0.011s
INFO infer_simple.py: 111: Processing image/nordic/livingroom (41).jpg -> image/nordicOut/livingroom (41).jpg.pdf
INFO infer_simple.py: 119: Inference time: 0.336s
INFO infer_simple.py: 121:  | im_detect_bbox: 0.297s
INFO infer_simple.py: 121:  | misc_mask: 0.006s
INFO infer_simple.py: 121:  | im_detect_mask: 0.021s
INFO infer_simple.py: 121:  | misc_bbox: 0.012s
INFO infer_simple.py: 111: Processing image/nordic/livingroom (65).jpg -> image/nordicOut/livingroom (65).jpg.pdf
INFO infer_simple.py: 119: Inference time: 0.582s
INFO infer_simple.py: 121:  | im_detect_bbox: 0.483s
INFO infer_simple.py: 121:  | misc_mask: 0.025s
INFO infer_simple.py: 121:  | im_detect_mask: 0.063s
INFO infer_simple.py: 121:  | misc_bbox: 0.011s
INFO infer_simple.py: 111: Processing image/nordic/livingroom (42).jpg -> image/nordicOut/livingroom (42).jpg.pdf
INFO infer_simple.py: 119: Inference time: 0.397s
INFO infer_simple.py: 121:  | im_detect_bbox: 0.348s
INFO infer_simple.py: 121:  | misc_mask: 0.014s
INFO infer_simple.py: 121:  | im_detect_mask: 0.024s
INFO infer_simple.py: 121:  | misc_bbox: 0.011s
