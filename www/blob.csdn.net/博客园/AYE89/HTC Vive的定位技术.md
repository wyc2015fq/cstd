
# HTC Vive的定位技术 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[HTC Vive的定位技术](https://www.cnblogs.com/eniac1946/p/7356644.html)
|Posted on|2017-08-14 11:00|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7356644)|[收藏](#)
|Lighthouse空间定位，|chaperone|系统避免实际障碍物
|HTC vive所用的Lighthouse技术属于激光定位技术，Oculus Rift以及索尼PlayStation VR所用的定位技术都属于光学定位技术，其中Oculus Rift是红外主动式光学技术，索尼PlayStation VR则是可见光主动式光学技术。
|HTC Vive的Lighthouse室内定位技术
|特点：|低成本，定位消费级VR，有较高精度和较低延时
|HTC Vive采用的激光定位技术，具体构成是利用光塔+头显/手柄上的光敏传感器共同完成定位
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170814095840100-392302148.png)
|上图，对角线两个光塔会形成一个3D空间
|描述数学模型：|[瞎猜 htc lighthouse 定位数学模型（一）](https://link.zhihu.com/?target=http%3A//blog.csdn.net/mov2012/article/details/54982683)
|精度测试，定位精度可达0.3mm：|[外国专家告诉你Lighthouse追踪到底有多准确](http://news.17173.com/content/2016-07-18/20160718110714496.shtml)
|HTC Vive的|chaperone系统
|可避免实际环境中家具及障碍物，利用头显上的前置摄像头
|简单来说，当使用者连按两次控制器上的菜单键以后，Chaperone 便会启动，并将|现实世界中的场景叠加到你眼前的虚拟画面之上|。另外，当你走近预设区域的边界时，Chaperone 也会自动开启，以此来引起用户的注意。
|预设区域：宽-高（隔离出一个区域）
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170814115704443-1517804833.png)
|定位空间：
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170814111531412-2132223257.png)
|chaperone view1
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170814111644865-97962524.png)
|chaperone view2：Tron Mode（右侧）
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170814111847193-31467036.png)
|How to use the Chaperone camera on the HTC Vive
|如何检测墙和家具？
|有人建议像扫地机器人一样，记录和学习用户行动过的轨迹，形成room mapping；或者用SDK手工做：
|That kind of user-directed room mapping will be important for when positional tracking takes off in VR experiences. There are many ways to do this--user defined dimensions, Kinect-style room scanning, or positionally tracked boundary markers. I believe that the|SteamVR boundary system|--which looks very much like a holodeck wall grid, albeit blue--is defined by the user typing in the dimensional space of the room. I would prefer a system where I could use the|SteamVR controllers|to manually map out my play area, to take into account irregular obstacles like desks, beds, and other furniture. It’s good to hear from developers that the boundary system is part of the SDK--it’s software that will be made available for anyone making a Lighthouse-based game.
|\#补充：
|1.|[sony VR 的object avoidance system ](http://www.telepresenceoptions.com/2013/12/sony_patents_vr_headset_with_o/)
|2. 头显前置摄像头API教程（OpenVR）：|[Vive 前置摄像头浅析1](http://blog.csdn.net/qq_15309121/article/details/51878655)|，2，3
|限制：
|1）. 摄像头分辨率太低600X400左右，放大画面后效果无法接受；
|2）. 打开摄像头后游戏帧率降低，实时调用会带来性能上的额外要求；
|3）.摄像头刷新率30-60HZ，有明显画面迟滞，调高刷新率会导致定位丢失；
|4）.无法感知深度信息，比如没法做到接到别人扔给你的水果。







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
