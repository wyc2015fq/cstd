# WPF 自定义控件的坑（蠢的：自定义控件内容不显示） - weixin_33985507的博客 - CSDN博客
2018年11月11日 13:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：22
原文:[WPF 自定义控件的坑（蠢的：自定义控件内容不显示）](https://blog.csdn.net/koloumi/article/details/73915903)
## 自定义控件不显示内容
```
由于工作需要在写WPF,其中想要实现一些自己的控件所以直接自定义了控件博主是继承了ContenControl的控件开始写的但是发现不管设置Content属性为任何都是不显示。
解决方案：
```
![这里写图片描述](https://img-blog.csdn.net/20170629155033410?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva29sb3VtaQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
在创建自定义控件的时候会自动生成一个布局文件会自动绑定到我们创建的自定义控件上这个时候默认使用的布局就是这个。
```
![这里写图片描述](https://img-blog.csdn.net/20170629155207514?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva29sb3VtaQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
比较简单的解决办法就是设置BaseOn属性直接继承父控件的样式即可使用。
```
