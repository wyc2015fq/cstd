# 在Android上实现图像颜色过滤与反转 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年12月31日 21:35:08[gloomyfish](https://me.csdn.net/jia20003)阅读数：13581








**在Android上实现图像颜色过滤与反转**

**一：原理**

Android已经提供了以resource形式加载图片，使用BitmapFactory的*decodeResource*

方法获取一个Bitmap对象，从Bitmap对象中读取图片的像素数组，基于像素ARGB值实现

颜色过滤和颜色反转。



颜色过滤（Color Filter）是值对RGB三种颜色通道，设定一个百分比数值，分别对三个颜

色通过提取百分比之后的数值。假设Red设置为0%就表示红色分量为0。



颜色反转(Color Invert)是指对原像素点的RGB三个值，分别减去255然后取绝对值，新的

RGB值将取代原来的值。



灰度过滤（Gray Filter）是指对一张RGB真彩色的图像转换为0 ～ 255灰度图像。



**二:程序关键点解析**

1.  如何在两个Intent之间传递值

在主Activity中使用如下代码启动Color Filter的Intent代码如下：



```java
Intent settingUIIntent = new Intent(this, SettingsUIActivity.class);
startActivityForResult(settingUIIntent, ImageConstants.REQUEST_RGB_ID);
```


在主Activity中重载onActivityResult()方法实现参数传递和结果解析。



Color Filter的UI画面如下：

![](http://hi.csdn.net/attachment/201112/31/0_1325337930aDi8.gif)


2.  Color Filter代码解析



```java
// reduce the RGB
r = (int)((float)r * redPrecent);
g = (int)((float)g * greenPrecent);
b = (int)((float)b * bluePrecent);

// color convert
if(isColorInvert) {
r = 255 - r;
g = 255 - g;
b = 255 - b;
}
 
// write the pixel data back
pixelData[index] = (a << 24) | (r<< 16) | (g << 8) | b;
```

3.  灰度图像

tr = tg= tb = (**int**)(0.299 * (**double**)tr + 0.587 * (**double**)tg +0.114 * (**double**)tb);



三：程序效果

基于Android 3.2版本开发与测试 – 主界面 +原图:

![](http://hi.csdn.net/attachment/201112/31/0_1325338524q0X1.gif)

红色分量为零时效果：

![](http://hi.csdn.net/attachment/201112/31/0_1325338543Zzks.gif)


Green分量为零时效果：

![](http://hi.csdn.net/attachment/201112/31/0_1325338580eC3V.gif)

 蓝色分量为零时效果：

![](http://hi.csdn.net/attachment/201112/31/0_1325338613W986.gif)

RGB颜色反转效果：

![](http://hi.csdn.net/attachment/201112/31/0_1325338627J20K.gif)

RGB灰度化以后效果：

![](http://hi.csdn.net/attachment/201112/31/0_13253386429E5E.gif)




