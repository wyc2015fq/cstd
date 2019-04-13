
# YOLO 与 增强现实 - intflojx的博客 - CSDN博客


2018年08月06日 13:42:52[paulpanwang](https://me.csdn.net/intflojx)阅读数：223


## 分类器
darknet-53(52层卷积层+1FC )
![这里写图片描述](https://img-blog.csdn.net/20180416195617359?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180416202034148?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
实现每秒最高的测量浮点运算。这意味着网络结构可以更好地利用GPU，从而使其评估效率更高，速度更快。
Darknet-53比ResNet-101更好，速度更快1：5倍。 Darknet-53与ResNet-152具有相似的性能，速度提高2倍。
## 多尺度预测
**坐标预测**： bbox 预测仍是yolov2的使用维度聚类（dimension clusters ）作为anchor boxes来预测边界框. 在训练期间，我们使用平方误差损失的总和。
**对象分数**： YOLOv3使用逻辑回归预测每个边界框（bounding box）的对象分数。 如果先前的边界框比之前的任何其他边界框重叠ground truth对象，则该值应该为1。如果以前的边界框不是最好的，但是确实将ground truth对象重叠了一定的阈值以上，我们会忽略这个预测，按照[15]进行。我们使用阈值0.5。与[15]不同，我们的系统只为每个ground truth对象分配一个边界框。如果先前的边界框未分配给grounding box对象，则不会对坐标或类别预测造成损失。
**类别预测**： 每个框使用多标签分类来预测边界框可能包含的类。在训练过程中，使用二元交叉熵损失来进行类别预测。
![这里写图片描述](https://img-blog.csdn.net/20180416215129969?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**多尺度：**YOLOv3预测3种不同尺度的框（boxes），每个尺度的3个框，所以，
张量为`N×N×[3 *（4 + 1 + 80）]`。
网络会在预测三种尺度的特征N分别为13，26，52，分别对应各三种anchor
`（116×90）; （156×198）; （373×326）`
`（30×61）;（62×45）; （59×119）;`
`（10×13）;（16×30）;（33×23）;`(大尺度用小anchor，提高小目标识别能力)
预测tx ty tw th
对tx和ty进行sigmoid，并加上对应的offset（下图Cx, Cy）
对th和tw进行exp，并乘以对应的锚点值
对tx,ty,th,tw乘以对应的步幅，即：416/13, 416 ⁄ 26, 416 ⁄ 52
最后，使用sigmoid对Objectness和Classes confidence进行sigmoid得到0~1的概率，之所以用sigmoid取代之前版本的softmax，原因是softmax会扩大最大类别概率值而抑制其他类别概率值
![这里写图片描述](https://img-blog.csdn.net/20180516201601712?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`layer     filters    size              input                output
    0 conv     32  3 x 3 / 1   416 x 416 x   3   ->   416 x 416 x  32
    1 conv     64  3 x 3 / 2   416 x 416 x  32   ->   208 x 208 x  64
    2 conv     32  1 x 1 / 1   208 x 208 x  64   ->   208 x 208 x  32
    3 conv     64  3 x 3 / 1   208 x 208 x  32   ->   208 x 208 x  64
    4 Shortcut Layer: 1
    5 conv    128  3 x 3 / 2   208 x 208 x  64   ->   104 x 104 x 128
    6 conv     64  1 x 1 / 1   104 x 104 x 128   ->   104 x 104 x  64
    7 conv    128  3 x 3 / 1   104 x 104 x  64   ->   104 x 104 x 128
    8 Shortcut Layer: 5
    9 conv     64  1 x 1 / 1   104 x 104 x 128   ->   104 x 104 x  64
   10 conv    128  3 x 3 / 1   104 x 104 x  64   ->   104 x 104 x 128
   11 Shortcut Layer: 8
   12 conv    256  3 x 3 / 2   104 x 104 x 128   ->    52 x  52 x 256
   13 conv    128  1 x 1 / 1    52 x  52 x 256   ->    52 x  52 x 128
   14 conv    256  3 x 3 / 1    52 x  52 x 128   ->    52 x  52 x 256
   15 Shortcut Layer: 12
   16 conv    128  1 x 1 / 1    52 x  52 x 256   ->    52 x  52 x 128
   17 conv    256  3 x 3 / 1    52 x  52 x 128   ->    52 x  52 x 256
   18 Shortcut Layer: 15
   19 conv    128  1 x 1 / 1    52 x  52 x 256   ->    52 x  52 x 128
   20 conv    256  3 x 3 / 1    52 x  52 x 128   ->    52 x  52 x 256
   21 Shortcut Layer: 18
   22 conv    128  1 x 1 / 1    52 x  52 x 256   ->    52 x  52 x 128
   23 conv    256  3 x 3 / 1    52 x  52 x 128   ->    52 x  52 x 256
   24 Shortcut Layer: 21
   25 conv    128  1 x 1 / 1    52 x  52 x 256   ->    52 x  52 x 128
   26 conv    256  3 x 3 / 1    52 x  52 x 128   ->    52 x  52 x 256
   27 Shortcut Layer: 24
   28 conv    128  1 x 1 / 1    52 x  52 x 256   ->    52 x  52 x 128
   29 conv    256  3 x 3 / 1    52 x  52 x 128   ->    52 x  52 x 256
   30 Shortcut Layer: 27
   31 conv    128  1 x 1 / 1    52 x  52 x 256   ->    52 x  52 x 128
   32 conv    256  3 x 3 / 1    52 x  52 x 128   ->    52 x  52 x 256
   33 Shortcut Layer: 30
   34 conv    128  1 x 1 / 1    52 x  52 x 256   ->    52 x  52 x 128
   35 conv    256  3 x 3 / 1    52 x  52 x 128   ->    52 x  52 x 256
   36 Shortcut Layer: 33
   37 conv    512  3 x 3 / 2    52 x  52 x 256   ->    26 x  26 x 512
   38 conv    256  1 x 1 / 1    26 x  26 x 512   ->    26 x  26 x 256
   39 conv    512  3 x 3 / 1    26 x  26 x 256   ->    26 x  26 x 512
   40 Shortcut Layer: 37
   41 conv    256  1 x 1 / 1    26 x  26 x 512   ->    26 x  26 x 256
   42 conv    512  3 x 3 / 1    26 x  26 x 256   ->    26 x  26 x 512
   43 Shortcut Layer: 40
   44 conv    256  1 x 1 / 1    26 x  26 x 512   ->    26 x  26 x 256
   45 conv    512  3 x 3 / 1    26 x  26 x 256   ->    26 x  26 x 512
   46 Shortcut Layer: 43
   47 conv    256  1 x 1 / 1    26 x  26 x 512   ->    26 x  26 x 256
   48 conv    512  3 x 3 / 1    26 x  26 x 256   ->    26 x  26 x 512
   49 Shortcut Layer: 46
   50 conv    256  1 x 1 / 1    26 x  26 x 512   ->    26 x  26 x 256
   51 conv    512  3 x 3 / 1    26 x  26 x 256   ->    26 x  26 x 512
   52 Shortcut Layer: 49
   53 conv    256  1 x 1 / 1    26 x  26 x 512   ->    26 x  26 x 256
   54 conv    512  3 x 3 / 1    26 x  26 x 256   ->    26 x  26 x 512
   55 Shortcut Layer: 52
   56 conv    256  1 x 1 / 1    26 x  26 x 512   ->    26 x  26 x 256
   57 conv    512  3 x 3 / 1    26 x  26 x 256   ->    26 x  26 x 512
   58 Shortcut Layer: 55
   59 conv    256  1 x 1 / 1    26 x  26 x 512   ->    26 x  26 x 256
   60 conv    512  3 x 3 / 1    26 x  26 x 256   ->    26 x  26 x 512
   61 Shortcut Layer: 58
   62 conv   1024  3 x 3 / 2    26 x  26 x 512   ->    13 x  13 x1024
   63 conv    512  1 x 1 / 1    13 x  13 x1024   ->    13 x  13 x 512
   64 conv   1024  3 x 3 / 1    13 x  13 x 512   ->    13 x  13 x1024
   65 Shortcut Layer: 62
   66 conv    512  1 x 1 / 1    13 x  13 x1024   ->    13 x  13 x 512
   67 conv   1024  3 x 3 / 1    13 x  13 x 512   ->    13 x  13 x1024
   68 Shortcut Layer: 65
   69 conv    512  1 x 1 / 1    13 x  13 x1024   ->    13 x  13 x 512
   70 conv   1024  3 x 3 / 1    13 x  13 x 512   ->    13 x  13 x1024
   71 Shortcut Layer: 68
   72 conv    512  1 x 1 / 1    13 x  13 x1024   ->    13 x  13 x 512
   73 conv   1024  3 x 3 / 1    13 x  13 x 512   ->    13 x  13 x1024
   74 Shortcut Layer: 71
   75 conv    512  1 x 1 / 1    13 x  13 x1024   ->    13 x  13 x 512
   76 conv   1024  3 x 3 / 1    13 x  13 x 512   ->    13 x  13 x1024
   77 conv    512  1 x 1 / 1    13 x  13 x1024   ->    13 x  13 x 512
   78 conv   1024  3 x 3 / 1    13 x  13 x 512   ->    13 x  13 x1024
   79 conv    512  1 x 1 / 1    13 x  13 x1024   ->    13 x  13 x 512
   80 conv   1024  3 x 3 / 1    13 x  13 x 512   ->    13 x  13 x1024
   81 conv     18  1 x 1 / 1    13 x  13 x1024   ->    13 x  13 x  18
   82 detection
   83 route  79
   84 conv    256  1 x 1 / 1    13 x  13 x 512   ->    13 x  13 x 256
   85 upsample            2x    13 x  13 x 256   ->    26 x  26 x 256
   86 route  85 61
   87 conv    256  1 x 1 / 1    26 x  26 x 768   ->    26 x  26 x 256
   88 conv    512  3 x 3 / 1    26 x  26 x 256   ->    26 x  26 x 512
   89 conv    256  1 x 1 / 1    26 x  26 x 512   ->    26 x  26 x 256
   90 conv    512  3 x 3 / 1    26 x  26 x 256   ->    26 x  26 x 512
   91 conv    256  1 x 1 / 1    26 x  26 x 512   ->    26 x  26 x 256
   92 conv    512  3 x 3 / 1    26 x  26 x 256   ->    26 x  26 x 512
   93 conv     18  1 x 1 / 1    26 x  26 x 512   ->    26 x  26 x  18
   94 detection
   95 route  91
   96 conv    128  1 x 1 / 1    26 x  26 x 256   ->    26 x  26 x 128
   97 upsample            2x    26 x  26 x 128   ->    52 x  52 x 128
   98 route  97 36
   99 conv    128  1 x 1 / 1    52 x  52 x 384   ->    52 x  52 x 128
  100 conv    256  3 x 3 / 1    52 x  52 x 128   ->    52 x  52 x 256
  101 conv    128  1 x 1 / 1    52 x  52 x 256   ->    52 x  52 x 128
  102 conv    256  3 x 3 / 1    52 x  52 x 128   ->    52 x  52 x 256
  103 conv    128  1 x 1 / 1    52 x  52 x 256   ->    52 x  52 x 128
  104 conv    256  3 x 3 / 1    52 x  52 x 128   ->    52 x  52 x 256
  105 conv     18  1 x 1 / 1    52 x  52 x 256   ->    52 x  52 x  18
  106 detection`
region层:参数anchors指定kmeans计算出来的anchor box的长宽的绝对值(与网络输入大小相关),num参数为anchor box的数量,
另外还有bias_match,classes,coords等参数.在parser.c代码中的parse_region函数中解析这些参数,并保存在region_layer.num参数保存在l.n变量中;anchors保存在l.biases数组中.region_layer的前向传播中使用for(n = 0; n < l.n; ++n)这样的语句,因此,如果在配置文件中anchors的数量大于num时,仅使用前num个,小于时内存越界.
region层的输入和输出大小与前一层(1x1 conv)的输出大小和网络的输入大小相关.
Detection层: 坐标及类别结果输出层.
使用k-means聚类来确定我们的边界框的先验。我们只是选择了9个聚类（clusters）和3个尺度（scales），然后在整个尺度上均匀分割聚类。在COCO数据集上，9个聚类是：（10×13）;（16×30）;（33×23）;（30×61）;（62×45）; （59×119）; （116×90）; （156×198）; （373×326）。
## 训练
仍然训练完整的图像，没有hard negative mining。我们使用多尺度训练，大量的data augmentation，batch normalization，以及所有标准的东西。
### Implement
![](https://img-blog.csdn.net/20180806133806680?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
目标检测结果重现
![](https://img-blog.csdn.net/2018080613405536?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180806134149200?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180806134212887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


