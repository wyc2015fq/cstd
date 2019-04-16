# 使用 ChromaKey 滤镜进行抠图 - 博琼 - 博客园







# [使用 ChromaKey 滤镜进行抠图](https://www.cnblogs.com/hebeiDGL/p/3487236.html)









**简介**

Nokia Imaging SDK  1.0 中新提供的 ChromaKey 滤镜是一个神奇的滤镜，它的基本原理就是把

一个指定范围值内的颜色变为透明或半透明，比如下面的 demo 演示的，看上去第一张照片的两个人物的

拍摄地点由屋子里变成了沙滩：

![](https://images0.cnblogs.com/blog/348363/201312/23120111-65348683cba1421ca7c21f6ad32c83b3.jpg)

原理是用 ChromaKey 滤镜把第一张图片的背景色去除，然后用 Blend 滤镜，与第二张图片进行融合，显示结果就是

第三张图片。



由此可以预见，使用这个滤镜，如果背景素材丰富的话，可以提供各种有趣的效果。在其它场景的应用交互中，**这个滤镜也是**

**大有作为的。**





步骤：

1）首先在 VS 中新建一个名为  ChromaKeyFilterSample 的解决方案，并且通过 NuGet 添加 Imaging SDK 类库，

具体参照上一篇文章。



2）在工程的根目录下面添加两张图片，作为下面的处理过程的素材：

图片一：

![](https://images0.cnblogs.com/blog/348363/201312/23120826-623c7476a2104375ad9a61348ca3b7a2.jpg)



图片二：

![](https://images0.cnblogs.com/blog/348363/201312/23120838-09395e9aa627462f944809c658c00e82.jpg)



3）在 MainPage 的 xaml 页面，添加 三个 Image 控件，分别显示 两张原图和 结果：

```
<Image x:Name="pic1" HorizontalAlignment="Left" Height="192" Stretch="Fill" Margin="10,25,0,0" VerticalAlignment="Top" Width="202"/>
 <Image x:Name="pic2" HorizontalAlignment="Left" Height="192" Stretch="Fill" Margin="236,25,0,0" VerticalAlignment="Top" Width="200"/>
 <Image x:Name="imgResult" HorizontalAlignment="Left" Height="450" Margin="10,280,0,0" VerticalAlignment="Top" Width="450"/>
```


4）在页面的 Loaded 事件中，分别显示两张原图：

```
// 存储两张原图片的流
        Stream stream_pic_1;
        Stream stream_pic_2;
       async void MainPage_Loaded(object sender, RoutedEventArgs e)
        {
            // 获取 xap 包中的 图片
            stream_pic_1 = App.GetResourceStream(new Uri("pic_1.jpg", UriKind.Relative)).Stream;
            stream_pic_2 = App.GetResourceStream(new Uri("pic_2.jpg", UriKind.Relative)).Stream;

            BitmapImage bitmap_1 = new BitmapImage();
            bitmap_1.SetSource(stream_pic_1);
            pic1.Source = bitmap_1;

            BitmapImage bitmap_2 = new BitmapImage();
            bitmap_2.SetSource(stream_pic_2);
            pic2.Source = bitmap_2;

        }
```




显示效果：

![](https://images0.cnblogs.com/blog/348363/201312/23121532-5cf017736fed4036b035b80dcb52915a.jpg)

为了显示效果明显，把 MainPage 页面的背景色改为了淡蓝色 ： Background="AliceBlue"



5）继续在 MainPage_Loaded 页面中添加处理代码。在 PhotoShop 中，用拾色器查看 图片一

的背景色大致为 #6f9c87，然后给图片一运用 ChromaKeyFilter 滤镜：

```
stream_pic_1.Position = 0;
 stream_pic_2.Position = 0;

 var filters = new IFilter[]
 {
     //6f9c87
     new ChromaKeyFilter(Windows.UI.Color.FromArgb(0xff, 0x6f, 0x9c, 0x87), 0.3d),//颜色值 和 颜色范围     
 };

 WriteableBitmap writeableBitmap = new WriteableBitmap(450, 450);

// 给图片添加滤镜的管道过程
 using (StreamImageSource streamImageSource = new StreamImageSource(stream_pic_1))
 using (FilterEffect filterEffect = new FilterEffect(streamImageSource) { Filters = filters })
 using (var renderer = new WriteableBitmapRenderer(filterEffect, writeableBitmap, OutputOption.Stretch))
 {
     await renderer.RenderAsync();

     imgResult.Source = writeableBitmap;
 }
```


具体滤镜的使用可以参考[链接](http://www.cnblogs.com/hebeiDGL/p/3428013.html)



到这里，运行代码显示效果可以看出，图片一 的背景色去除了，显示出了页面的淡白色：

![](https://images0.cnblogs.com/blog/348363/201312/23122824-f785ecc8ace6417a935bd3d5ad899c5a.jpg)





6）这个步骤就是把上一步中，图片一 的处理结果 和 图片二进行融合，继续在 MainPage_Loaded 方法中，

把上面的这行代码注释掉：

     // imgResult.Source = writeableBitmap;




然后继续添加 Blend 滤镜。BlendFilter 的一个属性就是指定它的 BitmapImageSource 类型的前景图片，然后

指定两张图片的融合方式：



```
var filtersBlend = new IFilter[]
 {
     // 为 writeableBitmap.AsBitmap() 方法添加命名空间 Nokia.InteropServices.WindowsRuntime 
     // writeableBitmap 是上一步的处理结果
     new BlendFilter
     {
         ForegroundSource = new BitmapImageSource(writeableBitmap.AsBitmap()), 
         BlendFunction = BlendFunction.Normal 
     }
 };

 // 沙滩背景 的流
 stream_pic_2.Position = 0;
 using (StreamImageSource streamImageSource = new StreamImageSource(stream_pic_2))
 using (FilterEffect filterEffect = new FilterEffect(streamImageSource) { Filters = filtersBlend })
 using (var renderer = new JpegRenderer(filterEffect))
 {
     Windows.Storage.Streams.IBuffer buf = await renderer.RenderAsync();

     Stream stream = System.Runtime.InteropServices.WindowsRuntime.WindowsRuntimeBufferExtensions.AsStream(buf);
     BitmapImage bi = new BitmapImage();
     bi.SetSource(stream);

     imgResult.Source = bi;// 显示结果
 }
```



运行结果，可以看出，两张照片已经融合到了一起：
![](https://images0.cnblogs.com/blog/348363/201312/23123701-ec08fc0f7dfe4fc48144f5aba1ae4492.jpg)



到这里，这个神奇的滤镜的讲解就完成了。



7）因为在 Iamging SDK 中，可以通过 **ColorImageSource**类，初始化一个纯色背景的图片源，通过它再跟

第一张图片进行融合，就可以改变背景的颜色。把 **步骤 6） 上面添加 ”沙滩背景“ 的代码注释掉**，添加下面代码，

直接把蓝色背景换成了耀眼的红色：



```
// 纯色
 using (ColorImageSource colorImageSource = new ColorImageSource(new Windows.Foundation.Size(450, 450),
                                                                  Windows.UI.Color.FromArgb(150, 255, 0, 0)))
 using (FilterEffect filterEffect = new FilterEffect(colorImageSource) { Filters = filtersBlend })
 using (var renderer = new WriteableBitmapRenderer(filterEffect, writeableBitmap, OutputOption.Stretch))
 {
     await renderer.RenderAsync();

     imgResult.Source = writeableBitmap;
 }
```



其中的，下面这行代码的意思是初始化一张 450x450 的背景为红色的半透明的 ImageSource 对象：

```
ColorImageSource colorImageSource = new ColorImageSource(new Windows.Foundation.Size(450, 450),
                                                                             Windows.UI.Color.FromArgb(150, 255, 0, 0))
```



运行代码显示效果：

![](https://images0.cnblogs.com/blog/348363/201312/23133819-8b9c3f3e974f41a2a08f9246fc588f6f.jpg)





8）另外，在 Imaging SDK 中，还提供了一个  **RadialGradient**类，用它可以初始化一个渐变色的 ImageSource 对象，

注释掉 7）中的代码，用下面的代码给 图片一 添加一个紫色的渐变背景色：

```
// 渐变色
 var rad = new RadialGradient(new Windows.Foundation.Point(0.5, 0.9), new EllipseRadius(0.3, 0.3));
 rad.Stops = new GradientStop[] 
                         {
                             new GradientStop() { Color = Windows.UI.Color.FromArgb(255, 0, 0, 0), Offset = 0 },
                             //e70aef
                             new GradientStop() { Color = Windows.UI.Color.FromArgb(0xff, 0xe7, 0x0a, 0xef), Offset = 1 }
                         };

 using(var gradImageSource = new GradientImageSource(new Windows.Foundation.Size(450, 450), rad))
 using (FilterEffect filterEffect = new FilterEffect(gradImageSource) { Filters = filtersBlend })
 //using (FilterEffect filterEffect = new FilterEffect(gradImageSource))
 using (var renderer = new WriteableBitmapRenderer(filterEffect, writeableBitmap, OutputOption.Stretch))
 {
     await renderer.RenderAsync();

     imgResult.Source = writeableBitmap;
 }
```





工程运行效果：
![](https://images0.cnblogs.com/blog/348363/201312/23134648-e1443995a63e4c34bacd39ea314e50a4.jpg)





源代码下载：[http://pan.baidu.com/s/1sjpsHVR](http://pan.baidu.com/s/1sjpsHVR)





代码运行说明：

在运行源代码时，会出现一个编译错误： Nokia Imaging SDK does not support the AnyCPU target platform.

因为 Nokia Imaging SDK 支持托管代码和本地代码，所以在编译前需要进行设置：

1）在模拟器上运行时：菜单 -> 生成 -> 配置管理器 -> 活动解决方案平台 -> x86 2）在真机上运行时：  菜单 -> 生成 -> 配置管理器 -> 活动解决方案平台 -> ARM

更多有关说明请参考： [http://developer.nokia.com/Resources/Library/Lumia/#!nokia-imaging-sdk/adding-libraries-to-the-project.html](http://developer.nokia.com/Resources/Library/Lumia/#!nokia-imaging-sdk/adding-libraries-to-the-project.html)












