# YOLOv3目标检测的TensorFlow实现，可用自己的数据来训练 - 知乎
# 



> 方栗子 发自 凹非寺 
量子位 报道 | 公众号 QbitAI
![](https://pic3.zhimg.com/v2-33ac1d981fa5c294d8dba5b272b628e6_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)△ 来自YOLOv3原作者
**YOLOv3**，快如闪电，可称目标检测之光。

PyTorch实现教程去年4月就出现了，TensorFlow实现一直零零星星。

现在，有位热心公益的程序猿 (Yunyang1994) ，为它做了纯TensorFlow代码实现。

这份实现，支持用自己的数据训练模型。

## **介绍一下**

TensorFlow实现，包含了以下部分：

> · YOLOv3架构
· 权重转换器 (Weight Converter)
· 基础版Demo
· GPU和CPU上都支持非极大抑制 (Non-Maximum Suppression)
· 训练pipeline
· COCO mAP计算
![](https://pic4.zhimg.com/v2-829c6bc8a99f36ceb286ae7e7510ac3b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)△ 来自YOLOv3原作者
## **快速开始**

四个步骤，速速上车。

**1.**复制这个文件：

`1$ git clone https://github.com/YunYang1994/tensorflow-yolov3.git`

**2.**在食用代码前，先安装一些依赖项：


```
1$ cd tensorflow-yolov3
2$ pip install -r ./docs/requirements.txt
```


**3.**把加载好的COCO权重导出为TF checkpoint (yolov3.ckpt) 和 frozen graph (yolov3_gpu_nms.pb) 。


如果你没有**yolov3.weights**的话，去下载，然后放到./checkpoint目录下。下载地址是：

[https://github.com/YunYang1994/tensorflow-yolov3/releases/download/v1.0/yolov3.weights](https://link.zhihu.com/?target=https%3A//github.com/YunYang1994/tensorflow-yolov3/releases/download/v1.0/yolov3.weights)

`1$ python convert_weight.py --convert --freeze`

**4.**然后，./checkpoint目录下就会出现一些.pb文件。现在可以跑Demo脚本了：


```
1$ python nms_demo.py
2$ python video_demo.py # if use camera, set video_path = 0
```




![](https://pic2.zhimg.com/v2-cc6ef373f484487a10269638d528f4d5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='281'></svg>)△ 来自YOLOv3原作者
## **拿自己的数据集训练**

## **快速训练**

这个Demo就是给大家一个粗略的感受，感受YOLOv3的训练过程到底是怎样的。

用python core/convert_tfrecord.py把你的图集转成TFRecords。


```
1$ python core/convert_tfrecord.py --dataset /data/train_data/quick_train_data/quick_train_data.txt  --tfrecord_path_prefix /data/train_data/quick_train_data/tfrecords/quick_train_data
2$ python quick_train.py  # start training
```


## **训练COCO数据集**

如果还没下载COCO2017数据集，请前往：

[http://cocodataset.org/](https://link.zhihu.com/?target=http%3A//cocodataset.org/)

再把数据集放到./data/train_data/COCO里面。


```
1$ cd data/train_data/COCO
2$ wget http://images.cocodataset.org/zips/train2017.zip
3$ unzip train2017.zip
4$ wget http://images.cocodataset.org/annotations/annotations_trainval2017.zip
5$ unzip annotations_trainval2017.zip
```


然后，就要从数据集里提取一些有用的信息了，比如边界框，拿这些信息生成你自己的.txt文件。

`1$ python core/extract_coco.py --dataset_info_path ./data/train_data/COCO/train2017.txt`



![](https://pic4.zhimg.com/v2-1fbc1c6ed0e17b87e9ee66be4402eb2f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)△ 来自YOLOv3原作者
上面这步得到的文件是./data/train_data/COCO/train2017.txt。拿一张图举栗，应该长这样：


```
1As a result, you will get ./data/train_data/COCO/train2017.txt. Here is an example row for one image:
2
3/home/yang/test/tensorflow-yolov3/data/train_data/train2017/000000458533.jpg 20 18.19 6.32 424.13 421.83 20 323.86 2.65 640.0 421.94
4/home/yang/test/tensorflow-yolov3/data/train_data/train2017/000000514915.jpg 16 55.38 132.63 519.84 380.4
5# image_path, category_id, x_min, y_min, x_max, y_max, category_id, x_min, y_min, ...
```


接下来，要把图像数据集转成.tfrecord，就是用二进制来保存数据。最后，可以训练啦。


```
1$ python core/convert_tfrecord.py --dataset ./data/train_data/COCO/train2017.txt  --tfrecord_path_prefix ./data/train_data/COCO/tfrecords/coco --num_tfrecords 100
2$ python train.py
```


## **COCO评估**

如果要看一下模型在COCO上的表现，就这样做：


```
1$ cd data/train_data/COCO
2$ wget http://images.cocodataset.org/zips/test2017.zip
3$ wget http://images.cocodataset.org/annotations/image_info_test2017.zip 
4$ unzip test2017.zip
5$ unzip image_info_test2017.zip
```




![](https://pic4.zhimg.com/v2-2dcab8a46416726a1447404df475b4db_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='281'></svg>)△ 来自YOLOv3原作者
## **“我今年没干啥”**

YOLO系列的作者，是华盛顿大学两位画风奇崛的研究人员，一个叫Joseph Redmon，一个叫Ali Farhadi。

去年3月YOLOv3发布，两位在论文里是这样描述这项成果的：

> 我今年基本没做啥研究，净刷推特了，也玩了一小会儿GAN。去年还剩一点动力没用完，就给YOLO更了个新。没什么特别有意思的东西，一些细小的改动而已。
![](https://pic2.zhimg.com/v2-c9bcff4a710b00b9fd27dbd6e3a51165_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='982' height='478'></svg>)△ Introduction满分
但严肃地说，**速度**是v3最主要的提升。一般来讲，YOLOv3比R-CNN快1000倍，比Fast R-CNN快100倍。

有一些人啊，看起来常年不做正事，但还是能拿出精彩的成果。

还有一些人呢...... (误)

YOLOv3 TensorFlow实现传送门：
[https://github.com/YunYang1994/tensorflow-yolov3](https://link.zhihu.com/?target=https%3A//github.com/YunYang1994/tensorflow-yolov3)

(早就出了的) YOLO v3 PyTorch教程传送门：
[https://github.com/ayooshkathuria/YOLO_v3_tutorial_from_scratch](https://link.zhihu.com/?target=https%3A//github.com/ayooshkathuria/YOLO_v3_tutorial_from_scratch)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态
[量子位​www.zhihu.com![图标](https://pic4.zhimg.com/v2-ca6e7ffc10a0d10edbae635cee82d007_ipico.jpg)](https://www.zhihu.com/org/liang-zi-wei-48)
欢迎大家关注我们，以及订阅[我们的知乎专栏](https://zhuanlan.zhihu.com/qbitai)

**诚挚招聘**

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。


