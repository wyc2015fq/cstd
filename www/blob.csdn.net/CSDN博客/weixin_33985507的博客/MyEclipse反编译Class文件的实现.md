# MyEclipse反编译Class文件的实现 - weixin_33985507的博客 - CSDN博客
2013年06月08日 20:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
对于需要查看Java Class文件源码的筒子们来说，必须在项目中导入Java源码才能查看Class文件的具体实现，这不仅十分的麻烦，因为有时我们并不可以获得Class文件对应的Java源码。今天就给大家介绍一款反编译Class文件的工具，有了它，可以查看任何Class文件的源码。
## 1.下载MyEclipse反编译Class文件的工具
工具的下载地址：[http://download.csdn.net/detail/yulei_qq/5543635](http://download.csdn.net/detail/yulei_qq/5543635)
## 2.工具的安装
下载后上面的工具之后，解压出来，我们可以看到有两个文件:**jad.exe、net.sf.jadclipse_3.3.0.jar ** 。
**第一步**：我们将jad.exe文件放到JDK的bin目录下，具体的可以查看下面的图片：
![](https://img-blog.csdn.net/20130608103301515)
**第二步**：将 net.sf.jadclipse_3.3.0.jar 拷贝到 MyEclipse安装目录下的plugins目录下面(接下来的步骤我会以我的计算机路径为例子说明)路径在：D:\Users\Administrator\AppData\Local\Genuitec\Common\plugins.具体文件位置见下图：
![](https://img-blog.csdn.net/20130608104122984)
**第三步**：在D:\Users\Administrator\AppData\Local\Genuitec\MyEclipse-8.6\dropins创建一个eclipse文件夹.
![](https://img-blog.csdn.net/20130608104701234)
**第四步**：在创建好的eclipse文件夹下，分别创建featues、plugins两个文件夹.
![](https://img-blog.csdn.net/20130608105137078)
**第六步：**将net.sf.jadclipse_3.3.0.jar 文件分别拷贝到创建好的featues、plugins两个文件夹里面.
![](https://img-blog.csdn.net/20130608105537375)
![](https://img-blog.csdn.net/20130608105733703)
**第七步**：重新启动MyEclipse，在Windows——>Perference——>Java下面应该会多出一个JadClipse目录. 如果没有出现的话，多试几次就OK了，我当时也是这样.
![](https://img-blog.csdn.net/20130608110512078)
**第八步**：点击JadClipse，在右边的窗口中配置JadClipse.  设置path to decompiler为jad.exe的全路径，如：C:\jdk\bin\jad.exe，在Directory for temporary files中指定l临时文件路径，如 ：D:\Users\Administrator\AppData\Local\Genuitec\MyEclipse-8.6\result ，其中result是新建一个文件夹. 同时需要选中 User Eclipse code formatter ,这样编译出来的Java 代码会和Class文件的格式一样.
![](https://img-blog.csdn.net/20130608111716203)
为了避免编译后的中文乱码问题，我们需要配置Misc，选中如下单选框.
![](https://img-blog.csdn.net/20130608112021531)
**第九步**：安装完成之后，双击Class文件，MyEclipse就自动反编译了.如果没有出现想要的结果，需要检查一下“*.class”默认关联的编辑器为“JadClipse Class File Viewer“ ，具体的操作还是看图吧：
![](https://img-blog.csdn.net/20130608113057515)
关联之后，就可以自动的反编译啦！

