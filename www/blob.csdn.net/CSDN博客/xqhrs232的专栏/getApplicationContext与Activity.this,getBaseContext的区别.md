# getApplicationContext与Activity.this,getBaseContext的区别 - xqhrs232的专栏 - CSDN博客
2012年03月07日 15:14:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：791标签：[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://hi.baidu.com/%CF%B5%B4%C7%CF%C2%B4%CE/blog/item/cc0d4eedacf34022b90e2d55.html](http://hi.baidu.com/%CF%B5%B4%C7%CF%C2%B4%CE/blog/item/cc0d4eedacf34022b90e2d55.html)
相关网帖::[http://aijiawang-126-com.iteye.com/blog/982734](http://aijiawang-126-com.iteye.com/blog/982734)
**getApplicationContext()**返回应用的上下文，生命周期是整个应用，应用摧毁它才摧毁
**Activity.this**的context 返回当前activity的上下文，属于activity ，activity 摧毁他就摧毁
**getBaseContext() **返回由构造函数指定或setBaseContext()设置的上下文
转自：[http://blog.sina.com.cn/s/blog_5da93c8f0100t77t.html](http://blog.sina.com.cn/s/blog_5da93c8f0100t77t.html)
