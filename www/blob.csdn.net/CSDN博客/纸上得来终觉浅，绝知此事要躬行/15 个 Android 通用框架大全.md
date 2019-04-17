# 15 个 Android 通用框架大全 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年01月03日 09:52:00[boonya](https://me.csdn.net/boonya)阅读数：363








![](http://static.oschina.net/uploads/space/2016/0630/164213_tgqJ_2652078.png)

**1. 缓存**
- 
[DiskLruCache](https://github.com/JakeWharton/DiskLruCache)    Java实现基于LRU的磁盘缓存


**2.图片加载**
- 
[Android Universal Image Loader](https://github.com/nostra13/Android-Universal-Image-Loader) 一个强大的加载，缓存，展示图片的库

- 
[Picasso](https://github.com/square/picasso)一个强大的图片下载与缓存的库

- 
[Fresco](https://github.com/facebook/fresco)  一个用于管理图像和他们使用的内存的库

- 
[Glide](https://github.com/bumptech/glide)   一个图片加载和缓存的库


**3. 图片处理**
- 
[Picasso-transformations](https://github.com/wasabeef/picasso-transformations) 一个为Picasso提供多种图片变换的库

- 
[Glide-transformations](https://github.com/wasabeef/glide-transformations)   一个为Glide提供多种图片变换的库

- 
[Android-gpuimage](https://github.com/CyberAgent/android-gpuimage)    基于OpenGL的Android过滤器


**4. 网络请求**
- 
[Android Async HTTP](https://github.com/loopj/android-async-http) Android异步HTTP库

- 
[AndroidAsync](https://github.com/koush/AndroidAsync)    异步Socket，HTTP(客户端+服务器)，WebSocket，和socket.io库。基于NIO而不是线程。

- 
[OkHttp](https://github.com/square/okhttp) 一个Http与Http/2的客户端

- 
[Retrofit](https://github.com/square/retrofit)   类型安全的Http客户端

- 
[Volley](https://android.googlesource.com/platform/frameworks/volley)  Google推出的Android异步网络请求框架和图片加载框架


**5. 网络解析**
- 
[Gson](https://github.com/google/gson)    一个Java序列化/反序列化库，可以将JSON和java对象互相转换

- 
[Jackson](https://github.com/codehaus/jackson) Jackson可以轻松地将Java对象转换成json对象和xml文档，同样也可以将json、xml转换成Java对象

- 
[Fastjson](https://github.com/alibaba/fastjson)    Java上一个快速的JSON解析器/生成器

- 
[HtmlPaser](https://sourceforge.net/projects/htmlparser/)   一种用来解析单个独立html或嵌套html的方式

- 
[Jsoup](https://github.com/jhy/jsoup)   一个以最好的DOM，CSS和jQuery解析html的库


**6. 数据库**
- 
[OrmLite](https://sourceforge.net/projects/ormlite/files/releases/com/j256/ormlite/)JDBC和Android的轻量级ORM java包

- 
[Sugar](https://github.com/satyan/sugar)  用超级简单的方法处理Android数据库

- 
[GreenDAO](https://github.com/greenrobot/greenDAO)   一种轻快地将对象映射到SQLite数据库的ORM解决方案

- 
[ActiveAndroid](https://github.com/pardom/ActiveAndroid)   以活动记录方式为Android SQLite提供持久化

- 
[SQLBrite](https://github.com/square/sqlbrite)  SQLiteOpenHelper 和ContentResolver的轻量级包装

- 
[Realm](https://github.com/jhy/jsoup)   移动数据库：一个SQLite和ORM的替换品


**7. 依赖注入**
- 
[ButterKnife](https://github.com/JakeWharton/butterknife) 将Android视图和回调方法绑定到字段和方法上


- 
[Dagger2](https://github.com/google/dagger)一个Android和java快速依赖注射器。

- 
[AndroidAnotations](https://github.com/excilys/androidannotations)  快速安卓开发。易于维护

- 
[RoboGuice](https://github.com/roboguice/roboguice)   Android平台的Google Guice


**8. 图表**
- 
[WilliamChart](https://github.com/diogobernardino/WilliamChart)    创建图表的Android库

- 
[HelloCharts](https://github.com/lecho/hellocharts-android) 兼容到API8的Android图表库

- 
[MPAndroidChart](https://github.com/PhilJay/MPAndroidChart) 一个强大的Android图表视图/图形库


**9. 后台处理**
- 
[Tape](https://github.com/square/tape)    一个轻快的，事务性的，基于文件的FIFO的库

- 
[Android Priority Job Queue](https://github.com/yigit/android-priority-jobqueue) 一个专门为Android轻松调度任务的工作队列


**10. 事件总线**
- 
[EventBus](https://github.com/greenrobot/EventBus)    安卓优化的事件总线，简化了活动、片段、线程、服务等的通信


- 
[Otto](https://github.com/square/otto)    一个基于Guava的增强的事件总线


**11. 响应式编程**
- 
[RxJava](https://github.com/ReactiveX/RxJava) JVM上的响应式扩展

- 
[RxJavaJoins](https://github.com/ReactiveX/RxJavaJoins) 为RxJava提供Joins操作

- 
[RxAndroid](https://github.com/ReactiveX/RxAndroid)   Android上的响应式扩展，在RxJava基础上添加了Android线程调度

- 
[RxBinding](https://github.com/JakeWharton/RxBinding)   提供用RxJava绑定Android UI的API

- 
[Agera](https://github.com/google/agera)  Android上的响应式编程


**12. Log框架**
- 
[Logger](https://github.com/orhanobut/logger)  简单，漂亮，强大的Android日志工具


- 
[Hugo](https://github.com/JakeWharton/hugo)    在调试版本上注解的触发方法进行日志记录

- 
[Timber](https://github.com/JakeWharton/timber)  一个小的，可扩展的日志工具


**13. 测试框架**
- 
[Mockito](https://github.com/mockito/mockito)Java编写的Mocking单元测试框架


- 
[Robotium](https://github.com/RobotiumTech/robotium)    Android UI 测试

- 
[Robolectric](https://github.com/xtremelabs/robolectric)Android单元测试框架


Android自带很多测试工具：JUnit，Monkeyrunner，UiAutomator，Espresso等

**14. 调试框架**
- 
[Stetho](https://github.com/facebook/stetho)  调试Android应用的桥梁，使得可以利用Chrome开发者工具进行调试


**15. 性能优化**
- 
[LeakCanary](https://github.com/square/leakcanary) 内存泄漏检测工具

- 
[ACRA](https://github.com/ACRA/acra)  Android应用程序崩溃报告


出处：[Android开发中文站](http://www.androidchina.net/4920.html#rd?sukey=3903d1d3b699c208f96a43a12fc2e7ae683c66523214d79bc81eec0c330d577be318928c65eee6cc5dc28db3e117d1ef)


文章转载自 开源中国社区 [[http://www.oschina.net]](https://www.oschina.net/)

本文地址：[https://www.oschina.net/news/73836/15-android-general-popular-frameworks](https://www.oschina.net/news/73836/15-android-general-popular-frameworks)




