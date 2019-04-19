# 什么是 Bitmap，而什么是 DIB 呢？两者有何区别呢 - xqhrs232的专栏 - CSDN博客
2012年11月05日 22:35:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：781
原文地址::[http://hi.baidu.com/dobest11111/item/a671c0c8b04ad00a0ad93a18](http://hi.baidu.com/dobest11111/item/a671c0c8b04ad00a0ad93a18)
■ DFB 与 DIB 的比较 （译文）-- 翻译 by 王颖◆
 介绍
什么是 Bitmap，而什么是 DIB 呢？两者有何区别呢？为什么 Windows API 有诸如这样的函数：CreateBitmap、CreateCompatibleBitmap、CreateDIBitmap、CreateDIBSection？而函数 SetBitmapBits 和 SetDIBits
 又有何区别呢？ 诚实地说，在很长一段时间，对于这些问题，我都没有合适的答案。我猜想并不是我一个人有这样的困惑。而这些问题对于我变得完全清晰，是在我开发了第一个内核模式的视频驱动程序以后。 我相信，每一个工作于 GDI 开发的程序员都应该知道这些问题的答案。 
◆ 2D-加速历史 什么是视频卡呢？很显然，视频卡是一种知道如何控制一个图像，并把它传送到一种显示设备（比如
 CRT 监视器等）的硬件设备。图像由视频卡的内部存储器（视频存储器）掌控，并且视频卡以一定的速率（刷新速率），为 CRT 监视器的阴极射线产生一个信号。这是通常的情况，而当今的视频卡还懂得做更多的事情。 什么是通常意义上的图像的大小呢？他们要消耗多少资源呢？大约来说，
一幅 640*480 的 8 位图像占用 300KB 内存；
一幅 800*600 的 16 位图像占用 937.5KB 内存；
一幅 1024*768 的 24 位图像占用 2.25MB 的内存。这个数字很大！如果你想要显示一幅这样的图像，你必须通过总线传送大于 2MB 的数据。假设你想要实现动态的图像（例如，使之移动），那将消耗大量的系统资源。而如果你想要对于屏幕上的图片做一些调整或改动，你就必须首先从视频存储器中抽取出来，然后修改它，最后再把它传回去。
然后，再来考虑一下画图的复杂度：画这样一幅图像，包括要改变几十万的像素点，对于CPU是一个巨大的负荷。 所有这些都导向了几个 2D 图像的发明。 ·
 处理画图指令的视频卡
这意味着，为了在屏幕上画"Hello World"，你不再需要合成输出图像然后把它上载到视频存储器中。现在，你可以仅仅发送包括所有参数（包括字体、画刷、布局位置等）的指令到视频卡的内部处理器中，然后聪明的视频卡将完成全部的处理。这样你就节省了 CPU 的时钟（在视频卡处理器工作时，CPU 可以做其他工作），并且你几乎不需要通过总线传送任何数据，仅仅是传送画图的参数。 ·
 设备管理位图（Device-managed Bitmap）（和框架缓存）
假设你想要在你的图像“之上”显示一些东西，然后移除它。当然，这里没有所谓的“之上”的概念，因为屏幕是一个平面。理论上，你不得不下载你的图像中的被替换的那部分并把它保存在某处（一个位图），然后在那里显示另外一幅图像，然后再上载之前保存的那副图像。现在，没有这样做的必要了。视频卡有比一个屏幕需要的更多的视频存储器。一部分视频存储器用于所谓的设备管理位图。视频卡可以在设备管理位图之间、设备管理位图和屏幕之间，完成他们的画图操作。这是一个很大的操作优势。 ◆
 术语 在我们继续之前，让我了解一些术语。 · 位图（Bitmap）
位图是一种描述矩形图像的数据块。它有大小和特定的格式。历史上的位图（bit-map）意味着一个包括指定颜色深度（一个像素的比特数）的矩阵，然而每位表示一个特定的像素颜色（或者是它的一部分）。但是在 Windows 98/2000 以后的操作系统，位图也有压缩的形式：JPG、PNG、RLE，所以严格来说，位图并不总仅仅是矩阵。 ·
 设备上下文（Device context，简称 DC）
设备上下文仅仅是一个抽象的结构体，控制着几个属性和其他画图对象的句柄。它不是一个设备，也不消耗大量的资源。你不再 DC 上画图。事实上，你在一个位图（位图被选择进 DC）或者一个由这个 DC 表示的设备上画图。事实上，只需要提供一些画图函数所需的默认的参数；否则每一个这样的函数都需要许多参数。例如，当你调用
 TextOut 函数时，你需要提供给它字符串和它的布局位置，但是还有其他参数，包括：字体、背景色和前景色、填充模式、剪切矩形、互相转换等。 · 设备无关位图（Device-independent bitmap，简称 DIB）
设备无关位图是一种已知像素格式的位图。它所包含的数据（位）保存在系统内存中，并且可被应用程序直接访问。换句话说，为了在 DIB 上画一个"Hello World"，你可以选择它进入一个 DC 然后调用 TextOut 函数（或其他函数），或者你可以直接改变它的一些位（你必须知道哪些位）。效果将会是一样的。
· 设备格式位图（Device-format bitmap，简称 DFB）或
     设备相关位图（Device-dependent bitmap，简称 DDB）
设备格式位图是一种由硬件设备（通常是一个视频卡）管理的位图。制造商可以以他希望的方式管理图像，所以像素的格式没有被定义。所以，你不能直接访问图像的数据。它也通常被保存在非系统内存（视频存储器）中。为了在 DFB 上画"Hello World"，你可以用 GDI 的TextOut 函数，其返回视频卡的驱动调用。
 我明确地定义了这些专业术语。因为在 GDI API 函数名会经常用到它们。在 GDI 函数名，所谓的"Bitmap"是 DIB 的反义词，然而实际上，DIB 只是一种特殊形式的"Bitmap"。实际上，DIBs 是 DFBs 的反义词，这两者都是"Bitmap"位图，但是它们被分别管理。它们应该被称为 CreateDIB*** 和 CreateDFB*** 函数，但是 GDI 却把它们命名为 CreateDIB*** 和 CreateBitmap*** 函数。在一些地方（并不是所有），函数名中的"Bitmap"部分，实际上应该被替换为"DFB"。所以，绝对不要依赖于函数名！一定要看函数的功能描述来确定其作用。 
◆ 关于 Windows 视频驱动体系 Windows 视频驱动体系是用来尽可能多地使用视频卡的硬件
 2D 加速的优势而设计的。但是，视频卡不是必须只是所有已知的 2D 加速技术；实际上，它根本没有支持 2D加速的必要。 每一个指向视频卡的画图指令，返回一个视频卡驱动调用的某个指针。驱动负责：以正确的方式（这一来于制造商）操作视频卡，所以视频驱动扮演硬件提取层的角色；
给硬件提供一个硬件无关的界面。 这是一系列基础的视频驱动必须执行的功能，包括：给设备设置特殊的视频模式；传送数据位，从设备来或者到设备中。所以视频卡根本不需要支持 2D 加速。另外，也有一些视频驱动可能执行的功能，包括：管理它自己的位图（DFBs）；支持一些 DFBs 位图（位图之间）的画图操作，比如位块转移（包括或者不包括像素格式转换），字体函数，alpha-blending（alpha
 混合），拉伸，倾斜填充，等等。每个这样的函数，即使被驱动报告给 OS 操作系统，也有权在任何时候拒绝工作。 这意味着什么呢？首先，你需要明白视频卡的驱动只负责它所管理的表面上的画图操作，表面可能是显示器或者 DFB。如果你要在 DIB 上画一些东西，视频卡根本不包括。现在试想一下，你调用 AlphaBlend GDI 函数（AlphaBlend 函数用来显示透明或半透明像素的位图），而 DC 表示的是显示器或者 DFB 位图的其中之一。
首先，它检查视频卡是否支持 alpha-blending 加速（如果是，它必须提供一个指向函数的指针）。如果是，OS 操作系统调用驱动支持的 DrvAlphaBlend 函数，否则是 Windows EngAlphaBlend 函数被执行。视频卡驱动检查它自己的 DrvAlphaBlend 函数所支持的参数，然后决定它是否愿意处理。如果是，它以硬件相关的方式调用视频卡（通过所谓的
 miniport 驱动）；否则驱动可以自由地调用EngAlphaBlend 函数做这个工作，就好像它根本没有支持 DrvAlphaBlend 函数。 那么，EngAlphaBlend 函数做什么呢？它必须完成 alpha-blending 的操作，虽然视频卡驱动并不支持它。在这种情况下，OS 操作系统暂停被请求的操作到一个简单的任务，并且再次调用视频驱动。例如，它会在系统内存（DIB）中合成结果图像，然后要求视频卡去绘制这个 DIB。或者它可以转换源参数（也可以是位图）到其他格式，排除驱动需要从一种格式转换到另一种格式，然后返回一个
 DrvAlphaBlend。驱动有权再次不执行这个调用，那么 OS 操作系统必须继续简化驱动的工作。在这种方式下，它可以到达一个非常基本的级别，直到驱动不能再拒绝处理。 所以，正像我们看到的，Windows 驱动体系知道如何利用硬件的 2D 加速，然后唯一一个视频卡强制的事情是必须知道传送图像，从或者到显示器中。 
◆ DFB vs DIB 在我们知道了 DFB 和 DIB 的区别以后，我们可以进一步的讨论。
 在 DFBs 上的画图操作是硬件加速的（至少是一部分），所以通常比较快。并且在它们之间传输和显示也非常快。但是，为什么我们想要使用 DIBs 呢？ 原因是，有时我们所需要的画图输出很复杂，以至于按照标准的 GDI 函数不能被表示。在这种情况下，最好有一个系统内存指针来指向图像的位并直接改变它。这不是一个简单的方法，但是它却有最好的性能，并且也普遍应用在图形处理程序中，比如：Photoshop、ACDSee等。另一种方法是只用 SetPixel 和 GetPixel 函数，以像素来建立图像像素。
为 DFBs 做这些将导致一个低级的处理能力，因为每一个这样的调用包括了很繁重的处理，但是为 DIBs 调用这些函数却是可以的。 
◆ GDI API 我们如何创建一个 DFB 和 DIB 呢？首先，没有一个函数确保创建
 DFB！
正如我们所知道的，视频驱动不需要支持 DFBs。并且即使它支持，它也可以拒绝处理，因为视频存储器不足，或者因为你要求的像素格式不被支持，或者不被视频卡加速。但是有函数可以尝试创建一个 DFB。如果它失败了，它将创建并返回一个 DIB 并带有适当的参数。这些函数是：CreateBitmap、CreateBitmapIndirect、CreateCompatibleBitmap（当
 DC 表示一个 DFB 或一个设备时），和 CreateDIBitmap，虽然它的名字很混淆。
 顺便说一句，我曾看过两个视频驱动的源码。它们都仅仅是当像素格式等同于当前显示器的格式时，才支持 DFBs，而且有些不管当前的视频模式，只支持单色的DFBs。 但是，有函数能确保创建一个 DIB。并且，仅有这样的一个函数。它是 CreateDIBSection。（再次声明，不要草考函数名，要看它的功能描述！）
 现在，假设我们有一个位图句柄 HBITMAP。我们如何知道它是一个 DFB 还是一个 DIB 呢？调用 GetObjectType 是不起作用的，因为它们都是 OBJ_BITMAP。幸运的是，有一个方法可以知道谁是谁。我偶然发现了这个方法。你可以调用 GetObject 函数，来用位图的参数填充
 BITMAP 结构体。如果 bmBits 成员是空，那么这个位图是 DFB，否则是 DIB。◆
 有用的提示 假设你有一个某自绘 GUI 或者动态的位图。这个位图不会改变；你也不会在这个为图上画图。
实际上，在这里一个 DFB 位图比一个 DIB 位图更有效。所以，如果你自己创建它，使用 CreateBitmap 来尝试创建一个 DFB。但是有时你从其他的函数中收到一个位图句柄，比如 LoadBitmap、LoadImage等。它们中的一些通常试图创建 DFBs，但是有些并不是。在这种情况下，下面的两个转换函数将非常有用！
 // 这个函数转换位图到 DFB
// 返回 True，如果转换成功
// 返回 False，如果转换不需要或者失败
bool ConvertToDFB(HBITMAP&
 hBitmap)
{
bool bConverted = false;
BITMAP stBitmap; // 判断如果是 DIB，否则是 DFB，就不需要转换了
if (GetObject(hBitmap, sizeof(stBitmap), &stBitmap) && stBitmap.bmBits)
{
    // 这是个 DIB。现在我们尝试创建一个相同大小，并且
    // 像素格式的 DFB（每次画它时可以忽略转换）。
    HDC hScreen = GetDC(NULL);
    if (hScreen)
    {
      HBITMAP hDfb = 
              CreateCompatibleBitmap(hScreen, 
      stBitmap.bmWidth, stBitmap.bmHeight);
      if (hDfb)
      {
        // 现在我们确实我们创建的不是 DIB
        if (GetObject(hDfb, sizeof(stBitmap), 
                   &stBitmap) && !stBitmap.bmBits)
        {
          // 好的，我们很幸运。现在我们需要传送这个图像到 DFB
          HDC hMemSrc = CreateCompatibleDC(NULL);
          if (hMemSrc)
          {
            HGDIOBJ hOldSrc = SelectObject(hMemSrc, hBitmap);
            if (hOldSrc)
            {
              HDC hMemDst = CreateCompatibleDC(NULL);
              if (hMemDst)
              {
                HGDIOBJ hOldDst = SelectObject(hMemDst, hDfb);
                if (hOldDst)
                {
                  // 用 BitBlt 函数转换图像。
                  // 这通常会以驱动的 DrvCopyBits 函数调用为结果
                  if (BitBlt(hMemDst, 0, 0, 
                        stBitmap.bmWidth, stBitmap.bmHeight, 
                        hMemSrc, 0, 0, SRCCOPY))
                    bConverted = true; // 成功                  VERIFY(SelectObject(hMemDst, hOldDst));
                }
                VERIFY(DeleteDC(hMemDst));
              }
              VERIFY(SelectObject(hMemSrc, hOldSrc));
            }
            VERIFY(DeleteDC(hMemSrc));
          }
        }        if (bConverted)
        {
          VERIFY(DeleteObject(hBitmap)); // 不再需要
          hBitmap = hDfb;
        }
        else
          VERIFY(DeleteObject(hDfb));
      }
      ReleaseDC(NULL, hScreen);
    }
}
return bConverted;
} 
// 这个函数转换位图到 DIB。
// 返回 True，如果转换成功
// 返回 False，如果转换不需要或者失败
bool ConvertToDIB(HBITMAP&
 hBitmap)
{
bool bConverted = false;
BITMAP stBitmap; // 判断如果是 DFB，否则是 DIB，就不需要转换了
if (GetObject(hBitmap, sizeof(stBitmap), 
            &stBitmap) && !stBitmap.bmBits)
{
    // 这是个 DFB。现在我们尝试创建一个相同大小，并且
    // 像素格式的 DIB（每次画它时可以忽略转换）。
    HDC hScreen = GetDC(NULL);
    if (hScreen)
    {
      union {
        BITMAPINFO stBitmapInfo;
        BYTE pReserveSpace[sizeof(BITMAPINFO) 
                     + 0xFF * sizeof(RGBQUAD)];
      };
      ZeroMemory(pReserveSpace, sizeof(pReserveSpace));
      stBitmapInfo.bmiHeader.biSize = sizeof(stBitmapInfo.bmiHeader);
      stBitmapInfo.bmiHeader.biWidth = stBitmap.bmWidth;
      stBitmapInfo.bmiHeader.biHeight = stBitmap.bmHeight;
      stBitmapInfo.bmiHeader.biPlanes = 1;
      stBitmapInfo.bmiHeader.biBitCount = stBitmap.bmBitsPixel;
      stBitmapInfo.bmiHeader.biCompression = BI_RGB;      if (stBitmap.bmBitsPixel <= 8)
      {
        stBitmapInfo.bmiHeader.biClrUsed = 
                        1 << stBitmap.bmBitsPixel;
        // 这个图像是调色板观看里的。
        // 所以我们需要合成它的调色板
      }
      stBitmapInfo.bmiHeader.biClrImportant = 
                       stBitmapInfo.bmiHeader.biClrUsed;      PVOID pBits;
      HBITMAP hDib = CreateDIBSection(hScreen, 
        &stBitmapInfo, DIB_RGB_COLORS, &pBits, NULL, 0);      if (hDib)
      {
        // 好的，我们很幸运。
        // 现在我们需要转换图像到 DFB
        HDC hMemSrc = CreateCompatibleDC(NULL);
        if (hMemSrc)
        {
          HGDIOBJ hOldSrc = SelectObject(hMemSrc, hBitmap);
          if (hOldSrc)
          {
            HDC hMemDst = CreateCompatibleDC(NULL);
            if (hMemDst)
            {
              HGDIOBJ hOldDst = SelectObject(hMemDst, hDib);
              if (hOldDst)
              {
                if (stBitmap.bmBitsPixel <= 8)
                {
                  // 获取 DFB 的调色板，并设置到 DIB 中
                  HPALETTE hPalette = 
                    (HPALETTE) GetCurrentObject(hMemSrc, OBJ_PAL);
                  if (hPalette)
                  {
                    PALETTEENTRY pPaletteEntries[0x100];
                    UINT nEntries = GetPaletteEntries(hPalette, 
                                    0, stBitmapInfo.bmiHeader.biClrUsed, 
                                    pPaletteEntries);
                    if (nEntries)
                    {
                      ASSERT(nEntries <= 0x100);
                      for (UINT nIndex = 0; nIndex < nEntries; nIndex++)
                        pPaletteEntries[nEntries].peFlags = 0;
                      VERIFY(SetDIBColorTable(hMemDst, 0, 
                        nEntries, (RGBQUAD*) pPaletteEntries) == nEntries);                    }
                  }
                }                // 用 BitBlt 函数转换图像。
                // 这通常会以驱动的 DrvCopyBits 函数调用为结果
                if (BitBlt(hMemDst, 0, 0, stBitmap.bmWidth, 
                      stBitmap.bmHeight, hMemSrc, 0, 0, SRCCOPY))
                  bConverted = true; // 成功                VERIFY(SelectObject(hMemDst, hOldDst));
              }
              VERIFY(DeleteDC(hMemDst));
            }
            VERIFY(SelectObject(hMemSrc, hOldSrc));
          }
          VERIFY(DeleteDC(hMemSrc));
        }        if (bConverted)
        {
          VERIFY(DeleteObject(hBitmap)); // 不再需要
          hBitmap = hDib;
        }
        else
          VERIFY(DeleteObject(hDib));
      }
      ReleaseDC(NULL, hScreen);
    }
}
return bConverted;
}
