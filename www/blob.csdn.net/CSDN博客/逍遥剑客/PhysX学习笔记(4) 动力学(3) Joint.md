# PhysX学习笔记(4): 动力学(3) Joint - 逍遥剑客 - CSDN博客
2009年07月28日 22:49:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3155
(关节)Joint 
Joint和contact是约束body运动的两种方式
Joint坐标系:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090728/Joint_Frame.JPG)
Joint Limit: 相当于关节的活动范围
Breakable Joint: 可以断开的, 由setBreakable控制. 断开时会响应onJointBreak()
Joint类型:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090728/Spherical%20Joint.JPG)
球形关节: 肩膀, 绳子
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090728/Revolute%20Joint.JPG)
卷动关节: 门
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090728/Prismatic%20Joint.JPG)
棱柱关节: 防震器
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090728/Cylindrical%20Joint.JPG)
圆柱关节: 天线
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090728/Fixed%20Joint.JPG)
固定关节: 可打断物体
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090728/Distance%20Joint.JPG)
距离关节: 钟摆, 弹簧
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090728/Point%20In%20Plane%20Joint.JPG)
点面关节: 磁铁
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090728/Point%20On%20Line%20Joint.JPG)
点线关节: 窗帘挂环
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090728/Pulley%20Joint.JPG)
滑轮关节
6DOF(Degree-of-Freedom)关节: 自定义
弹簧: 不建议使用, 应该用distance joint代替
