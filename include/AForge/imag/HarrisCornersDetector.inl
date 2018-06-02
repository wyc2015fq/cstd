

enum {RGB_R=0, RGB_G=1, RGB_B=2, RGB_A=3};

#include "Imaging/Image.inl"
#include "Imaging/Filters/Color_Filters/Grayscale.inl"
#include "Imaging/Filters/Convolution/GaussianBlur.inl"
#include "Imaging/Filters/Convolution/Convolution.inl"

/// <summary>
///   Harris Corners Detector.
/// </summary>
/// <remarks>
/// <para>This class implements the Harris corners detector.</para>
/// <para>Sample usage:</para>
/// <code>
/// // create corners detector's instance
/// HarrisCornersDetector hcd = new HarrisCornersDetector( );
/// // process image searching for corners
/// Point[] corners = hcd.ProcessImage( image );
/// // process points
/// foreach ( Point corner in corners )
/// {
///     // ... 
/// }
/// </code>
/// 
/// <para>
///   References:
///   <list type="bullet">
///     <item><description>
///       P. D. Kovesi. MATLAB and Octave Functions for Computer Vision and Image Processing.
///       School of Computer Science and Software Engineering, The University of Western Australia.
///       Available in: http://www.csse.uwa.edu.au/~pk/Research/MatlabFns/Spatial/harris.m</description></item>
///     <item><description>
///       C.G. Harris and M.J. Stephens. "A combined corner and edge detector", 
///       Proceedings Fourth Alvey Vision Conference, Manchester.
///       pp 147-151, 1988.</description></item>
///     <item><description>
///       Alison Noble, "Descriptions of Image Surfaces", PhD thesis, Department
///       of Engineering Science, Oxford University 1989, p45.</description></item>
///   </list>
/// </para>
/// </remarks>
/// 
/// <seealso cref="MoravecCornersDetector"/>
/// <seealso cref="SusanCornersDetector"/>
///
int HarrisCornersDetector(const img_t* image, double k, double threshold, IPOINT* cornersList, int cornersListSize)
{
    //double k = 0.04f;
    //double threshold = 1000.f;
    int srcPixelFormat = image->t;
    double sigma = 1.4;
    int r = 3;
    int cornersListLen=0;
    int width, height, stride, offset, hl=0, x, y;
    double* H=0;
    img_t grayImage[1] = {0}, diffx[1] = {0}, diffy[1] = {0}, diffxy[1] = {0};


    /// <summary>
    ///   Harris parameter k. Default value is 0.04.
    ///   Harris threshold. Default value is 1000.
    ///   Gaussian smoothing sigma. Default value is 1.4.
    ///   Non-maximum suppression window radius. Default value is 3.
    /// </summary>

    /// <summary>
    /// Process image looking for corners.
    /// </summary>
    /// 
    /// <param name="image">Source image data to process.</param>
    /// 
    /// <returns>Returns list of found corners (X-Y coordinates).</returns>
    /// 
    /// <exception cref="UnsupportedImageFormatException">
    ///   The source image has incorrect pixel format.
    /// </exception>
    /// 
    // check image format

    // make sure we have grayscale image
    
    // check image format
    if (
      (srcPixelFormat != PixelFormat_8bppIndexed) &&
      (srcPixelFormat != PixelFormat_24bppRgb) &&
      (srcPixelFormat != PixelFormat_32bppRgb) &&
      (srcPixelFormat != PixelFormat_32bppArgb)
      )
    {
      ASSERT(0 && "Unsupported pixel format of the source");
    }

    if (srcPixelFormat == PixelFormat_8bppIndexed)
    {
        imclone2(image, grayImage);
    }
    else
    {
        // create temporary grayscale image
        Grayscale(image, grayImage, GRY_BT709);
    }

    // get source image size

    width = grayImage->w;
    height = grayImage->h;
    stride = grayImage->s;
    offset = stride - width;


    // 1. Calculate partial differences
    
    Image_SetSize(diffx, height, width, PixelFormat_8bppIndexed);
    Image_SetSize(diffy, height, width, PixelFormat_8bppIndexed);
    Image_SetSize(diffxy, height, width, PixelFormat_8bppIndexed);
    
    {
        // Compute dx and dy
        byte* src = grayImage->tt.data;
        byte* dx = diffx->tt.data;
        byte* dy = diffy->tt.data;
        byte* dxy = diffxy->tt.data;
        int h, v;
        // for each line
        for (y = 0; y < height; y++)
        {
            // for each pixel
            for (x = 0; x < width; x++, src++, dx++, dy++)
            {
                // TODO: Place those verifications
                // outside the innermost loop
                if (x == 0 || x == width  - 1 ||
                    y == 0 || y == height - 1)
                {
                    *dx = *dy = 0; continue;
                }
                                            
                h = -(src[-stride - 1] + src[-1] + src[stride - 1]) +
                         (src[-stride + 1] + src[+1] + src[stride + 1]);
                *dx = (byte)(h > 255 ? 255 : h < 0 ? 0 : h);

                v = -(src[-stride - 1] + src[-stride] + src[-stride + 1]) +
                         (src[+stride - 1] + src[+stride] + src[+stride + 1]);
                *dy = (byte)(v > 255 ? 255 : v < 0 ? 0 : v);
            }
            src += offset;
            dx += offset;
            dy += offset;
        }
        
        // Compute dxy
        dx = (byte*)diffx->tt.data;
        dxy = (byte*)diffxy->tt.data;

        // for each line
        for (y = 0; y < height; y++)
        {
            // for each pixel
            for (x = 0; x < width; x++, dx++, dxy++)
            {
                if (x == 0 || x == width  - 1 ||
                    y == 0 || y == height - 1)
                {
                    *dxy = 0; continue;
                }

                v = -(dx[-stride - 1] + dx[-stride] + dx[-stride + 1]) +
                         (dx[+stride - 1] + dx[+stride] + dx[+stride + 1]);
                *dxy = (byte)(v > 255 ? 255 : v < 0 ? 0 : v);
            }
            dx += offset;
            dxy += offset;
        }
    }
    

    // 2. Smooth the diff images
    if (sigma > 0.0)
    {
        int intKernel[21*21]={0};
        int divisor = GaussianBlur(sigma, 5, intKernel);
        IRECT rect = iRECT(0, 0, width, height);
        ConvolutionInPlace(diffx, rect, intKernel, 5, iDIM(5, 5));
        ConvolutionInPlace(diffy, rect, intKernel, 5, iDIM(5, 5));
        ConvolutionInPlace(diffxy, rect, intKernel, 5, iDIM(5, 5));
    }

    // 3. Compute Harris Corner Response
    H = MALLOC(double, height*width);
    hl = width;
    
    {
        byte* ptrA = diffx->tt.data;
        byte* ptrB = diffy->tt.data;
        byte* ptrC = diffxy->tt.data;
        double M, A, B, C;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                A = *(ptrA++);
                B = *(ptrB++);
                C = *(ptrC++);

                // Harris corner measure
                M = (A * B - C * C) - (k * ((A + B) * (A + B)));

                if (M > threshold)
                    H[y*hl+x] = M;
                else H[y*hl+x] = 0;
            }

            ptrA += offset;
            ptrB += offset;
            ptrC += offset;
        }
    }

    // Free resources
    imfree(diffx);
    imfree(diffy);
    imfree(diffxy);

    if (srcPixelFormat != PixelFormat_8bppIndexed) {
        imfree(grayImage);
    }

    // 4. Suppress non-maximum points
    //IPOINT* cornersList = new List<IPOINT>();

    {
        int i, j, maxY, maxX;
        // for each row
        for (y = r, maxY = height - r; y < maxY && cornersListLen<cornersListSize; y++)
        {
            // for each pixel
            for (x = r, maxX = width - r; x < maxX && cornersListLen<cornersListSize; x++)
            {
                double currentValue = H[y*hl+x];

                // for each windows' row
                for (i = -r; (currentValue != 0) && (i <= r); i++)
                {
                    // for each windows' pixel
                    for (j = -r; j <= r; j++)
                    {
                        if (H[y + i, x + j] > currentValue)
                        {
                            currentValue = 0;
                            break;
                        }
                    }
                }

                // check if this point is really interesting
                if (currentValue != 0)
                {
                    cornersList[cornersListLen++] = iPOINT(x, y);
                }
            }
        }
    }
    
    FREE(H);
    imfree(diffxy);
    imfree(diffy);
    imfree(diffx);
    imfree(grayImage);

    return cornersListLen;
}

