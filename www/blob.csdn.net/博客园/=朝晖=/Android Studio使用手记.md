# Android Studio使用手记 - =朝晖= - 博客园
# [Android Studio使用手记](https://www.cnblogs.com/dhcn/p/7105623.html)
1、官方的Tip and Tricks：[http://developer.android.com/intl/zh-cn/sdk/installing/studio-tips.html](http://developer.android.com/intl/zh-cn/sdk/installing/studio-tips.html)
2、项目的目录树(Project Stucture)显示不正常解决方法：[http://stackoverflow.com/questions/16727505/android-studio-project-structure-not-coming-properly](http://stackoverflow.com/questions/16727505/android-studio-project-structure-not-coming-properly)  我用添加module的那个方法解决有效。
3、CSDN [唐韧_Ryan](http://my.csdn.net/tangren03)的使用教程：[http://blog.csdn.net/ryantang03/article/details/8941364](http://blog.csdn.net/ryantang03/article/details/8941364)
4、update的时候会报connection failed，据说是墙的关系，找到一有效解决办法：[http://blog.csdn.net/hil2000/article/details/11395485](http://blog.csdn.net/hil2000/article/details/11395485) 多说一句：0.3.1之后的版本好像不用这个方法可以连接成功。
5、editor编辑器字体修改，我的系统上[Android](http://lib.csdn.net/base/android) Studio默认的编辑器字体非常小，[android](http://lib.csdn.net/base/android)预先提供的编辑器界面主题(theme)不可修改，所以字体也修改不了，所以你需要在界面主题哪儿另存一个自己的主题，然后在这个自己的主题上就可以修改了。详见：[http://bbs.9ria.com/thread-200235-1-1.html](http://bbs.9ria.com/thread-200235-1-1.html)
6、先说错误提示，后面说问题原因和解决方法
```
Gradle: A problem occurred configuring project ':MyApp'.  
> Failed to notify project evaluation listener.  
   > Could not resolve all dependencies for configuration ':MyApp:_DebugCompile'.  
      > Could not find any version that matches com.android.support:appcompat-v7:+.  
        Required by:  
            MyAppProject:MyApp:unspecified
```
问题原因是某些依赖的SDK组件不存在，一般来讲，有可能是你应用兼容的最低版本编译有依赖问题，用SDK Manager下载对应的版本依赖即可。
8、Android Studio和eclipse之间的不同：[http://blog.csdn.net/u010966622/article/details/9181437](http://blog.csdn.net/u010966622/article/details/9181437)

