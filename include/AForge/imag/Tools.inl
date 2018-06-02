
/// <summary>
///   Static tool functions for imaging.
/// </summary>
///

#define SQRT2  1.4142135623730951

/// <summary>
///   Normalizes a set of homogeneous points so that the origin is located
///   at the centroid and the mean distance to the origin is sqrt(2).
/// </summary>
static int NormalizeH(int n, const PointH* points, double* T, PointH* pts)
{
    int i;
    double x, y, w;
    double xmean = 0, ymean = 0, scale;
    for (i = 0; i < n; i++)
    {
        x = points[i].x, y = points[i].y, w = points[i].w;
        pts[i].x = x / w, pts[i].y = y / w, pts[i].w = 1;
        xmean += pts[i].x, ymean += pts[i].y;
    }
    xmean /= n; ymean /= n;

    scale = 0;
    for (i = 0; i < n; i++)
    {
        x = points[i].x - xmean;
        y = points[i].y - ymean;

        scale += sqrt(x * x + y * y);
    }

    scale = (SQRT2 * n / scale);

    MatrixH_set9(T, scale, 0, -scale * xmean, 0, scale, -scale * ymean, 0, 0, 1 );
    MatrixH_TransformPointHs(T, pts, pts, n);
    return 0;
}

/// <summary>
///   Normalizes a set of homogeneous points so that the origin is located
///   at the centroid and the mean distance to the origin is sqrt(2).
/// </summary>
static int Normalize(int n, const DPOINT* points, double* T, DPOINT* pts)
{
    double x, y, xmean = 0, ymean = 0, scale;
    int i;
    for (i = 0; i < n; i++)
    {
        pts[i].x = points[i].x, pts[i].y = points[i].y;
        xmean += points[i].x, ymean += points[i].y;
    }
    xmean /= n; ymean /= n;
    scale = 0;
    for (i = 0; i < n; i++)
    {
        x = points[i].x - xmean;
        y = points[i].y - ymean;

        scale += sqrt(x * x + y * y);
    }

    scale = (SQRT2 * n / scale);
    MatrixH_set9(T, scale, 0, -scale * xmean, 0, scale, -scale * ymean, 0, 0, 1);
    return MatrixH_TransformPoints(T, pts, pts, n);
}

// ¹²Ïß
/// <summary>
///   Detects if three points are colinear.
/// </summary>
static bool Colinear(DPOINT pt1, DPOINT pt2, DPOINT pt3)
{
    return fabs( (pt1.y - pt2.y) * pt3.x + (pt2.x - pt1.x) * pt3.y +
         (pt1.x * pt2.y - pt1.y * pt2.x)) < SingleEpsilon;
}

/// <summary>
///   Detects if three points are colinear.
/// </summary>
static bool ColinearH(PointH pt1, PointH pt2, PointH pt3)
{
    return fabs(
     (pt1.y * pt2.w - pt1.w * pt2.y) * pt3.x +
     (pt1.w * pt2.x - pt1.x * pt2.w) * pt3.y +
     (pt1.x * pt2.y - pt1.y * pt2.x) * pt3.w) < SingleEpsilon;
}


//#region Algebra and geometry tools
/// <summary>
///   Creates an homography matrix matching points
///   from a set of points to another.
//    The number of points should be equal.
/// </summary>
static int HomographyH(int npt1, PointH* points1, PointH* points2, double* T)
{
    double T1[8], T2[8]; // Normalize input points
    int i, N;
    double* A = 0;
    // Initial argument checkings

    if (npt1 < 4) {
        ArgumentException("At least four points are required to fit an homography");
    }
    MatrixH_init(T1);
    MatrixH_init(T2);

    N = npt1;

    NormalizeH(npt1, points1, T1, points1);
    NormalizeH(npt1, points2, T2, points2);

    // Create the matrix A
    A = MALLOC(double, 3 * N * 9);
    for (i = 0; i < N; i++)
    {
        PointH p1 = points1[i];
        PointH p2 = points2[i];
        double* r;

        r = A + (i*3+0)*9;
        r[0] = 0;
        r[1] = 0;
        r[2] = 0;
        r[3] = -p2.w * p1.x;
        r[4] = -p2.w * p1.y;
        r[5] = -p2.w * p1.w;
        r[6] = p2.y * p1.x;
        r[7] = p2.y * p1.y;
        r[8] = p2.y * p1.w;
        
        r = A + (i*3+1)*9;
        r[0] = p2.w * p1.x;
        r[1] = p2.w * p1.y;
        r[2] = p2.w * p1.w;
        r[3] = 0;
        r[4] = 0;
        r[5] = 0;
        r[6] = -p2.x * p1.x;
        r[7] = -p2.x * p1.y;
        r[8] = -p2.x * p1.w;
        
        r = A + (i*3+2)*9;
        r[0] = -p2.y * p1.x;
        r[1] = -p2.y * p1.y;
        r[2] = -p2.y * p1.w;
        r[3] = p2.x * p1.x;
        r[4] = p2.x * p1.y;
        r[5] = p2.x * p1.w;
        r[6] = 0;
        r[7] = 0;
        r[8] = 0;
    }

    // Create the singular value decomposition
    {
      int m = 3*N;
      double V[9*9]={0};
      double W[9]={0};
      double* U = MALLOC(double, m*m);
      double H[8]={0};
      double invT2[9];
      //svd_init(3*N, 9, A, 9, , false, true);
      cvSVD_64f(A, 9, W, 1, U, m, V, 9, 0, m, 9);
      //double V = svd.RightSingularVectors;
      
      // Extract the homography matrix
      MatrixH_set9(H, V[0*9+8], V[1*9+8], V[2*9+8], V[3*9+8], V[4*9+8], V[5*9+8], V[6*9+8], V[7*9+8], V[8*9+8]);
      
      // Denormalize
      // H = T2.Inverse().Multiply(H.Multiply(T1));
      MatrixH_Multiply(H, T1, H);
      MatrixH_Inverse(T2, invT2);
      MatrixH_Multiply(invT2, H, H);
      FREE(U);
    }
    FREE(A);
    return 0;
}

/// <summary>
///   Creates an homography matrix matching points
///   from a set of points to another.
/// </summary>
static int Homography(int npt1, DPOINT* points1, int npt2, DPOINT* points2, double* T)
{
    double T1[8], T2[8]; // Normalize input points
    int i, N;
    double* A = 0;

    // Initial argument checkings
    if (npt1 != npt2) {
        ArgumentException("The number of points should be equal.");
    }

    if (npt1 < 4)
        ArgumentException("At least four points are required to fit an homography");


    N = npt1;
    MatrixH_init(T1);
    MatrixH_init(T2);

    // Normalize input points
    Normalize(npt1, points1, T1, points1);
    Normalize(npt1, points2, T2, points2);

    // Create the matrix A
    A = MALLOC(double, 3 * N * 9);
    for (i = 0; i < N; i++)
    {
        DPOINT pt = points1[i];
        double x = points2[i].x;
        double y = points2[i].y;
        double* r;
        
        r = A + (i*3+0)*9;
        r[0] = 0;
        r[1] = 0;
        r[2] = 0;
        r[3] = -pt.x;
        r[4] = -pt.y;
        r[5] = -1;
        r[6] = y * pt.x;
        r[7] = y * pt.y;
        r[8] = y;
        
        r = A + (i*3+1)*9;
        r[0] = pt.x;
        r[1] = pt.y;
        r[2] = 1;
        r[3] = 0;
        r[4] = 0;
        r[5] = 0;
        r[6] = -x * pt.x;
        r[7] = -x * pt.y;
        r[8] = -x;
        
        r = A + (i*3+2)*9;
        r[0] = -y * pt.x;
        r[1] = -y * pt.y;
        r[2] = -y;
        r[3] = x * pt.x;
        r[4] = x * pt.y;
        r[5] = x;
        r[6] = 0;
        r[7] = 0;
        r[8] = 0;
    }


#if 0
    // Create the singular value decomposition
    svd = new SingularValueDecomposition(A, false, true);
    double[,] V = svd.RightSingularVectors;


    // Extract the homography matrix
    MatrixH H = new MatrixH((double)V[0, 8], (double)V[1, 8], (double)V[2, 8],
                            (double)V[3, 8], (double)V[4, 8], (double)V[5, 8],
                            (double)V[6, 8], (double)V[7, 8], (double)V[8, 8]);

    // Denormalize
    H = T2.Inverse().Multiply(H.Multiply(T1));
#endif
    
    {
      int m = 3*N;
      double V[9*9]={0};
      double W[9]={0};
      double* U = MALLOC(double, m*m);
      double H[8]={0};
      double invT2[9];
      //svd_init(3*N, 9, A, 9, , false, true);
      cvSVD_64f(A, 9, W, 1, U, m, V, 9, 0, m, 9);
      //double V = svd.RightSingularVectors;
      
      // Extract the homography matrix
      MatrixH_set9(H, V[0*9+8], V[1*9+8], V[2*9+8], V[3*9+8], V[4*9+8], V[5*9+8], V[6*9+8], V[7*9+8], V[8*9+8]);
      
      // Denormalize
      // H = T2.Inverse().Multiply(H.Multiply(T1));
      MatrixH_Multiply(H, T1, H);
      MatrixH_Inverse(T2, invT2);
      MatrixH_Multiply(invT2, H, H);
      FREE(U);
    }
    FREE(A);

    return 0;
}

#if 0
//#region Image tools
/// <summary>
///   Computes the sum of the pixels in a given image.
/// </summary>
static int imSum(BitmapData image)
{
    if (image->t != PixelFormat_8bppIndexed) {
        UnsupportedImageFormatException("Only grayscale images are supported");
    }

    int width = image.Width;
    int height = image.Height;
    int offset = image->s - image.Width;

    int sum = 0;

    byte* src = (byte*)image.Scan0.ToPointer();
    
    for (y = 0; y < height; y++)
    {
      for (x = 0; x < width; x++, src++)
        sum += (*src);
      src += offset;
    }
    

    return sum;
}
/// <summary>
///   Computes the sum of the pixels in a given image.
/// </summary>
static int Sum(this BitmapData image, Rectangle rectangle)
{
    if (image->t != PixelFormat_8bppIndexed) {
        UnsupportedImageFormatException("Only grayscale images are supported");
    }

    int width = image.Width;
    int height = image.Height;
    int stride = image->s;
    int offset = image->s - image.Width;

    int rwidth = rectangle.Width;
    int rheight = rectangle.Height;
    int rx = rectangle.x;
    int ry = rectangle.y;
    int sum = 0;
    byte* src = (byte*)image.Scan0.ToPointer();

    for (y = 0; y < rheight; y++)
    {
        byte* p = src + stride * (ry + y) + rx;

        for (x = 0; x < rwidth; x++)
            sum += (*p++);
    }

    return sum;
}

/// <summary>
///   Computes the sum of the pixels in a given image.
/// </summary>
static int Sum(this Bitmap image)
{
    BitmapData data = image.LockBits(new Rectangle(0, 0, image.Width, image.Height),
        ImageLockMode.ReadOnly, image->t);

    int sum = Sum(data);

    image.UnlockBits(data);

    return sum;
}

/// <summary>
///   Computes the sum of the pixels in a given image.
/// </summary>
static int Sum(this Bitmap image, Rectangle rectangle)
{
    BitmapData data = image.LockBits(rectangle,
        ImageLockMode.ReadOnly, image->t);

    int sum = Sum(data);

    image.UnlockBits(data);

    return sum;
}

/// <summary>
///   Converts a given image into a array of double-precision
///   floating-point numbers scaled between -1 and 1.
/// </summary>
static double[] ToDoubleArray(this Bitmap image, int channel)
{
    return ToDoubleArray(image, channel, -1, 1);
}

/// <summary>
///   Converts a given image into a array of double-precision
///   floating-point numbers scaled between the given range.
/// </summary>
static double[] ToDoubleArray(this Bitmap image, int channel, double min, double max)
{
    BitmapData data = image.LockBits(new Rectangle(0, 0, image.Width, image.Height),
        ImageLockMode.ReadOnly, image->t);

    double[] array = ToDoubleArray(data, channel, min, max);

    image.UnlockBits(data);

    return array;
}

/// <summary>
///   Converts a given image into a array of double-precision
///   floating-point numbers scaled between -1 and 1.
/// </summary>
static double[] ToDoubleArray(this BitmapData image, int channel)
{
    return ToDoubleArray(image, channel, -1, 1);
}

/// <summary>
///   Converts a given image into a array of double-precision
///   floating-point numbers scaled between the given range.
/// </summary>
static double[] ToDoubleArray(this BitmapData image, int channel, double min, double max)
{
    int width = image.Width;
    int height = image.Height;
    int offset = image->s - image.Width;

    double[] data = new double[width * height];
    int dst = 0;

    
    {
        byte* src = (byte*)image.Scan0.ToPointer() + channel;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++, src++, dst++)
            {
                data[dst] = Accord.Math.Tools.Scale(0, 255, min, max, *src);
            }
            src += offset;
        }
    }

    return data;
}

/// <summary>
///   Converts a given image into a array of double-precision
///   floating-point numbers scaled between -1 and 1.
/// </summary>
static double[][] ToDoubleArray(this Bitmap image)
{
    return ToDoubleArray(image, -1, 1);
}

/// <summary>
///   Converts a given image into a array of double-precision
///   floating-point numbers scaled between the given range.
/// </summary>
static double[][] ToDoubleArray(this Bitmap image, double min, double max)
{
    BitmapData data = image.LockBits(new Rectangle(0, 0, image.Width, image.Height),
        ImageLockMode.ReadOnly, image->t);

    double[][] array = ToDoubleArray(data, min, max);

    image.UnlockBits(data);

    return array;
}

/// <summary>
///   Converts a given image into a array of double-precision
///   floating-point numbers scaled between the given range.
/// </summary>
static double[][] ToDoubleArray(this BitmapData image, double min, double max)
{
    int width = image.Width;
    int height = image.Height;
    int pixelSize = System.Drawing.Image_GetPixelFormatSize(image->t) / 8;
    int offset = image->s - image.Width * pixelSize;

    double[][] data = new double[width * height][];
    int dst = 0;

    
    {
        byte* src = (byte*)image.Scan0.ToPointer();

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++, dst++)
            {
                double[] pixel = data[dst] = new double[pixelSize];
                for (i = pixel.Length-1; i >= 0; i--, src++)
                    pixel[i] = Accord.Math.Tools.Scale(0, 255, min, max, *src);
            }
            src += offset;
        }
    }

    return data;
}



#region Conversions

/// <summary>
///   Converts an image given as a array of pixel values into
///   a <see cref="System.Drawing.Bitmap"/>.
/// </summary>
/// <param name="pixels">An array containing the grayscale pixel
/// values as <see cref="System.Double">doubles</see>.</param>
/// <param name="width">The width of the resulting image.</param>
/// <param name="height">The height of the resulting image.</param>
/// <param name="min">The minimum value representing a color value of 0.</param>
/// <param name="max">The maximum value representing a color value of 255. </param>
/// <returns>A <see cref="System.Drawing.Bitmap"/> of given width and height
/// containing the given pixel values.</returns>
static Bitmap ToBitmap(this double[] pixels, int width, int height, double min, double max)
{
    Bitmap bitmap = AForge.Imaging.Image.CreateGrayscaleImage(width, height);

    BitmapData data = bitmap.LockBits(new Rectangle(0, 0, width, height),
        ImageLockMode.WriteOnly, bitmap->t);

    int offset = data->s - width;
    int src = 0;

    byte* dst = (byte*)data.Scan0.ToPointer();

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++, src++, dst++)
        {
            *dst = (byte)Accord.Math.Tools.Scale(min, max, 0, 255, pixels[src]);
        }
        dst += offset;
    }

    bitmap.UnlockBits(data);
    return bitmap;
}

/// <summary>
///   Converts an image given as a array of pixel values into
///   a <see cref="System.Drawing.Bitmap"/>.
/// </summary>
/// <param name="pixels">An jagged array containing the pixel values
/// as double arrays. Each element of the arrays will be converted to
/// a R, G, B, A value. The bits per pixel of the resulting image
/// will be set according to the size of these arrays.</param>
/// <param name="width">The width of the resulting image.</param>
/// <param name="height">The height of the resulting image.</param>
/// <param name="min">The minimum value representing a color value of 0.</param>
/// <param name="max">The maximum value representing a color value of 255. </param>
/// <returns>A <see cref="System.Drawing.Bitmap"/> of given width and height
/// containing the given pixel values.</returns>
static Bitmap ToBitmap(this double[][] pixels, int width, int height, double min, double max)
{
    PixelFormat format;
    int channels = pixels[0].Length;

    switch (channels)
    {
        case 1:
            format = PixelFormat_8bppIndexed;
            break;

        case 3:
            format = PixelFormat_24bppRgb;
            break;

        case 4:
            format = PixelFormat_32bppArgb;
            break;

        default:
            ArgumentException("pixels");
    }


    Bitmap bitmap = new Bitmap(width, height, format);

    BitmapData data = bitmap.LockBits(new Rectangle(0, 0, width, height),
        ImageLockMode.WriteOnly, format);

    int pixelSize = System.Drawing.Image_GetPixelFormatSize(format) / 8;
    int offset = data->s - width * pixelSize;
    int src = 0;

    
    {
        byte* dst = (byte*)data.Scan0.ToPointer();

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++, src++)
            {
                for (c = channels - 1; c >= 0; c--, dst++)
                {
                    *dst = (byte)Accord.Math.Tools.Scale(min, max, 0, 255, pixels[src][c]);
                }
            }
            dst += offset;
        }
    }

    bitmap.UnlockBits(data);

    return bitmap;
}

/// <summary>
///   Converts an image given as a array of pixel values into
///   a <see cref="System.Drawing.Bitmap"/>.
/// </summary>
/// <param name="pixels">An jagged array containing the pixel values
/// as double arrays. Each element of the arrays will be converted to
/// a R, G, B, A value. The bits per pixel of the resulting image
/// will be set according to the size of these arrays.</param>
/// <param name="width">The width of the resulting image.</param>
/// <param name="height">The height of the resulting image.</param>
/// <returns>A <see cref="System.Drawing.Bitmap"/> of given width and height
/// containing the given pixel values.</returns>
static Bitmap ToBitmap(this double[][] pixels, int width, int height)
{
    return ToBitmap(pixels, width, height, -1, 1);
}

#endif
