
# 如何使用SharedPreferences在Android的存储，读取和编辑值 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月19日 18:22:57[Jlins](https://me.csdn.net/dyllove98)阅读数：1149标签：[android																](https://so.csdn.net/so/search/s.do?q=android&t=blog)个人分类：[android																](https://blog.csdn.net/dyllove98/article/category/1130032)


我需要要存储一个时间值，需要检索和编辑该值。有人可以知道下吗？如何使用
SharedPreferences 完成

### 处理方法
关于android shared preferences,你可以再你的activity 中加入下面代码来完成
SharedPreferences prefs = this.getSharedPreferences( "com.example.app", Context.MODE_PRIVATE);
通过如下方式读取   preferences:
String dateTimeKey = "com.example.app.datetime";
// use a default value using new Date()
long l = prefs.getLong(dateTimeKey, new Date().getTime());
通过下面进行编辑和保存 preferences
Date dt = getSomeDate();
prefs.edit().putLong(dateTimeKey, dt.getTime()).apply();
Android SDK中的示例目录中包含检索和存储共享例子,位置如下
<android-sdk-home>/samples/android-<platformversion>/ApiDemos directory

原文地址：[http://www.itmmd.com/201410/35.html ](http://www.itmmd.com/201410/35.html)
该文章由[萌萌的IT人](http://www.itmmd.com/)整理发布,转载须标明出处。

