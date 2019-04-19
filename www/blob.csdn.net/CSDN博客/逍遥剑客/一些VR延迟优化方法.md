# 一些VR延迟优化方法 - 逍遥剑客 - CSDN博客
2016年02月15日 15:10:02[xoyojank](https://me.csdn.net/xoyojank)阅读数：14561
VR中的”延迟”, 特指”Motion-To-Photon Latency”, 指的是从用户运动开始到相应画面显示到屏幕上所花的时间.  
![这里写图片描述](https://img-blog.csdn.net/20160215090916670)
这中间经过了大概这么几个步骤:
- 传感器采集运动输入数据
- 采集到的数据进行过滤并通过线缆传输到主机
- 游戏引擎根据获取的输入数据更新逻辑和渲染视口
- 提交到驱动并由驱动发送到显卡进行渲染
- 把渲染的结果提交到屏幕, 像素进行颜色的切换
- 用户在屏幕上看到相应的画面
> 
当然, 实际上还有很多细节问题, 比如屏幕上的像素并不是同一时间切换的, 可能面上面的那行先切换, 再一行行更新到最下面的, 在这里就不纠结这些细节了. 
这其中的每一个步骤都会产生一定的延迟, 而目前公认的大众能接受的延迟是20ms以下, 这基本上可以做为衡量一个VR头显是不是合格的一个标准. 虽然20ms是非常短的时间, 但通过努力还是可以达到的, 主要有这么几个思路:
# 硬件层面的优化
- 提升传感器的采样频率, 减少刷新率与传感器频率的同步等待时间消耗
- 提升传感器的精度, 减少对采样数据进行稳定性过滤产生的延迟
- 采用有线传输也有一部分原因是出于延迟的考虑
- 屏幕使用OLED替代LCD, 减少像素颜色切换的时间
- 提升屏幕刷新率, 主流的屏幕是60Hz, 那每帧就是16.67ms; 如果提升到90Hz, 那每帧就是11.11ms
大部分的手机VR产品在延迟上都是不合格的, 最明显的表现就是转头时的画面不连续/抖动/残影等:
- 市面上的手机采用OLED屏的还是少数, 比如iPhone配个VR壳子那延迟就很感人
- 如果依赖手机的陀螺仪进行转向模拟, 其精度和频率远远达不到要求
- 手机屏幕目前都是60Hz的刷新率, 在延迟上本身就受限
## 刷新率的提升
假设刷新率为60Hz, 并不是代表每帧就有16.67ms的延迟, 而是说屏幕图像每16.67ms才更新一次, 渲染选项中的”垂直同步”的概念就是来源于此. 这就对我们提交渲染画面的时机要求非常高, 如下图: 
![这里写图片描述](https://img-blog.csdn.net/20160215100204079)
为了方便计算, 这里先假设传感器, 传输, 屏幕像素切换的延迟都为0
- 假设我们在每帧开始的时候(上一次垂直同步结束)采样一次传感器数据, 在垂直同步之前完成提交, 那延迟就是16.67ms
- 如果当前帧无法在16.67ms内完成渲染, 比如花了17ms, 那么就会拖到下一帧进行提交, 屏幕上显示的画面就还是上一次的图像, 这时候的延迟就变成了16.67*2=33.33ms
这就对VR的渲染提出了非常高的要求:
- FPS必须达到刷新率的要求, 90Hz就是90Hz, 80FPS是不行的, 会被垂直同步拖累成45FPS
- FPS必须保证稳定, 偶尔掉一两帧在VR中的感觉非常明显, 可能某个物体的位置已经差了几十个像素了
以Oculus Rift(消费版)为例, 1080x1200x2的屏幕分辨率, 90Hz的刷新率, 再加上因为变形所需要的UpSampling, 实际的渲染画面就是3024x1680@90Hz, 这性能压力几乎与4k@60Hz相当. 所以, 单纯的提升刷新率和分辨率, 目前来说渲染能力还是跟不上. 不过既然有了性能需求, 硬件厂商才有前进动力, 对整个行业生态来说, 是件好事.
# 引擎层面的优化
除了拼命优化降低每帧画面的渲染时间外, 引擎层面还可以通过一些策略进行优化, 关键的思路就是: 能不能把采样传感器数据的时间点尽量延后, 让它与垂直同步的时间点尽量靠近?
这里我们仍然假设60Hz, 每帧时间16.67ms(约17ms), 忽略硬件延迟 
![这里写图片描述](https://img-blog.csdn.net/20160215105545180)
如果在游戏逻辑过程中(1ms时)采样传感器数据, 那延迟大约就是16ms 
![这里写图片描述](https://img-blog.csdn.net/20160215105600977)
如果在渲染线程进行绘制之前(5ms时), 重新再采样一下传感器数据, 修正一下视口信息(不会对游戏逻辑产生影响), 那延迟就缩短到了约12ms 
![这里写图片描述](https://img-blog.csdn.net/20160215105609526)
做过渲染优化的人都知道, 提交D3D Command后, 需要等待GPU执行完毕, 这部分时间在整帧时间中的占比还是相当高的. 那有没有办法在渲染完成之后, 提交到屏幕之前再次采样一次传感器数据呢? 如果像下图那样的话, 延迟可以缩短到3ms!!! 
![这里写图片描述](https://img-blog.csdn.net/20160215105618604)
这就是Timewarp的主要思想, 我们来看看它是怎么实现的
## Timewarp
了解过延迟渲染的人应该都知道, 我们可以利用ZBuffer的深度数据, 逆向推导出屏幕上每个像素的世界坐标 
![这里写图片描述](https://img-blog.csdn.net/20160215112048285)
这就意味着, 我们可以把所有像素变换到世界空间, 再根据新的摄像机位置, 重新计算每个像素的屏幕坐标, 生成一幅新的图像: 
![这里写图片描述](https://img-blog.csdn.net/20160215112540084)
可以看到之前被遮挡区域的像素是缺失的, 因为我们的摄像机位置变化了. 那如果摄像机位置不变, 仅仅是朝向变了呢? 这样就不存在像素可见性的变化了: 
![这里写图片描述](https://img-blog.csdn.net/20160215113158102)
Timewarp正是利用了这个特性, 在保证位置不变的情况下, 把渲染完的画面根据最新获取的传感器朝向信息计算出一帧新的画面, 再提交到显示屏. 由于角度变化非常小, 所以边缘不会出大面积的像素缺失情况.  
![这里写图片描述](https://img-blog.csdn.net/20160215145450588)
Oculus的Demo中可以停止渲染新的画面, 完全由单帧图像计算出各个朝向的新画面: 
![原始图像](https://img-blog.csdn.net/20160215141748699)![向左上看](https://img-blog.csdn.net/20160215141802613)![向右下看](https://img-blog.csdn.net/20160215141814574)
也就是说, 只要角度变化不是非常大(上图为了演示效果偏转的角度很大了), 可以通过这项技术”凭空渲染”出下一帧的图像, SONY的PSVR正是利用这一点, 把60FPS的画面Reproject成了120FPS.  
Timewarp只能处理头部转向, 不能处理头部移动的情况, 而且一旦错过了垂直同步的时机, 一样需要等待下一次垂直同步才能显示出来. 那能不能在每次垂直同步之前, 强制进行一次Timewarp呢? 那就需要驱动来开个后门了…
# 驱动层面的优化
假设垂直同步时, 当前帧还没有渲染完毕, 这时如果要进行Timewarp的话, 就需要驱动提供一种高优先级的异步调用, 这就是异步Timewarp的由来: Timewarp操作与场景渲染并行执行, 如果没有新的渲染画面, 就继续使用上一帧的画面进行Timewarp. 
![这里写图片描述](https://img-blog.csdn.net/20160215143747988)
这可以在一定程度上补偿FPS不达标造成的延迟问题, GearVR中正是应用了这项技术, 保证了手机VR的体验.  
当然, PC上使用项技术还是有一些限制:
- 必须是Fermi, Kepler, Maxwell(或更新)核心的GPU
- GPU是以DrawCall为单位调度的, 所以耗时太长的DrawCall是插入不了Timewarp绘制操作的
- 需要最新的Oculus和NVIDIA驱动支持
异步Timewarp并不是说FPS低于标准还能流畅跑, 这只是一种补救措施, 所以优化仍然要好好做-_-
驱动方面还有一些其它的优化空间, 比如强制提交渲染队列: 
![这里写图片描述](https://img-blog.csdn.net/20160215144746242)
如果驱动中缓存了3帧, 那延迟优化就白做了… 
![这里写图片描述](https://img-blog.csdn.net/20160215144833308)
另外就是大家耳熟能详的Back Buffer(Double Buffer Rendering), 其实也会增加一点延迟, 不如省掉这一步, 即Front Buffer Rendering, 或者叫Direct Mode: 
![这里写图片描述](https://img-blog.csdn.net/20160215145807883)
# 参考资料
[What is Motion-To-Photon Latency?](http://www.chioka.in/what-is-motion-to-photon-latency/)
[Optimizing VR Graphics with Late Latching](https://developer.oculus.com/blog/optimizing-vr-graphics-with-late-latching/)
[VR Direct: How NVIDIA Technology is Improving the VR Experience](www.reedbeta.com/talks/VR_Direct_GDC_2015.pptx)
[Virtual Reality with AMD LiquidVR™ Technology](http://www.amd.com/en-us/innovations/software-technologies/technologies-gaming/vr)
[Lessons from Integrating the Oculus Rift into Unreal Engine 4](http://static.oculus.com/connect/slides/OculusConnect_Epic_UE4_Integration_and_Demos.pdf)
[Oculus Rift - How Does Time Warping Work?](https://www.youtube.com/watch?v=WvtEXMlQQtI)
[Asynchronous Timewarp Examined](https://developer.oculus.com/blog/asynchronous-timewarp-examined/)
