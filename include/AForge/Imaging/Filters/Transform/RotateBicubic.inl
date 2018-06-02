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
    /// Rotate image using bicubic interpolation.
    /// </summary>
    /// 
    /// <remarks><para>The class implements image rotation filter using bicubic
    /// interpolation algorithm. It uses bicubic kernel W(x) as described on
    /// <a href="http://en.wikipedia.org/wiki/Bicubic_interpolation#Bicubic_convolution_algorithm">Wikipedia</a>
    /// (coefficient <b>a</b> is set to <b>-0.5</b>).</para>
    /// 
    /// <para><note>Rotation is performed in counterclockwise direction.</note></para>
    /// 
    /// <para>The filter accepts 8 bpp grayscale images and 24 bpp
    /// color images for processing.</para>
    ///
    /// <para>Sample usage:</para>
    /// <code>
    /// // create filter - rotate for 30 degrees keeping original image size
    /// RotateBicubic filter = new RotateBicubic( 30, true );
    /// // apply the filter
    /// Bitmap newImage = filter.Apply( image );
    /// </code>
    /// 
    /// <para><b>Initial image:</b></para>
    /// <img src="img/imaging/sample9.png" width="320" height="240" />
    /// <para><b>Result image:</b></para>
    /// <img src="img/imaging/rotate_bicubic.png" width="320" height="240" />
    /// </remarks>
    /// 
    /// <seealso cref="RotateBilinear"/>
    /// <seealso cref="RotateNearestNeighbor"/>
    /// 
    public class RotateBicubic : BaseRotateFilter
    {
        // format translation dictionary
        private Dictionary<PixelFormat, PixelFormat> formatTranslations = new Dictionary<PixelFormat, PixelFormat>( );

        /// <summary>
        /// Format translations dictionary.
        /// </summary>
        public override Dictionary<PixelFormat, PixelFormat> FormatTranslations
        {
            get { return formatTranslations; }
        }
        
        /// <summary>
        /// Initializes a new instance of the <see cref="RotateBicubic"/> class.
        /// </summary>
        /// 
        /// <param name="angle">Rotation angle.</param>
        /// 
        /// <remarks><para>This constructor sets <see cref="BaseRotateFilter.KeepSize"/> property
        /// to <see langword="false"/>.</para>
        /// </remarks>
        ///
        public RotateBicubic( double angle ) :
            this( angle, false )
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="RotateBicubic"/> class.
        /// </summary>
        /// 
        /// <param name="angle">Rotation angle.</param>
        /// <param name="keepSize">Keep image size or not.</param>
        /// 
        public RotateBicubic( double angle, bool keepSize ) :
            base( angle, keepSize )
        {
            formatTranslations[PixelFormat_8bppIndexed] = PixelFormat_8bppIndexed;
            formatTranslations[PixelFormat_24bppRgb]    = PixelFormat_24bppRgb;
        }

        /// <summary>
        /// Process the filter on the specified image.
        /// </summary>
        /// 
        /// <param name="sourceData">Source image data.</param>
        /// <param name="destinationData">Destination image data.</param>
        ///
        protected override  void ProcessFilter( UnmanagedImage sourceData, UnmanagedImage destinationData )
        {
            // get source image size
            int    width      = sourceData.Width;
            int    height     = sourceData.Height;
            double oldXradius = (double) ( width  - 1 ) / 2;
            double oldYradius = (double) ( height - 1 ) / 2;

            // get destination image size
            int    newWidth   = destinationData.Width;
            int    newHeight  = destinationData.Height;
            double newXradius = (double) ( newWidth  - 1 ) / 2;
            double newYradius = (double) ( newHeight - 1 ) / 2;

            // angle's sine and cosine
            double angleRad = -angle * CV_PI / 180;
            double angleCos = Math.Cos( angleRad );
            double angleSin = Math.Sin( angleRad );

            int srcStride = sourceData->s;
            int dstOffset = destinationData->s -
                ( ( destinationData->t == PixelFormat_8bppIndexed ) ? newWidth : newWidth * 3 );

            // fill values
            byte fillR = fillColor.R;
            byte fillG = fillColor.G;
            byte fillB = fillColor.B;

            // do the job
            byte* src = (byte*) sourceData->tt.data;
            byte* dst = (byte*) destinationData->tt.data;

            // destination pixel's coordinate relative to image center
            double cx, cy;
            // coordinates of source points and cooefficiens
            double  ox, oy, dx, dy, k1, k2;
            int     ox1, oy1, ox2, oy2;
            // destination pixel values
            double r, g, b;
            // width and height decreased by 1
            int ymax = height - 1;
            int xmax = width - 1;
            // temporary pointer
            byte* p;

            if ( destinationData->t == PixelFormat_8bppIndexed )
            {
                // grayscale
                cy = -newYradius;
                for (y = 0; y < newHeight; y++ )
                {
                    cx = -newXradius;
                    for (x = 0; x < newWidth; x++, dst++ )
                    {
                        // coordinates of source point
                        ox = angleCos * cx + angleSin * cy + oldXradius;
                        oy = -angleSin * cx + angleCos * cy + oldYradius;

                        ox1 = (int) ox;
                        oy1 = (int) oy;

                        // validate source pixel's coordinates
                        if ( ( ox1 < 0 ) || ( oy1 < 0 ) || ( ox1 >= width ) || ( oy1 >= height ) )
                        {
                            // fill destination image with filler
                            *dst = fillG;
                        }
                        else
                        {
                            dx = ox - (double) ox1;
                            dy = oy - (double) oy1;

                            // initial pixel value
                            g = 0;

                            for (n = -1; n < 3; n++ )
                            {
                                // get Y cooefficient
                                k1 = Interpolation.BiCubicKernel( dy - (double) n );

                                oy2 = oy1 + n;
                                if ( oy2 < 0 )
                                    oy2 = 0;
                                if ( oy2 > ymax )
                                    oy2 = ymax;

                                for (m = -1; m < 3; m++ )
                                {
                                    // get X cooefficient
                                    k2 = k1 * Interpolation.BiCubicKernel( (double) m - dx );

                                    ox2 = ox1 + m;
                                    if ( ox2 < 0 )
                                        ox2 = 0;
                                    if ( ox2 > xmax )
                                        ox2 = xmax;

                                    g += k2 * src[oy2 * srcStride + ox2];
                                }
                            }
                            *dst = (byte) MAX( 0, MIN( 255, g ) );
                        }
                        cx++;
                    }
                    cy++;
                    dst += dstOffset;
                }
            }
            else
            {
                // RGB
                cy = -newYradius;
                for (y = 0; y < newHeight; y++ )
                {
                    cx = -newXradius;
                    for (x = 0; x < newWidth; x++, dst += 3 )
                    {
                        // coordinates of source point
                        ox =  angleCos * cx + angleSin * cy + oldXradius;
                        oy = -angleSin * cx + angleCos * cy + oldYradius;

                        ox1 = (int) ox;
                        oy1 = (int) oy;

                        // validate source pixel's coordinates
                        if ( ( ox1 < 0 ) || ( oy1 < 0 ) || ( ox1 >= width ) || ( oy1 >= height ) )
                        {
                            // fill destination image with filler
                            dst[RGB_R] = fillR;
                            dst[RGB_G] = fillG;
                            dst[RGB_B] = fillB;
                        }
                        else
                        {
                            dx = ox - (float) ox1;
                            dy = oy - (float) oy1;

                            // initial pixel value
                            r = g = b = 0;

                            for (n = -1; n < 3; n++ )
                            {
                                // get Y cooefficient
                                k1 = Interpolation.BiCubicKernel( dy - (float) n );

                                oy2 = oy1 + n;
                                if ( oy2 < 0 )
                                    oy2 = 0;
                                if ( oy2 > ymax )
                                    oy2 = ymax;

                                for (m = -1; m < 3; m++ )
                                {
                                    // get X cooefficient
                                    k2 = k1 * Interpolation.BiCubicKernel( (float) m - dx );

                                    ox2 = ox1 + m;
                                    if ( ox2 < 0 )
                                        ox2 = 0;
                                    if ( ox2 > xmax )
                                        ox2 = xmax;

                                    // get pixel of original image
                                    p = src + oy2 * srcStride + ox2 * 3;

                                    r += k2 * p[RGB_R];
                                    g += k2 * p[RGB_G];
                                    b += k2 * p[RGB_B];
                                }
                            }
                            dst[RGB_R] = (byte) MAX( 0, MIN( 255, r ) );
                            dst[RGB_G] = (byte) MAX( 0, MIN( 255, g ) );
                            dst[RGB_B] = (byte) MAX( 0, MIN( 255, b ) );
                        }
                        cx++;
                    }
                    cy++;
                    dst += dstOffset;
                }
            }
        }
    }
}
