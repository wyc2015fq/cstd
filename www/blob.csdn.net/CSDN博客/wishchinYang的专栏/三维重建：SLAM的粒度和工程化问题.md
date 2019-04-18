# 三维重建：SLAM的粒度和工程化问题 - wishchinYang的专栏 - CSDN博客
置顶2019年04月14日 11:33:47[wishchin](https://me.csdn.net/wishchin)阅读数：5008
        百度百科的定义。此文引用了其他博客的一些图像，如有侵权，邮件联系删除。
        SLAM不是一个单一算法，是个工程。在[计算机视觉](http://baike.baidu.com/view/155265.htm)中, 三维重建是指根据单视图或者多视图的[图像重建](http://baike.baidu.com/view/1288444.htm)三维信息的过程. 由于单视频的信息不完全,因此三维重建需要利用经验知识.。而多视图的三维重建(类似人的双目定位)相对比较容易, 其方法是先对摄像机进行标定, 即计算出摄像机的图象[坐标系](http://baike.baidu.com/view/84791.htm)与[世界坐标系](http://baike.baidu.com/view/829500.htm)的关系.然后利用多个[二维](http://baike.baidu.com/view/719535.htm)图象中的信息重建出三维信息。
        三维重建根据时间和场景的粒度不同需要引入不同的定义和工程化方法：
**一、单幅图像的三维重建**（单帧三维图像获取-硬件级别工程化）
**0.硬件设备**
        单幅图像的三维重建主要讲解基于整图视觉的传感器，线装扫描思想和整图思想分别使用了局部和整体的思路来完成2.5D图像获取。
        游戏玩家是推进感知硬件发展的一个重要角色，参考：[AR和VR的演进](https://blog.csdn.net/wishchin/article/details/46515909)哲学 。消费级深度图像获取设备更多的卖给了游戏玩家，其中最典型的为PrimeSense 和微软合作开发的KinectV2.V1，索尼的PsEye，都有千万级别的销量，很多科研院所直接使用Kinect作为实验设备，两种设备都使用基于红外线的结构光思路，此外IntelRealSense设备价格较高且性能较差，但更适用于近距离使用，也能广泛被使用。在工业三维传感器厂商中，意法半导体、NI、德州仪器、英飞凌等一众公司亦有典型的3d传感器。
      赶上VR、AR风潮，以色列的inuitive在2015年即推出一款基于TOF的3d传感器，尺寸不到4cm~3，分辨率可达QVGA，有效感知距离为10-60cm，感知粒度在有效范围最远处，可识别粒度达到1cm。以色列Inuitive公司开发基于双[CEVA-MM3101引擎的3D视觉处理器](http://www.eefocus.com/consumer-electronics/332789/p3)，实现3D景深图。可直接嵌入AR眼镜，用于手势识别。但是介于驱动移植复杂度问题，我们未能取得合作把X86平台的驱动程序移植到安卓平台，因此更换了其他方案。
**1.单帧三维图像获取（**硬件级别光线分析**）**
        从传感器级别考虑光线对环境进行重建，可以直接引入测距方法得出感知点的距离——深度信息。（百科描述：测距仪是一种测量长度或者距离的工具，同时可以和测角设备或模块结合测量出角度，面积等参数。测距仪的形式很多，通常是一个长形圆筒，由物镜、目镜、显示装置（可内置）、电池等部分组成。）
        激光测距仪是利用调制激光的某个参数实现对目标的距离测量的仪器。按照测距方法分为 相位法 测距仪和脉冲法 测距仪，脉冲式激光测距仪是在工作时向目标射出一束或一序列短暂的脉冲激光束，由光电元件接收目标反射的[激光束](http://baike.baidu.com/view/369196.htm)，[计时器](http://baike.baidu.com/view/928508.htm)测定激光束从发射到接收的时间，计算出从观测者到目标的距离。相位法激光测距仪是利用检测发射光和反射光在空间中传播时发生的相位差来检测距离的。
        从单点延伸到整幅图像，延伸出**飞行时间图像传感器**和**相位图传感器**，并根据光线是否主动发射划分出**单目相机**（相位图）和**多目相机**（视差方法）。激光可以使用其他特定光线替代，比如红外光。在广泛被使用的直接深度图像获取设备之中，微软kinect、asus Xtion Pro、索尼psEye等使用相位法得到深度图像。
        此外，google project tango 作为此类新的代表，遵循根本原理：[谈了这么久的Project Tango？我们终于把它讲清楚](http://www.leiphone.com/news/201605/yPFSqJ7liGV1kPmS.html)。
       （Kinect 红外发射器(Infrared Projector)发射出雷射光，通过红外发射器镜头前的光栅(diffuser)，均匀地投射到测量空间，测量空间的粗糙物体反射，形成随机的散斑，再通过红外摄影机(Infrared Camera)记录空间的每个散斑，通过晶片的计算便得到3D深度图像。参考：[*Kinect 深度图*获取原理_winsolider_新浪博客](http://www.baidu.com/link?url=GMLZp21yFRIxmBZVy6nT9cXO_2i7-ge8IkdGvNTR-WihAjuMlxdTR5WTn94-sfVK53JnhrcOoyMEj4aTcdK8Dq&wd=&eqid=d5ca95af000073780000000358a54c6d)）
        深度图像获取设备的探索厂商Primesense的相位光获取深度图像的专利描述。
![谈了这么久的Project Tango？我们终于把它讲清楚](http://static.leiphone.com/uploads/new/article/740_740/201605/57411eb4f2034.png?imageMogr2/format/jpg/quality/90)
       TOF飞行时间法3D成像，是通过给目标连续发送光脉冲，然后利用传感器接收从物体返回的光，通过探测光脉冲的飞行时间来得到目标物的距离。TOF的深度精度不随距离改变而变化，基本能稳定在cm级.
       关于TOF的总结：[TOF相机总结](http://www.cnblogs.com/freshmen/p/4818196.html) ，此文对时间节点之前的主要TOF相机进行了总结。 目前主流的有TOF相机厂商有PMD、MESA、Optrima、微软等几家，其中MESA在科研领域使用较大，相机紧凑性好，而PMD是唯一一款能够在户内、户外均能使用的TOF相机，并且能够具有多种探测距离，可用于科研、工业等各种场合。而Optrima、微软（还不是真正的TOF技术）的相机主要面向家庭、娱乐应用，价位较低。
       工业领域，TOF占据主流，参考：[意法半导体：看好飞行时间测距传感器(ToF)市场发展前景](http://www.c114.net/news/211/a970284.html) 。意法半导体是全球第一大全功能集成飞行时间(ToF)传感器模块厂商，同时意法半导体的FlightSense也是全球市场排名第一的ToF解决方案
**2.单帧三维图像获取（二维图像的经验性恢复）**
        使用单幅图像进行深度图像恢复是一个经验性的问题，各个论文的改进只在于引入更加复杂的先验模型，此处不能再多加论述。
        经验性模型的意义是，在一个宽幅图片中，恢复出所谓的先验真实场景。 一堆趣图，人眼的三维重建错觉：你可知道，人眼的后面，跟着一整个大脑！
![](https://img-blog.csdn.net/20170330104532870?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170330104535603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170330104538745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        参考资料：[错觉大师埃舍尔](http://sanwen.net/a/qxwqooo.html)
        宗教是弱者的避难所，忠实信徒也是被教义摆布的真实羔羊：[世界上最不寻常的九个教堂](http://travel.sohu.com/20141008/n404918931.shtml)
        其实，铁轨的两端是不等宽的，这个铁轨根本就是一个装饰品。
**总结：**
       在工程领域，单幅图像进行三维重建可视为单一固定场景的恢复，即平时所说的2.5D场景。一般使用光线分析的方法，基于模型的经验性恢复是不可行的，所有基于经验的方法都会有经验性障碍，最终重建的准确性和这个先验模型的负责度和覆盖域的大小直接相关。
       单幅三维图像获取，即2.5d三维图像的可信度，更多的依赖所使用的硬件水平，基础理论和光线多能达到的精度。
**二、多幅图像的三维重建**
       算法界主要集中于处理场景重建，即从多幅图像中，重建出基于世界坐标系的真实的三维场景映射。并且在大型场景重建中划分出基于时序的SLAM和可以非时序的SFM，SLAM为即时相机位姿确定和地图重建，SFM为运动中结构重建。
       以重建理论为指导，若排除噪音问题，多幅图像的三维重建是完备的，从多幅图像中一定能重建出与真实场景一致的一个3d场景。
**1.摄像机-场景一方相对固定的环境重建**
         相机相对固定的方法为相机相对于世界坐标系是固定的，或者相机位姿变化为平滑可追踪，需要长期误差对整个重建过程影响不大。
**1.1. 像素级别重建，也称为稠密重建**
经典工程案例kinect fusion分析
- Depth Map Conversion
- Camera Tracking
- Volumetric Integration
- Raycasting
        参考：[kinect fusion 3D重建基本算法](http://blog.csdn.net/xiaohu50/article/details/51592503) ，此文分析的比较详细，并且给出레TSDF的伪代码。
**预处理**：kinectFusion实际还对原始的深度信息进行了一定的降噪平滑，采用双边滤波（Bilateral filtering），在保留边缘的基础上进行平滑，是个可以接受的选择。
**Depth Map Conversion**：主要是求出有原先的图像点u=(x,y)，以及深度值D(u)，求得每个点的法向量n(u)。根据相机的内部矩阵，将图像2D坐标转化为相机原点坐标系的3D点。
![这里写图片描述](https://img-blog.csdn.net/20160605191938611)
**Camera Tracking**：此步骤用ICP（iterative closest point）算法，求解出相机每次的相对位移与转动。相机位置可以用来将相机原点坐标系的结果转化到世界坐标系。
**Volumetric Integration**：上一环节已经形成了每个相机图像个子的T和R，可以将每一帧采集的数据转化成世界坐标系当中。通过本环节的操作，我们可以形成水密的物体重建，采用TSDF（Truncated Signed Distance Function）的方法。
**后处理Raycasting**：之前的操作只是形成了*t**s**d**f**a**v**g*表示的物体的重建结果，要形成我们能见到的图片，还需要相应的渲染和投影。       
**总结**：
          稠密重建对图像中每个点进行世界坐标系转化，使用类似于拼接的方法，直接使用多帧图像拼接出最终的三维场景。其中稠密匹配的方法主要是ICP，（iterative closest point）算法。**ICP方法**在PCL库中用作点云匹配最主要的方法。参考PCL：[ICP算法实现](https://segmentfault.com/a/1190000005930422)
![](https://img-blog.csdn.net/20170216172543277?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170216172747526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
               图：[ICP算法用于点云](http://blog.csdn.net/u010696366/article/details/8941938)[registration](http://blog.csdn.net/u010696366/article/details/8941938)                      图：[ICP用于kinect场景重建](http://blog.csdn.net/fuxingyin/article/details/51417822)
**2.摄像机不固定的环境重建（**SLAM、大规模静态场景重建**）**
        三维重建的大致过程可形式化为固定世界坐标系后逐渐增加场景元素，是一个逐渐拼接增加地图的过程。相机平滑可追踪相当于转变位姿后的单幅图像相对于世界坐标系静止，场景数据是单幅图像的几倍到十几倍，累计误差较小。若场景较大，并且向一个维度或者几个维度延伸较长，场景匹配的累计误差变得不可忽略，消误差的方法变得占据重要比重。
**2.1.像素级别重建**
         使用kinect适用小型目标或者小范围内的室内场景，需要相机追踪的长期累计误差对整个重建过程影响不大。对于大型地图，相机位姿转换漂移误差较大，且对三维图像进行配准拼接需要更高的计算量，在相对苛刻的环境中达不到实时性的要求。在特定的应用场景中，不对所有像素进行配准和拼接，只对特定的点和图像区块重建，引入稀疏重建的方法。
        此外，大规模静态场景的像素级别重建一般引入精确测距仪作为辅助，是一个**大型工程问题**，不是一个算法和框架可以描述的，需要更多设备和人的配合。再次不再对其进行多加描述。
![](https://img-blog.csdn.net/20170216174246937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170216174349250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                      图一： 点阵场景                                               图二：稀疏点场景
**2.2.特征点级别重建，也称为稀疏重建**
        场景从几平方米到几十平方米的室内以至于更宽广的室内环境，并且扩展到室外。由少量的设备完成满足特定需求的场景重建，相关行业都有各自的要求。其中对于机器人行业来说，机器人的重要功能为路径规划和位移，且特定机器人的运动场景较大，实时性较高，在多数情况下不需要进行像素级别的重建，进行特征点级别的稀疏重建是一种必要的方法。
        对于机器人学通常认为的SLAM问题，是基于相机不固定的三维场景重建问题，大多数情况下需要处理的是基于稀疏点的稀疏重建问题。
        即时相机不固定的未知场景环境重建，SLAM，**simultaneous localization and mapping** (**SLAM**)，即时位置确定和地图构建。在一个设定世界坐标系中，同时确定相机的位姿和构建增量地图。
        SLAM作为一个鸡和蛋问题，随着机器人学的发展已经有较长的历史，并发展处不同的方法。限制于硬件本身的计算能力，机器人广泛使用的SLAM方法一般为特征点级别的稀疏重建，重建的地图为拓扑地图。并由此发展出一些列的SLAM方法。
        单目SLAM方法和双目SLAM方法的区别在于双目给出了相机固定的基线，给出相对精确的绝对尺度，类似于一个深度相机。参考链接：[单目SLAM解决方案](http://blog.csdn.net/wishchin/article/details/49424467)。SLAM发展历程，对专注于SLAM的方法进行了梳理，比较详细的介绍了SLAM的基本线路—[SLAM的发展历程](http://blog.csdn.net/wishchin/article/details/47322271)。
        小型室内场景稀疏重建同样是可以一个工程师完全搞定的工程问题，slam开始从一个算法问题演变到一个由多个算法复合推进的工程问题。
![](https://img-blog.csdn.net/20170216175315525?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170216175432937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                     图：拓扑地图表示                                                                       图：白色标记点为特征点
**2.3.目标级别重建/语义SLAM/目标环境重建**
        更大范围的三维场景重建，比如无人机绘制地图以及地图公司实现三维导航和目标定位，三维重建/SLAM问题演化为长时序问题，面对更大的时序误差累计和数据增量累计问题。
        更大范围的三维场景重建需要更重要的消除误差的方法，大的拓扑地图消耗大量的特征点标记，而构建拓扑地图的特征点块描述能力有限，需要更高层次的描述来进行场景拼接。
        因此消除误差的方法引入了SLAM中经典的回环检测和目标识别，即SLAM过程引入了目标物体标记，演变为语义SLAM。
        回环检测经典算法为使用词袋模型，对成组的特征点进行模式识别，识别已扫描场景，找到“回环”，用来校正闭环内的误差。
        同时目标识别可以看做为建立一个描述能力更强的地图标记，并且这个标记相对于单幅图像几十个数百个特征点数量更少，在描述大范围场景时需要更少的拓扑标记。
        目标识别和目标级别地图构建一般作为特征点场景稀疏重建的**并行过程**，在此过程中可以引入一些列目标识别的方法。更大的场景和更多的目标需要对整个重建过程进行分层和适当颗粒化，因此适时引入场景识别，用以限制数据库中识别目标物体数目的数量，只对特定的场景进行特定的目标识别。
        工程化困难：基于目标级别的重建，即语义SLAM，场景标记和场景识别占据了更多的地位。语义SLAM的工程化重点开始偏向于场景识别，并在整个语义SLAM工程中占据更多的工作量。
![](https://img-blog.csdn.net/20170216174349250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170216174637740?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图1：稀疏地图中检测到回环                                            图2：目标地图，目标作为地图地标
             图片援引于：[SLAM系统SLAM-ware](http://m.51xw.net/tech/2015/07/4358.html)[http://m.51xw.net/tech/2015/07/4358.html](http://m.51xw.net/tech/2015/07/4358.html)
![](https://img-blog.csdn.net/20170216174958509?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
          场景识别：参考：[场景/图像的稀疏表示——ScSPM和LLC的总结](http://blog.csdn.net/wishchin/article/details/12452331)
**总结**：
        受制于计算和存储问题，随着场景的扩大，三维重建受到的限制增加，重建的粒度需要改变。
        对于小型目标或者场景可以使用固定相机，或者可以不必重视相机位姿变换累计误差，进行像素级别的稠密重建。实时性要求较高或者环境数据增加，图像数据和地图数据暴增，或者相机位姿转换空间变大时间变长，基于特征点块的稀疏重建作为一种合适的方法普遍被使用。对于更大的场景或者更长时间的相机位姿变换，出现更多的数据累计更多的误差，环境描述方法因此也发生变化，基于目标的环境表示作为大范围拓扑地图作为辅助重建的方法是合适的。此外随着目标标记物的增加，引入场景识别可以缩小目标识别的范围。
        三维重建问题，是个工程问题，根据不同的场景需要评估相应的复杂度。根据不同的复杂度进行工程模块分解，根据工程需要对每一层使用不同的方法。
