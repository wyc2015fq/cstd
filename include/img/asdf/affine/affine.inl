#include <cv.h>  
#include <highgui.h>  
#include <stdio.h>  
  
int main(int argc,char** argv)  
{  
    CvPoint2D32f srcTri[3],dstTri[3];  
    CvMat* rot_mat = cvCreateMat(2,3,CC_32FC1);  
    CvMat* warp_mat = cvCreateMat(2,3,CC_32FC1);  
    IplImage *src,*dst, *srcBak;  
    //int df1(0),df2(33),df3(85),df4(25),df5(15),df6(70);  
    int df1(0),df2(0),df3(0),df4(0),df5(0),df6(0);  
    double angle(-50.0),scale(0.6);  
    float delta = 0.01;  
    int pressKey;  
  
    if(!(src=cvLoadImage("12.bmp",1)) )  
    {  
        return -1;  
    }  
    srcTri[0].x = 0;  
    srcTri[0].y = 0;  
    srcTri[1].x = src->width-1;  
    srcTri[1].y = 0;  
    srcTri[2].x = 0;  
    srcTri[2].y = src->height-1;  
    dst = cvCloneImage(src);  
    srcBak = cvCloneImage(src);  
    cvCopy(src,srcBak);  
    dst->origin = src->origin;  
    cvZero(dst);  
    cvNamedWindow("at",1);  
    CvFont font = cvFont(1,1);  
  
    while(1)  
    {  
        dstTri[0].x = srcBak->width*delta*(df1%101);  
        dstTri[0].y = srcBak->height*delta*(df2%101);  
        dstTri[1].x = src->width-1 + srcBak->width*delta*(df3%101);  
        dstTri[1].y = srcBak->height*delta*(df4%101);  
        dstTri[2].x = srcBak->width*delta*(df5%101);  
        dstTri[2].y = src->height-1 + srcBak->height*delta*(df6%101);  
          
        cvGetAffineTransform(srcTri,dstTri,warp_mat);  
        cvWarpAffine(srcBak,dst,warp_mat);  
        cvCopy(dst,src);  
        //compute rotation matrix  
        CvPoint2D32f center = cvPoint2D32f(src->width/2,src->height/2);  
        cv2DRotationMatrix(center,angle,scale,rot_mat);  
        //do the transformation  
        cvWarpAffine(src,dst,rot_mat);  
        //--------------------------字符显示-----------------------------------------  
        char buf[8];  
        char dspStr1[32] = {'/0'};  //  
        char dspStr2[32] = {'/0'};  //  
        char dspStr3[32] = {'/0'};  //  
  
        memset(buf,'/0',sizeof(buf));  
        strcat(dspStr1,"|");  
        strcat(dspStr1,itoa(df4%101,buf,10));  
        strcat(dspStr1,",");  
        strcat(dspStr1,itoa(df5%101,buf,10));  
        strcat(dspStr1,",");  
        strcat(dspStr1,itoa(df6%101,buf,10));  
        strcat(dspStr1,"|");  
  
        strcat(dspStr2,"|angle=");  
        strcat(dspStr2,itoa(int(angle),buf,10));  
        strcat(dspStr2,",scale=");  
        strcat(dspStr2,itoa(int(scale*100),buf,10));  
        strcat(dspStr2,"%|");  
        //「」┌└┘┐|  
        strcat(dspStr3,"|");  
        strcat(dspStr3,itoa(df1%101,buf,10));  
        strcat(dspStr3,",");  
        strcat(dspStr3,itoa(df2%101,buf,10));  
        strcat(dspStr3,",");  
        strcat(dspStr3,itoa(df3%101,buf,10));  
        strcat(dspStr3,"|");  
        cvPutText(dst,dspStr1,iPOINT(dst->width-120,20),&font,cvScalar(0,0xff));  
        cvPutText(dst,dspStr2,iPOINT(dst->width-210,40),&font,cvScalar(0,0xff));  
        cvPutText(dst,dspStr3,iPOINT(dst->width-120,60),&font,cvScalar(0,0xff));  
        //-------------------------------------------------------------------------------------  
        cvShowImage("at",dst);  
        pressKey = cvWaitKey();  
    //  printf("src->width:%d/n",src->width);  
        printf("%c pressed/n",pressKey);  
        switch(pressKey)  
        {  
        case '1':  
            df1++;  
            break;  
        case 0x230000:  
            df1--;  
            break;  
        case '2':  
            df2++;  
            break;  
        case 0x280000:  
            df2--;  
            break;  
        case '3':  
            df3++;  
            break;  
        case 0x220000:  
            df3--;  
            break;  
        case '4':   //旋转角度  
            angle++;  
            break;  
        case 0x250000:  
            angle--;  
            break;  
        case '6':   //缩放  
            scale+=0.01;  
            break;  
        case 0x270000:  
            scale-=0.01;  
            break;  
        case '7':  
            df4++;  
            break;  
        case 0x240000:  
            df4--;  
            break;  
        case '8':  
            df5++;  
            break;  
        case 0x260000:  
            df5--;  
            break;  
        case '9':  
            df6++;  
            break;  
        case 0x210000:  
            df6--;  
            break;  
        case 27:  
            cvReleaseImage(&dst);  
            cvDestroyWindow("at");  
            cvReleaseMat(&rot_mat);  
            cvReleaseMat(&warp_mat);  
            return 0;  
        default:break;  
        }  
    }
  
    cvReleaseImage(&dst);  
    cvReleaseMat(&rot_mat);  
    cvReleaseMat(&warp_mat);  
    return 0;  
}  