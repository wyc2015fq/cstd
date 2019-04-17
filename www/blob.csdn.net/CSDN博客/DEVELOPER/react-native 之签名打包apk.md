# react-native 之签名打包apk - DEVELOPER - CSDN博客





2018年06月13日 23:30:08[学术袁](https://me.csdn.net/u012827205)阅读数：914
所属专栏：[react-native](https://blog.csdn.net/column/details/23827.html)









`react native`很好用，项目完成需要打包上线哦！好吧 

那~就签名打包上线喽… 
**当前的`react-native`项目代码基于`"react": "~15.4.0-rc.4","react-native": "0.41.0",`**

打开我的挚爱AS,你会发现build过程很慢，而且耐心等待其build完成之后，还是错误的**….**
**因为在`react-native-0.41.0`版本下>>>**

 - android/build.gradle中`'classpath 'com.android.tools.build:gradle:1.3.1'`

 - android/gradle/wrapper/gradle-wrapper.properties中`'distributionUrl=.../gradle-2.4-all.zip'`
而我们的AS基本上是很高的版本，并且就是依照这种低版本进行build，结果也许不会令你满意~ 

所以，干脆！就升级版本吧，下面是我升级过的版本，且编译运行都通过。无论是在AS或者WebStorm上面 
![这里写图片描述](https://img-blog.csdn.net/2018061409562464?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180614095647719?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在app下的gradle文件中`compileSdkVersion`、`buildToolsVersion`、`minSdkVersion`和`targetSdkVersion`依然保持原来的版本。通过build之后，一切回归于正常。
## 如何生成可发布的apk？

### 1-生成Android签名证书

打包apk，我们必须是要使用签名证书的，该签名证书我们可以从AS上获得，从新生成一个新的`xxx.jks`
![这里写图片描述](https://img-blog.csdn.net/20180614100654965?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

拿到签名证书之后，拷贝到`android/app目录`下即可。 
![这里写图片描述](https://img-blog.csdn.net/20180614101148679?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 2-设置gradle变量

由上图看，设置gradle变量，即编辑android/gradle.properties文件内容。添加下面代码

```
#android端进行apk打包使用
MYAPP_RELEASE_STORE_FILE=lushan2017.jks ##签名证书全名
MYAPP_RELEASE_KEY_ALIAS=lushan2017 ##签名证书全名别名
MYAPP_RELEASE_STORE_PASSWORD=lushan2017
MYAPP_RELEASE_KEY_PASSWORD=lushan2017
```

然后覆盖在`android/app/build.gradle文件`中两个内容

```
...
signingConfigs {
    release {
       storeFile file(MYAPP_RELEASE_STORE_FILE)
       storePassword MYAPP_RELEASE_STORE_PASSWORD
       keyAlias MYAPP_RELEASE_KEY_ALIAS
       keyPassword MYAPP_RELEASE_KEY_PASSWORD
    }
}
buildTypes {
   release {
      minifyEnabled enableProguardInReleaseBuilds
      proguardFiles getDefaultProguardFile("proguard-android.txt"), "proguard-rules.pro"
      signingConfig signingConfigs.release
   }
}
...
```

### 3-打包apk

在AS中点击执行`Build/Generate Signed Apk...`编译然后运行到手机上，你会发现….打开即崩溃。。不能正常运行在手机上！！为什么呢，奇诡？ 

既然`apk`在`AS`下编译的不能运行在手机上，那么就到`WebStorm`上编译出一个apk比较下。在`WebStorm Terminal终端`执行命令`gradlew assembleRelease`
![这里写图片描述](https://img-blog.csdn.net/20180614091750945?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

结果爆出问题`'gradlew'不是内部或外部命令，也不是可运行的程序或批处理文件。`，说明`gradle命令`在这里无法执行，也就是说明在本机中没有配置`gradle`的环境变量。此时就需要到`高级系统设置/环境变量/系统变量`进行配置。
![这里写图片描述](https://img-blog.csdn.net/20180614091758235?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

新建`系统变量` <下面是我个人的> 
`变量名(N)=GRADLE_HOME`； 
`变量值(V)=C:\Users\YJH\.gradle\wrapper\dists\gradle-3.3-all\55gk2rcmfc6p2dg9u9ohc3hw9\gradle-3.3`

至于变量值内容，你可以打开你的AS边拿到变量值。然后添加到系统变量的变量`Path`中，以方式`%GRADLE_HOME%\bin`加入即可。 
**Webstorm中再次执行编译打包apk命令**
`注意：切换到你的android目录下执行签名打包命令`
![这里写图片描述](https://img-blog.csdn.net/20180614091806624?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018061409181338?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

之后，就能看到编译成功的提示。然后在`webstorm`中路径`android/app/outputs/apk/app-release.apk`就能看到已成功签名打包的文件。 
![这里写图片描述](https://img-blog.csdn.net/20180614091819951?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`安装到手机运行成功。但是为什么在AS打包的apk文件却不能运行呢？`为了证明点什么，需要分别比较一下她们各自生成的apk 

进入AS打包签名的apk 
![这里写图片描述](https://img-blog.csdn.net/20180614091826615?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

进入Webstorm打包签名的apk 
![这里写图片描述](https://img-blog.csdn.net/20180614091833498?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

第一反应看到的是，apk包大小竟然不一样。分别进入apk包中分析包内有什么不同 
![这里写图片描述](https://img-blog.csdn.net/20180614091839311?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180614091846522?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过深入剖析，可以看到使用`AS进行签名打包的apk`中比使用`Webstorm进行签名打包的apk`少了一个`文件夹assets`。而在assets文件夹中是`bundle文件`。该bundle文件就是我们所编写的打包的js文件。 

在开发环境下，每次启动APP，都会连接`JS Server`将项目中编写的js文件代码加载到APP(React Native的动态更新)。 

签名打包后的`apk`变成了`生产环境`，无法在启动的时候连接JS Server加载相应的js文件。`导致APP因缺少相应的js而无法启动`。








