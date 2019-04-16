# Android开发学习总结(五)——Android应用目录结构分析 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Android开发学习总结(五)——Android应用目录结构分析](https://www.cnblogs.com/xdp-gacl/p/4338352.html)



## 一、手动创建android项目

　　手动创建一个Android项目，命名为HelloWorld，命令如下：

```
android create project -n HelloWorld -t 1 -p E:/Android/workspace/Helloworld  -k me.gacl.helloworld -a Helloworld
```

　　上面的命令中，-n指定项目的名称，-t指定android平台，-p指定项目存放路径，-k指定项目包名，-a指定Activity名称。

　　使用命令 android list targets 可查看可选的android平台，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/142038300584674.png)

　　由于我使用SDK Manager只下载了最新版本的Android5.0.1和Google Inc，因此"android list targets"命令查看到的可选的android平台就只有2个，可以看到，1对应的target是android-21，也就是Android 5.0.1。

　　运行上面的命令可以在E:/Android/workspace/Helloworld目录下创建一个Android项目，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/142048181522940.png)

　　生成的Android项目的目录结构如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/142053539641032.png)

## 二、Android目录结构说明

　　HelloWorld
　　　　|----bin
　　　　|----libs
　　　　|----res
　　　　|     |----drawable-ldpi
　　　　|     |----drawable-mdpi
　　　　|     |----drawable-hdpi
　　　　|     |----drawable-xhdpi
　　　　|     |----layout
　　　　|　  |　　 　　|----main.xml
　　　　|     |----values
　　　　|　　　　 　　|----strings.xml

　　　　|----src
　　　　|     |----me
　　　　|           |----gacl
　　　　|                  |----helloworld
　　　　|                          |----Helloworld.java
　　　　|----AndroidManifest.xml
　　　　|----ant.properties
　　　　|----build.xml
　　　　|----local.properties
　　　　|----proguard-project.txt
　　　　|----project.properties

　　下面介绍一下Android项目的主要目录和文件：

　　【bin】目录：该目录用于存放生成的目标文件，例如Java的二进制文件、资源打包文件(.ap_后缀)、Dalvik虚拟机的可执行性文件(.dex后缀)，打包好应用文件(.apk后缀)等。

　　【libs】目录：该目录用于存放需要使用的第三方jar包文件

　　【res】目录：该目录用于存放Android项目的各种资源文件，比如【layout】目录存放界面布局文件，例如main.xml，【values】目录下则存放各种XML格式的资源文件， 例如字符串资源文件：strings.xml；颜色资源文件：colors.xml； 尺寸资源文件：dimens.xml。 【drawable-ldpi】、【drawable-mdpi】、【drawable-hdpi】、【drawable-xhdpi】这4个子目录分别用于存放"低分辨率"、"中分辨率"、"高分辨率"、"超高分辨率"的4种图片文件。

　　【src】目录：该目录用于存放java源文件 

　　【AndroidManifest.xml】文件是Android项目的系统清单文件，它用于控制Android应用的名称、图标、访问权限等整体属性。 除此之外Android应用的Activity、Service、ContentProvider、BroadcastRecevier这4大组件都需要在该文件中配置。









