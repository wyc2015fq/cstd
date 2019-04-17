# 深度相机（六）--Kinect v2.0 手势样本库制作 - 工作笔记 - CSDN博客





2016年09月25日 22:59:17[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：9065
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









目录为1.如何使用Kinect Studio录制手势剪辑；2.如何使用Visual Gesture Builder创建手势项目；3.如何在我的C#程序中使用手势；4.关于录制、剪辑手势过程中的注意事项


　　1.使用KInect Studio录制手势剪辑


　　　　1.1打开Kinect Studio这款程序，当你安装完成Kinect SDK 2.0后它的快捷方式会出现在你的开始菜单中。

　　该软件是主要的功能是监视、录制、回放，我们使用它的录制功能录制手势原文件。软件的具体使用方法我们就不说了，在这里我们会详细说明我们使用的步骤。首先，你需要了解它能记录的数据类型是Color、Audio、Infrared、Depth这四种，之后它会根据Infrared和Depth来生成BodyFrame、BodyIndex。就是说你虽然在录制的时候选择了使用BodyFrame和BodyIndex，但是他们并不是被记录下来了，而是后期生成的。所以我们制作手势的话，最少需要选择的源为：Infrared、Depth、BodyFrame和BodyIndex。尽量不要选择Color，因为它的数据量太大，对我们的骨节点识别和手势动作的分析没有任何作用。



![](https://img-blog.csdn.net/20160925224549833)

　　1.2 录制原始数据剪辑

　　当你使用这款程序时会发现，它能记录的信号源分为两种，一种是普通数据，它的扩展名为xef；另一种是原始数据，它的扩展名为xrf。这两种录制数据最大的不同就是骨架数据的保存，当你使用普通数据时，kinect在生成深度或骨架时，恰巧改变一些潜在算法，你的骨架数据可能会无效，而且录制之后就不能进行更新处理。当你使用原始数据时，就可以确保录制的剪辑时刻真实有效。但是，原始数据的文件大小会很大，而且在录制完成后还需要转换才能给Visual
 Gesture Builder来使用。

![](https://img-blog.csdn.net/20160925224553014)

　　打开方法：设置里打开AdvancedSteamdisplay，这个选项在设置的最下面。

　　然后你就会发现可以记录的选项从原先的11个变成了现在的15个，然后你只需要选中其中的NuiRawIR11bit就可以记录到原始数据了。

　　录制过程：略

　　1.3
 原始数据的转换


　　原始数据是不能够被Visual
 Gesture Builder直接使用的，使用命令提示符调出KSConvert程序，转换方法为KSConvert.exe路径路径


![](https://img-blog.csdn.net/20160925224556529)

路径输入好后回车就可以转换了，当它转换完成后会出现：

![](https://img-blog.csdn.net/20160925224600029)

该路径必须是一个没有中文的路径，即没有中文文件夹，文件名不能是中文（c:\User\文档\…也不可以），否则会转换失败。

转化完成后，你需要在Kinect Studio中再次检查身体框架记录的文件，Visual Gesture Builder无法使用没有身体框架的文件。

2.使用Visual Gesture Builder创建手势项目

　　2.1新建解决方案　　File/NewSolution

　　　　类比Visual Studio 中的解决方案，可以存放多个手势项目。

　　2.2新建手势项目

　　　　Create New Project

　　　　也可以使用向导创建

![](https://img-blog.csdn.net/20160925224602873)
- Gesture Name：手势的名字
- Body Side：手势是哪一面的

　　　　可以选择Any、Right、Lift，当选择Any时代表你的手势不区分左右，就是说你想要一个右手举过头顶的手势识别，但是它会默认认为你左手举过头顶也符合
- Gesture
 Type：手势类型　　Discrete（AdaBoostTrigger）离散　　Continuous（RFRProgress）连续

　　　　当你需要考虑一个手势的进度时，那你需要将手势做成连续的。除此之外你应该使用离散的手势处理
- Training
 Settings

　　　　一些需要用到或者忽略的设置

2.3关于分析项目

项目创建完成后，你会在解决方案文件夹下得到两个东西（训练项目和分析项目），那个.a后缀名的实际上是你的分析项目，或者说是测试项目

你放置在训练项目中的手势剪辑会实际起作用，当你最后生成一个手势库文件时它默认也只识别训练项目中的剪辑。那么这么说分析项目是不是就没有用了呢？

并不是这样的，分析项目可以进行分析测试你手势的情况。比如，你同一个动作录制了10个剪辑（相信我，10个剪辑只能算基础），你在训练项目中添加了8个项目并标记好，生成库文件就能作用到分析项目中（需要将剩下的两个剪辑添加到分析项目中），从而看到你的手势实际标记的情况，是否需要修改，是否需要添加新的手势剪辑。当你又录制了新的剪辑后，添加到训练项目中去并标记好，重新生成库，重新分析，我们就能清楚的看到识别的效果的变化。

注意：

不要在训练和测试中用一样的剪辑，原因是你需要一个新的剪辑来确定你的标记是否合理，而不是用自己证明自己

2.4 添加剪辑并标记

　　添加剪辑后，你双击剪辑就能进去添加标记了。

![](https://img-blog.csdn.net/20160925224606061)



如何添加标记：

　　使用方向左右键进行移动，Shift是选择键（等同于一直按下鼠标左键），Enter标记为真，空格键标记为假

如果希望退回去检查,可以按下Control键加箭头键快速定位关键帧

如果只标记真，你未做标记的会被自动标记为假

如果是连续手势，Enter标记为1，空格键标记为0，使用1~9可以标记0.1~0.9。比如我想在0~1中间的某个位置标记0.5（就是说在这个时候当前剪辑的人所做的手势已经完成了手势规定的50%），那么我就直接按下“5”就可以了。

2.5分析测试组

在.a后缀名的那个项目添加一个剪辑作为测试，然后右击分析项目→分析，选择你想分析的手势库（gba或gbd文件，如何生成gba或gbd我们后面介绍）

　　2.6实时预览

这个是比较直接的进行测试，不需要剪辑你直接在Kinect前进行演示就可以，同样需要生成好的手势库文件。

注意：当你只有一两个剪辑制作的姿势库的话，其实只有30-50%的置信值

　　2.7返回值

离散手势返回的结果有一个布尔值，表示是否做了那个手势；还有一个float值，是置信值，这个值能判定这个手势能否有效利用

连续手势只返回一个float值，这里是进度值，表示我在做的姿势距离数据库中的姿势的完成度还有多远。通常值为0-1，但是你也可以标记其他的值

2.8连续手势的自动标记

以上的内容大多是在讲离散手势的，下面可是接触比较复杂的连续手势，这里需要注意一下，平时使用时要记录的是一个瞬间的结果还是几个需要一定时间才能完成的结果，这样来选择你的手势是连续的还是离散的

平时使用时，你的手势80-90%都是离散的

离散手势：手势正在形成，或者没有

连续手势：追踪一个单一手势的进程，追踪动画或者与动画相连接

信号是一个可返回的浮点值，总是运行着

连续型手势中，没有置信值，会有一个进度值，为了判断它是否有效，你应该吧它和一个离散型手势连接在一起来作为判定依据，来测试确保其中一个手势是真实有效的

连续型手势需要标记那个进度值，从0-1中的数值，但是也可以使用自动标记。自动标记的使用方法：

1.用一个没有任何标记的新文件，先把它添加到分析里。分析中发现漏掉了它，意味着你的培训集合微弱，所以要强化培训。分析会捕捉它

干脆就把所有新的剪辑放进分析，做标记

然后，一旦标记完成，分析也已经运行起来了，然后我再决定我是否想把它留存在分析里，还是移动到培训里

2.选择该clip，移动到培训选项    Move To Training

3.相同文件名的clip在解决方案里，存在于相对应的状态离散手势分析项目里，并且已经被标记

4.右键点击你的状态连续手势分析项目，选择Generate Tags，当你有一个连续型手势时，它会打开手势对话的连接，在你的集合里现有的离散型手势中，哪些是你想对应的。进行指定标记的值的时候，需要注意指定这一状态的初始值及结束值。比如视频中的姐姐将左转、平行、右转三个离散手势和连续手势进行对应（注意：用到的必须是同一个手势剪辑）时，她将左转设置成0.5-0，平行设置成0.5，右转设置成0.5-1

![](https://img-blog.csdn.net/20160925224609045)

点击确定标记，此时程序将查看尚未进行标记的所有有与之对应手势的剪辑来进行连续手势的自动标记

此时，当你点击某一个剪辑时就可以看到它已经被自动标记了。你可以再做细微调整，或根据需要进行中间手势（中值）的标记

调整好后，你需要选择AcceptTags，之后它就会从粉红色变成蓝色，表示这个标记不再是自动生成的了，而是人工检查的

有时候连续型手势的自动标记的内容有些你并不想要，那就删除

Ctrl+Shift+Delete

2.9离散手势的自动标记

其实离散型手势也是有自动标记的，但是没有生成标记选项，说白了就是利用分析项目的分析功能。

这时需要你首先已经存在一个手势数据库，也就是说，当你需要录制一个需要成百上千个剪辑的手势时，你可以先手动标记至少前5个（视频中的姐姐说5个使比较合适的数字，稍微复杂杂一点的需要10个，很细微的小手势的话需要大概20个，具体为5-20个之间，听起来好想不靠谱，不过她好想很认真的样子），然后利用这5个生成的数据库来进行自动标记。具体操作为

1.右键点击你的离散手势分析项目，点击分析，之后会出现分析结果。

![](https://img-blog.csdn.net/20160925224613552)

2.在单个剪辑中，点击分析手势GenerateTags，点击生成标记（可以看到上图其实已经是已经标记好后再进行分析的，注意不要在已经人为标记过的剪辑内点击生成标记，通常认为标记要比分析更为精确）

通常情况下，分析取决于你手势的质量，不过他在检测手势时有一点滞后，而且检查手势确定完毕的用时稍微长了一些

当你的数据库变得更加精确时，就会在这个功能的作用下节省很多时间，当它不够精确时就很难分析出来

2.9生成手势库

　　直接右键解决方案，Build的是gbd文件。右键某个训练项目，Build的是这个训练项目的手势文件。

3.程序中使用手势

录制、剪辑、标记、生成等工作做完，我们就得到了一个后缀名为gbd的手势数据库的文件，那么我们的工作就算完成的差不多了，接下来就是将该文件加进我们的项目中，然后使用。接下来我们将在一个demo项目里进行演示

首先，在使用手势数据库之前我们需要做这几件事：

1.加入手势数据库

在加入手势数据库时需要注意，将gbd或gba文件添加到项目目录下，右键属性，将复制到输出目录选择为始终复制，生成操作选择为内容

2.引用类库

Microsoft.Kinect.VisualGestureBuilder.dll

这个时候需要注意，其实手势的这个类库是有两个的，在C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409\Redist\VGB这个文件夹下有X64、X86两个文件夹，两个类库分别为X64、X86的，X64的稍微多10多kb，实际使用我也不清楚有没有影响，但是最好用X86的那个。这个需要注意一下。此外，在使用其他Kinect类库时，你会发现也会有两个，那么我们在引用的时候将鼠标移动到名称上会出现该类库的路径，可以看到它时X64还是X86，默认上面的那个是X86.

![](https://img-blog.csdn.net/20160925224621748)

3.在debug文件夹下加入vgbtechs文件夹

需要添加C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409\Redist\VGB\x86下的vgbtechs文件夹

在使用Kinectface来做面部识别时也会需要一个特殊的文件夹，请知晓

以上是我们使用手势库的先决条件，如果你已经完成了，那我们就继续进行吧！

需要用到的是KinectSensor,BodyFrameReader,Body,VisualGestureBuilderFrameReader,VisualGestureBuilderFrameSource……

以下是代码，这个代码不能直接使用，只是作为演示。先指定路径，然后遍历里面的手势并添加给VGB帧源，然后在帧到达事件处理程序中和手势库中的手势进行比对，输出结果。



![复制代码](http://common.cnblogs.com/images/copycode.gif)

1 private readonly string gestureDatabase = @"Database\Steering.gbd"; 2 3 private VisualGestureBuilderFrameSource vgbFrameSource = null; 4 private VisualGestureBuilderFrameReader
 vgbFrameReader = null; 5 6 public GestureDetector(KinectSensor kinectSensor) 7 { 8 if (kinectSensor == null) 9 {10 throw new ArgumentNullException("kinectSensor");11 }12 this.vgbFrameSource = new VisualGestureBuilderFrameSource(kinectSensor, 0);13 this.vgbFrameReader
 = this.vgbFrameSource.OpenReader();14 if (this.vgbFrameReader != null)15 {16 this.vgbFrameReader.IsPaused = true;17 }18 using (var database = new VisualGestureBuilderDatabase(this.gestureDatabase))19 {20 this.vgbFrameSource.AddGestures(database.AvailableGestures);21
 }22 23 foreach (var gesture in this.vgbFrameSource.Gestures)24 {25 this.vgbFrameSource.SetIsEnabled(gesture, false);26 }27 }28 29 public void UpdateGestureData()30 {31 using (var frame = this.vgbFrameReader.CalculateAndAcquireLatestFrame())//计算并生成最新的VGB帧32
 {33 if (frame != null)34 {35 var discreteResults = frame.DiscreteGestureResults;36 var continuousResults = frame.ContinuousGestureResults;37 38 if (discreteResults != null)39 {40 foreach (var gesture in this.vgbFrameSource.Gestures)41 {42 if (gesture.GestureType
 == GestureType.Discrete)43 {44 DiscreteGestureResult result = null;45 discreteResults.TryGetValue(gesture, out result);46 47 }48 49 if (continuousResults != null)50 {51 if (gesture.GestureType == GestureType.Continuous)52 {53 ContinuousGestureResult result
 = null;54 continuousResults.TryGetValue(gesture, out result);55 }56 }57 }58 }59 }60 }61 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)



4.注意事项

　　官方视频里给的建议：
- 
保持手势简单
- 复杂的手势应被分解成多个部分

- 
避免过度标记
- 
标记为手势的核心部分，避免标记准备动作和恢复性动作，原因是
- 一部分对于机器学习算法，会产生混乱。需要帮助算法明白什么是重点
- 你在程序中使用的肯定不止一个手势，会是多个，那么你就需要移除任何可能引起冲突的东


- 
从静态姿势中分离手势
- 避免混合和匹配的框架，与动态手势

- 
保持一致
- 开始和结束点应该是相同的，在所有剪辑的一个手势

- 
验证标签
- 缺失或不正确的标签将混淆机器学习算法




　　我给自己的注意事项：

1.Kinect录制或采集的图像都是成像(镜像)的，所以你看到的效果可能是左右对调的。你只需要记住，并不是以Kinect的位置看问题，是以操作者的角度看的。好在，有标注



![](https://img-blog.csdn.net/20160925224627795)



2.在本文中会出现一些专有名词，有解决方案、离散手势项目、离散手势分析项目、连续手势项目、连续手势分析项目、剪辑，大多和VisualStudio2015中的名词是相对应的级别

3.当标记变为蓝色时，不管在什么情况下（分析、离散、连续等）都是被人工确认过的且被接受的，这时程序会默认你已经知晓它的可使用性

4.所谓的机器学习，其实也只是你给它足够的资料库让它来进行分析对比。所以你给的库必须严谨、正确，不能模棱两可，它可不会思考



