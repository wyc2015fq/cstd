# guidance使用方式问题 - KinboSong的博客 - CSDN博客
2017年06月01日 13:35:21[songjinbo3](https://me.csdn.net/KinboSong)阅读数：1245
注意：guidance的左右相机数据都是矫正过后的
1、经验证，sdk和障碍物感知功能可以同时开启
2、由于guidance usb的带宽问题，不可能同时订阅相机数据和深度数据，并且最多只能订阅两个方向上的深度图数据；具体带宽是多少没有查到；可以参考下图：
![](https://img-blog.csdn.net/20170601145857196?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3、开启障碍物感知功能（配合M100使用）：
（1）开启dji GUIDANCE的障碍物感知功能
（2）开机dji go app的guidance
（3）5路传感器必须全部安装，并且安装的位置和方向正确
（4）飞行高度要大于0.5米
![](https://img-blog.csdn.net/20170601150128949?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvS2luYm9Tb25n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
