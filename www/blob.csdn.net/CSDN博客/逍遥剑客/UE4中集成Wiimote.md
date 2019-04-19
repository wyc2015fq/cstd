# UE4中集成Wiimote - 逍遥剑客 - CSDN博客
2015年06月30日 11:55:54[xoyojank](https://me.csdn.net/xoyojank)阅读数：3211标签：[Wiimote																[UE4](https://so.csdn.net/so/search/s.do?q=UE4&t=blog)](https://so.csdn.net/so/search/s.do?q=Wiimote&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
# Wiimote
最早在玩[Wii的模拟器](http://www.dolphin-emulator.com/)游戏时候, 就发现其中有关于Wiimote的一些设置, 了解到其实Wii的控制器可以连接到PC上使用. 去年做了Kinect的体感游戏后, 也想了解下Wii的, 所以就淘宝买了Wiimote Plus. 完整的Wiimote有三部分: 
- Wii手柄(这个集成了motion plus) 
![Wiimote](https://img-blog.csdn.net/20150630112622891)
- Nunchunk(外号”鸡腿”) 
![双截棍](https://img-blog.csdn.net/20150630111313998)
- IR感应条 
![感应条](https://img-blog.csdn.net/20150630111545933)
跟PC通过蓝牙(驱动比较折腾, 我用Toshiba的BlueStack成功), 连接即可, Nunchunk和感应条是可选的. 除了按键外, Wii手柄和Nunchunk都有朝向信息(Pitch, Roll), 配合IR感应条可以获取Yaw和屏幕坐标(类似空鼠), 另外还可以获取加速度信息和角速度信息(需要MotionPlus). 如果能使用Dolphin模拟器玩大多数Wii上的体感游戏, 那可以进行开发了.
# UE4集成
如果要进行开发, 首先要有Library. 开源的实现有很多, 参考[http://wiibrew.org/wiki/Wiimote/Library](http://wiibrew.org/wiki/Wiimote/Library)
由于带MotionPlus的型号比较新, 而这些Library都没有提供很好的支持, Dolphin中的控制器部分又没法直接分离出来, 所以我参考其他人的实现修改了一个WiiUse的版本: [https://github.com/xoyojank/wiiuse](https://github.com/xoyojank/wiiuse)
UE4中集成控制器是采用插件的形式, 实现IInputDevice, 发送按键和传感器信息即可 
![按键状态](https://img-blog.csdn.net/20150630114122589)
![发送按钮消息](https://img-blog.csdn.net/20150630114132178)
![发送传感器消息](https://img-blog.csdn.net/20150630114140036)
![功能开关](https://img-blog.csdn.net/20150630114316470)
# 应用
首先在插件管理器中启用Wiimote插件 
![这里写图片描述](https://img-blog.csdn.net/20150630114824432)
使用朝向信息控制飞行器角度 
![这里写图片描述](https://img-blog.csdn.net/20150630115059609)
按键响应同传统手柄一样 
![这里写图片描述](https://img-blog.csdn.net/20150630115412200)
# 参考资料
[http://wiibrew.org/wiki/Wiimote/Library](http://wiibrew.org/wiki/Wiimote/Library)
[https://github.com/rpavlik/wiiuse](https://github.com/rpavlik/wiiuse)
[http://www.dolphin-emulator.com/](http://www.dolphin-emulator.com/)
