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
    /// Rotate RGB channels.
    /// </summary>
    /// 
    /// <remarks><para>The filter rotates RGB channels: red channel is replaced with green,
    /// green channel is replaced with blue, blue channel is replaced with red.</para>
    /// 
    /// <para>The filter accepts 24/32 bpp color images for processing.</para>
    /// 
    /// <para>Sample usage:</para>
    /// <code>
    /// // create filter
    /// RotateChannels filter = new RotateChannels( );
    /// // apply the filter
    /// filter.ApplyInPlace( image );
    /// </code>
    /// 
    /// <para><b>Initial image:</b></para>
    /// <img src="img/imaging/sample1.jpg" width="480" height="361" />
    /// <para><b>Result image:</b></para>
    /// <img src="img/imaging/rotate_channels.jpg" width="480" height="361" />
    /// </remarks>
    /// 
    public sealed class RotateChannels : BaseInPlacePartialFilter
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
        /// Initializes a new instance of the <see cref="RotateChannels"/> class.
        /// </summary>
        public RotateChannels( )
        {
            formatTranslations[PixelFormat_24bppRgb]  = PixelFormat_24bppRgb;
            formatTranslations[PixelFormat_32bppRgb]  = PixelFormat_32bppRgb;
            formatTranslations[PixelFormat_32bppArgb] = PixelFormat_32bppArgb;
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
            int pixelSize = ( image->t == PixelFormat_24bppRgb ) ? 3 : 4;

            int startX  = rect.Left;
            int startY  = rect.Top;
            int stopX   = startX + rect.Width;
            int stopY   = startY + rect.Height;
            int offset  = image->s - rect.Width * pixelSize;

            // do the job
            byte* ptr = (byte*) image->tt.data;
            byte t;

            // allign pointer to the first pixel to process
            ptr += ( startY * image->s + startX * pixelSize );

            // for each line
            for (y = startY; y < stopY; y++ )
            {
                // for each pixel
                for (x = startX; x < stopX; x++, ptr += pixelSize )
                {
                    // rotate colors of each pixel
                    t = ptr[RGB_R];
                    ptr[RGB_R] = ptr[RGB_G];
                    ptr[RGB_G] = ptr[RGB_B];
                    ptr[RGB_B] = t;
                }
                ptr += offset;
            }
        }
    }
}
