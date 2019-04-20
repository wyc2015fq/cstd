# 对比激光SLAM与视觉SLAM：谁会成为未来主流趋势？ - 人工智能学家 - CSDN博客
2018年11月13日 23:14:39[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：311
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVUBJdQEBNnn7VicRcwSOEjFIktvJvhtbyavVndbJdHGFeSyagdbJmNElde4qpSozmnOfryw7CpPAA/640?wx_fmt=jpeg)
来源：智车科技
摘要：SLAM（同步定位与地图构建），是指运动物体根据传感器的信息，一边计算自身位置，一边构建环境地图的过程，解决机器人等在未知环境下运动时的定位与地图构建问题。
目前，SLAM 的主要应用于机器人、无人机、无人驾驶、AR、VR 等领域。其用途包括传感器自身的定位，以及后续的路径规划、运动性能、场景理解。 
由于传感器种类和安装方式的不同，SLAM 的实现方式和难度会有一定的差异。按传感器来分，SLAM 主要分为激光 SLAM 和 VSLAM 两大类。其中，激光 SLAM 比 VSLAM 起步早，在理论、技术和产品落地上都相对成熟。基于视觉的 SLAM 方案目前主要有两种实现路径，一种是基于 RGBD 的深度摄像机，比如 Kinect；还有一种就是基于单目、双目或者鱼眼摄像头的。VSLAM 目前尚处于进一步研发和应用场景拓展、产品逐渐落地阶段。
**激光 SLAM：**早在 2005 年的时候，激光 SLAM 就已经被研究的比较透彻，框架也已初步确定。激光 SLAM，是目前最稳定、最主流的定位导航方法。
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW9pHrL25zpycToeUXANiaA2KXsW89fxWJ8c5eDNxgq846GgNp58kAL6PCnkvkiaic6BvtyWkeEENM5hg/640?wx_fmt=gif)
激光 SLAM 地图构建
VSLAM（基于视觉的定位与建图）：随着计算机视觉的迅速发展，视觉 SLAM 因为信息量大，适用范围广等优点受到广泛关注。
（1）基于深度摄像机的 Vslam，跟激光 SLAM 类似，通过收集到的点云数据，能直接计算障碍物距离；
（2）基于单目、鱼眼相机的 VSLAM 方案，利用多帧图像来估计自身的位姿变化，再通过累计位姿变化来计算距离物体的距离，并进行定位与地图构建；
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW9pHrL25zpycToeUXANiaA2K3ELgm4EGkwvc3gbKw2uia2ibwPsmauYB01JrXXbwIyZDAZjy5DibnjDzA/640?wx_fmt=gif)
视觉 SLAM 地图构建，图片来源：百度 AI
一直以来，不管是产业界还是学术界，对激光 SLAM 和 VSLAM 到底谁更胜一筹，谁是未来的主流趋势这一问题，都有自己的看法和见解。下面就简单从几个方面对比了一下激光 SLAM 和 VSLAM。
**成本**
不管是 Sick，北洋，还是 Velodyne，价格从几万到几十万不等，成本相对来说比较高，但目前国内也有低成本激光雷达（RPLIDAR）解决方案。VSLAM 主要是通过摄像头来采集数据信息，跟激光雷达一对比，摄像头的成本显然要低很多。但激光雷达能更高精度的测出障碍点的角度和距离，方便定位导航。
**应用场景**
从应用场景来说，VSLAM 的应用场景要丰富很多。VSLAM 在室内外环境下均能开展工作，但是对光的依赖程度高，在暗处或者一些无纹理区域是无法进行工作的。而激光 SLAM 目前主要被应用在室内，用来进行地图构建和导航工作。
**地图精度**
激光 SLAM 在构建地图的时候，精度较高；VSLAM，比如常见的，大家也用的非常多的深度摄像机 Kinect，（测距范围在 3-12m 之间），地图构建精度约 3cm；所以激光 SLAM 构建的地图精度一般来说比 VSLAM 高，且能直接用于定位导航。
**易用性**
激光 SLAM 和基于深度相机的 VSLAM 均是通过直接获取环境中的点云数据，根据生成的点云数据，测算哪里有障碍物以及障碍物的距离。但是基于单目、双目、鱼眼摄像机的 VSLAM 方案，则不能直接获得环境中的点云，而是形成灰色或彩色图像，需要通过不断移动自身的位置，通过提取、匹配特征点，利用三角测距的方法测算出障碍物的距离。
**安装方式**
雷达最先开始应用于军事行业，后来逐渐民用。被大家广泛知晓最先应该是从谷歌的无人车上所知道的。当时 Velodyne 雷达体积、重量都较大，应用到一些实际场景中显然不适合。比如无人机、AR、VR 这种，本身体积就很小，再搭载大体积的激光雷达的话，根本无法使用，也影响美感和性能。所以 VSLAM 的出现，利用摄像头测距，弥补了激光雷达的这一缺点，安装方式可以随着场景的不同实现多元化。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/InRzPPAWvxXnic02rGOKnykzKBkk2ZiariaplSYkrgfqPvvze5KwjYmmCkHxS2EO4IfTvEw8jqmnASbkTqI9WonkA/640?wx_fmt=jpeg)
**其他**
除了上面几点之外，在探测范围、运算强度、实时数据生成、地图累计误差等方面，激光 SLAM 和视觉 SLAM 也会存在一定的差距。
比如：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW9pHrL25zpycToeUXANiaA2KQCbxOCPVpy9Tyuk6qnJ6HmeV2yZIW93XXBrSyIdKzPwib73wic3IHKYA/640?wx_fmt=png)
 注：左为 Lidar SLAM，右为 VSLAM，数据来源：KITTI
可以明显看出，对于同一个场景，VSLAM 在后半程中出现了偏差，这是因为累积误差所引起的，所以 VSLAM 要进行回环检验。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW9pHrL25zpycToeUXANiaA2KNKSSibGUGricice8Zuicj8ibMQd83twFWnzsVTl64gZMJ7FxOOgq7pqcpMQ/640?wx_fmt=png)
激光 SLAM 是目前比较成熟的定位导航方案，视觉 SLAM 是未来研究的一个主流方向。所以，未来，多传感器的融合是一种必然的趋势。取长补短，优势结合，为市场打造出真正好用的、易用的 SLAM 方案。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
