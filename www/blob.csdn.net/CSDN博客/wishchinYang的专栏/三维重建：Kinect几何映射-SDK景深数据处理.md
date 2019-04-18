# 三维重建：Kinect几何映射-SDK景深数据处理 - wishchinYang的专栏 - CSDN博客
2013年12月19日 18:27:12[wishchin](https://me.csdn.net/wishchin)阅读数：2247
         此文大量使用XML，非C类的代码，看看图即可。
         原文链接：[Kinect for Windows SDK开发入门(五)：景深数据处理](http://www.cnblogs.com/yangecnu/archive/2012/04/05/KinectSDK_Depth_Image_Processing_Part2.html)
### 3. 对物体进行测量
    像上篇文章中对深度值测量原理进行讨论的那样，像素点的X，Y位置和实际的宽度和高度并不一致。但是运用几何知识，通过他们对物体进行测量是可能的。每一个摄像机都有视场，焦距的长度和相机传感器的大小决定了视场角。Kinect中相机的水平和垂直视场角分别为57°和43°。既然我们知道了深度值，利用三角几何知识，就可以计算出物体的实际宽度。示意图如下：
![image](http://images.cnblogs.com/cnblogs_com/yangecnu/201204/201204050121028007.png)
    图中的公式在某些情况下可能不准确，Kinect返回的数据也有这个问题。这个简化的公式并没有考虑到游戏者的其他部分。尽管如此，公式依然能满足大部分的应用。这里只是简单地介绍了如何将Kinect数据映射到真实环境中。如果想得到更好的精度，则需要研究Kinect摄像头的焦距和摄像头的尺寸。
    在开始写代码前，先看看上图中的公式。摄像头的视场角是一个以人体深度位置为底的一个等腰三角形。人体的实际深度值是这个等腰三角形的高。可以将这个等腰三角形以人所在的位置分为两个直角三角形，这样就可以计算出底边的长度。一旦知道了底边的长度，我们就可以将像素的宽度转换为现实中的宽度。例如：如果我们计算出等腰三角形底边的宽度为1500mm，游戏者所占有的总象元的宽度为100，深度影像数据的总象元宽度为320。那么游戏者实际的宽度为468.75mm((1500/320)*100)。公式中，我们需要知道游戏者的深度值和游戏者占用的总的象元宽度。我们可以将游戏者所在的象元的深度值取平均值作为游戏者的深度值。之所以求平均值是因为人体不是平的，这能够简化计算。计算人物高度也是类似的原理，只不过使用的垂直视场角和深度影像的高度。
    知道了原理之后，就可以开始动手写代码实现了。先创建一个新的项目然后编写发现和初始化KinectSensor的代码，将DepthStream和SkeletonStream均初始化，然后注册KinectSnsor的DepthFrameReady事件。主UI界面中的代码如下：
```
<Window x:Class="KinectTakingMeasure.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        Title="MainWindow" Height="800" Width="1200" WindowStartupLocation="CenterScreen">
    <Grid>
        <StackPanel Orientation="Horizontal">
            <Image x:Name="DepthImage" />
            <ItemsControl x:Name="PlayerDepthData" Width="300" TextElement.FontSize="20">
                <ItemsControl.ItemTemplate>
                    <DataTemplate>
                        <StackPanel Margin="0,15">
                            <StackPanel Orientation="Horizontal">
                                <TextBlock Text="PlayerId:" />
                                <TextBlock Text="{Binding Path=PlayerId}" />
                            </StackPanel>
                            <StackPanel Orientation="Horizontal">
                                <TextBlock Text="Width:" />
                                <TextBlock Text="{Binding Path=RealWidth}" />
                            </StackPanel>
                            <StackPanel Orientation="Horizontal">
                                <TextBlock Text="Height:" />
                                <TextBlock Text="{Binding Path=RealHeight}" />
                            </StackPanel>
                        </StackPanel>
                    </DataTemplate>
                </ItemsControl.ItemTemplate>
            </ItemsControl>
        </StackPanel>
    </Grid>
</Window>
```
      使用ItemControl的目的是用来显示结果。方法创建了一个对象来存放用户的深度数据以及计算得到的实际宽度和高度值。程序创建了一个这样的对象数组。他是ItemControl的ItemsSource值。UI定义了一个模板用来展示和游戏者深度值相关的数据，这个模板使用的对象取名为PlayerDepthData。下面的名为ClaculatePlayerSize的方法将作为DepthFrameReady事件发生时执行的操作。
```java
private void KinectDevice_DepthFrameReady(object sender, DepthImageFrameReadyEventArgs e)
{
    using (DepthImageFrame frame = e.OpenDepthImageFrame()){
        if (frame != null) {
            frame.CopyPixelDataTo(this._DepthPixelData);
            CreateBetterShadesOfGray(frame, this._DepthPixelData);
            CalculatePlayerSize(frame, this._DepthPixelData);
        }
    }
}
private void CalculatePlayerSize(DepthImageFrame depthFrame, short[] pixelData)
{
    int depth;
    int playerIndex;
    int pixelIndex;
    int bytesPerPixel = depthFrame.BytesPerPixel;
    PlayerDepthData[] players = new PlayerDepthData[6];
    for (int row = 0; row < depthFrame.Height; row++){
        for (int col = 0; col < depthFrame.Width; col++){
            pixelIndex = col + (row * depthFrame.Width);
            depth = pixelData[pixelIndex] >> DepthImageFrame.PlayerIndexBitmaskWidth;
            if (depth != 0){
                playerIndex = (pixelData[pixelIndex] & DepthImageFrame.PlayerIndexBitmask) - 1;
                if (playerIndex > -1){
                    if (players[playerIndex] == null) {
                        players[playerIndex] = new PlayerDepthData(playerIndex + 1, depthFrame.Width, depthFrame.Height);
                    }
                    players[playerIndex].UpdateData(col, row, depth);
                }
            }
        }
    }
    PlayerDepthData.ItemsSource = players;
}
```
   粗体部分代码中使用了PlayerDepthData对象。CalculatePlayerSize方法遍历深度图像中的象元，然后提取游戏者索引位及其对应的深度值。算法忽略了所有深度值为0的象元以及游戏者之外的象元。对于游戏者的每一个象元，方法调用PlayerDepthData对象的UpdateData方法。处理完所有象元之后，将游戏者数组复制给名为PlayerDepthData的ItemControl对象的数据源。对游戏者宽度高度的计算封装在PlayerDepthData这一对象中。
    PlayerDepthData对象的代码如下：
```java
class PlayerDepthData
{
    #region Member Variables
    private const double MillimetersPerInch = 0.0393700787;
    private static readonly double HorizontalTanA = Math.Tan(57.0 / 2.0 * Math.PI / 180);
    private static readonly double VerticalTanA = Math.Abs(Math.Tan(43.0 / 2.0 * Math.PI / 180));
    private int _DepthSum;
    private int _DepthCount;
    private int _LoWidth;
    private int _HiWidth;
    private int _LoHeight;
    private int _HiHeight;
    #endregion Member Variables
    #region Constructor
    public PlayerDepthData(int playerId, double frameWidth, double frameHeight)
    {
        this.PlayerId = playerId;
        this.FrameWidth = frameWidth;
        this.FrameHeight = frameHeight;
        this._LoWidth = int.MaxValue;
        this._HiWidth = int.MinValue;
        this._LoHeight = int.MaxValue;
        this._HiHeight = int.MinValue;
    }
    #endregion Constructor
    #region Methods
    public void UpdateData(int x, int y, int depth)
    {
        this._DepthCount++;
        this._DepthSum += depth;
        this._LoWidth = Math.Min(this._LoWidth, x);
        this._HiWidth = Math.Max(this._HiWidth, x);
        this._LoHeight = Math.Min(this._LoHeight, y);
        this._HiHeight = Math.Max(this._HiHeight, y);
    }
    #endregion Methods
    #region Properties
    public int PlayerId { get; private set; }
    public double FrameWidth { get; private set; }
    public double FrameHeight { get; private set; }
    public double Depth
    {
        get { return this._DepthSum / (double)this._DepthCount; }
    }
    public int PixelWidth
    {
        get { return this._HiWidth - this._LoWidth; }
    }
    public int PixelHeight
    {
        get { return this._HiHeight - this._LoHeight; }
    }
    public string RealWidth
    {
        get
        {
            double inches = this.RealWidthInches;
return string.Format("{0:0.0}mm", inches * 25.4);
}
    }
    public string RealHeight
    {
        get
        {
            double inches = this.RealHeightInches;
            return string.Format("{0:0.0}mm", inches * 25.4);
}
    }
    public double RealWidthInches
    {
        get
        {
            double opposite = this.Depth * HorizontalTanA;
            return this.PixelWidth * 2 * opposite / this.FrameWidth * MillimetersPerInch;
        }
    }
    public double RealHeightInches
    {
        get
        {
            double opposite = this.Depth * VerticalTanA;
            return this.PixelHeight * 2 * opposite / this.FrameHeight * MillimetersPerInch;
        }
    }
    #endregion Properties
}
```
   单独编写PlayerDepthData这个类的原因是封装计算逻辑。这个类有两个输入点和两个输出点。构造函数以及UpdateData方法是两个输入点。ReadlWith和RealHeight两个属性是两个输出点。这两个属性是基于上图中的公式计算得出的。公式使用平均深度值，深度数据帧的宽度和高度，和游戏者总共所占有的象元。平均深度值和所有的象元是通过参数传入到UpdateData方法中然后计算的出来的。真实的宽度和高度值是基于UpdateData方法提供的数据计算出来的。下面是我做的6个动作的不同截图，右边可以看到测量值，手上拿了键盘用来截图。
![未标题-1](http://images.cnblogs.com/cnblogs_com/yangecnu/201204/20120405012114307.png)![未标题-2](http://images.cnblogs.com/cnblogs_com/yangecnu/201204/201204050121249807.png)
![未标题-3](http://images.cnblogs.com/cnblogs_com/yangecnu/201204/201204050121341541.png)![未标题-4](http://images.cnblogs.com/cnblogs_com/yangecnu/201204/201204050121482248.png)
![未标题-5](http://images.cnblogs.com/cnblogs_com/yangecnu/201204/201204050122031535.png)![未标题-6](http://images.cnblogs.com/cnblogs_com/yangecnu/201204/201204050122145297.png)
    以上测量结果只是以KinectSensor能看到的部分来进行计算的。拿上图1来说。显示的高度是1563mm，宽度为622mm。这里高度存在偏差，实际高度应该是1665左右，可能是脚部和头部测量有误差。以上代码可以同时测量6个游戏者，但是由于只有我一个人，所以做了6个不同的动作，截了6次图。还可以看到一点的是，如上面所讨论的，当只有一个游戏者时，游戏者索引值不一定是从1开始，从上面6幅图可以看出，进出视野会导致游戏者索引值发生变化，值是不确定的。
### 4.深度值图像和视频图像的叠加
       在之前的例子中，我们将游戏者所属的象元用黑色显示出来，而其他的用白色显示，这样就达到了提取人物的目的。我们也可以将人物所属的象元用彩色表示，而将其他部分用白色表示。但是，有时候我们想用深度数据中游戏者所属的象元获取对应的彩色影像数据并叠加到视频图像中。这在电视制作和电影制作中很常见，这种技术叫做绿屏抠像，就是演员或者播音员站在绿色底板前，然后录完节目后，绿色背景抠出，换成其他场景，在一些科幻电影中演员不可能在实景中表演时常采用的造景手法。我们平常照证件照时，背景通常是蓝色或者红色，这样也是便于选取背景颜色方便抠图的缘故。在Kinect中我们也可以达到类似的效果。Kinect
 SDK使得这个很容易实现。
      Note:这是现实增强的一个基本例子，现实增应用非常有趣而且能够获得非常好的用于体验。许多艺术家使用Kinect来进行现实增强交互时展览。另外，这种技术也通常作为广告和营销的工具。
      前面的例子中，我们能够判断哪个像素是否有游戏者。但是这个只能对于景深数据使用。不幸的是，景深数据影像的象元不能转换到彩色影像中去，即使两者使用相同的分辨率。因为这两个摄像机位于Kinect上的不同位置，所以产生的影像不能够叠加到一起。就像人的两只眼睛一样，当你只睁开左眼看到的景象和只睁开右眼看到的景象是不一样的，人脑将这两只眼睛看到的景物融合成一幅合成的景象。
      幸运的是，Kinect SDK提供了一些方法来方便我们进行这些转换，这些方法位于KinectSensor对象中，他们是MapDepthToColorImagePoint，MapDepthToSkeletonPoint，MapSkeletonPointToColor和MapSkeletonPointToDepth。在DepthImageFrame对象中这些方法的名字有点不同(MapFromSkeletonPoint,MapToColorImagePoint及MapToSkeletonPoint)，但功能是相似的。在下面的例子中，我们使用MapDepthToColorImagePoint方法来将景深影像中游戏者所属的象元转换到对应的彩色影像中去。细心的读者可能会发现，没有一个方法能够将彩色影像中的象元转换到对应的景深影像中去。
       创建一个新的工程，添加两个Image对象。第一个Image是背景图片。第二个Image是前景图像。在这个例子中，为了使景深影像和彩色影像尽可能的接近，我们采用轮询的方式。每一个影像都有一个Timestamp对象，我们通过比较数据帧的这个值来确定他们是否足够近。注册KinectSensor对象的AllFrameReady事件，并不能保证不同数据流产生的数据帧时同步的。这些帧不可能同时产生，但是轮询模式能够使得不同数据源产生的帧能够尽可能的够近。下面的代码展现了实现方式:
```java
private KinectSensor _KinectDevice;
private WriteableBitmap _GreenScreenImage;
private Int32Rect _GreenScreenImageRect;
private int _GreenScreenImageStride;
private short[] _DepthPixelData;
private byte[] _ColorPixelData;
private bool _DoUsePolling;
private void CompositionTarget_Rendering(object sender, EventArgs e)
{
    DiscoverKinect();
    if (this.KinectDevice != null){
        try{
            using (ColorImageFrame colorFrame = this.KinectDevice.ColorStream.OpenNextFrame(100)){
                using (DepthImageFrame depthFrame = this.KinectDevice.DepthStream.OpenNextFrame(100)){
                    RenderGreenScreen(this.KinectDevice, colorFrame, depthFrame);
                }
            }
        }
        catch (Exception){
            //Do nothing, because the likely result is that the Kinect has been unplugged.     
        }
    }
}
private void DiscoverKinect()
{
    if (this._KinectDevice != null && this._KinectDevice.Status != KinectStatus.Connected){
        UninitializeKinectSensor(this._KinectDevice);
        this._KinectDevice = null;
    }
    if (this._KinectDevice == null){
        this._KinectDevice = KinectSensor.KinectSensors.FirstOrDefault(x => x.Status == KinectStatus.Connected);
        if (this._KinectDevice != null){
            InitializeKinectSensor(this._KinectDevice);
        }
    }
}
private void InitializeKinectSensor(KinectSensor sensor)
{
    if (sensor != null) {
        sensor.DepthStream.Range = DepthRange.Default;
        sensor.SkeletonStream.Enable();
        sensor.DepthStream.Enable(DepthImageFormat.Resolution640x480Fps30);
        sensor.ColorStream.Enable(ColorImageFormat.RgbResolution1280x960Fps12);
        DepthImageStream depthStream = sensor.DepthStream;
        this._GreenScreenImage = new WriteableBitmap(depthStream.FrameWidth, depthStream.FrameHeight, 96, 96, PixelFormats.Bgra32, null);
        this._GreenScreenImageRect = new Int32Rect(0, 0, (int)Math.Ceiling(this._GreenScreenImage.Width), (int)Math.Ceiling(this._GreenScreenImage.Height));
        this._GreenScreenImageStride = depthStream.FrameWidth * 4;
        this.GreenScreenImage.Source = this._GreenScreenImage;
        this._DepthPixelData = new short[this._KinectDevice.DepthStream.FramePixelDataLength];
        this._ColorPixelData = new byte[this._KinectDevice.ColorStream.FramePixelDataLength];
        if (!this._DoUsePolling){
            sensor.AllFramesReady += KinectDevice_AllFramesReady;
        }
        sensor.Start();
    }
}
private void UninitializeKinectSensor(KinectSensor sensor)
{
    if (sensor != null){
        sensor.Stop();
        sensor.ColorStream.Disable();
        sensor.DepthStream.Disable();
        sensor.SkeletonStream.Disable();
        sensor.AllFramesReady -= KinectDevice_AllFramesReady;
    }
}
    以上代码有三个地方加粗。第一地方引用了RenderGreenScreen方法。第二个和第三个地方我们初始化了彩色和景深数据流。当在两个图像之间转换时，将彩色图形的分辨率设成景深数据的两倍能够得到最好的转换效果。
    RenderGreenScreen方法中执行实际的转换操作。首先通过移除没有游戏者的象元创建一个新的彩色影像。算法遍历景深数据的每一个象元，然后判断游戏者索引是否有有效值。然后获取景深数据中游戏者所属象元对应的彩色图像上的象元，将获取到的象元存放在象元数组中。代码如下：
private void RenderGreenScreen(KinectSensor kinectDevice, ColorImageFrame colorFrame, DepthImageFrame depthFrame)
{
  if (kinectDevice != null && depthFrame != null && colorFrame != null){
    int depthPixelIndex;
    int playerIndex;
    int colorPixelIndex;
    ColorImagePoint colorPoint;
    int colorStride = colorFrame.BytesPerPixel * colorFrame.Width;
    int bytesPerPixel = 4;
    byte[] playerImage = new byte[depthFrame.Height * this._GreenScreenImageStride];
    int playerImageIndex = 0;
    depthFrame.CopyPixelDataTo(this._DepthPixelData);
    colorFrame.CopyPixelDataTo(this._ColorPixelData);
    for (int depthY = 0; depthY < depthFrame.Height; depthY++){
      for (int depthX = 0; depthX < depthFrame.Width; depthX++, playerImageIndex += bytesPerPixel){
        depthPixelIndex = depthX + (depthY * depthFrame.Width);
        playerIndex = this._DepthPixelData[depthPixelIndex] & DepthImageFrame.PlayerIndexBitmask;
        if (playerIndex != 0){
          colorPoint = kinectDevice.MapDepthToColorImagePoint(depthFrame.Format, depthX, depthY, this._DepthPixelData[depthPixelIndex], colorFrame.Format);
          colorPixelIndex = (colorPoint.X * colorFrame.BytesPerPixel) + (colorPoint.Y * colorStride);
           playerImage[playerImageIndex] = this._ColorPixelData[colorPixelIndex];         //Blue    
           playerImage[playerImageIndex + 1] = this._ColorPixelData[colorPixelIndex + 1];     //Green
           playerImage[playerImageIndex + 2] = this._ColorPixelData[colorPixelIndex + 2];     //Red
           playerImage[playerImageIndex + 3] = 0xFF;                                          //Alpha
          }
        }
      }
     this._GreenScreenImage.WritePixels(this._GreenScreenImageRect, playerImage, this._GreenScreenImageStride, 0);
    }
}
```
     PlayerImage位数组存储了所有属于游戏者的彩色影像象元。从景深数据对应位置获取到的彩色影像象元的大小和景深数据象元大小一致。与景深数据每一个象元占两个字节不同。彩色影像数据每个象元占4个字节，蓝绿红以及Alpha值各占一个字节，在本例中Alpha值很重要，它用来确定每个象元的透明度，游戏者所拥有的象元透明度设置为255(0xFF)不透明而其他物体则设置为0，表示透明。
    MapDepthToColorImagePoint方法接受景深象元位置以及深度值，返回对应的对应彩色影像中象元的位置。剩下的代码获取游戏者在彩色影像中的象元并将其存储到PlayerImage数组中。当处理完所有的景深数据象元后，代码更新Image的数据源。运行程序后，需要站立一段时间后人物才能够显示出来，如果移动太快，可能出来不了，因为景深数据和彩色数据不能够对齐，可以看到任务轮廓有一些锯齿和噪声，但要处理这些问题还是有点麻烦的，它需要对象元进行平滑。要想获得最好的效果，可以将多帧彩色影像合称为一帧。运行程序后结果如下图，端了个键盘，人有点挫：
![xiaoguo](http://images.cnblogs.com/cnblogs_com/yangecnu/201204/201204050122395404.png)
### 5.结语
    本文首先介绍了关于景深数据的简单图像数据，包括景深数据的直方图显示以及一些图像处理相关的算法，然后介绍了景深数据中的游戏者索引位，借助索引位，我们实现了人物宽度和高度的计算，最后借助景深数据结合彩色影像数据，将景深影像和视频图像进行了叠加。
    至此，景深数据处理介绍完了，后面将会开始介绍Kinect的骨骼追踪技术，敬请期待。
    点击[此处](http://files.cnblogs.com/yangecnu/KinectSDK_Depth_Image_Processing_Part2.rar)下载本文所有代码，希望对您了解Kinect SDK有所帮助！
