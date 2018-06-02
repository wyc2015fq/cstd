// AForge Image Processing Library
// AForge.NET framework
//
// Copyright © Andrew Kirillov, 2005-2008
// andrew.kirillov@gmail.com
//

namespace AForge.Imaging.Filters
{
    using System;
    using System.Collections.Generic;
    using System.Drawing;
    using System.Drawing.Imaging;

    /// <summary>
    /// Invert image.
    /// </summary>
    /// 
    /// <remarks><para>The filter inverts colored and grayscale images.</para>
    ///
    /// <para>The filter accepts 8, 16 bpp grayscale and 24, 48 bpp color images for processing.</para>
    /// 
    /// <para>Sample usage:</para>
    /// <code>
    /// // create filter
    /// Invert filter = new Invert( );
    /// // apply the filter
    /// filter.ApplyInPlace( image );
    /// </code>
    /// 
    /// <para><b>Initial image:</b></para>
    /// <img src="img/imaging/sample1.jpg" width="480" height="361" />
    /// <para><b>Result image:</b></para>
    /// <img src="img/imaging/invert.jpg" width="480" height="361" />
    /// </remarks>
    ///
    public sealed class Invert : BaseInPlacePartialFilter
    {
        // private format translation dictionary
        private Dictionary<PixelFormat, PixelFormat> formatTranslations = new Dictionary<PixelFormat, PixelFormat>( );

        /// <summary>
        /// Format translations dictionary.
        /// </summary>
        public override Dictionary<PixelFormat, PixelFormat> FormatTranslations
        {
            get { return formatTranslations; }
        }
        
        /// <summary>   
        /// Initializes a new instance of the <see cref="Invert"/> class.
        /// </summary>
        public Invert( )
        {
            formatTranslations[PixelFormat_8bppIndexed]    = PixelFormat_8bppIndexed;
            formatTranslations[PixelFormat_24bppRgb]       = PixelFormat_24bppRgb;
            formatTranslations[PixelFormat_16bppGrayScale] = PixelFormat_16bppGrayScale;
            formatTranslations[PixelFormat_48bppRgb]       = PixelFormat_48bppRgb;
        }

        /// <summary>
        /// Process the filter on the specified image.
        /// </summary>
        /// 
        /// <param name="image">Source image data.</param>
        /// <param name="rect">Image rectangle for processing by the filter.</param>
        ///
        protected override  void ProcessFilter( UnmanagedImage image, Rectangle rect )
        {
            int pixelSize = ( ( image->t == PixelFormat_8bppIndexed ) ||
                              ( image->t == PixelFormat_16bppGrayScale ) ) ? 1 : 3;

            int startY  = rect.Top;
            int stopY   = startY + rect.Height;

            int startX  = rect.Left * pixelSize;
            int stopX   = startX + rect.Width * pixelSize;

            byte* basePtr = (byte*) image->tt.data;

            if (
                ( image->t == PixelFormat_8bppIndexed ) ||
                ( image->t == PixelFormat_24bppRgb ) )
            {
                int offset = image->s - ( stopX - startX );

                // allign pointer to the first pixel to process
                byte* ptr = basePtr + ( startY * image->s + rect.Left * pixelSize );

                // invert
                for (y = startY; y < stopY; y++ )
                {
                    for (x = startX; x < stopX; x++, ptr++ )
                    {
                        // ivert each pixel
                        *ptr = (byte) ( 255 - *ptr );
                    }
                    ptr += offset;
                }
            }
            else
            {
                int stride = image->s;

                // allign pointer to the first pixel to process
                basePtr += ( startY * image->s + rect.Left * pixelSize * 2 );

                // invert
                for (y = startY; y < stopY; y++ )
                {
                    ushort* ptr = (ushort*) ( basePtr );

                    for (x = startX; x < stopX; x++, ptr++ )
                    {
                        // ivert each pixel
                        *ptr = (ushort) ( 65535 - *ptr );
                    }
                    basePtr += stride;
                }
            }
        }
    }
}
