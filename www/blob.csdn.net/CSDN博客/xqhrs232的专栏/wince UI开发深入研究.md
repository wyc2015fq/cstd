# wince UI开发深入研究 - xqhrs232的专栏 - CSDN博客
2013年02月19日 00:28:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：995
原文地址::[http://blog.csdn.net/hnhyhongmingjiang/article/details/7586566](http://blog.csdn.net/hnhyhongmingjiang/article/details/7586566)
相关网帖
1.wince ui界面库网站----[http://www.wceui.cn/](http://www.wceui.cn/)
1：WINCE UI开发图形渲染选择
后续
2：WINCE UI 利器
xnview
3：图片打包内存解压技术
wince上图片的高效显示一直是一个问题，最近在TELECHIP系列上在研究图形如何在更高效的显示，决定放弃以前的UI架构，目前暂时准备实验的是一个打包资源然后内存读取再利用图形加速这块，目前初步实验阶段，先记录下
#include <imaging.h> 
#include <initguid.h>
#include <imgguids.h>
void CpngxianshiDlg::OnPaint()
{
 CPaintDC dc(this); // device context for painting
 CFile file;
 CFileException fe;
 CString FileName ;
 DWORD dwFileLength;
 BYTE* pBuff;
 FileName=L"\\Storage Card\\JPG_171.jpg";
 if(0 == file.Open(FileName, CFile::modeRead | CFile::shareDenyWrite, &fe))  //打开文件
 {
  AfxMessageBox(L"打开文件失败");
  return ;
 }
 dwFileLength = file.GetLength();  //取得文件总长度
 pBuff=new BYTE[dwFileLength];  
 file.Seek(0 ,CFile::begin);  //定位
 file.Read((BYTE*)(pBuff) , dwFileLength); //读字节
 file.Close();
 IImagingFactory   *pImgFactory   =   NULL;
 IImage   *pImage   =   NULL;
 HRESULT hrCoInit;
 HDC hdcCont=NULL,hdcMem=NULL;
 ImageInfo pimageinfo;//= new  sizeof(ImageInfo);
 RECT rect;
 //pImgFactory->CreateBitmapFromBuffer( );
 //if(FAILED(pImagingFactory->CreateImageFromBuffer(byData,dwLen,BufferDisposalFlag::BufferDisposalFlagCoTaskMemFree,&pImage)))goto End;
 //ImageInfo imgInfo;
 //使用IImagingFactory的CreateImageFromBuffer获取IImage指针时，需要注意参数的设置，
 //只有在第三个参数为BufferDisposalFlagNone时才能使用Release，不然会报内存错误。
 //如果参数为BufferDisposalFlagGlobalFree,则图片内存需要使用GlobalFree来释放(GlobalFree的使用要注意)。
 //if (FAILED(hrCoInit=CoInitializeEx(NULL, COINIT_MULTITHREADED)))goto End;
 //if(FAILED(CoCreateInstance(CLSID_ImagingFactory,NULL,CLSCTX_INPROC_SERVER,IID_IImagingFactory,(void**) &pImagingFactory)))goto End;
 hrCoInit=CoInitializeEx(NULL, COINIT_MULTITHREADED);
 CoCreateInstance(CLSID_ImagingFactory,NULL,1,IID_IImagingFactory,(void**) &pImgFactory);
 pImgFactory->CreateImageFromBuffer(pBuff,dwFileLength,BufferDisposalFlag::BufferDisposalFlagCoTaskMemFree,&pImage);
 pImage->GetImageInfo( &pimageinfo);
 rect.top =0;
 rect.left =0;
 rect.right =pimageinfo.Width ;
 rect.bottom =pimageinfo.Height ;
 pImage->Draw(dc,&rect,NULL);
 pImgFactory->Release();
 pImage->Release();
 delete pBuff;
}
//############################
void DrawImage(HDC hdc,RECT& rc,UINT nIDResource,CString lpType)
{
    HINSTANCE hInst = AfxGetResourceHandle();
    HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nIDResource), lpType); 
if (!hRsrc)
return;
//资源大小
    DWORD len = SizeofResource(hInst, hRsrc);
//转换图片资源为字节数组
    BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
if (!lpRsrc)
return;
    IImagingFactory *pImgFactory = NULL;
    IImage *pImage = NULL;
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
if (SUCCEEDED(CoCreateInstance (CLSID_ImagingFactory,
                                    NULL,
                                    CLSCTX_INPROC_SERVER,
                                    IID_IImagingFactory,
                                    (void**)&pImgFactory)))
    {
if (SUCCEEDED(pImgFactory->CreateImageFromBuffer(lpRsrc,len,BufferDisposalFlagNone,&pImage)))
        {
            pImage->Draw(hdc, &rc, NULL);
            pImage->Release();
        }
        pImgFactory->Release();
    }
    CoUninitialize();
//释放资源
    FreeResource(hRsrc);
}
代码说明：
　　　　　　a).　　关键部分为将LoadResource转换成BYTE*以及CreateImageFromBuffer方法。
