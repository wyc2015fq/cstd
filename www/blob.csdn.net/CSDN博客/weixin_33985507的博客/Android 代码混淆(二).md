# Android 代码混淆(二) - weixin_33985507的博客 - CSDN博客
2016年08月22日 20:56:05[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
文章转自我[个人博客](http://www.dove.farbox.com)
Android 代码混淆(一) 中已经记录并走了混淆的整个流程，用命令行进行混淆的操作，并验证了三个过程，这篇文章会记录一下在 `Android Studio` 下混淆的操作，以及具体需要的注意的一些事项。
#### 基本操作及整体的流程
1.修改`build.gradle`脚本
```
buildTypes {
            release {
                minifyEnabled true//开启混淆
                proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'//配置内容
            }
            debug {
                minifyEnabled true//开启混淆
                proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'//配置内容
            }
        }
```
2.修改proguard-rules.pro文件，写入基础的几个混淆规则，避开`Android`自带的一些类
```
# 四大组件及基本类
    -keep public class * extends android.app.Activity
    -keep public class * extends android.app.Application
    -keep public class * extends android.app.Service
    -keep public class * extends android.content.BroadcastReceiver
    -keep public class * extends android.content.ContentProvider
    -keep public class * extends android.app.backup.BackupAgentHelper
    -keep public class * extends android.preference.Preference
    -keep public class com.android.vending.licensing.ILicensingService
    -keep public class * extends android.app.Fragment
    -keep public class * extends android.support.v4.**
    -keep public class * extends android.support.annotation.**
    -keep public class * extends android.support.v7.**
    -keep public class android.app.Notification
    -keep public class android.webkit.**
    #保护WebView对HTML页面的API不被混淆
    -keep class **.Webview2JsInterface {*; }
    -keep public class * extends android.app.Dialog
    -keep public class * extends android.view
    
    # 所有枚举类型不要混淆
    -keepclassmembers enum * {
    public static **[] values();
    public static ** valueOf(java.lang.String);
    }
    
    # 保持 native 方法不被混淆
    -keepclasseswithmembernames class * {
    native <methods>;
    }
    #保持R文件不被混淆，否则，你的反射是获取不到资源id的
    -keep class **.R*{*;}
    # parcelable 不被混淆
    -keep class * implements android.os.Parcelable {
    public static finalandroid.os.ParcelableCreator *;
    }
    
    #保持实现"Serializable"接口的类不被混淆
    -keepnames class * implements java.io.Serializable
    
    #保护实现接口Serializable的类中，指定规则的类成员不被混淆
    -keepclassmembers class * implements java.io.Serializable {
    static final long serialVersionUID;
    private static final java.io.ObjectStreamField[] serialPersistentFields;
    !static !transient <fields>;
    !private <methods>;
    !private <methods>;
    private void writeObject(java.io.ObjectOutputStream);
    private void readObject(java.io.ObjectInputStream);
    java.lang.Object writeReplace();
    java.lang.Object readResolve();
    }
```
3.直接跑一个debug版的进行测试，把打包好的APK修改成.zip文件，解压，取出文件中dex包。
![Paste_Image.png](http://upload-images.jianshu.io/upload_images/227965-b39ae2fc2afec7b2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
￼
把解压获取的`dex`文件，通过 [dex2jar工具](https://github.com/pxb1988/dex2jar) (一个把`dex`包转换成`jar`包的工具)，把`dex`转换成`jar`包。其具体使用不在此详细讲。
下图中我没有把d2j-dex2jar配置进环境变量，所以进入对应目录跑的脚本。
￼
![Paste_Image.png](http://upload-images.jianshu.io/upload_images/227965-ac1300a272f2eb1e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
运行完后可以看到，当前目录下多了一个`classes-dex2jar.jar`的文件，该文件就是decode出来的`jar`包，注意名字可能不一样
￼
![Paste_Image.png](http://upload-images.jianshu.io/upload_images/227965-9ac23b215fec90ef.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
接下来就是和前一篇文章一样，直接用`Intelij`导入，看class文件，包内类名字已有变化，部分类已被移除，同时部分类也被改成final类型。具体就不上图了。主要看下面的注意事项。
#### 注意事项
上述流程在如果是项目一开始就进行混淆，大部分情况下是能够成功混淆，并且不会出现很大问题，只需要在项目进行过程中注意，新加的第三方类库，反射代码。但是，如果你面对的是一个沉积已久，并且项目庞大，而又从未写过混淆的项目，那你可能会在混淆开启时，面临几千个混淆时的`warning`和`note`，最终无法编译通过。这中间主要是大量第三方库的报错。下面记录一下我在处理这种情况时，遇到的问题以及解决方案
##### warning 处理
写了个[脚本](https://github.com/flyer88/Some_Python_For_Android)，过滤出大部分错误，生成proguard文件，然后继续处理剩下的个别遗留问题，脚本写的比较菜，轻喷。
写几个典型的例子：
1.情况一:
```
Note: xxxxxx calls 'Class.getEnclosingMethod'
    #这种情况就是调用了反射，找到xxxxx对应调用的类，并且设为入口点
```
2.情况二:
```
Warning: AAAAAAA: can't find superclass or interface BBBBBBB
    Warning: AAAAAAA: can't find referenced class BBBBBBB 
    #这两种种情况就是找不到BBBBBB了，直接把BBBBB设为入口点，同时给AAAAA打上-dontwarn既可以,如下
    -keep class BBBBBB
    -dontwarn AAAAAA
```
3.情况三:
```
Note:AAAAA accesses a declared field BBBBB dynamically 
        Maybe this is program field 'CCCCC'
        Maybe .....
    #这种情况下，需要处理CCCC,把它设为入口点，同样设置对AAAAA设置-dontwarn
    -keep class CCCCC
    #不一定是-keep，也有可能其他的，例如 -keepattribute 更为合适
    -dontwarn AAAAA
```
##### 多个 module 的混淆
有时候会碰到多个`module`混淆的情况,多数情况下，为了清晰处理会给每个`module`都写上对应的混淆规则，同时需要修改`build.gradle`的配置，而不是之前的写法，具体可以参考[so上的解释](http://stackoverflow.com/questions/30820915/android-studio-proguard-handling-in-multi-library-projects)
这种方案，需要`module`之间的依赖清晰，最底层的`module`会被最先混淆，然后一步一步倒推上去，直到主`module`,多为`app module`。
```
buildTypes {
        release {
            consumerProguardFiles 'proguard-project.txt'
        }
    }
```
然而，我碰到的情况则是，多个`module`中有相同的包名，这时候视图去每个`module`自顾自混淆的情况下是不可能。因为同包名的情况下，混淆器是无法一个一个`module`的进行混淆。
所以最终的解决方案是，面对这种项目，还是在主`module`中进行混淆吧。
##### 反射的处理
反射举个例子：
`    Class<?> a = Class.forName("com.dove.xu.a");``    -keep class com.dove.xu.a{*;}    `
不过，此处在考虑到自己代码的同时，需要注意第三方类库。类似的`json`处理库，`retrofit`等都是有反射代码的。
##### 常用系统和第三方库的混淆规则
下面这个github库收藏了大量第三方库的混淆规则，可以去看一下
[snippets](https://github.com/krschultz/android-proguard-snippets)
基本的系统混淆规则，在一开始则整体流程中也已记录，就不重复了。
最后，需要注意的是网上也会有大量现成的第三方类库的混淆规则。但是在抄的时候也需要注意，不同的版本混淆规则不一定相同，所以一定要注意，在拷贝完以后，看一下规则，是否符合自己的版本，包名是否正确。
举个自己碰到的例子：
[Butterknife 8.2.1 混淆规则,摘自官方github](https://github.com/JakeWharton/butterknife/blob/master/butterknife/proguard-rules.txt)
```
# Retain generated class which implement ViewBinder.
    -keep public class * implements butterknife.internal.ViewBinder { public <init>(); }
    
    # Prevent obfuscation of types which use ButterKnife annotations since the simple name
    # is used to reflectively look up the generated ViewBinder.
    -keep class butterknife.*
    -keepclasseswithmembernames class * { @butterknife.* <methods>; }
    -keepclasseswithmembernames class * { @butterknife.* <fields>; }
```
[Butterknife 5.2.1 混淆规则，摘自官方github](https://github.com/chalup/proguard-please-repo/blob/master/com.jakewharton/butterknife/5.1.2/proguard.cfg)
```
-dontwarn butterknife.internal.**
    -keep class **$$ViewInjector { *; }
    -keepnames class * { @butterknife.InjectView *;}
```
以上就是我在Android混淆时，学到的知识以及碰到的问题。特此记录
