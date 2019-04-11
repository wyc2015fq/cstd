# Halcon Image与OpenCV IplImage图像数据相互转换

2015年06月09日 21:45:02 [龙舞天涯](https://me.csdn.net/u011135902) 阅读数：1510



  这里贴出我用的两个函数，用于IplImage 与 Hobject 之间的相互转换，他们的数据类型分别是IPL_DEPTH_8U和'byte' 

  在Halcon中，图像数据是一个通道(channel)一个通道保持的，而在OpenCV中多个通道的数据分别在各个像素中。



```cpp
Hobject IplImageToHImage(IplImage *pImage)
{  
    Hobject Hobj;
    if (3 == pImage->nChannels)
    {
        IplImage *pImageRed,*pImageGreen,*pImageBlue;
        pImageRed = cvCreateImage(cvGetSize(pImage),IPL_DEPTH_8U,1);
        pImageGreen = cvCreateImage(cvGetSize(pImage),IPL_DEPTH_8U,1);
        pImageBlue = cvCreateImage(cvGetSize(pImage),IPL_DEPTH_8U,1);
        cvSplit(pImage,pImageBlue,pImageGreen,pImageRed,NULL);
        uchar *dataRed = new uchar[pImage->width*pImage->height];
        uchar *dataGreen = new uchar[pImage->width*pImage->height];;
        uchar *dataBlue = new uchar[pImage->width*pImage->height];;
             
        int height = pImage->height;
        int width =pImage->width;
        for(int i = 0; i<height; i++)
        {
            memcpy(dataRed + width*i,pImageRed->imageData + pImageRed->widthStep*i, width);
            memcpy(dataGreen + width*i,pImageGreen->imageData + pImageGreen->widthStep*i, width);
            memcpy(dataBlue + width*i,pImageBlue->imageData + pImageBlue->widthStep*i, width);
        }
        gen_image3(&Hobj,"byte",pImage->width,pImage->height,(Hlong)(dataRed),(Hlong)(dataGreen),(Hlong)(dataBlue));
        cvReleaseImage(&pImageRed);
        cvReleaseImage(&pImageGreen);
        cvReleaseImage(&pImageBlue);
        delete[] dataRed;
        delete[] dataGreen;
        delete[] dataBlue;
    }
    if (1 == pImage->nChannels)
    {
        int height = pImage->height;
        int width =pImage->width;
        uchar *dataGray = new uchar[width*height];
             
        for(int i = 0; i<height; i++)
        {
            memcpy(dataGray + width*i,pImage->imageData + pImage->widthStep*i, width);
        }
        gen_image1(&Hobj,"byte",pImage->width,pImage->height,(Hlong)(dataGray));
        delete[] dataGray;
    }   
    return Hobj;
}
IplImage* HImageToIplImage(Hobject &Hobj)
{
    IplImage *pImage;
    HTuple htChannels;
    char cType[MAX_STRING];
    Hlong     width,height;
    width = height =0;  
    //转换图像格式
    convert_image_type(Hobj,&Hobj,"byte");
    count_channels(Hobj,&htChannels);
    if (htChannels[0].I() == 1)  
    {
        unsigned char *ptr;
        get_image_pointer1(Hobj,(Hlong *)&ptr,cType,&width,&height);
        pImage = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
        for(int i = 0; i<height; i++)
        {
            memcpy(pImage->imageData+ pImage->widthStep*i, ptr + width*i, width);
        }
    }
    if (htChannels[0].I() == 3)  
    {
     
        unsigned char *ptrRed,*ptrGreen,*ptrBlue;
        ptrRed = ptrGreen =ptrBlue = NULL;
     
        get_image_pointer3(Hobj,(Hlong *)&ptrRed,(Hlong *)&ptrGreen,(Hlong *)&ptrBlue,cType,&width,&height);
     
        IplImage *pImageRed,*pImageGreen,*pImageBlue;
        pImage = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
        pImageRed = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
        pImageGreen = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
        pImageBlue = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
        for(int i = 0; i<height; i++)
        {
            memcpy(pImageRed->imageData+ pImageRed->widthStep*i, ptrRed + width*i, width);
            memcpy(pImageGreen->imageData+ pImageGreen->widthStep*i, ptrGreen + width*i, width);
            memcpy(pImageBlue->imageData+ pImageBlue->widthStep*i, ptrBlue + width*i, width);
        }
        cvMerge(pImageBlue,pImageGreen,pImageRed,NULL,pImage);
        cvReleaseImage(&pImageRed);
        cvReleaseImage(&pImageGreen);
        cvReleaseImage(&pImageBlue);
    }  
    return pImage;
}
```

