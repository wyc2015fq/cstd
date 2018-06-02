
/// <summary>
/// Base class for image grayscaling.
/// </summary>
/// 
/// <remarks><para>This class is the base class for image grayscaling. Other
/// classes should inherit from this class and specify <b>RGB</b>
/// coefficients used for color image conversion to grayscale.</para>
/// 
/// <para>The filter accepts 24, 32, 48 and 64 bpp color images and produces
/// 8 (if source is 24 or 32 bpp image) or 16 (if source is 48 or 64 bpp image)
/// bpp grayscale image.</para>
/// 
/// <para>Sample usage:</para>
/// <code>
/// // create grayscale filter (BT709)
/// Grayscale filter = new Grayscale( 0.2125, 0.7154, 0.0721 );
/// // apply the filter
/// Bitmap grayImage = filter.Apply( image );
/// </code>
/// 
/// <para><b>Initial image:</b></para>
/// <img src="img/imaging/sample1.jpg" im->w="480" im->h="361" />
/// <para><b>Result image:</b></para>
/// <img src="img/imaging/grayscale.jpg" im->w="480" im->h="361" />
/// </remarks>
///
/// <seealso cref="GrayscaleBT709"/>
/// <seealso cref="GrayscaleRMY"/>
/// <seealso cref="GrayscaleY"/>
///

enum { GRY_BT709, GRY_RMY, GRY_Y };
int Grayscale(const img_t* im, img_t* im2, int CommonAlgorithms)
{
    /// <summary>
    /// Set of predefined common grayscaling algorithms, which have aldready initialized
    /// grayscaling coefficients.
    /// </summary>
    // CommonAlgorithms

    /// <summary>
    /// Grayscale image using BT709 algorithm.
    /// </summary>
    /// 
    /// <remarks><para>The instance uses <b>BT709</b> algorithm to convert color image
    /// to grayscale. The conversion coefficients are:
    /// <list type="bullet">
    /// <item>Red: 0.2125;</item>
    /// <item>Green: 0.7154;</item>
    /// <item>Blue: 0.0721.</item>
    /// </list></para>
    /// 
    /// <para>Sample usage:</para>
    /// <code>
    /// // apply the filter
    /// Bitmap grayImage = Grayscale.CommonAlgorithms.BT709.Apply( image );
    /// </code>
    /// </remarks>
    /// 

    /// <summary>
    /// Grayscale image using R-Y algorithm.
    /// </summary>
    /// 
    /// <remarks><para>The instance uses <b>R-Y</b> algorithm to convert color image
    /// to grayscale. The conversion coefficients are:
    /// <list type="bullet">
    /// <item>Red: 0.5;</item>
    /// <item>Green: 0.419;</item>
    /// <item>Blue: 0.081.</item>
    /// </list></para>
    /// 
    /// <para>Sample usage:</para>
    /// <code>
    /// // apply the filter
    /// Bitmap grayImage = Grayscale.CommonAlgorithms.RMY.Apply( image );
    /// </code>
    /// </remarks>
    /// 

    /// <summary>
    /// Grayscale image using Y algorithm.
    /// </summary>
    /// 
    /// <remarks><para>The instance uses <b>Y</b> algorithm to convert color image
    /// to grayscale. The conversion coefficients are:
    /// <list type="bullet">
    /// <item>Red: 0.299;</item>
    /// <item>Green: 0.587;</item>
    /// <item>Blue: 0.114.</item>
    /// </list></para>
    /// 
    /// <para>Sample usage:</para>
    /// <code>
    /// // apply the filter
    /// Bitmap grayImage = Grayscale.CommonAlgorithms.Y.Apply( image );
    /// </code>
    /// </remarks>
    /// 
      static double gryarg[][3] = { { 0.2125, 0.7154, 0.0721 },
          { 0.5000, 0.4190, 0.0810 },
          { 0.2990, 0.5870, 0.1140 }};

    // RGB coefficients for grayscale transformation

    /// <summary>
    /// Portion of red channel's value to use during conversion from RGB to grayscale.
    /// </summary>
    double RedCoefficient = gryarg[CommonAlgorithms][0];
    /// <summary>
    /// Portion of green channel's value to use during conversion from RGB to grayscale.
    /// </summary>
    double GreenCoefficient = gryarg[CommonAlgorithms][1];
    /// <summary>
    /// Portion of blue channel's value to use during conversion from RGB to grayscale.
    /// </summary>
    double BlueCoefficient = gryarg[CommonAlgorithms][2];

    /// <summary>
    /// Process the filter on the specified image.
    /// </summary>
    /// 
    /// <param name="im">Source image data.</param>
    /// <param name="im2">Destination image data.</param>
    /// 

    int x, y;
    int srcPixelFormat = im->t;
    
    if (
        ( srcPixelFormat == PixelFormat_24bppRgb ) ||
        ( srcPixelFormat == PixelFormat_32bppRgb ) ||
        ( srcPixelFormat == PixelFormat_32bppArgb ) )
    {
        int pixelSize = ( srcPixelFormat == PixelFormat_24bppRgb ) ? 3 : 4;
        int srcOffset, dstOffset;

        int rc = (int) ( 0x10000 * RedCoefficient );
        int gc = (int) ( 0x10000 * GreenCoefficient );
        int bc = (int) ( 0x10000 * BlueCoefficient );

        // do the job
        const byte* src = im->tt.data;
        byte* dst;
        Image_SetSize(im2, im->h, im->w, PixelFormat_8bppIndexed);
        srcOffset = im->s - im->w * pixelSize;
        dstOffset = im2->s - im->w;
        dst = im2->tt.data;

        // for each line
        for (y = 0; y < im->h; y++ )
        {
            // for each pixel
            for (x = 0; x < im->w; x++, src += pixelSize, dst++ )
            {
                *dst = (byte) ( ( rc * src[RGB_R] + gc * src[RGB_G] + bc * src[RGB_B] ) >> 16 );
            }
            src += srcOffset;
            dst += dstOffset;
        }
    }
    else
    {
        int pixelSize = ( srcPixelFormat == PixelFormat_48bppRgb ) ? 3 : 4;
        int srcStride = im->s;
        int dstStride = im2->s;

        const byte* srcBase = im->tt.data;
        byte* dstBase;
        Image_SetSize(im2, im->h, im->w, PixelFormat_16bppGrayScale);
        dstBase = im2->tt.data;

        // for each line
        for (y = 0; y < im->h; y++ )
        {
            ushort* src = (ushort*) ( srcBase + y * srcStride );
            ushort* dst = (ushort*) ( dstBase + y * dstStride );

            // for each pixel
            for (x = 0; x < im->w; x++, src += pixelSize, dst++ )
            {
                *dst = (ushort) ( RedCoefficient * src[RGB_R] + GreenCoefficient * src[RGB_G] + BlueCoefficient * src[RGB_B] );
            }
        }
    }
    return 0;
}

