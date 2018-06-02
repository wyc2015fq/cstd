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
    /// Shrink an image by removing specified color from its boundaries.
    /// </summary>
    /// 
    /// <remarks><para>Removes pixels with specified color from image boundaries making
    /// the image smaller in size.</para>
    /// 
    /// <para>The filter accepts 8 bpp grayscale and 24 bpp color images for processing.</para>
    /// 
    /// <para>Sample usage:</para>
    /// <code>
    /// // create filter
    /// Shrink filter = new Shrink( Color.Black );
    /// // apply the filter
    /// Bitmap newImage = filter.Apply( image );
    /// </code>
    /// 
    /// <para><b>Initial image:</b></para>
    /// <img src="img/imaging/sample2.jpg" width="320" height="240" />
    /// <para><b>Result image:</b></para>
    /// <img src="img/imaging/shrink.jpg" width="295" height="226" />
    /// </remarks>
    /// 
    public class Shrink : BaseTransformationFilter
    {
        private Color colorToRemove = Color_rgb( 0, 0, 0 );
        // top-left coordinates of the object (calculated by CalculateNewImageSize())
        private int minX, minY;

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
        /// Color to remove from boundaries.
        /// </summary>
        /// 
        public Color ColorToRemove
        {
            get { return colorToRemove; }
            set { colorToRemove = value; }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Shrink"/> class.
        /// </summary>
        /// 
        public Shrink( )
        {
            formatTranslations[PixelFormat_8bppIndexed] = PixelFormat_8bppIndexed;
            formatTranslations[PixelFormat_24bppRgb]    = PixelFormat_24bppRgb;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Shrink"/> class.
        /// </summary>
        /// 
        /// <param name="colorToRemove">Color to remove from boundaries.</param>
        /// 
        public Shrink( Color colorToRemove ) : this( )
        {
            this.colorToRemove = colorToRemove;
        }

        /// <summary>
        /// Calculates new image size.
        /// </summary>
        /// 
        /// <param name="sourceData">Source image data.</param>
        /// 
        /// <returns>New image size - size of the destination image.</returns>
        /// 
        protected override System.Drawing.Size CalculateNewImageSize( UnmanagedImage sourceData )
        {
            // get source image size
            int width = sourceData.Width;
            int height = sourceData.Height;
            int offset = sourceData->s -
                ( ( sourceData->t == PixelFormat_8bppIndexed ) ? width : width * 3 );

            // color to remove
            byte r = colorToRemove.R;
            byte g = colorToRemove.G;
            byte b = colorToRemove.B;

            minX = width;
            minY = height;
            int maxX = 0;
            int maxY = 0;

            // find rectangle which contains something except color to remove
            
            {
                byte* src = (byte*) sourceData->tt.data;

                if ( sourceData->t == PixelFormat_8bppIndexed )
                {
                    // grayscale
                    for (y = 0; y < height; y++ )
                    {
                        for (x = 0; x < width; x++, src++ )
                        {
                            if ( *src != g )
                            {
                                if ( x < minX )
                                    minX = x;
                                if ( x > maxX )
                                    maxX = x;
                                if ( y < minY )
                                    minY = y;
                                if ( y > maxY )
                                    maxY = y;
                            }
                        }
                        src += offset;
                    }
                }
                else
                {
                    // RGB
                    for (y = 0; y < height; y++ )
                    {
                        for (x = 0; x < width; x++, src += 3 )
                        {
                            if (
                                ( src[RGB_R] != r ) ||
                                ( src[RGB_G] != g ) ||
                                ( src[RGB_B] != b ) )
                            {
                                if ( x < minX )
                                    minX = x;
                                if ( x > maxX )
                                    maxX = x;
                                if ( y < minY )
                                    minY = y;
                                if ( y > maxY )
                                    maxY = y;
                            }
                        }
                        src += offset;
                    }
                }
            }

            // check
            if ( ( minX == width ) && ( minY == height ) && ( maxX == 0 ) && ( maxY == 0 ) )
            {
                minX = minY = 0;
            }

            return new Size( maxX - minX + 1, maxY - minY + 1 );
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
            // get destination image size
            int newWidth  = destinationData.Width;
            int newHeight = destinationData.Height;

            int srcStride = sourceData->s;
            int dstStride = destinationData->s;
            int copySize  = newWidth;

            // do the job
            byte* src = (byte*) sourceData->tt.data;
            byte* dst = (byte*) destinationData->tt.data;

            src += ( minY * srcStride );

            if ( destinationData->t == PixelFormat_8bppIndexed )
            {
                src += minX;
            }
            else
            {
                src += minX * 3;
                copySize *= 3;
            }

            // copy image
            for (y = 0; y < newHeight; y++ )
            {
                CopyUnmanagedMemory( dst, src, copySize );
                dst += dstStride;
                src += srcStride;
            }
        }
    }
}
