# VC实现基于Office 2007 MODI 12.0实现图形文字识别(OCR)的程序 - 专注于数据挖掘算法研究和应用 - CSDN博客





2012年02月07日 16:23:10[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：12974








1.安装Microsoft Office Document Imaging 

    1）查看是否安装：开始->所有程序->Microsoft Office->工具-> Microsoft Office Document Imaging  

![](http://hi.csdn.net/attachment/201105/21/0_1305971404M8rU.gif)

    或者 

       C:/Program Files/Common Files/Microsoft Shared/MODI/12.0

   2）如未安装，则打开：控制面板->添加或删除程序->Microsoft Office 2007->更改->添加或删除功能->继续->office工具->Microsoft Office Document Imaging ->从本机运行全部程序->继续

![](http://hi.csdn.net/attachment/201105/21/0_1305971535m100.gif)

      需要指定office2007安装盘路径，按照提示即可；

2.开发：

   VC6.0建个MFC的Dialog based工程“OCR”，然后"Project"->"add to project"->"Components and controls"，在Registered ActiveX Controls中找到Microsoft Office Document Imaging Viewer Control 12.0，INSERT建立一个类。


![](http://hi.csdn.net/attachment/201105/21/0_1305973486n3Xb.gif)

       然后从控件工具箱中拖一个到窗口上，右键classWizard，Member Variables找到IDC_MIDOCVIEW1然后AddVariable变量名为m_MIDOCtrl。然后在窗口上拖三个按钮控件"Brower"、"OCR"和"CLOSE"、一个静态文本控件“IDC_STATIC_PATH”和一个可编辑文本框控件“IDC_EDIT_TEXT”。

![](http://hi.csdn.net/attachment/201105/21/0_1305973566gJa9.gif)

      然后在你的文件里面找到OCRDlg.cpp，在最上面写的#import "C:/Program Files/Common Files/Microsoft Shared/MODI/12.0/MDIVWCTL.DLL"，路径需要你自己改一下，大多数就是这个路径，然后编译一下，在Debug下能找到MDIVWCTL.tlh，复制到工程文件夹中，在OCRDlg.cpp中写入#include "MDIVWCTL.tlh"，然后删除刚刚写的#import. "C:/Program Files/Common
 Files/Microsoft Shared/MODI/12.0/MDIVWCTL.DLL"。

1）实现打开TIF图片并显示在 ActiveX Control 中，Browser按钮函数：

void COCRDlg::OnBtnBrowser() 

{

 // TODO: Add extra validation here

 CString csFilePath;  

    CFileDialog FileDlg(TRUE,"tif",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,"Image Files (*.tif;*.tif)|*.tif;*.tif|All Files(*.*)|*.*||"); 



    if(FileDlg.DoModal()==IDOK)  

    {  

        csFilePath =  FileDlg.GetPathName();  

        SetDlgItemText(IDC_STATIC_PATH,csFilePath);  

        ((CMiDocView *) GetDlgItem( IDC_MIDOCVIEW1 ))->SetFileName(csFilePath) ; 


        ((CMiDocView *) GetDlgItem( IDC_MIDOCVIEW1 ))->SetFitMode(1);  

    }  

}

2）COCRDlg类添加成员函数bReadOCRByMODIAXCtrl(CString csFilePath, CString &csText)，主实现识别

  BOOL COCRDlg::bReadOCRByMODIAXCtrl(CString csFilePath, CString &csText)  

{  

    BOOL bRet  = TRUE;  

    HRESULT hr = 0;  

    csText.Empty();  

    IUnknown   *pVal    = NULL;  

    MODI::IDocument  *IDobj   = NULL;  

    MODI::ILayout      *ILayout = NULL;  

    MODI::IImages    *IImages = NULL;  

    MODI::IImage     *IImage  = NULL;  

    MODI::IWords     *IWords  = NULL;  

    MODI::IWord      *IWord   = NULL;  

    try  

    {  

        pVal = (IUnknown *) m_MIDOCtrl.GetDocument();   

        if ( pVal != NULL )  

        {  


            //Already has image in it, Don't need to create again   

            //Just get IDocument interface   

            hr = pVal->QueryInterface(IID_IDocument,(void**) &IDobj);  

            if ( SUCCEEDED(hr) )  

            {  


                hr = IDobj->OCR(MODI::miLANG_ENGLISH/*miLANG_CHINESE_SIMPLIFIED*/,1,1); 


                if ( SUCCEEDED(hr) )  

                {  



                    IDobj->get_Images(&IImages);  

                    long iImageCount=0;  

                    IImages->get_Count(&iImageCount);  

                    for ( int img =0; img<iImageCount;img++)  

                    {  

                        IImages->get_Item(img,(IDispatch**)&IImage);  

                        IImage->get_Layout(&ILayout);  


                        long numWord=0;  

                        ILayout->get_NumWords(&numWord);  

                        ILayout->get_Words(&IWords);  

                        IWords->get_Count(&numWord);  


                        for ( long i=0; i<numWord;i++)  

                        {  

                            IWords->get_Item(i,(IDispatch**)&IWord);  

                            CString csTemp;  

                            BSTR result;  

                            IWord->get_Text(&result);  

                            char buf[256];  

                            sprintf(buf,"%S",result);  

                            csTemp.Format("%s",buf);  


                            csText += csTemp;  

                            csText +=" ";  

                        }  

                        //Release all objects   

                        IWord->Release();  

                        IWords->Release();  

                        ILayout->Release();  

                        IImage->Release();  

                    }  

                    IImages->Release();  

                } else {  

                    bRet = FALSE;  

                }  

            } else {  

                bRet = FALSE;  

            }  


            IDobj->Close(0);  

            IDobj->Release();  

            pVal->Release();  


        } else {  

            bRet = FALSE;  

        }  

        pVal    = NULL;  

        IDobj   = NULL;  

        ILayout = NULL;  

        IImages = NULL;  

        IImage  = NULL;  

        IWords  = NULL;  

        IWord   = NULL;  

    }  

    catch(...)  

    {  

    }  

    return bRet;  

}  

 此时VC编译器会显示IID_IDocument未声明，后面编辑MDIVWCTL.tlh即可。 

3）双击OCR按钮，写入响应函数，调用识别函数：



void COCRDlg::OnBtnOCR() 

{

 // TODO: Add your control notification handler code here

 CString csText;  

    CString csFilePath;  

    GetDlgItemText(IDC_STATIC_PATH,csFilePath);  


    if ( bReadOCRByMODIAXCtrl(csFilePath,csText) == TRUE )  

    {  

        SetDlgItemText(IDC_EDIT_TEXT,csText);  

    }else   

        SetDlgItemText(IDC_EDIT_TEXT,"ERROR");    

}




4）解决IID_IDocument未声明的问题，编辑MDIVWCTL.tlh，在在最后#pragma pack(pop)之前加入如下：

    //   

// Named GUID constants initializations   

//   

extern "C" const GUID __declspec(selectany) LIBID_MODI =  

{0xa5ededf4,0x2bbc,0x45f3,{0x82,0x2b,0xe6,0x0c,0x27,0x8a,0x1a,0x79}};  

extern "C" const GUID __declspec(selectany) IID_IMiFont =  

{0x1ffc9a7b,0x7606,0x4c97,{0x81,0x19,0x78,0x67,0x3e,0x9d,0x48,0x21}};  

extern "C" const GUID __declspec(selectany) IID_IMiRect =  

{0x87d6fb47,0x7cfa,0x4db3,{0x82,0xcd,0xe8,0xdc,0xc5,0xbd,0xa7,0xab}};  

extern "C" const GUID __declspec(selectany) IID_IMiRects =  

{0x895c88a9,0x8598,0x44cc,{0x91,0xc4,0x80,0x10,0xae,0x24,0x75,0xeb}};  

extern "C" const GUID __declspec(selectany) IID_IWord =  

{0x5612275b,0xb4f1,0x42af,{0xb6,0x96,0x36,0x0d,0x40,0xdf,0x04,0x1d}};  

extern "C" const GUID __declspec(selectany) IID_IWords =  

{0xd8363824,0x4b07,0x423b,{0x88,0x03,0x49,0xb4,0x70,0x43,0x4f,0x3d}};  

extern "C" const GUID __declspec(selectany) IID_ILayout =  

{0xc300c846,0xa3fd,0x4a5b,{0xad,0x65,0x4a,0x6a,0xb4,0x6b,0x78,0x21}};  

extern "C" const GUID __declspec(selectany) IID_IImage =  

{0xac0d48a6,0x886d,0x4eb5,{0xa8,0xa1,0x09,0x3d,0x60,0xb9,0xa8,0x4a}};  

extern "C" const GUID __declspec(selectany) IID_IImages =  

{0x77096557,0x54ca,0x45c8,{0x81,0xc9,0xe7,0x0c,0x2a,0xba,0xa0,0xfb}};  

extern "C" const GUID __declspec(selectany) IID_IDocument =  

{0xd4073843,0xa58a,0x469a,{0xa8,0xe2,0xcf,0xf3,0xff,0x77,0xee,0x4e}};  

extern "C" const GUID __declspec(selectany) DIID__IDocumentEvents =  

{0x0cf31963,0x5e4d,0x4772,{0xae,0x8d,0x82,0xfe,0x5a,0x08,0x2b,0x26}};  

extern "C" const GUID __declspec(selectany) CLSID_Document =  

{0x40942a6c,0x1520,0x4132,{0xbd,0xf8,0xbd,0xc1,0xf7,0x1f,0x54,0x7b}};  

extern "C" const GUID __declspec(selectany) CLSID_Layout =  

{0x1afc4117,0x128e,0x4314,{0x9d,0x53,0x64,0xcb,0xda,0x5c,0x7e,0x02}};  

extern "C" const GUID __declspec(selectany) DIID__IImageEvents =  

{0x99f3a122,0x448f,0x4d36,{0x82,0x43,0x48,0xbf,0xe3,0x2d,0x50,0xb2}};  

extern "C" const GUID __declspec(selectany) CLSID_Image =  

{0x56f963ec,0x6efc,0x4a6b,{0x9a,0x1e,0x5b,0xfe,0x54,0x5c,0x89,0xd0}};  

extern "C" const GUID __declspec(selectany) CLSID_Word =  

{0x5995e30f,0x59eb,0x42ae,{0xbd,0xcf,0x89,0x08,0x27,0x1c,0x0b,0x32}};  

extern "C" const GUID __declspec(selectany) CLSID_MiFont =  

{0xc5eae79b,0xaccc,0x4e51,{0x8e,0xd5,0xa1,0x6f,0x42,0xfb,0x7b,0x1f}};  

extern "C" const GUID __declspec(selectany) CLSID_Words =  

{0x72acbce3,0x9067,0x4d5e,{0x95,0x39,0x34,0x97,0xfe,0xcd,0x03,0x2d}};  

extern "C" const GUID __declspec(selectany) CLSID_Images =  

{0xf961d185,0x2cc1,0x43db,{0x80,0xab,0x56,0x7f,0xa1,0x02,0x51,0xec}};  

extern "C" const GUID __declspec(selectany) CLSID_MiRect =  

{0x00607e44,0x15df,0x49bc,{0xaf,0x0c,0xa9,0x02,0x3e,0x06,0x07,0x91}};  

extern "C" const GUID __declspec(selectany) CLSID_MiRects =  

{0xcb2fbd52,0x56c1,0x4dd8,{0x89,0x7f,0x96,0x1d,0x15,0x43,0xb8,0x77}};  

extern "C" const GUID __declspec(selectany) IID_IMiSelectRect =  

{0xea280c3c,0xe4b7,0x42bf,{0xac,0xc8,0xfe,0x3a,0xd3,0x58,0x16,0x38}};  

extern "C" const GUID __declspec(selectany) IID_IMiSelectRects =  

{0x3a1e1b7a,0xc041,0x4ddc,{0xbf,0x3b,0x04,0x2a,0x0b,0x95,0xb8,0x2b}};  

extern "C" const GUID __declspec(selectany) IID_IMiSelectableItem =  

{0x01c4414a,0xd123,0x4bc7,{0xa1,0xfa,0x64,0xe3,0x76,0xc0,0x16,0x55}};  

extern "C" const GUID __declspec(selectany) IID_IMiSelectableImage =  

{0xf6379198,0x3b20,0x461a,{0xb3,0xa9,0x19,0x19,0x45,0x75,0x25,0x57}};  

extern "C" const GUID __declspec(selectany) IID_IMiSearchCallback =  

{0x8376d508,0x78ca,0x416d,{0xa9,0x03,0x2e,0xd6,0x2e,0x91,0xe2,0x9b}};  

extern "C" const GUID __declspec(selectany) IID_IMiDocView =  

{0xf958524a,0x8422,0x4b07,{0xb6,0x9e,0x19,0x9f,0x24,0x21,0xed,0x89}};  

extern "C" const GUID __declspec(selectany) DIID__IMiDocViewEvents =  

{0x9028b775,0xec59,0x4118,{0x86,0x2a,0xef,0xde,0xaf,0x59,0x55,0xa4}};  

extern "C" const GUID __declspec(selectany) IID_IMiDocSearch =  

{0xbc06ed64,0xa40c,0x4fb4,{0xa6,0x60,0xac,0x6d,0xbc,0x75,0x22,0x92}};  

extern "C" const GUID __declspec(selectany) CLSID_MiDocView =  

{0xef347a62,0xba21,0x42e4,{0x94,0xa0,0x1c,0x0a,0x6d,0x7f,0xdf,0xe7}};  

extern "C" const GUID __declspec(selectany) CLSID_MiDocSearch =  

{0xfa554db9,0xc806,0x46a9,{0xae,0xf8,0xb5,0xaf,0x89,0x1a,0x94,0xf9}};  

extern "C" const GUID __declspec(selectany) CLSID_MiSelectRect =  

{0xa4d2b51f,0x809f,0x4c53,{0x91,0x47,0x55,0x16,0x87,0x18,0x16,0xbd}};  

extern "C" const GUID __declspec(selectany) CLSID_MiSelectRects =  

 {0x23e9e94b,0xc202,0x4e9c,{0x8c,0x2e,0xf5,0x3b,0xc5,0x36,0xff,0x1d}};  

这是office 2003生成的MDIVWCTL.tlh文件中存在的，但在office 2007生成的MDIVWCTL.tlh文件中缺失，因此要补上。

3.运行

运行程序，单击Browse导入测试图片，然后单击OCR进行光学字符识别，效果如图

![](http://hi.csdn.net/attachment/201105/21/0_13059752128Kk1.gif)



![](http://hi.csdn.net/attachment/201105/21/0_1305975326m1hy.gif)

   特别说明：

   1）只能识别TIF格式图片；

   2）仅对英文识别较为准确；



