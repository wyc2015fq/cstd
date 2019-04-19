# 如何运行Android模拟器 - 2019跟着小虎玩着去软考 - CSDN博客
2011年07月27日 13:05:03[littletigerat](https://me.csdn.net/littletigerat)阅读数：7568

**如何运行Android模拟器**
**![](http://hi.csdn.net/attachment/201107/27/0_1311743096raob.gif)**
# 一．查看当前支持哪些版本
Android list targets
在列出的版本中，需要记住id值，这个值在第三步中使用
# 二．AVD是什么
AVD是Android Virtual Device的缩写，Android虚拟设备，AVD就相当于是一个模拟器的，可以利用AVD创建基于不同版本的模拟器，然后使用emulator-avd avdName命令启动不同的模拟器。
 在android sdk 1.5 版本以后的开发中，必须至少创建一个AVD，每个AVD模拟一套虚拟设备来运行Android应用程序。
# 三．如何创建AVD
android create avd –n avdName –t 2
说明：
-n之后的avdName表示该虚拟设备的名称
n是name的缩写
-t之后的2表示创建1.5版本的avd
t是target的缩写
这里t代表target目标平台的API Level，我们可以使用android list targets来查看这里t的可选参数
命令格式：android create avd -n [avd名称] -t [api level]
android create avd –name adk_1_5_version --target 2
 在创建avd文件得第二步，控制台提示“是否自定义自己得设备”，默认选项是"no",如果输入 "y",接下来可以一步步根据提示，定制自己得模拟器性能参数。
# 四．如何查看创建AVD是否成功
android list avd
# 五．如何启动模拟器
emulator -avd avdName
# 六．如何选择启动的皮肤
emulator -avd avdName -skin HVGA-L
# 七．模拟器如何横纵摆放
按Ctrl+F11，可以直接改变模拟器的横纵摆放
# 八．创建SDCard镜像文件
mksdcard -l sdcard 512M  e:\sdcard.img 
这样就创建了一个512M的SD卡镜像文件:查看E盘，E盘下会有一个sdcard.img 文件
# **九．运行AVD时加载sdcard**
emulator -avd MyAVD -sdcard sdcard.img 
如果不想每次都加-sdcard $sdcard_path,有两个办法 
解决办法一：可以把sdcard.img 复制到C:\Documents and Settings\Administrator\.android\avd\下对应的AVD目录下 
解决办法二：修改配置文件，C:\Documents and Settings\Administrator\.android\avd\AVD_NAME.avd\，打开config.ini，加入以下行：sdcard.path=你的SD卡路径
# **十．AVD路径设置**
不管用Eclipse还是从命令行创建Android Emulator时，相应的文件是被放置到 “C:\Documents and Settings\Administrator\.android“ 下面。如果想改变AVD的路径设置，也有2个办法 
解决办法一： 
   新添加一个环境变量，名字为 "ANDROID_SDK_HOME”，然后把变量值改为你想将".android”文件夹(AVD信息)所放置的位置 
解决办法二： 
   修改配置文件中的路径，比如我的AVD名称是MyAVD, 打开 
      C:\Documents and Settings\Administrator\.android\avd目录中的文件MyAVD.ini文件，找到 
       target=android-5
      path =C:\Documents and  Settings\Administrator\.android\avd\MyAVD.avd 
     修改其路径即可
 若遇到ERROR: unknown virtual device name 这样的问题一般是由于AVD文件路径引用错误造成的。
