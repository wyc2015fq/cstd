# SkBitmap类介绍 - xqhrs232的专栏 - CSDN博客
2011年06月23日 17:45:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：5895
原文地址::[http://blogold.chinaunix.net/u2/81094/showart_1867387.html](http://blogold.chinaunix.net/u2/81094/showart_1867387.html)
The SkBitmap class is internally reference counted. To prevent memory errors, it is generally best to copy the objects rather than hold pointers to
 them. These copies are lightweight so you should not be concerned about performance.
SkBitmap
内部有参考计数器。为了防止内存错误，最好是copy
对象，而不是使用指针。这些copy是轻量级的copy，不会引起效能问题。
Bitmap: SkBitmap.h
//SkBitmap.h
/** /class SkBitmap
The SkBitmap class specifies a raster bitmap. A bitmap has an integer width
and height, and a format (config), and a pointer to the actual pixels.
Bitmaps can be drawn into a SkCanvas, but they are also used to specify the target
of a SkCanvas' drawing operations.
*/
//SkBitmap是光栅位图。有整数的宽高和格式。可以直接访问像素。能够用来做SkCanvas画布。
classSkBitmap {
public:
classAllocator;
enum Config {
kNo_Config,//!< bitmap has not been configured
kA1_Config,//!< 1-bit per pixel, (0 is transparent, 1 is opaque)
kA8_Config,//!< 8-bits per pixel, with only alpha specified (0 is transparent, 0xFF is opaque)
kIndex8_Config,//!< 8-bits per pixel, using SkColorTable to specify the colors
kRGB_565_Config,//!< 16-bits per pixel, (see SkColorPriv.h for packing)
kARGB_4444_Config,//!< 16-bits per pixel, (see SkColorPriv.h for packing)
kARGB_8888_Config,//!< 32-bits per pixel, (see SkColorPriv.h for packing)
kRLE_Index8_Config,
kConfigCount
};
/** Default construct creates a bitmap with zero width and height, and no pixels.
Its config is set to kNo_Config.
*/
SkBitmap();
/** Constructor initializes the new bitmap by copying the src bitmap. All fields are copied,
but ownership of the pixels remains with the src bitmap.
*/
SkBitmap(constSkBitmap& src);
/** Decrements our (shared) pixel ownership if needed.
*/
~SkBitmap();
/** Copies the src bitmap into this bitmap. Ownership of the src bitmap's pixels remains
with the src bitmap.
*/
SkBitmap& operator=(constSkBitmap& src);
/** Swap the fields of the two bitmaps. This routine is guaranteed to never fail or throw.
*/
//This method is not exported to java.
voidswap(SkBitmap&
other);
//把当前的SkBitmap和另外的SkBitmap
 other做交换。
/** Return true if the bitmap has empty dimensions.
*/
boolempty()
const { return 0 ==
fWidth || 0 == fHeight; }
//是否有宽高
/** Return true if the bitmap has no pixels nor a pixelref. Note: this can
return true even if the dimensions of the bitmap are > 0 (see empty()).
*/
boolisNull()
const { return
NULL == fPixels && 
NULL == fPixelRef; }
//是否有像素
/** Return the config for the bitmap.
*/
Configconfig() 
const { return (Config)fConfig; }
/** DEPRECATED, use config()
*/
ConfiggetConfig()
const { return
this->config(); }
/** Return the bitmap's width, in pixels.
*/
intwidth()
const { return
fWidth; }
/** Return the bitmap's height, in pixels.
*/
intheight()
const { return
fHeight; }
/** Return the number of bytes between subsequent rows of the bitmap.
*/
introwBytes()
const { return
fRowBytes; }
//位图的宽，高，格式，以每行的字节数
/** Return the shift amount per pixel (i.e. 0 for 1-byte per pixel, 1 for
2-bytes per pixel configs, 2 for 4-bytes per pixel configs). Return 0
for configs that are not at least 1-byte per pixel (e.g. kA1_Config
or kNo_Config)
*/
intshiftPerPixel()
const { return
fBytesPerPixel >> 1; }
/** Return the number of bytes per pixel based on the config. If the config
does not have at least 1 byte per (e.g. kA1_Config) then 0 is returned.
*/
intbytesPerPixel()
const { return
fBytesPerPixel; }
//每像素的字节数
/** Return the rowbytes expressed as a number of pixels (like width and
height). Note, for 1-byte per pixel configs like kA8_Config, this will
return the same as rowBytes(). Is undefined for configs that are less
than 1-byte per pixel (e.g. kA1_Config)
*/
introwBytesAsPixels()
const { return
fRowBytes >> (fBytesPerPixel >> 1); }
//一行的像素数
/** Return the address of the pixels for this SkBitmap.
*/
void* getPixels()
const { return
fPixels; }
//位图数据的起始地址
/** Return the byte size of the pixels, based on the height and rowBytes.
Note this truncates the result to 32bits. Call getSize64() to detect
if the real size exceeds 32bits.
*/
size_tgetSize()
const { return
fHeight * fRowBytes; }
//获得像素的字节空间大小，如果大小草根32位（即4294967296）则会截断，也就是只取最后的32位。
//可以使用getSize64()函数代替，不过会稍微慢一点。
/** Return the byte size of the pixels, based on the height and rowBytes.
This routine is slightly slower than getSize(), but does not truncate
the answer to 32bits.
*/
Sk64 getSize64() 
const {
Sk64 size;
size.setMul(fHeight,
fRowBytes);
returnsize;
}
/** Returns true if the bitmap is opaque (has no translucent/transparent pixels).
*/
boolisOpaque()
const;
/** Specify if this bitmap's pixels are all opaque or not. Is only meaningful for configs
that support per-pixel alpha (RGB32, A1, A8).
*/
voidsetIsOpaque(bool);
//根据当前的格式判断是否为不透明的。
/** Reset the bitmap to its initial state (see default constructor). If we are a (shared)
owner of the pixels, that ownership is decremented.
*/
voidreset();
//清楚当前位图状态，释放位图
/** Given a config and a width, this computes the optimal rowBytes value. This is called automatically
if you pass 0 for rowBytes to setConfig().
*/
staticintComputeRowBytes(Config 
c, intwidth);
/** Return the bytes-per-pixel for the specified config. If the config is
not at least 1-byte per pixel, return 0, including for kNo_Config.
*/
staticintComputeBytesPerPixel(Config 
c);
/** Return the shift-per-pixel for the specified config. If the config is
not at least 1-byte per pixel, return 0, including for kNo_Config.
*/
staticintComputeShiftPerPixel(Config 
c) {
returnComputeBytesPerPixel(c) >> 1;
}
static Sk64 ComputeSize64(Config,
intwidth, 
intheight);
staticsize_tComputeSize(Config, intwidth, int
height);
//根据参数计算一些需要的值
/** Set the bitmap's config and dimensions. If rowBytes is 0, then
ComputeRowBytes() is called to compute the optimal value. This resets
any pixel/colortable ownership, just like reset().
*/
voidsetConfig(Config,
intwidth, 
intheight, introwBytes = 0);
//设置格式
/** Use this to assign a new pixel address for an existing bitmap. This
will automatically release any pixelref previously installed. Only call
this if you are handling ownership/lifetime of the pixel memory.
If the bitmap retains a reference to the colortable (assuming it is
not null) it will take care of incrementing the reference count.
@param pixelsAddress for the pixels, managed by the caller.
@param ctableColorTable (or null) that matches the specified pixels
*/
voidsetPixels(void*
p, SkColorTable* 
ctable = NULL);
//使用已有的图像数据作为当前位图的数据，这里要注意的是如何确保图像数据和宽，高，格式等的匹配。
/** Use the standard HeapAllocator to create the pixelref that manages the
pixel memory. It will be sized based on the current width/height/config.
If this is called multiple times, a new pixelref object will be created
each time.
If the bitmap retains a reference to the colortable (assuming it is
not null) it will take care of incrementing the reference count.
@param ctableColorTable (or null) to use with the pixels that will
be allocated. Only used if config == Index8_Config
@return true if the allocation succeeds. If not the pixelref field of
the bitmap will be unchanged.
*/
boolallocPixels(SkColorTable*
ctable = NULL) {
returnthis->allocPixels(NULL,
ctable);
}
/** Use the specified Allocator to create the pixelref that manages the
pixel memory. It will be sized based on the current width/height/config.
If this is called multiple times, a new pixelref object will be created
each time.
If the bitmap retains a reference to the colortable (assuming it is
not null) it will take care of incrementing the reference count.
@param allocator The Allocator to use to create a pixelref that can
manage the pixel memory for the current
width/height/config. If allocator is NULL, the standard
HeapAllocator will be used.
@param ctableColorTable (or null) to use with the pixels that will
be allocated. Only used if config == Index8_Config.
If it is non-null and the config is not Index8, it will
be ignored.
@return true if the allocation succeeds. If not the pixelref field of
the bitmap will be unchanged.
*/
boolallocPixels(Allocator*
allocator, SkColorTable*
ctable);
//这个函数很重要，创建位图数据参考计数器。因为只有有了参考计数器，才能对位图的多重引用产生作用。
//注意，这里也会分配位图的内存空间。空间的大小是当前图像配置的大小。这里同样可以看到，对位图数据访问时需要lock和unlock.
/** Return the current pixelref object, of any
*/
SkPixelRef* pixelRef() 
const { returnfPixelRef; }
/** Return the offset into the pixelref, if any. Will return 0 if there is
no pixelref installed.
*/
size_tpixelRefOffset()
const { return
fPixelRefOffset; }
/** Assign a pixelref and optional offset. Pixelrefs are reference counted,
so the existing one (if any) will be unref'd and the new one will be
ref'd.
*/
SkPixelRef* setPixelRef(SkPixelRef* 
pr, size_toffset = 0);
/** Call this to ensure that the bitmap points to the current pixel address
in the pixelref. Balance it with a call to unlockPixels(). These calls
are harmless if there is no pixelref.
*/
voidlockPixels()
const;
/** When you are finished access the pixel memory, call this to balance a
previous call to lockPixels(). This allows pixelrefs that implement
cached/deferred image decoding to know when there are active clients of
a given image.
*/
voidunlockPixels()
const;
//在访问时对图像数据进行锁定
/** Call this to be sure that the bitmap is valid enough to be drawn (i.e.
it has non-null pixels, and if required by its config, it has a
non-null colortable. Returns true if all of the above are met.
*/
boolreadyToDraw()
const {
returnthis->getPixels() !=
NULL &&
((this->config() != kIndex8_Config &&
this->config() != kRLE_Index8_Config) ||
fColorTable != 
NULL);
}
//确保当前SkBitmap可以用来绘图了。
/** Return the bitmap's colortable (if any). Does not affect the colortable's
reference count.
*/
SkColorTable* 
getColorTable() const { 
returnfColorTable; }
/** Returns a non-zero, unique value corresponding to the pixels in our
pixelref, or 0 if we do not have a pixelref. Each time the pixels are
changed (and notifyPixelsChanged is called), a different generation ID
will be returned.
*/
uint32_t getGenerationID() 
const;
/** Call this if you have changed the contents of the pixels. This will in-
turn cause a different generation ID value to be returned from
getGenerationID().
*/
voidnotifyPixelsChanged()
const;
//通知图像有改变
/** Initialize the bitmap's pixels with the specified color+alpha, automatically converting into the correct format
for the bitmap's config. If the config is kRGB_565_Config, then the alpha value is ignored.
If the config is kA8_Config, then the r,g,b parameters are ignored.
*/
voideraseARGB(U8CPU
a, U8CPU r, U8CPU
g, U8CPU b) 
const;
/** Initialize the bitmap's pixels with the specified color+alpha, automatically converting into the correct format
for the bitmap's config. If the config is kRGB_565_Config, then the alpha value is presumed
to be 0xFF. If the config is kA8_Config, then the r,g,b parameters are ignored and the
pixels are all set to 0xFF.
*/
voideraseRGB(U8CPU
r, U8CPU g, U8CPU
b) const {
this->eraseARGB(0xFF,
r, g, 
b);
}
/** Initialize the bitmap's pixels with the specified color, automatically converting into the correct format
for the bitmap's config. If the config is kRGB_565_Config, then the color's alpha value is presumed
to be 0xFF. If the config is kA8_Config, then only the color's alpha value is used.
*/
voideraseColor(SkColor
c) const {
this->eraseARGB(SkColorGetA(c), SkColorGetR(c), SkColorGetG(c),
SkColorGetB(c));
}
//用指定的颜色擦除
/** Scroll (a subset of) the contents of this bitmap by dx/dy. If there are
no pixels allocated (i.e. getPixels() returns null) the method will
still update the inval region (if present).
@param subset The subset of the bitmap to scroll/move. To scroll the
entire contents, specify [0, 0, width, height] or just
pass null.
@param dx The amount to scroll in X
@param dy The amount to scroll in Y
@param inval Optional (may be null). Returns the area of the bitmap that
was scrolled away. E.g. if dx = dy = 0, then inval would
be set to empty. If dx >= width or dy >= height, then
inval would be set to the entire bounds of the bitmap.
@return true if the scroll was doable. Will return false if the bitmap
uses an unsupported config for scrolling (only kA8,
kIndex8, kRGB_565, kARGB_4444, kARGB_8888 are supported).
If no pixels are present (i.e. getPixels() returns false)
inval will still be updated, and true will be returned.
*/
boolscrollRect(const SkIRect*
subset, int
dx, intdy,
SkRegion* inval = 
NULL) const;
/** Returns the address of the specified pixel. This performs a runtime
check to know the size of the pixels, and will return the same answer
as the corresponding size-specific method (e.g. getAddr16). Since the
check happens at runtime, it is much slower than using a size-specific
version. Unlike the size-specific methods, this routine also checks if
getPixels() returns null, and returns that. The size-specific routines
perform a debugging assert that getPixels() is not null, but they do
not do any runtime checks.
*/
void* getAddr(intx, int
y) const;
//获得指定点的地址
/** Returns the address of the pixel specified by x,y for 32bit pixels.
*/
inline uint32_t* 
getAddr32(intx,
inty) 
const;
/** Returns the address of the pixel specified by x,y for 16bit pixels.
*/
inline uint16_t* 
getAddr16(intx,
inty) 
const;
/** Returns the address of the pixel specified by x,y for 8bit pixels.
*/
inline uint8_t* 
getAddr8(intx,
inty) 
const;
/** Returns the address of the byte containing the pixel specified by x,y
for 1bit pixels.
*/
inline uint8_t* 
getAddr1(intx,
inty) 
const;
/** Returns the color corresponding to the pixel specified by x,y for
colortable based bitmaps.
*/
inline SkPMColor 
getIndex8Color(int
x, inty) 
const;
/** Set dst to be a set of this bitmap. If possible, it will share the
pixel memory, and just point into a subset of it. However, if the config
does not support this, a local copy will be made and associated with
the dst bitmap. If the subset rectangle, intersected with the bitmap's
dimensions is empty, or if there is an unsupported config, false will be
returned and dst will be untouched.
@param dstThe bitmap that will be set to a subset of this bitmap
@param subset The rectangle of pixels in this bitmap that dst will
reference.
@return true if the subset copy was successfully made.
*/
boolextractSubset(SkBitmap*
dst, const SkIRect&
subset) const;
/** Tries to make a new bitmap based on the dimensions of this bitmap,
setting the new bitmap's config to the one specified, and then copying
this bitmap's pixels into the new bitmap. If the conversion is not
supported, or the allocator fails, then this method returns false and
dst is left unchanged.
@param dstThe bitmap to be sized and allocated
@param c The desired config for dst
@param allocator Allocator used to allocate the pixelref for the dst
bitmap. If this is null, the standard HeapAllocator
will be used.
@return true if the copy could be made.
*/
boolcopyTo(SkBitmap*
dst, Config c, 
Allocator* allocator = 
NULL) const;
boolhasMipMap()
const;
voidbuildMipMap(boolforceRebuild = false);
voidfreeMipMap();
//在做绘图时使用了MipMap结构。这个结构可以防止拷贝。具体功能还没有细看。
/** Given scale factors sx, sy, determine the miplevel available in the
bitmap, and return it (this is the amount to shift matrix iterators
by). If dst is not null, it is set to the correct level.
*/
intextractMipLevel(SkBitmap*
dst, SkFixed sx, SkFixed
sy);
voidextractAlpha(SkBitmap*
dst) const {
this->extractAlpha(dst,
NULL, NULL);
}
voidextractAlpha(SkBitmap*
dst, const SkPaint*
paint,
SkIPoint* offset) 
const;
voidflatten(SkFlattenableWriteBuffer&)
const;
voidunflatten(SkFlattenableReadBuffer&);
SkDEBUGCODE(void
validate() const;)
classAllocator :
public SkRefCnt {
public:
/** Allocate the pixel memory for the bitmap, given its dimensions and
config. Return true on success, where success means either setPixels
or setPixelRef was called. The pixels need not be locked when this
returns. If the config requires a colortable, it also must be
installed via setColorTable. If false is returned, the bitmap and
colortable should be left unchanged.
*/
virtualboolallocPixelRef(SkBitmap*,
SkColorTable*) = 0;
//这里分配位图的空间
};
/** Subclass of Allocator that returns a pixelref that allocates its pixel
memory from the heap. This is the default Allocator invoked by
allocPixels().
*/
classHeapAllocator :
publicAllocator {
public:
virtualboolallocPixelRef(SkBitmap*,
SkColorTable*);
};
classRLEPixels {
public:
RLEPixels(intwidth, int
height);
virtual ~RLEPixels();
uint8_t* packedAtY(inty) const {
SkASSERT((unsigned)y < (unsigned)fHeight);
returnfYPtrs[y];
}
// called by subclasses during creation
voidsetPackedAtY(inty, uint8_t* addr) {
SkASSERT((unsigned)y < (unsigned)fHeight);
fYPtrs[y] =
addr;
}
private:
uint8_t** fYPtrs;
intfHeight;
};
private:
#ifdef
SK_SUPPORT_MIPMAP
structMipMap;
mutableMipMap*
fMipMap;
#endif
mutable SkPixelRef* 
fPixelRef;
//位图的引用参考计数器
mutablesize_tfPixelRefOffset;
//位图的引用参考计数器的偏移？
mutableintfPixelLockCount;
//位图数据锁定计数器
// either user-specified (in which case it is not treated as mutable)
// or a cache of the returned value from fPixelRef->lockPixels()
mutablevoid*fPixels;
//位图数据
mutableSkColorTable*
fColorTable;// only meaningful for kIndex8
enum Flags {
kImageIsOpaque_Flag= 0x01
};
uint32_tfRowBytes;
//位图一行的字节数
uint16_tfWidth, 
fHeight;
//位图宽高
uint8_tfConfig;
//位图格式
uint8_tfFlags;
//图像是否为不透明的标签，有两种状况，一种是图像有一部为透明的，一部份为不透明的；还有一种状况是整个图像有同样的透明度。
uint8_tfBytesPerPixel;
// based on config
//位图的一个像素占用的字节数
/*Unreference any pixelrefs or colortables
*/
voidfreePixels();
voidupdatePixelsFromRef()
const;
static SkFixed 
ComputeMipLevel(SkFixed sx, SkFixed 
dy);
};
SkBitmap是个位图类。但它的内部有自己的一些逻辑，尤其是使用了参考计数器，可以避免多重copy，同样增加了锁机制。关键的是位图的空间可以由外部给定。
