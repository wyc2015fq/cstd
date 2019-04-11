# Halcon10 与 VC＋＋交互，通过picture control显示图像

2015年05月25日 19:50:47 [龙舞天涯](https://me.csdn.net/u011135902) 阅读数：597



vs2010运行通过。VC＋＋做的。

这个程序大概时这样的，左边是一个picture control，点击识别，通过调用halcon读图片，显示在vc＋＋的mfc的picture control控件上。

![img](https://img-blog.csdn.net/20130821082835203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG9uZ2hhaXFpYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



步骤：

## 1、添加halcon头文件

![img](https://img-blog.csdn.net/20130821083301609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG9uZ2hhaXFpYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## 2、添加一些类库

![img](https://img-blog.csdn.net/20130821083528187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG9uZ2hhaXFpYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 3、写一个picture control窗口，你再在OnInitDialog里调用来初始化呀。

```cpp
void CHalconOCRExampleDlg::InitHalconWindow(void)  
{  
    //定义显示的起点和长宽高   
    HTuple HWindowRow,HWindowColumn,HWindowWidth,HWindowHeight;    
    //定义窗口ID     
    HTuple HWindowID;  
  
    //定义一个CRect类的对象，用于获取图像框的尺寸和位置     
    CRect Rect;    
    CStatic* pWnd = (CStatic*)GetDlgItem(IDC_PIC_HWINDOW);    
   
    CDC *pDC = pWnd->GetDC();    
    pWnd->GetWindowRect(&Rect);    
   
    //设置显示窗口的起点和宽高     
    HWindowRow = 0;    
    HWindowColumn = 0;    
    HWindowWidth = Rect.Width();    
    HWindowHeight = Rect.Height();  
    //设定固定的窗口大小，而不是根据控件大小设定     
    //GetDlgItem(IDC_PIC_HWINDOW)->SetWindowPos(NULL,0,0,Rect.Width(),Rect.Height(),SWP_NOZORDER | SWP_NOMOVE);     
    try    
    {    
        set_window_attr("border_width",0);    
        HWindowID = (Hlong)pWnd->m_hWnd;    
open_window(HWindowRow,HWindowColumn,HWindowWidth,HWindowHeight,HWindowID,"visible","",&m_HWindowID);
        set_draw(m_HWindowID, "margin"/,"margin"/);//设置显示模式是margin(边缘)     
    }    
    catch (HException& except)    
    {    
        TRACE(except.message);    
    }    
}  

//嗯，对了，你还要在类里声明几个变量，如下：
HTuple m_HWindowID;//picture control经过处理之后标示符，以后显示图像时，直接显示在m_HWindowID即可。  
HTuple m_FGHandle,m_ImageWidth, m_ImageHeight;//m_FGHandle是视频设备  
Hobject m_LiveImage;//从图片或视频中抓取图像到这个变量里面  
```

## 4、这时，我们需要显示图片的时候就可以调用

1. disp_obj(m_LiveImage, m_HWindowID);  

比如说像我的这个小程序：

```cpp
read_image(&m_LiveImage, "C:/Users/longx-app/Desktop/Images/lena.bmp");  
  
CRect Rect;    
CStatic* pWnd = (CStatic*)GetDlgItem(IDC_PIC_HWINDOW);    
  
CDC *pDC = pWnd->GetDC();    
pWnd->GetWindowRect(&Rect);   
  
get_image_pointer1(m_LiveImage, NULL, NULL, &m_ImageWidth, &m_ImageHeight);//得到图像的宽高和指针  
set_part(m_HWindowID, 0, 0, m_ImageHeight - 1, m_ImageWidth -1);//使图像显示适应窗口大小  
   
disp_obj(m_LiveImage, m_HWindowID);  
```





ok！我是一个没耐心写东西的人，就当做笔记。代码下载：





http://download.csdn.net/detail/loveq369/5984609