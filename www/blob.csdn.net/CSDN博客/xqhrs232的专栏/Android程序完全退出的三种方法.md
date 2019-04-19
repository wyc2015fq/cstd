# Android程序完全退出的三种方法 - xqhrs232的专栏 - CSDN博客
2011年01月08日 09:25:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：623标签：[android																[api																[service																[任务																[java																[c#](https://so.csdn.net/so/search/s.do?q=c#&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.android123.com.cn/androidkaifa/519.html](http://www.android123.com.cn/androidkaifa/519.html)
很多网友可能发现自己的Android程序有很多Activity，比如说主窗口A，调用了子窗口B，在B中如何关闭整个Android应用程序呢? 这里Android123给大家三种比较简单的方法实现。
  首先要说明在B中直接使用finish()，接下来手机显示的还是主窗口A，所以一起来看看Android开发网是如何实现的吧.
  1. Dalvik VM的本地方法
  android.os.Process.killProcess(android.os.Process.myPid())    //获取PID，目前获取自己的也只有该API，否则从/proc中自己的枚举其他进程吧，不过要说明的是，结束其他进程不一定有权限，不然就乱套了。
  System.exit(0);   //常规java、c#的标准退出法，返回值为0代表正常退出
 2. 任务管理器方法
   首先要说明该方法运行在Android 1.5 API Level为3以上才可以，同时需要权限android.permission.RESTART_PACKAGES，我们直接结束自己的package即可，直接使用ActivityManager类的restartPackage方法即可，参数为package name，该类通过getSystemService(Context.ACTIVITY_SERVICE)来实例化ActivityManager对象，这种方法系统提供的，但需要显示声明权限，所以使用中需要综合考虑。
 3. 根据Activity的声明周期
   我们知道Android的窗口类提供了历史栈，我们可以通过stack的原理来巧妙的实现，这里我们在A窗口打开B窗口时在Intent中直接加入标志Intent.FLAG_ACTIVITY_CLEAR_TOP，这样开启B时将会清除该进程空间的所有Activity。
 在A窗口中使用下面的代码调用B窗口
Intent intent = new Intent(); 
intent.setClass(Android123.this, CWJ.class);
intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);  //注意本行的FLAG设置
startActivity(intent);
 接下来在B窗口中需要退出时直接使用finish方法即可全部退出。  如果您有更好的建议可以联系 [android123@163.com](mailto:android123@163.com) 我们及时补充。
