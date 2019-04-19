# iOS-传感器及使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月02日 22:29:18[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4458
**下面是去apple官网找的iphone6包含的传感器：**
![](http://upload-images.jianshu.io/upload_images/2385282-4e597e66416b7978.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPhone6的感应器.png
# 1、传感器的类型
iPhone内置的传感器有
运动传感器\加速度传感器\加速计（Motion/Accelerometer Sensor）
环境光传感器（Ambient Light Sensor）
距离传感器（Proximity Sensor）
磁力计传感器（Magnetometer Sensor）
内部温度传感器（Internal Temperature Sensor）
湿度传感器（Moisture Sensor）
陀螺仪（Gyroscope）
指纹识别传感器 （Touch ID）
##### 2、环境光传感器（Ambient Light Sensor）
是iPhone和Mac设备中最为古老的传感器成员
它能够让你在使用 Mac、iPhone、iPad时，眼睛更为舒适
从一个明亮的室外走入相对黑暗的室内后，iOS设备会自动调低亮度，让屏幕显得不再那么光亮刺眼
当你使用iPhone拍照时，闪光灯会在一定条件下自动开启
几乎所有的Mac 都带有背光键盘，当周围光线弱到一定条件时，会自动开启键盘背光
##### 3、距离传感器（Proximity Sensor）
用于检测是否有其他物体靠近设备屏幕
当你打电话或接电话时将电话屏幕贴近耳边，iPhone会自动关闭屏幕 ，好处是
节省电量
防止耳朵或面部不小心触摸屏幕而引发一些不想要的意外操作
题外话：利用距离传感器，能找出很多电视剧的穿帮镜头
##### 4、磁力计传感器（Magnetometer Sensor）
可以感应地球磁场， 获得方向信息， 使位置服务数据更精准
可以用于电子罗盘和导航应用
iPad的Smart Cover盒盖睡眠操作就是基于磁力计传感器
##### 5、内部温度传感器（Internal Temperature Sensor）
从 iPad一代开始，iOS设备都加入了一个内部温度传感器，用于检测内部组件温度，当温度超过系统设定的阈值时,会出现以下提示
##### 6、湿度传感器（Moisture Sensor）
湿度传感器跟其他基于微电子的传感器不同，是一个简单的物理传感器
简单来说，湿度传感器就是一张遇水变红的试纸
Apple的维修人员就是通过检测试纸是否变红，来判断设备是否进水
（设备进水不在保修范围之内）
##### 7、陀螺仪（Gyroscope）
陀螺仪是随着iPhone4的上市首次出现在iOS设备上的传感器
陀螺仪的原理是检测设备在X、Y、Z轴上所旋转的角速度
陀螺仪在赛车类游戏中有重大作用：
模拟汽车驾驶时方向盘旋转的动作
使得这类游戏的操控体验更为真实 
##### 8、加速计传感器（Motion/Accelerometer Sensor）
最早出现在iOS设备上的传感器之一
加速计用于检测设备在X、Y、Z轴上的加速度 （哪个方向有力的作用）
加速计可以用于检测设备的摇晃，经典应用场景：摇一摇、计步器
#### 总结：
![](http://upload-images.jianshu.io/upload_images/2385282-1977b1f0d730eb64.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
755161-20151001011039027-1674981830.png
## 距离传感器的使用
![](http://upload-images.jianshu.io/upload_images/2385282-7f4ef36596f45f6f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
proximity.png
## 加速计的使用
这要用到iOS的一个核心运动框架CoreMotion.framework
> - 1.使用iOS给我们提供的CoreMotion.framework框架,主要是为了访问**加速度计**和**陀螺仪**相关的数据
- 2.它不仅仅提供给你获得实时的加速度值和旋转速度值，更重要的是，苹果在其中集成了很多算法，可以直接给你输出把重力加速度分量剥离的加速度，省去你的高通滤波操作，以及提供给你一个专门的设备的三维位置信息。
#### CoreMotion的使用
**CoreMotion**主要负责下面三种数据:
- 1.加速度值CMAccelerometerData
- 2.陀螺仪值CMGyroData
- 3.设备motion值CMDeviceMotion (实际上，这个设备motion值就是通过加速度和旋转速度进行变换算出来的)
##### CMDeviceMotion属性介绍
> - **1**、attitude：通俗来讲，就是告诉你手机在当前空间的位置和姿势。
- **2**、gravity：重力信息，其本质是重力加速度矢量在当前设备的参考坐标系中的表达。
- **3**、userAcceleration：加速速信息。
- **4**、rotationRate：计时的旋转速率，是陀螺仪的输出。
##### CoreMotion中有两种获取数据的方式：
> - 1、Push
方式：提供一个线程管理器NSOperationQueue和一个回调Block，CoreMotion自动在每一个采样数据到来的时候回调这个Block，进行处理。在这种情况下，Block中的操作会在你自己的**主线程**内执行。
- 2、Pull
方式：你必须主动去向CMMotionManager要数据，这个数据就是最近一次的采数据。你不去要，CMMotionManager就不会给你。
###### push方式获取加速计数据
![](http://upload-images.jianshu.io/upload_images/2385282-85b54dd4d680822f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Push.png
###### push方式获取陀螺仪数据
![](http://upload-images.jianshu.io/upload_images/2385282-edbfb74fe1f6f2fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gyro-Push.png
###### push方式获取计步器数据
*下面是iOS7的用法*
![](http://upload-images.jianshu.io/upload_images/2385282-cf62c38c1bd5ebbf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
cmStepCounter.png
*iOS8之后取代CMStepCounter的是 CMPedometer*,代码如下
首先声明一个属性
@property (nonatomic, strong) CMPedometer *pedometer;
接下来就可以初始化,通过下面的代码就可以了,注意红框框选住的是获取某一段时间所走步数的代码
![](http://upload-images.jianshu.io/upload_images/2385282-5b85968c76dea8f2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iOS8计步.png
文／suppermann（简书作者）
原文链接：http://www.jianshu.com/p/300f6f516e0f
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
