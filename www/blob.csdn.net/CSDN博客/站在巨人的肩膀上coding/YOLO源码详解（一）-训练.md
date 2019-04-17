# YOLO源码详解（一）-训练 - 站在巨人的肩膀上coding - CSDN博客





2018年07月11日 23:10:23[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：298








本系列作者：木凌 
时间：2016年11月。 
文章连接：[http://blog.csdn.net/u014540717](http://blog.csdn.net/u014540717)
QQ交流群：554590241

本系列文章会持续更新，主要会分以下几个部分： 
１、darknet下的yolo源代码解读 
２、将yolo移植到mxnet下 
３、模型压缩与加速 
白天需要工作，只有晚上时间写，所以可能更新速度有点慢，还有就是该系列博文不一定会严格按照以上三点的顺序来写，也可能移植到caffe下，在caffe下进行压缩和加速。

## 一、训练

我用的是VOC2007的数据集，下载指令如下：

```
$curl -O http://pjreddie.com/media/files/VOCtrainval_06-Nov-2007.tar
$curl -O http://pjreddie.com/media/files/VOCtest_06-Nov-2007.tar
$tar xf VOCtrainval_06-Nov-2007.tar
$tar xf VOCtest_06-Nov-2007.tar
```
- 1
- 2
- 3
- 4

运行以下代码，将`.xml` 文件转换成`.txt` 文件，以备YOLO训练时数据解析：

```python
import xml.etree.ElementTree as ET
import pickle
import os
from os import listdir, getcwd
from os.path import join

sets=[('2007', 'train'), ('2007', 'val')]

classes = ["aeroplane", "bicycle", "bird", "boat", "bottle", "bus", "car", "cat", "chair", "cow", "diningtable", "dog", "horse", "motorbike", "person", "pottedplant", "sheep", "sofa", "train", "tvmonitor"]


def convert(size, box):
    dw = 1./size[0]
    dh = 1./size[1]
    x = (box[0] + box[1])/2.0
    y = (box[2] + box[3])/2.0
    w = box[1] - box[0]
    h = box[3] - box[2]
    x = x*dw
    w = w*dw
    y = y*dh
    h = h*dh
    return (x,y,w,h)

def convert_annotation(year, image_id):
    in_file = open('VOCdevkit/VOC%s/Annotations/%s.xml'%(year, image_id))
    out_file = open('VOCdevkit/VOC%s/labels/%s.txt'%(year, image_id), 'w')
    tree=ET.parse(in_file)
    root = tree.getroot()
    size = root.find('size')
    w = int(size.find('width').text)
    h = int(size.find('height').text)

    for obj in root.iter('object'):
        difficult = obj.find('difficult').text
        cls = obj.find('name').text
        if cls not in classes or int(difficult) == 1:
            continue
        cls_id = classes.index(cls)
        xmlbox = obj.find('bndbox')
        b = (float(xmlbox.find('xmin').text), float(xmlbox.find('xmax').text), float(xmlbox.find('ymin').text), float(xmlbox.find('ymax').text))
        bb = convert((w,h), b)
        out_file.write(str(cls_id) + " " + " ".join([str(a) for a in bb]) + '\n')

wd = getcwd()

for year, image_set in sets:
    if not os.path.exists('VOCdevkit/VOC%s/labels/'%(year)):
        os.makedirs('VOCdevkit/VOC%s/labels/'%(year))
    image_ids = open('VOCdevkit/VOC%s/ImageSets/Main/%s.txt'%(year, image_set)).read().strip().split()
    list_file = open('%s_%s.txt'%(year, image_set), 'w')
    for image_id in image_ids:
        list_file.write('%s/VOCdevkit/VOC%s/JPEGImages/%s.jpg\n'%(wd, year, image_id))
        convert_annotation(year, image_id)
    list_file.close()
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55

在`yolo.c`中找到以下代码，并修改：

```
//指向你刚才生成的train.txt文件
char *train_images = "/data/voc/train.txt";
//新建个文件夹，然后指向这里就可以，训练生成的模型文件会保存在这里
char *backup_directory = "/home/pjreddie/backup/";
```
- 1
- 2
- 3
- 4

运行以下指令开始训练：
`./darknet yolo train cfg/yolo.train.cfg extraction.conv.weights`- 1

## 二、源码解读

1.首先我们看一下训练的数据流，从main函数开始看，该函数在`darknet.c` 文件中：

```cpp
//darknet.c
int main(int argc, char **argv)
{
    //test_resize("data/bad.jpg");
    //test_box();
    //test_convolutional_layer();
    if(argc < 2){
        fprintf(stderr, "usage: %s <function>\n", argv[0]);
        return 0;
    }
    gpu_index = find_int_arg(argc, argv, "-i", 0);
    if(find_arg(argc, argv, "-nogpu")) {
        gpu_index = -1;
    }

#ifndef GPU
    gpu_index = -1;
#else
    if(gpu_index >= 0){
        cuda_set_device(gpu_index);
    }
#endif

    if (0 == strcmp(argv[1], "average")){
        average(argc, argv);
    } else if (0 == strcmp(argv[1], "yolo")){
        //第一个参数是yolo，所以跳转到run_yolo函数
        run_yolo(argc, argv);
    } else {
        fprintf(stderr, "Not an option: %s\n", argv[1]);
    }
    return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33

```cpp
//yolo.c
void run_yolo(int argc, char **argv)
{
    char *prefix = find_char_arg(argc, argv, "-prefix", 0);
    float thresh = find_float_arg(argc, argv, "-thresh", .2);
    int cam_index = find_int_arg(argc, argv, "-c", 0);
    int frame_skip = find_int_arg(argc, argv, "-s", 0);
    if(argc < 4){
        fprintf(stderr, "usage: %s %s [train/test/valid] [cfg] [weights (optional)]\n", argv[0], argv[1]);
        return;
    }

    char *cfg = argv[3];
    char *weights = (argc > 4) ? argv[4] : 0;
    char *filename = (argc > 5) ? argv[5]: 0;
    if(0==strcmp(argv[2], "test")) test_yolo(cfg, weights, filename, thresh);
    //第二个参数是train，所以跳转到了train_yolo函数
    else if(0==strcmp(argv[2], "train")) train_yolo(cfg, weights);
    voc_names, 20, frame_skip, prefix);
}


void train_yolo(char *cfgfile, char *weightfile)
{
    char *train_images = "/data/voc/train.txt";
    char *backup_directory = "/home/pjreddie/backup/";
    /*srand函数是随机数发生器的初始化函数。
    srand和rand()配合使用产生伪随机数序列。rand函数在产生随机数前，需要系统提供的生成伪随机数序列的
    种子，rand根据这个种子的值产生一系列随机数。如果系统提供的种子没有变化，每次调用rand函数生成的伪
    随机数序列都是一样的。*/
    srand(time(0));
    /*第三个参数是：`cfg/yolo.train.cfg`，`basecfg()`这个函数把`cfg/yolo.train.cfg`变成了
    `yolo0train.cfg`，然后用base指针指向`yolo0train.cfg`*/
    char *base = basecfg(cfgfile);
    //打印"yolo"字样
    printf("%s\n", base);
    float avg_loss = -1;
    //解析网络构架，下面会仔细分析该函数
    network net = parse_network_cfg(cfgfile);
    //加载预训练参数，下面会仔细分析该函数
    if(weightfile){
        load_weights(&net, weightfile);
    }
    printf("Learning Rate: %g, Momentum: %g, Decay: %g\n", net.learning_rate, net.momentum, net.decay);
    /*imgs是一次加载到内存的图像数量，如果占内存太大的话可以把subdivisions或者batch调小一点
    详细可以参考知乎：[深度机器学习中的batch的大小对学习效果有何影响？](https://www.zhihu.com/question/32673260)*/
    int imgs = net.batch*net.subdivisions;
    //net.seen就是已经经过网络训练（看）的图片数量，算出的i就是已经经过了多少次训练
    int i = *net.seen/imgs;
    data train, buffer;


    layer l = net.layers[net.n - 1];
    //side就是论文中的7
    int side = l.side;
    int classes = l.classes;
    //jitter是什么意思呢？可以参考这篇博客：[非均衡数据集处理：利用抖动(jittering)生成额外数据](http://weibo.com/1402400261/EgMr4vCC2?type=comment#_rnd1478833653326)
    float jitter = l.jitter;

    list *plist = get_paths(train_images);
    //int N = plist->size;
    char **paths = (char **)list_to_array(plist);

    load_args args = {0};
    args.w = net.w;
    args.h = net.h;
    args.paths = paths;
    //n就是一次加载到内存中的图片数量
    args.n = imgs;
    //m是待训练图片的总数量
    args.m = plist->size;
    args.classes = classes;
    args.jitter = jitter;
    //7*7个网格
    args.num_boxes = side;
    args.d = &buffer;
    args.type = REGION_DATA;

    //调节图片旋转角度、曝光度、饱和度、色调等，来增加图片数量
    args.angle = net.angle;
    args.exposure = net.exposure;
    args.saturation = net.saturation;
    args.hue = net.hue;
    //声明线程ID
    pthread_t load_thread = load_data_in_thread(args);
    clock_t time;
    //while(i*imgs < N*120){
    while(get_current_batch(net) < net.max_batches){
        i += 1;
        time=clock();
        /*pthread_join()函数，以阻塞的方式等待thread指定的线程结束。当函数返回时，被等待线程的资源被
        收回。如果线程已经结束，那么该函数会立即返回。*/
        pthread_join(load_thread, 0);
        train = buffer;
        load_thread = load_data_in_thread(args);

        printf("Loaded: %lf seconds\n", sec(clock()-time));

        time=clock();
        //开始训练
        float loss = train_network(net, train);
        if (avg_loss < 0) avg_loss = loss;
        avg_loss = avg_loss*.9 + loss*.1;

        printf("%d: %f, %f avg, %f rate, %lf seconds, %d images\n", i, loss, avg_loss, get_current_rate(net), sec(clock()-time), i*imgs);
        //每100次或者1000次保存一次权重
        if(i%1000==0 || (i < 1000 && i%100 == 0)){
            char buff[256];
            sprintf(buff, "%s/%s_%d.weights", backup_directory, base, i);
            save_weights(net, buff);
        }
        free_data(train);
    }
    char buff[256];
    sprintf(buff, "%s/%s_final.weights", backup_directory, base);
    save_weights(net, buff);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117

看到这里，我想你已经能可以自己来训练VOC数据集了。但这只是刚刚开始，我们要想将YOLO移植到mxnet下，就需要了解其具体函数的实现过程，请看下一节内容。



