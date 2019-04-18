# 2017-12-30  官方文档的Hello Extensions学习 - weixin_33985507的博客 - CSDN博客
2017年12月30日 23:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
一个最简单的Extension里边，有三样东西：
- manifest.json
- .png（一张有特定要求的图片）
- .html (一个html文件）
官方给的第一个例子Hello Extension里，给出了以上三样东西。
- 下面是manifest.json的内容：
![719742-c467906c87741298.png](https://upload-images.jianshu.io/upload_images/719742-c467906c87741298.png)
插件导入浏览器后与manifest.json内容的比较
- 下面是点击图标后的情形，也可以通过快捷键Ctrl+Shift+F得到相同的效果。（官方文档的图片链接失效了，我自己在网上随便找了个图标，256*256的png图片）：
![719742-0ed388af10e428fe.png](https://upload-images.jianshu.io/upload_images/719742-0ed388af10e428fe.png)
点击图标后出现一个小弹窗
弹出的小弹窗通过html文件实现，在这个Hello Extension里是hello.html，内容是显示一个一级标题的“Hello Extensions”文本。
