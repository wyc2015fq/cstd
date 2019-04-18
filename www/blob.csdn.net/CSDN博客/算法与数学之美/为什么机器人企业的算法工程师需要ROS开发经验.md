# 为什么机器人企业的算法工程师需要ROS开发经验 - 算法与数学之美 - CSDN博客
2018年03月22日 00:00:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：545
ROS的英文全称是Robot Operating System，即机器人操作系统。TA集成了各种工具、库以及规定，以便简化在各种平台下进行机器人开发的复杂性，为开发者提供了一个自由而宽松的框架，专门用来开发机器人的应用。
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/GsVBs6tAVTpmSJuJrjAUPzyaTysJgo9cgH6JKrA1Q3Rib1aYibz286JCgj90vc5N01TbaSZaCMSR8jFCMhKaNZjw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
图１　支持ROS的部分机器人
从2007年发展至今，ROS已成为世界上应用范围最广的通用机器人软件开发平台，这主要得益于其通用性、开源性、复用性、社区性等特点。目前全球都点燃了ROS编程热，机器人研究者们可以使用其他ROS用户贡献出的大量ROS package来编程，社区的功能包数、关注度和文章成指数型变化。
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/GsVBs6tAVTpmSJuJrjAUPzyaTysJgo9c45NxVclTibhpHxn8HDUmFAXny6325UMsyR3KPoEYWnCJiczb5dLJHPvw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
图２　ROS社区的功能包数、关注度和文章变化
ROS已经涵盖了导航定位（SLAM）、3D物体识别、运动规划、多关节机械臂运动控制、机器学习等，甚至可以让机器人协同完成打桌球、人机互动等高难度组合动作。除了我们熟知的百度Apollo无人车是基于ROS开发的，新松机器人、大疆、地平线等诸多机器人或者无人驾驶的企业在招聘ROS软件开发工程师，或者具备ROS开发经验的算法工程师。
为了尽快掌握ROS基础，并实践基于ROS的机器人常见任务（机器感知、SLAM自主导航、机械臂控制），专注于前沿科技在线教育的深蓝学院，联合知名机器人博客“古月居”博主古月老师，推出**《机器人操作系统ROS理论与实践》**在线系列课程。课程每章节均安排有作业及参考资料，根据作业打分评选优秀学员，优秀学员可获得证书，并推荐到实习就业。
**课程讲师**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/GsVBs6tAVTpmSJuJrjAUPzyaTysJgo9cxsNfNtd1DcyWg5K2esx7nic8aXJxYBaTUMuIicpUa7p4GibNVQLNBcCoQ/640?wx_fmt=jpeg)
**胡春旭**（古月），知名机器人博客“古月居”博主，主编书籍《ROS机器人开发实践》，深圳星河智能科技有限公司联合创始人，华中科技大学自动化学院硕士。目前主要从事机器人控制系统的研究开发。
作为国内最早一批ROS开发者，具备多年开发、应用经验，参与研发过轮式移动机器人、机械臂等机器人产品。曾获开源硬件与嵌入式大赛机器人组全国冠军，飞思卡尔杯智能汽车竞赛全国一等奖，中国人工智能机器人学术会议十佳论文等奖项。
**课程特色**
**1. **理论结合实践，降低入门门槛；
**2. **课上在线答疑，课下微信群答疑；
**3. **作业设置新颖，批阅讲解仔细；
**4. **优秀学员获颁证书，推荐实习就业；
**5. **课程**PPT和代码**，提前公布下载。
**课程目录**
**1. ****认识ROS**
    1.1 课程介绍
    1.2 ROS简介
    1.3 ROS系统架构
    1.4 ROS安装
    1.5 第一个ROS例程——小海龟仿真
**2. ****ROS基础**
    2.1 创建工作空间
    2.2 ROS通信编程
        2.2.1 Topic编程
        2.2.2 Service编程
        2.2.3 Action编程
    2.3 实现分布式通讯
    2.4 ROS中的关键组件
        2.4.1 Launch启动文件
        2.4.2 TF坐标变换
        2.4.3 Qt工具箱
        2.4.4 rviz可视化平台
        2.4.5 gazebo物理仿真环境
**3. ****机器人系统设计**
    3.1 机器人的定义与组成
    3.2 机器人系统构建
        3.2.1 执行机构
        3.2.2 驱动系统
        3.2.3 传感系统
        3.2.4 控制系统
    3.3 URDF机器人建模
        3.3.1 URDF模型
        3.3.2 创建机器人模型
        3.3.3 添加传感器
**4. ****机器人仿真**
    4.1 机器人模型优化
        4.1.1 xacro文件格式
        4.1.2 xacro的可编程特性
        4.1.3 URDF模型的优化
    4.2 ArbotiX+rviz功能仿真
    4.3 gazebo物理仿真
        4.3.1 ros_control
        4.3.2 创建仿真环境
        4.3.3 机器人仿真
        4.3.4 传感器仿真
**5. ****机器人感知**
    5.1 机器视觉
        5.1.1 ROS中的图像数据
        5.1.2 摄像头标定
        5.1.3 ROS中的OpenCV接口
        5.1.4 人脸识别/物体识别
        5.1.5 二维码识别
    5.2 机器语音
        5.2.1 ROS集成科大讯飞SDK
        5.2.2 实现智能语音助手
**6. ****机器人SLAM与自主导航**
    6.1 机器人必备条件
    6.2 ROS中常用SLAM功能包的应用
        6.2.1 gmapping SLAM
        6.2.2 hector SLAM
        6.2.3 ORB_SLAM
        6.2.4 google cartographer SLAM
    6.3 ROS中的导航框架
    6.4 导航框架的应用
        6.4.1 本地规划器与全局规划器的配置
        6.4.2 Gazebo导航仿真
**7. ****MoveIt!机械臂控制**
    7.1 MoveIt!系统架构
    7.2 创建机械臂模型
    7.3 Setup Assistant配置机械臂
    7.4 添加机器人控制器
    7.5 MoveIt!编程学习
    7.6 Gazebo机械臂仿真
    7.7 ROS-I框架介绍
**8. ****ROS机器人综合应用**
    8.1 ROS机器人实例介绍
           8.1.1 PR2
           8.1.2 TurtleBot
           8.1.3 HRMRP
           8.1.4 Kungfu ARM
    8.2 构建综合机器人平台mbot
           8.2.1 移动机器人+机械臂
           8.2.2 综合应用
**9. ****ROS 2.0**
    9.1 为什要有ROS 2
    9.2 什么是ROS 2
    9.3 如何安装ROS 2
    9.4 ROS 2中的话题与服务编程
    9.5 ROS 2与ROS 1的集成
    9.6 课程总结与展望
**报名咨询**
课程费用499元，现在报名可领取** 100元立减优惠券 **；课程采用在线授课，一年内可以随时回放。
请添加工作人员「**深蓝学院**」**助教**报名
回复“算法与数学之美”领取优惠码
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/GsVBs6tAVTpfWKJOvQic5WLW9fddD23bAjxwS7okW9geonanpC3ToZt4X1XMZRDJZG3eoTRMJzH4z5M9rxnWGiag/640?wx_fmt=jpeg)
