# AlphaBlend/StretchBlt/BitBlt 三个函数介绍 - xqhrs232的专栏 - CSDN博客
2012年09月11日 16:18:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1247
原文地址::[http://www.cnblogs.com/bigbigtree/archive/2012/02/10/2345265.html](http://www.cnblogs.com/bigbigtree/archive/2012/02/10/2345265.html)
相关网帖
1.实现AlphaBlend混合----[http://hi.baidu.com/mikenoodle/item/7b28a9c0d8ac762c47d5c0bb](http://hi.baidu.com/mikenoodle/item/7b28a9c0d8ac762c47d5c0bb)

**AlphaBlend **
**该函数显示拥有透明或半透明像素的位图。 **
**Syntax**
BOOL AlphaBlend(
    HDC   **hdcDest**,
    int     **nXOriginDest**,  int   **nYOriginDest**,  int   **nWidthDest**,  int   **nHeightDest**,
    HDC   **hdcSrc**,
    int     **nXOriginSrc**,    int   **nYOriginSrc**,    int   **nWidthSrc**,   int   **nHeightSrc**,
    BLENDFUNCTION  **blendFunction**
    );
**Parameters **
hdcDest: [in] 目标DC的句柄
nXOriginDest、nYOriginDest: [in] 目标矩形左上角X坐标和Y坐标，按逻辑单元
nWidthDest、nHeightDest: [in] 目标矩形的宽度和高度，不能为负数，按逻辑单元
hdcSrc: [in] 源DC的句柄
nXOriginSrc、nYOriginSrc: [in] 源矩形左上角X坐标和Y坐标，按逻辑单元
nWidthSrc、nHeightSrc: [in] 源矩形的宽度和高度，不能为负数，按逻辑单元
blendFunction: [in] 指定用于源位图和目标位图使用的alpha混合功能，用于整个源位图的全局alpha值和格式信息。源和目标混合功能当前只限为**AC_SRC_OVER**。
**Return Value**
如果成功，返回TRUE；失败返回FALSE
获取扩展的错误信息，调用 GetLastError ， GetLastError会返回：**ERROR_INVALID_PARAMETER**，代表有一个或多个输入参数错误。
**Remarks**
   如果源矩形和目标矩形大小不同，源位图被缩放以匹配目标矩形。如果使用**SetStretchBltMode **函数，**iStretchMode **值自动转换为**COLORONCOLOR **。
   目标坐标使用为目标设备环境指定的转换方式进行转换。源坐标则使用为源设备环境指定的当前转换方式进行转换。
   如果源设备环境标识为增强型图元文件设备环境，那么会出错（并且该函数返回FALSE）。
   如果目标和源位图的色彩格式不同，那么AlphaBlend将源位图转换以匹配目标位图。
   AlphaBlend不支持镜像。如果源或目标区域的宽度或高度为负数，那么调用将失败。
   当在打印机上显示时，首先调用 **GetDeviceCaps** 并传入 **SHADEBLENDCAPS **参数检测打印机是否支持 AlphaBlend 混合。记住，对于显示DC，所有混合操作都被支持而且这些标志代表操作是否支持加速。
   源矩形必须位于源表面内（即，源矩形范围不能超出源位图的范围），否则将发生错误，函数返回FALSE.
**   BLENDFUNCTION **结构的 **SourceConstantaAlpha **成员指定使用在整个源位图上的 alpha 透明值。SourceConstantAlpha 值和每个像素的alpha值合并。如果 SourceConstantAlpha 为0，那么图像就为完全透明；如果 SourceConstantAlpha 为255，则图像为不透明。 
**Requirements**
Header: windows.h 
Library: coredll.lib 
Windows Mobile: Windows Mobile Version 5.0 and later  
**BLENDFUNCTION**
**该结构控制指定用于源位图和目标位图使用混合功能**
Syntax
typedef struct _BLENDFUNCTION {
  BYTE  **BlendOp**;
  BYTE  **BlendFlags**;
  BYTE  **SourceConstantAlpha**;
  BYTE  **AlphaFormat**;
}BLENDFUNCTION, *PBLENDFUNCTION, *LPBLENDFUNCTION; 
**Members:**
BlendOp: 指定源混合操作。目前，唯一的源和目标混合操作被定义为 AC_SRC_OVER。 详情，请参阅下面的备注部分。
BlendFlags: 必须为 0。
SourceConstantAlpha: 指定用于整张源位图的Alpha透明度值。 SourceConstantAlpha 值和每个像素的alpha值合并。如果 SourceConstantAlpha 为0，那么图像就为完全透明；如果 SourceConstantAlpha 为255，则图像为不透明。
AlphaFormat: 该成员控制源和目标位图被解释的方式。 下面显示了AlphaFormat值：
**    AC_SRC_ALPHA**      此标志位图时设置有一个Alpha通道（即每像素都有alpha值）。 由于此API使用预乘α，图中的红色，绿色和蓝色通道值必须预乘alpha通道值。 例如如果alpha通道值为x，那么在调用之前必，红色，绿色和蓝色通道须乘以x然后除以0xff。 
Remarks
   当AlphaFormat参数为**AC_SRC_ALPHA**，源位图必须是32色。 如果不是，AlphaBlend函数将失败。
   当BlendOp参数为**AC_SRC_OVER**，源位图将根据源像素的alpha值置于目标位图上。
   如果源位图没有设置AC_SRC_ALPHA，由在SourceConstantAlpha值确定源和目标位图混合，如下表所示。 注意 ，SourceConstantAlpha的值为SCA。 此外，SCA除以255是因为它的值范围为0至255.
Dst.Red   = Src.Red   * (SCA/255.0) + Dst.Red   * (1.0 - (SCA/255.0)) 
Dst.Green = Src.Green * (SCA/255.0) + Dst.Green * (1.0 - (SCA/255.0)) 
Dst.Blue  = Src.Blue  * (SCA/255.0) + Dst.Blue  * (1.0 - (SCA/255.0))
   如果目标位图有alpha通道，则混合如下。
 Dst.Alpha = Src.Alpha * (SCA/255.0) + Dst.Alpha * (1.0 - (SCA/255.0))
   如果源位图不使用SourceConstantAlpha（即，它等于0xFF），每个像素的alpha值决定源和目标位图的混合，如下列公式所示。
Dst.Red   = Src.Red   + (1 - Src.Alpha) * Dst.Red 
Dst.Green = Src.Green + (1 - Src.Alpha) * Dst.Green 
Dst.Blue  = Src.Blue  + (1 - Src.Alpha) * Dst.Blue 
   如果目标位图的alpha通道，则混合如下。
Dst.alpha = Src.Alpha + (1 - Src.Alpha) * Dst.Alpha
   如果源位图既有SourceConstantAlpha（也就是说，它不是0xFF）和每像素都有alpha值，那么源位图先预乘以SourceConstantAlpha，然后根据每个像素的alpha值混合。如下列方程显示。 请注意，SourceConstantAlpha除以255是因为它的值范围从0到255。
Src.Red   = Src.Red   * SourceConstantAlpha / 255.0; 
Src.Green = Src.Green * SourceConstantAlpha / 255.0; 
Src.Blue  = Src.Blue  * SourceConstantAlpha / 255.0; 
Src.Alpha = Src.Alpha * SourceConstantAlpha / 255.0; 
Dst.Red   = Src.Red   + (1 - Src.Alpha) * Dst.Red 
Dst.Green = Src.Green + (1 - Src.Alpha) * Dst.Green 
Dst.Blue  = Src.Blue  + (1 - Src.Alpha) * Dst.Blue 
Dst.Alpha = Src.Alpha + (1 - Src.Alpha) * Dst.Alpha
**StretchBlt **
**函数从源矩形中复制一个位图到目标矩形，必要时按目前目标设备设置的模式进行图像的拉伸或压缩以满足目标矩形的尺寸。**
**原型：**
BOOL StretchBlt(
    HDC **hdcDest**, 
    int **nXOriginDest**, int **nYOriginDest**,  int **nWidthDest**, int **nHeighDest**, 
    HDC **hdcSrc**, 
    int **nXOriginSrc**,   int **nYOriginSrc**,   int **nWidthSrc**,   int **nHeightSrc**, 
    DWORD **dwRop**)；
**参数：**
hdcDest：指向目标设备环境的句柄。
nXOriginDest、nYOriginDest：指定目标矩形左上角的X轴和Y轴坐标，按逻辑单位表示。
nWidthDest、nHeightDest：指定目标矩形的宽度和高度，按逻辑单位表示。
hdcSrc：指向源设备环境的句柄。
nXOriginSrc、nYOriginSrc：指向源矩形区域左上角的X轴和Y轴坐标，按逻辑单位表示。
nWidthSrc、nHeightSrc：指定源矩形的宽度和高度，按逻辑单位表示。
dwRop：指定要进行的光栅操作。光栅操作码定义了系统如何在输出操作中组合颜色，这些操作包括画刷、源位图和目标位图等对象。参考 [BitBlt](http://www.cnblogs.com/Lisen/archive/2010/01/21/1653062.html) 可了解常用的光栅操作码列表。
在Windows CE 1.0和1.01版中，参数dwRop只支持**SRCCOPY **和 **SRCINVERT **。
**返回值：**
如果函数执行成功，那么返回值为非零，如果函数执行失败，那么返回值为零。若想获得更多的错误信息，请调用**GetLastError**函数。
**说明：**
   StretchBlt函数在内存中对源位图进行扩展或压缩，然后将结果拷贝到目标矩形中。如果模板要与结果组合，则在扩展后的位图拷贝到目标矩形后才组合。
   当正在记录一个增强型图元文件时，如果源设备环境标识为一个增强型图元文件设备环境，那么会出现错误（函数返回FALSE）。
   如果指定的光栅操作需要画刷，那么系统使用当前已被选入到目标DC的画刷。
   目标坐标使用当前为目标DC指定的转换方式进行转换。源坐标则使用当前为源DC指定的转换方式进行转换。
   如果源转换发生了旋转或剪接，将产生错误。
   如果目标位图、源位图和模板位图的颜色格式不一致，StretchBlt会转换源位图和模板位图以匹配目标位图。
   如果要将黑白位图转换为彩色位图，它将背景色设置为白位（1），前景色设置为黑位（0）。如果要将彩色位图转换为黑白位图，函数设置与背景色匹配的像素为白（1），其它像素为黑（0），用到了带颜色的设备上下文中的前景色和背景色。
   如果nWidthSrc和nWidthDest或者nHeightSrc和nHeightDest大小不同，StretchBlt创建一个位图镜像。
       如果nWidthSrc和nWidthDest不同，函数按照X轴创建位图镜像。
       如果nHeightSrc和nHeightDest 不同，函数按照Y轴创建位图镜像。
   Windows CE 1.0 和 1.01 不支持镜像。
   并不是所有设备都支持StretchBlt，调用**GetDeviceCaps**获取支持。
   如果hdc指定镜像DC，水平坐标将从右到左增加，而不是从左到右。
   更多关于从右到左向位块传输显示信息（For information about blitting to displays with right-to-left orientations），参见 [Creating Bitmaps](http://msdn.microsoft.com/en-us/library/ms906571.aspx)。 
**SetSTretchBltMode**
**该函数可以设置指定设备环境中的位图拉伸模式。**
**原型：**
int SetSTretchBltMode(HDC **hdc**, int **iStretchMode**)；
**参数：**
hdc：设备环境句柄。
iStretchMode：指定拉伸模式。它可以取下列值，这些值的含义如下：
|值|描述|
|----|----|
|**BLACKONWHITE**：|对消除的和存在的像素的颜色值进行布尔AND（与）操作运算。如果该位图是单色位图，那么该模式以牺牲白色像素为代价，保留黑色像素点。|
|**COLORONCOLOR**|删除像素。该模式删除所有消除的像素行，不保留其信息。|
|**BILINEAR**|在X和Y轴方向线性修改颜色值。该模式仅当缩放源图像时工作，剪切时无效。该模式只在**SRCCOPY**, **SRCAND**, and**SRCPAINT **光栅操作下工作。如果设置为其他光栅操作符，**COLORONCOLOR **模式将被用于大于1bpp位深的位图，**BLACKONWHITE **模式将被用于1bpp位深的位图。|
**返回值：**
   如果函数执行成功，那么返回值就是先前的拉伸模式，如果函数执行失败，那么返回值为0。若想获得更多错误信息，请调用GetLastError函数，该值可能为下面的值**。**
|值|意义|
|----|----|
|**ERROR_INVALID_PARAMETER**|一个或多个输入参数非法|
**说明：**
   拉伸模式在应用程序调用StretchBit函数时定义系统如何将位图的行或列与显示设备上的现有像素点进行组合。
**BLACKONWHITE**(STRETCH_ANDSCANS)和**WHITEONBLACK**(STRETCH_ORSCANS)模式典型地用来保留单色位图中的前景像素。**COLORONCOLOR**(STRETCH_DELETESCANS)模式则典型地用于保留彩色位图中的颜色。
**HALFTONE**模式比其他三种模式需要对源图像进行更多的处理，也比其他模式慢，但它能产生高质量图像，也应注意在设置HALFTONE模式之后，应调用**SetBrushOrgEx**函数以避免出现刷子没对准现象。
   根据设备驱动程序的功能不同，其他一些拉伸模式也可能有效。
**BitBlt**
**该函数对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。**
**原型：**
BOOL BitBlt(
    HDC  **hdcDest**,
    int  **nXDest**,  int  **nYDest**,  int  **nWidth**,  int  **nHeight**,
    HDC  **hdcSrc**,
    int  **nXSrc**,  int  **nYSrc**,
    DWORD  **dwRop**)；
**参数：**
hdcDest：指向目标设备环境的句柄。
nXDest、nYDest：指定目标矩形区域左上角的X轴和Y轴逻辑坐标
nWidth、nHeight：指定源和目标矩形区域的逻辑宽度和逻辑高度。
hdcSrc：指向源设备环境的句柄。
nXSrc、nYSrc：指定源矩形区域左上角的X轴和Y轴逻辑坐标。
dwRop：指定光栅操作代码。这些代码将定义源矩形区域的颜色数据，如何与目标矩形区域的颜色数据组合以完成最后的颜色。下面列出了一些常见的光栅操作代码：
|值|描述|
|----|----|
|**BLACKNESS**|表示使用与物理调色板的索引0相关的色彩来填充目标矩形区域，（对缺省的物理调色板而言，该颜色为黑色）。|
|**DSTINVERT**|表示使目标矩形区域颜色取反。|
|**MERGECOPY **|表示使用布尔型AND（与）操作符将源矩形区域的颜色与特定模式组合一起。|
|**MERGEPAINT**|通过使用布尔型OR（或）操作符将源矩形区域的颜色取反后与目标矩形区域的颜色合并。|
|**NOTSRCCOPY**|将源矩形区域颜色取反，拷贝到目标矩形区域。|
|**NOTSRCERASE**|使用布尔类型的OR（或）操作符组合源和目标矩形区域的颜色值，然后将合成的颜色取反。|
|**PATCOPY**|将特定的模式拷贝到目标位图上。|
|**PATINVERT**|通过使用布尔型XOR（异或）操作符将源和目标矩形区域内的颜色合并。|
|**PATPAINT**|通过使用布尔型OR（或）操作符将源矩形区域取反后的颜色值与特定模式的颜色合并。然后使用OR（或）操作符将该操作的结果与目标矩形区域内的颜色合并。|
|**SRCAND**|通过使用布尔型AND（与）操作符来将源和目标矩形区域内的颜色合并。|
|**SRCCOPY**|将源矩形区域直接拷贝到目标矩形区域。|
|**SRCERASE**|通过使用布尔型AND（与）操作符将目标矩形区域颜色取反后与源矩形区域的颜色值合并。|
|**SRCINVERT**|通过使用布尔型XOR（异或）操作符将源和目标矩形区域的颜色合并。|
|**SRCPAINT**|通过使用布尔型OR（或）操作符将源和目标矩形区域的颜色合并。|
|**WHITENESS**|使用与物理调色板中索引1有关的颜色填充目标矩形区域。（对于缺省物理调色板来说，这个颜色就是白色）。|
  完整的光栅操作(ROP)码，参见 [Ternary Raster Operations](http://msdn.microsoft.com/en-us/library/aa452783.aspx).
**返回值：**
   如果函数成功，那么返回值非零；如果函数失败，则返回值为零。调用GetLastError函数获取扩展错误信息。
**说明：**
   如果在源设备环境中可以实行旋转或剪切变换，那么函数**BitBlt**返回一个错误。
   如果存在其他变换（并且目标设备环境中匹配变换无效），那么目标设备环境中的矩形区域将在需要时进行拉伸、压缩或旋转。
   如果源和目标设备环境的颜色格式不匹配，那么**BitBlt**函数将源场景的颜色格式转换成能与目标格式匹配的格式。
   当正在记录一个增强型图元文件时，如果源设备环境标识为一个增强型图元文件设备环境，那么会出现错误。
   并不是所有的设备都支持**BitBlt**函数。更多信息，调用**GetDeviceCaps **函数，将第二个参数赋值为**RC_BITBLT**来查看设备是否支持。(For more information, see the RC_BITBLT raster capability entry in the GetDeviceCaps function, **as well as the MaskBlt and StretchBlt functions**. )
   如果源和目标设备环境代表不同的设备，那么BitBlt函数返回错误。
   更多关于从右到左向位块传输显示信息（*For information about blitting to displays with right-to-left orientations*），参见 [Creating Bitmaps](http://msdn.microsoft.com/en-us/library/ms906571.aspx)。 
   在Windows CE 1.0和1.01版中，参数dwRop只可以指定为下列值：SRCCOPY、SRCAND、SRCPAINT、SRCINVERT。在Windows CE 2.0及以后版中，参数dwRop可以是任何ROP3代码值。 
下面是MSDN上的例子：
HBITMAP CopyBitmap( HBITMAP hbm) {
    HDC hdcSrc = CreateCompatibleDC(NULL);
    HDC hdcDst = CreateCompatibleDC(NULL);
    HBITMAP hbmOld, hbmOld2, hbmNew;
    BITMAP bm;
    GetObject(hbm, sizeof(bm), &bm);
    hbmOld = SelectObject(hdcSrc, hbm);
    hbmNew = CreateBitmap( bm.bmWidth, bm.bmHeight, bm.bmPlanes,
        bm.bmBitsPixel,
        NULL);
    hbmOld2 = SelectObject(hdcDst, hbmNew);
    BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);
    SelectObject(hdcSrc, hbmOld);
    SelectObject(hdcDst, hbmOld2);
    DeleteDC(hdcSrc);
    DeleteDC(hdcDst);
    return hbmNew;
} 
