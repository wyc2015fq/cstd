# canvas-实现刮刮乐效果 - weixin_33985507的博客 - CSDN博客
2017年03月23日 23:31:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
![4319236-94aff00b55391362.png](https://upload-images.jianshu.io/upload_images/4319236-94aff00b55391362.png)
QQ截图20170323232902.png
![4319236-94946874ec3edef9.png](https://upload-images.jianshu.io/upload_images/4319236-94946874ec3edef9.png)
QQ截图20170323232817.png
实现思路：
0.设置canvas的大小和背景图片一样大
1.获取canvas标签和上下文
2.添加蒙版
3.设置合成属性为context.globalCompositeOperation = 'destination-out';
4.监听鼠标（画圆）
5.清除鼠标移动和抬起
