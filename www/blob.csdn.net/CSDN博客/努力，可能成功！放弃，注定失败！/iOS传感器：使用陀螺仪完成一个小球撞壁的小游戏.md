# iOS传感器：使用陀螺仪完成一个小球撞壁的小游戏 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月08日 09:23:39[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：844
**本文来自简书，原文地址:[http://www.jianshu.com/p/0763d4ac256d](http://www.jianshu.com/p/0763d4ac256d)**
上一次借着实现一个随屏幕旋转的小玩意，了解了iPhone内置的加速计。今天咱们继续搞点好玩的东东。按照计划这次要看看陀螺仪了。
![](http://upload-images.jianshu.io/upload_images/2248583-fa298cacd4a978bf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/400)
一个超级贱的利用陀螺仪的APP
.png
最终咱们会完成一个小球撞壁的小游戏。小球可以感受到重力，从而能够随着手机的运动来一起运动。为了增加一点点趣味性，对小球的运动范围做了限制。当小球碰到屏幕的边缘的时候，会进行反弹，相反方向运动。咱们一起来看看实现后的实况录像：
![](http://upload-images.jianshu.io/upload_images/2248583-f4462d180e7f4edf.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/400)
ball.gif
今天的代码比起上次的加速计稍微多了一点点，所以就提供了源码供大家批评。同时由于这部分Swfit和Objective-C略微有不太一样的地方，所以源码提供了两版。
其实不管是加速计还是今天的陀螺仪，都是用到了上次说的iOS当中的那个核心运动框架`CoreMotion`。
# 1. 陀螺仪介绍
陀螺仪主要是用来测量沿着某个特定的坐标轴旋转速度的。在使用中，陀螺仪始终指向一个固定的方向，当运动物体的运动方向偏离预定方向时，陀螺仪就可以感受出来。
在手机上，仅用加速度计没办法测量或重构出完整的3D动作，测不到转动的动作的，加速计只能检测轴向的线性动作。但陀螺仪则可以对转动、偏转的动作做很好的测量，这样就可以精确分析判断出使用者的实际动作。而后根据动作，可以对手机做相应的操作。
## 1.1 陀螺仪的应用场景
各位童鞋相比都玩过Wii，那个体感手柄肯定就用到了陀螺仪。玩家通过挥动运动手柄，来控制游戏。例如乒乓球、网球、赛车等等。有一些酷炫的APP会通过小幅度的倾斜，偏转手机，实现彩蛋功能，例如放大缩小之类的。或者把手机屏幕翻转，就可以拒接电话或者静音啥的。
拍照类的APP也会通过陀螺仪把拍照时候手的抖动反馈交给图像处理器，以便抓到更清晰稳定的图片。
还有一些是最近刚刚看到的好贱好贱的APP。例如`Send Me To Heaven`，游戏的玩法超级简单，只需向天空抛掷手机，扔得越高，分数也就越高。
![](http://upload-images.jianshu.io/upload_images/2248583-d3d5f4ccc75f4c71.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/400)
Send Me To Heaven.png
> 
Throw Me App
另外一个贱不拉几的APP。这是一个相机APP，使用时打开APP并将手机抛向空中，当手机在空中时，使用陀螺仪和加速计探测手机是否达到了最高点，且摄像头是否向下。随后，该应用将激活摄像头快门进行拍照。
![](http://upload-images.jianshu.io/upload_images/2248583-7b108adf7ad602c2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/400)
Throw Me App.png
## 1.2 陀螺仪在iOS中的使用
iPhone、iPad、iWatch都有内置的陀螺仪，也都可以让开发者进行调用。同样，用一张图展现一下：
![](http://upload-images.jianshu.io/upload_images/2248583-e9ad623589f97ca3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/400)
image.png
# 2. 陀螺仪的使用
## 2.1 使用步骤
陀螺仪同样也是通过`CoreMotion`这个框架来管理的，所以和加速计一样，四个标准步骤：
- 初始化CMMotionManager管理对象；2. 调用管理对象的对象方法获取数据；3. 处理数据；4. 当不需要使用的时候，停止获取数据。
## 2.2 陀螺仪数据获取的两种方法
CoreMotion中有2种获取数据方式，一种叫做PUSH的方式，一种叫做PULL的方式。顾名思义，PUSH就是被动的获取。设定完了之后，线程定时把获取到的数据推送回来。可想而知，对于资源的消耗是会稍微大一点的。
PULL，就是要去索取。拉一下才会获取到数据。不要不给。上一次加速计咱们给出的代码是OC的，今天咱们就用Swift的。
## 2.2.1 PULL的方式
```
private func useGyroPull() {
        //判断陀螺仪可不可用
        if manager.isGyroAvailable {
            //设置陀螺仪多久采样一次
            manager.gyroUpdateInterval = 0.1
            //开始更新，后台线程开始运行。这是Pull方式。
            manager.startGyroUpdates()
        }
        //获取并处理陀螺仪数据。这里我们就只是简单的做了打印。
        print("X = \(manager.gyroData?.rotationRate.x ?? 0)","Y = \(manager.gyroData?.rotationRate.y ?? 0)","Z = \(manager.gyroData?.rotationRate.z ?? 0)")
    }
```
## 2.2.2 PUSH的方式
```
private func useGyroPush() {
        //判断陀螺仪可不可用
        if manager.isGyroAvailable {
            //设置陀螺仪多久采样一次
            manager.gyroUpdateInterval = 0.1
            //Push方式获取和处理数据，这里我们一样只是做了简单的打印。把采样的工作放在了主线程中。
            manager.startGyroUpdates(to: OperationQueue.main, withHandler: { (gyroData, error) in
                print("X = \(self.manager.gyroData?.rotationRate.x ?? 0)","Y = \(self.manager.gyroData?.rotationRate.y ?? 0)","Z = \(self.manager.gyroData?.rotationRate.z ?? 0)")
            })
        } else {
            print("陀螺仪不可用")
        }
    }
```
# 3. 开始我们的小游戏
## 3.1 思维导图
![](http://upload-images.jianshu.io/upload_images/2248583-aa69aeb359d435f8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
小球撞壁游戏.png
## 3.2 实现
### 3.2.1 以X轴边界值处理及碰壁后速度处理为例
```
//            对球在X轴碰壁进行处理
            if currentPoint.x <=  imageWidth / 2 {
              currentPoint.x = imageWidth / 2
                ballXVelocity = -ballXVelocity * 0.8
            }
            if currentPoint.x >= bounds.size.width - imageWidth / 2 {
                currentPoint.x = bounds.size.width - imageWidth / 2
                ballXVelocity = -ballXVelocity * 0.8
            }
```
### 3.2.2 开启陀螺仪并更新
```
manager.deviceMotionUpdateInterval = 1 / 60
        //注意一下，在Swift没有了NSOperation。被OperationQueue取代了。
        manager.startDeviceMotionUpdates(to: OperationQueue.main) { (motion, error) in
            self.ballView!.accelleration = (motion?.gravity)!
            //开启主队列异步线程，更新球的位置。
            DispatchQueue.main.async {
                self.ballView!.updateLocation(multiplier: 5000)
            }
```
### 3.2.3 更新小球的位置
```
func updateLocation(multiplier : Double) {
        if (lastUpdateTime != nil) {
            let updatePeriod : Double = Date.init().timeIntervalSince(lastUpdateTime!)
            ballXVelocity = ballXVelocity + accelleration.x * updatePeriod
            ballYVelocity = ballYVelocity + accelleration.y * updatePeriod
            let coefficient = updatePeriod * multiplier
            currentPoint = CGPoint(x: currentPoint.x + (CGFloat)(ballXVelocity * coefficient), y: currentPoint.y - (CGFloat)(ballYVelocity * coefficient))
        }
        lastUpdateTime = Date()
    }
```
## 3.3 关于Swift中重写set/get
其实写到这里的时候才突然想起来，咱们从来没有说过Swift怎么重写Set/Get方法。而且貌似也没有分享过iOS开发中多线程的东东。下个系列可以就谢谢多线程相关的玩意儿吧，如果多线程这部分不太明白的话，对不住对不住对不住，马上补上。
在swift中其实重新set不太常见，但这都是OC留下来的臭毛病，就非要重新咋办？
可以看看这篇文章[iOS 重写Swift中的set和get方法](http://www.jianshu.com/p/%5Bhttp://www.jianshu.com/p/bc67ca442c9c)。]([http://www.jianshu.com/p/bc67ca442c9c)。](http://www.jianshu.com/p/bc67ca442c9c)%E3%80%82))
这个不是重点，咱们在写小球的时候用到的是didSet这个方法。这是啥呐？这是swift当中的观察者，用来监视属性除了初始化之外的属性变化。
- `didSet`:在属性值改变后触发，didSet可以带一个oldName的参数，表示旧的属性，不带的话默认命名为oldValue。
- `willSet`:在属性值改变前触发，可以带一个newName的参数，没有的话，该参数默认命名为newValue。
源代码下载地址：OC+Swift两版。[下载地址](https://git.oschina.net/atypical/sensor.git)
[iOS传感器系列之一：加速传感器](http://www.jianshu.com/p/ae35e488808c)
[iOS传感器系列之二：陀螺仪](http://www.jianshu.com/p/0763d4ac256d)
