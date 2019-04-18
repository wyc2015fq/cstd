# Android 获取imageview的图,在另一个imageview里显示。 - weixin_33985507的博客 - CSDN博客
2016年06月22日 12:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
当我点击默认头像里的其中一个然后在点确定就在最上面的那个imageview里显示选择的头像。求大神。
![](http://c.hiphotos.baidu.com/zhidao/wh%3D600%2C800/sign=61731edc58afa40f3c93c6db9b542f77/bd3eb13533fa828bb49d340dff1f4134960a5aa5.jpg)
```
img1和img2都是ImageView，要把img1中的图片显示到img2中
前景(对应src属性)
img2.setImageDrawable(img1.getDrawable());
背景(对应background属性)
img2.setBackgroundDrawable(img1.getBackground());
```
