# RGB_D_开发征程（使用Kinect） - wishchinYang的专栏 - CSDN博客
2013年08月20日 09:19:49[wishchin](https://me.csdn.net/wishchin)阅读数：1325
学习历程依此为纲！
**Kinect学习资料：**
[kinect和openNI](http://blog.csdn.net/chenli2010/article/details/6887646)开发资料汇总：[http://blog.csdn.net/chenli2010/article/details/6887646](http://blog.csdn.net/chenli2010/article/details/6887646)
原始博客已删除，我还是复制一下：
Kinect到手快一个月了，期间查阅了很多资料，见识了很多牛人，他们的工作如此漂亮，让我大开眼界。现将自己所掌握的资料汇总于此，以便随时查阅。
首先是csdn上小斤童鞋的系列文章：
[Kinect开发教程一：OpenNI的安装与开发环境配置](http://blog.csdn.net/chenxin_130/article/details/6693390)
[Kinect开发教程二：OpenNI读取深度图像与彩色图像并显示](http://blog.csdn.net/chenxin_130/article/details/6696187)
[Kinect开发教程三：利用OpenNI进行手势识别](http://blog.csdn.net/chenxin_130/article/details/6703301)
[Kinect开发教程四：用Kinect控制鼠标玩水果忍者PC版](http://blog.csdn.net/chenxin_130/article/details/6706630)
其次是mp77技术交流频道上的系列文章，基于openNI的《user guide》来写的，简明易懂（作者自称西大学生，俺不知是哪个西大）：
[Kinect内幕——解析SDK(OpenNI
 Framework 1)](http://www.mp77.cn/blog/?p=339)  ：PrimeSense OpenNI Framework的基本架构和设计思路
[Kinect内幕——解析SDK(OpenNI
 Framework 2)](http://www.mp77.cn/blog/?p=342)  ：工作节点的创建和销毁
[Kinect内幕——解析SDK(OpenNI
 Framework 3)](http://www.mp77.cn/blog/?p=344)  ：openNI编程
[Kinect内幕——解析SDK(OpenNI
 Framework 4)](http://www.mp77.cn/blog/?p=346)  ：工作节点的配置
然后是台湾牛人heresy的系列介绍文章，他好像是第一个用汉字介绍Kinect和openNI的先驱者，小斤童鞋的文章基本上是参照他的（万恶的资本主义世界的人咋就有了得天独厚的优势！）：
[透過
 OpneNI 讀取 Kinect 深度影像資料](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=215)
[透過
 OpneNI 合併 Kinect 深度以及彩色影像資料](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=216)
[透過
 OpenNI 建立 Kinect 3D Point Cloud](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=217)
[錄製、重播
 OpenNI 擷取到的資料](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=221)
[透過
 OpenNI / NITE 分析人體骨架（上）](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=219)
[透過
 OpenNI / NITE 分析人體骨架（下）](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=220)
[OpenNI
 的手勢偵測](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=234)
[OpenNI
 人體骨架分析部分補充](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=240)
[讓
 OpenNI 可以透過微軟 Kinect SDK 讀取 Kinect 的資料！](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=245)
[OpenNI
 XnSkeletonJointOrientation 簡單分析](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=258)
[使用
 XML 設定檔來初始化 OpenNI](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=259)
[OpenNI
 的手部追蹤、外加簡單的滑鼠模擬](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=261)
[OpenNI
 的 User Generator](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=262)
晨宇思 远的[博客](http://blog.csdn.net/chenyusiyuan)，关于openNI的好像只有下面一篇文章，但他的opencv的文章有很多，很值得一读：
[OpenCV学习笔记（20）Kinect
 + OpenNI + OpenCV + OpenGL 组合体验](http://blog.csdn.net/chenyusiyuan/article/details/6279762)
同样是牛人heresy的系列文章，只是它长在一个不和谐的地方，怎么访问大家都懂的：
[OpenNI / Kinect 相關文章目录](http://kheresy.wordpress.com/index_of_openni_and_kinect/)
继续来CSDN上的童鞋博客：
[利用KINECT+OPENCV检测手势的演示程序](http://blog.csdn.net/firefight/article/details/6304050)
国内kinect开发的主要社区之一，其中有很多资源，有时间不妨多逛逛：
[kinect 体感游戏网](http://www.cnkinect.com/)
cnkinect上的童鞋发起的*Kinect for PC* 项目，是学习kinect的又一个好地方：
[kinect for pc 项目的wiki主页](http://www.javaforge.com/project/Kinect2PC)
**PCL学习资料：**
        小南南的博客：[http://blog.csdn.net/chen_nan/article/details/7072110](http://blog.csdn.net/chen_nan/article/details/7072110)不过写的太乱了！
PCL官方学习资料：
        PCL中国的教程非常详细：[http://www.pclcn.org/study/news.php?lang=cn&class1=85&class2=102&class3=105&page=8](http://www.pclcn.org/study/news.php?lang=cn&class1=85&class2=102&class3=105&page=8)（从第八页开始）
        还有一点要补充：中文官方 的教程 有些不妥的地方：就是 为了卖书准备的.....
        所以不想 读教程时有骨鲠在喉的感觉，还是看英文官网吧！！！
