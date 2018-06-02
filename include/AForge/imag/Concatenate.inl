
/// <summary>
///   Concatenation filter.
/// </summary>
/// <remarks>
///   Concatenates two images side by side in a single image.
/// </remarks>
/// 

typedef enum {
#define PIXELFORMATDEF(a, b)  a,
#include "PixelFormat.inl"
#undef PIXELFORMATDEF
} PixelFormat;


static int Image_GetPixelFormatSize(int pixelFormat) {
  int pixelFormatTable[]={
#define PIXELFORMATDEF(a, b)  b,
#include "PixelFormat.inl"
#undef PIXELFORMATDEF
  };
  ASSERT( (pixelFormat>=0 && pixelFormat<countof(pixelFormatTable)) && "Can not create image with specified pixel format." );
  return pixelFormatTable[pixelFormat];
}

/// <summary>
/// Format translations dictionary.
/// </summary>


/// <summary>
///   Creates a new concatenation filter.
/// </summary>
/// <param name="overlayImage">The first image to concatenate.</param>



/// <summary>
/// Process the filter on the specified image.
/// </summary>
/// 
/// <param name="im2">Source image data.</param>
/// <param name="im3">Destination image data.</param>
///
void Concatenate(const img_t* im1, const img_t* im2, img_t* im3)
{
    int pixelSize = im2->c;
    int copySize1 = im1->w * pixelSize;
    int copySize2 = im2->w * pixelSize;

    // do the job
    
    /// <summary>
    ///   Calculates new image size.
    /// </summary>
    int initeddst = imsetsize(im3, MAX(im1->h, im2->h), im1->w + im2->w, im2->c, 1);

    const byte* src1 = im1->tt.data;
    const byte* src2 = im2->tt.data;
    byte* dst = im3->tt.data;
    int y;
    // for each line
    for (y = 0; y < im3->h; y++)
    {
        if (y < im1->h)
            memcpy(dst, src1, copySize1);
        if (y < im2->h)
            memcpy(dst + copySize1, src2, copySize2);
        
        src1 += im1->s;
        src2 += im2->s;
        dst += im3->s;
    }
    
    // Release
}


