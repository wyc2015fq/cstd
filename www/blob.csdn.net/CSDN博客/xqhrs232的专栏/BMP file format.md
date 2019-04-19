# BMP file format - xqhrs232的专栏 - CSDN博客
2012年09月26日 13:31:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1945标签：[file																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)
个人分类：[C/C++/VC++/VS																[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://en.wikipedia.org/wiki/BMP_file_format](http://en.wikipedia.org/wiki/BMP_file_format)
相关网帖
1.16位位图格式（转）----[http://hi.baidu.com/rmgsxpxtzebcdpd/item/2fa5bf4e7f576c0be9350470](http://hi.baidu.com/rmgsxpxtzebcdpd/item/2fa5bf4e7f576c0be9350470)

From Wikipedia, the free encyclopedia
[](http://en.wikipedia.org/wiki/BMP_file_format#mw-head)[](http://en.wikipedia.org/wiki/BMP_file_format#p-search)
Type of format[Raster graphics](http://en.wikipedia.org/wiki/Raster_graphics)
The **BMP file format**, also known as **bitmap image file** or **Device Independent Bitmap (DIB) file format** or simply a **bitmap**, is
 a [raster graphics](http://en.wikipedia.org/wiki/Raster_graphics)[image
 file format](http://en.wikipedia.org/wiki/Image_file_format) used to store [bitmap](http://en.wikipedia.org/wiki/Bitmap)[digital
 images](http://en.wikipedia.org/wiki/Digital_image), independently of the [display device](http://en.wikipedia.org/wiki/Display_device) (such as a graphics adapter), especially on [Microsoft
 Windows](http://en.wikipedia.org/wiki/Microsoft_Windows) and [OS/2](http://en.wikipedia.org/wiki/OS/2)operating systems.
The BMP file format is capable of storing 2D digital images of arbitrary width, height, and resolution, both[monochrome](http://en.wikipedia.org/wiki/Monochrome) and
 color, in various [color depths](http://en.wikipedia.org/wiki/Color_depth), and optionally with [data
 compression](http://en.wikipedia.org/wiki/Data_compression), [alpha channels](http://en.wikipedia.org/wiki/Alpha_compositing), and[color
 profiles](http://en.wikipedia.org/wiki/Color_management).
## Contents
 [[hide](http://en.wikipedia.org/wiki/BMP_file_format#)] 
- [1Device-independent
 bitmaps and the BMP file format](http://en.wikipedia.org/wiki/BMP_file_format#Device-independent_bitmaps_and_the_BMP_file_format)
- [2File structure](http://en.wikipedia.org/wiki/BMP_file_format#File_structure)- [2.1DIBs in memory](http://en.wikipedia.org/wiki/BMP_file_format#DIBs_in_memory)
- [2.2Bitmap file
 header](http://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header)
- [2.3DIB
 header (bitmap information header)](http://en.wikipedia.org/wiki/BMP_file_format#DIB_header_.28bitmap_information_header.29)
- [2.4Color table](http://en.wikipedia.org/wiki/BMP_file_format#Color_table)
- [2.5Pixel storage](http://en.wikipedia.org/wiki/BMP_file_format#Pixel_storage)- [2.5.1Pixel
 array (bitmap data)](http://en.wikipedia.org/wiki/BMP_file_format#Pixel_array_.28bitmap_data.29)
- [2.5.2Compression](http://en.wikipedia.org/wiki/BMP_file_format#Compression)
- [2.5.3Pixel format](http://en.wikipedia.org/wiki/BMP_file_format#Pixel_format)
- [2.6Example 1](http://en.wikipedia.org/wiki/BMP_file_format#Example_1)
- [2.7Example 2](http://en.wikipedia.org/wiki/BMP_file_format#Example_2)
- [3Usage of BMP
 format](http://en.wikipedia.org/wiki/BMP_file_format#Usage_of_BMP_format)
- [4Related formats](http://en.wikipedia.org/wiki/BMP_file_format#Related_formats)
- [5See also](http://en.wikipedia.org/wiki/BMP_file_format#See_also)
- [6References](http://en.wikipedia.org/wiki/BMP_file_format#References)
- [7External links](http://en.wikipedia.org/wiki/BMP_file_format#External_links)
## [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=1)]Device-independent bitmaps and the BMP file format
Microsoft has defined a particular representation of color bitmaps of different color depths, as an aid to exchanging bitmaps between devices and applications with a variety of internal representations.
 They called these device-independent bitmaps or DIBs, and the file format for them is called DIB file format or BMP image file format. According to Microsoft support:[[1]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBhelp-0)
> 
*A device-independent bitmap (DIB) is a format used to define device-independent bitmaps in various color resolutions. The main purpose of DIBs is to allow bitmaps to be moved from one device to another (hence, the device-independent part of the name). A DIB is an external format, in contrast to a device-dependent bitmap, which appears in the system as a bitmap object (created by an application...). A DIB is normally transported in metafiles (usually using the StretchDIBits() function), BMP files, and the Clipboard (CF_DIB data format).*
The following sections discuss the data stored in the BMP file or DIB in detail. This is the standard BMP file format.[[1]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBhelp-0) Some
 applications create bitmap image files which are not compliant with the current Microsoft documentation. Also, not all fields are used; a value of 0 will be found in these unused fields.
## [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=2)]File structure
The bitmap image file consists of fixed-size structures (headers) as well as variable-size structures appearing in a predetermined sequence. Many different versions of some of these structures
 can appear in the file, due to the long evolution of this file format.
![](http://upload.wikimedia.org/wikipedia/commons/thumb/c/c4/BMPfileFormat.png/427px-BMPfileFormat.png)
![](http://bits.wikimedia.org/static-1.20wmf12/skins/common/images/magnify-clip.png)
Diag. 1 – The structure of the bitmap image file
Referring to the diagram 1, the bitmap file is composed of structures in the following order:
|Structure Name|Optional|Size|Purpose|Comments|
|----|----|----|----|----|
|**Bitmap File Header**|No|14 Bytes|To store general information about the Bitmap Image File|Not needed after the file is loaded in memory|
|**DIB Header**|No|Fixed-size(however 7 different versions exist)|To store detailed information about the bitmap image and define the pixel format|Immediately follows the Bitmap File Header|
|**Extra bit masks**|Yes|3 or 4 DWORDs[[2]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-AlphaBitFields-1)(12 or 16 Bytes)|To define the pixel format|Present only in case the DIB Header is the BITMAPINFOHEADER|
|**Color Table**|Semi-optional|Variable-size|To define colors used by the bitmap image data (Pixel Array)|Mandatory for [color depths](http://en.wikipedia.org/wiki/Color_depth) <= 8|
|**Gap1**|Yes|Variable-size|Structure alignment|An artifact of the File Offset to PixelArray in the Bitmap File Header|
|**Pixel Array**|No|Variable-size|To define the actual values of the pixels|The pixel format is defined by the DIB Header or Extra bit masks. Each row in the Pixel Array is padded to a multiple of 4 bytes in size|
|**Gap2**|Yes|Variable-size|Structure alignment|An artifact of the ICC Profile Data offset field in the DIB Header|
|**ICC Color Profile**|Yes|Variable-size|To define the color profile for color management|Can also contain a path to an external file containing the color profile. When loaded in memory as "non-packed DIB", it is located between the color table and gap1.[[3]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBHeaderTypes-2)|
### [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=3)]DIBs in memory
A bitmap image file loaded into memory becomes a DIB data structure – an important component of the Windows GDI API. The in-memory DIB data structure is almost the same as the BMP file format,
 but it does not contain the 14-byte bitmap file header and begins with the DIB header. For DIBs loaded in memory, the color table can also consist of 16 bit entries, that constitute indexes to the currently realized palette[[4]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-BITMAPINFO-3) (an
 additional level of indirection), instead of explicit RGB color definitions. In all cases, the pixel array must begin at a memory address that is a multiple of 4 bytes. In non-packed DIBs loaded in memory, the optional color profile data should be located
 immediately after the color table and before the gap1 and pixel array[[3]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBHeaderTypes-2) (unlike
 in diag. 1).
When the size of gap1 and gap2 is zero, the in-memory DIB data structure is customarily referred to as "packed DIB" and can be referred to by a single pointer pointing to the beginning of the
 DIB header. In all cases, the pixel array must begin at a memory address that is a multiple of 4 bytes. In some cases it may be necessary to adjust the number of entries in the color table in order to force the memory address of the pixel array to a multiple
 of 4 bytes.[[4]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-BITMAPINFO-3) For "packed
 DIBs" loaded in memory, the optional color profile data should immediately follow the pixel array, as depicted in diag. 1 (with gap1=0 and gap2=0).[[3]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBHeaderTypes-2)
"Packed DIBs" are required by [Windows](http://en.wikipedia.org/wiki/Microsoft_Windows) clipboard API functions as well as by some Windows patterned brush and resource functions.[[5]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-PackedDIB-4)
### [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=4)]Bitmap file header
This block of bytes is at the start of the file and is used to identify the file. A typical application reads this block first to ensure that the file is actually a BMP file and that it is
 not damaged. The first two bytes of the BMP file format are the character 'B' then the character 'M' in 1-byte [ASCII](http://en.wikipedia.org/wiki/ASCII) encoding. All of the integer
 values are stored in [little-endian](http://en.wikipedia.org/wiki/Little-endian) format (i.e. least-significant byte first).
|Offset#|Size|Purpose|
|----|----|----|
|0000h|2 bytes|the [header field](http://en.wikipedia.org/wiki/Magic_number_(programming)) used to identify the BMP & DIB file is 0x42 0x4D in [hexadecimal](http://en.wikipedia.org/wiki/Hexadecimal), same as BM in ASCII. The following entries are possible:- **BM** – Windows 3.1x, 95, NT, ... etc.- **BA** – OS/2 struct Bitmap Array- **CI** – OS/2 struct Color Icon- **CP** – OS/2 const Color Pointer- **IC** – OS/2 struct Icon- **PT** – OS/2 Pointer|
|0002h|4 bytes|the size of the BMP file in bytes|
|0006h|2 bytes|reserved; actual value depends on the application that creates the image|
|0008h|2 bytes|reserved; actual value depends on the application that creates the image|
|000Ah|4 bytes|the offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found.|
### [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=5)]DIB header (bitmap information header)
This block of bytes tells the application detailed information about the image, which will be used to display the image on the screen. The block also matches the header used internally by Windows
 and OS/2 and has several different variants. All of them contain a dword (32 bit) field, specifying their size, so that an application can easily determine which header is used in the image. The reason that there are different headers is that Microsoft extended
 the DIB format several times. The new extended headers can be used with some GDI functions instead of the older ones, providing more functionality. Since the GDI supports a function for loading bitmap files, typical Windows applications use that functionality.
 One consequence of this is that for such applications, the BMP formats that they support match the formats supported by the Windows version being run. See the table below for more information.
|Size|Header Name|OS support|Features Added (incremental)|in Bitmap Files Written by|
|----|----|----|----|----|
|12|BITMAPCOREHEADEROS21XBITMAPHEADER|[OS/2](http://en.wikipedia.org/wiki/OS/2) and also all Windows versions since [Windows 3.0](http://en.wikipedia.org/wiki/Windows_3.0)|||
|64|BITMAPCOREHEADER2OS22XBITMAPHEADER|[OS/2](http://en.wikipedia.org/wiki/OS/2)|Adds halftoning. Adds RLE and Huffman 1D compression.||
|40|BITMAPINFOHEADER|all Windows versions since[Windows 3.0](http://en.wikipedia.org/wiki/Windows_3.0)|Removes RLE-24 and Huffman 1D compression. Adds 16bpp and 32bpp pixel formats. Adds optional RGB bit masks.|Adobe Photoshop|
|52|BITMAPV2INFOHEADER|Undocumented.|Removes optional RGB bit masks. Adds mandatory RGB bit masks.||
|56|BITMAPV3INFOHEADER|Undocumented.|Adds mandatory alpha channel bit mask.|Adobe Photoshop|
|108|BITMAPV4HEADER|all Windows versions since Windows [95](http://en.wikipedia.org/wiki/Windows_95)/[NT4](http://en.wikipedia.org/wiki/Windows_NT_4.0)|Adds color space type and [gamma correction](http://en.wikipedia.org/wiki/Gamma_correction)||
|124|BITMAPV5HEADER|Windows [98](http://en.wikipedia.org/wiki/Windows_98)/[2000](http://en.wikipedia.org/wiki/Windows_2000) and newer|Adds [ICC color profiles](http://en.wikipedia.org/wiki/ICC_profile)||
Versions after BITMAPCOREHEADER only add fields to the end of the header of the previous version. For example: BITMAPV2INFOHEADER adds fields to BITMAPINFOHEADER and BITMAPV3INFOHEADER adds
 fields to BITMAPV2INFOHEADER (or, BITMAPINFOHEADER is a stripped version of BITMAPV2INFOHEADER)
For compatibility reasons, most applications use the older DIB headers for saving files. With OS/2 being obsolete, for now the common format is the BITMAPINFOHEADER header. See next table for
 its description. All values are stored as unsigned integers, unless explicitly noted.
|Offset #|Size|Purpose|
|----|----|----|
|0Eh|4|the size of this header (40 bytes)|
|12h|4|the bitmap width in pixels (signed integer).|
|16h|4|the bitmap height in pixels (signed integer).|
|1Ah|2|the number of color planes being used. Must be set to 1.|
|1Ch|2|the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.|
|1Eh|4|the compression method being used. See the next table for a list of possible values.|
|22h|4|the image size. This is the size of the raw bitmap data (see below), and should not be confused with the file size.|
|26h|4|the horizontal resolution of the image. (pixel per meter, signed integer)|
|2Ah|4|the vertical resolution of the image. (pixel per meter, signed integer)|
|2Eh|4|the number of colors in the color palette, or 0 to default to 2*n*.|
|32h|4|the number of important colors used, or 0 when every color is important; generally ignored.|
Note: The image size field can be 0 for BI_RGB bitmaps.
The compression method field (bytes #30-33) can have the following values:
|Value|Identified by|Compression method|Comments|
|----|----|----|----|
|0|BI_RGB|none|Most common|
|1|BI_RLE8|[RLE](http://en.wikipedia.org/wiki/Run-length_encoding) 8-bit/pixel|Can be used only with 8-bit/pixel bitmaps|
|2|BI_RLE4|RLE 4-bit/pixel|Can be used only with 4-bit/pixel bitmaps|
|3|BI_BITFIELDS|Bit field or Huffman 1D compression for BITMAPCOREHEADER2|Pixel format defined by bit masks or Huffman 1D compressed bitmap for BITMAPCOREHEADER2|
|4|BI_JPEG|JPEG or RLE-24 compression for BITMAPCOREHEADER2|The bitmap contains a [JPEG](http://en.wikipedia.org/wiki/JPEG) image or RLE-24 compressed bitmap for BITMAPCOREHEADER2|
|5|BI_PNG|PNG|The bitmap contains a [PNG](http://en.wikipedia.org/wiki/Portable_Network_Graphics) image|
|6|BI_ALPHABITFIELDS|Bit field|This value is valid in Windows CE .NET 4.0 and later.|
Note: BI_JPEG and BI_PNG are for printer drivers and are not supported when rendering to the screen.[[6]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-5)
The OS/2 BITMAPCOREHEADER header is also popular:
|Offset|Size|Purpose|
|----|----|----|
|0Eh|4|the size of this header (12 bytes)|
|12h|2|the bitmap width in pixels.|
|14h|2|the bitmap height in pixels.|
|16h|2|the number of color planes; 1 is the only legal value|
|18h|2|the number of bits per pixel. Typical values are 1, 4, 8 and 24.|
Note: OS/2 BITMAPCOREHEADER bitmaps cannot be compressed and cannot be 16 or 32 bits/pixel. All values in the OS/2 BITMAPCOREHEADER header are unsigned integers.
A 16-bit and 32-bit version of DIB with an integrated alpha channel has been introduced with the undocumented BITMAPV3INFOHEADER and with the documented BITMAPV4HEADER (since [Windows
 95](http://en.wikipedia.org/wiki/Windows_95)) and is used within [Windows XP](http://en.wikipedia.org/wiki/Windows_XP) logon and theme system as well as Microsoft Office (since v2000); it is supported by some [image
 editing](http://en.wikipedia.org/wiki/Image_editing) software, such as [Adobe Photoshop](http://en.wikipedia.org/wiki/Adobe_Photoshop) since version 7 and [Adobe
 Flash](http://en.wikipedia.org/wiki/Adobe_Flash) since version MX 2004 (then known as Macromedia Flash). It is also supported by [GIMP](http://en.wikipedia.org/wiki/GIMP), [Google
 Chrome](http://en.wikipedia.org/wiki/Google_Chrome), [MS-PowerPoint](http://en.wikipedia.org/wiki/Microsoft_PowerPoint) and MS-Word.
### [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=6)]Color table
The color table (palette) occurs in the BMP image file directly after the BMP file header, the DIB header (and after optional three red, green and blue bitmasks if the BITMAPINFOHEADER header
 with BI_BITFIELDS option is used). Therefore, its offset is the size of the BITMAPFILEHEADER plus the size of the DIB header (plus optional 12 bytes for the three bit masks).
*Note: On [Windows CE](http://en.wikipedia.org/wiki/Windows_CE) the BITMAPINFOHEADER header can be used with the BI_ALPHABITFIELDS[[2]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-AlphaBitFields-1) option in the biCompression member. In such case, four optional bitmasks follow the BITMAPINFOHEADER header instead of the two bitmask mentioned above, thus the color table's offset is the size of the BITMAPFILEHEADER plus the size of the BITMAPINFOHEADER header plus the 16 bytes of the four bitmasks (red, green, blue and alpha).*
The number of entries in the palette is either 2*n* or a smaller number specified in the header (in the OS/2 BITMAPCOREHEADER header format, only the
 full-size palette is supported).[[1]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBhelp-0)[[7]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-OS2gff-6) In
 most cases, each entry in the color table occupies 4 bytes, in the order blue, green, red, 0x00 (see below for exceptions).
The color table is a block of bytes (a table) listing the colors used by the image. Each pixel in an indexed color image is described by a number of bits (1, 4, or 8) which is an index of a
 single color described by this table. The purpose of the color palette in indexed color bitmaps is to inform the application about the actual color that each of these index values corresponds to. The purpose of the color table in non-indexed (non-palettized)
 bitmaps is to list the colors used by the bitmap for the purposes of optimization on devices with limited color display capability and to facilitate future conversion to different pixel formats and paletization.
The colors in the color table are usually specified in the 4-byte per entry [8.8.8.0.8](http://en.wikipedia.org/wiki/RGBAX) format
 (in [RGBAX](http://en.wikipedia.org/wiki/RGBAX) notation). The color table used with the OS/2 BITMAPCOREHEADER uses the 3-byte per entry [8.8.8.0.0](http://en.wikipedia.org/wiki/RGBAX) format.[[1]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBhelp-0)[[7]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-OS2gff-6) For
 DIBs loaded in memory, the color table can optionally consist of 2-byte entries - these entries constitute indexes to the currently realized palette[[4]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-BITMAPINFO-3) instead
 of explicit RGB color definitions.
Microsoft does not disallow the presence of a valid alpha channel bit mask[[8]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-7) in
 BITMAPV4HEADER and BITMAPV5HEADER for 1bpp, 4bpp and 8bpp indexed color images, which indicates that the color table entries can also specify an alpha component using the [8.8.8.[0-8].[0-8]](http://en.wikipedia.org/wiki/RGBAX) format
 via the RGBQUAD.rgbReserved[[9]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-8) member. However,
 some versions of Microsoft's documentation disallow this feature by stating that the RGBQUAD.rgbReserved member "must be zero".
As mentioned above, the color table is normally not used when the pixels are in the 16-bit per pixel (16bpp) format (and higher); there are normally no color table entries in those bitmap image
 files. However, the Microsoft documentation (on the MSDN web site as of Nov. 16, 2010) specifies that for 16bpp (and higher), the color table can be present to store a list of colors intended for optimization on devices with limited color display capability,
 while it also specifies, that in such cases, no indexed palette entries are present in this Color Table. This may seem like a contradiction if no distinction is made between the mandatory palette entries and the optional color list.
### [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=7)]Pixel storage
The bits representing the bitmap pixels are [packed](http://en.wikipedia.org/wiki/Packed) in
 rows. The size of each row is rounded up to a multiple of 4 bytes (a 32-bit [DWORD](http://en.wikipedia.org/wiki/DWORD)) by padding.
For images with height > 1, multiple padded rows are stored consecutively, forming a Pixel Array.
The total number of bytes necessary to store one row of pixels can be calculated as:
![\mbox{RowSize} = \left\lfloor\frac { \mbox{BitsPerPixel} \cdot \mbox{ImageWidth}+31 }{32}\right\rfloor \cdot 4,](http://upload.wikimedia.org/math/a/2/a/a2ae03037f8bf48ffe736e9d5b331c88.png)*ImageWidth* is expressed in pixels.
The total amount of bytes necessary to store an array of pixels in an n bits per pixel (bpp) image, with [2n](http://en.wikipedia.org/wiki/Power_of_two) colors,
 can be calculated by accounting for the effect of rounding up the size of each row to a multiple of a 4 bytes, as follows:
![\mbox{PixelArraySize} = \mbox{RowSize} \cdot \left | \mbox{ImageHeight} \right  \vert](http://upload.wikimedia.org/math/4/b/e/4be03da5f469979c07cf95e6207b3a2d.png)*ImageHeight* is expressed in pixels. The absolute value is necessary because ImageHeight can be negative
The total bitmap image file size can be approximated as:
![\mbox{FileSize} \approx 54 + 4 \cdot 2^\mbox{bpp} + \mbox{PixelArraySize}](http://upload.wikimedia.org/math/9/c/6/9c6480c9c56458964e66186059fbf197.png),
for *BPP* ≤ 8 ( because for pixels larger than 8 bits, the [palette](http://en.wikipedia.org/wiki/Palette_(computing)) is not mandatory )
Only images with 8 or fewer bits per pixel must account for the [palette](http://en.wikipedia.org/wiki/Palette_(computing)).
 16bpp images (or higher), may omit the [palette](http://en.wikipedia.org/wiki/Palette_(computing)) part from the size calculation, as follows:
![\mbox{FileSize} \approx 54 + \mbox{PixelArraySize}](http://upload.wikimedia.org/math/4/b/4/4b47d7ed9aadfc528623817cf48456b5.png),
for *Bits per Pixel* > 8.
In the formulas above, the number 54 is the combined size of the 14 byte bitmap file header and the 40 byte popular WindowsDIB header – the BITMAPINFOHEADER  (some other DIB header versions
 will be larger or smaller than that as described by the table above) and the expression ![4 \cdot 2^n](http://upload.wikimedia.org/math/c/c/0/cc02d9602bc019ca809aa56daf2e7ba5.png) 
 is the size of the color palette in bytes.
This total file size formula is only an approximation, since the size of the color palette will be ![3 \cdot 2^n](http://upload.wikimedia.org/math/5/c/f/5cf84ffff352bfee47d447957b4edea9.png) bytes
 for the OS/2 DIB header version BITMAPCOREHEADER, and some files may define only the number of colors needed by the image, potentially fewer than ![2^n](http://upload.wikimedia.org/math/9/a/a/9aa0ec0374c89d2f7f3d9cd2e05a4bc5.png).[[1]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBhelp-0)
An additional size uncertainty is introduced by the optional presence of the 12 or 16 bytes needed for the extra bit masks stored immediately after the BITMAPINFOHEADER DIB header and the variable-size GAP depicted in Diag.1
#### [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=8)]Pixel array (bitmap data)
The pixel array is a block of 32-bit DWORDs, that describes the image pixel by pixel. Normally pixels are stored "upside-down" with respect to normal image [raster
 scan](http://en.wikipedia.org/wiki/Raster_scan) order, starting in the lower left corner, going from left to right, and then row by row from the bottom to the top of the image.[[1]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBhelp-0)
Unless BITMAPCOREHEADER is used, uncompressed Windows bitmaps also can be stored from the top to bottom, when the Image Height value is negative.
In the original OS/2 DIB, the only four legal values of color depth were 1, 4, 8, and 24 bits per pixel (bpp).[[1]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBhelp-0) Contemporary
 DIB Headers allow pixel formats with 1, 2, 4, 8, 16, 24 and 32 bits per pixel (bpp).[[10]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-9)
Padding bytes (not necessarily 0) must be appended to the end of the rows in order to bring up the length of the rows to a multiple of four bytes. When the pixel array is loaded into memory,
 each row must begin at a memory address that is a multiple of 4. This address/offset restriction is mandatory only for Pixel Arrays loaded in memory. For file storage purposes, only the size of each row must be a multiple of 4 bytes while the file offset can
 be arbitrary.[[1]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBhelp-0)
A 24-bit bitmap with Width=1, would have 3 bytes of data per row (blue, green, red) and 1 byte of padding, while Width=2 would have 2 bytes of padding, Width=3 would have 3 bytes of padding, and Width=4 would not have any padding at all.
bitmap image files are typically much larger than image file formats compressed with other algorithms, for the same image. For example, the 1058×1058 [Wikipedia
 logo](http://en.wikipedia.org/wiki/File:Wikipedia-logo.png), which occupies about 271 kB in the lossless [PNG](http://en.wikipedia.org/wiki/Portable_Network_Graphics) format, takes about 3358 kB as a 24bpp BMP image
 file. Uncompressed formats are generally unsuitable for transferring images on the internet or other slow or capacity-limited media.
#### [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=9)]Compression
[Indexed](http://en.wikipedia.org/wiki/Palette_(computing)) color images may be compressed with 4-bit
 or 8-bit [RLE](http://en.wikipedia.org/wiki/Run-length_encoding) or [Huffman](http://en.wikipedia.org/wiki/Huffman_coding) 1D
 algorithm.
[OS/2](http://en.wikipedia.org/wiki/OS/2) BITMAPCOREHEADER2 [24bpp](http://en.wikipedia.org/wiki/Color_depth) images
 may be compressed with the 24-bit [RLE](http://en.wikipedia.org/wiki/Run-length_encoding) algorithm.
The [16bpp](http://en.wikipedia.org/wiki/Color_depth) and [32bpp](http://en.wikipedia.org/wiki/Color_depth) images
 are *always* stored uncompressed.
Note that images in all color depths can be stored without compression if so desired.
#### [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=10)]Pixel format
In a bitmap image file on a disk or a bitmap image in memory, the pixels can be defined by a varying number of bits.
- The 1-bit per pixel (1bpp) format supports 2 distinct colors, (for example: black and white). The pixel values are stored in each bit, with the first (left-most) pixel in the most-significant bit of the first byte.[[1]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBhelp-0) Each
 bit is an index into a table of 2 colors. An unset bit will refer to the first color table entry, and a set bit will refer to the last (second) color table entry.
- The 2-bit per pixel (2bpp) format supports 4 distinct colors and stores 4 pixels per 1 byte, the left-most pixel being in the two most significant bits ([Windows
 CE](http://en.wikipedia.org/wiki/Windows_CE) only[[11]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-PSformats-10)). Each
 pixel value is a 2-bit index into a table of up to 4 colors.
- The 4-bit per pixel (4bpp) format supports 16 distinct colors and stores 2 pixels per 1 byte, the left-most pixel being in the more significant [nibble](http://en.wikipedia.org/wiki/Nibble).[[1]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBhelp-0) Each
 pixel value is a 4-bit index into a table of up to 16 colors.
- The 8-bit per pixel (8bpp) format supports 256 distinct colors and stores 1 pixel per 1 byte. Each byte is an index into a table of up to 256 colors.
- The 16-bit per pixel (16bpp) format supports 65536 distinct colors and stores 1 pixel per 2 byte WORD. Each WORD can define the alpha, red, green and blue [samples](http://en.wikipedia.org/wiki/Sample_(graphics)) of
 the pixel.
- The 24-bit pixel (24bpp) format supports 16,777,216 distinct colors and stores 1 pixel value per 3 bytes. Each pixel value defines the red, green and blue samples of the pixel (8.8.8.0.0 in RGBAX notation). Specifically in the
 order (blue, green and red, 8-bits per each sample).[[1]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-DIBhelp-0)
- The 32-bit per pixel (32bpp) format supports 4,294,967,296 distinct colors and stores 1 pixel per 4 byte DWORD. Each DWORD can define the Alpha, Red, Green and Blue samples of the pixel.
In order to resolve the ambiguity of which bits define which samples, the DIB Headers provide certain defaults as well as specific BITFIELDS which are bit masks that define the membership of
 particular group of bits in a pixel to a particular [channel](http://en.wikipedia.org/wiki/Channel_(digital_image)).
The following diagram defines this mechanism:
![The BITFIELDS mechanism depicted in RGBAX sample length notation](http://upload.wikimedia.org/wikipedia/commons/3/3c/BitfieldsSLN.png)
Diag. 2 – The BITFIELDS mechanism for a 32 bit pixel depicted in RGBAX sample length notation
The sample fields defined by the BITFIELDS bit masks have to be contiguous and non-overlapping.
The red, green and blue bit masks are valid only when the Compression member of the DIB header is set to BI_BITFIELDS.
The alpha bit mask is valid whenever it is present in the DIB header or when the Compression member of the DIB header is set to BI_ALPHABITFIELDS[[2]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-AlphaBitFields-1)([Windows
 CE](http://en.wikipedia.org/wiki/Windows_CE) only).
![The BITFIELDS mechanism depicted in RGBAX sample length notation](http://upload.wikimedia.org/wikipedia/commons/8/84/SLNotation44440.png)
Diag.3 – The pixel format with an alpha channel in a 16 bit pixel (in RGBAX sample Lngth notation) actually generated by Adobe Photoshop[[11]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-PSformats-10)
The table below lists all of the possible pixel formats of a DIB (in RGBAX notation).
![All of the possible pixel formats in a DIB](http://upload.wikimedia.org/wikipedia/commons/d/da/AllBMPformats.png)
The BITFIELD mechanism described above allows for the definition of tens of thousands different pixel formats, however only several of them are used in practice,[[11]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-PSformats-10) such
 as:
- 8.8.8.0.0
- 8.8.8.0.8
- 8.8.8.8.0
- 5.5.5.0.1
- 5.5.5.1.0
- 5.6.5.0.0
- 4.4.4.0.4
- 4.4.4.4.0
- All palettized formats (marked in yellow in the table above)
### [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=11)]Example 1
Following is an example of a 2×2 pixel, 24-bit bitmap (Windows DIB header BITMAPINFOHEADER) with pixel format 8.8.8.0.0 (in RGBAX notation).
[](http://en.wikipedia.org/w/index.php?title=File:Bmp_format.svg&page=1)
Example of a 2×2 pixel bitmap, with 24 bits/pixel encoding
|Offset|Size|Hex Value|Value|Description|
|----|----|----|----|----|
|BMP Header| | | | |
|0h|2|`42 4D`|"BM"|Magic number (unsigned integer 66, 77)|
|2h|4|`46 00 00 00`|70 Bytes|Size of the BMP file|
|6h|2|`00 00`|Unused|Application specific|
|8h|2|`00 00`|Unused|Application specific|
|Ah|4|`36 00 00 00`|54 bytes|Offset where the pixel array (bitmap data) can be found|
|DIB Header| | | | |
|Eh|4|`28 00 00 00`|40 bytes|Number of bytes in the DIB header (from this point)|
|12h|4|`02 00 00 00`|2 pixels|Width of the bitmap in pixels|
|16h|4|`02 00 00 00`|2 pixels|Height of the bitmap in pixels|
|1Ah|2|`01 00`|1 plane|Number of color planes being used|
|1Ch|2|`18 00`|24 bits|Number of bits per pixel|
|1Eh|4|`00 00 00 00`|0|BI_RGB, no pixel array compression used|
|22h|4|`10 00 00 00`|16 bytes|Size of the raw data in the pixel array (including padding)|
|26h|4|`13 0B 00 00`|2,835 pixels/meter|Horizontal resolution of the image|
|2Ah|4|`13 0B 00 00`|2,835 pixels/meter|Vertical resolution of the image|
|2Eh|4|`00 00 00 00`|0 colors|Number of colors in the palette|
|32h|4|`00 00 00 00`|0 important colors|0 means all colors are important|
|Start of pixel array (bitmap data)| | | | |
|36h|3|`00 00 FF`|0 0 255|Red, Pixel (0,1)|
|39h|3|`FF FF FF`|255 255 255|White, Pixel (1,1)|
|3Ch|2|`00 00`|0 0|Padding for 4 byte alignment (could be a value other than zero)|
|3Eh|3|`FF 00 00`|255 0 0|Blue, Pixel (0,0)|
|41h|3|`00 FF 00`|0 255 0|Green, Pixel (1,0)|
|44h|2|`00 00`|0 0|Padding for 4 byte alignment (could be a value other than zero)|
### [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=12)]Example 2
Following is an example of a 4×2 pixel, 32-bit bitmap with opacity values in the alpha channel (Windows DIB Header BITMAPV4HEADER) with pixel format 8.8.8.8.0 (in RGBAX notation).
[](http://en.wikipedia.org/w/index.php?title=File:Bmp_format2.svg&page=1)
Example of a 4×2 pixel bitmap, with 32 bits/pixel encoding
|Offset|Size|Hex Value|Value|Description|
|----|----|----|----|----|
|BMP Header| | | | |
|0h|2|`42 4D`|"BM"|Magic number (unsigned integer 66, 77)|
|2h|4|`9A 00 00 00`|154 Bytes|Size of the BMP file|
|6h|2|`00 00`|Unused|Application specific|
|8h|2|`00 00`|Unused|Application specific|
|Ah|4|`7A 00 00 00`|122 bytes from the start of the file|Offset where the pixel array (bitmap data) can be found|
|DIB Header| | | | |
|Eh|4|`6C 00 00 00`|108 bytes|Number of bytes in the DIB header (from this point)|
|12h|4|`04 00 00 00`|4 pixels|Width of the bitmap in pixels|
|16h|4|`02 00 00 00`|2 pixels|Height of the bitmap in pixels|
|1Ah|2|`01 00`|1 plane|Number of color planes being used|
|1Ch|2|`20 00`|32 bits|Number of bits per pixel|
|1Eh|4|`03 00 00 00`|3|BI_BITFIELDS, no pixel array compression used|
|22h|4|`20 00 00 00`|32 bytes|Size of the raw data in the pixel array (including padding)|
|26h|4|`13 0B 00 00`|2,835 pixels/meter|Horizontal physical resolution of the image|
|2Ah|4|`13 0B 00 00`|2,835 pixels/meter|Vertical physical resolution of the image|
|2Eh|4|`00 00 00 00`|0 colors|Number of colors in the palette|
|32h|4|`00 00 00 00`|0 important colors|0 means all colors are important|
|36h|4|`00 00 FF 00`|00FF0000 in big-endian|Red channel bit mask (valid because BI_BITFIELDS is specified)|
|3Ah|4|`00 FF 00 00`|0000FF00 in big-endian|Green channel bit mask (valid because BI_BITFIELDS is specified)|
|3Eh|4|`FF 00 00 00`|000000FF in big-endian|Blue channel bit mask (valid because BI_BITFIELDS is specified)|
|42h|4|`00 00 00 FF`|FF000000 in big-endian|Alpha channel bit mask|
|46h|4|`20 6E 69 57`|LCS_WINDOWS_COLOR_SPACE|Type of Color Space|
|4Ah|24h|`24* 00...00`|CIEXYZTRIPLE Color Space endpoints|Unused when LCS_WINDOWS_COLOR_SPACE is specified|
|6Eh|4|`00 00 00 00`|0 Red Gamma|Unused when LCS_WINDOWS_COLOR_SPACE is specified|
|72h|4|`00 00 00 00`|0 Green Gamma|Unused when LCS_WINDOWS_COLOR_SPACE is specified|
|76h|4|`00 00 00 00`|0 Blue Gamma|Unused when LCS_WINDOWS_COLOR_SPACE is specified|
|Start of the Pixel Array (the bitmap Data)| | | | |
|7Ah|4|`FF 00 00 7F`|255 0 0 127|Blue (Alpha: 127), Pixel (0,1)|
|7Eh|4|`00 FF 00 7F`|0 255 0 127|Green (Alpha: 127), Pixel (1,1)|
|82h|4|`00 00 FF 7F`|0 0 255 127|Red (Alpha: 127), Pixel (2,1)|
|86h|4|`FF FF FF 7F`|255 255 255 127|White (Alpha: 127), Pixel (3,1)|
|8Ah|4|`FF 00 00 FF`|255 0 0 255|Blue (Alpha: 255), Pixel (0,0)|
|8Eh|4|`00 FF 00 FF`|0 255 0 255|Green (Alpha: 255), Pixel (1,0)|
|92h|4|`00 00 FF FF`|0 0 255 255|Red (Alpha: 255), Pixel (2,0)|
|96h|4|`FF FF FF FF`|255 255 255 255|White (Alpha: 255), Pixel (3,0)|
Note that the bitmap data starts with the lower left hand corner of the image.
## [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=13)]Usage of BMP format
The simplicity of the BMP file format, and its widespread familiarity in Windows and elsewhere, as well as the fact that this format is relatively well [documented](http://en.wikipedia.org/wiki/Documentation) and
 free of patents, makes it a very common format that image processing programs from many operating systems can read and write.
Many older [graphical user interfaces](http://en.wikipedia.org/wiki/Graphical_user_interface) used
 bitmaps in their built-in graphics subsystems;[[12]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-11) for
 example, the Microsoft Windows and OS/2 platforms'[GDI](http://en.wikipedia.org/wiki/Graphics_Device_Interface) subsystem, where the specific format used is the *Windows and OS/2 bitmap file format*, usually named with the [file extension](http://en.wikipedia.org/wiki/File_extension) of `.BMP` or `.DIB`.
While most BMP files have a relatively large file size due to lack of any compression (or generally low-ratio [run-length
 encoding](http://en.wikipedia.org/wiki/Run-length_encoding) on palletized images), many BMP files can be considerably compressed with [lossless
 data compression](http://en.wikipedia.org/wiki/Lossless_data_compression) algorithms such as [ZIP](http://en.wikipedia.org/wiki/ZIP_(file_format)) because they contain redundant data. Some formats, such
 as [RAR](http://en.wikipedia.org/wiki/RAR), even include routines specifically targeted at efficient compression of such data.
## [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=14)]Related formats
Main article: [Image file formats](http://en.wikipedia.org/wiki/Image_file_formats)
The [X Window System](http://en.wikipedia.org/wiki/X_Window_System) uses a similar [XBM](http://en.wikipedia.org/wiki/X_Bitmap) format
 for black-and-white images, and [XPM](http://en.wikipedia.org/wiki/X_PixMap) (*pixelmap*) for color images. There are also a variety of "raw" formats, which saves raw data
 with no other information. The [Portable Pixmap](http://en.wikipedia.org/wiki/Portable_Pixmap) (PPM) and [Truevision
 TGA](http://en.wikipedia.org/wiki/Truevision_TGA) formats also exist, but are less often used – or only for special purposes; for example, TGA can contain transparency information.
Numerous other bitmap file formats are in existence, though most are not widely used.[[13]](http://en.wikipedia.org/wiki/BMP_file_format#cite_note-12)
## [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=15)]See also
- [Comparison of graphics file formats](http://en.wikipedia.org/wiki/Comparison_of_graphics_file_formats)
## [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=16)]References
- ^ [**a**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBhelp_0-0)[**b**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBhelp_0-1)[**c**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBhelp_0-2)[**d**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBhelp_0-3)[**e**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBhelp_0-4)[**f**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBhelp_0-5)[**g**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBhelp_0-6)[**h**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBhelp_0-7)[**i**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBhelp_0-8)[**j**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBhelp_0-9)[**k**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBhelp_0-10)["DIBs
 and Their Uses"](http://support.microsoft.com/kb/q81498/). *Microsoft Help and Support*. 2005-02-11.
- ^ [**a**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-AlphaBitFields_1-0)[**b**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-AlphaBitFields_1-1)[**c**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-AlphaBitFields_1-2)MSDN
 - BITMAPINFOHEADER: [BI_ALPHABITFIELDS in biCompression member](http://msdn.microsoft.com/en-us/library/aa452885.aspx)
- ^ [**a**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBHeaderTypes_2-0)[**b**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBHeaderTypes_2-1)[**c**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-DIBHeaderTypes_2-2)MSDN [Bitmap
 Header Types](http://msdn.microsoft.com/en-us/library/dd183386%28VS.85%29.aspx)
- ^ [**a**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-BITMAPINFO_3-0)[**b**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-BITMAPINFO_3-1)[**c**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-BITMAPINFO_3-2)MSDN [BITMAPINFO
 Structure](http://msdn.microsoft.com/en-us/library/dd183375%28VS.85%29.aspx)
- **[^](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-PackedDIB_4-0)**Feng
 Yuan - Windows graphics programming: Win32 GDI and DirectDraw: [Packed Device-Independent
 Bitmap (CreateDIBPatternBrush, CreateDIBPatternBrushPt, FindResource, LoadResource, LockResource)](http://books.google.com/books?id=-O92IIF1Bj4C&pg=PA595)
- **[^](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-5)**["JPEG
 and PNG Extensions for Specific Bitmap Functions and Structures"](http://msdn.microsoft.com/en-us/library/dd145023(VS.85).aspx).
- ^ [**a**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-OS2gff_6-0)[**b**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-OS2gff_6-1)["GFF
 Format Summary: OS/2 Bitmap"](http://netghost.narod.ru/gff/graphics/summary/os2bmp.htm).
- **[^](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-7)**MSDN
 - BITMAPV4HEADER: [The member bV4AlphaMask](http://msdn.microsoft.com/en-us/library/dd183380%28VS.85%29.aspx)
- **[^](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-8)**MSDN
 - RGBQUAD: [rgbReserved member](http://msdn.microsoft.com/en-us/library/dd162938%28VS.85%29.aspx)
- **[^](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-9)**MSDN
 - BITMAPINFOHEADER: [The member biBitCount](http://msdn.microsoft.com/en-us/library/dd183376%28VS.85%29.aspx)
- ^ [**a**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-PSformats_10-0)[**b**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-PSformats_10-1)[**c**](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-PSformats_10-2)MSDN: [Windows
 CE - BITMAPINFOHEADER Structure](http://msdn.microsoft.com/en-us/library/ms959648.aspx)
- **[^](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-11)**Julian
 Smart, Stefan Csomor, and Kevin Hock (2006). [*Cross-Platform GUI Programming with Wxwidgets*](http://books.google.com/books?id=CyMsvtgnq0QC&pg=PA265&dq=bitmap+pixmap+gui). Prentice Hall. [ISBN](http://en.wikipedia.org/wiki/International_Standard_Book_Number)[0-13-147381-6](http://en.wikipedia.org/wiki/Special:BookSources/0-13-147381-6).
- **[^](http://en.wikipedia.org/wiki/BMP_file_format#cite_ref-12)**["List
 of bitmap file types"](http://www.file-extensions.org/filetype/extensions/name/Bitmap+image/). *Search File-Extensions.org*.
## [[edit](http://en.wikipedia.org/w/index.php?title=BMP_file_format&action=edit&section=17)]External links
- [For a table view of the bitmap file format](http://atlc.sourceforge.net/bmp.html#_toc381201084) at sourceforge
- [Bitmap File Structure](http://www.digicamsoft.com/bmp/bmp.html)
- [An introduction to DIBs (Device Independent Bitmaps)](http://www.herdsoft.com/ti/davincie/imex3j8i.htm)
- [BMP test images](http://vaxa.wvnet.edu/vmswww/bmp.html)
- [Simple bitmap loader C++ class](http://www.kalytta.com/bitmap.h)
- [The BMP File Format, Part 1 By David Charlap, Dr. Dobb's
 journal of software tools, Volume 20, March 1995](http://drdobbs.com/architecture-and-design/184409517)
- [Another source for BMP entry in GFF (the Encyclopedia of Graphic
 File Formats)](http://www.fileformat.info/format/bmp/egff.htm)

