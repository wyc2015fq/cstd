# 使用Glide以及OkHttp集成 - =朝晖= - 博客园
# [使用Glide以及OkHttp集成](https://www.cnblogs.com/dhcn/p/7121421.html)
1、glide的使用：
添加依赖：
- compile 'com.github.bumptech.glide:glide:3.7.0'  
调用代码：
- ImageView imageView = (ImageView) findViewById(R.id.my_image_view);  
- 
- Glide.with(this).load("http://goo.gl/gEgYUd").into(imageView);  
2、添加OKHttp3集成：
- compile 'com.github.bumptech.glide:okhttp3-integration:1.4.0@aar'  
- compile 'com.squareup.okhttp3:okhttp:3.1.2'  
3、出现的问题
- You must not call setTag() on a view Glide is targeting   
解决方案：把seTag调用改为setTag(key,Object)调用：
- ImageView.setTag(R.id.ImagViewId,saveObject);  
参考：http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/kyleceshen/article/details/49806267
https://github.com/bumptech/glide

