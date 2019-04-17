# OpenCV IplImage FlyCapture2 Image Conversion 两种图像类的相互转化 - Grandyang - 博客园







# [OpenCV IplImage FlyCapture2 Image Conversion 两种图像类的相互转化](https://www.cnblogs.com/grandyang/p/4109742.html)







OpenCV的IplImag和 FlyCapture2 的 Image是两种常见的图片格式，在实际的应用中，我们通常要混合使用OpenCV和FlyCapture2这两个SDK，所以这两种图片格式之间的相互转换就是经常需要做的。

首先分别来看看两种图片类的定义：

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



Image:

```
/**
         * Construct an Image object with the specified arguments.
         * Ownership of the image buffer is not transferred to the Image object.
         * It is the user's responsibility to delete the buffer when it is
         * no longer in use.
         *
         * @param rows Rows in the image.
         * @param cols Columns in the image.
         * @param stride Stride of the image buffer.
         * @param pData Pointer to the image buffer.
         * @param dataSize Size of the image buffer.
         * @param format Pixel format.
         * @param bayerFormat Format of the Bayer tiled raw image.
         */ 
        Image( 
            unsigned int    rows,
            unsigned int    cols,
            unsigned int    stride, 
            unsigned char*  pData, 
            unsigned int    dataSize,
            PixelFormat     format, 
            BayerTileFormat bayerFormat = NONE );
```



下面我们来进行相互转换：

1. FlyCapture2 Image to OpenCV IplImage



```
IplImage* ConvertImageToOpenCV(Image* pImage)
{
    IplImage* cvImage = NULL;
    bool bColor = true;
    CvSize mySize;
    mySize.height = pImage->GetRows();
    mySize.width = pImage->GetCols();
    bool bInitialized = false;
    Image colorImage;
    switch ( pImage->GetPixelFormat() )
    {
        case PIXEL_FORMAT_MONO8:     cvImage = cvCreateImageHeader(mySize, 8, 1 );
                                     cvImage->depth = IPL_DEPTH_8U;
                                     cvImage->nChannels = 1;
                                     bColor = false;
                                     break;
        case PIXEL_FORMAT_411YUV8:   cvImage = cvCreateImageHeader(mySize, 8, 3 );
                                     cvImage->depth = IPL_DEPTH_8U;
                                     cvImage->nChannels = 3;
                                     break;
        case PIXEL_FORMAT_422YUV8:   cvImage = cvCreateImageHeader(mySize, 8, 3 );
                                     cvImage->depth = IPL_DEPTH_8U;
                                     cvImage->nChannels = 3;
                                     break;
        case PIXEL_FORMAT_444YUV8:   cvImage = cvCreateImageHeader(mySize, 8, 3 );
                                     cvImage->depth = IPL_DEPTH_8U;
                                     cvImage->nChannels = 3;
                                     break;
        case PIXEL_FORMAT_RGB8:      cvImage = cvCreateImageHeader(mySize, 8, 3 );
                                     cvImage->depth = IPL_DEPTH_8U;
                                     cvImage->nChannels = 3;
                                     break;
        case PIXEL_FORMAT_MONO16:    cvImage = cvCreateImageHeader(mySize, 16, 1 );
                                     cvImage->depth = IPL_DEPTH_16U;
                                     cvImage->nChannels = 1;
                                     bColor = false;
                                     break;
        case PIXEL_FORMAT_RGB16:     cvImage = cvCreateImageHeader(mySize, 16, 3 );
                                     cvImage->depth = IPL_DEPTH_16U;
                                     cvImage->nChannels = 3;
                                     break;
        case PIXEL_FORMAT_S_MONO16:  cvImage = cvCreateImageHeader(mySize, 16, 1 );
                                     cvImage->depth = IPL_DEPTH_16U;
                                     cvImage->nChannels = 1;
                                     bColor = false;
                                     break;
        case PIXEL_FORMAT_S_RGB16:   cvImage = cvCreateImageHeader(mySize, 16, 3 );
                                     cvImage->depth = IPL_DEPTH_16U;
                                     cvImage->nChannels = 3;
                                     break;
        case PIXEL_FORMAT_RAW8:      cvImage = cvCreateImageHeader(mySize, 8, 3 );
                                     cvImage->depth = IPL_DEPTH_8U;
                                     cvImage->nChannels = 3;
                                     break;
        case PIXEL_FORMAT_RAW16:     cvImage = cvCreateImageHeader(mySize, 8, 3 );
                                     cvImage->depth = IPL_DEPTH_8U;
                                     cvImage->nChannels = 3;
                                     break;
        case PIXEL_FORMAT_MONO12:    printf("Not supported by OpenCV");
                                     bColor = false;
                                     break;
        case PIXEL_FORMAT_RAW12:     printf("Not supported by OpenCV");
                                     break;
        case PIXEL_FORMAT_BGR:       cvImage = cvCreateImageHeader(mySize, 8, 3 );
                                     cvImage->depth = IPL_DEPTH_8U;
                                     cvImage->nChannels = 3;
                                     break;
        case PIXEL_FORMAT_BGRU:      cvImage = cvCreateImageHeader(mySize, 8, 4 );
                                     cvImage->depth = IPL_DEPTH_8U;
                                     cvImage->nChannels = 4;
                                     break;
        case PIXEL_FORMAT_RGBU:      cvImage = cvCreateImageHeader(mySize, 8, 4 );
                                     cvImage->depth = IPL_DEPTH_8U;
                                     cvImage->nChannels = 4;
                                     break;
        default: printf("Some error occured...\n");
                 return NULL;
    }

    if(bColor) {
        if(!bInitialized)
        {
            colorImage.SetData(new unsigned char[pImage->GetCols() * pImage->GetRows()*3], pImage->GetCols() * pImage->GetRows()*3);
            bInitialized = true;
        }

        pImage->Convert(PIXEL_FORMAT_BGR, &colorImage); //needs to be as BGR to be saved

        cvImage->width = colorImage.GetCols();
        cvImage->height = colorImage.GetRows();
        cvImage->widthStep = colorImage.GetStride();

        cvImage->origin = 0; //interleaved color channels

        cvImage->imageDataOrigin = (char*)colorImage.GetData(); //DataOrigin and Data same pointer, no ROI
        cvImage->imageData         = (char*)(colorImage.GetData());
        cvImage->widthStep      = colorImage.GetStride();
        cvImage->nSize = sizeof (IplImage);
        cvImage->imageSize = cvImage->height * cvImage->widthStep;
    }
    else
    {
        cvImage->imageDataOrigin = (char*)(pImage->GetData());
        cvImage->imageData         = (char*)(pImage->GetData());
        cvImage->widthStep         = pImage->GetStride();
        cvImage->nSize             = sizeof (IplImage);
        cvImage->imageSize         = cvImage->height * cvImage->widthStep;

        //at this point cvImage contains a valid IplImage
     }
    return cvImage;
}
```



2. OpenCV IplImage to FlyCapture2 Image



```
/**
 * Convert OpenCV IplImage to FlyCapture2 Image type
 */
Image* ConvertIplImageToImage(IplImage* cvImage)
{
    Image* pImage = new Image();
    Error error;
    BayerTileFormat btf = GBRG;
    if (cvImage->depth == IPL_DEPTH_8U) {
        if (cvImage->nChannels == 1) error = pImage->SetDimensions(cvImage->height, cvImage->width, cvImage->widthStep, PIXEL_FORMAT_MONO8, btf);
        if (cvImage->nChannels == 3) error = pImage->SetDimensions(cvImage->height, cvImage->width, cvImage->widthStep, PIXEL_FORMAT_RGB8, btf);
        if (cvImage->nChannels == 4) error = pImage->SetDimensions(cvImage->height, cvImage->width, cvImage->widthStep, PIXEL_FORMAT_BGRU, btf);
    } else if (cvImage->depth == IPL_DEPTH_16U) {
        if (cvImage->nChannels == 1) error = pImage->SetDimensions(cvImage->height, cvImage->width, cvImage->widthStep, PIXEL_FORMAT_MONO16, btf);
        if (cvImage->nChannels == 3) error = pImage->SetDimensions(cvImage->height, cvImage->width, cvImage->widthStep, PIXEL_FORMAT_RGB16, btf);
    } else error = pImage->SetDimensions(cvImage->height, cvImage->width, cvImage->widthStep, UNSPECIFIED_PIXEL_FORMAT, btf);
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
        return NULL;
    }

    error = pImage->SetData((const unsigned char*)cvImage->imageData, cvImage->imageSize);
    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();
        return NULL;
    }
    return pImage;
}
```














