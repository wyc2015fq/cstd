# 多开 MFC线程 - wishchinYang的专栏 - CSDN博客
2013年07月17日 12:30:05[wishchin](https://me.csdn.net/wishchin)阅读数：935
序言：我才编程几年啊！就要处理多线程。对于只写函数的我，这难度简直了！不过MFC的多线程，貌似比较简单，还能处理的了。
（1）.开MFC多个线程
      在视频采集的过程中，如果不使用媒体计数器，会造成主线程只用于处理采集处理，主界面失去响应；
      多开几个线程达到此效果.
 一： 头文件：AviTestDlg.h
```cpp
//建立参数结构体
typedef struct                       //传入线程参数结构体wishchin
   {
    IplImage*                 TheImage;           //与CAviTestDlg 同参数？ wishchin
    CAviTestDlg*              dlg;                    //与主对话框成为友元 wishchin
    }lparameter,lparameter2 , *lpParam;                          //生成参数结构体
   //类内设置友元，声明全局线程为友元
class CAviTestDlg : public CDialog
   {
  public:
    friend UINT CPicThreadInfo(lpParam lp);//线程设置为友元，为使线程访问主对话框函数及变量
    friend UINT CPicThreadInfo2(lpParam lpFuc);
   }
```
二：源码文件 AviTestDlg.cpp
```cpp
//添加全局线程
  UINT CPicThreadInfo(lpParam lp);
 //在按下控件后，创建线程
 void CAviTestDlg::OnBnClickedTraceStart()
{
    //测试代码，按下开始追踪，创建新线程  wishchin
    UpdateData(TRUE);
    ///////////////////////////////////////////
    //初始化参数。。。
    lpParam lp = new lparameter;
    lp->TheImage               =TheImage;
    lp->dlg                     =this;
    StartCapture(lp);//调用开始捕捉函数
}
//开始捕捉函数
void CAviTestDlg::StartCapture(lpParam lp)
{
    vector<CWinThread*>  handle;
    //////////////////////////
    lpParam parameter= new lparameter;     ///////////////////////// 初始化参数列表   在线程函数内部初始化 可以？wishchin        
    parameter->TheImage              =        lp->TheImage ;
    parameter->dlg                           =        lp->dlg;
    ///////////////////////////////////////////////////////////////////
    CWinThread *pMyThread = AfxBeginThread(AFX_THREADPROC(CPicThreadInfo),parameter);  ////创建线程函数CPicThreadInfo wishchin
    pMyThread->m_bAutoDelete = FALSE;
    pMyThread->ResumeThread();
    handle.push_back(pMyThread);  //用向量来容纳线程， 真正地开始创建线程 
}
//线程函数
UINT CPicThreadInfo(lpParam lp)
   {
    IplImage*              TheImage                =        lp->TheImage ;
    CAviTestDlg*         dlg                            =        lp->dlg;
    /////////////////////////////////////////////////////////////////
    //打开视频文件
    CvCapture* pCapture = cvCaptureFromFile("C:\\test.avi");    //****测试代码，为少点击一个按钮。。wishchin
    if( !(pCapture = cvCaptureFromFile(FilePath)))              //argv[1] filename
      {
        fprintf(stderr, "Can not open video file %s\n", "E:\\test.avi");
        return -2;
       }
  }
```
过程是这样的：
       初始化参数——>初始化线程——>线程函数.
后记：使用AFX函数还是不太好的，涉及到内存问题；最后改用了BOOST函数对象，利用boost线程来实现多线程........
