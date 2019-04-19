# OpenCV DNN之YOLO实时对象检测-流浪的鱼-51CTO博客
## OpenCV DNN之YOLO实时对象检测
OpenCV在3.3.1的版本中开始正式支持Darknet网络框架并且支持YOLO1与YOLO2以及YOLO Tiny网络模型的导入与使用。YOLO是一种比SSD还要快的对象检测网络模型，算法作者在其论文中说FPS是Fast R-CNN的100倍，基于COCO数据集跟SSD网络的各项指标对比
![OpenCV DNN之YOLO实时对象检测](https://s1.51cto.com/images/blog/201804/07/0ff883ea77d0918ff0749e2e1c8d288c.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
在最新的OpenCV3.4上我也测试了YOLO3，发现不支持，因为YOLO3有个新层类型shortcut，OpenCV3.4的Darknet暂时还不支持。这里首先简单的介绍一下YOLO网络基本结构，然后在通过代码演示Darknet支持的YOLO在OpenCV使用。
### 一：YOLO网络
对象检测网络基本上可以分为两种，一种称为两步法、另外一种称为一步法，很显然基于图像分类加上滑动窗口的方式最早的R-CNN就是两步法的代表之一，两步法的前面基本上是一个卷积神经网络，可以是VGGNet或者Inception之类的，然后再加上一个滑动窗口，但是这种方法太慢，所以就有了区域推荐(RP)，预先推荐一些感兴趣的区域，进行预言，这些方法普遍有一个缺点，计算量比较大，导致性能低下无法实时，而YOLO采样了一种完全不同的方法，达到对图像每个区域只计算一次(You Look at Once - YOLO)，YOLO把图像分为13x13的Cell(网格)：
![OpenCV DNN之YOLO实时对象检测](https://s1.51cto.com/images/blog/201804/07/442c254a9a0eda8672d5f0bb2b350c1d.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
每个Cell预测5个BOX，同时YOLO也会生成一个置信分数，告诉每个BOX包含某个对象的可能性是多少，注意置信分数不会直接说明BOX内是检测到何种对象，最终那些得分高的BOX被加粗显示如下：
![OpenCV DNN之YOLO实时对象检测](https://s1.51cto.com/images/blog/201804/07/00b5e558962bec0a5be0d1e3b9565a68.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
对于每个BOX来说，Cell会预测检测对象类别，这部分的工作就像是一个分类器一样，基于VOC数据集20中对象检测，YOLO结合分数与分类信息对每个BOX给出一个最终可能对象类型的可能性值，如下图，×××区域85%可能性是狗：
![OpenCV DNN之YOLO实时对象检测](https://s1.51cto.com/images/blog/201804/07/c0089d64821efbc9392ad7969d3c7fe9.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
因为总数是13x13的网格，每个网格预言5个BOX，所以最终有854个BOX，证据表明绝大多数的BOX得分会很低，我们只要保留30%BOX即可(取决于你自己的阈值设置)，最终输出：
![OpenCV DNN之YOLO实时对象检测](https://s1.51cto.com/images/blog/201804/07/0d553b556c4a5254d369b049ff776f5e.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
从上面可以看出整个图像只是被计算了一次，真正做到了降低计算量，提高了检测实时性。上述检测使用的YOLO的网络结构如下：
![OpenCV DNN之YOLO实时对象检测](https://s1.51cto.com/images/blog/201804/07/19b46c647b402a00a153cde73516904f.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
发现只有CNN层，没有FC层，是不是简单到爆，最后说一下为什么最后一层卷积层深度是125，
因为每个Cell检测5个BOX，对每个BOX来说，包含如下数据
- BOX本身信息，x、y、w、h
- 置信分数
- 基于VOC数据集的20个对象类别
所以对每个BOX来说有25个参数，5个BOX= 5x25=125个参数。
上面是得到的网络模型就是tiny-YOLO网络模型，可以在移动端实时对象检测。这个跟作者在论文中提到的稍微有点差异，论文中作者是输入图像为448x448，分为7x7的网格(Cell)，结构如下：
![OpenCV DNN之YOLO实时对象检测](https://s1.51cto.com/images/blog/201804/07/a68412da4d38850df344253b705160c5.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
最终输出是每个Cell预测两个BOX，做20个分类，它得到最终是
- BOX本身信息，x、y、w、h
- 置信分数
深度 = S*S*(B*5+20)， 其中20个表示分类数目，S表示网络分割，B表示BOX个数。S=7、B=2，最终输出是7*7*30
## 二：在OpenCV中使用YOLO
OpenCV在3.3.1版本中开始支持Darknet，可能有人会问，Darknet是什么鬼，它是YOLO的作者自己搞出来的深度学习框架，支持C/C++/Python语言，支持YOLOv1、YOLOv2、YOLOv3等网络模型训练与使用。但是在OpenCV只是前馈网络，只支持预测，不能训练。OpenCV中基于YOLO模型我使用的是tiny-YOLO网络模型，支持20中对象检测。代码实现步骤如下：
**1. 加载网络模型**
```
String modelConfiguration = "D:/vcprojects/images/dnn/yolov2-tiny-voc/yolov2-tiny-voc.cfg";
String modelBinary = "D:/vcprojects/images/dnn/yolov2-tiny-voc/yolov2-tiny-voc.weights";
dnn::Net net = readNetFromDarknet(modelConfiguration, modelBinary);
if (net.empty())
{
    printf("Could not load net...\n");
    return;
}
```
**2. 加载分类信息**
```
vector<string> classNamesVec;
ifstream classNamesFile("D:/vcprojects/images/dnn/yolov2-tiny-voc/voc.names");
if (classNamesFile.is_open())
{
    string className = "";
    while (std::getline(classNamesFile, className))
        classNamesVec.push_back(className);
}
```
**3. 加载测试图像**
```
// 加载图像
Mat frame = imread("D:/vcprojects/images/fastrcnn.jpg");
Mat inputBlob = blobFromImage(frame, 1 / 255.F, Size(416, 416), Scalar(), true, false);
net.setInput(inputBlob, "data");
```
**4. 检测与显示**
```
// 检测
Mat detectionMat = net.forward("detection_out");
vector<double> layersTimings;
double freq = getTickFrequency() / 1000;
double time = net.getPerfProfile(layersTimings) / freq;
ostringstream ss;
ss << "detection time: " << time << " ms";
putText(frame, ss.str(), Point(20, 20), 0, 0.5, Scalar(0, 0, 255));
// 输出结果
for (int i = 0; i < detectionMat.rows; i++)
{
    const int probability_index = 5;
    const int probability_size = detectionMat.cols - probability_index;
    float *prob_array_ptr = &detectionMat.at<float>(i, probability_index);
    size_t objectClass = max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr;
    float confidence = detectionMat.at<float>(i, (int)objectClass + probability_index);
    if (confidence > confidenceThreshold)
    {
        float x = detectionMat.at<float>(i, 0);
        float y = detectionMat.at<float>(i, 1);
        float width = detectionMat.at<float>(i, 2);
        float height = detectionMat.at<float>(i, 3);
        int xLeftBottom = static_cast<int>((x - width / 2) * frame.cols);
        int yLeftBottom = static_cast<int>((y - height / 2) * frame.rows);
        int xRightTop = static_cast<int>((x + width / 2) * frame.cols);
        int yRightTop = static_cast<int>((y + height / 2) * frame.rows);
        Rect object(xLeftBottom, yLeftBottom,
            xRightTop - xLeftBottom,
            yRightTop - yLeftBottom);
        rectangle(frame, object, Scalar(0, 0, 255), 2, 8);
        if (objectClass < classNamesVec.size())
        {
            ss.str("");
            ss << confidence;
            String conf(ss.str());
            String label = String(classNamesVec[objectClass]) + ": " + conf;
            int baseLine = 0;
            Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
            rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom),
                Size(labelSize.width, labelSize.height + baseLine)),
                Scalar(255, 255, 255), CV_FILLED);
            putText(frame, label, Point(xLeftBottom, yLeftBottom + labelSize.height),
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
        }
    }
}
imshow("YOLO-Detections", frame);
```
**5. 运行效果**
![OpenCV DNN之YOLO实时对象检测](https://s1.51cto.com/images/blog/201804/07/cda6584b7e0cc23f98037ab852b145e7.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
**我的课程：**
[学习OpenCV3.3深度神经网络(DNN)模块-应用视频教程](http://edu.51cto.com/course/11516.html)
