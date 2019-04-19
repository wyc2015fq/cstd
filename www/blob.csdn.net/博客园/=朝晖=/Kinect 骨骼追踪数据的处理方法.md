# Kinect 骨骼追踪数据的处理方法 - =朝晖= - 博客园
# [Kinect 骨骼追踪数据的处理方法](https://www.cnblogs.com/dhcn/p/7130901.html)
http://www.ituring.com.cn/article/196144
> 
作者/ 吴国斌
博士，PMP，微软亚洲研究院学术合作经理。负责中国高校及科研机构Kinect for Windows学术合作计划及微软精英大挑战Kinect主题项目。曾担任微软TechEd2011 Kinect论坛讲师，微软亚洲教育高峰会Kinect分论坛主席，中国计算机学会学科前沿讲习班Kinect主题学术主任。
骨骼追踪技术是Kinect的核心技术，它可以准确标定人体的20个关键点，并能对这20个点的位置进行实时追踪。利用这项技术，可以开发出各种基于体感人机交互的有趣应用。
## **骨骼追踪数据的结构**
目前，Kinect for Windows SDK中的骨骼API可以提供位于Kinect前方至多两个人的位置信息，包括详细的姿势和骨骼点的三维坐标信息。另外，Kinect for Windows SDK最多可以支持20个骨骼点。数据对象类型以骨骼帧的形式提供，每一帧最多可以保存20个点，如图1所示。
![](http://www.ituring.com.cn/figures/2015/manong_game/082.png)
**图1　20个骨骼点示意图**
在SDK中每个骨骼点都是用`Joint`类型来表示的，每一帧的20个骨骼点组成基于`Joint`类型的集合。此类型包含3个属性，具体内容如下所示。
- 
`JointType`：骨骼点的类型，这是一种枚举类型，列举出了20个骨骼点的特定名称，比如“HAND_LEFT”表示该骨骼点是左手节点。
- 
`Position`：`SkeletonPoint`类型表示骨骼点的位置信息。`SkeletonPoint`是一个结构体，包含X、Y、Z三个数据成员，用以存储骨骼点的三维坐标。
- 
`TrackingState`：`JointTrackingState`类型也是一种枚举类型，表示该骨骼点的追踪状态。其中，`Tracked`表示正确捕捉到该骨骼点，`NotTracked`表示没有捕捉到骨骼点，`Inferred`表示状态不确定。
## **半身模式**
如果应用程序只需要捕捉上半身的姿势动作，就可以采用Kinect for Windows SDK提供的半身模式（Seated Mode）。在半身模式下，系统只捕捉人体上半身10个骨骼点的信息，而忽略下半身另外10个骨骼点的位置信息，这样就解决了用户坐在椅子上时无法被Kinect识别的问题，即使下半身骨骼点的数据不稳定或是不存在也不会对上半身的骨骼数据造成影响。而且当用户距离Kinect设备只有0.4米时，应用程序仍能正常地进行骨骼追踪，这就大幅提高了骨骼追踪的性能。
半身模式定义在枚举类型`SkeletonTrackingMode`中，该类型包含两个枚举值：Default和Seated。前者为默认的骨骼追踪模式，会正常捕捉20个骨骼点；后者为半身模式，选择该值则只捕捉上半身的10个骨骼点。
开发者可以通过改变`SkeletonStream`对象的`TrackingMode`属性来设置骨骼追踪的模式，代码如下：
`kinectSensor.SkeletonStream.TrackingMode = SkeletonTrackingMode.Seated;`
## **骨骼追踪数据的获取方式**
应用程序获取下一帧骨骼数据的方式同获取彩色图像和深度图像数据的方式一样，都是通过调用回调函数并传递一个缓存实现的，获取骨骼数据调用的是`OpenSkeletonFrame()`函数。如果最新的骨骼数据已经准备好了，那么系统就会将其复制到缓存中；但如果应用程序发出请求时，新的骨骼数据还未准备好，此时可以选择等待下一个骨骼数据直至其准备完毕，或者立即返回稍后再发送请求。对于NUI骨骼API而言，相同的骨骼数据只会提供一次。
NUI骨骼API提供了两种应用模型，分别是轮询模型和时间模型，简要介绍如下。
- 
轮询模型是读取骨骼事件最简单的方式，通过调用`SkeletonStream`类的`OpenNextFrame()`函数即可实现。`OpenNextFrame()`函数的声明如下所示。
```
public SkeletonFrame OpenNextFrame (
     int millisecondsWait
)
```
可以传递参数指定等待下一帧骨骼数据的时间。当新的数据准备好或是超出等待时间时，`OpenNextFrame()`函数才会返回。
- 
时间模型以事件驱动的方式获取骨骼数据，更加灵活、准确。应用程序传递一个事件处理函数给`SkeletonFrameReady`事件，该事件定义在`KinectSensor`类中。当下一帧的骨骼数据准备好时，会立即调用该事件回调函数。因此Kinect应用应该通过调用`OpenSkeletonFrame()`函数来实时获取骨骼数据。
## **实例——调用API获取骨骼数据并实时绘制**
本实例程序将实现获取骨骼数据，然后将骨骼点的坐标作为Ellipse控件的20个位置坐标，同时用线段将相应的点连接起来，最后将绘制出的骨架映射到彩色图像上。读者可以在实例1的基础上开始本实例，具体操作步骤如下所示。
1. 在`Window_Loaded()`函数中添加下列骨骼数据流的启动函数，并添加`kinectSensor_SkeletonFrameReady`事件处理函数相应的`SkeletonFrameReady`事件。
```
kinectSensor.SkeletonStream.Enable();
kinectSensor.SkeletonFrameReady += new
     EventHandler<SkeletonFrameReadyEventArgs>(kinectSensor_SkeletonFrameReady);
```
2. 准备WPF界面。通过以下代码在界面上添加20个小圆点，分别跟踪由Kinect for Windows SDK获取到的人体的20个关键点，并将这20个点标记为不同的颜色。
```
<Canvas Name="SkeletonCanvas" Visibility="Visible">
      <Ellipse Canvas.Left="0" Canvas.Top="0" Height="10" Name="headPoint" Width="10" Fill="Red" />
    <Ellipse Canvas.Left="10" Canvas.Top="0" Height="10" Name="shouldercenterPoint" Width="10" Fill="Blue" />
    <Ellipse Canvas.Left="20" Canvas.Top="0" Height="10" Name="shoulderrightPoint" Width="10" Fill="Orange" />
    …省略中间的Ellipse定义
     <Image Canvas.Left="303" Canvas.Top="161" Height="150" Name="image1"
        Stretch="Fill" Width="200" />
</Canvas>
```
此时，设计窗口如图2所示。
![](http://www.ituring.com.cn/figures/2015/manong_game/083.png)
**图2　WPF设计界面**
3. 编写`kinectSensor_SkeletonFrameReady()`事件处理函数。正确连接Kinect后，当用户站在Kinect前并且Kinect能够正确识别人体时，将触发该事件处理函数，其代码如下：
```
private void kinectSensor_SkeletonFrameReady(object sender, SkeletonFrameReadyEventArgs e)
{
    using (SkeletonFrame skeletonFrame = e.OpenSkeletonFrame())
    {
        if (skeletonFrame != null)
        {
            skeletonData = new
                Skeleton[kinectSensor.SkeletonStream.FrameSkeletonArrayLength];
                  skeletonFrame.CopySkeletonDataTo(this.skeletonData);
            Skeleton skeleton = (from s in skeletonData
                where s.TrackingState == SkeletonTrackingState.Tracked
                select s).FirstOrDefault();
            if (skeleton!=null)
            {
              SetAllPointPosition(skeleton);
            }
        }
    }
}
```
上述代码使用LINQ语句来获取`TrackingState`等于`Tracked`的骨骼数据。目前SDK最多可以追踪两幅骨骼。为了简化起见，本实例只对捕捉到的第一幅骨骼进行追踪和显示。
4. 在`Skeleton`对象的`Joints`属性集合中保存了所有骨骼点的信息，每个骨骼点的信息都是一个`Joint`对象。为了得到特定的骨骼点，同样使用LINQ语句对`Joint`的`JointType`属性进行筛选，相关代码如下：
```
Joint headJoint = (from j in skeleton.Joints
                   where j.JointType == JointType.Head
                   select j).FirstOrDefault();
```
在本实例程序中，需要遍历每个骨骼点，并分别对其进行处理。这里使用`foreach`语句来实现，并根据`JointType`属性进行处理。在`SetAllPointPosition()`函数中可以看到具体的实现细节。
```
foreach (Joint joint in skeleton.Joints)
{
    Point jointPoint = GetDisplayPosition(joint);
    switch (joint.JointType)
    {
        case JointType.Head:
            SetPointPosition(headPoint, joint);
            headPolyline.Points.Add(jointPoint);
            break;
            ...
    }
}
```
5. 前面提到，`Joint`的`Position`属性的X、Y、Z表示该骨骼点的三维位置，其中X和Y的范围都是-1～1，而Z是Kinect到识别物体的距离。
为了能更好地将这20个点显示出来，需要对`Position`的X值和Y值进行缩放，可以通过以下函数实现。
```
private Point GetDisplayPosition(Joint joint)
{
    var scaledJoint = joint.ScaleTo(640, 480);
    return new Point(scaledJoint.Position.X, scaledJoint.Position.Y);
}
```
上面语句中，`ScaleTo`函数的最后两个参数640和480分别代表原始数据X和Y的最大值，通过该语句可以将X坐标放大到0～640范围内的任意值，将Y坐标放大到0～480范围内的任意值。该坐标是相对于应用程序窗口的左上角(0,0)而言的，窗口的宽和高分别是640和480，以保证彩色图像和骨骼绘制的结果相匹配。
其中，`ScaleTo()`函数是`Coding4Fun`的`Help`类中的方法。`Coding4Fun`是一个Kinect开发辅助类库。读者可以从[http://c4fkinect.codeplex.com/](http://c4fkinect.codeplex.com/)下载该类库，并通过“Add Reference”菜单项将Coding4Fun.Kinect.Wpf.dll添加到项目中。
6. 编写一个函数，将每个骨骼点转换后的（X,Y）坐标值分别映射到相应的Ellipse控件的`Left`和`Top`属性上，其代码如下：
```
private void SetPointPosition(FrameworkElement ellipse, Joint joint)
{
    var scaledJoint = joint.ScaleTo(640, 480);
    Canvas.SetLeft(ellipse, scaledJoint.Position.X);
    Canvas.SetTop(ellipse, scaledJoint.Position.Y);
    SkeletonCanvas.Children.Add(ellipse);
}
```
使用`Polyline`类表示骨架线，显而易见，骨架由5条多段线组成，分别定义它们，并在遍历所有骨骼点时分类存储相应的点。详见`SetAllPointPosition()`函数，相关代码如下：
```
Polyline headPolyline = new Polyline();
Polyline handleftPolyline = new Polyline();
Polyline handrightPolyline = new Polyline();
Polyline footleftPolyline = new Polyline();
Polyline footrightPolyline = new Polyline();
private void SetAllPointPosition(Skeleton skeleton)
{
    SkeletonCanvas.Children.Clear();
    headPolyline.Points.Clear();
    handleftPolyline.Points.Clear();
    handrightPolyline.Points.Clear();
    footleftPolyline.Points.Clear();
    footrightPolyline.Points.Clear();
    foreach (Joint joint in skeleton.Joints)
    {
        Point jointPoint = GetDisplayPosition(joint);
        switch (joint.JointType)
        {
            case JointType.Head:
                SetPointPosition(headPoint, joint);
                headPolyline.Points.Add(jointPoint);
                break;
            case JointType.ShoulderCenter:
                SetPointPosition(shouldercenterPoint, joint);
                headPolyline.Points.Add(jointPoint);
                handleftPolyline.Points.Add(jointPoint);
                handrightPolyline.Points.Add(jointPoint);
                break;
            case JointType.ShoulderLeft:
                SetPointPosition(shoulderleftPoint, joint);
                handleftPolyline.Points.Add(jointPoint);
                break;
                ...
            case JointType.FootRight:
                SetPointPosition(footrightPoint, joint);
                footrightPolyline.Points.Add(jointPoint);
                break;
            default:
                ;
                break;
        }
    }
    headPolyline.Stroke = new SolidColorBrush(Colors.Blue);
    headPolyline.StrokeThickness = 5;
    SkeletonCanvas.Children.Add(headPolyline);
    handleftPolyline.Stroke = new SolidColorBrush(Colors.Blue);
    handleftPolyline.StrokeThickness = 5;
    SkeletonCanvas.Children.Add(handleftPolyline);
    handrightPolyline.Stroke = new SolidColorBrush(Colors.Blue);
    handrightPolyline.StrokeThickness = 5;
    SkeletonCanvas.Children.Add(handrightPolyline);
    footleftPolyline.Stroke = new SolidColorBrush(Colors.Blue);
    footleftPolyline.StrokeThickness = 5;
    SkeletonCanvas.Children.Add(footleftPolyline);
    footrightPolyline.Stroke = new SolidColorBrush(Colors.Blue);
    footrightPolyline.StrokeThickness = 5;
    SkeletonCanvas.Children.Add(footrightPolyline);
}
```
7. 运行程序，显示结果如图3所示。
![](http://www.ituring.com.cn/figures/2015/manong_game/084.png)
**图3　全身骨骼点运行结果**
8. 若要使用半身模式，只需在初始化`kinectSensor`对象时添加以下语句即可。
`kinectSensor.SkeletonStream.TrackingMode = SkeletonTrackingMode.Seated;`
运行结果如图4所示。
![](http://www.ituring.com.cn/figures/2015/manong_game/085.png)
**图4　半身模式运行结果**
由于RGB图像数据与深度图像数据（骨骼数据）的空间坐标系是不同的，前者的原点是RGB摄像头，后者的原点是红外摄像头，因此本实例中使用获取的骨骼点坐标直接绘制在RGB图像上会有相应的误差。若要修正这些误差，可以调用Kinect for Windows SDK提供的映射函数，将骨骼点坐标映射到RGB图像坐标上。具体做法为将上面用的ScaleTo函数替换为MapSkeletonPointToColorPoint，使用方法如下所示：
```
ColorImagePoint colorImagePoint =
    kinectSensor.CoordinateMapper.MapSkeletonPointToColorPoint
    (joint.Position, ColorImageFormat.RgbResolution640x480Fps30);
```
## **骨骼点旋转信息**
除了跟踪骨骼点的位置，Kinect SDK还能计算出骨骼点的旋转信息。这是Kinect SDK 1.5版本新增的功能，利用此功能可以计算出人体骨骼在Yaw轴的旋转情况，在此之前，仅通过骨骼点位置是无法实现此类计算的。根据相对参照系不同，旋转信息可以分为相对旋转信息和绝对旋转信息，这两种信息均包含了其旋转的矩阵参数和四元数参数。开发者可以使用这些数据方便地进行动作识别以及控制人形3D模型。
### **骨骼点旋转信息存储方式**
在Kinect SDK中，骨骼点旋转信息定义为`BoneOrientation`类，包含以下数据成员。
- 
StartJoint：起始骨骼点；
- 
EndJoint：结束骨骼点；
- 
HierarchicalRotation：相对旋转信息；
- 
AbsoluteRotation：绝对旋转信息。
在学习相对旋转信息和绝对旋转信息的具体含义之前，我们首先要定义骨骼点坐标系。对Kinect跟踪到的20个骨骼点进行分层：将“髋部中心”作为初始骨骼点，相邻的骨骼点逐层向下延伸，如图5所示。
![](http://www.ituring.com.cn/figures/2015/manong_game/086.png)
**图5　骨骼点分层图**
而骨骼点坐标系即为以该骨骼点为原点，以其上层骨骼点到它的直线方向为y轴正方向的坐标系，如图6所示。
![](http://www.ituring.com.cn/figures/2015/manong_game/087.png)
**图6　相对旋转信息**
其中，相对旋转信息就代表了一段骨骼中，起始骨骼点和结束骨骼点的两个坐标系之间的转移参数。相应地，绝对旋转信息代表了结束骨骼点坐标系和Kinect空间坐标系之间的转移参数，如图7所示。
![](http://www.ituring.com.cn/figures/2015/manong_game/088.png)
**图7　绝对旋转信息**
### **在骨骼数据回调函数中获取骨骼点旋转信息**
由于骨骼点旋转信息包含在骨骼数据流中，因此需要在骨骼数据的回调函数中获取相应的数据。在获取了一帧`SkeletonFrame`中的`SkeletonData`之后，可以使用下列代码读取骨骼点旋转信息：
```
foreach (Skeleton skeleton in this.skeletonData)
{
    if (skeleton.TrackingState == SkeletonTrackingState.Tracked)
    {
        foreach (BoneOrientation orientation in skeleton.BoneOrientations)
        {
            BoneRotation hierarchical = orientation.HierarchicalRotation;
            BoneRotation absolute = orientation.AbsoluteRotation;
        }
    }
}
```
可以看出，读取骨骼点旋转信息的方法和读取骨骼数据类似，其中`BoneRotation`类型的数据记录了旋转信息的矩阵和四元数。
### **综述**
虽然骨骼点旋转信息仅仅是依靠骨骼数据计算出来的，但是利用这一数据可以极其简便地完成人体姿态和动作的识别以及三维模型控制。不过需要注意的是，这里得到的旋转信息是由骨骼数据计算而来的原始数据，由于骨骼跟踪数据本身的抖动原因，旋转信息也会产生很大的噪声。因此，必须要先对其进行一定程度的降噪和平滑处理，才能在应用程序中使用。
## **小结**
本文主要介绍了骨骼追踪数据的结构，并结合实例3讲解了骨骼数据的获取和处理方式，实例4通过Kinect实现了对PPT播放的控制，这能够帮助读者更形象地理解如何利用骨骼追踪数据实现体感应用程序。另外，本文所介绍的半身模式和骨骼点旋转信息是Kinect最新版的SDK中新增的特性，半身模式支持只需上体姿势控制的应用；骨骼点旋转信息的加入有效地提高了特定姿势识别的准确度，这使得应用都更具真实感。
![](http://www.ituring.com.cn/figures/2015/manong_game/089.png)
Kinect是微软公司推出的最新的基于体感交互的人机交互设备。[《Kinect体感人机交互开发基础》](http://www.ituring.com.cn/book/965)分为3个部分，首先介绍了Kinect的结构和功能以及如何配置相关的开发环境，接着结合实例介绍如何使用Kinect for Windows SDK提供的API，最后通过4个实例详细讲述了使用Kinect for Windows SDK开发项目的实现过程。本文节选自[《Kinect体感人机交互开发基础》](http://www.ituring.com.cn/book/965)。

