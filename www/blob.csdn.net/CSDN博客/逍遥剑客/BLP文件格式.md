# BLP文件格式 - 逍遥剑客 - CSDN博客
2008年12月03日 01:33:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4572
**.BLP** files are texture files used in games made by [Blizzard Entertainment](http://en.wikipedia.org/wiki/Blizzard_Entertainment), also used in other games like [Neverwinter Nights](http://en.wikipedia.org/wiki/Neverwinter_Nights). While Blizzard provides an [Interface AddOn Kit](http://www.blizzard.com/support/wow/?id=aww01671p) for extracting the [user interface](http://en.wikipedia.org/wiki/User_interface) files from the *[World of Warcraft](http://en.wikipedia.org/wiki/World_of_Warcraft)*[.MPQs](http://en.wikipedia.org/wiki/.MPQ), they do not provide a utility to view the .BLP files contained within. Also, .BLP graphics were used for Warcraft III, and are also stored within [.MPQs](http://en.wikipedia.org/wiki/.MPQ).
**Format**The file starts with a proprietary header, followed by the texture data. The texture data are typically stored in [DXT1](http://en.wikipedia.org/wiki/DXT1), [DXT3](http://en.wikipedia.org/wiki/DXT3), uncompressed or possibly [DXT5](http://en.wikipedia.org/wiki/DXT5). It is important to note that the size of the image is a power of 2.
**Converters**
Several third party applications exist that can convert .BLP files to [.tga](http://en.wikipedia.org/wiki/Truevision_TGA) files and vice versa. Some of the programs were made for *[Warcraft III](http://en.wikipedia.org/wiki/Warcraft_III)* textures, however, and don't work for all Blizzard game textures. These converters have become very important to the *Warcraft III* modding community, and have been the doorway to the vast amount of customised skins available for download.
**Specification**
Note that all types are [little-endian](http://en.wikipedia.org/wiki/Little-endian).
Only the BLP2 format is shown here; for the BLP1 format, refer to [BLP version 1 format specifications](http://files.magosx.com/download.php?file=MagosBlpFormat.txt)
**BLP2 Format**
struct BLP2Header
{
    FourCC      ID; // Always 'BLP2' 
    UInt32      Type;
    UInt8       Encoding;
    UInt8       AlphaDepth;
    UInt8       AlphaEncoding;
    UInt8       HasMips;
    UInt32      Width;
    UInt32      Height;
    UInt32      Offsets[16];
    UInt32      Lengths[16];
    RGBAColor8  Palette[256];
};
**Type**    0: JPEG compression
    1: Uncompressed or DirectX compression
**Encoding**
    1: Uncompressed
    2: DirectX compression
**AlphaDepth**
    0: No alpha channel
    1: 1 bit alpha
    8: 8 bit alpha
**AlphaEncoding**
    0: DXT1 alpha (0 or 1 bit alpha)
    1: DXT2/3 alpha (4 bit alpha)
    7: DXT4/5 alpha (interpolated alpha)
**HasMips**
    0: No mip levels
    1: Mip levels present (the number of levels is determined by the image size)
**Width, Height**: Dimensions of the image in pixels (always a power of two)
**Offsets[0]**: Offset from the start of the file to the image data
**Lengths[0]**: Length in bytes of the image data
**Palette**: 4-byte BGRA color values for paletted textures (this field is present regardless of whether the texture actually uses palettes
**Type 1 Encoding 1 AlphaDepth 0 (uncompressed paletted image with no alpha)**
Each byte of the image data is an index into Palette which contains the actual RGB value for the pixel. Although the palette entries are 32-bits, the alpha value of each Palette entry may contain garbage and should be discarded.
**Type 1 Encoding 1 AlphaDepth 1 (uncompressed paletted image with 1-bit alpha)**
This is the same as Type 1 Encoding 1 AlphaDepth 0 except that immediately following the index array is a second image array containing 1-bit alpha values for each pixel. The first byte of the array is for pixels 0 through 7, the second byte for pixels 8 through 15 and so on. Bit 0 of each byte corresponds to the first pixel (leftmost) in the group, bit 7 to the rightmost. A set bit indicates the pixel is opaque while a zero bit indicates a transparent pixel.
**Type 1 Encoding 1 AlphaDepth 8 (uncompressed paletted image with 8-bit alpha)**
This is the same as Type 1 Encoding 1 AlphaDepth 0 except that immediately following the index array is a second image array containing the actual 8-bit alpha values for each pixel. This second array starts at BLP2Header.Offset[0] + BLP2Header.Width * BLP2Header.Height.
**Type 1 Encoding 2 AlphaDepth 0 (**[**DXT1**](http://en.wikipedia.org/wiki/DXT1)** no alpha)**
The image data are formatted using [DXT1](http://en.wikipedia.org/wiki/DXT1) compression with no alpha channel.
**Type 1 Encoding 2 AlphaDepth 1 (**[**DXT1**](http://en.wikipedia.org/wiki/DXT1)** one bit alpha)**
The image data are formatted using [DXT1](http://en.wikipedia.org/wiki/DXT1) compression with a one-bit alpha channel.
**Type 1 Encoding 2 AlphaDepth 8 AlphaEncoding 1(**[**DXT3**](http://en.wikipedia.org/wiki/DXT3)**)**
The image data are formatted using [DXT3](http://en.wikipedia.org/wiki/DXT3) compression.
**Type 1 Encoding 2 AlphaDepth 8 AlphaEncoding 7(**[**DXT5**](http://en.wikipedia.org/wiki/DXT5)**)**
The image data are formatted using [DXT5](http://en.wikipedia.org/wiki/DXT5) compression.
**External links**
- [.blp to .tga (and vice versa) converter](http://www.hiveworkshop.com/resources_new/tools/346/) for [Mac OS X](http://en.wikipedia.org/wiki/Mac_OS_X)
- [.blp viewer](http://blp.dynalias.net/Public/BLP_Viewer/) (for [World of Warcraft](http://en.wikipedia.org/wiki/World_of_Warcraft) BLPs) for [Mac OS X](http://en.wikipedia.org/wiki/Mac_OS_X)
- [XnView](http://xnview.com/) Graphic file browser for [Windows OS](http://en.wikipedia.org/wiki/Windows_OS) that also handles .blp files 
- [The Hive Workshop's skin database](http://www.hiveworkshop.com/resources_new/skins/), currently at over 2300 *Warcraft III* skins.
