# Android自动化测试之monkeyrunner - xqhrs232的专栏 - CSDN博客
2012年02月29日 16:09:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：587标签：[android																[测试																[command																[returning																[tools																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=tools&t=blog)](https://so.csdn.net/so/search/s.do?q=returning&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/andy_android/article/details/7242092](http://blog.csdn.net/andy_android/article/details/7242092)
前言：
最近开始研究Android自动化测试方法，对其中的一些工具、方法和框架做了一些简单的整理，其中包括android测试框架、CTS、Monkey、Monkeyrunner、benchmark、其它test tool等等。因接触时间很短，很多地方有不足之处，希望能和大家多多交流。
一、什么是monkeyrunner
monkeyrunner工具提供了一个API，使用此API写出的程序可以在Android代码之外控制Android设备和模拟器。通过monkeyrunner，您可以写出一个Python程序去安装一个Android应用程序或测试包，运行它，向它发送模拟击键，截取它的用户界面图片，并将截图存储于工作站上。monkeyrunner工具的主要设计目的是用于测试功能/框架水平上的应用程序和设备，或用于运行单元测试套件，但您当然也可以将其用于其它目的。
二、monkeyrunner工具同Monkey工具的差别
Monkey：
Monkey工具直接运行在设备或模拟器的[adb](http://developer.android.com/guide/developing/tools/adb.html) shell中，生成用户或系统的伪随机事件流。
monkeyrunner：
monkeyrunner工具则是在工作站上通过API定义的特定命令和事件控制设备或模拟器。
三、monkeyrunner的测试类型
1、多设备控制：monkeyrunnerAPI可以跨多个设备或模拟器实施测试套件。您可以在同一时间接上所有的设备或一次启动全部模拟器（或统统一起），依据程序依次连接到每一个，然后运行一个或多个测试。您也可以用程序启动一个配置好的模拟器，运行一个或多个测试，然后关闭模拟器。
2、 功能测试： monkeyrunner可以为一个应用自动贯彻一次功能测试。您提供按键或触摸事件的输入数值，然后观察输出结果的截屏。
3、 回归测试：monkeyrunner可以运行某个应用，并将其结果截屏与既定已知正确的结果截屏相比较，以此测试应用的稳定性。 
4、 可扩展的自动化：由于monkeyrunner是一个API工具包，您可以基于Python模块和程序开发一整套系统，以此来控制Android设备。除了使用monkeyrunnerAPI之外，您还可以使用标准的Python os和subprocess模块来调用AndroidDebug Bridge这样的Android工具。
四、运行monkeyrunner
您可以直接使用一个代码文件运行monkeyrunner，抑或在交互式对话中输入monkeyrunner语句。不论使用哪种方式，您都需要调用SDK目录的tools子目录下的monkeyrunner命令。如果您提供一个文件名作为运行参数，则monkeyrunner将视文件内容为Python程序，并加以运行；否则，它将提供一个交互对话环境。
monkeyrunner的命令语法为：
monkeyrunner-plugin <plugin_jar> <program_filename> <program_options>
五、实例
以sample中的ApiDemos为例，先将其生成ApiDemos.apk。
前提：已有device连接
1、 将ApiDemos.apk放在$Android_Root\tools下。
2、 在$Android_Root\tools下新建一个monkeyrunnerprogram.py文件，里面内容为：
```
1 # Imports the monkeyrunner modules used by this program 2   3  from com.android.monkeyrunner import MonkeyRunner, MonkeyDevice, MonkeyImage 4  5  # Connects to the current device, returning a MonkeyDevice object 6   7 device = MonkeyRunner.waitForConnection() 8  9  # Installs the Android package. Notice that this method returns a boolean, so you can test 10   11  # to see if the installation worked. 12   13 device.installPackage('./ApiDemos.apk') 14  15  16  # Runs the component 17   18 device.startActivity(component='com.example.android.apis/.ApiDemos') 19  20  21  # Presses the Menu button 22   23 device.press('KEYCODE_MENU','DOWN_AND_UP') 24  25  26  # Takes a screenshot 27   28 result = device.takeSnapshot() 29  30  31  # Writes the screenshot to a file 32   33 result.writeToFile('./shot1.png','png')
[]()
```
注意：SDK上的例子有些错误，不可直接复制，否则执行命令时会发生错误。具体可与我的上面这段代码对照。
3、 打开命令行转到Android_Root\tools目录下运行一下命令：
monkeyrunnermonkeyrunnerprogram.py
11030715:33:19.625:I [main] [com.android.monkeyrunner.MonkeyManager] Monkey Command:wake.
11030715:33:20.625:I [main] [com.android.monkeyrunner.MonkeyManager] Monkey Command:wake.
11030715:33:21.625:I [main] [com.android.monkeyrunner.MonkeyManager] Monkey Command:wake.
11030715:33:22.718:S [pool-1-thread-1] [com.android.monkeyrunner.adb.AdbMonkeyDevice]Error starting command: monkey --port 12345
11030715:33:22.718:S [pool-1-thread-1][com.android.monkeyrunner.adb.AdbMonkeyDevice]com.android.ddmlib.ShellCommandUnresponsiveException
11030715:33:22.718:S [pool-1-thread-1][com.android.monkeyrunner.adb.AdbMonkeyDevice] at com.android.ddmlib.AdbHelper.executeRemoteCommand(AdbHelper.java:408)
11030715:33:22.718:S [pool-1-thread-1][com.android.monkeyrunner.adb.AdbMonkeyDevice] at com.android.ddmlib.Device.executeShellCommand(Device.java:276)
11030715:33:22.718:S [pool-1-thread-1][com.android.monkeyrunner.adb.AdbMonkeyDevice] at com.android.monkeyrunner.adb.AdbMonkeyDevice$1.run(AdbMonkeyDevice.java:89)
11030715:33:22.718:S [pool-1-thread-1][com.android.monkeyrunner.adb.AdbMonkeyDevice] at java.util.concurrent.Executors$RunnableAdapter.call(UnknownSource)
11030715:33:22.718:S [pool-1-thread-1][com.android.monkeyrunner.adb.AdbMonkeyDevice] at java.util.concurrent.FutureTask$Sync.innerRun(Unknown Source)
11030715:33:22.718:S [pool-1-thread-1][com.android.monkeyrunner.adb.AdbMonkeyDevice] at java.util.concurrent.FutureTask.run(Unknown Source)
11030715:33:22.718:S [pool-1-thread-1][com.android.monkeyrunner.adb.AdbMonkeyDevice] at java.util.concurrent.ThreadPoolExecutor$Worker.runTask(UnknownSource)
11030715:33:22.718:S [pool-1-thread-1] [com.android.monkeyrunner.adb.AdbMonkeyDevice]  atjava.util.concurrent.ThreadPoolExecutor$Worker.run(UnknownSource)
11030715:33:22.718:S [pool-1-thread-1][com.android.monkeyrunner.adb.AdbMonkeyDevice] at java.lang.Thread.run(UnknownSource)
11030715:33:57.437:I [main] [com.android.monkeyrunner.MonkeyManager] Monkey Command: press KEYCODE_MENU.
11030715:33:59.171:I [main] [com.android.monkeyrunner.MonkeyManager] Monkey Command: quit.
注：里面exception的提示我们可以忽略，因为我们可以看见 Monkey Command: press KEYCODE_MENU已经执行成功。
4、 可以Android_Root\tools下查看生成的shot1.png的截图。
六、实例扩展
因为ApiDemos首页上按下MENU键没有菜单出现，为了更加形象化，在实例五的基础上继续试验：
1、 在$Android_Root\tools下新建一个monkeyrunnerprogram1.py文件，里面内容为：
```
1 # Imports the monkeyrunner modules used by this program 2   3  from com.android.monkeyrunner import MonkeyRunner, MonkeyDevice, MonkeyImage 4  5 # Connects to the current device, returning a MonkeyDevice object 6  7 device = MonkeyRunner.waitForConnection() 8  9 # Takes a screenshot 10  11 result = device.takeSnapshot() 12  13 # Writes the screenshot to a file 14  15 result.writeToFile('./shotbegin.png','png') 16  17 # Presses the Down button 18  19 device.press('KEYCODE_DPAD_DOWN','DOWN_AND_UP') 20  21 device.press('KEYCODE_DPAD_DOWN','DOWN_AND_UP') 22  23 device.press('KEYCODE_DPAD_DOWN','DOWN_AND_UP') 24  25 device.press('KEYCODE_DPAD_DOWN','DOWN_AND_UP') 26  27 device.press('KEYCODE_DPAD_DOWN','DOWN_AND_UP') 28  29 # Takes a screenshot 30  31 result = device.takeSnapshot() 32  33 # Writes the screenshot to a file 34  35 result.writeToFile('./shotend.png','png')
[]()
```
2、  将画面定位在Apidemos的首页，并将光标定位在第一项上。
3、  在$Android_Root\tools目录下运行一下命令：
monkeyrunnermonkeyrunnerprogram1.py
4、在运行过程中我们可以看见光标不断向下移动，并且可以在当前目录下我们自定义的截图：
运行前：shotbegin.png
![](http://pic002.cnblogs.com/images/2011/214630/2011031018192770.png)
运行后（做了五次下移操作）：shotend.png
![](http://pic002.cnblogs.com/images/2011/214630/2011031018194173.png)
参考资料：
[http://developer.android.com/guide/developing/tools/monkeyrunner_concepts.html](http://developer.android.com/guide/developing/tools/monkeyrunner_concepts.html)
