# CDC类中，常用的一些成员函数 - ljx0305的专栏 - CSDN博客
2009年11月01日 22:03:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：665标签：[windows																[microsoft																[作业																[图形																[框架																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=作业&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[VC																[MFC](https://blog.csdn.net/ljx0305/article/category/403913)](https://blog.csdn.net/ljx0305/article/category/401790)

CDC 
CObject 
└CDC 
CDC类定义的是设备上下文对象的类。 
CDC对象提供处理显示器或打印机等设备上下文的成员函数，以及处理与窗口客户区对应的显示上下文的成员。 
通过CDC对象的成员函数进行所有的绘图。 
类对设备上下文操作提供了成员函数，处理绘图工具。安全型图形设备接口（GDI）对象收集，以及处理颜色和调色板。 
它还为获取和设置绘图属性、映射，处理视点、窗口扩展、转换坐标，处理区域、剪贴、绘制直线及绘制简单椭圆和多边形等形状提供了成员函数。 
另外还为绘制文本、处理字体，使用打印机跳转，滚动和播放元文件提供成员函数。 
使用CDC对象时要构造它，然后调用与它平等的、使用设备上下文的Windows函数的成员函数。
**注意： **
Windows 95下所有屏幕坐标限制在16位以内。因此向CDC成员函数传递的int必须在-32768至32767之间。 
为了特定用途，Microsoft基本类库提供了几个CDC派生类。 CPaintDC 包括BeginPaint和EndPaint调用。 
CClientDC 管理窗口用户区对应的显示上下文。 
CWindowDC 管理与整个窗口对应的显示上下文，包括它的结构和控件。 
CMetaFileDC 与带元文件的设备上下文对应。 
CDC包含m_hDC和m_hAttribDC两个设备上下文，它们在CDC对象创建时参考同一个设备。
CDC指导所有对m_hDC的输出GDI调用以及对m_hAttribDC的大部分属性GDI调用（GetTextColor是属性调用的例子，而SetTextColor是一个输出调用）。 
例如框架使用这两个设备上下文实现CMetaFileDC对象，在从物理设备读取属性期间向元文件发送输出。 
打印预览以相同风格在框架中实现。还可以以相似方法在特殊应用代码中使用这两个设备上下文。 
可能多次需要来自m_hDC和m_hAttribDC设备上下文的文本度量信息。 
以下几对函数提供了这项功能： 
**使用m_hAttribDC 使用m_hDC **
GetTextExtent GetOutputTextExtent 
GetTabbedTextExtent GetOutputTabbedTextExtent 
GetTextMetrics GetOutputTextMetrics 
GetCharWidth GetOutputCharWidth 
**有关CDC的更多信息，请参阅联机文档“Visual C++程序员指南”中的“设备上下文”。 **
#include <afxwin.h> 
请参阅：CPaintDC, CClientDC, CWindowDC, CMetaFileDC 
**CDC类成员 **
数据成员 m_hDC CDC对象使用的输出设备上下文 
m_hAttribDC CDC对象使用的输出属性上下文 
构造函数 CDC 构造一个CDC对象 
初始化 CreateDC 为指定设备创建设备上下文 
CreateIC 为指定设备创建信息上下文。这提供了一种不创建设备上下文即获取有关设备信息的快速方式 
CreateCompatibleDC 创建内存设备上下文，与另一个设备上下文匹配。可以用它在内存中准备图像 
DeleteDC 删除CDC对象对应的Windows设备上下文 
FromHandle 给定设备上下文句柄时，返回指向CDC对象的指针。如果CDC对象未附加到句柄，则创建并附加一个临时CDC对象 
DeleteTempMap 由CWinApp空闲时间句柄调用，删除FromHandle创建的临时CDC对象。同时分离设备上下文 
Attach 把Windows设备上下文附加到这个CDC对象 
Detach 从CDC对象分离Windows设备上下文. 
SetAttribDC 设置属性设备上下文m_hAttribDC 
SetOutputDC 设置输出设备上下文m_hDC 
ReleaseAttribDC 释放属性设备上下文m_hAttribDC 
ReleaseOutputDC 释放输出设备上下文m_hDC 
GetCurrentBitmap 返回指向当前选择的CBitmap对象的指针 
GetCurrentBrush 返回指向当前选择的CBrush对象的指针 
GetCurrentFont 返回指向当前选择的CFont对象的指针 
GetCurrentPalette 返回指向当前选择的CPalette对象的指针 
GetCurrentPen 返回指向当前选择的CPen对象的指针 
GetWindow 返回显示设备上下文对应的窗口 
设备上下文函数 GetSafeHdc 返回输出设备上下文m_hDC 
SaveDC 保存设备上下文当前状态 
RestoreDC 恢复SaveDC保存的设备上下文以前状态 
ResetDC 更新m_hAttribDC设备上下文 
GetDeviceCaps 获取有关给定显示设备功能的指定类型指定设备信息 
IsPrinting 确定正在使用的设备上下文是否用于打印 
安全型选择帮助器 GetBrushOrg 获取当前画刷的起点 
SetBrushOrg 指定选入设备上下文的下一个画刷的起点 
EnumObjects 枚举设备上下文中有效的笔和画刷 
颜色和颜色调色板函数 SelectObject 选择笔等GDI绘图对象 
SelectStockObject 选择Windows提供的预定义的一个笔、画刷或字体 
绘图工具函数 GetNearestColor 获取与给定设备能显示的指定逻辑颜色最接近的逻辑色 
SelectPalette 选择逻辑调色板 
RealizePalette 把当前逻辑调色板中的调色板入口映射到系统调色板 
UpdateColors 通过客户区当前颜色在像素基础上与系统调色板的匹配，更新设备上下文的客户区 
GetHalftoneBrush 获取半色调画刷 
**绘图属性函数 GetBkColor 获取当前背景色 **
SetBkColor 设置当前背景色 
GetBkMode 获取背景模式 
SetBkMode 设置背景模式 
GetPolyFillMode 获取当前多边形填充模式 
SetPolyFillMode 设置多边形填充模式 
GetROP2 获取当前绘图模式 
SetROP2 设置当前绘图模式 
GetStretchBltMode 获取当前位图拉伸模式 
SetStretchBltMode 设置位图拉伸模式 
GetTextColor 获取当前文本颜色 
SetTextColor 设置文本颜色 
GetColorAdjustment 获取用于设备上下文的颜色调整值 
SetColorAdjustment 使用指定值设置设备上下文的颜色调整值 
**映射函数 GetMapMode 获取当前映射模式 **
SetMapMode 设置当前映射模式 
GetViewportOrg 获取视区起点的X和Y坐标 
SetViewportOrg 设置视区起点 
OffsetViewportOrg 相对于当前视区起点坐标修改视区起点 
GetViewportExt 获取视区X和Y范围 
SetViewportExt 设置视区X和Y范围 
ScaleViewportExt 相对于当前值修改视区范围 
GetWindowOrg 获取参应窗口起点的X和Y坐标 
SetWindowOrg 设置设备上下文的窗口起点 
OffsetWindowOrg 相对于当前窗口起点的坐标修改窗口起点 
GetWindowExt 获取对应窗口的X和Y坐标 
SetWindowExt 设置对应窗口的X和Y坐标 
ScaleWindowExt 相对于当前值修改窗口范围 
**坐标函数 DPtoHIMETRIC 设备单位转换为HIMETRIC单位 **
LPtoHIMETRIC 逻辑单位转换为HIMETRIC单位 
DPtoLP 设备单位转换为逻辑单位 
LPtoDP 逻辑单位转换为设备单位 
HIMETRICtoDPHIMETRIC 单位转换为设备单位 
HIMETRICtoLPHIMETRIC 单位转换为逻辑单位 
区域函数 FillRgn 用指定画刷填充指定区域 
FrameRgn 用画刷在指定区域周围绘制边线 
InvertRgn 反转区域中的颜色 
PaintRgn 用选取的画刷填充区域 
**剪切函数 SetBoundsRect 控制指定设备上下文绑定的矩形信息的累计 **
GetBoundsRect 返回到设备上下文当前累计的绑定矩形 
GetClipBox 获取当前剪切边界周围最紧绑定矩形的维数 
SelectClipRgn 使用指定模式合并给定区域和当前剪切区域 
ExcludeClipRect 创建包含现有剪切区域减掉指定矩形的一个新剪切区域 
ExcludeUpdateRgn 通过从剪切区域排除窗口中的更新区域，防止在窗口的无效区域中绘图 
IntersectClipRect 通过构成当前区域和矩形的交集创建一个新的剪切区域 
OffsetClipRgn 移动给定设备的剪切区域 
PtVisible 确定给定点是否在剪切区域内 
RectVisible 确定给定矩形的任何部分是否在剪切区域内 
**线输出函数 GetCurrentPosition 获取笔的当前位置（以逻辑坐标表示） **
MoveTo 移动当前位置 
LineTo 从当前位置到一点画直线，但不包括那个点 
Arc 画一段椭圆弧 
ArcTo 画一段椭圆弧。除了更新当前位置以外，这个函数与Arc类似 
AngleArc 画一条线段和圆弧，把当前位置移到圆弧终点 
GetArcDirection 对设备上下文返回当前圆弧方向 
SetArcDirection 设备圆弧和矩形函数要用到的绘图方向 
PolyDraw 画一组线段和Bezier样条。这个函数更新当前位置Polyline画一组与指定点连接的线段 
PolyPolyline 画多组相连线段。这个函数不使用也不更新当前位置 
PolylineTo 画一条或多条直线，并把当前位置移到最后一条直线的终点 
PolyBezier 画一条或多条Bezier样条。不使用也不更新当前位置 
PolyBezierTo 画一条或多条Bezier样条，并把当前位置移到最后一条Bezier样条的终点 
**简单绘图函数 FillRect 用指定画刷填充给定矩形 **
FrameRect 在矩形周围绘制边界 
InvertRect 反转矩形内容 
DrawIcon 绘制图标 
DrawDragRect 拖拽矩形时擦除并重绘它 
FillSolidRect 用实颜色填充矩形 
Draw3Drect 绘制三维矩形 
DrawEdge 绘制矩形边 
DrawFrameControl 绘制帧控件 
DrawState 显示图像并应用可视化效果来表示一个状态 
**椭圆和多边形函数 Chord 绘制椭圆弧（椭圆和一条线段相交围成的闭合图形） **
DrawFocusRect 绘制用于表示焦点的风格的矩形 
Ellipse 绘制椭圆 
Pie 绘制饼形图 
Polygon 绘制多边形，包含由线段连接的一个或多个点（顶点） 
PolyPolygon 创建使用当前多边形填充模式的两个或多个多边形，多边形可以相互分开或叠加 
Polyline 绘制多边形，包含连接指定点的一组线段 
Rectangle 使用当前笔绘制矩形，用当前画刷填充 
RoundRect 使用当前笔绘制圆角矩形，用当前画刷填充 
**位图函数 PatBlt 创建位特征 **
BitBlt 从指定设备上下文拷贝位图 
StretchBlt 把位图由源矩形和设备移动到目标矩形，必要时拉伸或压缩位图以适合目标矩形的维数 
GetPixel 获取指定点像素的RGB颜色值 
SetPixel 设置指定点像素为最接近指定色的近似值 
SetPixelV 设置指定点坐标为最接近指定色的近似值。SetPixelV比SetPixel快，原因是它不用返回实际绘制点的颜色值 
FloodFill 用当前画刷填充区域 
ExtFloodFill 用当前画刷填充区域。比FloodFill成员函数提供更多灵活性 
MaskBlt 使用给定屏蔽和光栅操作对源和目标位图合并颜色数据 
PlgBlt 从源设备上下文的指定矩形到给定设备上下文中指定平行多边形，执行颜色数据位的位块传递 
**文本函数 TextOut 用当前选取字体在指定位置写字符串 **
ExtTextOut 用当前选取字体在矩形区域写字符串 
TabbedTextOut 在指定位置写字符串，制表符扩展为制表符停止位置数组中指定值 
DrawText 在指定矩形内绘制格式化文本 
GetTextExtent 使用当前字体在属性设备上下文上计算文本行的宽度和高度，确定维数 
GetOutputTextExtent 使用当前字体在输出设备上下文上计算文本行的宽度和高度，确定维数 
GetTabbedTextExtent 使用当前字体在属性设备上下文上计算字符串的宽度和高度 
GetOutputTabbedTextExtent 使用当前字体在输出设备上下文上计算字符串的宽度和高度 
GrayString 在给定位置绘制暗（灰色）文本 
GetTextAlign 获取文本对齐标记 
SetTextAlign 设置文本对齐标记 
GetTextFace 以空终止字符串形式把当前字体字样名拷贝到缓冲区 
GetTextMetrics 从属性设备上下文为当前字体获取度量 
GetOutputTextMetrics 从输出设备上下文为当前字体获取度量 
SetTextJustification 把空格添加到字符串的中断字符 
GetTextCharacterExtra 对字符间空格数量获取当前设置 
SetTextCharacterExtra 设置字符间空格数量 
**字体函数 GetFontData 从比例字体文件获取字体度量信息。获取的信息通过指定字体文件与返回信息长度的偏移量标识 **
GetKerningPairs 获取指定设备上下文中当前选取的字体字符实对 
GetOutlineTextMetrics 获取用于TrueType字体的字体度量信息 
GetGlyphOutline 获取用于当前字体中外线字符串的外线曲线或位图 
GetCharABCWidths 从当前字体获取给定范围相继字符以逻辑单位计数的宽度 
GetCharWidth 从当前字体获取给定范围相继字符的分数宽度 
GetOutputCharWidth 从使用输出设备上下文的当前字体获取相继字符组中各个字符的宽度 
SetMapperFlags 改变字体映射表在从逻辑字体到物理字符映射时使用的算法 
GetAspectRatioFilter 获取当前长宽比过滤器的设置 
**打印机跳离函数 QueryAbort 对打印应用调用AbortPro回滚函数，询问打印是否应终止 **
Escape 允许应用访问不能通过GDI直接使用的特殊设备的功能。同时允许访问Windows跳离函数。应用调用的Escape调用经翻译传送到设备驱动程序 
DrawEscape 访问通过图形设备接口（GDI）不能直接使用的视频显示器的绘图功能 
StartDoc 通告设备驱动程序开始一项新打印作业 
StartPage 通告设备驱动程序开始新页 
EndPage 通告设备驱动程序页结束 
SetAbortProc 如果打印作业必须终止，设置Windows调用的由程序员提供的回滚函数 
AbortDoc 终止当前打印任务，擦除自上次调用StartDoc成员函数以来应用写入设备的任何内容 
EndDoc 结束由StartDoc成员函数启动的打印作业 
滚动函数 ScrollDC 水平和垂直滚动位矩形 
元文件函数 PlayMetaFile 在给定设备上播放指定元文件类型。增强版本的PlayMetaFile显示以增强格式元文件保存的图片。元文件可以播放任意次数 
AddMetaFileComment 从缓冲区把注释拷贝到指定增强格式元文件 
**路径函数 AbortPath 关闭并拒绝设备上下文中任何路径 **
BeginPath 打开设备上下文中一个路径括号 
CloseFigure 关闭路径中一个打开的图表 
EndPath 关闭一个路径括号，并把括号定义的路径选择到设备上下文 
FillPath 关闭当前路径中任何打开的图表，并用当前画刷和多边形填充模式填充路径内部 
FlattenPath 变形选取到当前设备上下文路径中的任何曲线，把曲线变成一系列直线 
GetMiterLimit 返回用于设备上下文的限制 
GetPath 获取定义直线端点和从选择到设备上下文路径中找到的曲线控制点的坐标 
SelectClipPath 对设备上下文选择当前路径作为剪切区域，通过使用指定模式合并新区域和任何现有剪切区域 
SetMiterLimit 设备用于设备上下文的连接长度限制 
StrokeAndFillPath 关闭路径中任何打开的图表，使用当前笔点路径外线，使用当前画刷填充它内部 
StrokePath 使用当前笔描绘指定路径 
WidenPath 如果路径使用过了当前选取到设备上下文中的笔点，重新定义当前路径为要画的区域
来自：http://hi.baidu.com/wuxiaoming1733/blog/item/5959731e8efc78fc1ad576ef.html
