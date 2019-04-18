# MIL与opencv的转化 - weixin_33985507的博客 - CSDN博客
2007年12月17日 20:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedBlock.gif)/**//* This is a standalone program. Pass an image name as a first parameter of the program.
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)   Switch between standard and probabilistic Hough transform by changing "#if 1" to "#if 0" and back */
![](https://blog.csdn.net/Images/OutliningIndicators/None.gif)#include <cv.h>
![](https://blog.csdn.net/Images/OutliningIndicators/None.gif)#include <highgui.h>
![](https://blog.csdn.net/Images/OutliningIndicators/None.gif)#include <math.h>
![](https://blog.csdn.net/Images/OutliningIndicators/None.gif)#include <stdio.h>
![](https://blog.csdn.net/Images/OutliningIndicators/None.gif)#include <mil.h>
![](https://blog.csdn.net/Images/OutliningIndicators/None.gif)//cxcore.lib cv.lib highgui.lib mil.lib milcor.lib milgen.lib milmet2.lib milcor2.lib milmet2d.lib milmet2cl.lib milpul.lib milorion.lib mil1394.lib milvga.lib 
![](https://blog.csdn.net/Images/OutliningIndicators/None.gif)#define CHECK_EXIT(x) {if(!(x)){printf("Error %s\n",#x);exit(0);}}
![](https://blog.csdn.net/Images/OutliningIndicators/None.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/None.gif)class CMIL_CV
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)    MIL_ID MilApplication,                           /**//* Application identifier.   */
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)          MilSystem,                                /**//* System identifier.        */
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//MilDisplay,                               /* Display identifier.       */ 
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)          MilDigitizer,                             /**//* Digitizer identifier.     */
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)          MilImageDisp;                             /**//* Image buffer identifier.  */
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)staticvoid MouseCallback (intevent, int x, int y, int flags, void* param) 
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)if(event==CV_EVENT_LBUTTONDOWN)//CV_EVENT_MOUSEMOVE)
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            IplImage *img=*(IplImage**)param;//为什么直接传param就不行
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            CHECK_EXIT(CV_IS_IMAGE(img));
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            CvFont ft=cvFont(2,1);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)char s[100];
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            sprintf(s,"%3d,%3d %d",x,y,CV_IMAGE_ELEM(img,BYTE,y,x));;
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            cvPutText(img,s,cvPoint(100,100),&ft,CV_RGB(0,0,0));
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            printf("%s\n",s);
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)public:
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)enum![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        Mil_CH0=M_CH0,
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        Mil_CH1=M_CH1,
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    };
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)    BOOL MilGetImage(int Channel,IplImage *&dst)
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)try
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MbufClear(MilImageDisp, 0);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MdigChannel(MilDigitizer, Channel);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MdigGrab(MilDigitizer, MilImageDisp);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)int w=MbufInquire(MilImageDisp,M_SIZE_X,M_NULL);;
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)int h=MbufInquire(MilImageDisp,M_SIZE_Y,M_NULL);;
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)int band=MbufInquire(MilImageDisp,M_SIZE_BAND,M_NULL);;
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)int pitch=MbufInquire(MilImageDisp,M_PITCH_BYTE,M_NULL);;
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//    IplImage *dst=cvCreateImage(cvSize(w,h),8,band);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)if(!dst)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                dst=cvCreateImage(cvSize(w,h),8,band);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//            CHECK_EXIT(dst->width==w && dst->height==h && dst->nChannels=band);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)char* p=dst->imageData;
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MbufGet(MilImageDisp,p);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            dst->origin = IPL_ORIGIN_TL;
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//cvFlip(dst,dst,0);
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)catch(![](https://www.cnblogs.com/Images/dot.gif))
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)return FALSE; 
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)return TRUE;
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    };
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)    BOOL MilOpen(char*DCF_NAME)
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)try
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MappAlloc(M_DEFAULT, &MilApplication);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MsysAlloc(M_SYSTEM_METEOR_II, M_DEF_SYSTEM_NUM, M_SETUP, &MilSystem);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//MdispAlloc(MilSystem, M_DEFAULT, M_DEF_DISPLAY_FORMAT, M_DEFAULT, &MilDisplay);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MdigAlloc(MilSystem, M_DEFAULT, DCF_NAME, M_DEFAULT, &MilDigitizer);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            printf(" Allocate a display buffer, clear it and display it. \n");
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MbufAllocColor(MilSystem,
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                MdigInquire(MilDigitizer, M_SIZE_BAND, M_NULL),
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                (long) (MdigInquire(MilDigitizer, M_SIZE_X, M_NULL)),
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                (long) (MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL)),
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)8L+M_UNSIGNED, 
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                M_IMAGE+M_GRAB+M_DISP,
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)&MilImageDisp);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MdigControl(MilDigitizer, M_GRAB_MODE, M_SYNCHRONOUS);   //must!!
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MdigControl(MilDigitizer, M_CAMERA_LOCK, M_ENABLE);//
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MbufClear(MilImageDisp, 0);
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)catch(![](https://www.cnblogs.com/Images/dot.gif))
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)return FALSE;
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)return TRUE;
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)    BOOL MilClose()
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)try![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MbufFree(MilImageDisp);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MdigFree(MilDigitizer);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MsysFree(MilSystem);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MappFree(MilApplication);  
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)        }catch (![](https://www.cnblogs.com/Images/dot.gif)) ![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)return FALSE;
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)return TRUE;
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)void Test()
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        CHECK_EXIT(MilOpen("H:/wqj/project/CMILL_CV/DCF6.dcf"));
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        IplImage *img0=0;    //必须初始化为0
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        IplImage *img1=0;
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)int Image_id=0;
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        cvNamedWindow( "CH0", 1 );
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        cvNamedWindow( "CH1", 1 );
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        cvSetMouseCallback( "CH0", MouseCallback,&img0);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        cvSetMouseCallback( "CH1", MouseCallback,&img1);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        printf("Press g to save image in current directory.\n");
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        printf("Press Esc to Exit.\n");
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)do
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//奇怪,抓到的图都是CH0的//--要设置同步
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MilGetImage(M_CH0,img0);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            CHECK_EXIT(img0);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)//printf("ImageSize %d*%d*%d\n",img0->width,img0->height,img0->nChannels);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            MilGetImage(M_CH1,img1);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            CHECK_EXIT(img1);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)int c=cvWaitKey(100);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)if(c=='g')
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)char img_name[100];
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                sprintf(img_name,"left_%.2d.bmp",Image_id);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                cvSaveImage(img_name,img0);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                sprintf(img_name,"right_%.2d.bmp",Image_id);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                cvSaveImage(img_name,img1);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                printf("save image %d ok\n",Image_id);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)                Image_id++;
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)elseif(c==27)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)break;
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            cvShowImage( "CH0", img0 );
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)            cvShowImage( "CH1", img1 );
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)while(1);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        printf("Exit..\n");
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        CHECK_EXIT(MilClose());
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        cvReleaseImage(&img0);
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)        cvReleaseImage(&img1);
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)};
![](https://blog.csdn.net/Images/OutliningIndicators/None.gif)int main(int argc, char** argv)
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockStart.gif)![](https://blog.csdn.net/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)    CMIL_CV milcv;
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)    milcv.Test();
![](https://blog.csdn.net/Images/OutliningIndicators/InBlock.gif)return0;
![](https://blog.csdn.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](https://blog.csdn.net/Images/OutliningIndicators/None.gif)
![](https://blog.csdn.net/Images/OutliningIndicators/None.gif)
