
#include "Core/SystemTools.inl"

/// <summary>
/// Image in unmanaged memory.
/// </summary>
/// 
/// <remarks>
/// <para>The class represents wrapper of an image in unmanaged memory. Using this class
/// it is possible as to allocate new image in unmanaged memory, as to just wrap provided
/// pointer to unmanaged memory, where an image is stored.</para>
/// 
/// <para>Usage of unmanaged images is mostly beneficial when it is required to apply <b>multiple</b>
/// image processing routines to a single image. In such scenario usage of .NET managed images 
/// usually leads to worse performance, because each routine needs to lock managed image
/// before image processing is done and then unlock it after image processing is done. Without
/// these lock/unlock there is no way to get direct access to managed image's data, which means
/// there is no way to do fast image processing. So, usage of managed images lead to overhead, which
/// is caused by locks/unlock. Unmanaged images are represented internally using unmanaged memory
/// buffer. This means that it is not required to do any locks/unlocks in order to get access to image
/// data (no overhead).</para>
/// 
/// <para>Sample usage:</para>
/// <code>
/// // sample 1 - wrapping .NET image into unmanaged without
/// // making extra copy of image in memory
/// BitmapData imageData = image.LockBits(
///     new Rectangle( 0, 0, image.Width, image.Height ),
///     ImageLockMode.ReadWrite, image->t );
/// 
/// try
/// {
///     img_t unmanagedImage = new img_t( imageData ) );
///     // apply several routines to the unmanaged image
/// }
/// finally
/// {
///     image.UnlockBits( imageData );
/// }
/// 
/// 
/// // sample 2 - converting .NET image into unmanaged
/// img_t unmanagedImage = img_t.FromManagedImage( image );
/// // apply several routines to the unmanaged image
/// ...
/// // conver to managed image if it is required to display it at some points[i] of time
/// img_t* managedImage = unmanagedImage.ToManagedImage( );
/// </code>
/// </remarks>
/// 

/// <summary>
/// Allocate new image in unmanaged memory.
/// </summary>
/// 
/// <param name="im->w">Image im->w.</param>
/// <param name="height">Image height.</param>
/// <param name="pixelFormat">Image pixel format.</param>
/// 
/// <returns>Return image allocated in unmanaged memory.</returns>
/// 
/// <remarks><para>Allocate new image with specified attributes in unmanaged memory.</para>
/// 
/// <para><note>The method supports only
/// <see cref="System.Drawing.Imaging.t">Format8bppIndexed</see>,
/// <see cref="System.Drawing.Imaging.t">Format16bppGrayScale</see>,
/// <see cref="System.Drawing.Imaging.t">Format24bppRgb</see>,
/// <see cref="System.Drawing.Imaging.t">Format32bppRgb</see>,
/// <see cref="System.Drawing.Imaging.t">Format32bppArgb</see>,
/// <see cref="System.Drawing.Imaging.t">Format32bppPArgb</see>,
/// <see cref="System.Drawing.Imaging.t">Format48bppRgb</see>,
/// <see cref="System.Drawing.Imaging.t">Format64bppArgb</see> and
/// <see cref="System.Drawing.Imaging.t">Format64bppPArgb</see> pixel formats.
/// In the case if <see cref="System.Drawing.Imaging.t">Format8bppIndexed</see>
/// format is specified, pallete is not not created for the image (supposed that it is
/// 8 bpp grayscale image).
/// </note></para>
/// </remarks>
/// 
/// <exception cref="UnsupportedImageFormatException">Unsupported pixel format was specified.</exception>
/// <exception cref="InvalidImagePropertiesException">Invalid image size was specified.</exception>
/// 

static int Image_SetSize( img_t* im, int height, int width, PixelFormat pixelFormat )
{
  int bytesPerPixel = Image_GetPixelFormatSize(pixelFormat)/8;
  
  // check image size
  if ( ( width <= 0 ) || ( height <= 0 ) )
  {
     InvalidImagePropertiesException( "Invalid image size specified." );
  }
  
  // allocate memory for the image
  imsetsize(im, height, width, bytesPerPixel, 1);
  im->t = pixelFormat;
  return 0;
}

/// <summary>
/// Clone the unmanaged images.
/// </summary>
/// 
/// <returns>Returns clone of the unmanaged image.</returns>
/// 
/// <remarks><para>The method does complete cloning of the object.</para></remarks>
/// 
int Image_Clone(const img_t* im, img_t* b)
{
    // allocate memory for the image
    Image_SetSize(b, im->h, im->w, im->t);
    memcpy2d(b->tt.data, b->s, im->tt.data, im->s, im->h, b->w*b->c);
    return 0;
}

/// <summary>
/// Copy unmanaged image.
/// </summary>
/// 
/// <param name="destImage">Destination image to copy this image to.</param>
/// 
/// <remarks><para>The method copies current unmanaged image to the specified image.
/// Size and pixel format of the destination image must be exactly the same.</para></remarks>
/// 
/// <exception cref="InvalidImagePropertiesException">Destination image has different size or pixel format.</exception>
/// 
int Image_Copy( const img_t* im, img_t* destImage )
{
    if (
        ( im->w != destImage->w ) || ( im->h != destImage->h ) ||
        ( im->t != destImage->t ) )
    {
        InvalidImagePropertiesException( "Destination image has different size or pixel format." );
    }

    if ( im->s == destImage->s )
    {
        // copy entire image
        CopyUnmanagedMemory( destImage->tt.data, im->tt.data, im->s * im->h );
    }
    else
    {
        int i;
        int dstStride = destImage->s;
        int copyLength = ( im->s < dstStride ) ? im->s : dstStride;

        byte* src = im->tt.data;
        byte* dst = destImage->tt.data;

        // copy line by line
        for (i = 0; i < im->h; i++ )
        {
            CopyUnmanagedMemory( dst, src, copyLength );

            dst += dstStride;
            src += im->s;
        }
    }
    return 0;
}

/// <summary>
/// Core image relatad methods.
/// </summary>
/// 
/// <remarks>All methods of this class are static and represent general routines
/// used by different image processing classes.</remarks>
/// 

/// <summary>
/// Check if specified 8 bpp image is grayscale.
/// </summary>
/// 
/// <param name="image">Image to check.</param>
/// 
/// <returns>Returns <b>true</b> if the image is grayscale or <b>false</b> otherwise.</returns>
/// 
/// <remarks>The methods checks if the image is a grayscale image of 256 gradients.
/// The method first examines if the image's pixel format is
/// <see cref="System.Drawing.Imaging->t">Format8bppIndexed</see>
/// and then it examines its palette to check if the image is grayscale or not.</remarks>
/// 
static bool IsGrayscale( const img_t* image, const ColorPalette* cp )
{
    bool ret = false;

    // check pixel format
    if ( image->t == PixelFormat_8bppIndexed )
    {
        // check palette
        PaletteEntry c;
        bool ret = true;
        int i;
        // init palette
        for (i = 0; i < 256; i++ )
        {
            c = cp[i];
            if ( ( c.r != i ) || ( c.g != i ) || ( c.b != i ) )
            {
                ret = false;
                break;
            }
        }
    }
    return ret;
}

/// <summary>
/// Set pallete of the 8 bpp indexed image to grayscale.
/// </summary>
/// 
/// <param name="image">Image to initialize.</param>
/// 
/// <remarks>The method initializes palette of
/// <see cref="System.Drawing.Imaging->t">Format8bppIndexed</see>
/// image with 256 gradients of gray color.</remarks>
/// 
/// <exception cref="UnsupportedImageFormatException">Provided image is not 8 bpp indexed image.</exception>
/// 
static int SetGrayscalePalette( ColorPalette* cp )
{
    int i;
    // init palette
    for (i = 0; i < 256; i++ )
    {
        cp[i] = Color_rgb( i, i, i );
    }
    // set palette back
    return 0;
}

/// <summary>
/// Create and initialize new 8 bpp grayscale image.
/// </summary>
/// 
/// <param name="im->w">Image width.</param>
/// <param name="height">Image height.</param>
/// 
/// <returns>Returns the created grayscale image.</returns>
/// 
/// <remarks>The method creates new 8 bpp grayscale image and initializes its palette.
/// Grayscale image is represented as
/// <see cref="System.Drawing.Imaging->t">Format8bppIndexed</see>
/// image with palette initialized to 256 gradients of gray color.</remarks>
/// 
static int Image_SetGrayscaleSize( img_t* im, int height, int width )
{
    // create new image
    Image_SetSize( im, height, width, PixelFormat_8bppIndexed );
    // set palette to grayscale
    // return new image
    return 0;
}

/// <summary>
/// Convert bitmap with 16 bits per plane to a bitmap with 8 bits per plane.
/// </summary>
/// 
/// <param name="bimap">Source image to convert.</param>
/// 
/// <returns>Returns new image which is a copy of the source image but with 8 bits per plane.</returns>
/// 
/// <remarks><para>The routine does the next pixel format conversions:
/// <list type="bullet">
/// <item><see cref="PixelFormat_16bppGrayScale">Format16bppGrayScale</see> to
/// <see cref="PixelFormat_8bppIndexed">Format8bppIndexed</see> with grayscale palette;</item>
/// <item><see cref="PixelFormat_48bppRgb">Format48bppRgb</see> to
/// <see cref="PixelFormat_24bppRgb">Format24bppRgb</see>;</item>
/// <item><see cref="PixelFormat_64bppArgb">Format64bppArgb</see> to
/// <see cref="PixelFormat_32bppArgb">Format32bppArgb</see>;</item>
/// <item><see cref="PixelFormat_64bppPArgb">Format64bppPArgb</see> to
/// <see cref="PixelFormat_32bppPArgb">Format32bppPArgb</see>.</item>
/// </list>
/// </para></remarks>
/// 
/// <exception cref="UnsupportedImageFormatException">Invalid pixel format of the source image.</exception>
/// 
static int Convert16bppTo8bpp( const img_t* im, img_t* newImage )
{
    int layers = 0;
    ASSERT(im!=newImage);

    // create new image depending on source image format
    switch ( im->t )
    {
        case PixelFormat_16bppGrayScale:
            // create new grayscale image
            Image_SetSize( newImage, im->h, im->w, PixelFormat_8bppIndexed );
            layers = 1;
            break;

        case PixelFormat_48bppRgb:
            // create new color 24 bpp image
            Image_SetSize( newImage, im->h, im->w, PixelFormat_24bppRgb );
            layers = 3;
            break;

        case PixelFormat_64bppArgb:
            // create new color 32 bpp image
            Image_SetSize( newImage, im->h, im->w, PixelFormat_32bppArgb );
            layers = 4;
            break;

        case PixelFormat_64bppPArgb:
            // create new color 32 bpp image
            Image_SetSize( newImage, im->h, im->w, PixelFormat_32bppPArgb );
            layers = 4;
            break;

        default:
            UnsupportedImageFormatException( "Invalid pixel format of the source image." );
    }

    {
        // base pointers
      int x, y, lineSize;

        for (y = 0; y < im->h; y++ )
        {
            ushort* sourcePtr = (ushort*) ( im->tt.data + y * im->s );
            byte* newPtr = ( newImage->tt.data + y * newImage->s );

            for (x = 0, lineSize = im->w * layers; x < lineSize; x++, sourcePtr++, newPtr++ )
            {
                *newPtr = (byte) ( *sourcePtr >> 8 );
            }
        }
    }

    return 0;
}

/// <summary>
/// Convert bitmap with 8 bits per plane to a bitmap with 16 bits per plane.
/// </summary>
/// 
/// <param name="bimap">Source image to convert.</param>
/// 
/// <returns>Returns new image which is a copy of the source image but with 16 bits per plane.</returns>
/// 
/// <remarks><para>The routine does the next pixel format conversions:
/// <list type="bullet">
/// <item><see cref="PixelFormat_8bppIndexed">Format8bppIndexed</see> (grayscale palette assumed) to
/// <see cref="PixelFormat_16bppGrayScale">Format16bppGrayScale</see>;</item>
/// <item><see cref="PixelFormat_24bppRgb">Format24bppRgb</see> to
/// <see cref="PixelFormat_48bppRgb">Format48bppRgb</see>;</item>
/// <item><see cref="PixelFormat_32bppArgb">Format32bppArgb</see> to
/// <see cref="PixelFormat_64bppArgb">Format64bppArgb</see>;</item>
/// <item><see cref="PixelFormat_32bppPArgb">Format32bppPArgb</see> to
/// <see cref="PixelFormat_64bppPArgb">Format64bppPArgb</see>.</item>
/// </list>
/// </para></remarks>
/// 
/// <exception cref="UnsupportedImageFormatException">Invalid pixel format of the source image.</exception>
/// 
static int Convert8bppTo16bpp( const img_t* im, img_t* newImage )
{
    int layers = 0;

    // create new image depending on source image format
    switch ( im->t )
    {
        case PixelFormat_8bppIndexed:
            // create new grayscale image
            Image_SetSize( newImage, im->h, im->w, PixelFormat_16bppGrayScale );
            layers = 1;
            break;

        case PixelFormat_24bppRgb:
            // create new color 48 bpp image
            Image_SetSize( newImage, im->h, im->w, PixelFormat_48bppRgb );
            layers = 3;
            break;

        case PixelFormat_32bppArgb:
            // create new color 64 bpp image
            Image_SetSize( newImage, im->h, im->w, PixelFormat_64bppArgb );
            layers = 4;
            break;

        case PixelFormat_32bppPArgb:
            // create new color 64 bpp image
            Image_SetSize( newImage, im->h, im->w, PixelFormat_64bppPArgb );
            layers = 4;
            break;

        default:
            UnsupportedImageFormatException( "Invalid pixel format of the source image." );
    }

    {
        // base pointers
        int x, y, lineSize;

        for (y = 0; y < im->h; y++ )
        {
            byte* sourcePtr = ( im->tt.data + y * im->s );
            ushort* newPtr  = (ushort*) ( newImage->tt.data + y * newImage->s );

            for (x = 0, lineSize = im->w * layers; x < lineSize; x++, sourcePtr++, newPtr++ )
            {
                *newPtr = (ushort) ( *sourcePtr << 8 );
            }
        }
    }

    return 0;
}

/// <summary>
/// Collect pixel values from the specified list of coordinates.
/// </summary>
/// 
/// <param name="points">List of coordinates to collect pixels' value from.</param>
/// 
/// <returns>Returns array of pixels' values from the specified coordinates.</returns>
/// 
/// <remarks><para>The method goes through the specified list of points and for each points[i] retrievs
/// corresponding pixel's value from the unmanaged image.</para>
/// 
/// <para><note>For grayscale image the output array has the same length as number of points in the
/// specified list of points. For color image the output array has triple length, containing pixels'
/// values in RGB order.</note></para>
/// 
/// <para><note>The method does not make any checks for valid coordinates and leaves this up to user.
/// If specified coordinates are out of image's bounds, the result is not predictable (crash in most cases).
/// </note></para>
/// 
/// <para><note>This method is supposed for images with 8 bpp channels only (8 bpp grayscale image and
/// 24/32 bpp color images).</note></para>
/// </remarks>
/// 
/// <exception cref="UnsupportedImageFormatException">Unsupported pixel format of the source image. Use Collect16bppPixelValues() method for
/// images with 16 bpp channels.</exception>
/// 

int Collect8bppPixelValues( const img_t* im, int n, const IPOINT* points, img_t* pixelValues )
{
    int pixelFormat = im->t;
    int pixelSize = Image_GetPixelFormatSize( pixelFormat ) / 8;

    if ( ( pixelFormat == PixelFormat_16bppGrayScale ) || ( pixelSize > 4 ) )
    {
        UnsupportedImageFormatException( "Unsupported pixel format of the source image. Use Collect16bppPixelValues() method for it." );
    }

    //byte[] pixelValues = new byte[points.Count * ( ( pixelFormat == PixelFormat_8bppIndexed ) ? 1 : 3 )];

    {
        int i = 0;
        const byte* ptr;

        if ( pixelFormat == PixelFormat_8bppIndexed )
        {
            imsetsize(pixelValues, 1, n, 1, 1 );
            for (i=0; i<n; ++i)
            {
                ptr = im->tt.data + im->s * points[i].y + points[i].x;
                pixelValues->tt.data[i++] = *ptr;
            }
        }
        else
        {
            imsetsize(pixelValues, 1, n, 3, 1 );
            for (i=0; i<3*n; ++i)
            {
                ptr = im->tt.data + im->s * points[i].y + points[i].x * pixelSize;
                pixelValues->tt.data[i++] = ptr[RGB_R];
                pixelValues->tt.data[i++] = ptr[RGB_G];
                pixelValues->tt.data[i++] = ptr[RGB_B];
            }
        }
    }

    return 0;
}

/// <summary>
/// Collect coordinates of none black pixels within specified rectangle of the image.
/// </summary>
/// 
/// <param name="rect">Image's rectangle to process.</param>
/// 
/// <returns>Returns list of points, which have other than black color.</returns>
///
int CollectActivePixels( const img_t* im, IRECT rc, arr_t* arr )
{
    int pixelFormat = im->t;
    int pixelSize = Image_GetPixelFormatSize( pixelFormat ) / 8;

    // correct rectangle
    IRECT rect = iRectInter( rc, iRECT( 0, 0, im->w, im->h ) );

    int startX = rect.l;
    int startY = rect.t;
    int stopX  = rect.r;
    int stopY  = rect.b;

    arr_setsize(arr, RCH(&rc)*RCW(&rc), sizeof(IPOINT));

    {
        IPOINT* pixels=(IPOINT*)arr->tt.data;
        const byte* basePtr = im->tt.data;
        int i=0, x, y;

        if ( ( pixelFormat == PixelFormat_16bppGrayScale ) || ( pixelSize > 4 ) )
        {
            int pixelWords = pixelSize >> 1;

            for (y = startY; y < stopY; y++ )
            {
                ushort* ptr = (ushort*) ( basePtr + y * im->s + startX * pixelSize );

                if ( pixelWords == 1 )
                {
                    // grayscale images
                    for (x = startX; x < stopX; x++, ptr++ )
                    {
                        if ( *ptr != 0 )
                        {
                            pixels[i++] = iPOINT( x, y );
                        }
                    }
                }
                else
                {
                    // color images
                    for (x = startX; x < stopX; x++, ptr += pixelWords )
                    {
                        if ( ( ptr[RGB_R] != 0 ) || ( ptr[RGB_G] != 0 ) || ( ptr[RGB_B] != 0 ) )
                        {
                            pixels[i++] = iPOINT( x, y );
                        }
                    }
                }
            }
        }
        else
        {
            for (y = startY; y < stopY; y++ )
            {
                const byte* ptr = basePtr + y * im->s + startX * pixelSize;

                if ( pixelSize == 1 )
                {
                    // grayscale images
                    for (x = startX; x < stopX; x++, ptr++ )
                    {
                        if ( *ptr != 0 )
                        {
                            pixels[i++] = iPOINT( x, y );
                        }
                    }
                }
                else
                {
                    // color images
                    for (x = startX; x < stopX; x++, ptr += pixelSize )
                    {
                        if ( ( ptr[RGB_R] != 0 ) || ( ptr[RGB_G] != 0 ) || ( ptr[RGB_B] != 0 ) )
                        {
                            pixels[i++] = iPOINT( x, y );
                        }
                    }
                }
            }
        }
    }

    return 0;
}

/// <summary>
/// Set pixels with the specified coordinates to the specified color.
/// </summary>
/// 
/// <param name="coordinates">List of points to set color for.</param>
/// <param name="color">Color to set for the specified points.</param>
/// 
/// <remarks><para><note>For images having 16 bpp per color plane, the method extends the specified color
/// value to 16 bit by multiplying it by 256.</note></para></remarks>
///
int Image_SetPixels( img_t* im, int n, const IPOINT* points, Color color )
{
    int pixelFormat = im->t;
    int pixelSize = Image_GetPixelFormatSize( pixelFormat ) / 8;
    byte* basePtr = im->tt.data;

    byte red   = color.r;
    byte green = color.g;
    byte blue  = color.b;
    byte alpha = color.a;
    int i;

    switch ( pixelFormat )
    {
        case PixelFormat_8bppIndexed:
            {
                byte grayValue = (byte) ( 0.2125 * red + 0.7154 * green + 0.0721 * blue );

                for (i=0; i<n; ++i )
                {
                    if ( ( points[i].x >= 0 ) && ( points[i].y >= 0 ) && ( points[i].x < im->w ) && ( points[i].y < im->h ) )
                    {
                        byte* ptr = basePtr + points[i].y * im->s + points[i].x;
                        *ptr = grayValue;
                    }
                }
            }
            break;

        case PixelFormat_24bppRgb:
        case PixelFormat_32bppRgb:
            {
                for (i=0; i<n; ++i )
                {
                    if ( ( points[i].x >= 0 ) && ( points[i].y >= 0 ) && ( points[i].x < im->w ) && ( points[i].y < im->h ) )
                    {
                        byte* ptr = basePtr + points[i].y * im->s + points[i].x * pixelSize;
                        ptr[RGB_R] = red;
                        ptr[RGB_G] = green;
                        ptr[RGB_B] = blue;
                    }
                }
            }
            break;

        case PixelFormat_32bppArgb:
          {
                for (i=0; i<n; ++i )
                {
                    if ( ( points[i].x >= 0 ) && ( points[i].y >= 0 ) && ( points[i].x < im->w ) && ( points[i].y < im->h ) )
                    {
                        byte* ptr = basePtr + points[i].y * im->s + points[i].x * pixelSize;
                        ptr[RGB_R] = red;
                        ptr[RGB_G] = green;
                        ptr[RGB_B] = blue;
                        ptr[RGB_A] = alpha;
                    }
                }
            }
            break;

        case PixelFormat_16bppGrayScale:
            {
                ushort grayValue = (ushort) ( (ushort) ( 0.2125 * red + 0.7154 * green + 0.0721 * blue ) << 8 );
                
                for (i=0; i<n; ++i )
                {
                    if ( ( points[i].x >= 0 ) && ( points[i].y >= 0 ) && ( points[i].x < im->w ) && ( points[i].y < im->h ) )
                    {
                        ushort* ptr = (ushort*) ( basePtr + points[i].y * im->s ) + points[i].x;
                        *ptr = grayValue;
                    }
                }
            }
            break;

        case PixelFormat_48bppRgb:
            {
                ushort red16   = (ushort) ( red   << 8 );
                ushort green16 = (ushort) ( green << 8 );
                ushort blue16  = (ushort) ( blue  << 8 );
                
                for (i=0; i<n; ++i )
                {
                    if ( ( points[i].x >= 0 ) && ( points[i].y >= 0 ) && ( points[i].x < im->w ) && ( points[i].y < im->h ) )
                    {
                        ushort* ptr = (ushort*) ( basePtr + points[i].y * im->s + points[i].x * pixelSize );
                        ptr[RGB_R] = red16;
                        ptr[RGB_G] = green16;
                        ptr[RGB_B] = blue16;
                    }
                }
            }
            break;

        case PixelFormat_64bppArgb:
            {
                ushort red16   = (ushort) ( red   << 8 );
                ushort green16 = (ushort) ( green << 8 );
                ushort blue16  = (ushort) ( blue  << 8 );
                ushort alpha16 = (ushort) ( alpha << 8 );
                
                for (i=0; i<n; ++i )
                {
                    if ( ( points[i].x >= 0 ) && ( points[i].y >= 0 ) && ( points[i].x < im->w ) && ( points[i].y < im->h ) )
                    {
                        ushort* ptr = (ushort*) ( basePtr + points[i].y * im->s + points[i].x * pixelSize );
                        ptr[RGB_R] = red16;
                        ptr[RGB_G] = green16;
                        ptr[RGB_B] = blue16;
                        ptr[RGB_A] = alpha16;
                    }
                }
            }
            break;

        default:
            UnsupportedImageFormatException( "The pixel format is not supported: " && pixelFormat );
    }
    return 0;
}

int Image_SetPixel( img_t* im, int x, int y, byte r, byte g, byte b, byte a )
{
    if ( ( x >= 0 ) && ( y >= 0 ) && ( x < im->w ) && ( y < im->h ) )
    {
        int pixelFormat = im->t;
        int pixelSize = Image_GetPixelFormatSize( pixelFormat ) / 8;
        byte* ptr = im->tt.data + y * im->s + x * pixelSize;
        ushort* ptr2 = (ushort*) ptr;

        switch ( pixelFormat )
        {
            case PixelFormat_8bppIndexed:
                *ptr = (byte) ( 0.2125 * r + 0.7154 * g + 0.0721 * b );
                break;

            case PixelFormat_24bppRgb:
            case PixelFormat_32bppRgb:
                ptr[RGB_R] = r;
                ptr[RGB_G] = g;
                ptr[RGB_B] = b;
                break;

            case PixelFormat_32bppArgb:
                ptr[RGB_R] = r;
                ptr[RGB_G] = g;
                ptr[RGB_B] = b;
                ptr[RGB_A] = a;
                break;

            case PixelFormat_16bppGrayScale:
                *ptr2 = (ushort) ( (ushort) ( 0.2125 * r + 0.7154 * g + 0.0721 * b ) << 8 );
                break;

            case PixelFormat_48bppRgb:
                ptr2[RGB_R] = (ushort) ( r << 8 );
                ptr2[RGB_G] = (ushort) ( g << 8 );
                ptr2[RGB_B] = (ushort) ( b << 8 );
                break;

            case PixelFormat_64bppArgb:
                ptr2[RGB_R] = (ushort) ( r << 8 );
                ptr2[RGB_G] = (ushort) ( g << 8 );
                ptr2[RGB_B] = (ushort) ( b << 8 );
                ptr2[RGB_A] = (ushort) ( a << 8 );
                break;

            default:
                UnsupportedImageFormatException( "The pixel format is not supported: " + pixelFormat );
        }
    }
    return 0;
}

/// <summary>
/// Get color of the pixel with the specified coordinates.
/// </summary>
/// 
/// <param name="x">X coordinate of the pixel to get.</param>
/// <param name="y">Y coordinate of the pixel to get.</param>
/// 
/// <returns>Return pixel's color at the specified coordinates.</returns>
/// 
/// <remarks>
/// <para><note>In the case if the image has 8 bpp grayscale format, the method will return a color with
/// all R/G/B components set to same value, which is grayscale intensity.</note></para>
/// 
/// <para><note>The method supports only 8 bpp grayscale images and 24/32 bpp color images so far.</note></para>
/// </remarks>
/// 
/// <exception cref="ArgumentOutOfRangeException">The specified pixel coordinate is out of image's bounds.</exception>
/// <exception cref="UnsupportedImageFormatException">Pixel format of this image is not supported by the method.</exception>
/// 
Color Image_GetPixel( const img_t* im, int x, int y )
{
    Color color = {0};
    if ( ( x < 0 ) || ( y < 0 ) )
    {
        ArgumentOutOfRangeException( "x The specified pixel coordinate is out of image's bounds." );
    }

    if ( ( x >= im->w ) || ( y >= im->h ) )
    {
        ArgumentOutOfRangeException( "y The specified pixel coordinate is out of image's bounds." );
    }

    {
        int pixelFormat = im->t;
        int pixelSize = Image_GetPixelFormatSize( pixelFormat ) / 8;
        const byte* ptr = im->tt.data + y * im->s + x * pixelSize;

        switch ( pixelFormat )
        {
            case PixelFormat_8bppIndexed:
                color = Color_rgb( *ptr, *ptr, *ptr );
                break;

            case PixelFormat_24bppRgb:
            case PixelFormat_32bppRgb:
                color = Color_rgb( ptr[RGB_R], ptr[RGB_G], ptr[RGB_B] );
                break;

            case PixelFormat_32bppArgb:
                color = Color_argb( ptr[RGB_A], ptr[RGB_R], ptr[RGB_G], ptr[RGB_B] );
                break;

            default:
                UnsupportedImageFormatException( "The pixel format is not supported: " + pixelFormat );
        }
    }

    return color;
}

/// <summary>
/// Collect pixel values from the specified list of coordinates.
/// </summary>
/// 
/// <param name="points">List of coordinates to collect pixels' value from.</param>
/// 
/// <returns>Returns array of pixels' values from the specified coordinates.</returns>
/// 
/// <remarks><para>The method goes through the specified list of points and for each points[i] retrievs
/// corresponding pixel's value from the unmanaged image.</para>
/// 
/// <para><note>For grayscale image the output array has the same length as number of points in the
/// specified list of points. For color image the output array has triple length, containing pixels'
/// values in RGB order.</note></para>
/// 
/// <para><note>The method does not make any checks for valid coordinates and leaves this up to user.
/// If specified coordinates are out of image's bounds, the result is not predictable (crash in most cases).
/// </note></para>
/// 
/// <para><note>This method is supposed for images with 16 bpp channels only (16 bpp grayscale image and
/// 48/64 bpp color images).</note></para>
/// </remarks>
/// 
/// <exception cref="UnsupportedImageFormatException">Unsupported pixel format of the source image. Use Collect8bppPixelValues() method for
/// images with 8 bpp channels.</exception>
///
int Collect16bppPixelValues( const img_t* im, int n, const IPOINT* points, img_t* im2 )
{
    int pixelFormat = im->t;
    int pixelSize = Image_GetPixelFormatSize( pixelFormat ) / 8;
    ushort* pixelValues = 0;

    if ( ( pixelFormat == PixelFormat_8bppIndexed ) || ( pixelSize == 3 ) || ( pixelSize == 4 ) )
    {
        UnsupportedImageFormatException( "Unsupported pixel format of the source image. Use Collect8bppPixelValues() method for it." );
    }

    //ushort[] pixelValues = new ushort[points.Count * ( ( pixelFormat == PixelFormat_16bppGrayScale ) ? 1 : 3 )];

    {
        const byte* basePtr = im->tt.data;
        ushort* ptr;
        int i = 0;

        if ( pixelFormat == PixelFormat_16bppGrayScale )
        {
            imsetsize(im2, 1, n, sizeof(ushort), 1);
            pixelValues = im2->tt.u2;

            for (i=0; i<n; ++i)
            {
                ptr = (ushort*) ( basePtr + im->s * points[i].y + points[i].x * pixelSize );
                pixelValues[i++] = *ptr;
            }
        }
        else
        {
            imsetsize(im2, 1, n, 3*sizeof(ushort), 1);
            pixelValues = im2->tt.u2;
            
            for (i=0; i<n; ++i)
            {
                ptr = (ushort*) ( basePtr + im->s * points[i].y + points[i].x * pixelSize );
                pixelValues[i++] = ptr[RGB_R];
                pixelValues[i++] = ptr[RGB_G];
                pixelValues[i++] = ptr[RGB_B];
            }
        }
    }

    return 0;
}


