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
    /// Rotate image using bilinear interpolation.
    /// </summary>
    /// 
    /// <para><note>Rotation is performed in counterclockwise direction.</note></para>
    /// 
    /// <remarks><para>The class implements image rotation filter using bilinear
    /// interpolation algorithm.</para>
    /// 
    /// <para>The filter accepts 8 bpp grayscale images and 24 bpp
    /// color images for processing.</para>
    ///
    /// <para>Sample usage:</para>
    /// <code>
    /// // create filter - rotate for 30 degrees keeping original image size
    /// RotateBilinear filter = new RotateBilinear( 30, true );
    /// // apply the filter
    /// Bitmap newImage = filter.Apply( image );
    /// </code>
    /// 
    /// <para><b>Initial image:</b></para>
    /// <img src="img/imaging/sample9.png" width="320" height="240" />
    /// <para><b>Result image:</b></para>
    /// <img src="img/imaging/rotate_bilinear.png" width="320" height="240" />
    /// </remarks>
    /// 
    /// <seealso cref="RotateNearestNeighbor"/>
    /// <seealso cref="RotateBicubic"/>
    /// 
    public class RotateBilinear : BaseRotateFilter
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
        /// Initializes a new instance of the <see cref="RotateBilinear"/> class.
        /// </summary>
        /// 
        /// <param name="angle">Rotation angle.</param>
        /// 
        /// <remarks><para>This constructor sets <see cref="BaseRotateFilter.KeepSize"/> property
        /// to <see langword="false"/>.</para>
        /// </remarks>
        /// 
		public RotateBilinear( double  angle ) :
            this( angle, false )
		{
		}

        /// <summary>
        /// Initializes a new instance of the <see cref="RotateBilinear"/> class.
        /// </summary>
        /// 
        /// <param name="angle">Rotation angle.</param>
        /// <param name="keepSize">Keep image size or not.</param>
        /// 
        public RotateBilinear( double angle, bool keepSize ) :
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
            int     width      = sourceData.Width;
            int     height     = sourceData.Height;
            double  oldXradius = (double) ( width  - 1 ) / 2;
            double  oldYradius = (double) ( height - 1 ) / 2;

            // get destination image size
            int     newWidth   = destinationData.Width;
            int     newHeight  = destinationData.Height;
            double  newXradius = (double) ( newWidth  - 1 ) / 2;
            double  newYradius = (double) ( newHeight - 1 ) / 2;

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
            // coordinates of source points
            double  ox, oy, tx, ty, dx1, dy1, dx2, dy2;
            int     ox1, oy1, ox2, oy2;
            // width and height decreased by 1
            int ymax = height - 1;
            int xmax = width - 1;
            // temporary pointers
            byte* p1, p2, p3, p4;

            // check pixel format
            if ( destinationData->t == PixelFormat_8bppIndexed )
            {
                // grayscale
                cy = -newYradius;
                for (y = 0; y < newHeight; y++ )
                {
                    // do some pre-calculations of source points' coordinates
                    // (calculate the part which depends on y-loop, but does not
                    // depend on x-loop)
                    tx = angleSin * cy + oldXradius;
                    ty = angleCos * cy + oldYradius;

                    cx = -newXradius;
                    for (x = 0; x < newWidth; x++, dst++ )
                    {
                        // coordinates of source point
                        ox = tx + angleCos * cx;
                        oy = ty - angleSin * cx;

                        // top-left coordinate
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
                            // bottom-right coordinate
                            ox2 = ( ox1 == xmax ) ? ox1 : ox1 + 1;
                            oy2 = ( oy1 == ymax ) ? oy1 : oy1 + 1;

                            if ( ( dx1 = ox - (double) ox1 ) < 0 )
                                dx1 = 0;
                            dx2 = 1.0 - dx1;

                            if ( ( dy1 = oy - (double) oy1 ) < 0 )
                                dy1 = 0;
                            dy2 = 1.0 - dy1;

                            p1 = src + oy1 * srcStride;
                            p2 = src + oy2 * srcStride;

                            // interpolate using 4 points
                            *dst = (byte) (
                                dy2 * ( dx2 * p1[ox1] + dx1 * p1[ox2] ) +
                                dy1 * ( dx2 * p2[ox1] + dx1 * p2[ox2] ) );
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
                    // do some pre-calculations of source points' coordinates
                    // (calculate the part which depends on y-loop, but does not
                    // depend on x-loop)
                    tx = angleSin * cy + oldXradius;
                    ty = angleCos * cy + oldYradius;

                    cx = -newXradius;
                    for (x = 0; x < newWidth; x++, dst += 3 )
                    {
                        // coordinates of source point
                        ox = tx + angleCos * cx;
                        oy = ty - angleSin * cx;

                        // top-left coordinate
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
                            // bottom-right coordinate
                            ox2 = ( ox1 == xmax ) ? ox1 : ox1 + 1;
                            oy2 = ( oy1 == ymax ) ? oy1 : oy1 + 1;

                            if ( ( dx1 = ox - (float) ox1 ) < 0 )
                                dx1 = 0;
                            dx2 = 1.0f - dx1;

                            if ( ( dy1 = oy - (float) oy1 ) < 0 )
                                dy1 = 0;
                            dy2 = 1.0f - dy1;

                            // get four points
                            p1 = p2 = src + oy1 * srcStride;
                            p1 += ox1 * 3;
                            p2 += ox2 * 3;

                            p3 = p4 = src + oy2 * srcStride;
                            p3 += ox1 * 3;
                            p4 += ox2 * 3;

                            // interpolate using 4 points

                            // red
                            dst[RGB_R] = (byte) (
                                dy2 * ( dx2 * p1[RGB_R] + dx1 * p2[RGB_R] ) +
                                dy1 * ( dx2 * p3[RGB_R] + dx1 * p4[RGB_R] ) );

                            // green
                            dst[RGB_G] = (byte) (
                                dy2 * ( dx2 * p1[RGB_G] + dx1 * p2[RGB_G] ) +
                                dy1 * ( dx2 * p3[RGB_G] + dx1 * p4[RGB_G] ) );

                            // blue
                            dst[RGB_B] = (byte) (
                                dy2 * ( dx2 * p1[RGB_B] + dx1 * p2[RGB_B] ) +
                                dy1 * ( dx2 * p3[RGB_B] + dx1 * p4[RGB_B] ) );
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
