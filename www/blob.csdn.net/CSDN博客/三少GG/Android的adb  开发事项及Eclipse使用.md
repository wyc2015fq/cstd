# Android的adb ||开发事项及Eclipse使用 - 三少GG - CSDN博客
2012年01月01日 14:13:26[三少GG](https://me.csdn.net/scut1135)阅读数：1586标签：[android																[eclipse																[shell																[tools																[properties																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=properties&t=blog)](https://so.csdn.net/so/search/s.do?q=tools&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
2.  [http://blog.sina.com.cn/s/blog_639fb8350100mm5b.html](http://blog.sina.com.cn/s/blog_639fb8350100mm5b.html)
Android创建SD卡镜像文件：
进入SDK安装目录的**tools**目录下，执行以下命令：
$./mksdcard -l sdcard <size>M<path>/<filename>.img
以带SD卡的方式启动Android模拟器：
进入SDK安装目录的**tools**目录下，执行以下命令：
$./emulator -sdcard <sd image filename> -avd <avdname>
Android的adb工具的使用：
该工具主要用于与模拟器或真机交互。
位置：<android sdk path>/tools/adb **!!!Grant: 现在sdk r5应该为****<android sdk path>/platform-tools/**adb
常用命令：
1、**安装adk程序：$./adb install <apk file name>**
**2、进入shell$./adb shell**
3、执行一条shell命令：
$./adb shell <command>
4、删除：
首先进入shell：
$./adb shell
再执行删除操作：
#rm [-r] <file name>
-r参数：用于删除文件夹。
5、**拷贝文件$./adb push <local file name><target path>$./adb pull <target file name><local path>**
6、取得ID及序列号：
$./adb get-product
$./adb get-serialno
7、提高执行权限：
$./adb remount
## Android开发一些事项及Eclipse使用
Android添加source到Eclipse：
1、将sdk源代码目录文件夹 sources 拷贝到sdk的以下目录：
platforms/<sdk version name>/
2、在eclipse中添加一个用户库，并将android.jar加入到该库中。
3、将该自定义库的 source 指定到 sources 文件夹。
4、移除工程自带的Android库，加入自定义库。
5、刷新工程，这时可以查看skd的source了。
6、将引用的自定义库删除。
7、在项目属性的Android项中重新选择对应的 Target。
8、项目右键 -> Android Tools -> Fix Project Properties。
3. 如何使用Android SDK开发Android应用
http://hi.baidu.com/324280429/blog/item/4c33e9a4da1bd4fc9052eeda.html/cmtid/4f71a7065eed117503088164
> 
> 
从官方文档和实践可以总结出几点：
1、可以使用eclipse来编辑JAVA程序、检查错误（主要是类库包含和语法方面），但是不能在eclipse上编译运行android源码，还是得在shell中make(或mm或mmm)
2、android源码文件夹里提供有一些eclipse配置文件，
.claapath：eclipse工程的配置文件，方便我们直接把android源码相应的文件和JAVA包导入工程
android-formatting.xml和android.importorder：这个很重要，主要是用来规范我们的编码风格，更容易使我们的代码风格一致
3、把android源码作为一个工程导入eclipse时，必须注意两点
1)、新建的工程必须是java project，不能是android project，否则会破坏android源码（一般是多添加文件/文件夹）
2)、导入前最好检查.classpath里的文件在android源码中是否有相应的文件（文件夹），否则也会破坏android源码（一般是多添加文件/文件夹）
> 
总的来说：
1、用eclipse来编辑代码、检查错误
2、不在eclipse上编译、运行android源码程序，只能在命令行通过make(或mm或mmm)编译android源码
3、可以在eclipse上调试android源码程序（原理：eclipse通过ddms服务器在emulator上进行调试），并可以单步调试、断点调试。
4、需要调试的程序把它从/system/app/移除，安装到data/app下，这样更方便
5、安装、卸载程序通过adb push 和adb shell rm更方便

