# 关于 AlphaBlend 和 32bpp 的反锯齿图标 - xqhrs232的专栏 - CSDN博客
2012年11月01日 21:11:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1099
原文地址::[http://www.haogongju.net/art/840107](http://www.haogongju.net/art/840107)
相关网帖
1.DirectDraw版的AlphaBlend----[http://www.haogongju.net/art/684371](http://www.haogongju.net/art/684371)

在早前的一篇[文章](http://www.haogongju.net/tag/%E6%96%87%E7%AB%A0)中我曾经[研究](http://www.haogongju.net/tag/%E7%A0%94%E7%A9%B6)过带有
 alpha 通道的[图标](http://www.haogongju.net/tag/%E5%9B%BE%E6%A0%87)，实际上 [XP](http://www.haogongju.net/tag/XP)[系统](http://www.haogongju.net/tag/%E7%B3%BB%E7%BB%9F)已经开始支持这样的图标，也就是32
 bpp（b[it](http://www.haogongju.net/tag/IT)s per pixel）的图标了。在本文最后给出的MSDN[链接](http://www.haogongju.net/tag/%E9%93%BE%E6%8E%A5)中[可以](http://www.haogongju.net/tag/%E5%8F%AF%E4%BB%A5)介绍[开发者](http://www.haogongju.net/tag/%E5%BC%80%E5%8F%91%E8%80%85)如何创建
 32 bpp 的图标，[不过](http://www.haogongju.net/tag/%E4%B8%8D%E8%BF%87)不幸的是，VS[开发](http://www.haogongju.net/tag/%E5%BC%80%E5%8F%91)环境不支持[编辑](http://www.haogongju.net/tag/%E7%BC%96%E8%BE%91)这样的图标，而且原生的Photoshop也不支持（尽管有ICO格式[插件](http://www.haogongju.net/tag/%E6%8F%92%E4%BB%B6)），只能借助其他专业的图标[制作](http://www.haogongju.net/tag/%E5%88%B6%E4%BD%9C)[工具](http://www.haogongju.net/tag/%E5%B7%A5%E5%85%B7)，同样不幸的是，其他图标制作工具我用的并不顺手（至少没有PS那样熟练），所以我只能借助
 Photoshop 和图标制作工具两者同时使用，从而可以完成制作 32 bpp 图标。
（一）关于 AlphaBlend；
　　　　AlphaBlend 是[提供](http://www.haogongju.net/tag/%E6%8F%90%E4%BE%9B) alpha 通道的贴图的 [API](http://www.haogongju.net/tag/API) 函数，即每个像素都带有一个独立的
 alpha 值，去指定该像素在最终合成结果中占据的比例，其[作用](http://www.haogongju.net/tag/%E4%BD%9C%E7%94%A8)就相当于Photoshop中的图层蒙版。这是 Msimg32.dll 中提供的函数，因此[我们](http://www.haogongju.net/tag/%E6%88%91%E4%BB%AC)要使用这个函数，用以下语句指定
 lib [文件](http://www.haogongju.net/tag/%E6%96%87%E4%BB%B6)（或在[项目](http://www.haogongju.net/tag/%E9%A1%B9%E7%9B%AE)属性中添加）：
#[pr](http://www.haogongju.net/tag/PR)agma [com](http://www.haogongju.net/tag/com)ment(lib,
 "Msimg32.lib")
　　　　这个函数是从一个 HDC 拷贝到另一个 HDC，选入DC的[图片](http://www.haogongju.net/tag/%E5%9B%BE%E7%89%87)需要具有4个通道，即按照地址从低到高的顺序是B，G，R，A。在提供 alhpa通道的[图片格式](http://www.haogongju.net/tag/%E5%9B%BE%E7%89%87%E6%A0%BC%E5%BC%8F)里比较[常用](http://www.haogongju.net/tag/%E5%B8%B8%E7%94%A8)的是PNG格式，在
 .[net](http://www.haogongju.net/tag/net) 里，GDI+可以从PNG图片加载出一个位图，然后用 Graphics.Draw[Image](http://www.haogongju.net/tag/Image) 就可以看到合成的效果了，使用起来是非常简单的。在
 C++ 中的贴图则分为数种（BitBlt，StretchBlt，PlgBlt，MaskBlt，TransparentBlt，AlphaBlend ...），前面大部分都是完整像素的传送和位操作，要实现两个像素的 alpha 合成，显然要使用的是 AlphaBlend，由于这个函数在调用前还有一个特殊要求（如果你没有[注意](http://www.haogongju.net/tag/%E6%B3%A8%E6%84%8F)到这个要求，则很可能会对调用结果感到困惑），它在MSDN中的位置比较隐晦，是在介绍其最后一个参数
 BLENDFUNCTION 中的一个成员（Alpha[For](http://www.haogongju.net/tag/for)mat）的取值时提到的：
　　　　这个要求是: 在调用 AlhpaBlend 之前，图片的 alpha 通道[必须](http://www.haogongju.net/tag/%E5%BF%85%E9%A1%BB)先[应用](http://www.haogongju.net/tag/%E5%BA%94%E7%94%A8)到
 RGB 通道上（premultipl[ie](http://www.haogongju.net/tag/IE)d alpha）。
　　　　在之前我的文章中已给出了对一个 CImage 对象应用 alpha 通道的代码。这里我们不[继续](http://www.haogongju.net/tag/%E7%BB%A7%E7%BB%AD)给出他的代码，现在假设我把一个PNG[存储](http://www.haogongju.net/tag/%E5%AD%98%E5%82%A8)为32bpp的BMP
 位图，然后可以从文件加载或者添加到PE文件的[资源](http://www.haogongju.net/tag/%E8%B5%84%E6%BA%90)里面。然后就可以使用 LoadImage 从文件加载或者用 LoadBitmap 从资源加载了，如果从文件加载，必须指定一个标志：LR_CR[EA](http://www.haogongju.net/tag/EA)TEDIB[SEC](http://www.haogongju.net/tag/SEC)TION。
　　　　假设从资源中加载了这个位图，我们可以用下面的代码，预先应用 alhpa 通道：
代码
　　　　下图可直观的看到应用前和应用后的图片的区别，左侧是原始位图，其右侧是应用了Alpha通道后的位图，这就是在调用 AlphaBlend 之前应该选入Src DC的位图。虽然我们可以在运行时再去应用 alpha 通道，但是假如图片是已经确定的话（例如添加到PE的资源），那么我们为什么不先把比较消耗精力的这一步事先做掉呢？
![](http://pic002.cnblogs.com/images/2010/22214/2010100723490889.jpg)
　　　　上面的代码中，加载位图以后，为了[修改](http://www.haogongju.net/tag/%E4%BF%AE%E6%94%B9)像素，我使用 GetDIBits
 函数去[获取](http://www.haogongju.net/tag/%E8%8E%B7%E5%8F%96)像素[数据](http://www.haogongju.net/tag/%E6%95%B0%E6%8D%AE)的一个拷贝，为此需要提供一个HDC，如果不提供HDC，就无法得到位图的像素（这让我有点不[理解](http://www.haogongju.net/tag/%E7%90%86%E8%A7%A3)，我只想得到DIB的数据而已，不关心设备，但是API一定要求提供HDC有点不合情理），所以这里先获取程序的主窗口的HDC，得到位图数据然后我们把
 alhpa 通道数据应用到 RGB 通道上。显然这样处理以后，图片会比原来的“变暗”[一些](http://www.haogongju.net/tag/%E4%B8%80%E4%BA%9B)，图像中完全透明的部分在结果中将完全变成黑色，半透明的部分会有所变暗。这样处理的要求可能是 API
 觉得这样它的计算量可以有所减少（实际上也没少到哪里去），因为RGB通道可以直接和背景色被加权后（1- alpha/255) 的结果做加法。
　　　　这样处理后的位图就可以选入DC，然后调用 AlhpaBlend 函数了。这个函数的参数列表几乎和 TransparentBlt 一致，所以如果你的代码中使用了 TransparentBlt 则前面的参数无须改动，只需注意这个函数最后一个参数是 BLENDFUNCTION，从名称看仿佛是混合函数，实际上它只是一个含有4个整数的结构体而已。这个参数的设置决定了如何合成。SourceConstantAlpha
 提供了整体的 Alpha 值，相当于PS中的图层不透明度，它是作用于整体的。Alpha 通道则相当于PS中的蒙版，[控制](http://www.haogongju.net/tag/%E6%8E%A7%E5%88%B6)每个像素的参与合成比例，是一对一的作用在每个像素上的。关于成员的设置可以参考MSDN。下面就是使用
 AlhpaBlend 的代码：
代码
　　　　效果如下图所示，这是把图像在纯色背景上合成的结果，可以看出图像的半透明部分（例如底部阴影）受到的背景色[影响](http://www.haogongju.net/tag/%E5%BD%B1%E5%93%8D)。如果背景是其他图片，则可以看到两个图片合成的结果。
![](http://pic002.cnblogs.com/images/2010/22214/2010100722505383.jpg)
（二）适用于XP系统中的反锯齿图标。
　　　　关于 AlphaBlend 就简单说到这里。AlphaBlend 可以使图片完美的融入到[任何](http://www.haogongju.net/tag/%E4%BB%BB%E4%BD%95)的背景中，而不必关心背景的[内容](http://www.haogongju.net/tag/%E5%86%85%E5%AE%B9)，这是其最大的优点。图标同样具有这个要求，图标的格式是在 [Windows](http://www.haogongju.net/tag/Windows) 的早期[定义](http://www.haogongju.net/tag/%E5%AE%9A%E4%B9%89)的，那时候的设备条件也远没有发展到现在的地步，所以那时定义的图标，只需要定义透明部分，不透明部分就够了，所以图标的
 MASK 基于节省存储[空间](http://www.haogongju.net/tag/%E7%A9%BA%E9%97%B4)的考虑被定义成了单色位图（二元图）。然后到了XP[时代](http://www.haogongju.net/tag/%E6%97%B6%E4%BB%A3)，随着硬件水平改善，CPU的运算[速度](http://www.haogongju.net/tag/%E9%80%9F%E5%BA%A6)，显示器的表达颜色数量，存储设备的空间都大大提高，对 [UI](http://www.haogongju.net/tag/UI) 美观性的要求又开始突出起来，仅提高图标图片本身的颜色数量（从16色，256色仅仅提高到24bpp）还不够，于是
 XP 引入了带有 alpha 的 32 bpp图标，图标格式的定义没有改变，由于32 bpp 图标具有 alpha 通道，所以其mask数据块已经变得可有可无了，因为其作用完全可以由 alpha 通道提供。实际上从更合理的角度考虑，图标中的每个图片都有一个图片信息头，在图标的图片信息头中，再提供一个 mask 的 bpp 数据（而不是像现在这样一律假定bpp为1），将会比现在的格式定义更完美。显然，使用带有 alpha 通道的图标绘制要比之前的简单图标工作量更大，原来只需要两次位操作性质的数据块传送，效率很高，而
 Alpha 合成则无法整块传送，需逐点合成。
　　　　现在的 XP 系统中大量的应用了 32 BPP 的图标，这些图标通常带有渐隐的阴影效果，可以完美融合在背景中，因此也称作反锯齿图标。而早期的图标文件的像素要么透明，要么完全可见，因此是能够看出锯齿痕迹的。下面就是这两种图标的显示效果的区别：
![](http://pic002.cnblogs.com/images/2010/22214/2010100723102310.jpg)
　　　　上面的是 32 bpp 的图标，可以看到它在任意背景上都能呈现视觉感舒适的阴影效果，而下面的是普通图标，没有办法提供完美的阴影效果。现在商业[软件](http://www.haogongju.net/tag/%E8%BD%AF%E4%BB%B6)[基本](http://www.haogongju.net/tag/%E5%9F%BA%E6%9C%AC)都提供了这种反锯齿的
 32bpp 图标，比如qq等等。
[window](http://www.haogongju.net/tag/window)s为了适应多种硬件条件考虑，[建议](http://www.haogongju.net/tag/%E5%BB%BA%E8%AE%AE)图标含有多个图片，首先按照颜色数量（BPP）从低到高排列，然后再同一个BPP中按照尺寸从大到小排列，系统在显示时从图标中按照一些原则（具体准则请参考MSDN）去抽取图标最[适合](http://www.haogongju.net/tag/%E9%80%82%E5%90%88)的图片。对
 32 BPP的图标，系统的建议是提供 3 种典型尺寸（48*48，32*32，16*16），尺寸24*24一般是在开始[菜单](http://www.haogongju.net/tag/%E8%8F%9C%E5%8D%95)上使用，属于可选的，三种典型BPP（16色，256色，真彩色（bpp
 = 24或32））。因此要在程序中使用反锯齿图标，我们通常至少需要 9 幅图片。
　　　　但是在[IDE](http://www.haogongju.net/tag/IDE)中我们没法编辑这样的图标，注意如果导入了32bpp的图标到IDE中也决不能用IDE去做任何修改！否则很可能损坏其显示效果。因此我们需要借助[专用](http://www.haogongju.net/tag/%E4%B8%93%E7%94%A8)的图标制作工具。这里我使用的是：[Icon](http://www.haogongju.net/tag/icon)Workshop，这是老外编写的（一般看起来比较好比较强大比较[细心](http://www.haogongju.net/tag/%E7%BB%86%E5%BF%83)的东西的作者不可能是[中国](http://www.haogongju.net/tag/%E4%B8%AD%E5%9B%BD)人，这是一个[规律](http://www.haogongju.net/tag/%E8%A7%84%E5%BE%8B)。。。），网络有[免费](http://www.haogongju.net/tag/%E5%85%8D%E8%B4%B9)的汉化版本[下载](http://www.haogongju.net/tag/%E4%B8%8B%E8%BD%BD)。但是可能是我不熟悉的缘故，它的编辑[功能](http://www.haogongju.net/tag/%E5%8A%9F%E8%83%BD)我用的不惯，所以我先在
 Photoshop 中编辑图片，然后存储为 PNG 格式，再用 IconWorkshop 打开PNG，然后就是复制粘贴，IconWorkshop 有个很方便的功能，可以自动的以现有图片为基础产生所有其他大小和BPP的图片，然后再保存为 ICO 格式就可以了。
　　　　假如不自己制作，也可以借助工具（例如ICONPRO）从系统的 [shell](http://www.haogongju.net/tag/shell)32.dll
 中导出一些图标，把这样的图标添加到资源中，然后可以绘制他们，但是要注意，不能用常规的[方法](http://www.haogongju.net/tag/%E6%96%B9%E6%B3%95)去做，否则绘制出来的图标是看不到反锯齿效果的。正确的方法是用 LoadImage 指定 LR_CREATEDIBSECTION
 去加载图标，然后用 DrawIconEx 去指定大小的去绘制。代码如下：
代码
　　　　绘制的效果如下所示，这个图标就是我从 Shell32.dll 中抽取的图标，可以看到其柔和的阴影，我把图标绘制在比真正尺寸少许放大的白色矩形上，这样可以很[容易](http://www.haogongju.net/tag/%E5%AE%B9%E6%98%93)看出图标的尺寸大概有多大，这些图片都是图标实际提供的（而不是从缩放得到），因此能够[保证](http://www.haogongju.net/tag/%E4%BF%9D%E8%AF%81)[设计](http://www.haogongju.net/tag/%E8%AE%BE%E8%AE%A1)时的效果：
![](http://pic002.cnblogs.com/images/2010/22214/2010100723295712.jpg)
　　　　反锯齿的图标也可以放在 Static 图形控件中，显示效果是同样的，当然我们要先借助其他工具制作出反锯齿的图标。实际上不仅仅是图标，还有很多位置的小位图，比如自定义绘制菜单项左侧的小位图，各种TreeView，ListView等控件中使用的小位图，可能都是用 32 BPP 的图片通过 alpha 合成来实现的，这样就可达到更柔和更美观的显示效果。
（三）我所编写的范例程序；
　　　　（1）我写了一个小范例程序用来演示 AlphaBlend 的效果，同时也可读取一个32BPP的图片（BMP或者PNG格式），然后保存为应用了Alpha通道后的BMP格式位图。这样的结果图片就可以直接添加到项目资源中，然后应用到Alpha合成的场合。这个工具的主要代码在文中已经提供，也没有什么[技术](http://www.haogongju.net/tag/%E6%8A%80%E6%9C%AF)含量，因此不再提供源码下载。
　　　　（2）我写的另一个小程序用来打开一个ICO文件，然后可以展示图标中每个图像的XOR Mask，AND Mask， XOR中的Alpha通道（仅32bpp具有）部分。右下角是通过加载图标的方式绘制的图标。通过上方的组合框可以切换图片中的图像。这个工具还有一个功能，可以把图标的当前图像另存为 BMP 格式的位图文件（然后就可以在Photoshop中观察其 Alpha 通道）。通过这个工具，主要可以观察 32 bpp 的图标的 Alpha 通道，以帮助理解反锯齿效果的[原理](http://www.haogongju.net/tag/%E5%8E%9F%E7%90%86)，你可看到，AND
 MASK一定是“锯齿”的，因为它是二元图像只有两种颜色，如果有 Alhpa 通道，Alpha 通道会和AND MASK很像，但是黑白是相反的（因为它们的应用场合不同，因此黑白色的意义也不同，AND MASK 用于在背景上擦出黑色的绘制区域，其白色的意义是“保留背景原样”，Alpha 通道用于像素合成，其白色的意义是“此处像素完全不透明”），Alpha 通道的黑白色之间一般会有柔和的过渡（不然提供 Alpha 通道的意义就不大了），这正是图标绘制出来以后具有反锯齿效果的原因。其效果如下所示：
![](http://pic002.cnblogs.com/images/2010/22214/2010102401160361.jpg)
　　　　在这个工具中值得一提的是，对于 bpp 为16的图像在绘制 AND MASK 时必须[特别](http://www.haogongju.net/tag/%E7%89%B9%E5%88%AB)处理，bpp的取值通常是1，4，8，16，24，32。BPP 为 1，4，8 （注意和bpp
 = 8的灰度图像是有区别的）的彩色[索引](http://www.haogongju.net/tag/%E7%B4%A2%E5%BC%95)图像需要图像提供调色板（我在绘制时，把彩色索引图像升级成了 24 bpp 的像素数据块）。bpp等于16，24和32的图像都是通过像素数据块本身来提供
 RGB 通道的。bpp=16的特殊在于，它用两个字节（WORD）表示一个像素。
　　　　通道如何分布取决于 biCompression 的取值：
　　　　如果 biCompression 是 BI_RGB，表示无压缩，没有调色板。对于 biCompression 为
 BI_RGB 时，从低位到高位每5位表示一个通道（最高位没有用处，也没有 Alpha 通道），每个通道只有 2^5=32 级灰度，三个通道[分享](http://www.haogongju.net/tag/%E5%88%86%E4%BA%AB)两个字节。这样在绘制 AND MASK 时不能使用在
 24bpp 及以上的方式去设置像素数据（每个通道可独占一个字节）。例如在 AND MASK 中，白色是 0x7FFF（最高位为0），黑色是 0x0000，最大亮度是31 ( 0x1F )。
　　　　如果 biCompression 是 BI_BITFIELDS，（备注：这个值仅可能在 bpp 为16或32的图像中可能使用），则调色板中提供三个元素，分别是
 RGB 通道的 mask ，用于指定某个通道在像素数据的占用的是哪些位。例如如果 RGB 通道各占用 5 位，那么它们的 mask 分别是 0x7C00，0x03E0，0x001F。 
　　　　这个[小工具](http://www.haogongju.net/tag/%E5%B0%8F%E5%B7%A5%E5%85%B7)的源码下载链接如下：
[http://files.cnblogs.com/hoodlum1980/IconImg_src.rar](http://files.cnblogs.com/hoodlum1980/IconImg_src.rar)
参考文献：
　　　　（1）本文说明了如何创建W[IN](http://www.haogongju.net/tag/in)XP图标（给[图标设计](http://www.haogongju.net/tag/%E5%9B%BE%E6%A0%87%E8%AE%BE%E8%AE%A1)师阅读）
　　　　ms-help://MS.VSCC.v80/MS.MSDN.v80/MS.WIN32COM.v10.en/dnwxp/[html](http://www.haogongju.net/tag/html)/winxpicons.htm#winxpicons_step5
　　　　（2）本文比较[重要](http://www.haogongju.net/tag/%E9%87%8D%E8%A6%81)的是，介绍了如何使用XP风格控件（接受系统[样式](http://www.haogongju.net/tag/%E6%A0%B7%E5%BC%8F)的管理）。和32位的图标（支持alpha通道）
　　　　ms-help://MS.VSCC.v80/MS.MSDN.v80/MS.WIN32COM.v10.en/shellcc/platform/commctls/userex/cookbook.htm 
　　　　（3）关于ICO文件格式，可以参考我之前写的[博客](http://www.haogongju.net/tag/%E5%8D%9A%E5%AE%A2)：《[[VC6]
 图像文件格式数据查看器](http://www.cnblogs.com/hoodlum1980/archive/2010/09/05/1818308.html)》
　　　　在这里我再简要[总结](http://www.haogongju.net/tag/%E6%80%BB%E7%BB%93)下ICO文件格式：
Header：（6个字节。含图像个数）
Entries：（含每个图像的信息，这里的高度等于实际的高度，但这里的尺寸数据大小只有1个字节，因此应该以后面的BitmapInfoHeader中的尺寸为准）
Images：
[0] : BitmapInfoHeader  （这里的高度是实际的二倍，图像的大小应该以这里的数据为准）
RGBQUAD[]           （调色板，它有没有，如果有则含有多少个颜色，都取决于BitmapInfoHeader中的信息， 通常在 bpp<=8 时具有）
XOR MASK Bits       （[扫描](http://www.haogongju.net/tag/%E6%89%AB%E6%8F%8F)行按照4
 bytes 对齐，bpp 由前面的信息指定）
AND MASK Bits       （扫描行按照 4 bytes 对齐，bpp = 1）
