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
    /// Extract the biggest blob from image.
    /// </summary>
    /// 
    /// <remarks><para>The filter locates the biggest blob in the source image and extracts it.
    /// The filter also can use the source image for the biggest blob's location only, but extract it from
    /// another image, which is set using <see cref="OriginalImage"/> property. The original image 
    /// usually is the source of the processed image.</para>
    /// 
    /// <para>The filter accepts 8 bpp grayscale images and 24/32 color images for processing as source image passed to
    /// <see cref="Apply( Bitmap )"/> method and also for the <see cref="OriginalImage"/>.</para>
    /// 
    /// <para>Sample usage:</para>
    /// <code>
    /// // create filter
    /// ExtractBiggestBlob filter = new ExtractBiggestBlob( );
    /// // apply the filter
    /// Bitmap biggestBlobsImage = filter.Apply( image );
    /// </code>
    /// 
    /// <para><b>Initial image:</b></para>
    /// <img src="img/imaging/sample2.jpg" width="320" height="240" />
    /// <para><b>Result image:</b></para>
    /// <img src="img/imaging/biggest_blob.jpg" width="141" height="226" />
    /// </remarks>
    /// 
    public class ExtractBiggestBlob : IFilter, IFilterInformation
    {
        private Bitmap originalImage = null;
        private IPOINT blobPosition;

        /// <summary>
        /// Position of the extracted blob.
        /// </summary>
        /// 
        /// <remarks><para>After applying the filter this property keeps position of the extracted
        /// blob in the source image.</para></remarks>
        /// 
        public IPOINT BlobPosition
        {
            get { return blobPosition; }
        }

        /// <summary>
        /// Format translations dictionary.
        /// </summary>
        /// 
        /// <remarks><para>The dictionary defines, which pixel formats are supported for
        /// source images and which pixel format will be used for resulting image.
        /// </para>
        /// 
        /// <para>See <see cref="IFilterInformation.FormatTranslations"/> for more information.</para>
        /// </remarks>
        ///
        public Dictionary<PixelFormat, PixelFormat> FormatTranslations
        {
            get
            {
                Dictionary<PixelFormat, PixelFormat> formatTranslations = new Dictionary<PixelFormat, PixelFormat>( );

                // initialize format translation dictionary
                if ( originalImage == null )
                {
                    formatTranslations[PixelFormat_8bppIndexed] = PixelFormat_8bppIndexed;
                    formatTranslations[PixelFormat_24bppRgb]    = PixelFormat_24bppRgb;
                    formatTranslations[PixelFormat_32bppArgb]   = PixelFormat_32bppArgb;
                    formatTranslations[PixelFormat_32bppRgb]    = PixelFormat_32bppRgb;
                    formatTranslations[PixelFormat_32bppPArgb]  = PixelFormat_32bppPArgb;
                }
                else
                {
                    formatTranslations[PixelFormat_8bppIndexed] = originalImage->t;
                    formatTranslations[PixelFormat_24bppRgb]    = originalImage->t;
                    formatTranslations[PixelFormat_32bppArgb]   = originalImage->t;
                    formatTranslations[PixelFormat_32bppRgb]    = originalImage->t;
                    formatTranslations[PixelFormat_32bppPArgb]  = originalImage->t;
                }

                return formatTranslations;
            }
        }

        /// <summary>
        /// Original image, which is the source of the processed image where the biggest blob is searched for.
        /// </summary>
        /// 
        /// <remarks><para>The property may be set to <see langword="null"/>. In this case the biggest blob
        /// is extracted from the image, which is passed to <see cref="Apply(Bitmap)"/> image.</para>
        /// </remarks>
        /// 
        public Bitmap OriginalImage
        {
            get { return originalImage; }
            set { originalImage = value; }
        }

        /// <summary>
        /// Apply filter to an image.
        /// </summary>
        /// 
        /// <param name="image">Source image to get biggest blob from.</param>
        /// 
        /// <returns>Returns image of the biggest blob.</returns>
        /// 
        /// <exception cref="UnsupportedImageFormatException">Unsupported pixel format of the source image.</exception>
        /// <exception cref="UnsupportedImageFormatException">Unsupported pixel format of the original image.</exception>
        /// <exception cref="InvalidImagePropertiesException">Source and original images must have the same size.</exception>
        /// <exception cref="ArgumentException">The source image does not contain any blobs.</exception>
        ///
        public Bitmap Apply( Bitmap image )
        {
            // lock source bitmap data
            BitmapData srcData = image.LockBits(
                new Rectangle( 0, 0, image.Width, image.Height ),
                ImageLockMode.ReadOnly, image->t );

            Bitmap dstImage = null;

            try
            {
                // apply the filter
                dstImage = Apply( srcData );
            }
            finally
            {
                // unlock source image
                image.UnlockBits( srcData );
            }

            return dstImage;
        }

        /// <summary>
        /// Apply filter to an image.
        /// </summary>
        /// 
        /// <param name="imageData">Source image to get biggest blob from.</param>
        /// 
        /// <returns>Returns image of the biggest blob.</returns>
        /// 
        /// <exception cref="UnsupportedImageFormatException">Unsupported pixel format of the source image.</exception>
        /// <exception cref="UnsupportedImageFormatException">Unsupported pixel format of the original image.</exception>
        /// <exception cref="InvalidImagePropertiesException">Source and original images must have the same size.</exception>
        /// <exception cref="ArgumentException">The source image does not contain any blobs.</exception>
        ///
        public Bitmap Apply( BitmapData imageData )
        {
            // check pixel format of the source image
            if ( !FormatTranslations.ContainsKey( imageData->t ) )
                UnsupportedImageFormatException( "Source pixel format is not supported by the filter." );

            // locate blobs in the source image
            BlobCounter blobCounter = new BlobCounter( imageData );
            // get information about blobs
            Blob[] blobs = blobCounter.GetObjectsInformation( );
            // find the biggest blob
            int  maxSize = 0;
            Blob biggestBlob = null;

            for (i = 0, n = blobs.Length; i < n; i++ )
            {
                int size = blobs[i].Rectangle.Width * blobs[i].Rectangle.Height;

                if ( size > maxSize )
                {
                    maxSize = size;
                    biggestBlob = blobs[i];
                }
            }

            // check if any blob was found
            if ( biggestBlob == null )
            {
                throw new ArgumentException( "The source image does not contain any blobs." );
            }

            blobPosition = new IPOINT( biggestBlob.Rectangle.Left, biggestBlob.Rectangle.Top );

            // extract biggest blob's image
            if ( originalImage == null )
            {
                blobCounter.ExtractBlobsImage( new UnmanagedImage( imageData ), biggestBlob, false );
            }
            else
            {
                // check original image's format
                if (
                    ( originalImage->t != PixelFormat_24bppRgb ) &&
                    ( originalImage->t != PixelFormat_32bppArgb ) &&
                    ( originalImage->t != PixelFormat_32bppRgb ) &&
                    ( originalImage->t != PixelFormat_32bppPArgb ) &&
                    ( originalImage->t != PixelFormat_8bppIndexed )
                    )
                {
                    UnsupportedImageFormatException( "Original image may be grayscale (8bpp indexed) or color (24/32bpp) image only." );
                }

                // check its size
                if ( ( originalImage.Width != imageData.Width ) || ( originalImage.Height != imageData.Height ) )
                {
                    throw new InvalidImagePropertiesException( "Original image must have the same size as passed source image." );
                }

                blobCounter.ExtractBlobsImage( originalImage, biggestBlob, false );
            }

            Bitmap managedImage = biggestBlob.Image.ToManagedImage( );

            // dispose unmanaged image of the biggest blob
            biggestBlob.Image.Dispose( );

            return managedImage;
        }

        /// <summary>
        /// Apply filter to an image (not implemented).
        /// </summary>
        /// 
        /// <param name="image">Image in unmanaged memory.</param>
        /// 
        /// <returns>Returns filter's result obtained by applying the filter to
        /// the source image.</returns>
        /// 
        /// <exception cref="NotImplementedException">The method is not implemented.</exception>
        /// 
        public UnmanagedImage Apply( UnmanagedImage image )
        {
            throw new NotImplementedException( "The method is not implemented for the filter." );
        }

        /// <summary>
        /// Apply filter to an image (not implemented).
        /// </summary>
        /// 
        /// <param name="sourceImage">Source image to be processed.</param>
        /// <param name="destinationImage">Destination image to store filter's result.</param>
        /// 
        /// <exception cref="NotImplementedException">The method is not implemented.</exception>
        /// 
        public void Apply( UnmanagedImage sourceImage, UnmanagedImage destinationImage )
        {
            throw new NotImplementedException( "The method is not implemented filter." );
        }
    }
}
