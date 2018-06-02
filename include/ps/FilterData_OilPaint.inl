// Photoshop 油画效果滤镜
// 本滤镜是我采用 PS SDK 开发而成，而滤镜的算法具体是由谁提出的可能不详，我是参考了 FilterExplorer 的源码（VC 6），本算法的主要参考来源是该项目中的 Filters.cpp, 作者是 Jason Waltman (18, April, 2001) 。另国内另一个用C#语言编写的软件 PhotoSprite （Version 3.0 ，2006，由 联骏 编写）其中的油画滤镜的算法应该也是引用自了前者（或其他同源代码）。在研究此滤镜算法时，我主要参考的是前者的 C++ 代码，本文中对该算法的概念性描述属于我的理解和解读。但该算法的效率并不高，我将该算法的效率大大提高，关于模板尺寸的时间复杂度从 O ( n^2 ) 改进为线性复杂度 O ( n )，关于像素数量的复杂度的常数系数大大减小，对同一测试样本（某个 1920 * 1200 像素 RGB 图像）相同参数的处理速度从大约 35 秒降低到大约 3 秒，处理速度大概提高到10 ~ 12倍左右（粗略估算）。
// 本文主要是发布 Photoshop 油画效果滤镜（OilPaint）。算法并非我提出，可以参考本文的参考资料。该滤镜在用 C# 开发的国产软件 PhotoSprite 中可以看到。2010 年曾有人请求我帮助开发该滤镜，现在我花了大概几天时间将其开发出来并免费提供。
// （1）对油画滤镜的算法的概念性描述
// 这是我通过阅读 FilterExplorer 源码后得到的理解。该滤镜有两个参数，一个是模板半径（radius），则模板尺寸是（radius * 2 + 1）*（radius * 2 + 1）大小，也就是以当前像素为中心，向外扩展 radius 个像素的矩形区域，作为一个搜索范围，我们暂时将它称为“模板”（实际上该算法并不是例如高斯模糊，自定滤镜那种标准模板法，仅仅是处理过程类似，因此我才能实现稍后介绍的优化）。
// 另一个参数是光滑度（smoothness），实际上他是灰度桶的个数。我们假设把像素的灰度/亮度（ 0 ~ 255 ）均匀的分成 smoothness 个区间，则每个区间我们在此称它为一个桶（bucket），这样我们就有很多个桶，暂时称之为桶阵列（buckets）。
// 该算法遍历图上的每个像素，针对当前位置 (x, y) 像素，将模板范围内的所有像素灰度化，即把图像变成灰度图像，然后把像素值进一步离散化，即根据像素的灰度落入的区间，把模板内的像素依次投入到相应的桶中。然后从这些桶中找到一个落入像素个数最多的桶，并对该桶中的所有像素求出颜色平均值，作为位置 (x, y) 的结果值。
// 上面的算法描述，用下面的示意图来表示。中间的图像是从原图灰度化+离散化（相当于 Photoshop 中的色调分离）的结果，小方框表示的是模板。下方表示的是桶阵列（8 个桶，即把0~255的灰度值离散化成 8 个区间段）。
//
// （2）对老外已有代码的效率的改进
//   如果把已有的代码原样移植到 PS 滤镜中并不难，我大概花了 1 ~ 2 天的业余时间就基本调试成功了。但是在阅读老外的源码时，我明显感觉到原有代码的效率不够高。该算法遍历一次图像即可完成，对每个像素的处理是常数时间，因此针对像素数量（图像长度*图像宽度）是O（n）复杂度，但是原有代码的常系数较大，例如，每次计算像素结果时，都要重新计算模板范围内像素的灰度，并把它投入桶中，实际上造成大量的重复性计算。
// 2.1 为此，我的第一个改进是在 PS 中把当前的整个图像贴片进行灰度化并离散化（投入桶中），这样在用模板遍历贴片时，就不需要重复性的计算灰度并离散化了。这样大概把算法的运行速度提高了一倍左右（针对某个样本，处理速度从20多秒提高到10秒左右）。
// 2.2 但这样对速度的提高仍不够显著。因此我进行另一项更重要的优化，即把针对模板尺寸的复杂度从平方降低到线性复杂度。这个依据是，考虑模板在当前行间从左向右逐格移动，模板中部像素（相邻两个模板的交集）在结果中的统计数据是不变的。仅有最左侧一列移出模板，最右侧一列进入模板，因此我们在遍历图像时就不必管模板中部像素，只需要处理模板的两个边缘即可。如下图所示（半径为2，模板尺寸是 5 * 5 像素）：
//
// 当到达贴片右侧边缘时，我们不是类似回车换行那样重新复位到行首，而是把模板向下移动一行，进入下一行尾部，然后再向左平移，这样模板的行进轨迹就成为一个蛇形迂回步进的轨迹。当这样改进以后，我们遍历像素的时候就仅仅需要处理模板的两个边缘像素即可。这样，就把针对模板尺寸（参数中的半径）从O（n^2）降低到O（n），从而使该算法的运算速度大大提高，结合优化 2.1 ，最终使算法的运算速度大概提高了 11 倍（该数值仅仅是粗略估算，未经过大量样本测试），优化后的算法对大图像的处理时间也是变得可以接受的。
// 【注意】我能做到这样的优化的原因是该滤镜算法并不是标准的模板算法，它的本质是求模板范围内的统计信息，即结果和像素的模板坐标无关。这就好像是我们想得到某局部范围的人口数，男女比例等信息一样。因此我们按以上方法进行优化。
// 模板移动的轨迹是蛇形迂回步进，例如：
// → → → → → → →
//                     ↓
// ← ← ← ← ← ← ←
// ↓
// → → ...
// 2.3 原有代码对半径的范围限制是（1 ~ 5），由于我优化了代码，所以我把半径的区间可以大大提高，我把半径设置到 100 时我发现半径太大没什么意义，因为基本已经看不出原图是什么了。
// 【总结】改进后的代码的更具技巧性和挑战性，包括大量底层的指针操作和不同矩形（输入贴片，输出贴片，模板）之间的坐标定位，代码可读性上可能略有降低，但只要理解上面的原理，代码依然是具有较好的可读性的。此外，我还对该算法的改进想到，把模板从矩形改为“圆形”，以及在遍历图像时，使模板半径和桶个数这两个参数随机抖动，但这些改进都会使 2.2 中提到的优化失效，会使算法的速度又降回到较低的水平。
// （3）用多线程技术提高缩略图显示效率，避免影响 UI 线程的交互性
// 在参数对话框上显示缩略图的技术，可参考我之前发表的编写 PS 滤镜教程中的第四篇文章，在此不再叙述。这里我讲解的是更新缩略图时对 UI 交互的改进，以及缩放平移技术。
// 下图是在 Photoshop 中调用此滤镜时，弹出的参数设置对话框。用户可以拖动滑杆控件（TrackBar，又称为 Slider），也可以直接在后面的文本框中输入来改变参数。缩略图将实时性的更新已反应新参数。原来的滤镜实现中，我把更新缩略图的处理放在和对话框UI的相同线程内。这样做的话就会引入下面的问题，当用户很快的拖动滑杆控件时，由于参数改变的速度很快，而 UI 线程可能正忙于处理缩略图数据而短期被“阻塞”，使其不能立即响应后续的控件事件，即表现为滑杆控件的拖动不够流畅，有跳跃，顿挫，迟钝感，对鼠标拖动的反馈不够灵敏。
//
// 为了改进该问题，不影响 UI 线程，我准备把消耗时间较多的处理缩略图任务放到一个新的线程中去完成，当线程完成缩略图处理后即通知对话框更新其视图。当拖动 Trackbar 时，UI 线程会以很快的频率收到控件通知，好像“浪涌”一样，这要求后续到达的 UI 事件能让正在运行的线程任务很快的中途中止并退出。
// 我把滤镜算法提取出来作为一个共享的函数，这样滤镜的实际处理和更新缩略图可以共享这个函数。在 PS 实际调用滤镜，以及更新缩略图期间，实际上都要求滤镜算法定期检测“任务取消”事件。例如，在 PS 调用滤镜时，如果用户按下 ESC 键，就会立即放弃一个比较耗时的滤镜操作。在更新缩略图时，如果发生 UI 事件的浪涌，同样要求处理线程能够迅速中止。
// 在滤镜的核心算法函数中，我定期检测“任务取消”事件，由于在 PS 调用滤镜时的测试取消和更新缩略图时测试取消的方法不同，因此在滤镜算法函数中，我增加了一个回调函数参数（TestAbortProc）。这样，在 PS 调用滤镜实际处理时，使用的是 PS 内置的回调函数去检测取消事件，在更新对话框的缩略图时，我用自己提供的一个回调函数检测取消事件（该函数检测一个布尔变量来获知是否有新的 UI 事件等待处理）。
// 缩略图的处理，我使用的是单线程。即每个新的 UI 事件到来时，都要检测缩略图处理线程是否正在运行，如果是，我置有新的 UI 事件的标记，然后等待线程退出，等上一个线程退出后我才会启动新的线程，这样处理缩略图的线程永远只有一个，而不会在 UI 事件连续到来时开启过多的线程，这样做的好处是逻辑清晰，容易控制，不会让我们陷入线程太多无法维护的麻烦之中。缺点是，尽管线程定期检测取消事件，但线程中止还是需要少量时间的，这导致 UI 线程依然可能存在微小的“停顿感”，但它是微不足道的，比起在 UI 线程里来更新缩略图已取得本质的提升。
// 改进以后，可以用很快的速度拖动参数对话框上的两个滑竿，尽管这个滤镜核心算法的运算量较大，但我们能看到，参数对话框依然具有很流畅的响应。
// （4）缩略图的缩放和平移功能
// 实际上不管缩放还是平移，更新缩略图的数据本身并不难。难点主要在缩略图的平移，因为涉及到鼠标交互，这需要非常扎实的 windows 编程技巧和对 windows 程序底层机制的理解。
// 缩略图的拖动有以下两种方式：
// 4.1 直接拖动结果图片。
// 这又分为两种方法。其一是比较完美的拖动效果，但以浪费一定空间和时间为代价，且编码也具有一定挑战性。即把缩略图的输入数据外扩成 9 倍大小，并在内存中得到结果图片。显示的时候仅仅显示的是结果图的中央部分。在拖动时，缩略图中不会出现空白部分。
// 另一种方法是在拖动时把当前结果图进行快照（截屏），然后拖动时，仅仅把截屏结果贴到屏幕相应位置上。这样的效率较高，但缺点是拖动时能看到缩略图旁边有空白部分。这种方法常用语更新视图的成本较大的情况，例如矢量图绘制等。我在这个滤镜中实现的方法属于这种方法。
// 4.2 拖动图片为原始输入图片。
// 即拖动时，使用的图片是原始数据，而不是结果图片，这也是降低更新数据成本的一种折中方式。例如在 Photoshop 的内置滤镜高斯模糊中，就是采用这种方法。当拖动缩略图时，显示的缩略图是原图，仅在鼠标释放以后，才显示成预览效果。这样做是比较经济并且有效的。因为我们请求原始数据的成本不高，但用滤镜处理一次缩略图的成本较高。
// 这里额外提一点技术上的细节，要注意，由于鼠标可能移动到客户区以外（成为负数），这时不能直接使用 LOWORD ( lParam ) 和 HIWORD ( lParam )  去获得客户区坐标（因为WORD是无符号数），使用前应该它们转换成有符号数（short）。正确的方法是使用 windowsx.h 头文件中的宏：GET_X_LPARAM 和 GET_Y_LPARAM。
// （5）该滤镜的下载连接（附件中含有我写的 PS 插件安装工具，可简化用户安装）
//  【该插件最近于 2013-4-1 更新，增强了 UI 交互性能】
// //我开发的 PS 插件最新合集（含 ICO，OilPaint，DrawTable 等）
// http://files.cnblogs.com/hoodlum1980/PsPlugIns_V2013.zip
// 安装后并重启 Photoshop 以后：
// 在菜单：滤镜 - hoodlum1980 - OilPaint 中调用该滤镜。
// 在菜单：帮助 - 关于增效工具 - OilPaint... 中可以看到关于对话框（和我开发的 ICO 文件格式插件的关于对话框的外观几乎一致）。
// 在菜单：帮助 - 系统信息 中可以看到“OilPaint”一项是否已被加载及其版本信息。
// （6）一些不太重要的补充说明
// 6.1 我采用的输出贴片大小是 128 * 128 像素，在滤镜处理过程中，你看到的 Photoshop 状态栏上的进度条的每一次步进表示完成了一个输出贴片。输入贴片通常大于等于输出贴片，输入贴片大小和滤镜参数中的半径是相关的（在四个方向上向外扩模板半径的像素距离）。
// 6.2 在滤镜核心算法中，为了提高对取消事件的灵敏性，在当前行中我每处理16个像素就检测一次取消（行中像素索引 & 0x0F == 0x0F）。在每行处理结束后（列循环中）也检测一次取消。但这个检测频率略显过于频繁了，过于频繁可能会增大函数调用的成本开销。
// 6.3 采用相同图像，相同参数，我对我的滤镜，FilterExplorer，PhotoSprite 三种软件分别处理，然后再在 Photoshop 中比较。由于我的算法是参考 FilterExplorer 的源码，并在它的算法上改进而来，因此我的算法和 FilterExplorer 是等效的，只是效率更高，因此结果完全相同。但我的滤镜以及FilterExplorer的结果，同 PhotoSprite 相比较总体效果非常接近，但结果有细微的不同。我查看了下 PhotoSprite 的代码发现，这是在图像灰度化的算法的差异造成的（当我把 PhotoSprite 的灰度化算法调整为和 FilterExplorer 中相同后，处理结果就变成相同了）。
// 在 PhotoSprite 中，对像素灰度化采用的方法是：
// gray = ( byte ) ( ( 19661 * R + 38666 * G + 7209 * B ) >> 16 );
// 在 FilterExplorer / 我开发的滤镜中，对像素灰度化采用的方法是：
// gray = ( byte ) ( 0.3 * R + 0.59 * G + 0.11 * B ) ;
// PhotoSprite 中的灰度化法将浮点乘法转换成了整数乘法，效率可能会有一点点提高吧，但在此处表现并不显著。
// （7）参考资料
// 7.1  FilterExplorer 源码。
// 7.2  Photoshop 6.0 SDK Documents。
// 7.3  FillRed 和 IcoFormat 插件源码，by hoodlum1980（ myself ）。
// （8）修正历史
// 8.01 [H] 修复滤镜在 Continue 调用中，分配灰度桶内存时，传递的内存大小不正确（错误的设置成灰度化位图的空间大小）的BUG。该 BUG 容易在以下条件下被触发：文档尺寸太小，或半径参数很小且光滑度参数很大，这些因素可能导致某个贴片过小。这时由于灰度桶空间的分配小于实际需求的大小，则可能后续代码发生内存越界，进而使 PS 进程意外终止。2011-1-19  19:01。
// 8.02 [M] 新增缩略图缩放按钮和功能，且优化了放大缩小按钮处理时的代码，降低放大缩小时的闪烁感。2011-1-19 19:06。
// 8.03 [M] 新增缩略图鼠标拖动功能，且进一步调整了代码，使缩略图矩形固定下来，完全避免了缩放时的闪烁。2011-1-19 23:50。
// 8.04 [L] 新增功能，当鼠标移动到缩略图上和进行拖放时，光标变成伸开/抓住的手形。这是通过调用 PS 中的 Suite PEA UI Hooks suite 中的函数实现的，即在缩略图上看到的是 PS 内部的光标。2011-1-20 1:23。
// 8.05 [M] 修复在参数对话框上点击放大，缩小按钮时，半径参数折算的不正确的 BUG。此 BUG 使点击放大缩小按钮后，缩略图的显示不准。2011-1-20 1:55。
// 8.06 [L] 把关于对话框上的网址链接调整为 SysLink 控件，这样可以大大简化关于对话框的窗口过程代码。2011-1-20 18:12。
// 8.07 [L] 更新插件安装辅助工具，使它可以一次安装多个插件。2011-1-20 21:15。
// 8.08 [L] 由于缩放缩略图时计算有误差（原因不详，可能是因为浮点计算误差），可能使靠近边缘的右下角图像无法平移到缩略图视图中，因此对平移范围新增误差缓冲。2011-1-27。
// 8.09 [L] 美化：把滤镜参数对话框的放大缩小按钮，改为使用 DirectUI 技术实现（新增一个类 CImgButton），其界面效果比用原始按钮控件更好。2011-2-14。
// 8.10 [M] 性能：调整了插件在参数对话框时，调节半径和光滑度参数的滑竿的交互性能。2013-4-1。

typedef unsigned char uint8;

//从灰度桶阵列中，提取出最多像素的那个桶，并把桶中像素求平均值作为 RGB 结果。
void GetMostFrequentColor(UINT* buckets, int smoothness, uint8* pRed, uint8* pGreen, uint8* pBlue)
{
  UINT maxCount = 0;
  int i, index = 0;

  for (i = 0; i <= smoothness; i++) {
    if (buckets[ i * 4 ] > maxCount) {
      maxCount = buckets[ i * 4 ];
      index = i;
    }
  }

  if (maxCount > 0) {
    *pRed = (uint8)(buckets[ index * 4 + 1 ] / maxCount);   //Red
    *pGreen = (uint8)(buckets[ index * 4 + 2 ] / maxCount); //Green
    *pBlue = (uint8)(buckets[ index * 4 + 3 ] / maxCount);  //Blue
  }
}

//=========================================
//    缩略图和实际处理共享的滤镜算法
//=========================================
//
// 默认把数据当作是RGB, GrayData 是单通道数据，矩形和 InRect 一致
//
// bInitGray: 是否需要算法重新计算灰度数据
// rowBytes: inData/outData, 扫描行宽度
// colBytes: inData/outData, 对于interleave分布，等于通道数，集中分布时该为1
// planeBytes: 每个通道的字节数（对于interleave分布，该参数的值为1）
// grayData: 由于仅一个通道，所以grayColumnBytes一定是1；
// buckets: 灰度桶； 每个灰度占据4个UINT，0-count，1-redSum，2-greenSum，3-blueSum
// retVal：如果没有被打断，返回TRUE，否则返回FALSE（说明被用户取消或后续UI事件打断）
BOOL FilterData_OilPaint(
    uint8* pDataIn, const RECT inRect, int inRowBytes, int inColumnBytes, int inPlaneBytes,
    uint8* pDataOut, const RECT outRect, int outRowBytes, int outColumnBytes, int outPlaneBytes,
    uint8* pDataGray, int grayRowBytes, BOOL bInitGray,
    int radius,
    int smoothness,
    UINT* buckets
)
{
  int indexIn, indexOut, indexGray, i, j, i2, j2, k;    //像素索引
  uint8 red, green, blue;
  //设置边界
  int imaxOut = (outRect.right - outRect.left);
  int jmaxOut = (outRect.bottom - outRect.top);
  int imaxIn = (inRect.right - inRect.left);
  int jmaxIn = (inRect.bottom - inRect.top);
  //获取两个矩形（inRect和outRect）之间的偏移，即 outRect 左上角在 inRect 区中的坐标
  int x0 = outRect.left - inRect.left;
  int y0 = outRect.top - inRect.top;
  int colLeave, colEnter, yMin, yMax;
  int rowLeave, rowEnter, xMin, xMax;
  int direction;

  // 灰度离散化应该作为原子性操作，不应该分割
  if (bInitGray) {
    //把 In 贴片灰度化并离散化
    double scale = smoothness / 255.0;

    for (j = 0; j < jmaxIn; j++) {
      for (i = 0; i < imaxIn; i++) {
        indexIn = i * inColumnBytes + j * inRowBytes; //源像素[x, y]
        red = pDataIn[indexIn];
        green = pDataIn[indexIn + inPlaneBytes];
        blue = pDataIn[indexIn + inPlaneBytes * 2];
#define GET_GRAY(R, G, B)   (( byte ) ( 0.3 * R + 0.59 * G + 0.11 * B ))
        pDataGray[grayRowBytes * j + i] = (uint8)(GET_GRAY(red, green, blue) * scale);
      }
    }
  }

  // 模板和统计数据
  // 灰度桶 count, rSum, gSum, bSum
  memset(buckets, 0, (smoothness + 1) *sizeof(UINT) * 4);
  //初始化第一个模板位置的数据
  yMin = MAX(-y0, -radius);
  yMax = MIN(-y0 + jmaxIn - 1, radius);
  xMin = MAX(-x0,              -radius);
  xMax = MIN(-x0 + imaxIn - 1, radius);

  for (j2 = yMin; j2 <= yMax; j2++) {
    for (i2 = xMin; i2 <= xMax; i2++) {
      indexIn = (j2 + y0) * inRowBytes + (i2 + x0) * inColumnBytes;
      indexGray = (j2 + y0) * grayRowBytes + (i2 + x0);
      buckets[ pDataGray[indexGray] * 4 ]++; //count
      buckets[ pDataGray[indexGray] * 4 + 1 ] += pDataIn[indexIn]; //redSum
      buckets[ pDataGray[indexGray] * 4 + 2 ] += pDataIn[indexIn + inPlaneBytes]; //greenSum
      buckets[ pDataGray[indexGray] * 4 + 3 ] += pDataIn[indexIn + inPlaneBytes * 2]; //greenSum
    }
  }

  //进入模板的蛇形迂回循环
  for (j = 0; j < jmaxOut; j++) {
    //direction：水平移动方向（ 1 - 向右移动； 0 - 向左移动）
    direction = 1 - (j & 1);
    //找到最大的那个像素
    GetMostFrequentColor(buckets, smoothness, &red, &green, &blue);

    if (direction) {
      indexOut = j * outRowBytes;
    }
    else {
      indexOut = j * outRowBytes + (imaxOut - 1) * outColumnBytes;
    }

    pDataOut[ indexOut                     ] = red;
    pDataOut[ indexOut + outPlaneBytes     ] = green;
    pDataOut[ indexOut + outPlaneBytes * 2 ] = blue;
    i = direction ? 1 : (imaxOut - 2);

    for (k = 1; k < imaxOut; k++) { //k 是无意义的变量，仅为了在当前行中前进
      //每 64 个点测试一次用户取消 ( 在每行中间有一次测试 )
      if (direction) { //向右移动
        colLeave = i - radius - 1;
        colEnter = i + radius;
      }
      else { //向左移动
        colLeave = i + radius + 1;
        colEnter = i - radius;
      }

      yMin = MAX(-y0,              j - radius);
      yMax = MIN(-y0 + jmaxIn - 1, j + radius);

      //移出当前模板的那一列
      if ((colLeave + x0) >= 0 && (colLeave + x0) < imaxIn) {
        for (j2 = yMin; j2 <= yMax; j2++) {
          indexIn = (j2 + y0) * inRowBytes + (colLeave + x0) * inColumnBytes;
          indexGray = (j2 + y0) * grayRowBytes + (colLeave + x0);
          buckets[ pDataGray[indexGray] * 4     ]--; //count
          buckets[ pDataGray[indexGray] * 4 + 1 ] -= pDataIn[indexIn]; //redSum
          buckets[ pDataGray[indexGray] * 4 + 2 ] -= pDataIn[indexIn + inPlaneBytes]; //greenSum
          buckets[ pDataGray[indexGray] * 4 + 3 ] -= pDataIn[indexIn + inPlaneBytes * 2]; //greenSum
        }
      }

      //进入当前模板的那一列
      if ((colEnter + x0) >= 0 && (colEnter + x0) < imaxIn) {
        for (j2 = yMin; j2 <= yMax; j2++) {
          indexIn = (j2 + y0) * inRowBytes + (colEnter + x0) * inColumnBytes;
          indexGray = (j2 + y0) * grayRowBytes + (colEnter + x0);
          buckets[ pDataGray[indexGray] * 4     ]++; //count
          buckets[ pDataGray[indexGray] * 4 + 1 ] += pDataIn[indexIn]; //redSum
          buckets[ pDataGray[indexGray] * 4 + 2 ] += pDataIn[indexIn + inPlaneBytes]; //greenSum
          buckets[ pDataGray[indexGray] * 4 + 3 ] += pDataIn[indexIn + inPlaneBytes * 2]; //greenSum
        }
      }

      //找到最大的那个像素
      GetMostFrequentColor(buckets, smoothness, &red, &green, &blue);
      //目标像素[i, j]
      indexOut =  j * outRowBytes + i * outColumnBytes;
      pDataOut[ indexOut                     ] = red;
      pDataOut[ indexOut + outPlaneBytes     ] = green;
      pDataOut[ indexOut + outPlaneBytes * 2 ] = blue;
      i += direction ? 1 : -1;
    }

    //把模板向下移动一行
    rowLeave = j - radius;
    rowEnter = j + radius + 1;

    if (direction) {
      xMin = MAX(-x0, (imaxOut - 1) - radius);
      xMax = MIN(-x0 + imaxIn - 1, (imaxOut - 1) + radius);
      indexOut = (j + 1) * outRowBytes + (imaxOut - 1) * outColumnBytes; //目标像素[i, j]
    }
    else {
      xMin = MAX(-x0,              -radius);
      xMax = MIN(-x0 + imaxIn - 1, radius);
      indexOut = (j + 1) * outRowBytes; //目标像素[i, j]
    }

    //移出当前模板的那一列
    if ((rowLeave + y0) >= 0 && (rowLeave + y0) < jmaxIn) {
      for (i2 = xMin; i2 <= xMax; i2++) {
        indexIn = (rowLeave + y0) * inRowBytes + (i2 + x0) * inColumnBytes;
        indexGray = (rowLeave + y0) * grayRowBytes + (i2 + x0);
        buckets[ pDataGray[indexGray] * 4     ]--; //count
        buckets[ pDataGray[indexGray] * 4 + 1 ] -= pDataIn[indexIn]; //redSum
        buckets[ pDataGray[indexGray] * 4 + 2 ] -= pDataIn[indexIn + inPlaneBytes]; //greenSum
        buckets[ pDataGray[indexGray] * 4 + 3 ] -= pDataIn[indexIn + inPlaneBytes * 2]; //greenSum
      }
    }

    //进入当前模板的那一列
    if ((rowEnter + y0) >= 0 && (rowEnter + y0) < jmaxIn) {
      for (i2 = xMin; i2 <= xMax; i2++) {
        indexIn = (rowEnter + y0) * inRowBytes + (i2 + x0) * inColumnBytes;
        indexGray = (rowEnter + y0) * grayRowBytes + (i2 + x0);
        buckets[ pDataGray[indexGray] * 4     ]++; //count
        buckets[ pDataGray[indexGray] * 4 + 1 ] += pDataIn[indexIn]; //redSum
        buckets[ pDataGray[indexGray] * 4 + 2 ] += pDataIn[indexIn + inPlaneBytes]; //greenSum
        buckets[ pDataGray[indexGray] * 4 + 3 ] += pDataIn[indexIn + inPlaneBytes * 2]; //greenSum
      }
    }
  }

  return TRUE;
}