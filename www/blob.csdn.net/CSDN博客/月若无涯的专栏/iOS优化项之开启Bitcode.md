# iOS优化项之开启Bitcode - 月若无涯的专栏 - CSDN博客
2015年12月30日 16:05:05[月若无涯](https://me.csdn.net/u010124617)阅读数：2204
先贴上一段官方解释：
> 
Bitcode. When you archive for submission to the App Store, Xcode will compile your app into an intermediate representation. The App Store will then compile the bitcode down into the 64 or 32 bit executables as necessary.
简单说，以前是把所有的arm7、arm64平台的源码编译好，然后打成一个App；现在开启Bitcode之后，用户在AppStore下载到的就只有自己平台对应的那部分，自然是减少了很多不必要的浪费。
经过不断的联系各个SDK项目组，终于把项目中的所有第三方SDK都更新到支持Bitcode了，针对iPhone6 Plus设备的优化后效果如下： 
![本机测试效果](https://img-blog.csdn.net/20151230155508717)
可以看到效果还是很明显的。
开启该优化方式也是傻瓜式的，见下图 
![这里写图片描述](https://img-blog.csdn.net/20151230155807953)
然后就是编译检查各个SDK是否都支持，如果遇到不支持的，去寻找最新版本更新掉即可。
PS：针对部分使用了类似ShareSDK之类的分享组件的开发者，建议可以去各个平台官方下载对应的SDK，笔者也是如此才完成的。
