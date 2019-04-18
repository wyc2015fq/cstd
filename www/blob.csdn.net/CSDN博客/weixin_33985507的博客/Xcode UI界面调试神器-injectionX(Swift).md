# Xcode UI界面调试神器-injectionX(Swift) - weixin_33985507的博客 - CSDN博客
2018年09月30日 11:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：16
> 
最近用Swift写代码的时候，使用InjectionIII愣是成功不了，很气！！！
##### [InjectionIII的使用方式](https://www.jianshu.com/p/aee437dc0461)
刚发现InjectionIII的时候只使用OC进行了测试，并没有使用Swift，所以只知道怎么做，但是会不会成功也就没有关注了。今天Google了一下，看了一眼[InjectionIII的GitHub](https://github.com/johnno1962/InjectionIII)上面的有Xcode10的解决版本。如下  [InjectionX下载链接](http://johnholdsworth.com/InjectionX.app.zip)
Xcode10(Swift)使用的时候报错
```
Error loading /Applications/InjectionIII.app/Contents/Resources/iOSInjection.bundle/iOSInjection:  
dlopen(/Applications/InjectionIII.app/Contents/Resources/iOSInjection.bundle/iOSInjection, 265): 
Symbol not found: __T0BOWV
  Referenced from: /Applications/InjectionIII.app/Contents/Resources/iOSInjection.bundle/iOSInjection
  Expected in: /Users/leo.yqjr/Library/Developer/CoreSimulator/Devices/
4921C265-7854-4C9D-B407-69EC5C09B74E/data/Containers/Bundle/Application/4763B53B-CF31-4A95-8F02-CE1707A6478C/
mfzb.app/Frameworks/libswiftCore.dylib
 in /Applications/InjectionIII.app/Contents/Resources/iOSInjection.bundle/iOSInjection
```
代码(`InjectionIII.app换成InjectionX.app`)
```
#if DEBUG
// 把InjectionIII.app换成InjectionX.app
Bundle(path: "/Applications/InjectionIII.app/Contents/Resources/iOSInjection.bundle")?.load()
#endif
```
![293993-32b95e3f6b92eab0.png](https://upload-images.jianshu.io/upload_images/293993-32b95e3f6b92eab0.png)
Xcode10
换了个App，成功了
![293993-0c9df03aaa019170.png](https://upload-images.jianshu.io/upload_images/293993-0c9df03aaa019170.png)
成功
![293993-1638d027f5220c09.png](https://upload-images.jianshu.io/upload_images/293993-1638d027f5220c09.png)
两个App，打开InjectionX
