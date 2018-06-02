
/// <summary>
/// Convolution filter.
/// </summary>
/// 
/// <remarks><para>The filter implements convolution operator, which calculates each pixel
/// of the result image as weighted sum of the correspond pixel and its neighbors in the source
/// image. The weights are set by <see cref="Kernel">convolution kernel</see>. The weighted
/// sum is divided by <see cref="Divisor"/> before putting it into result image and also
/// may be thresholded using <see cref="Threshold"/> value.</para>
/// 
/// <para>Convolution is a simple mathematical operation which is fundamental to many common
/// image processing filters. Depending on the type of provided kernel, the filter may produce
/// different results, like blur image, sharpen it, find edges, etc.</para>
/// 
/// <para>The filter accepts 8 and 16 bpp grayscale images and 24, 32, 48 and 64 bpp
/// color images for processing. Note: depending on the value of <see cref="ProcessAlpha"/>
/// property, the alpha channel is either copied as is or processed with the kernel.</para>
/// 
/// <para>Sample usage:</para>
/// <code>
/// // define emboss kernel
/// int[,] kernel = {
///             { -2, -1,  0 },
///             { -1,  1,  1 },
///             {  0,  1,  2 } };
/// // create filter
/// Convolution filter = new Convolution( kernel );
/// // apply the filter
/// filter.ApplyInPlace( image );
/// </code>
/// 
/// <para><b>Initial image:</b></para>
/// <img src="img/imaging/sample5.jpg" width="480" height="361" />
/// <para><b>Result image:</b></para>
/// <img src="img/imaging/emboss.jpg" width="480" height="361" />
/// </remarks>
/// 

int Convolution(const img_t* source, img_t* destination, IRECT rect, const int* kernel, int kl, IDIM ksz)
{
    int i, j, k, n;
    // convolution kernel
    // int[,] kernel;
    // division factor
    int divisor = 1;
    // threshold to add to weighted sum
    int threshold = 0;
    // kernel size
    int size = MAX(ksz.len[0], ksz.len[1]);
    // use dynamic divisor for edges
    bool dynamicDivisorForEdges = true;
    // specifies if alpha channel must be processed or just copied
    bool processAlpha = false;
    int srcPixelFormat = source->t;

    /// <summary>
    /// Convolution kernel.
    /// </summary>
    /// 
    /// <remarks>
    /// <para><note>Convolution kernel must be square and its width/height
    /// should be odd and should be in the [3, 99] range.</note></para>
    /// 
    /// <para><note>Setting convolution kernel through this property does not
    /// affect <see cref="Divisor"/> - it is not recalculated automatically.</note></para>
    /// </remarks>
    /// 
    /// <exception cref="ArgumentException">Invalid kernel size is specified.</exception>
    /// 

    // check kernel size
    if ( ( size < 3 ) || ( size > 21 ) || ( size % 2 == 0 ) ) {
        ASSERT( 0 && "Invalid kernel size." );
    }

    /// <summary>
    /// Division factor.
    /// </summary>
    /// 
    /// <remarks><para>The value is used to divide convolution - weighted sum
    /// of pixels is divided by this value.</para>
    /// 
    /// <para><note>The value may be calculated automatically in the case if constructor
    /// with one parameter is used (<see cref="Convolution( int[,] )"/>).</note></para>
    /// </remarks>
    /// 
    /// <exception cref="ArgumentException">Divisor can not be equal to zero.</exception>
    /// 

    /// <summary>
    /// Threshold to add to weighted sum.
    /// </summary>
    /// 
    /// <remarks><para>The property specifies threshold value, which is added to each weighted
    /// sum of pixels. The value is added right after division was done by <see cref="Divisor"/>
    /// value.</para>
    /// 
    /// <para>Default value is set to <b>0</b>.</para>
    /// </remarks>
    /// 
    
    /// <summary>
    /// Use dynamic divisor for edges or not.
    /// </summary>
    /// 
    /// <remarks><para>The property specifies how to handle edges. If it is set to
    /// <see langword="false"/>, then the same divisor (which is specified by <see cref="Divisor"/>
    /// property or calculated automatically) will be applied both for non-edge regions
    /// and for edge regions. If the value is set to <see langword="true"/>, then dynamically
    /// calculated divisor will be used for edge regions, which is sum of those kernel
    /// elements, which are taken into account for particular processed pixel
    /// (elements, which are not outside image).</para>
    /// 
    /// <para>Default value is set to <see langword="true"/>.</para>
    /// </remarks>
    /// 
  

    /// <summary>
    /// Specifies if alpha channel must be processed or just copied.
    /// </summary>
    /// 
    /// <remarks><para>The property specifies the way how alpha channel is handled for 32 bpp
    /// and 64 bpp images. If the property is set to <see langword="false"/>, then alpha
    /// channel's values are just copied as is. If the property is set to <see langword="true"/>
    /// then alpha channel is convolved using the specified kernel same way as RGB channels.</para>
    /// 
    /// <para>Default value is set to <see langword="false"/>.</para>
    /// </remarks>
    /// 


    /// <summary>
    /// Initializes a new instance of the <see cref="Convolution"/> class.
    /// </summary>
    /// 
    /// <param name="kernel">Convolution kernel.</param>
    /// 
    /// <remarks><para>Using this constructor (specifying only convolution kernel),
    /// <see cref="Divisor">division factor</see> will be calculated automatically
    /// summing all kernel values. In the case if kernel's sum equals to zero,
    /// division factor will be assigned to 1.</para></remarks>
    /// 
    /// <exception cref="ArgumentException">Invalid kernel size is specified. Kernel must be
    /// square, its width/height should be odd and should be in the [3, 25] range.</exception>
    /// 
    
    divisor = 0;

    // calculate divisor
    for (i = 0, n = ksz.len[0]; i < n; i++ )
    {
        for (j = 0, k = ksz.len[1]; j < k; j++ )
        {
            divisor += kernel[i*kl+j];
        }
    }
    if ( divisor == 0 )
        divisor = 1;
    
    if ( divisor == 0 )
      ASSERT( "Divisor can not be equal to zero." );
    

    /// <summary>
    /// Initializes a new instance of the <see cref="Convolution"/> class.
    /// </summary>
    /// 
    /// <param name="kernel">Convolution kernel.</param>
    /// <param name="divisor">Divisor, used used to divide weighted sum.</param>
    /// 
    /// <exception cref="ArgumentException">Invalid kernel size is specified. Kernel must be
    /// square, its width/height should be odd and should be in the [3, 25] range.</exception>
    /// <exception cref="ArgumentException">Divisor can not be equal to zero.</exception>
    /// 

    /// <summary>
    /// Process the filter on the specified image.
    /// </summary>
    /// 
    /// <param name="source">Source image data.</param>
    /// <param name="destination">Destination image data.</param>
    /// <param name="rect">Image rectangle for processing by the filter.</param>
    /// 
    {
        int pixelSize = Image_GetPixelFormatSize( srcPixelFormat ) / 8;

        // processing start and stop X,Y positions
        int startX  = rect.l;
        int startY  = rect.t;
        int stopX   = rect.r;
        int stopY   = rect.b;
        int x, y;

        // check pixel size to find if we deal with 8 or 16 bpp channels
        if ( ( pixelSize <= 4 ) && ( pixelSize != 2 ) )
        {
            int srcStride = source->s;
            int dstStride = destination->s;

            int srcOffset = srcStride - RCW(&rect) * pixelSize;
            int dstOffset = dstStride - RCW(&rect) * pixelSize;

            const byte* src = source->tt.data;
            byte* dst = destination->tt.data;

            // allign pointers to the first pixel to process
            src += ( startY * srcStride + startX * pixelSize );
            dst += ( startY * dstStride + startX * pixelSize );

            // do the processing job
            if ( source->t == PixelFormat_8bppIndexed )
            {
                // grayscale image
                // Process8bppImage( src, dst, srcStride, dstStride, srcOffset, dstOffset, startX, startY, stopX, stopY );
                
// Process 8 bpp grayscale images
//int Process8bppImage( const byte* src, byte* dst, int srcStride, int dstStride, int srcOffset, int dstOffset, int startX, int startY, int stopX, int stopY, const int* kernel )
{
    // loop and array indexes
    int i, j, t, k, ir, jr;
    // kernel's radius
    int radius = size >> 1;
    // color sums
    long g, div;

    // kernel size
    int kernelSize = size * size;
    // number of kernel elements taken into account
    int processedKernelSize;

    // for each line
    for (y = startY; y < stopY; y++ )
    {
        // for each pixel
        for (x = startX; x < stopX; x++, src++, dst++ )
        {
            g = div = processedKernelSize = 0;

            // for each kernel row
            for (i = 0; i < size; i++ )
            {
                ir = i - radius;
                t = y + ir;

                // skip row
                if ( t < startY )
                    continue;
                // break
                if ( t >= stopY )
                    break;

                // for each kernel column
                for (j = 0; j < size; j++ )
                {
                    jr = j - radius;
                    t = x + jr;

                    // skip column
                    if ( t < startX )
                        continue;

                    if ( t < stopX )
                    {
                        k = kernel[i*kl+j];

                        div += k;
                        g += k * src[ir * srcStride + jr];
                        processedKernelSize++;
                    }
                }
            }

            // check if all kernel elements were processed
            if ( processedKernelSize == kernelSize )
            {
                // all kernel elements are processed - we are not on the edge
                div = divisor;
            }
            else
            {
                // we are on edge. do we need to use dynamic divisor or not?
                if ( !dynamicDivisorForEdges )
                {
                    // do
                    div = divisor;
                }
            }

            // check divider
            if ( div != 0 )
            {
                g /= div;
            }
            g += threshold;
            *dst = (byte) ( ( g > 255 ) ? 255 : ( ( g < 0 ) ? 0 : g ) );
        }
        src += srcOffset;
        dst += dstOffset;
    }
}

            }
            else
            {
                // RGB image
                if ( ( pixelSize == 3 ) || ( !processAlpha ) )
                {
                    //Process24bppImage( src, dst, srcStride, dstStride, srcOffset, dstOffset, startX, startY, stopX, stopY, pixelSize );
                    
// Process 24 bpp images or 32 bpp images with copying alpha channel
//int Process24bppImage( const byte* src, byte* dst, int srcStride, int dstStride, int srcOffset, int dstOffset, int startX, int startY, int stopX, int stopY, int pixelSize )
{
    // loop and array indexes
    int i, j, t, k, ir, jr;
    // kernel's radius
    int radius = size >> 1;
    // color sums
    long r, g, b, div;

    // kernel size
    int kernelSize = size * size;
    // number of kernel elements taken into account
    int processedKernelSize;

    const byte* p;

    // for each line
    for (y = startY; y < stopY; y++ )
    {
        // for each pixel
        for (x = startX; x < stopX; x++, src += pixelSize, dst += pixelSize )
        {
            r = g = b = div = processedKernelSize = 0;

            // for each kernel row
            for (i = 0; i < size; i++ )
            {
                ir = i - radius;
                t = y + ir;

                // skip row
                if ( t < startY )
                    continue;
                // break
                if ( t >= stopY )
                    break;

                // for each kernel column
                for (j = 0; j < size; j++ )
                {
                    jr = j - radius;
                    t = x + jr;

                    // skip column
                    if ( t < startX )
                        continue;

                    if ( t < stopX )
                    {
                        k = kernel[i*kl+j];
                        p = &src[ir * srcStride + jr * pixelSize];

                        div += k;

                        r += k * p[RGB_R];
                        g += k * p[RGB_G];
                        b += k * p[RGB_B];

                        processedKernelSize++;
                    }
                }
            }

            // check if all kernel elements were processed
            if ( processedKernelSize == kernelSize )
            {
                // all kernel elements are processed - we are not on the edge
                div = divisor;
            }
            else
            {
                // we are on edge. do we need to use dynamic divisor or not?
                if ( !dynamicDivisorForEdges )
                {
                    // do
                    div = divisor;
                }
            }

            // check divider
            if ( div != 0 )
            {
                r /= div;
                g /= div;
                b /= div;
            }
            r += threshold;
            g += threshold;
            b += threshold;

            dst[RGB_R] = (byte) ( ( r > 255 ) ? 255 : ( ( r < 0 ) ? 0 : r ) );
            dst[RGB_G] = (byte) ( ( g > 255 ) ? 255 : ( ( g < 0 ) ? 0 : g ) );
            dst[RGB_B] = (byte) ( ( b > 255 ) ? 255 : ( ( b < 0 ) ? 0 : b ) );

            // take care of alpha channel
            if ( pixelSize == 4 )
                dst[RGB_A] = src[RGB_A];
        }
        src += srcOffset;
        dst += dstOffset;
    }
}

                }
                else
                {
                    //Process32bppImage( src, dst, srcStride, dstStride, srcOffset, dstOffset, startX, startY, stopX, stopY );
                    
// Process 32 bpp images including alpha channel
// int Process32bppImage( const byte* src, byte* dst, int srcStride, int dstStride, int srcOffset, int dstOffset, int startX, int startY, int stopX, int stopY )
{
    // loop and array indexes
    int i, j, t, k, ir, jr;
    // kernel's radius
    int radius = size >> 1;
    // color sums
    long r, g, b, a, div;

    // kernel size
    int kernelSize = size * size;
    // number of kernel elements taken into account
    int processedKernelSize;

    const byte* p;

    // for each line
    for (y = startY; y < stopY; y++ )
    {
        // for each pixel
        for (x = startX; x < stopX; x++, src += 4, dst += 4 )
        {
            r = g = b = a = div = processedKernelSize = 0;

            // for each kernel row
            for (i = 0; i < size; i++ )
            {
                ir = i - radius;
                t = y + ir;

                // skip row
                if ( t < startY )
                    continue;
                // break
                if ( t >= stopY )
                    break;

                // for each kernel column
                for (j = 0; j < size; j++ )
                {
                    jr = j - radius;
                    t = x + jr;

                    // skip column
                    if ( t < startX )
                        continue;

                    if ( t < stopX )
                    {
                        k = kernel[i*kl+j];
                        p = &src[ir * srcStride + jr * 4];

                        div += k;

                        r += k * p[RGB_R];
                        g += k * p[RGB_G];
                        b += k * p[RGB_B];
                        a += k * p[RGB_A];

                        processedKernelSize++;
                    }
                }
            }

            // check if all kernel elements were processed
            if ( processedKernelSize == kernelSize )
            {
                // all kernel elements are processed - we are not on the edge
                div = divisor;
            }
            else
            {
                // we are on edge. do we need to use dynamic divisor or not?
                if ( !dynamicDivisorForEdges )
                {
                    // do
                    div = divisor;
                }
            }

            // check divider
            if ( div != 0 )
            {
                r /= div;
                g /= div;
                b /= div;
                a /= div;
            }
            r += threshold;
            g += threshold;
            b += threshold;
            a += threshold;

            dst[RGB_R] = (byte) ( ( r > 255 ) ? 255 : ( ( r < 0 ) ? 0 : r ) );
            dst[RGB_G] = (byte) ( ( g > 255 ) ? 255 : ( ( g < 0 ) ? 0 : g ) );
            dst[RGB_B] = (byte) ( ( b > 255 ) ? 255 : ( ( b < 0 ) ? 0 : b ) );
            dst[RGB_A] = (byte) ( ( a > 255 ) ? 255 : ( ( a < 0 ) ? 0 : a ) );
        }
        src += srcOffset;
        dst += dstOffset;
    }
}

                }
            }
        }
        else
        {
            int dstStride = destination->s / 2;
            int srcStride = source->s / 2;

            // base pointers
            const ushort* baseSrc = source->tt.u2;
            ushort* baseDst = destination->tt.u2;
            
            pixelSize /= 2;

            // allign pointers by X
            baseSrc += ( startX * pixelSize );
            baseDst += ( startX * pixelSize );

            if ( source->t == PixelFormat_16bppGrayScale )
            {
                // 16 bpp grayscale image
                //Process16bppImage( baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY );
                
// Process 16 bpp grayscale images
// int Process16bppImage( ushort* baseSrc, ushort* baseDst, int srcStride, int dstStride, int startX, int startY, int stopX, int stopY )
{
    // loop and array indexes
    int i, j, t, k, ir, jr;
    // kernel's radius
    int radius = size >> 1;
    // color sums
    long g, div;

    // kernel size
    int kernelSize = size * size;
    // number of kernel elements taken into account
    int processedKernelSize;

    // for each line
    for (y = startY; y < stopY; y++ )
    {
        const ushort* src = baseSrc + y * srcStride;
        ushort* dst = baseDst + y * dstStride;

        // for each pixel
        for (x = startX; x < stopX; x++, src++, dst++ )
        {
            g = div = processedKernelSize = 0;

            // for each kernel row
            for (i = 0; i < size; i++ )
            {
                ir = i - radius;
                t = y + ir;

                // skip row
                if ( t < startY )
                    continue;
                // break
                if ( t >= stopY )
                    break;

                // for each kernel column
                for (j = 0; j < size; j++ )
                {
                    jr = j - radius;
                    t = x + jr;

                    // skip column
                    if ( t < startX )
                        continue;

                    if ( t < stopX )
                    {
                        k = kernel[i*kl+j];

                        div += k;
                        g += k * src[ir * srcStride + jr];
                        processedKernelSize++;
                    }
                }
            }

            // check if all kernel elements were processed
            if ( processedKernelSize == kernelSize )
            {
                // all kernel elements are processed - we are not on the edge
                div = divisor;
            }
            else
            {
                // we are on edge. do we need to use dynamic divisor or not?
                if ( !dynamicDivisorForEdges )
                {
                    // do
                    div = divisor;
                }
            }

            // check divider
            if ( div != 0 )
            {
                g /= div;
            }
            g += threshold;
            *dst = (ushort) ( ( g > 65535 ) ? 65535 : ( ( g < 0 ) ? 0 : g ) );
        }
    }
}

            }
            else
            {
                // RGB image
                if ( ( pixelSize == 3 ) || ( !processAlpha ) )
                {
                    //Process48bppImage( baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY, pixelSize );
                    
// Process 48 bpp images or 64 bpp images with copying alpha channel
// int Process48bppImage( ushort* baseSrc, ushort* baseDst, int srcStride, int dstStride, int startX, int startY, int stopX, int stopY, int pixelSize )
{
    // loop and array indexes
    int i, j, t, k, ir, jr;
    // kernel's radius
    int radius = size >> 1;
    // color sums
    long r, g, b, div;

    // kernel size
    int kernelSize = size * size;
    // number of kernel elements taken into account
    int processedKernelSize;

    const ushort* p;

    // for each line
    for (y = startY; y < stopY; y++ )
    {
        const ushort* src = baseSrc + y * srcStride;
        ushort* dst = baseDst + y * dstStride;

        // for each pixel
        for (x = startX; x < stopX; x++, src += pixelSize, dst += pixelSize )
        {
            r = g = b = div = processedKernelSize = 0;

            // for each kernel row
            for (i = 0; i < size; i++ )
            {
                ir = i - radius;
                t = y + ir;

                // skip row
                if ( t < startY )
                    continue;
                // break
                if ( t >= stopY )
                    break;

                // for each kernel column
                for (j = 0; j < size; j++ )
                {
                    jr = j - radius;
                    t = x + jr;

                    // skip column
                    if ( t < startX )
                        continue;

                    if ( t < stopX )
                    {
                        k = kernel[i*kl+j];
                        p = &src[ir * srcStride + jr * pixelSize];

                        div += k;

                        r += k * p[RGB_R];
                        g += k * p[RGB_G];
                        b += k * p[RGB_B];

                        processedKernelSize++;
                    }
                }
            }

            // check if all kernel elements were processed
            if ( processedKernelSize == kernelSize )
            {
                // all kernel elements are processed - we are not on the edge
                div = divisor;
            }
            else
            {
                // we are on edge. do we need to use dynamic divisor or not?
                if ( !dynamicDivisorForEdges )
                {
                    // do
                    div = divisor;
                }
            }

            // check divider
            if ( div != 0 )
            {
                r /= div;
                g /= div;
                b /= div;
            }
            r += threshold;
            g += threshold;
            b += threshold;

            dst[RGB_R] = (ushort) ( ( r > 65535 ) ? 65535 : ( ( r < 0 ) ? 0 : r ) );
            dst[RGB_G] = (ushort) ( ( g > 65535 ) ? 65535 : ( ( g < 0 ) ? 0 : g ) );
            dst[RGB_B] = (ushort) ( ( b > 65535 ) ? 65535 : ( ( b < 0 ) ? 0 : b ) );

            // take care of alpha channel
            if ( pixelSize == 4 )
                dst[RGB_A] = src[RGB_A];
        }
    }
}

                }
                else
                {
                    // Process64bppImage( baseSrc, baseDst, srcStride, dstStride, startX, startY, stopX, stopY );
                    
// Process 64 bpp images including alpha channel
// int Process64bppImage( ushort* baseSrc, ushort* baseDst, int srcStride, int dstStride, int startX, int startY, int stopX, int stopY )
{
    // loop and array indexes
    int i, j, t, k, ir, jr;
    // kernel's radius
    int radius = size >> 1;
    // color sums
    long r, g, b, a, div;

    // kernel size
    int kernelSize = size * size;
    // number of kernel elements taken into account
    int processedKernelSize;

    const ushort* p;

    // for each line
    for (y = startY; y < stopY; y++ )
    {
        const ushort* src = baseSrc + y * srcStride;
        ushort* dst = baseDst + y * dstStride;

        // for each pixel
        for (x = startX; x < stopX; x++, src += 4, dst += 4 )
        {
            r = g = b = a = div = processedKernelSize = 0;

            // for each kernel row
            for (i = 0; i < size; i++ )
            {
                ir = i - radius;
                t = y + ir;

                // skip row
                if ( t < startY )
                    continue;
                // break
                if ( t >= stopY )
                    break;

                // for each kernel column
                for (j = 0; j < size; j++ )
                {
                    jr = j - radius;
                    t = x + jr;

                    // skip column
                    if ( t < startX )
                        continue;

                    if ( t < stopX )
                    {
                        k = kernel[i*kl+j];
                        p = &src[ir * srcStride + jr * 4];

                        div += k;

                        r += k * p[RGB_R];
                        g += k * p[RGB_G];
                        b += k * p[RGB_B];
                        a += k * p[RGB_A];

                        processedKernelSize++;
                    }
                }
            }

            // check if all kernel elements were processed
            if ( processedKernelSize == kernelSize )
            {
                // all kernel elements are processed - we are not on the edge
                div = divisor;
            }
            else
            {
                // we are on edge. do we need to use dynamic divisor or not?
                if ( !dynamicDivisorForEdges )
                {
                    // do
                    div = divisor;
                }
            }

            // check divider
            if ( div != 0 )
            {
                r /= div;
                g /= div;
                b /= div;
                a /= div;
            }
            r += threshold;
            g += threshold;
            b += threshold;
            a += threshold;

            dst[RGB_R] = (ushort) ( ( r > 65535 ) ? 65535 : ( ( r < 0 ) ? 0 : r ) );
            dst[RGB_G] = (ushort) ( ( g > 65535 ) ? 65535 : ( ( g < 0 ) ? 0 : g ) );
            dst[RGB_B] = (ushort) ( ( b > 65535 ) ? 65535 : ( ( b < 0 ) ? 0 : b ) );
            dst[RGB_A] = (ushort) ( ( a > 65535 ) ? 65535 : ( ( a < 0 ) ? 0 : a ) );
        }
    }
}

                }
            }
        }
    }
  return 0;
}


int ConvolutionInPlace(img_t* source, IRECT rect, const int* kernel, int kl, IDIM ksz) {
  img_t dst[1]={0};
  Image_Clone(source, dst);
  Convolution(dst, source, rect, kernel, kl, ksz);
  imfree(dst);
  return 0;
}