
# 多目标跟踪 | AI产品经理需要了解的CV通识（三） - 喜欢打酱油的老鸟 - CSDN博客


2018年07月29日 08:31:29[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：173


[https://blog.csdn.net/pA2elX78qaJTADH/article/details/79042736](https://blog.csdn.net/pA2elX78qaJTADH/article/details/79042736)
![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ6zt9dsMgd4jPuIEVnoDRBViaSJCibHA0Jvlzibv9Hu2qCBibTRBakI9S70g/0?wx_fmt=jpeg)
**多目标跟踪（MOT, Multiple Object Tracking）**
多目标跟踪是CV领域一个热门方向，广泛应用于机器人导航、智能监控视频、工业检测、航空航天等领域。主要任务是**找到图像序列中运动的物体，并将不同帧的运动物体一一对应，最后给出不同物体的运动轨迹**。
1
**术语了解**
**1、检测：定位目标在图像中的位置。**检测方法很多，例如帧间差分法、背景减除法、光流法等等；另外，检测常与识别结合。
**2、跟踪：**在连续图像序列中完成对目标的检测，并把物理意义下同一目标相关联。
**3、轨迹（Trajectory）：**一条轨迹对于这一目标在一段时间内的位置序列；是多目标跟踪系统的输出量。
**4、数据关联：**用于解决目标间的匹配问题；是多目标跟踪的核心问题。
**5、跟踪置信度：**跟踪算法反应每一次跟踪的可靠程度。
2
**技术流程详解**
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ6m7uYwP4Fsa3xicLcrZOzOjwsquMMmzKFUnfXGZvcBsPD6XibmJLBFic5g/0?wx_fmt=png)
**1、图像采集**
多目标跟踪对于图像采集要求评估的因素与人脸识别差别不大。详见上一篇文章《人脸识别 | AI产品经理需要了解的CV通识（二）》。
**2、图像预处理**
典型的图像预处理方法是**直方图均衡**和**滤波**。
**1）直方图均衡**
直方图描述了一副图像的灰度级内容，而直方图均衡化的主要目的，是为了**提高对比度和灰度色调的变化，使图像更加清晰**。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ6jwNan5KBFVGpmeyUPLuf4vf0k1wHBKl2Uk9aLKbH2U29FN864hPmwQ/0?wx_fmt=png)
**2）滤波**
图像滤波的主要目的是，在保持图像特征的情况下进行噪声消除，具体可以分为**线性滤波**和**非线性滤波**。非线性滤波相比线性滤波来说，更能**保护图像细节**。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ6aZEy1QL5XYROz3yx5YpaYbqmrTxjJv9Dbm8cO3ARNOpnsPZoiajaUJg/0?wx_fmt=png)
**3、基于深度学习的多目标检测识别**
目标检测的实质是多目标的定位，即要在图片中定位**多个目标物体**。
例如下图，既要定位各个目标，还需要将不同目标用不同颜色的框表示。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ6RzcUq5Tjqic2Jw7Jg2BTRlZUsl28EKaa9YyRcj5jKia4gtXPoANTZABw/0?wx_fmt=png)
*（By 深思考人工智能）*
**1）基于深度学习的目标检测算法：YOLO、YOLOv2、YOLO9000**
YOLO（You Only Look Once）是基于深度学习方法的端到端实时目标检测系统。YOLO的升级版有两种：YOLOv2和YOLO9000。**YOLO9000可以识别超过9000类别。**
YOLO对图片的处理步骤主要分为三个部分：
a.将图片缩放到448*448大小
b.运行卷积网络
c.设置模型置信度阈值，输出结果
![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ62NsqZSZ95dHD2JvTPwv1RV2jicIsGDReR3ElMUhg2UEiaDc81kI4Mibsw/0?wx_fmt=jpeg)
*（By 代码学习者coding）*
**2）基于深度学习的目标检测算法：SSD**
SSD（Single Shot MultiBOX Detector），截止目前是**主要的检测框架之一**，相比Fast-RNN速度更快；相比YOLO有明显的mAP优势（mAP：平均正确率），但不及YOLO9000。
SSD缺点：
a.调试过程非常依赖经验
b.检测小目标的召回率值不是很高
**3）YOLO和SSD效果比较**
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ6D8djNExA42u9B2yaPSDBNjazCvAVyZ0KiaQjMmRAtyp2FbLKibaYaH1Q/0?wx_fmt=png)
*（By 代码学习者coding）*
从图上可以看出，YOLO漏检率较高，SSD相对来说表现较好。
**4）目标检测识别相关指标**
**a.精确率：**计算方式详见《人脸识别 | AI产品经理需要了解的CV通识（二）》
**b.召回率：**同上。
**c.识别精度：**目标检测中衡量检测出精度的指标是**平均正确率均值**mAP(mean average precision）
首先我们了解一下AP（平均正确率）
**平均正确率**：precision/recall曲线下方围成的曲面面积，如下图：
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ61tlHs9K0MHXA2KGfUj2Gak9sBcA1nE2XgvzsRHjwdpLEWtLHIxybng/0?wx_fmt=png)
其中横坐标是recall，纵坐标是precision。AP是这个曲线下的面积，**而mAP就是多个类别AP的平均值，这个值介于0到1之间，且越大越好**。
**d.检测响应：**检测过程的输出量。
**e.识别效率：**识别的速度。相对应选择效率极大化的迭代算法。
**f.交并比（IoU）：**可以理解为系统预测出来的框与原来图中标记框的重合程度，最理想的情况是完全重叠，即比值为1。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ6NnywKciapAfuXNdkwiaXGC2LaCv6kzZS5icAr870qGMAmDVOXsXN7Ol4g/0?wx_fmt=png)
计算公式：
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ6f2licGbNmq9Etia4MAibGrG4L2DQD3H0Gh1gEISmxiaiaibU41PXXbGVNfXQ/0?wx_fmt=png)
**4、多目标跟踪**
下图中的曲线是该目标的跟踪路径，图中线条即代表各个目标的**轨迹**。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ61mia8olTdQwSODaliaIes0x7PqcypHMrAqaGdf3MgC6wAu5HNpYUeJow/0?wx_fmt=png)
目前跟踪的困难点：
a.遮挡、目标消失后再出现，会有丢失目标的现象；
b.相似目标容易被当成同一目标；
由于以上原因，在实际场景里多目标跟踪可能会发生人员重复检测、漏检、同一目标多条轨迹等情况。如果需求对人员统计非常严格，可以考虑由其他方案完成。多目标跟踪应用于一个区域（一个摄像头画面内），计算**该区域****内**的目标数以及目标跟踪这样的场景会更好。
3
**多目标跟踪的应用场景**
目标识别常用来确定某画面或视频中包含什么物体、各个物体在什么位置、各个物体的轨迹。因此常用于监控，人机交互和虚拟现实的场景。
**1）人员检测：**计算画面中行人的数目，并确定其位置。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ6aTodWeAS6k2jNLjTvyjNZMhcf4y0cfnwMMy95Dqa7USklf6f2dZmiaw/0?wx_fmt=png)
应用场景：
a.可用于计算区域**人员密度过高告警**。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ6qBchF2282mwTib01exWM9Fmic9kCUBM4lnt0L2hnPWDXkvHNQXLOkHGw/0?wx_fmt=png)
产品功能考虑：在监控画面对区域进行划分，并对人员密度（梳理）进行设置。阈值的设置可以考虑和时间结合，在某一段时间内超过某阈值即可告警。
利用本算法进行人员计算的缺点是如果发生目标丢失，会将同一人的两个状态识别为两个人，使得真实数目与检测数目误差大。
b.可用于**范围监测告警（越界监测）**：例如闯红灯、翻墙等事件。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ6UHEw1wDpq5KUaaTPBhvWH9LlI0cpHTFjr79ERL2D4qiaIZccL0grib6g/0?wx_fmt=png)
产品功能考虑：在监控地图里设置关注区域，当关注区域一旦出现人/车/其他物体，则可立即实现告警。特别适合对入侵行为需要重点防范的场合，比如展馆、监狱、禁区等地。
c.**异常行为检测**：目标突然发生剧烈变化，如打架斗殴等行为。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ66lrKQZ9ibNqN771TU2giartcmlT7W0KAW8LseYQaCRMellKSIu9pn9xA/0?wx_fmt=png)
产品功能考虑：对目标设定异常行为检测，当出现异常行为时即可告警。异常行为检测适合在对异常行为有需要重点防范的场合，比如学校、公共区域等地方。
**2）车辆识别：**计算画面中车辆的数目，并确定其位置。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ60PFgy3qGkZulWYaRRbeOYaAJMHxckeERDkPdlicyx7rMCPiavMWCyrNg/0?wx_fmt=png)
还可以和车型识别、车颜色识别、车辆逆流检测等结合。实现对车辆特点的全识别。
应用场景：
a.可用于**交通疏散**，针对有可能发生拥堵的区域提前进行部署。
b.可用于**追踪黑名单车辆**：车辆检测可识别车辆类型、车辆颜色等等，这些信息均可用来定位目标。
c.**防车辆套牌**方案：车辆识别和车牌识别结合，防止车辆套牌案件发生
3）应用于**智能驾驶方案**：目标识别和场景分割、SLAM结合，可识别出道路路况，提供智能驾驶所需要的路边物体信息。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ6BKKm1p8aYbicalUz0HWJAOibTA52hnbSOIiaXEjzehDeDqUPIz1DAqMOg/0?wx_fmt=png)
4
**多目标跟踪的应用场景**
多目标跟踪监控类的产品现在已经有不少，拿商汤的软件产品作为典型例子：
![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ6NTsiaxX5haqTgkwXpnB5b2gFAWNEpic53ebapic51stpkWogfHmoAXkzg/0?wx_fmt=jpeg)
**1、产品功能分析**
产品的功能主要有以下内容：摄像头个数、行人人次、非机动车车次、机动车车次、告警次数、监控画面、目标抓拍。
我们从大的划分来看，主要是四块：**设备、统计、监控、抓拍**。除了这四块之外，根据经验我们还需要考虑到**历史的分析和检索**。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEgQfibmsaAUloxbkhM0aksZ66ylkJSluScPyTsM7mohvldeQloiaG1IteyzDvJjsibDoy369E6wibL6DA/0?wx_fmt=png)
**1）设备**
商汤的产品只体现了设备的数目，而更重要的是设备的筛选功能。其实设备的控制非常复杂，我们常常为了监控摄像头的状态设计一个新的管理系统。在另外一个设备的管理系统中，我们需要跟踪设备的使用状态，设备的详细信息（对应的摄像头编号位置等），以及摄像头的定位。
**2）统计**
统计除了上例时间段的统计之外，还可以进行**某一时刻某一区域上的目标统计**。某一时刻的统计的意义比时间段统计在一些特定需求上更为重要。特别是某时刻目标数如果超过阈值，跟告警系统匹配，能够捕捉群体行为。某时刻的目标数目是否超过设定阈值，是多目标最常见的需求，也是典型应用之一。
**3）监控**
监控界面结合了多种类别的识别：例如行人的性别、头发长短、衣物类型等等。这也是多目标最“酷炫”的一部分，人们常常被系统的自动识别吸引。多目标在前端的表示常常是**一个Boxing加一条尾巴**，即**目标的定位和轨迹**。而目标旁的标签（属性），则是多目标识别和其他垂直方向识别的结合。
产品在这块有两部分工作：
a.跟进多目标的**漏检率**以及**IoU**，以及思考同一管道多种属性识别的结合，以应对不同场景需求。更偏向技术类。
b.关注监控系统的**流畅性**和**稳定性**。更偏向业务类。
**4）抓拍**
**抓拍也是体现多目标价值的典型功能之一。**抓拍能将监控中的目标单独地隔离出来，这是后续检索目标功能实现的前提。抓拍到的图片在界面展示时可利用缓存，之后建议存储至云端。
**5）历史**
**a.搜索：**跟普通搜索系统相比，多目标系统必不可少的一个功能一定是图片检索。在系统中利用文字（与多目标的标签关联）或图片进行图片检索，能更容易查找到目标。
**b.分析：**利用大数据技术，对历史的人流、车流、告警记录进行分析，有利于目标管控和后期阈值设置。
**2、产品系统分析**
**1）实时性**
监控系统最重要的一点是实时性，相比于普通监控产品，AI监控系统对设备硬件的要求更高。图像处理速度和网速需要被综合评估，监控系统的延时现象需要控制在用户可接受范围内。
**2）耦合性**
AI监控系统链路长、设计到的子系统众多，因此在产品架构设计上需要考虑到程序之间的耦合性。耦合程度太高会使系统的升级更新与维护更加困难。
**3）稳定性**
这里的稳定性不仅包括外界环境的影响，还有系统自身算法更新、程序更新对系统的影响。特别是**算法更新**，不仅需要在指标上有所提高，还需要具有一定兼容性。算法更新常常导致程序短时间内无法使用，所以每一次算法替换都需要谨慎考虑。

