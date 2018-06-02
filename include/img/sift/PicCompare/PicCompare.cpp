// PicCompare.cpp : Defines the entry point for the console application.
//

#include <cv.h>
#include <highgui.h>
#include <direct.h>
#include <conio.h>
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

//using namespace std;

#define COMPARE_PATH_LEN 255

int HistogramBins = 256;
float HistogramRange1[2]={0,255};
float *HistogramRange[1]={&HistogramRange1[0]};

char g_imgsrc1[COMPARE_PATH_LEN];
char g_imgsrc2[COMPARE_PATH_LEN];
bool g_bShowWindow = false;

#define __out
void parseArgs(int, _TCHAR*[]);
bool cvt2PhysicalPath(const char* srcbuff, __out char* outbuff, int maxlen );
void showHelp();

int _tmain(int argc, _TCHAR* argv[])
{
  showHelp();
  parseArgs(argc, argv);
  
  IplImage *Image1=cvLoadImage(g_imgsrc1, 0);
  IplImage *Image2=cvLoadImage(g_imgsrc2, 0);
  if(Image1==NULL)
  {
    printf("program cannot load the first picture!\n");
    return 0;
  }
  if(Image2==NULL)
  {
    printf("program cannot load the second picture!\n");
    return 0;
  }
  
  CvHistogram *Histogram1=cvCreateHist(1,&HistogramBins,CC_HIST_SPARSE,HistogramRange);
  CvHistogram *Histogram2=cvCreateHist(1,&HistogramBins,CC_HIST_SPARSE,HistogramRange);
  
  cvCalcHist(&Image1,Histogram1);
  cvCalcHist(&Image2,Histogram2);
  
  cvNormalizeHist(Histogram1,1);
  cvNormalizeHist(Histogram2,1);
  if(g_bShowWindow)
  {
    cvNamedWindow( "img1",   CC_WINDOW_AUTOSIZE ); //创建用于显示源图像的窗口
    cvNamedWindow( "img2",   CC_WINDOW_AUTOSIZE ); //创建用于显示目标图像的窗口
    
    cvShowImage( "img1", Image1);   //显示源图像
    cvShowImage( "img2", Image2);   //显示目标图像
  }
  //void cvDestroyAllWindows(void);
  
  printf("CC_COMP_CORREL : %.4f\n",cvCompareHist(Histogram1,Histogram2,CC_COMP_CORREL));
  printf("CC_COMP_CHISQR : %.4f\n",cvCompareHist(Histogram1,Histogram2,CC_COMP_CHISQR));
  printf("CC_COMP_INTERSECT : %.4f\n",cvCompareHist(Histogram1,Histogram2,CC_COMP_INTERSECT));
  printf("CC_COMP_BHATTACHARYYA : %.4f\n",cvCompareHist(Histogram1,Histogram2,CC_COMP_BHATTACHARYYA));
  
  if(g_bShowWindow)
  {
    cvWaitKey(-1);   //等待用户响应
    cvReleaseImage(&Image1); //释放源图像占用的内存
    cvReleaseImage(&Image2); //释放目标图像占用的内存
    cvDestroyWindow("img1" ); //销毁窗口“src”
    cvDestroyWindow("img2" ); //销毁窗口“dst”
  }
  _getch();
  return 0;
}


void parseArgs(int argc, _TCHAR* argv[])
{
  memset(g_imgsrc1, 0, COMPARE_PATH_LEN);
  memset(g_imgsrc2, 0, COMPARE_PATH_LEN);
  for(int i=0; i< argc; i++)
  {
    char* szSearch = NULL;
    szSearch = strstr(argv[i], "exe");
    if(szSearch!=NULL)
      continue;
    
    if(strlen(argv[i])>3)
    {
      if(strlen(g_imgsrc1)==0)
      {
        if(!cvt2PhysicalPath(argv[i], g_imgsrc1,COMPARE_PATH_LEN))
        {
          continue;;
        }
        else
        {
          printf("Image 1: %s\n", g_imgsrc1);
        }
        
      }else if(strlen(g_imgsrc2)==0)
      {
        if(!cvt2PhysicalPath(argv[i], g_imgsrc2,COMPARE_PATH_LEN))
        {
          continue;
        }
        else
        {
          printf("Image 2: %s\n", g_imgsrc2);
        }
        
      }
    }
    else
    {
      if(strchr(argv[i], 's')!=NULL)
      {
        g_bShowWindow = true;
      }
      else if(strchr(argv[i], 'h')!=NULL)
      {
        showHelp();
      }
      
    }
  }
  
  if(strlen(g_imgsrc1)==0)
  {
    printf("Please input the first picture[example: C:\\img.jpg].\n");
    scanf("%s",g_imgsrc1);
    //cin>>g_imgsrc1;
  }
  if(strlen(g_imgsrc2)==0)
  {
    printf("Please input the second picture[example: C:\\img.jpg].\n");
    scanf("%s",g_imgsrc2);
    //cin>>g_imgsrc2;
  }
  
}

bool cvt2PhysicalPath(const char* srcbuff, __out char* outbuff, int maxlen )
{
  if(strchr(srcbuff,':')) //物理路径
  {
    strcpy(outbuff, srcbuff);
    return true;
  }
  
  if(maxlen<=0)
    return false;
    /* DWORD WINAPI GetFullPathName(
    __in          LPCTSTR lpFileName,
    __in          DWORD nBufferLength,
    __out         LPTSTR lpBuffer,
    __out         LPTSTR* lpFilePart
    );
  */
  
  int nRet = GetFullPathName(srcbuff, maxlen, outbuff, NULL);
  if(nRet>maxlen)
    return false;
  else
    return true;
  
}

void showHelp()
{
  printf("The program compares 2 pictures and gives out the similarity in 4 ways.\n\n");
  printf("picCompare.exe [picpath1] [picpath2] [-s] [-h]\n\n");
  printf("picpath1\tThe first picture to compare.\n");
  printf("picpath2\tThe second picture to compare.\n");
  printf("-s\t\tShow the pictures in a window.\n");
  printf("-h\t\tShow help content what you're reading now.\n\n");
  printf("If either of 2 paths is not specified, you can specify it later.\n\n");
  return;
}