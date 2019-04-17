# FlyCapture2 fc2Image OpenCV IplImage Conversion 两种图像格式之间的转换 - Grandyang - 博客园







# [FlyCapture2 fc2Image OpenCV IplImage Conversion 两种图像格式之间的转换](https://www.cnblogs.com/grandyang/p/4612228.html)







fc2Image是FlyCapture SDK的C语言库中的图片格式，由于在Windows上的MinGW无法编译FlyCapture2的C++库，只能使用C语言库，所以当我们在同时使用OpenCV的图像格式IplImage时，有时候就需要两种格式相互转换。如果需要FlyCapture2 Image和OpenCV IplImage之间的转换，可以参见我之前的博客[OpenCV IplImage FlyCapture2 Image Conversion 两种图像类的相互转化](http://www.cnblogs.com/grandyang/p/4109742.html)。我们先来分别看看两种图像格式的定义：

IplImage:

```
typedef struct _IplImage
{
    int  nSize;             /* sizeof(IplImage) */
    int  ID;                /* version (=0)*/
    int  nChannels;         /* Most of OpenCV functions support 1,2,3 or 4 channels */
    int  alphaChannel;      /* Ignored by OpenCV */
    int  depth;             /* Pixel depth in bits: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16S,
                               IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F are supported.  */
    char colorModel[4];     /* Ignored by OpenCV */
    char channelSeq[4];     /* ditto */
    int  dataOrder;         /* 0 - interleaved color channels, 1 - separate color channels.
                               cvCreateImage can only create interleaved images */
    int  origin;            /* 0 - top-left origin,
                               1 - bottom-left origin (Windows bitmaps style).  */
    int  align;             /* Alignment of image rows (4 or 8).
                               OpenCV ignores it and uses widthStep instead.    */
    int  width;             /* Image width in pixels.                           */
    int  height;            /* Image height in pixels.                          */
    struct _IplROI *roi;    /* Image ROI. If NULL, the whole image is selected. */
    struct _IplImage *maskROI;      /* Must be NULL. */
    void  *imageId;                 /* "           " */
    struct _IplTileInfo *tileInfo;  /* "           " */
    int  imageSize;         /* Image data size in bytes
                               (==image->height*image->widthStep
                               in case of interleaved data)*/
    char *imageData;        /* Pointer to aligned image data.         */
    int  widthStep;         /* Size of aligned image row in bytes.    */
    int  BorderMode[4];     /* Ignored by OpenCV.                     */
    int  BorderConst[4];    /* Ditto.                                 */
    char *imageDataOrigin;  /* Pointer to very origin of image data
                               (not necessarily aligned) -
                               needed for correct deallocation */
}
IplImage;
```



fc2Image:

```
typedef struct _fc2Image
{
    unsigned int rows;
    unsigned int cols;
    unsigned int stride;
    unsigned char* pData;
    unsigned int dataSize;
    unsigned int receivedDataSize;
    fc2PixelFormat format;
    fc2BayerTileFormat bayerFormat;

    fc2ImageImpl imageImpl;

} fc2Image;
```



下面我们来进行相互转换：

1. FlyCapture2 fc2Image OpenCV IplImage

```
/**
 * Convert FlyCapture2 fc2Image to OpenCV IplImage type
 */
IplImage* ConvertFC2ImageToOpenCV(fc2Image* pImage)
{
    IplImage* cvImage = NULL;
    bool bColor = true;
    CvSize mySize;
    mySize.height = pImage->rows;
    mySize.width = pImage->cols;
    bool bInitialized = false;
    fc2Image colorImage;
    fc2Error error;
    switch ( pImage->format )
    {
    case FC2_PIXEL_FORMAT_MONO8:     cvImage = cvCreateImageHeader(mySize, 8, 1 );
        cvImage->depth = IPL_DEPTH_8U;
        cvImage->nChannels = 1;
        bColor = false;
        break;
    case FC2_PIXEL_FORMAT_411YUV8:   cvImage = cvCreateImageHeader(mySize, 8, 3 );
        cvImage->depth = IPL_DEPTH_8U;
        cvImage->nChannels = 3;
        break;
    case FC2_PIXEL_FORMAT_422YUV8:   cvImage = cvCreateImageHeader(mySize, 8, 3 );
        cvImage->depth = IPL_DEPTH_8U;
        cvImage->nChannels = 3;
        break;
    case FC2_PIXEL_FORMAT_444YUV8:   cvImage = cvCreateImageHeader(mySize, 8, 3 );
        cvImage->depth = IPL_DEPTH_8U;
        cvImage->nChannels = 3;
        break;
    case FC2_PIXEL_FORMAT_RGB8:      cvImage = cvCreateImageHeader(mySize, 8, 3 );
        cvImage->depth = IPL_DEPTH_8U;
        cvImage->nChannels = 3;
        break;
    case FC2_PIXEL_FORMAT_MONO16:    cvImage = cvCreateImageHeader(mySize, 16, 1 );
        cvImage->depth = IPL_DEPTH_16U;
        cvImage->nChannels = 1;
        bColor = false;
        break;
    case FC2_PIXEL_FORMAT_RGB16:     cvImage = cvCreateImageHeader(mySize, 16, 3 );
        cvImage->depth = IPL_DEPTH_16U;
        cvImage->nChannels = 3;
        break;
    case FC2_PIXEL_FORMAT_S_MONO16:  cvImage = cvCreateImageHeader(mySize, 16, 1 );
        cvImage->depth = IPL_DEPTH_16U;
        cvImage->nChannels = 1;
        bColor = false;
        break;
    case FC2_PIXEL_FORMAT_S_RGB16:   cvImage = cvCreateImageHeader(mySize, 16, 3 );
        cvImage->depth = IPL_DEPTH_16U;
        cvImage->nChannels = 3;
        break;
    case FC2_PIXEL_FORMAT_RAW8:      cvImage = cvCreateImageHeader(mySize, 8, 3 );
        cvImage->depth = IPL_DEPTH_8U;
        cvImage->nChannels = 3;
        break;
    case FC2_PIXEL_FORMAT_RAW16:     cvImage = cvCreateImageHeader(mySize, 8, 3 );
        cvImage->depth = IPL_DEPTH_8U;
        cvImage->nChannels = 3;
        break;
    case FC2_PIXEL_FORMAT_MONO12:    printf("Not supported by OpenCV");
        bColor = false;
        break;
    case FC2_PIXEL_FORMAT_RAW12:     printf("Not supported by OpenCV");
        break;
    case FC2_PIXEL_FORMAT_BGR:       cvImage = cvCreateImageHeader(mySize, 8, 3 );
        cvImage->depth = IPL_DEPTH_8U;
        cvImage->nChannels = 3;
        break;
    case FC2_PIXEL_FORMAT_BGRU:      cvImage = cvCreateImageHeader(mySize, 8, 4 );
        cvImage->depth = IPL_DEPTH_8U;
        cvImage->nChannels = 4;
        break;
    case FC2_PIXEL_FORMAT_RGBU:      cvImage = cvCreateImageHeader(mySize, 8, 4 );
        cvImage->depth = IPL_DEPTH_8U;
        cvImage->nChannels = 4;
        break;
    default: printf("Some error occured...\n");
        return NULL;
    }

    if(bColor) {
        error = fc2CreateImage( &colorImage );
        if ( error != FC2_ERROR_OK )
        {
            printf( "Error in fc2CreateImage: %d\n", error );
        }
        if(!bInitialized)
        {
            fc2SetImageData(&colorImage, (unsigned char*)malloc(sizeof(int) * pImage->cols * pImage->rows*3), pImage->cols * pImage->rows*3);
            bInitialized = true;
        }
        fc2ConvertImageTo(FC2_PIXEL_FORMAT_BGR, pImage, &colorImage); //needs to be as BGR to be saved
        if ( error != FC2_ERROR_OK )
        {
            printf( "Error in fc2ConvertImageTo: %d\n", error );
        }
        cvImage->width = colorImage.cols;
        cvImage->height = colorImage.rows;
        cvImage->widthStep = colorImage.stride;
        cvImage->origin = 0; //interleaved color channels
        cvImage->imageDataOrigin = (char*)colorImage.pData; //DataOrigin and Data same pointer, no ROI
        cvImage->imageData = (char*)(colorImage.pData);
        cvImage->widthStep = colorImage.stride;
        cvImage->nSize = sizeof (IplImage);
        cvImage->imageSize = cvImage->height * cvImage->widthStep;
        error = fc2DestroyImage(&colorImage);
        if ( error != FC2_ERROR_OK )
        {
            printf( "Error in fc2CreateImaged: %d\n", error );           
            return -1;
        }
    }
    else
    {
        cvImage->imageDataOrigin   = (char*)(pImage->pData);
        cvImage->imageData         = (char*)(pImage->pData);
        cvImage->widthStep         = pImage->stride;
        cvImage->nSize             = sizeof (IplImage);
        cvImage->imageSize         = cvImage->height * cvImage->widthStep;
    }
    return cvImage;
}
```



2. OpenCV IplImage to FlyCapture2 fc2Image

```
/**
 * Convert OpenCV IplImage to FlyCapture2 fc2Image type
 */
fc2Image* ConvertIplImageToFC2Image(IplImage* cvImage)
{
    fc2Image* pImage;
    fc2Error error;
    fc2BayerTileFormat btf = FC2_BT_GBRG;
    error = fc2CreateImage(pImage);
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2CreateImage: %d\n", error );
    }
    if (cvImage->depth == IPL_DEPTH_8U) {
        if (cvImage->nChannels == 1) error = fc2SetImageDimensions(pImage, cvImage->height, cvImage->width, cvImage->widthStep, FC2_PIXEL_FORMAT_MONO8, btf);
        if (cvImage->nChannels == 3) error = fc2SetImageDimensions(pImage, cvImage->height, cvImage->width, cvImage->widthStep, FC2_PIXEL_FORMAT_RGB8, btf);
        if (cvImage->nChannels == 4) error = fc2SetImageDimensions(pImage, cvImage->height, cvImage->width, cvImage->widthStep, FC2_PIXEL_FORMAT_BGRU, btf);
    } else if (cvImage->depth == IPL_DEPTH_16U) {
        if (cvImage->nChannels == 1) error = fc2SetImageDimensions(pImage, cvImage->height, cvImage->width, cvImage->widthStep, FC2_PIXEL_FORMAT_MONO16, btf);
        if (cvImage->nChannels == 3) error = fc2SetImageDimensions(pImage, cvImage->height, cvImage->width, cvImage->widthStep, FC2_PIXEL_FORMAT_RGB16, btf);
    } else error = fc2SetImageDimensions(pImage, cvImage->height, cvImage->width, cvImage->widthStep, FC2_UNSPECIFIED_PIXEL_FORMAT, btf);
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2SetImageDimensions: %d\n", error );
    }
    fc2SetImageData(pImage, (const unsigned char*)cvImage->imageData, cvImage->imageSize);
    if (error != FC2_ERROR_OK)
    {
        printf( "Error in fc2SetImageData: %d\n", error );
    }
    return pImage;
}
```














