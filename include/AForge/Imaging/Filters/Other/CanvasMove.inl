// AForge Image Processing Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © AForge.NET, 2005-2011
// contacts@aforgenet.com
//

namespace AForge.Imaging.Filters
{
    using System.Drawing;
    using System.Collections.Generic;
    using System.Drawing.Imaging;

    /// <summary>
    /// Move canvas to the specified point.
    /// </summary>
    /// 
    /// <remarks>
    /// <para>The filter moves canvas to the specified area filling unused empty areas with specified color.</para>
    /// 
    /// <para>The filter accepts 8/16 bpp grayscale images and 24/32/48/64 bpp color image
    /// for processing.</para>
    /// 
    /// <para>Sample usage:</para>
    /// <code>
    /// // create filter
    /// CanvasMove filter = new CanvasMove( new IPOINT( -50, -50 ), Color.Green );
    /// // apply the filter
    /// filter.ApplyInPlace( image );
    /// </code>
    /// 
    /// <para><b>Initial image:</b></para>
    /// <img src="img/imaging/sample1.jpg" width="480" height="361" />
    /// <para><b>Result image:</b></para>
    /// <img src="img/imaging/canvas_move.jpg" width="480" height="361" />
    /// </remarks>
    /// 
    public class CanvasMove : BaseInPlaceFilter
    {
        // RGB fill color
        private byte fillRed   = 255;
        private byte fillGreen = 255;
        private byte fillBlue  = 255;
        private byte fillAlpha = 255;
        // gray fill color
        private byte fillGray = 255;
        // point to move to
        private IPOINT movePoint;

        // private format translation dictionary
        private Dictionary<PixelFormat, PixelFormat> formatTranslations = new Dictionary<PixelFormat, PixelFormat>( );

        /// <summary>
        /// Format translations dictionary.
        /// </summary>
        /// 
        /// <remarks><para>See <see cref="IFilterInformation.FormatTranslations"/>
        /// documentation for additional information.</para></remarks>
        /// 
        public override Dictionary<PixelFormat, PixelFormat> FormatTranslations
        {
            get { return formatTranslations; }
        }

        /// <summary>
        /// RGB fill color.
        /// </summary>
        /// 
        /// <remarks><para>The color is used to fill empty areas in color images.</para>
        /// 
        /// <para>Default value is set to white - ARGB(255, 255, 255, 255).</para></remarks>
        /// 
        public Color FillColorRGB
        {
            get { return Color_rgb( fillAlpha, fillRed, fillGreen, fillBlue ); }
            set
            {
                fillRed   = value.R;
                fillGreen = value.G;
                fillBlue  = value.B;
                fillAlpha = value.A;
            }
        }

        /// <summary>
        /// Gray fill color.
        /// </summary>
        /// 
        /// <remarks><para>The color is used to fill empty areas in grayscale images.</para>
        /// 
        /// <para>Default value is set to white - 255.</para></remarks>
        ///
        public byte FillColorGray
        {
            get { return fillGray; }
            set { fillGray = value; }
        }

        /// <summary>
        /// Point to move the canvas to.
        /// </summary>
        /// 
        public IPOINT MovePoint
        {
            get { return movePoint; }
            set { movePoint = value; }
        }

        // Private constructor to do common initialization
        private CanvasMove( )
        {
            formatTranslations[PixelFormat_8bppIndexed]    = PixelFormat_8bppIndexed;
            formatTranslations[PixelFormat_16bppGrayScale] = PixelFormat_16bppGrayScale;
            formatTranslations[PixelFormat_24bppRgb]       = PixelFormat_24bppRgb;
            formatTranslations[PixelFormat_32bppArgb]      = PixelFormat_32bppArgb;
            formatTranslations[PixelFormat_32bppRgb]       = PixelFormat_32bppRgb;
            formatTranslations[PixelFormat_48bppRgb]       = PixelFormat_48bppRgb;
            formatTranslations[PixelFormat_64bppArgb]      = PixelFormat_64bppArgb;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="CanvasMove"/> class.
        /// </summary>
        /// 
        /// <param name="movePoint">Point to move the canvas to.</param>
        /// 
        public CanvasMove( IPOINT movePoint )
            : this( )
        {
            this.movePoint = movePoint;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="CanvasMove"/> class.
        /// </summary>
        /// 
        /// <param name="movePoint">Point to move the canvas.</param>
        /// <param name="fillColorRGB">RGB color to use for filling areas empty areas in color images.</param>
        /// 
        public CanvasMove( IPOINT movePoint, Color fillColorRGB )
            : this( )
        {
            this.movePoint = movePoint;
            this.fillRed   = fillColorRGB.R;
            this.fillGreen = fillColorRGB.G;
            this.fillBlue  = fillColorRGB.B;
            this.fillAlpha = fillColorRGB.A;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="CanvasMove"/> class.
        /// </summary>
        /// 
        /// <param name="movePoint">Point to move the canvas.</param>
        /// <param name="fillColorGray">Gray color to use for filling empty areas in grayscale images.</param>
        /// 
        public CanvasMove( IPOINT movePoint, byte fillColorGray )
            : this( )
        {
            this.movePoint = movePoint;
            this.fillGray  = fillColorGray;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="CanvasMove"/> class.
        /// </summary>
        /// 
        /// <param name="movePoint">Point to move the canvas.</param>
        /// <param name="fillColorRGB">RGB color to use for filling areas empty areas in color images.</param>
        /// <param name="fillColorGray">Gray color to use for filling empty areas in grayscale images.</param>
        /// 
        public CanvasMove( IPOINT movePoint, Color fillColorRGB, byte fillColorGray )
            : this( )
        {
            this.movePoint = movePoint;
            this.fillRed   = fillColorRGB.R;
            this.fillGreen = fillColorRGB.G;
            this.fillBlue  = fillColorRGB.B;
            this.fillAlpha = fillColorRGB.A;
            this.fillGray  = fillColorGray;
        }

        /// <summary>
        /// Process the filter on the specified image.
        /// </summary>
        /// 
        /// <param name="image">Source image data.</param>
        ///
        protected override void ProcessFilter( UnmanagedImage image )
        {
            int pixelSize = Image_GetPixelFormatSize( image->t ) / 8;

            switch ( pixelSize )
            {
                case 1:
                case 3:
                case 4:
                    ProcessFilter8bpc( image );
                    break;
                case 2:
                case 6:
                case 8:
                    ProcessFilter16bpc( image );
                    break;
            }
        }

        // Process the filter on the image with 8 bits per color channel
        private  void ProcessFilter8bpc( UnmanagedImage image )
        {
            int pixelSize = Image_GetPixelFormatSize( image->t ) / 8;
            bool is32bpp = ( pixelSize == 4 );

            // get image width and height
            int width  = image.Width;
            int height = image.Height;
            int stride = image->s;

            int movePointX = movePoint.X;
            int movePointY = movePoint.Y;

            // intersection rectangle
            Rectangle intersect = Rectangle.Intersect(
                new Rectangle( 0, 0, width, height ),
                new Rectangle( movePointX, movePointY, width, height ) );

            // start, stop and step for X adn Y
            int yStart  = 0;
            int yStop   = height;
            int yStep   = 1;
            int xStart  = 0;
            int xStop   = width;
            int xStep   = 1;

            if ( movePointY > 0 )
            {
                yStart = height - 1;
                yStop  = -1;
                yStep  = -1;
            }
            if ( movePointX > 0 )
            {
                xStart = width - 1;
                xStop  = -1;
                xStep  = -1;
            }

            // do the job
            byte* src = (byte*) image->tt.data;
            byte* pixel, moved;

            if ( image->t == PixelFormat_8bppIndexed )
            {
                // grayscale image
                for (y = yStart; y != yStop; y += yStep )
                {
                    for (x = xStart; x != xStop; x += xStep )
                    {
                        // current pixel
                        pixel = src + y * stride + x;

                        if ( intersect.Contains( x, y ) )
                        {
                            moved = src + ( y - movePointY ) * stride + ( x - movePointX );

                            *pixel = *moved;
                        }
                        else
                        {
                            *pixel = fillGray;
                        }
                    }
                }
            }
            else
            {
                // color image
                for (y = yStart; y != yStop; y += yStep )
                {
                    for (x = xStart; x != xStop; x += xStep )
                    {
                        // current pixel
                        pixel = src + y * stride + x * pixelSize;

                        if ( intersect.Contains( x, y ) )
                        {
                            moved = src + ( y - movePointY ) * stride + ( x - movePointX ) * pixelSize;

                            pixel[RGB_R] = moved[RGB_R];
                            pixel[RGB_G] = moved[RGB_G];
                            pixel[RGB_B] = moved[RGB_B];

                            if ( is32bpp )
                            {
                                pixel[RGB_A] = moved[RGB_A];
                            }
                        }
                        else
                        {
                            pixel[RGB_R] = fillRed;
                            pixel[RGB_G] = fillGreen;
                            pixel[RGB_B] = fillBlue;

                            if ( is32bpp )
                            {
                                pixel[RGB_A] = fillAlpha;
                            }
                        }
                    }
                }
            }
        }

        // Process the filter on the image with 16 bits per color channel
        private  void ProcessFilter16bpc( UnmanagedImage image )
        {
            int pixelSize = Image_GetPixelFormatSize( image->t ) / 8;
            bool is64bpp = ( pixelSize == 8 );

            // pad fill colours to 16-bits
            ushort fillRed   = (ushort) ( this.fillRed   << 8 );
            ushort fillGreen = (ushort) ( this.fillGreen << 8 );
            ushort fillBlue  = (ushort) ( this.fillBlue  << 8 );
            ushort fillAlpha = (ushort) ( this.fillAlpha << 8 );

            // get image width and height
            int width  = image.Width;
            int height = image.Height;
            int stride = image->s;

            int movePointX = movePoint.X;
            int movePointY = movePoint.Y;

            // intersection rectangle
            Rectangle intersect = Rectangle.Intersect(
                new Rectangle( 0, 0, width, height ),
                new Rectangle( movePointX, movePointY, width, height ) );

            // start, stop and step for X and Y
            int yStart = 0;
            int yStop = height;
            int yStep = 1;
            int xStart = 0;
            int xStop = width;
            int xStep = 1;

            if ( movePointY > 0 )
            {
                yStart = height - 1;
                yStop = -1;
                yStep = -1;
            }
            if ( movePointX > 0 )
            {
                xStart = width - 1;
                xStop = -1;
                xStep = -1;
            }

            // do the job
            byte* src = (byte*) image->tt.data;
            ushort* pixel, moved;

            if ( image->t == PixelFormat_16bppGrayScale )
            {
                // grayscale image
                for (y = yStart; y != yStop; y += yStep )
                {
                    for (x = xStart; x != xStop; x += xStep )
                    {
                        // current pixel
                        pixel = (ushort*) ( src + y * stride + x * 2 );

                        if ( intersect.Contains( x, y ) )
                        {
                            moved = (ushort*) ( src + ( y - movePointY ) * stride + ( x - movePointX ) * 2 );
                            *pixel = *moved;
                        }
                        else
                        {
                            *pixel = fillGray;
                        }
                    }
                }
            }
            else
            {
                // color image
                for (y = yStart; y != yStop; y += yStep )
                {
                    for (x = xStart; x != xStop; x += xStep )
                    {
                        // current pixel
                        pixel = (ushort*) ( src + y * stride + x * pixelSize );

                        if ( intersect.Contains( x, y ) )
                        {
                            moved = (ushort*) ( src + ( y - movePointY ) * stride + ( x - movePointX ) * pixelSize );

                            pixel[RGB_R] = moved[RGB_R];
                            pixel[RGB_G] = moved[RGB_G];
                            pixel[RGB_B] = moved[RGB_B];

                            if ( is64bpp )
                            {
                                pixel[RGB_A] = moved[RGB_A];
                            }
                        }
                        else
                        {
                            pixel[RGB_R] = fillRed;
                            pixel[RGB_G] = fillGreen;
                            pixel[RGB_B] = fillBlue;

                            if ( is64bpp )
                            {
                                pixel[RGB_A] = fillAlpha;
                            }
                        }
                    }
                }
            }
        }
    }
}