# 使用libpng读取png图像 - xqhrs232的专栏 - CSDN博客
2012年06月22日 22:21:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3385
原文地址::[http://hi.baidu.com/foolwish/item/7ffe763133197f38b2c0c59f](http://hi.baidu.com/foolwish/item/7ffe763133197f38b2c0c59f)
正在看Non-Local Mean的文章，发现这方面的程序还真不好找，文章挺多，但是作者大部分都不给程序，或者给出的也是linux下的程序。好不容易找到一个，说是windows上也能用，但是调试了才发现里面用的一些头文件什么的都是linux里的，可以找到，但是也比较麻烦。里面使用到了libpng的东西，在网上查了下，发现下面这篇文章比较有用，搬过来放着。其中需要下载的代码的网址：
libpng1.4.7：[http://sourceforge.net/projects/libpng/files/libpng14/1.4.7/](http://sourceforge.net/projects/libpng/files/libpng14/1.4.7/)
zlib:[http://gnuwin32.sourceforge.net/packages/zlib.htm](http://gnuwin32.sourceforge.net/packages/zlib.htm)
额，突然发现其实有现成的lib文件可以用：
[http://b.qzone.qq.com/cgi-bin/blognew/blog_output_data?uin=908384536&blogid=1270347158&styledm=ctc.qzonestyle.gtimg.cn&imgdm=ctc.qzs.qq.com&bdm=b.qzone.qq.com&mode=2&numperpage=15&property=GoRE×tamp=1272339979](http://b.qzone.qq.com/cgi-bin/blognew/blog_output_data?uin=908384536&blogid=1270347158&styledm=ctc.qzonestyle.gtimg.cn&imgdm=ctc.qzs.qq.com&bdm=b.qzone.qq.com&mode=2&numperpage=15&property=GoRE&timestamp=1272339979)
里面有/MD和/MT版本的lib下载，也有源代码
再贴个DCMTK的网址，医学图像的工具，说不定哪天用上了
[http://bbs.hc3i.cn/viewthread.php?tid=26234&fromuid=7](http://bbs.hc3i.cn/viewthread.php?tid=26234&fromuid=7)
[使用libpng读取PNG图片像素数据](http://blog.csdn.net/yshen_dublin/article/details/4954011)
转自：http://blog.csdn.net/yshen_dublin/article/details/4954011
libpng 是一套免费的、公开源代码的程序库，支持对 PNG 图形文件的创建、读写等操作。可以到[www.libpng.org](http://www.libpng.org/) 上下载源代码。libpng 使用 zlib 程序库作为压缩引擎，zlib 也是著名的 gzip (GNU zip) 所采用的压缩引擎。zlib是通用的压缩库，提供了一套 in-memory 压缩和解压函数，并能检测解压出来的数据的完整性(integrity)。zlib
 也支持读写gzip (.gz) 格式的文件这些都是免费和公开的哦。在下载了pbglib，并且要下载zlib，如何才能加到代码中呢？以VC6.为例来说明。其他CBuild等编译器相信也一样的。比如我们从网上下载了一个lpng1210.zip和zlib-1.2.3.tar.tar。首先把他们解压。在lpng的目录中寻找projects/visualc6/libpng.dsw。并在zlib中寻找zlib-1.2.3/projects/visualc6/zlib.dsw。用vc打开libpng工程文件，把zlib的工程文件也添加进去。设置好在setting中设置好的zlib.lib和pbg.lib的输出路径，以及include路径（这个简单方法相信大家都明白吧）。运行后，就生成了两个静态库了。lib做好以后，就可以将lib加到我们要做的工程中去。在link中设置好，并将png.h和zlib.h都加进去。这样就可以正式的使用libpng了！
**下面到重点内容了。如何用pnglib实现对文件的读写？现在让我们来试着写一段代码来读取PNG像素数据(from File or Stream)，并把读取的数据存储到一个数组rgba中,用来生成OpenGl纹理**
typedef struct {
    u8* pixelData;
    int imageWidth;
    int imageHeight;
}ImageInfo;
typedef struct {
    u8* data;
    int size;
    int offset;
}ImageSource;
//----------------------------------------------------------------------------------------------------------------
static void pngReaderCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
    ImageSource* isource = (ImageSource*)png_get_io_ptr(png_ptr);
    if(isource->offset + length <= isource->size)
    {
       memcpy(data, isource->data + isource->offset, length);
       isource->offset += length;
    }
    else
    {
        png_error(png_ptr,"pngReaderCallback failed");
    }
}
//----------------------------------------------------------------------------------------------------------------
ImageInfo*  decodePNGFromStream(const u8* pixelData, const u32& dataSize)
{
    png_structp png_ptr;
    png_infop info_ptr;
    int width, height, rowBytes;
    png_byte color_type;  //可以是PNG_COLOR_TYPE_RGB,PNG_COLOR_TYPE_PALETTE.......等
    png_byte bit_depth;
    png_colorp palette;
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,NULL,NULL);
    if (!png_ptr)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
        TFC_DEBUG("ReadPngFile: Failed to create png_ptr");
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
        TFC_DEBUG("ReadPngFile: Failed to create info_ptr");
    }
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
        TFC_DEBUG("ReadPngFile: Failed to read the PNG file");
    }
    ImageSource imgsource;
    imgsource.data = (u8*)pixelData;
    imgsource.size = dataSize;
    imgsource.offset = 0;
    //define our own callback function for I/O instead of reading from a file
    png_set_read_fn(png_ptr,&imgsource, **pngReaderCallback**);
    /* **************************************************
     * The low-level read interface in libpng (http://www.libpng.org/pub/png/libpng-1.2.5-manual.html)
     * **************************************************
     */
    png_read_info(png_ptr, info_ptr);
    width = info_ptr->width;
    height = info_ptr->height;
    color_type = info_ptr->color_type;
    bit_depth = info_ptr->bit_depth;
    rowBytes = info_ptr->rowbytes;
    palette= info_ptr->palette;
    // Convert stuff to appropriate formats!
    if(color_type==PNG_COLOR_TYPE_PALETTE)
    {
        png_set_packing(png_ptr);
        png_set_palette_to_rgb(png_ptr); //Expand data to 24-bit RGB or 32-bit RGBA if alpha available.
    }
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_gray_1_2_4_to_8(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);
    if (bit_depth == 16)
        png_set_strip_16(png_ptr);
    //Expand paletted or RGB images with transparency to full alpha channels so the data will be available as RGBA quartets.
    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(png_ptr);
    }
    //png_read_update_info(png_ptr, info_ptr);
    u8* rgba = new u8[width * height * 4];  //each pixel(RGBA) has 4 bytes
    png_bytep * row_pointers;
    row_pointers = (png_bytep*)png_malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++)
    {
        row_pointers[y] = (png_bytep)png_malloc(width<<2); //each pixel(RGBA) has 4 bytes
    }
    png_read_image(png_ptr, row_pointers);
    //unlike store the pixel data from top-left corner, store them from bottom-left corner for OGLES Texture drawing...
    int pos = (width * height * 4) - (4 * width);
    for(int row = 0; row < height; row++)
    {
       for(int col = 0; col < (4 * width); col += 4)
       {
           rgba[pos++] = row_pointers[row][col];        // red
           rgba[pos++] = row_pointers[row][col + 1];    // green
           rgba[pos++] = row_pointers[row][col + 2];    // blue
           rgba[pos++] = row_pointers[row][col + 3];    // alpha
       }
       pos=(pos - (width * 4)*2); //move the pointer back two rows
    }
    ImageInfo* imageInfo = (ImageInfo*)kdMalloc(sizeof(ImageInfo));
    imageInfo->pixelData = rgba;
    imageInfo->imageHeight = height;
    imageInfo->imageWidth = width;
    //clean up after the read, and free any memory allocated
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return imageInfo;
}
//----------------------------------------------------------------------------------------------------------------
ImageInfo*   decodePNGFromFile(char* fileName)
{
    char png_header[8];
    png_structp png_ptr;
    png_infop info_ptr;
    int width, height, rowBytes;
    png_byte color_type; 
    png_byte bit_depth;
    png_colorp palette; 
    /* open file and test for it being a png */
    FILE *file = fopen(fileName, "rb");
    fread(png_header, 1, 8, file);
    if(png_sig_cmp((png_bytep)png_header, 0, 8))
    {
        TFC_DEBUG("Not a PNG file...");
        fclose(file);
    }
    /* initialise structures for reading a png file */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
        TFC_DEBUG("ReadPngFile: Failed to read the PNG file");
        fclose(file);
    }
    //I/O initialisation methods
    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, 8);  //Required!!!
    /* **************************************************
     * The high-level read interface in libpng (http://www.libpng.org/pub/png/libpng-1.2.5-manual.html)
     * **************************************************
     */
       png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
       width = info_ptr->width;
       height = info_ptr->height;
       unsigned char* rgba = new unsigned char[width * height * 4];  //each pixel(RGBA) has 4 bytes
       png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);
       //Original PNG pixel data stored from top-left corner, BUT OGLES Texture drawing is from bottom-left corner
       //int pos = 0;
       //for(int row = 0; row < height; row++)
       //{
           //for(int col = 0; col < (4 * width); col += 4)
           //{
              //rgba[pos++] = row_pointers[row][col];     // red
              //rgba[pos++] = row_pointers[row][col + 1]; // green
              //rgba[pos++] = row_pointers[row][col + 2]; // blue
              //rgba[pos++] = row_pointers[row][col + 3]; // alpha
           //}
       //}
       //unlike store the pixel data from top-left corner, store them from bottom-left corner for OGLES Texture drawing...
       int pos = (width * height * 4) - (4 * width);
       for(int row = 0; row < height; row++)
       {
          for(int col = 0; col < (4 * width); col += 4)
          {
              rgba[pos++] = row_pointers[row][col];        // red
              rgba[pos++] = row_pointers[row][col + 1]; // green
              rgba[pos++] = row_pointers[row][col + 2]; // blue
              rgba[pos++] = row_pointers[row][col + 3]; // alpha
          }
          pos=(pos - (width * 4)*2); //move the pointer back two rows
       }
    ImageInfo* imageInfo = (ImageInfo*)kdMalloc(sizeof(ImageInfo));
    imageInfo->pixelData = rgba;
    imageInfo->imageHeight = height;
    imageInfo->imageWidth = width;
    //clean up after the read, and free any memory allocated
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(file);
    return imageInfo;
}
//----------------------------------------------------------------------------------------------------------------
int createPNGTextureFromStream(const u8* pixelData, const u32& dataSize)
{
        GLuint textureID;
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1,&textureID);
        glBindTexture(GL_TEXTURE_2D,textureID);
        ImageInfo* imageInfo = decodePNGFromStream(pixelData, dataSize)； 
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,imageInfo->imageWidth,imageInfo->imageHeight,0,
                                 GL_RGBA,GL_UNSIGNED_BYTE,imageInfo->pixelData);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        delete[] imageInfo->pixelData;
        delete imageInfo;
        return textureID;
}
//----------------------------------------------------------------------------------------------------------------
void main()
{
      //Testcase1: decoding png data from a raw png bufferstream and create the corresponding Texture
      //假设我们从某个地方可以拿到一个unsigned char* 的PNG数据源pixelData.
      int texId = createPNGTextureFromStream(pixelData, dataSize);
      //Testcase2: decoding png data from a given png file and and create the corresponding Texture
      char* fileName = "example.png";
      int texId = createPNGTextureFromFile(fileName);
      //现在我们就可以用创建出来的textureID来绘制纹理了。。。。
}
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
**附录  让我们打开pnglib下面的一个example.c**
/* Read a PNG file. You may want to return an error code if the read 
* fails (depending upon the failure). There are two "prototypes" given 
* here - one where we are given the filename, and we need to open the 
* file, and the other where we are given an open file (possibly with 
* some or all of the magic bytes read - see comments above). 
*/ 
#ifdef open_file /* prototype 1 */ 
void read_png(char *file_name) /* We need to open the file */ 
{ 
定义两个很重要的pngwen文件指针。png_info里面放的是pngchuck和png的数据。
png_structp png_ptr; 
png_infop info_ptr;
这些是读出图像需要的参数，图像高。宽、深度、图像类型、交错类型。
unsigned int sig_read = 0; 
png_uint_32 width, height; 
int bit_depth, color_type, interlace_type; 
FILE *fp;
打开文件，把文件指针给png_prt
if ((fp = fopen(file_name, "rb")) == NULL) 
return (ERROR); 
#else no_open_file /* prototype 2 */ 
void read_png(FILE *fp, unsigned int sig_read) /* file is already open */ 
{ 
png_structp png_ptr; 
png_infop info_ptr; 
png_uint_32 width, height; 
int bit_depth, color_type, interlace_type; 
#endif no_open_file /* only use one prototype! */ 
/* Create and initialize the png_struct with the desired error handler 
* functions. If you want to use the default stderr and longjump method, 
* you can supply NULL for the last three parameters. We also supply the 
* the compiler header file version, so that we know if the application 
* was compiled with a compatible version of the library. REQUIRED 
*/ 
png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
png_voidp user_error_ptr, user_error_fn, user_warning_fn); 
if (png_ptr == NULL) 
{ 
fclose(fp); 
return (ERROR); 
} 
/* Allocate/initialize the memory for image information. REQUIRED. */ 
info_ptr = png_create_info_struct(png_ptr); 
if (info_ptr == NULL) 
{ 
fclose(fp); 
png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL); 
return (ERROR); 
} 
/* Set error handling if you are using the setjmp/longjmp method (this is 
* the normal method of doing things with libpng). REQUIRED unless you 
* set up your own error handlers in the png_create_read_struct() earlier. 
*/ 
if (setjmp(png_jmpbuf(png_ptr))) 
{ 
/* Free all of the memory associated with the png_ptr and info_ptr */ 
png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL); 
fclose(fp); 
/* If we get here, we had a problem reading the file */ 
return (ERROR); 
} 
/* One of the following I/O initialization methods is REQUIRED */ 
#ifdef streams /* PNG file I/O method 1 */ 
/* Set up the input control if you are using standard C streams */ 
png_init_io(png_ptr, fp); 
#else no_streams /* PNG file I/O method 2 */ 
/* If you are using replacement read functions, instead of calling 
* png_init_io() here you would call: 
*/ 
png_set_read_fn(png_ptr, (void *)user_io_ptr, user_read_fn); 
/* where user_io_ptr is a structure you want available to the callbacks */ 
#endif no_streams /* Use only one I/O method! */ 
/* If we have already read some of the signature */ 
png_set_sig_bytes(png_ptr, sig_read); 
#ifdef hilevel 
/* 
* If you have enough memory to read in the entire image at once, 
* and you need to specify only transforms that can be controlled 
* with one of the PNG_TRANSFORM_* bits (this presently excludes 
* dithering, filling, setting background, and doing gamma 
* adjustment), then you can read the entire image (including 
* pixels) into the info structure with this call: 
*/ 
png_read_png(png_ptr, info_ptr, png_transforms, png_voidp_NULL); 
#else 
/* OK, you're doing it the hard way, with the lower-level functions */ 
/* The call to png_read_info() gives us all of the information from the 
* PNG file before the first IDAT (image data chunk). REQUIRED 
*/ 
png_read_info(png_ptr, info_ptr); 
png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 
&interlace_type, int_p_NULL, int_p_NULL); 
/* Set up the data transformations you want. Note that these are all 
* optional. Only call them if you want/need them. Many of the 
* transformations only work on specific types of images, and many 
* are mutually exclusive. 
*/ 
/* tell libpng to strip 16 bit/color files down to 8 bits/color */ 
png_set_strip_16(png_ptr); 
/* Strip alpha bytes from the input data without combining with the 
* background (not recommended). 
*/ 
png_set_strip_alpha(png_ptr); 
/* Extract multiple pixels with bit depths of 1, 2, and 4 from a single 
* byte into separate bytes (useful for paletted and grayscale images). 
*/ 
png_set_packing(png_ptr); 
/* Change the order of packed pixels to least significant bit first 
* (not useful if you are using png_set_packing). */ 
png_set_packswap(png_ptr); 
/* Expand paletted colors into true RGB triplets */ 
if (color_type == PNG_COLOR_TYPE_PALETTE) 
png_set_palette_rgb(png_ptr); 
/* Expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel */ 
if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) 
png_set_gray_1_2_4_to_8(png_ptr); 
/* Expand paletted or RGB images with transparency to full alpha channels 
* so the data will be available as RGBA quartets. 
*/ 
if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) 
png_set_tRNS_to_alpha(png_ptr); 
/* Set the background color to draw transparent and alpha images over. 
* It is possible to set the red, green, and blue components directly 
* for paletted images instead of supplying a palette index. Note that 
* even if the PNG file supplies a background, you are not required to 
* use it - you should use the (solid) application background if it has one. 
*/ 
png_color_16 my_background, *image_background; 
if (png_get_bKGD(png_ptr, info_ptr, &image_background)) 
png_set_background(png_ptr, image_background, 
PNG_BACKGROUND_GAMMA_FILE, 1, 1.0); 
else 
png_set_background(png_ptr, &my_background, 
PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0); 
/* Some suggestions as to how to get a screen gamma value */ 
/* Note that screen gamma is the display_exponent, which includes 
* the CRT_exponent and any correction for viewing conditions */ 
if (/* We have a user-defined screen gamma value */) 
{ 
screen_gamma = user-defined screen_gamma; 
} 
/* This is one way that applications share the same screen gamma value */ 
else if ((gamma_str = getenv("SCREEN_GAMMA")) != NULL) 
{ 
screen_gamma = atof(gamma_str); 
} 
/* If we don't have another value */ 
else 
{ 
screen_gamma = 2.2; /* A good guess for a PC monitors in a dimly 
lit room */ 
screen_gamma = 1.7 or 1.0; /* A good guess for Mac systems */ 
} 
/* Tell libpng to handle the gamma conversion for you. The final call 
* is a good guess for PC generated images, but it should be configurable 
* by the user at run time by the user. It is strongly suggested that 
* your application support gamma correction. 
*/ 
int intent; 
if (png_get_sRGB(png_ptr, info_ptr, &intent)) 
png_set_gamma(png_ptr, screen_gamma, 0.45455); 
else 
{ 
double image_gamma; 
if (png_get_gAMA(png_ptr, info_ptr, &image_gamma)) 
png_set_gamma(png_ptr, screen_gamma, image_gamma); 
else 
png_set_gamma(png_ptr, screen_gamma, 0.45455); 
} 
/* Dither RGB files down to 8 bit palette or reduce palettes 
* to the number of colors available on your screen. 
*/ 
if (color_type & PNG_COLOR_MASK_COLOR) 
{ 
int num_palette; 
png_colorp palette; 
/* This reduces the image to the application supplied palette */ 
if (/* we have our own palette */) 
{ 
/* An array of colors to which the image should be dithered */ 
png_color std_color_cube[MAX_SCREEN_COLORS]; 
png_set_dither(png_ptr, std_color_cube, MAX_SCREEN_COLORS, 
MAX_SCREEN_COLORS, png_uint_16p_NULL, 0); 
} 
/* This reduces the image to the palette supplied in the file */ 
else if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette)) 
{ 
png_uint_16p histogram = NULL; 
png_get_hIST(png_ptr, info_ptr, &histogram); 
png_set_dither(png_ptr, palette, num_palette, 
max_screen_colors, histogram, 0); 
} 
} 
/* invert monochrome files to have 0 as white and 1 as black */ 
png_set_invert_mono(png_ptr); 
/* If you want to shift the pixel values from the range [0,255] or 
* [0,65535] to the original [0,7] or [0,31], or whatever range the 
* colors were originally in: 
*/ 
if (png_get_valid(png_ptr, info_ptr, PNG_INFO_sBIT)) 
{ 
png_color_8p sig_bit; 
png_get_sBIT(png_ptr, info_ptr, &sig_bit); 
png_set_shift(png_ptr, sig_bit); 
} 
/* flip the RGB pixels to BGR (or RGBA to BGRA) */ 
if (color_type & PNG_COLOR_MASK_COLOR) 
png_set_bgr(png_ptr); 
/* swap the RGBA or GA data to ARGB or AG (or BGRA to ABGR) */ 
png_set_swap_alpha(png_ptr); 
/* swap bytes of 16 bit files to least significant byte first */ 
png_set_swap(png_ptr); 
/* Add filler (or alpha) byte (before/after each RGB triplet) */ 
png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER); 
/* Turn on interlace handling. REQUIRED if you are not using 
* png_read_image(). To see how to handle interlacing passes, 
* see the png_read_row() method below: 
*/ 
number_passes = png_set_interlace_handling(png_ptr); 
/* Optional call to gamma correct and add the background to the palette 
* and update info structure. REQUIRED if you are expecting libpng to 
* update the palette for you (ie you selected such a transform above). 
*/ 
png_read_update_info(png_ptr, info_ptr); 
/* Allocate the memory to hold the image using the fields of info_ptr. */ 
/* The easiest way to read the image: */ 
png_bytep row_pointers[height]; 
for (row = 0; row < height; row++) 
{ 
row_pointers[row] = png_malloc(png_ptr, png_get_rowbytes(png_ptr, 
info_ptr)); 
} 
/* Now it's time to read the image. One of these methods is REQUIRED */ 
#ifdef entire /* Read the entire image in one go */ 
png_read_image(png_ptr, row_pointers); 
#else no_entire /* Read the image one or more scanlines at a time */ 
/* The other way to read images - deal with interlacing: */ 
for (pass = 0; pass < number_passes; pass++) 
{ 
#ifdef single /* Read the image a single row at a time */ 
for (y = 0; y < height; y++) 
{ 
png_read_rows(png_ptr, &row_pointers[y], png_bytepp_NULL, 1); 
} 
#else no_single /* Read the image several rows at a time */ 
for (y = 0; y < height; y += number_of_rows) 
{ 
#ifdef sparkle /* Read the image using the "sparkle" effect. */ 
png_read_rows(png_ptr, &row_pointers[y], png_bytepp_NULL, 
number_of_rows); 
#else no_sparkle /* Read the image using the "rectangle" effect */ 
png_read_rows(png_ptr, png_bytepp_NULL, &row_pointers[y], 
number_of_rows); 
#endif no_sparkle /* use only one of these two methods */ 
} 
/* if you want to display the image after every pass, do 
so here */ 
#endif no_single /* use only one of these two methods */ 
} 
#endif no_entire /* use only one of these two methods */ 
/* read rest of file, and get additional chunks in info_ptr - REQUIRED */ 
png_read_end(png_ptr, info_ptr); 
#endif hilevel 
/* At this point you have read the entire image */ 
/* clean up after the read, and free any memory allocated - REQUIRED */ 
png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL); 
/* close the file */ 
fclose(fp); 
/* that's it */ 
return (OK); 
}
