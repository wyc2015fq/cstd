# iphone各个型号屏幕分辨率总结 - xqhrs232的专栏 - CSDN博客
2019年02月22日 16:58:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：408
原文地址::[https://blog.csdn.net/amyloverice/article/details/79389357](https://blog.csdn.net/amyloverice/article/details/79389357)
iPhone：
iPhone 1G 320x480
iPhone 3G 320x480
iPhone 3GS 320x480
iPhone 4 640x960
iPhone 4S 640x960
iPhone 5 640x1136
iPhone 5S 640x1136
iPhone 5C 640x1136
iPhone 6 750x1334
iPhone 6 Plus 1080x1920 （开发应按照1242x2208适配）
iPhone 6S 750x1334
iPhone 6S Plus 1080x1920 （开发应按照1242x2208适配）
iPhone SE 640x1136
iPhone 7 750x1334
iPhone 7 Plus 1080x1920 （开发应按照1242x2208适配）
iPod Touch:
iPod Touch 1G 320x480
iPod Touch 2G 320x480
iPod Touch 3G 320x480
iPod Touch 4G 640x960
iPod Touch 5G 640x1136
iPad:
iPad 1 1024x768
iPad 2 1024x768
The New iPad 2048x1536
iPad mini  1024x768
iPad 4  2048x1536
iPad Air 2048x1536
iPad mini 2 2048x1536
iPad Air 2  2048x1536
iPad mini 3  2048x1536
iPad mini 4  2048x1536
iPad Pro  2732x2048
iPhone5在@2x屏幕尺寸是，640x1136px；对应的@1x，屏幕尺寸就是320x568px；
iPhone6在@2x屏幕尺寸是：750x1334px；对应的@1x，屏幕尺寸就是375x667px；
iPhone6Plus在@3x屏幕尺寸是：750x1334px；对应的@1x，屏幕尺寸就是414x736px
iPhone6Plus的@3x，iPhone6的@2x, iPhone5的@2x的介绍
iPhone6Plus是5.5英寸屏幕，1242x2208px-@3x的像素分辨率，逻辑分辨率是414x736pt-@1x。物理尺寸是1080x1920px。这个物理尺寸，也是安卓机目前最流行的大屏设计稿尺寸。
iPhone6是4.7英寸屏幕，750x1334px-@2x的像素分辨率，逻辑分辨率是375x667pt-@1x。
iPhone5是4英寸屏幕，640x1135px-@2x的像素分辨率，逻辑分辨率是320x568pt-@1x
【综上所述】iPhone6的原型规范如下：
界面尺寸布局：满屏尺寸750x1334px
高度电量条高度40px，导航栏高度88px，标签栏高度98px；
各区域图标大小导航栏图标44px，标签栏图标50px；
各区域文字大小电量条文字22px，导航栏-文字32px，标签栏字20px；
常用的文字大小：32px，30px，28px，26px，24px，22px，20px；
常用的颜色：背景浅灰色#f2f2f2,文字深黑色#323232，边框色深灰#CCCCCC；
常用可点击区域的高度：88px；
单行文字的背景框的高度：88px，双行则为：176px，三行则为：264px；
常用间距：亲密距离：20px；疏远距离：30px，其它距离：10px，44px等；
按钮和文本框，原型图做成直角的，圆角半径是多少，由Ui来设计；
这种情况，需要修改原型。单个页面的逻辑流程图或用户学习使用时间，超过其它页面平均数的3倍以上；
1.iPhone尺寸规格
设备
iPhone
宽
Width
高
Height
对角线
Diagonal
逻辑分辨率(point)
Scale Factor
设备分辨率(pixel)
PPI
3GS
2.4 inches (62.1 mm)
4.5 inches (115.5 mm)
3.5-inch
320x480
@1x
320x480
163
4(s)
2.31 inches (58.6 mm)
4.5 inches (115.2 mm)
3.5-inch
320x480
@2x
640x960
326
5c
2.33 inches (59.2 mm)
4.90 inches (124.4 mm)
4-inch
320x568
@2x
640x1136
326
5(s)
2.31 inches (58.6 mm)
4.87 inches (123.8 mm)
4-inch
320x568
@2x
640x1136
326
6
2.64 inches (67.1 mm)
5.44 inches (138.3 mm)
4.7-inch
375x667
@2x
750x1334
326
6+
3.07 inches (77.9 mm)
6.23 inches (158.2 mm)
5.5-inch
414x736
@3x
(1242x2208->)
1080x1920
401 
   iPhone SE 和 iPhone 5 的显示屏尺寸一致，4-inch display（对角线长）：
   iPhone 7 和iPhone 6(s) 的显示屏尺寸一致，4.7-inch display（对角线长）：
    iPhone 7 Plus 和 iPhone 6(s) Plus 的显示屏尺寸一致，5.5-inch display（对角线长）：
2.单位inch（英吋）
    1 inch = 2.54cm = 25.4mm
3.iPhone手机宽高
    上表中的宽高（width/height）为手机的物理尺寸，包括显示屏和边框。
    以下为iPhone4s的宽高示意图：
4.屏幕尺寸
    我们通常所说的iPhone5屏幕尺寸为4英寸、iPhone6屏幕尺寸为4.7英寸，指的是显示屏对角线的长度（diagonal）。
    以下为iPhone5~6+的屏幕尺寸规格示意图：
5.像素密度PPI
    PPI（Pixel Per Inch by diagonal）：表示沿着对角线，每英寸所拥有的像素（Pixel）数目。
    PPI数值越高，代表显示屏能够以越高的密度显示图像，即通常所说的分辨率越高、颗粒感越弱。
    根据勾股定理，可以得知iPhone4(s)的PPI计算公式为:
    计算结果稍有出入，这是因为像素的离散采样有锯齿效应。
6.缩放因子（scale factor between logic point and device pixel）
    （1）Scale起源
    早期的iPhone3GS的屏幕分辨率是320*480（PPI=163），iOS绘制图形（CGPoint/CGSize/CGRect）均以point为单位（measured in points）：
    1 point = 1 pixel（Point Per Inch=Pixel Per Inch=PPI）
    后来在iPhone4中，同样大小（3.5 inch）的屏幕采用了Retina显示技术，横、纵向方向像素密度都被放大到2倍，像素分辨率提高到(320x2)x(480x2)= 960x640（PPI=326）， 显像分辨率提升至iPhone3GS的4倍（1个Point被渲染成1个2x2的像素矩阵）。
    但是对于开发者来说，iOS绘制图形的API依然沿袭point(pt，注意区分印刷行业的“磅”)为单位。在同样的逻辑坐标系下（320x480）：
    1 point = scale*pixel（在iPhone4~6中，缩放因子scale=2；在iPhone6+中，缩放因子scale=3）。
    可以理解为：
    scale=绝对长度比（point/pixel）=单位长度内的数量比（pixel/point）
    （2）UIScreen.scale
    UIScreen.h中定义了该属性：
    // The natural scale factor associated with the screen.(read-only)
    @property(nonatomic,readonly) CGFloat scale  NS_AVAILABLE_IOS(4_0);
    --------------------------------------------------------------------------------
    This value reflects the scale factor needed to convert from the default logical coordinate space into the device coordinate space of this screen.
    The default logical coordinate space is measured using points. For standard-resolution displays, the scale factor is 1.0 and one point equals one pixel. For Retina displays, the scale factor is 2.0 and one point is represented by four pixels.
    --------------------------------------------------------------------------------
    为了自动适应分辨率，系统会根据设备实际分辨率，自动给UIScreen.scale赋值，该属性对开发者只读。
    （3）UIScreen.nativeScale
    iOS8新增了nativeScale属性：
    // Native scale factor of the physical screen
    @property(nonatomic,readonly) CGFloat nativeScale NS_AVAILABLE_IOS(8_0);
    以下是iPhone6+下的输出，初步看来nativeScale与scale没有太大区别：
    --------------------------------------------------------------------------------
        (lldb)p (CGFloat)[[UIScreen mainScreen] scale]
        (CGFloat) $1 = 3
        (lldb) p(CGFloat)[[UIScreen mainScreen] nativeScale]
        (CGFloat) $2 = 3
    --------------------------------------------------------------------------------
    （4）机型判别
    在同样的逻辑分辨率下，可以通过scale参数识别是iPhone3GS还是iPhone4(s)。以下基于nativeScale参数，定义了探测机型是否为iPhone6+的宏：
    --------------------------------------------------------------------------------
    // not UIUserInterfaceIdiomPad
    #define IS_IPHONE (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    // detect iPhone6 Plus based on its native scale
    #define IS_IPHONE_6PLUS (IS_IPHONE && [[UIScreenmainScreen] nativeScale] == 3.0f)
    --------------------------------------------------------------------------------
    那么，同样的分辨率和scale，如何区分机型iPhone4与4s、iPhone5与5s呢？通过[[UIDevice currentDevice] model]只能判别iPhone、iPad、iPod大类，要判断iPhone具体机型型号，则需要通过sysctlbyname("hw.machine")获取详细的设备参数信息予以甄别。
7.Resolutions &Rendering
8.@2x/@3x以及高倍图适配
    （1）@2x
    @2x means the same “double”retina resolution that we’veseen on all iOS devices with retina displays to date, where each virtual pointin the user interface is represented by two physical pixels on thedisplay in each dimension, horizontal and vertical.
    iPhone3GS时代，我们为一个应用提供图标（或按钮提供贴图），只需要icon.png。针对现在的iPhone4~6 Retina显示屏，需要制作额外的@2x高分辨率版本。
    例如在iPhone3GS中，scale=1，用的图标是50x50pixel（logicalimage.size=50x50point）；在iPhone4~6中，scale=2，则需要100×100pixel（logical image.size=50x50point，乘以image.scale=dimensions in pixels），并且命名为icon@2x.png。
    如果APP要同时兼容iPhone3GS~iPhone6，则需要提供icon.png/icon@2x.png两种分辨率的图片。
    （2）@3x
    @3x means a new “triple” retina resolution, where eachuser interface point is represented by three display pixels. A single @2x pointis a 2 × 2 square of 4 pixels; an @3x point is a 3 × 3 square of 9 pixels.”
    iPhone6+在实际渲染时，downsampling/1.15（1242x2208->1080x1920），准确的讲，应该是@2.46x。苹果为方便开发者用的是@3x的素材，然后再缩放到@2.46x上。
    参考：《为什么iPhone 6 Plus要将3x渲染的2208x1242分辨率缩小到1080p屏幕上？》《详解 iPhone 6 Plus 的奇葩分辨率》《iPhone 6 Plus屏幕分辨率》
    如果APP要同时兼容iPhone3GS~iPhone6+，则需要提供icon.png/icon@2x.png/icon@3x.png三种分辨率的图片。
    需要注意的是，iOS APP图标的尺寸和命名都需要遵守相关规范。
    （3）高倍图文件命名
    对于iPhone3、4/5/6、6+三类机型，需要按分辨率提供相应的高倍图并且文件名添加相应后缀，否则会拉伸（stretchable/resizable）失真（模糊或边角出现锯齿）。
    以下基于UIImage的两类初始化API简介高倍图的适配：
    <1>+imageNamed：该方法使用系统缓存，适合表视图重复加载图像的情形。同时该API根据UIScreen的scale，自动查找包含对应高倍图后缀名（@2x）的文件，如果找到二倍图，则image.scale=2.0，对应逻辑size大小以point度量（pixel度量的一半）；如果没找到设置默认image.scale=1.0，对应逻辑size大小同像素尺寸。因此，使用该方法，无需特意指定高倍图后缀。在实际运行时，系统如果发现当前设备是Retina屏（scale=2），会自动寻找"*@2x.png"命名格式的图片，加载针对Retina屏的图片素材，否则会失真。
    <2>+imageWithContentsOfFile/+imageWithData:(scale:)/-initWithContentsOfFile:/-initWithData:(scale:)
    这组方法创建的UIImage对象没有使用系统缓存，并且指定文件名必须包含明确的高倍图后缀。如果文件名包含@2x后缀，则image.scale=2.0；否则默认image.scale=1.0，同样对于Retina屏将会失真。
    <3>目前，适配iPhone6+时，除了一些铺满全屏的大图（LogoIcon、LaunchImage）需提供三倍图，其他的小图仍可沿用原有的二倍图自适应拉伸。
9.Screen Bounds & Application Frame
    （1）UIScreen.bounds
    // Bounds of entire screen in points（本地坐标系，起点为[0,0]）
    @property(nonatomic,readonly) CGRect bounds; 
    --------------------------------------------------------------------------------
    //考虑转屏的影响，按照实际屏幕方向（UIDeviceOrientation）的宽高
    #define SCREEN_WIDTH ([UIScreenmainScreen].bounds.size.width)
    #define SCREEN_HEIGHT ([UIScreenmainScreen].bounds.size.height)
    #define STATUSBAR_HEIGHT ([UIApplicationsharedApplication].statusBarFrame.size.height)
    //不考虑转屏的影响，只取竖屏（UIDeviceOrientationPortrait）的宽高
    #define SCREEN_WIDTH MIN([UIScreenmainScreen].bounds.size.width, [UIScreenmainScreen].bounds.size.height)
    #define SCREEN_HEIGHT MAX([UIScreenmainScreen].bounds.size.height, [UIScreenmainScreen].bounds.size.width)
    #define STATUSBAR_HEIGHT MIN([UIApplicationsharedApplication].statusBarFrame.size.width, [UIApplicationsharedApplication].statusBarFrame.size.height)
    --------------------------------------------------------------------------------
     （2）UIScreen.nativeBounds
    iOS8新增了nativeBounds属性，输出竖屏像素级分辨率：
    // The bounding rectangle of the physical screen,measured in pixels. (read-only)
    // This rectangle is based on the device in a portrait-up orientation. This value does not change as the device rotates.
    @property(nonatomic,readonly) CGRect nativeBounds NS_AVAILABLE_IOS(8_0);
    以下是iPhone6+下的输出：
    --------------------------------------------------------------------------------
       (lldb) poNSStringFromCGRect([(UIScreen*)[UIScreen mainScreen] bounds])
        {{0, 0}, {414, 736}}
        (lldb) poNSStringFromCGRect([(UIScreen*)[UIScreen mainScreen] nativeBounds])
        {{0, 0}, {1242, 2208}}
    --------------------------------------------------------------------------------
    （3）UIScreen.applicationFrame
    // Frame of application screen area in points (i.e.entire screen minus status bar if visible)
    // bounds除去系统状态栏
    @property(nonatomic,readonly) CGRect applicationFrame; 
    --------------------------------------------------------------------------------
    // APPFRAME_WIDTH=SCREEN_WIDTH
    #define APPFRAME_WIDTH ([UIScreen mainScreen].applicationFrame.size.width)
    // APPFRAME_HEIGHT=SCREEN_HEIGHT-STATUSBAR_HEIGHT
    //注意：横屏（UIDeviceOrientationLandscape）时，iOS8默认隐藏状态栏，此时APPFRAME_HEIGHT=SCREEN_HEIGHT
    #define APPFRAME_HEIGHT ([UIScreen mainScreen].applicationFrame.size.height)
    -------------------------------------------------------------------------------- 
    （4）bounds和frame的区别
    下图展示了bounds和frame的区别：
10.机型尺寸适配（Screen Scale Adaption）
    从iPhone3GS/iPhone4(s)过渡到iPhone5(s)时，在逻辑上宽度不变高度稍高，之前旧的素材和布局通过AutoresizingFlexible简单适配即可运行得很好，但由于高宽比增大，上下两端出现黑粗边（典型如LaunchImage）。从分辨率的角度来看，除了需要提供LaunchImage这种满屏图，其他基本沿用二倍图（@2x）；从屏幕尺寸角度来看，需要对纵向排版略加调整。
    从iPhone5(s)发展到iPhone6(+)，由于高宽比保持不变，iOS对图标、图片、字体进行等比放大自适应，清晰度会有所降低。同时，绝对坐标布局会导致在大屏下出现偏左偏上的问题。从分辨率的角度来看，iPhone6沿用二倍图（@2x），但需为iPhone6+提供更高的三倍图（@3x）；从屏幕尺寸角度来看，需要重新对UI元素尺寸和布局进行适配，以期视觉协调。
    （1）按宽度适配
    我们先来看一下iPhone4~6(+)的屏幕高宽比：
       iPhone4(s)：分辨率960*640，高宽比1.5
       iPhone5(s)：分辨率1136*640，高宽比1.775
       iPhone6：分辨率1334*750，高宽比1.779
       iPhone6+：分辨率1920*1080，高宽比1.778
    可粗略认为iPhone5(s)、6(+)的高宽比是一致的（16:9），即可以等比例缩放。因此可以按宽度适配：
        fitScreenWidth= width*(SCREEN_WIDTH/320)
    这样，共有iPhone3/4/5、6、6+三组宽度，在iPhone6、6+下将按比例横向放大。
    （2）按高度适配
    在同样的宽度下，iPhone4(s)的屏高比iPhone5(s)低，若纵向排版紧张，可以iPhone5(s)为基准，按高度适配：
        fitScreenHeight= height*(SCREEN_HEIGHT/568)
    共有iPhone3/4、5、6、6+四组高度，在iPhone3/4下将按比例纵向缩小，在iPhone6、6+下将按比例纵向放大。
    这里需要注意iPhone/iOS双环上网的热点栏对纵向布局的影响：iPhone作为个人热点且有连接时，系统状态栏下面会多一行热点连接提示栏"Personal Hotspot: * Connection"，纵向会下压20pt，[UIApplication sharedApplication].statusBarFrame高度变为40pt；当所有连接都断开时，热点栏消失，纵向高度恢复正常为20pt。详情可参考《iPhone/iOS开启个人热点的纵向适配小结》。
    （3）按字体适配
    另外，iPhone的【设置】【通用】【辅助功能】中可以设置调节【更大字体】，APP也可以按字号适配：
    例如适配表视图（UITableView:UIScrollView），无法左右滑动，因此无论字号缩放比例多大，横向都不应超过SCREEN_WIDTH。注意限定控件元素内容区域宽度以及间距，并设置适当的LineBreakMode。表视图支持上下滑动，因此纵向上的表格行高和内容区域高度可按字号缩放。
    对于纵向也不支持滑动的视图，在屏幕可见视区内排版时，最好不要随字号缩放，否则可能超出既定宽高。
11.UI相对布局
    考虑到iPhone机型的多样性，不可能针对iPhone4(s)、5(s)、6、6+四种屏幕尺寸出四套视觉交互稿，也不要基于某一机型从上往下、从左往右给绝对标注，而应该关注subView在superView中的相对位置（EdgeInsets/Frame/Center）以及siblingView之间的偏移（Offset），尽量给出适合Autolayout的相对布局比例（理想情况是只给百分比）。假如交互按照iPhone5(s)下绝对标注，则在iPhone4(s)上可能挤出屏幕底部，而在iPhone6(+)上则可能横向偏左或纵向偏上。
    开发人员基于与屏幕边缘的间距（Margin/EdgeInsets），定位边缘处的控件（钉钉子）作为参照，然后基于控件尺寸和间隙进行相对计算排版。这样，若钉子移动，相邻控件将顺向偏移，不会因为局部调整而出现凌乱。
    我们截取 iPhone5s QQ 文件助手列表中的文件cell，使用SketchMeasure 对其进行测量标注。
打开[截图.png]文件，由于不包含 Sketch 图元对象，无法进行 measure 标注（提示：请在画板中使用该功能）。因此，第一步需要对要相对标注的各个UI元素进行对象化。依次 Insert Artboard 创建图层 frame、thumbnail、title、detail、source、button。
选中整个cell的frame（bounds），进行 Measure size：width=640px（SCREEN_WIDTH），height=168px。默认横向尺寸和纵向参考线都居中导致标注重叠，将纵向参考线右移至合适位置；也可选中WIDTH标注图层中的text和label元素，在不移动参考线的前提下，利用鼠标局部移动标注字面量。
（1）缩略图标
在左侧边栏layer list中选中frame，再command选中文件类型缩略图对象thumbnail，进行 Measure spacing，丈量缩略图左侧相对frame的间距为24px。
选中文件类型缩略图对象thumbnail，进行 measure size，由于正方形等宽为112px*112px，故横纵标注重叠无影响。thumbnail在frame中纵向整体居中，因此上下边距计算均摊即可，无需再给定标注。
（2）传输按钮
对 button 和 frame 进行 Measure spacing，丈量按钮右侧相对frame的间距为24px。
选中按钮button进行 measure size，其宽高为144px*60px。横纵标注重叠影响视觉，将纵向参考线右移至合适位置。
button在frame中纵向整体居中，因此上下边距计算均摊即可，无需再给定标注。
（3）文件信息
对thumbnail和detail（title、source等宽且左对齐）进行 Measure spacing，丈量图标右侧相对detail的间距为20px；对button和detail进行 Measure spacing，丈量按钮左侧相对detail间距为20px。这样，左侧钉住thumbnail，右侧钉住button，中间信息部分的宽度无需给定，计算被动约束的横向余量即可。具体编程时，调用 sizeWithFont/boundingRectWithSize 可动态计算每行 label 的自然宽度，一般title都会超过约束宽度，因此需设置 lineBreakMode指定Wrapping省略或Truncating截断格式。
选中文件信息第1行标题title和第2行详情detail，进行 Measure spacing，测量纵向相对间距10px；第2行详情detail和第3行来源的纵向相对间距也为10px。一般 UILabel 的文本在给定字体下的纵向自然显示无约束，调用 sizeWithFont/boundingRectWithSize 可动态计算每行 label 的自然高度，title的上间距和source的底间距无需给定，通过计算纵向余量均摊即可。
    可简单的基于屏宽横纵比例进行scale缩放，将以上测量出的标注应用到iPhone6(+)大屏下，当然交互设计工程师最好还是针对特定机型都给定适配标注。苹果在WWDC2012 iOS6中已提出了Auto Layout的概念，即使用约束条件来定义视图的位置和尺寸，以适应不同尺寸和分辨率的屏幕。
12.DEPRECATED API适配
    最后，除了对屏幕尺寸和分辨率进行适配之外，还需对iOS SDK中相关的DEPRECATED API进行适配。典型的如：
    （1）UILineBreakMode->NSLineBreakMode
    （2）UITextAlignment->NSTextAlignment
    （3）sizeWithFont:->boundingRectWithSize:
    （4）stretchableImageWithLeftCapWidth:topCapHeight:->resizableImageWithCapInsets:
    （5）...
参考：
《iOS设备的屏幕尺寸、分辨率及其屏幕边长比例》
《iOS判断设备屏幕尺寸、分辨率》
《iOS8中的UIScreen》
《Detecting iPhone 6/6+ screen sizes in point values》
《iOS8已出，@3x图让我们何去何从？》
《在Xcode 6中用矢量化PDF(vectorized PDF)来支持各种尺寸的iPhone》
《iOS8适配须知》
《适配iOS8备忘录》
《iOS界面适配(一)(二)(三)》
《iPhone 6/6+适配心得》
《iOS8/Xcode6/iPhone6(+)适配》
《APP适配iOS8，iPhone6(+)截图简要说明》
《按比例快速兼容适配iPhone6/6 Plus》
《iOS的APP如何适应iPhone 5s/6/6+三种屏幕的尺寸？》
