# VR开发--Cardboard项目三:通过外置设备控制视野移动 - weixin_33985507的博客 - CSDN博客
2016年12月29日 12:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
![642887-ecf7f1cda5dae982.gif](https://upload-images.jianshu.io/upload_images/642887-ecf7f1cda5dae982.gif)
效果图
前面讲解的项目,已经能够让我们观看虚拟世界,并进行一些简单交互
[http://www.jianshu.com/p/643890e4c57d](https://www.jianshu.com/p/643890e4c57d)
[http://www.jianshu.com/p/3696bc837551](https://www.jianshu.com/p/3696bc837551)
但是我们发现自己并不能移动.只能待在一个固定的角落,,这简直不能忍受!
那么我们就简单来做一下:
##### 1.导入Unity资源包
![642887-8e21376b043efdaa.png](https://upload-images.jianshu.io/upload_images/642887-8e21376b043efdaa.png)
可加群获取:134688909
这个里面包含了第一人称与第三人称的脚本,通过他们我们可以快速搞定第一人称视野.
##### 2.制作第一人称视野
![642887-9a7f9a7f6005773b.png](https://upload-images.jianshu.io/upload_images/642887-9a7f9a7f6005773b.png)
Paste_Image.png
![642887-920c50a1994ce283.png](https://upload-images.jianshu.io/upload_images/642887-920c50a1994ce283.png)
调整摄像机位置
这就完成第一人称控制了.
##### 3.第三人称控制
可以使用资源包自带的,也可以使用我们写的相机跟随代码
```
```
##### 4.蓝牙手柄控制
拿到映射的轴,对应蓝牙的按键
这里以第一人称来进行修改
首先了解:
###### 4-1第一人称的视线是跟随鼠标移动，现在要使其跟随cardboard跟踪到的头部姿态的运动而移动；
###### 4-2第一人称的前后左右移动是由键盘上的控键控制，现在要由蓝牙手柄上的按键来控制；
4-1:如何做?
打开FirstPersonController.cs文件
![642887-d52f22ff828faf2c.png](https://upload-images.jianshu.io/upload_images/642887-d52f22ff828faf2c.png)
注释掉它
注释掉后,发现视野就不能通过鼠标旋转了.
紧接着,修改它
![642887-2aec8d4f379fbd40.png](https://upload-images.jianshu.io/upload_images/642887-2aec8d4f379fbd40.png)
Paste_Image.png
![642887-031a39948583fbe6.png](https://upload-images.jianshu.io/upload_images/642887-031a39948583fbe6.png)
Paste_Image.png
4-2:使用蓝牙的按键来控制第一人称的前后左右移动
其实Unity中已经为我们写好了控制一些按钮的事件.只需要测试一下就可以(Ps:因为每个蓝牙手柄的触发模式不同,所以要测试一下.我表示见过奇葩的.)具体的就写个Demo搞搞.....如果不会写请入群联系我...
将其中的值替换成你的蓝牙按键就可以了.
![642887-48f30448b8388e86.png](https://upload-images.jianshu.io/upload_images/642887-48f30448b8388e86.png)
Paste_Image.png
![642887-f501e0a0427ecbdb.png](https://upload-images.jianshu.io/upload_images/642887-f501e0a0427ecbdb.png)
Paste_Image.png
至此,修改完毕
