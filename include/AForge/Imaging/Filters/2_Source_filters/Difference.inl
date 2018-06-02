// AForge Image Processing Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © AForge.NET, 2005-2011
// contacts@aforgenet.com
//

namespace AForge.Imaging.Filters
{
    using System;
    using System.Collections.Generic;
    using System.Drawing;
    using System.Drawing.Imaging;

    /// <summary>
    /// Difference filter - get the difference between overlay and source images.
    /// </summary>
    /// 
    /// <remarks><para>The difference filter takes two images (source and
    /// <see cref="BaseInPlaceFilter2.OverlayImage">overlay</see> images)
    /// of the same size and pixel format and produces an image, where each pixel equals
    /// to absolute difference between corresponding pixels from provided images.</para>
    /// 
    /// <para>The filter accepts 8 and 16 bpp grayscale images and 24, 32, 48 and 64 bpp
    /// color images for processing.</para>
    /// 
    /// <para><note>In the case if images with alpha channel are used (32 or 64 bpp), visualization
    /// of the result image may seem a bit unexpected - most probably nothing will be seen
    /// (in the case if image is displayed according to its alpha channel). This may be
    /// caused by the fact that after differencing the entire alpha channel will be zeroed
    /// (zero difference between alpha channels), what means that the resulting image will be
    /// 100% transparent.</note></para>
    /// 
    /// <para>Sample usage:</para>
    /// <code>
    /// // create filter
    /// Difference filter = new Difference( overlayImage );
    /// // apply the filter
    /// Bitmap resultImage = filter.Apply( sourceImage );
    /// </code>
    ///
    /// <para><b>Source image:</b></para>
    /// <img src="img/imaging/sample6.png" width="320" height="240" />
    /// <para><b>Overlay image:</b></para>
    /// <img src="img/imaging/sample7.png" width="320" height="240" />
    /// <para><b>Result image:</b></para>
    /// <img src="img/imaging/difference.png" width="320" height="240" />
    /// </remarks>
    ///
    /// <seealso cref="Intersect"/>
    /// <seealso cref="Merge"/>
    /// <seealso cref="Add"/>
    /// <seealso cref="Subtract"/>
    /// 
    public sealed class Difference : BaseInPlaceFilter2
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
        /// Initializes a new instance of the <see cref="Difference"/> class.
        /// </summary>
        public Difference( )
        {
            InitFormatTranslations( );
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Difference"/> class.
        /// </summary>
        /// 
        /// <param name="overlayImage">Overlay image.</param>
        /// 
        public Difference( Bitmap overlayImage )
            : base( overlayImage )
        {
            InitFormatTranslations( );
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Difference"/> class.
        /// </summary>
        /// 
        /// <param name="unmanagedOverlayImage">Unmanaged overlay image.</param>
        /// 
        public Difference( UnmanagedImage unmanagedOverlayImage )
            : base( unmanagedOverlayImage )
        {
            InitFormatTranslations( );
        }

        // Initialize format translation dictionary
        private void InitFormatTranslations( )
        {
            formatTranslations[PixelFormat_8bppIndexed]    = PixelFormat_8bppIndexed;
            formatTranslations[PixelFormat_24bppRgb]       = PixelFormat_24bppRgb;
            formatTranslations[PixelFormat_32bppRgb]       = PixelFormat_32bppRgb;
            formatTranslations[PixelFormat_32bppArgb]      = PixelFormat_32bppArgb;
            formatTranslations[PixelFormat_16bppGrayScale] = PixelFormat_16bppGrayScale;
            formatTranslations[PixelFormat_48bppRgb]       = PixelFormat_48bppRgb;
            formatTranslations[PixelFormat_64bppArgb]      = PixelFormat_64bppArgb;
        }

        /// <summary>
        /// Process the filter on the specified image.
        /// </summary>
        /// 
        /// <param name="image">Source image data.</param>
        /// <param name="overlay">Overlay image data.</param>
        ///
        protected override  void ProcessFilter( UnmanagedImage image, UnmanagedImage overlay )
        {
            PixelFormat pixelFormat = image->t;
            // get image dimension
            int width  = image.Width;
            int height = image.Height;
            // pixel value
            int v;

            if (
                ( pixelFormat == PixelFormat_8bppIndexed ) ||
                ( pixelFormat == PixelFormat_24bppRgb ) ||
                ( pixelFormat == PixelFormat_32bppRgb ) ||
                ( pixelFormat == PixelFormat_32bppArgb ) )
            {
                // initialize other variables
                int pixelSize = ( pixelFormat == PixelFormat_8bppIndexed ) ? 1 :
                    ( pixelFormat == PixelFormat_24bppRgb ) ? 3 : 4;
                int lineSize  = width * pixelSize;
                int srcOffset = image->s - lineSize;
                int ovrOffset = overlay->s - lineSize;

                // do the job
                byte * ptr = (byte*) image->tt.data;
                byte * ovr = (byte*) overlay->tt.data;

                // for each line
                for (y = 0; y < height; y++ )
                {
                    // for each pixel
                    for (x = 0; x < lineSize; x++, ptr++, ovr++ )
                    {
                        // abs(sub)
                        v = (int) *ptr - (int) *ovr;
                        *ptr = ( v < 0 ) ? (byte) -v : (byte) v;
                    }
                    ptr += srcOffset;
                    ovr += ovrOffset;
                }
            }
            else
            {
                // initialize other variables
                int pixelSize = ( pixelFormat == PixelFormat_16bppGrayScale ) ? 1 :
                    ( pixelFormat == PixelFormat_48bppRgb ) ? 3 : 4;
                int lineSize  = width * pixelSize;
                int srcStride = image->s;
                int ovrStride = overlay->s;

                // do the job
                byte* basePtr = (byte*) image->tt.data;
                byte* baseOvr = (byte*) overlay->tt.data;

                // for each line
                for (y = 0; y < height; y++ )
                {
                    ushort * ptr = (ushort*) ( basePtr + y * srcStride );
                    ushort * ovr = (ushort*) ( baseOvr + y * ovrStride );

                    // for each pixel
                    for (x = 0; x < lineSize; x++, ptr++, ovr++ )
                    {
                        // abs(sub)
                        v = (int) *ptr - (int) *ovr;
                        *ptr = ( v < 0 ) ? (ushort) -v : (ushort) v;
                    }
                }
            }
        }
    }
}
