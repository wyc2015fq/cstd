
# 自有数据集上,如何用keras最简单训练YOLOv3目标检测 - 素质云笔记-Recorder... - CSDN博客

2019年01月02日 14:20:03[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：1085


[qqwweee/keras-yolo3](https://github.com/qqwweee/keras-yolo3)是最简单的自数据训练yolov3的开源项目了。非常简单，相比其他的开源项目，太适合新手练习yolov3。
而公开的很多开源框架的都是基于VOC/COCO来写预训练，整理数据起麻烦不少。
本来笔者看到mxnet/gluoncv有yolov3的自训练，而且Mxnet还进行一定改进把精度提升了不少，还欢欣鼓舞的去尝试，但是一旦遇到坑，基本没法解决。。社区人太少，搜不到前人的经验，报错信息稀奇古怪，定位到报错code较难，留言给社区也一时半会儿没人回，还真是从入门到放弃。。
在此之上进行一些微调，我的项目地址：[keras-yolo3-improved](https://github.com/mattzheng/keras-yolo3-improved)
其中，
selfdata_keras_yolov3.ipynb，自己训练时候的ipynb
selfdata_yolov3_test.ipynb，自己预测时候的ipynb
yolo_matt.py,预测时候改进输出结果

---文章目录

---[1 数据准备](#1__18)

---[2 训练：](#2__39)

---[3 预测：](#3__94)


---1 数据准备

---最简单是因为把数据整理成以下的样子就可以开始训练：

---path/to/img1.jpg 50,100,150,200,0 30,50,200,120,3
path/to/img2.jpg 120,300,250,600,2
...

---也就是：地址，xmin,ymin,xmax,ymax，类别ID然后空格下一个box，每张图一行。

---例子：

---images/images_all/86900fb6gy1fl4822o7qmj22ao328qv7.jpg 10,259,399,580,27
images/images_all/b95fe9cbgw1eyw88vlifjj20c70hsq46.jpg 10,353,439,640,29
images/images_all/005CsCZ0jw1f1n8kcj8m1j30ku0kumz6.jpg 75,141,343,321,27

---2 训练：

---keras源码中有两段训练：

---第一段冻结前面的249层进行迁移学习（原有的yolov3）

---第二段解冻全部层进行训练

---笔者自己的训练数据集是专业领域的图像，所以基本第一阶段的迁移学习阶段没啥用，因为与原有的yolov3训练集差异太大，如果你也是，请直接开始第二段或者重新根据

---[darknet53](https://pjreddie.com/media/files/darknet53.conv.74)

---训练。

---那么这边就有三样可能需要预下载的模型：

---yolo_weights.h5 预训练模型（用作迁移）

---python convert.py -w yolov3.cfg yolov3.weights model_data/yolo_weights.h5

---darknet53.weights （用作重新训练）

---wget https://pjreddie.com/media/files/darknet53.conv.74

---yolo.h5 （yolov3-VOC训练模型，可以直接用来做预测 ）

---python convert.py yolov3.cfg yolov3.weights model_data/yolo.h5

---来看看训练时候需要的参数：

---class yolo_args:
        annotation_path = 'train.txt'
        log_dir = 'logs/003/'
        classes_path = 'model_data/class_file_en.txt'
        anchors_path = 'model_data/yolo_anchors.txt'
        input_shape = (416,416) \# multiple of 32, hw
        \# 608*608  416*416  320*320
        val_split = 0.1
        batch_size = 16
        epochs_stage_1 = 10
        stage_1_train = False
        epochs_finally = 100
        finally_train = True
        weights_path =   'logs/003/ep009-loss33.297-val_loss32.851.h5'\# 可以使用'model_data/tiny_yolo_weights.h5' 也可以使用tiny_yolo的：'model_data/yolo_weights.h5'
        
        
    \# train
    _main(yolo_args)

---annotation_path就是数据集准备的txt


---log_dir ，Model存放地址，譬如：

---events.out.tfevents.1545966202

---、

---ep077-loss19.318-val_loss19.682.h5


---classes_path ，分类内容


---anchors_path ，yolo anchors，可自行调整，也可以使用默认的


---input_shape ，一般是416


---epochs_stage_1 = 10

---和

---stage_1_train = False

---，是同一个，也就是是否进行迁移学习（

---stage_1_train

---），要学习的话，学习几个epoch（

---epochs_stage_1

---）


---epochs_finally = 100

---和

---finally_train = True

---，是，是否进行后面开放所有层的学习（

---finally_train

---），学习几个epoch（

---epochs_finally

---）


---weights_path ，调用model的路径


---这里需要注意：

---如果要在之前训练基础上，追加训练，一般要把batch_size设置小一些，然后加载之前的权重。

---3 预测：

---来看一个简单的预测

---import sys
import argparse
from yolo import YOLO, detect_video
from PIL import Image
yolo_test_args = {
    "model_path": 'model_data/yolo.h5',
    "anchors_path": 'model_data/yolo_anchors.txt',
    "classes_path": 'model_data/coco_classes.txt',
    "score" : 0.3,
    "iou" : 0.45,
    "model_image_size" : (416, 416),
    "gpu_num" : 1,
}

yolo_test = YOLO(**yolo_test_args)
image = Image.open('images/part1/path1.jpg')
r_image = yolo_test.detect_image(image)
r_image.show()

---直接返回的是带框的图片，如果你要输出boxes，可以自己改一下

---detect_image

---函数。

---此时注意以下：

---out_boxes, out_scores, out_classes

---中out_boxes，每个Boxes代表的是：

---y_min, x_min, y_max, x_max

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190102141537605.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
---在此之上，进行预测结果优化，可参考：

---yolo_matt.py

---:

---import sys
import argparse
from yolo_matt import YOLO, detect_video
from PIL import Image
yolo_test_args = {
    "model_path": 'logs/003/ep077-loss19.318-val_loss19.682.h5',
    "anchors_path": 'model_data/yolo_anchors.txt',
    "classes_path": 'model_data/class_file_en.txt',
    "score" : 0.2,\# 0.2
    "iou" : 0.1,\# 0.45
    "model_image_size" : (416, 416),
    "gpu_num" : 1,
}
yolo_test = YOLO(**yolo_test_args)
\# 输出内容整理
def _get_class(classes_path):
    classes_path = os.path.expanduser(classes_path)
    with open(classes_path) as f:
        class_names = f.readlines()
    class_names = [c.strip() for c in class_names]
    return class_names
def yolov3_output(image,out_boxes,out_scores,out_classes):
    output = []
    yolo_classes = _get_class(yolo_test_args['classes_path'])
    for n,box in enumerate(out_boxes):
        y_min, x_min, y_max, x_max = box
        y_min = max(0, np.floor(y_min + 0.5).astype('int32'))
        x_min = max(0, np.floor(x_min + 0.5).astype('int32'))
        y_max = min(image.size[1], np.floor(y_max + 0.5).astype('int32'))
        x_max = min(image.size[0], np.floor(x_max + 0.5).astype('int32'))
        score = out_scores[n]
        yo_class = yolo_classes[out_classes[n]]
        output.append({ 'y_min':y_min, 'x_min':x_min, 'y_max':y_max, 'x_max':x_max,\
                       'width':image.size[0],'height':image.size[1],\
                       'score':score,'yo_class':yo_class})
    return output
    
image = Image.open('images/images_all/path1.jpg')
r_image,out_boxes, out_scores, out_classes = yolo_test.detect_image(image)
output = yolov3_output(r_image,out_boxes,out_scores,out_classes)

---输出结果类似：

---{
'path1.jpg': 
[{'y_min': 416,   'x_min': 34,   'y_max': 754,   'x_max': 367,   'width': 440,   'height': 783,   'score': 0.9224778,   'yo_class': 'class1'},
  {'y_min': 428,   'x_min': 3,   'y_max': 783,   'x_max': 352,   'width': 440,   'height': 783,   'score': 0.2180994,   'yo_class': 'class2'}]
  }


