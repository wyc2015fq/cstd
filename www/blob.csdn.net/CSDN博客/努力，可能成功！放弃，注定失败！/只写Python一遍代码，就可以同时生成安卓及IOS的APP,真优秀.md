# 只写Python一遍代码，就可以同时生成安卓及IOS的APP,真优秀 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年04月10日 11:23:26[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：102
**前言：**
**用Python写安卓APP肯定不是最好的选择，但是肯定是一个很偷懒的选择**
我们使用kivy开发安卓APP，Kivy是一套专门用于跨平台快速应用开发的开源框架，使用Python和Cython编写，对于多点触控有着非常良好的支持，不仅能让开发者快速完成简洁的交互原型设计，还支持代码重用和部署，绝对是一款颇让人惊艳的NUI框架。
因为跨平台的，所以只写一遍代码，就可以同时生成安卓及IOS的APP，很酷吧。
# **kivy安装**
- **环境说明：笔者在用的是Python2.7.10**
- **这里仅介绍windows平台安装**
- **所有平台参考: [https://kivy.org/#download](https://kivy.org/#download)**
**更新pip,setuptools**
```
python -m pip install --upgrade pip wheel setuptools
```
</pre>
然后是安装所需要的依赖
```
python -m pip install docutils pygmentspypiwin32 kivy.deps.sdl2 kivy.deps.glew \
kivy.deps.gstreamer --extra-index-url https://kivy.org/downloads/packages/simple/
```
</pre>
值得注意的是，上面的安卓需要访问Google，所以请自备梯子，而且kivy.deps.gstreamer这个包比较大(95MB)，可以单独本地安装，[http://pan.baidu.com/s/1o7mlxNk](http://pan.baidu.com/s/1o7mlxNk)
然后就是安装kivy了
```
python -m pip install kivy
```
</pre>
至此，安装就已经完毕了，值得注意的是64位系统没有开启虚拟化支持，在导入kivy的时候会报错，如果是64位系统就设置一下机器的BIOS，开启虚拟化支持吧。
**注：这里只是kivy的运行环境，这样我就能直接在windows机器上直接调试了，怎么将代码编译成APK文件我们会在后面讲到。**
如果kivy 在python 中应该就能导入了。
![](https://upload-images.jianshu.io/upload_images/14581851-a72367f7aec6ee43?imageMogr2/auto-orient/strip%7CimageView2/2/w/867/format/webp)
image
按照“世界惯例”咱们来“hello”一下吧
**新建一个py文件**
```
from kivy.app import App
from kivy.uix.button importButton
class TestApp(App):
 def build(self):
 return Button(text='Hello,kivy')
TestApp().run()
```
![](https://upload-images.jianshu.io/upload_images/14581851-e7676242577e4878?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
image
然后会弹出一个框，大概如下，点击”hello,kivy” 会变颜色
![](https://upload-images.jianshu.io/upload_images/14581851-6625411efec7b130?imageMogr2/auto-orient/strip%7CimageView2/2/w/474/format/webp)
image
点击窗口并按“F1 ”会这个窗口的一些属性
![](https://upload-images.jianshu.io/upload_images/14581851-fb26d0a22019ef92?imageMogr2/auto-orient/strip%7CimageView2/2/w/561/format/webp)
image
然后我们回过头看一看代码。
```
##导入App，然后让TestApp这个类继承
from kivy.app import App
##导入一个Button,运维有这个button，当你点击的时候才会有所反应
from kivy.uix.button importButton
###定义类，名字必须是xxxApp
class TestApp(App):
 ###build一个Button
 def build(self):
 ###返回一个Button,文字内容是“Hello,kivy”
 return Button(text='Hello,kivy')
##运行,因为继承了App，所以才有的run这个方法
TestApp().run()
```
在windows上运行当然没有什么太大的意义，怎么在安卓手机上运行才是我们想要的，这时我们需要一个编译环境。
# **官方说明的环境，如下：**
You’ll need:
- A linux computer or a virtual machine
- Java
- Python 2.7 (not 2.6.)
- Jinja2 (python module)
- Apache ant
- Android SDK
虽然官方提供了一个似乎还不错的虚拟机镜像，但是还是有很多内容需要翻出去，所以笔者在这里提供相对而言更加完善的镜像
root密码：kivy
默认使用账户kivy，密码：kivy123
当然你也可以下载官方镜像，因为第一次编译需要去国外下一大堆东西，所以请自备梯子。
在笔者提供的镜像里，桌面上有一个dev_and，只要将上面写的代码，放入这个文件夹即可(当然也可以在其他目录，后面会讲到)。
```
cd Desktop/dev_and/
##初始化会在当前目录生成一个buildozer.spec文件 用于配置生成的apk相关信息
buildozer init
###修改buildozer.spec文件
vi buildozer.spec
```
**至少修改下面三项**
```
# (str) Title of your application
title = helloworld
# (str) Package name
package.name = helloapp
# (str) Package domain (needed for android/ios packaging)
package.domain = youer.com
```
然后注释
```
# (str) Application versioning (method 1)
#version.regex = __version__ = ['"](.*)['"]
#version.filename = %(source.dir)s/main.py
```
下面这行改为非注释
```
version = 1.2.0
```
最后我们生成我们需要的apk文件
```
buildozer -v android debug
```
buildozer命令会在当前文件夹创建一个bin，该文件夹里面有我们想要的apk文件
```
helloapp-1.2.0-debug.apk
```
**效果图**
![](https://upload-images.jianshu.io/upload_images/14581851-19aea25b0a912be9?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
image
主要由三部分组成，一是素材，图片音频之类的文件，二是Python代码，三是kv文件，这个kv文件有点像html中的css。
Python代码的文件名一般命名为main.py
然后一定有一个叫做XXXApp的类，并继承App。
比如该类叫做GameApp，那么该目录下的kv文件则必须为Game，如上图所示，如果不是，那么kv文件中的一些设定就不会生效。
比如设定一个标签
```
Label:
 id: time
 text: 'xxxx'
 font_size: 60
```
id为time，text文本内容为'xxxx',然后字体为60
作者：妄心xyx
链接：https://www.jianshu.com/p/2d4e92826a2e
来源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
