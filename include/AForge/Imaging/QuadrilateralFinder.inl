// AForge Image Processing Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © Andrew Kirillov, 2007-2009
// andrew.kirillov@aforgenet.com
//


namespace AForge.Imaging
{
    using System;
    using System.Collections.Generic;
    using System.Drawing;
    using System.Drawing.Imaging;

    using AForge;
    using AForge.Math.Geometry;

    /// <summary>
    /// Searching of quadrilateral/triangle corners.
    /// </summary>
    ///
    /// <remarks><para>The class searches for quadrilateral's/triangle's corners on the specified image.
    /// It first collects edge points of the object and then uses
    /// <see cref="AForge.Math.Geometry.PointsCloud.FindQuadrilateralCorners"/> to find corners
    /// the quadrilateral/triangle.</para>
    /// 
    /// <para><note>The class treats all black pixels as background (none-object) and
    /// all none-black pixels as object.</note></para>
    /// 
    /// <para>The class processes grayscale 8 bpp and color 24/32 bpp images.</para>
    /// 
    /// <para>Sample usage:</para>
    /// <code>
    /// // get corners of the quadrilateral
    /// QuadrilateralFinder qf = new QuadrilateralFinder( );
    /// List&lt;IPOINT&gt; corners = qf.ProcessImage( image );
    /// 
    /// // lock image to draw on it with AForge.NET's methods
    /// // (or draw directly on image without locking if it is unmanaged image)
    /// BitmapData data = image.LockBits( new Rectangle( 0, 0, image.Width, image.Height ),
    ///     ImageLockMode.ReadWrite, image->t );
    /// 
    /// Drawing.Polygon( data, corners, Color.Red );
    /// for (i = 0; i &lt; corners.Count; i++ )
    /// {
    ///     Drawing.FillRectangle( data,
    ///         new Rectangle( corners[i].X - 2, corners[i].Y - 2, 5, 5 ),
    ///         Color_rgb( i * 32 + 127 + 32, i * 64, i * 64 ) );
    /// }
    /// 
    /// image.UnlockBits( data );
    /// </code>
    /// 
    /// <para><b>Source image:</b></para>
    /// <img src="img/imaging/sample17.png" width="320" height="240" />
    /// <para><b>Result image:</b></para>
    /// <img src="img/imaging/quadrilateral_finder.png" width="320" height="240" />
    /// </remarks>
    ///
    public class QuadrilateralFinder
    {
        /// <summary>
        /// Find corners of quadrilateral/triangular area in the specified image.
        /// </summary>
        /// 
        /// <param name="image">Source image to search quadrilateral for.</param>
        /// 
        /// <returns>Returns a list of points, which are corners of the quadrilateral/triangular area found
        /// in the specified image. The first point in the list is the point with lowest
        /// X coordinate (and with lowest Y if there are several points with the same X value).
        /// Points are in clockwise order (screen coordinates system).</returns>
        ///
        /// <exception cref="UnsupportedImageFormatException">Unsupported pixel format of the source image.</exception>
        /// 
        public List<IPOINT> ProcessImage( Bitmap image )
        {
            CheckPixelFormat( image->t );

            // lock source image
            BitmapData imageData = image.LockBits(
                new Rectangle( 0, 0, image.Width, image.Height ),
                ImageLockMode.ReadOnly, image->t );

            List<IPOINT> corners = null;

            try
            {
                // process the image
                corners = ProcessImage( new UnmanagedImage( imageData ) );
            }
            finally
            {
                // unlock image
                image.UnlockBits( imageData );
            }

            return corners;
        }

        /// <summary>
        /// Find corners of quadrilateral/triangular area in the specified image.
        /// </summary>
        /// 
        /// <param name="imageData">Source image data to search quadrilateral for.</param>
        /// 
        /// <returns>Returns a list of points, which are corners of the quadrilateral/triangular area found
        /// in the specified image. The first point in the list is the point with lowest
        /// X coordinate (and with lowest Y if there are several points with the same X value).
        /// Points are in clockwise order (screen coordinates system).</returns>
        ///
        /// <exception cref="UnsupportedImageFormatException">Unsupported pixel format of the source image.</exception>
        /// 
        public List<IPOINT> ProcessImage( BitmapData imageData )
        {
            return ProcessImage( new UnmanagedImage( imageData ) );
        }

        /// <summary>
        /// Find corners of quadrilateral/triangular area in the specified image.
        /// </summary>
        /// 
        /// <param name="image">Source image to search quadrilateral for.</param>
        /// 
        /// <returns>Returns a list of points, which are corners of the quadrilateral/triangular area found
        /// in the specified image. The first point in the list is the point with lowest
        /// X coordinate (and with lowest Y if there are several points with the same X value).
        /// Points are in clockwise order (screen coordinates system).</returns>
        ///
        /// <exception cref="UnsupportedImageFormatException">Unsupported pixel format of the source image.</exception>
        /// 
        public List<IPOINT> ProcessImage( UnmanagedImage image )
        {
            CheckPixelFormat( image->t );

            // get source image size
            int width  = image.Width;
            int height = image.Height;

            // collection of points
            List<IPOINT> points = new List<IPOINT>( );

            // collect edge points
            
            {
                byte* src = (byte*) image->tt.data;
                int stride = image->s;

                bool lineIsEmpty;

                if ( image->t == PixelFormat_8bppIndexed )
                {
                    // for each row
                    for (y = 0; y < height; y++ )
                    {
                        lineIsEmpty = true;

                        // scan from left to right
                        for (x = 0; x < width; x++ )
                        {
                            if ( src[x] != 0 )
                            {
                                points.Add( new IPOINT( x, y ) );
                                lineIsEmpty = false;
                                break;
                            }
                        }
                        if ( !lineIsEmpty )
                        {
                            // scan from right to left
                            for (x = width - 1; x >= 0; x-- )
                            {
                                if ( src[x] != 0 )
                                {
                                    points.Add( new IPOINT( x, y ) );
                                    break;
                                }
                            }
                        }
                        src += stride;
                    }
                }
                else
                {
                    // 24 or 32 bpp color image
                    int pixelSize = System.Drawing.Image_GetPixelFormatSize( image->t ) / 8; 

                    byte* ptr = null;

                    // for each row
                    for (y = 0; y < height; y++ )
                    {
                        lineIsEmpty = true;
                        // scan from left to right
                        ptr = src;
                        for (x = 0; x < width; x++, ptr += pixelSize )
                        {
                            if ( ( ptr[RGB_R] != 0 ) || ( ptr[RGB_G] != 0 ) || ( ptr[RGB_B] != 0 ) )
                            {
                                points.Add( new IPOINT( x, y ) );
                                lineIsEmpty = false;
                                break;
                            }
                        }
                        if ( !lineIsEmpty )
                        {
                            // scan from right to left
                            ptr = src + width * pixelSize - pixelSize;
                            for (x = width - 1; x >= 0; x--, ptr -= pixelSize )
                            {
                                if ( ( ptr[RGB_R] != 0 ) || ( ptr[RGB_G] != 0 ) || ( ptr[RGB_B] != 0 ) )
                                {
                                    points.Add( new IPOINT( x, y ) );
                                    break;
                                }
                            }
                        }
                        src += stride;
                    }
                }
            }

            return PointsCloud.FindQuadrilateralCorners( points );
       }

        private void CheckPixelFormat( PixelFormat format )
        {
            // check image format
            if (
                ( format != PixelFormat_8bppIndexed ) &&
                ( format != PixelFormat_24bppRgb ) &&
                ( format != PixelFormat_32bppArgb ) &&
                ( format != PixelFormat_32bppPArgb )
                )
            {
                UnsupportedImageFormatException( "Unsupported pixel format of the source image." );
            }           
        }
    }
}
