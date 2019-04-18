# VC常用代码集 - 深之JohnChen的专栏 - CSDN博客

2008年06月19日 15:36:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：5026


1、写日志函数 

void WriteLog(CString strLog) 

{ 

 CString strLogPath = "";  

char *pszPath; 

 DWORD nlen = 256; 

 pszPath = new char[nlen]; 

 memset(pszPath,0,sizeof(char)*nlen);

GetModuleFileName(NULL,pszPath,nlen);  

strLogPath = pszPath;  

strLogPath = strLogPath.Left(strLogPath.ReverseFind('//')+1);  

strLogPath = strLogPath + "ErrorInfo.log";  

if(pszPath) 

 {   

delete []pszPath;  

 pszPath = NULL;  

}  

CFile cf;  

OFSTRUCT of;  

HFILE hf = OpenFile(strLogPath,&of,OF_EXIST);  

if(hf==-1)  

{  

 cf.Open(strLogPath,CFile::modeCreate|CFile::modeWrite);  

} 

else 

 {   

cf.Open(strLogPath,CFile::modeWrite); 

  cf.SeekToEnd();  

} 

 int iTempValue = GetLastError(); 

 CString strWrite = ""; 

 strWrite = strLog + "/r/n"; 

 CCriticalSection sl; 

 sl.Lock();  

cf.Write(strWrite.GetBuffer(strWrite.GetLength()),strWrite.GetLength()); 

 sl.Unlock(); 

 cf.Close(); 

}

void WriteLog2(CString strLog) 

//带记录时间 

{  

CString strLogPath = ""; 

 char *pszPath;  

DWORD nlen = 256; 

 pszPath = new char[nlen];  

memset(pszPath,0,sizeof(char)*nlen);  

GetModuleFileName(NULL,pszPath,nlen);  

strLogPath = pszPath; 

 strLogPath = strLogPath.Left(strLogPath.ReverseFind('//')+1);  

strLogPath = strLogPath + "ErrorInfo.log";  

if(pszPath)  

{   

delete []pszPath;  

 pszPath = NULL;  

}  

CFile cf;  

OFSTRUCT of;  

HFILE hf = OpenFile(strLogPath,&of,OF_EXIST);  

if(hf==-1)  

{   

cf.Open(strLogPath,CFile::modeCreate|CFile::modeWrite); 

 }  

else  

{   

cf.Open(strLogPath,CFile::modeWrite);   

cf.SeekToEnd();  

}  

time_t ltime; 

struct tm *gmt;  time( <ime );     

gmt = gmtime( <ime );  

CString strTemp = "";     

strTemp.Format(" %s", asctime( gmt ) ); 

 CString strWrite = "";  

strWrite = strLog + strTemp + "/r/n";  

CCriticalSection sl;  

sl.Lock();  

cf.Write(strWrite.GetBuffer(strWrite.GetLength()),strWrite.GetLength());  

sl.Unlock();  

cf.Close(); 

}

2、返回的出错编码所对应错误信息 FormatMessage()得到由GetLastError()返回的出错编码所对应错误

信息的示例：    

 LPVOID lpMsgBuf;    

 FormatMessage(      FORMAT_MESSAGE_ALLOCATE_BUFFER |      FORMAT_MESSAGE_FROM_SYSTEM |      FORMAT_MESSAGE_IGNORE_INSERTS,      NULL,      GetLastError(),      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,      0,      NULL     );          

// Process any inserts in lpMsgBuf.    

 // ...         

 // Display the string.    

 MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK |MB_ICONINFORMATION );         

 // Free the buffer.     

LocalFree( lpMsgBuf ); 

3、保存位图

static BOOL SaveBitmap (HBITMAP hBmp, const char* pszFile)

{   

BITMAP bmp;   

PBITMAPINFO pbmi;   

WORD    cClrBits;   /* Retrieve the bitmap's color format, width, and height. */  

if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))    

 return FALSE;  

 /* Convert the color format to a count of bits. */  

cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);  

if (cClrBits == 1)     

cClrBits = 1;   

else if (cClrBits <= 4)    

 cClrBits = 4;  

 else if (cClrBits <= 8)    

cClrBits = 8;   

else if (cClrBits <= 16)     

cClrBits = 16;   

else if (cClrBits <= 24)    

cClrBits = 24;  

 else     cClrBits = 32;

  /*    * Allocate memory for the BITMAPINFO structure. (This structure    *

contains a BITMAPINFOHEADER structure and an array of

RGBQUAD data    * structures.)      */   

if (cClrBits < 24)    

 pbmi = (PBITMAPINFO) GlobalAlloc(LPTR,     sizeof(BITMAPINFOHEADER) +     sizeof(RGBQUAD) * (1 << cClrBits));  

/*    * There is no RGBQUAD array for the 24-bit-per-pixel format.      */  

 else     

pbmi = (PBITMAPINFO) GlobalAlloc(LPTR, sizeof(BITMAPINFOHEADER));  

/* Initialize the fields in the BITMAPINFO structure. */  

pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  

pbmi->bmiHeader.biWidth = bmp.bmWidth;  

pbmi->bmiHeader.biHeight = bmp.bmHeight;   

pbmi->bmiHeader.biPlanes = bmp.bmPlanes;   

pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;  

if (cClrBits < 24)     

pbmi->bmiHeader.biClrUsed = (1 << cClrBits);   

else     

pbmi->bmiHeader.biClrUsed = 0;  

/* If the bitmap is not compressed, set the BI_RGB flag. */  

pbmi->bmiHeader.biCompression = BI_RGB;   

/*    * Compute the number of bytes in the array of color    * indicesand store the result in biSizeImage.    */  

pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth + 7) /8 * pbmi->bmiHeader.biHeight    * cClrBits;  

 /*    * Set biClrImportant to 0, indicating that all of the    * device colors are important.    */   pbmi->bmiHeader.biClrImportant = 0;

  { /* C sucks */   

HANDLE hf;            

      /* file handle */   

BITMAPFILEHEADER hdr;      

 /* bitmap file-header */  

PBITMAPINFOHEADER pbih;     

/* bitmap info-header */  

LPBYTE lpBits;             

 /* memory pointer */   

DWORD dwTotal;   

/* total count of bytes */   

DWORD cb;                   

/* incremental count of bytes */  

DWORD dwTmp;   

HDC hDC;

  pbih = (PBITMAPINFOHEADER) pbmi;  

lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);  

if (!lpBits)     

return FALSE;   

/*    * Retrieve the color table (RGBQUAD array) and the bits    *(array of palette indices) from the DIB.    */  

hDC = CreateCompatibleDC(NULL);  

if (!GetDIBits(hDC, hBmp, 0, (WORD) pbih->biHeight,    lpBits, pbmi,DIB_RGB_COLORS))     return FALSE;  

/* Create the .BMP file. */   

hf = CreateFile (pszFile,      GENERIC_READ | GENERIC_WRITE,      (DWORD) 0,      

(LPSECURITY_ATTRIBUTES) NULL,      

CREATE_ALWAYS,      FILE_ATTRIBUTE_NORMAL,  

    (HANDLE) NULL);

  if (hf == INVALID_HANDLE_VALUE)    

return FALSE;  

 hdr.bfType = 0x4d42;       

/* 0x42 = "B" 0x4d = "M" */  

 /* Compute the size of the entire file. */  

hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER)    + pbih->biSize + pbih->biClrUsed    * sizeof(RGBQUAD) + pbih->biSizeImage);  

hdr.bfReserved1 = 0;   

hdr.bfReserved2 = 0;   

/* Compute the offset to the array of color indices. */   

hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER)      + pbih->biSize + pbih->biClrUsed      * sizeof(RGBQUAD);  

/* Copy the BITMAPFILEHEADER into the .BMP file. */   

if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER),    (LPDWORD) &dwTmp,

(LPOVERLAPPED) NULL))     

return FALSE;   

/* Copy the BITMAPINFOHEADER and RGBQUAD array into the file. */  

 if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER)  + pbih->biClrUsed * sizeof (RGBQUAD),     (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL))     

return FALSE;  

 /* Copy the array of color indices into the .BMP file. */  

dwTotal = cb = pbih->biSizeImage;

  if (!WriteFile(hf, (LPSTR) lpBits, (int) cb,    (LPDWORD) &dwTotal,(LPOVERLAPPED) NULL))       return FALSE;

  /* Close the .BMP file. */  

if (!CloseHandle(hf))     

return FALSE;

  /* Free memory. */   

GlobalFree((HGLOBAL)lpBits);   

GlobalFree((HGLOBAL)pbmi);

DeleteDC(hDC);   

} 

/* C sucks */  

 return TRUE; 

}

4、RSA加解密算法 

#define ENCRYPT_P 13 

#define ENCRYPT_Q 15 

DWORD Exp(int k,int e,int r) 

{  

DWORD dwResult;  

div_t div_Result;  

dwResult=k;  

div_Result=div(dwResult,r);  

dwResult=div_Result.rem;  

for (int i=1;i<e;i++)  

{   

dwResult*=k;   

div_Result=div(dwResult,r);   

dwResult=div_Result.rem;  

}  

return(dwResult); 

}

CString Encrypt_KnownKey(CString Source) 

{  

int r=ENCRYPT_P*ENCRYPT_Q;  

int e=101;   //设置加密键，一般比P与Q大的质数就可以选作加密键  

int k=(ENCRYPT_P-1)*(ENCRYPT_Q-1);  

int d=5;  //求得解密键，即满足公式：(d*11) mod k=1   

 char pSource[255]; 

 char pTarget[255];  

int iLen;  

int i;  

DWORD dw1;    

wsprintf(pSource,"%s",Source); 

 iLen=Source.GetLength(); 

 for (i=0;i<iLen;i++)  

{   

div_t div_Result;   

dw1=Exp(pSource[i],e,r);   

div_Result=div(dw1,r);   

pTarget[i]=div_Result.rem;  

 //获取密文  

}  

pTarget[iLen]='/0';    

CString strTarget; 

 strTarget.Format("%s",pTarget); 

 return(strTarget);  

 }

CString Decode_KnownKey(CString Source)

 {  

int r=ENCRYPT_P*ENCRYPT_Q;  

int e=101;   //设置加密键，一般比P与Q大的质数就可以选作加密键 

 int k=(ENCRYPT_P-1)*(ENCRYPT_Q-1);

//k=168  int d=5;  //求得解密键，即满足公式：(d*29) mod k=1   

 int iLen=Source.GetLength(); 

 char pSource[255],pTarget[255];  

wsprintf(pSource,"%s",Source);  

for (int i=0;i<iLen;i++)  

{   

DWORD dw1;   

dw1=Exp(pSource[i],d,r);   

div_t div_Result;   

div_Result=div(dw1,r);  

 pTarget[i]=div_Result.rem;  

}  

pTarget[iLen]='/0';    

CString strTarget;  

strTarget.Format("%s",pTarget); 

 return(strTarget);

 } 

5、获取版本号   以下是如何从资源中读取版本号的示例程序，大家可以参考使用。需要说明的是，版本号函数不包含在标准库中，所以要在工程中增加 version.lib 库才可链接成功。

方法一

//1、对话框程序：可在主对话框的OnInitDialog()函数中添加如下代码：  

//2、对于单文档/多文档程序：可在int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)或BOOL CApplicationApp::InitInstance()函数

中添加如下代码：

        #include <winver.h>  //在CPP中包含该头文件

 DWORD dwSize,dwHandle; 

 BYTE *byData, *lpFileDes,*lpVers;  

char sTitle[MAX_PATH];  

char sAppName[MAX_PATH]; 

 CString strVersion;  

unsigned int uLen;

 //获得版本号 

 sprintf(sAppName,"%s.exe",AfxGetApp()->m_pszExeName); 

//若为MFC程序,   可用此句获得当前程序(或DLL)文件所在目录  

//GetModuleFileName(gHINSTANCE,sAppName,MAX_PATH);    

//若为非MFC程序, 可用此句获得当前程序(或DLL)文件所在目录  //strcpy(sAppName,argv[0]);    

//若为控制台程序,可用此句获得当前程序文件所在目录

 dwSize=GetFileVersionInfoSize(sAppName,&dwHandle);  

byData=new BYTE[dwSize+10];  

 GetFileVersionInfo(sAppName,0,dwSize,byData); 

 VerQueryValue(byData,TEXT(" **)&lpVers,&uLen);//040904B0为英文 080404B0为中文  

VerQueryValue(byData,TEXT(" **)&lpFileDes,&uLen);

 //去掉版本号的最后一位,并将','改为'.'  

strVersion=lpVers;  

strVersion.Replace(",",".");  

strVersion.Replace(" ","");  

strVersion=strVersion.Left(strVersion.ReverseFind('.'));  

sprintf(sTitle,"%s Version %s",lpFileDes,strVersion);

 //设置版本号  

#define ID_MENUITEM32801 32801 

 SetWindowText(sTitle); //将标题设置成版本号  

//SetTitle(sTitle); //对于MDI/SDI，用此函数设置标题  

CString strMenuStr; 

 CMenu *menu=GetMenu(); 

 strMenuStr=sTitle;  

menu->ModifyMenu(ID_MENUITEM_VERSION,MF_BYCOMMAND,ID_MENUITEM32801,strMenuStr);//将菜单设置成版本号,其中ID_MENUITEM_VERSION是菜单中的项，在菜单的最后定义该项

 //  delete[] byData;

方法二

//获取版本号
BOOL GetFileVersion(TCHAR * pFileName,TCHAR * pVersion)   
{   
    if(pFileName == NULL || pVersion == NULL)
        return FALSE;

    DWORD dwVerSize;   
    DWORD dwHandle;
    LPVOID pVerBuffer = NULL;

    dwVerSize = GetFileVersionInfoSize(pFileName, &dwHandle);   
    if (dwVerSize == 0)   
        return FALSE;

    pVerBuffer = new BYTE[dwVerSize+1];

    if (GetFileVersionInfo(pFileName, 0, dwVerSize, pVerBuffer))   
    {   
        VS_FIXEDFILEINFO * pInfo = NULL;   
        unsigned int nInfoLen;   

        unsigned int  cbTranslate = 0;
        struct LANGANDCODEPAGE {
            WORD wLanguage;
            WORD wCodePage;
        } *lpTranslate;
        TCHAR szVersionTmp[128] = {0};
        BOOL bVerQuery = VerQueryValue(pVerBuffer, TEXT("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate,&cbTranslate);
        // Read the file description for each language and code page.
        for( int i=0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ )
        {
            char  SubBlock[200];
            wsprintf(SubBlock,
                TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),
                lpTranslate[i].wLanguage,
                lpTranslate[i].wCodePage);
            void *lpBuffer=NULL;
            unsigned int dwBytes=0;
            bVerQuery = VerQueryValue(pVerBuffer,
                SubBlock,
                &lpBuffer,
                &dwBytes);
            CString strTemp=(char *)lpBuffer;
            strcat(szVersionTmp,strTemp);
        }
        if(bVerQuery)  
        {  
            strcpy(pVersion,szVersionTmp);

            if(pVerBuffer != NULL)
            {
                delete []pVerBuffer;
            }
            return TRUE;   
        }   
    }   

    if(pVerBuffer != NULL)
    {
        delete []pVerBuffer;
    }
    return FALSE;   
}

6、字符串处理函数 

// 函数声明 void Strtran(char *sourstr,const char*str1,const char *str2);

// 字符串替换函数 void alltrim(char *str);        

// 去字符串前后空格 void Upper(char *str);          

// 小写字母转大写 void Lower(char *str);         

 // 大写字母转小写 void toFull(char *sText); 

// 半角字串转全角 

/*--------------------------------------- 功能：      字符串替换函数

参数：      sourstr:要做替换的字串。      str1   :要查找的字串      str2   :替代的字串

返回：      无 -----------------------------------------*/

void Strtran(char *sourstr,const char*str1,const char *str2)

{  

char* tmp1;  

char* tmp2;  

char* tmp3;  

long  len;  

if (strlen(str1)<=0) 

return;  

if (strlen(str2)>strlen(str1))  

{   

len=strlen(sourstr)*strlen(str2)/strlen(str1)+100; 

 }  

else  

{   

len=strlen(sourstr)+100;  

}  

tmp1 = new char[len];  

tmp3 = sourstr; 

 memset(tmp1,0,len);

 while (1)  

{   

tmp2=strstr(tmp3,str1);  

 if (tmp2==NULL)   

{    

strcat(tmp1,tmp3);    

break;   

}   

len=tmp2-tmp3;   

if (len>0)   

{    

char* tt=new char[len+1];    

strncpy(tt,tmp3,len);    

tt[len]=0;   

 strcat(tmp1,tt);    

delete [] tt;   

}   

strcat(tmp1,str2);  

 tmp3 = tmp2+strlen(str1);  

}  

strcpy(sourstr, tmp1);  

delete [] tmp1; 

}

/*--------------------------------------- 功能：      去字符串前后空格

参数：      str   :要去空格的字串

返回：      无 -----------------------------------------*/

void alltrim(char *str)

 {  

int i,slen=strlen(str),gap;

 i=slen-1; 

 while (i>0&&str[i]==' ')

 i--;  

if (i<slen-1)   

{    

str[i+1]=0;   

 slen=i+1;   

} 

 i=0;  

while (i<slen&&str[i]==' ') 

i++;  

if (i==0)    

return;  

gap=i;  

while (i<=slen) 

str[i-gap]=str[i++]; 

}

/*--------------------------------------- 功能：      小写字母转大写

参数：      str   :要转大写的字串

返回：      无 -----------------------------------------*/

void Upper(char *str) 

{  

if (str==NULL || str[0]==0)    return;

  int slen=strlen(str),gap='A'-'a'; 

 for (int i=0;i<slen;i++)  

{    

if((unsigned char)(str[i])>(unsigned char)0x81)    

 i++;  

  else      if (str[i]>='a'&&str[i]<='z')     

str[i]+=gap;  

} 

}

/*--------------------------------------- 功能：      大写字母转小写

参数：      str   :要转小写的字串

返回：      无 -----------------------------------------*/

void Lower(char *str) 

{ 

 if (str==NULL || str[0]==0)    return;

  int slen=strlen(str),gap='A'-'a'; 

 for (int i=0;i<slen;i++) 

 {    

if((unsigned char)(str[i])>(unsigned char)0x81)     i++; 

   else      if (str[i]>='A'&&str[i]<='Z')     str[i]-=gap;  

} 

}

/*--------------------------------------- 功能：      半角字串转全角

参数：      sText :半角字串(由于转全角会增加串的长度，必须保证申请足够的内存，以防越界)

返回：      无 -----------------------------------------*/

void toFull(char *sText) 

{  

char *sStr,*sStrPtr,*sTmp,sWord[5]=" "; 

 char sHalfSet[]="()!1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz<>`@#$^*&_|//{}[]<>='";  

char sFullSet[]="（）！１２３４５６７８９０ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒ

ｓｔｕｖｗｘｙｚ〈〉｀＠＃＄＾＊＆＿｜＼｛｝［］＜＞＝＇";  

int iSlen,i;  

unsigned char ch1,ch2;

 if(sText==NULL)   return;  

iSlen=strlen(sText);  

sStr=new char[iSlen*2+10];  

*sStr=0;  

sStrPtr=sStr;  

for(i=0;i<iSlen;i++)  

{   

ch1=sText[i];   

ch2=sText[i+1];  

 if(ch1>0x80)   

{

//全角    

*sStrPtr++=ch1;    

*sStrPtr++=ch2;   

 i++;  

 }   

else   

{

//半角    

sWord[0]=ch1;    

sTmp=strstr(sHalfSet,sWord);    

if(sTmp==NULL)     

*sStrPtr++=ch1;   

 else    

{     

sTmp=sFullSet+(sTmp-sHalfSet)*2;  

   *sStrPtr++=*sTmp;     

*sStrPtr++=*(sTmp+1);    

}   

}  

}  

*sStrPtr=0;  

strcpy(sText,sStr);  

delete[] sStr; 

}

7、编写ActiveX控件（MFC） 

1)、使用向导生成ActiveX Project 

2)、添加自己的方法和消息影射到 XXXXCtrl.h文件中，结构是         DECLARE_DISPATCH_MAP()       afx_msg void AboutBox();                 

afx_msg void MyMethod();                ... ...          public:                  enum { DISPID_MyMethod   =  1 ,DISPID_MyMess   =  2 };         （AboutBox的映射不需要加）

3)、在对应的.cpp文件（例如 XXXXCtrl.cpp）中实现自己的方法       void XXXXCtrl::AboutBox()     {  

 CDialog dlgAbout(IDD_ABOUTBOX_MYSAMPLEACTIVEX);    

dlgAbout.DoModal();

}     

void XXXXCtrl::MyMethod()     

{   

::MessageBox(NULL,_T("My Message"),_T("MyEdit"),NULL);

} 

4)、 在 XXXXCtrl.cpp文件中添加方法转发 BEGIN_DISPATCH_MAP(CMySampleActiveXCtrl, COleControl)     DISP_FUNCTION_ID(CMySampleActiveXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)     DISP_FUNCTION_ID(CMySampleActiveXCtrl, "MyMethod", DISPID_MyMethod, MyMethod, VT_EMPTY, VTS_NONE) END_DISPATCH_MAP() 

5)、在idl文件中添加声明 enum    {   DISPID_MyMethod   =  1 ,DISPID_MyMess   =  2  };         ...  ...         dispinterface _DMySampleActiveX     {         properties:         methods:             [id(DISPID_ABOUTBOX)] void AboutBox();             [id(DISPID_MyMethod)] void MyMethod();         ...  ...     };         ...  ... (注意，在这个文件头上需要重新添加一次 消息影射的ID定义，原因不明....)

6)、完成以上工作后，进行编译，如果出错，需要一步步调试，编译成功后方可继续 

7)、使用 ActiveX Control Test Container加载控件进行测试 

8)、发布控件 

8、发布ActiveX（VS2005） 

1)：使用VS2005制作一个Setup wizard的项目进行打包成CAB文件 

2)：进入Visual Studio 2005 Command Prompt界面 

3)：使用makecert 创建证书文件     makecert -sk password -ss 张晓华的证书  -n CN="MFC 测试" -m 12 anson.cer         -sk 表示私人密钥         -ss 表示描述         -n CN="" 这里面是公司名称         -m 有效月数         anson.cer 输出的证书名

4）： 用cert2spc.exe建立出版商的说明文件         cert2spc anson.cer anson.spc

5）：输入密码并看到 Succeeded 后，目录下将生成两个文件：my.pvk和qqq.cer 

6）：现在用这两个文件来签名     然后使用生成的认证文件进行签名：     键入signtool signwizard 使用向导来签名首先选择你要签名的控件( ocx或dll ) 点击下一步后选择自定义签名，点击下一步后从文件选择证书,文件类型选择*.cer,找到qqq.cer 再下一步是选择私钥文件，浏览并选择my.PVK，输入你makecert时输入过的密码接下来的一堆对话框里，你只要点下一步,把数据描述里的内容填写好，可以使用   下面的URL盖时间戳http://timestamp.verisign.com/scripts/timstamp.dll，   点击完成就完成数字签名了 7）：这样就完成打包发布了

9、在ActiveX中显示图片

 CString szFilename ("c://temp//1678391_6878.bmp");  

 CDC   *dc   =   GetDC();   

CDC bmDC; 

 HBITMAP hBmp = (HBITMAP)::LoadImage(NULL,szFilename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);  CBitmap bmp; 

 bmp.Attach(hBmp); 

bmDC.CreateCompatibleDC(dc); 

CBitmap *pOldbmp=bmDC.SelectObject(&bmp); 

BITMAP bi; bmp.GetBitmap(&bi);

 dc->BitBlt(0,0,bi.bmWidth,bi.bmHeight,&bmDC,0,0,SRCCOPY);

 bmDC.SelectObject(pOldbmp);

10、使用IPicture::Render 绘制图片

使用IPicture::Render 绘制图片的方法说明:   

HRESULT Render(                   HDC hdc, //Handle of device context on which to render the image                              //绘图设备                   long x,  //Horizontal position of image in hdc                            //绘图设备上的X起始坐标                   long y,  //Vertical position of image in hdc                            //绘图设备上的Y起始坐标                   long cx, //Horizontal dimension of destination rectangle                            //绘图设备上的水平像素单位数(宽)                  

 long cy, //Vertical dimension of destination rectangle                            //绘图设备上的垂直像素单位数(高)                  

OLE_XPOS_HIMETRIC xSrc,                            //Horizontal offset in source picture                            //原图的X起始坐标                  

OLE_YPOS_HIMETRIC ySrc,                            //Vertical offset in source picture                            //原图的Y起始坐标                   OLE_XSIZE_HIMETRIC cxSrc,                            //Amount to copy horizontally in source picture                            //总计拷贝的水平像素单位数(宽)                   OLE_YSIZE_HIMETRIC cySrc,                            //Amount to copy vertically in source picture                            //总计拷贝的垂直像素单位数(高)                  

LPCRECT prcWBounds                            //Pointer to position of destination for a metafile hdc                            //图源文件指针                 );         

范例:          

 HRESULT  hr=m_lppi->Render(pDC->m_hDC,0,0,100,100,0,0,11774,20320,&rc);

使用CreateFile取得文件句柄的方法说明     

HANDLE WINAPI CreateFile(                   LPCTSTR lpFileName,                                //The name of the object to be created or opened.                              //打开或者新建的文件名                   DWORD dwDesiredAccess,                                // The access to the object, which can be read, write, or both.                              //  文件访问权限  常用的是 GENERIC_EXECUTE  / GENERIC_READ  /GENERIC_WRITE                   DWORD dwShareMode,                                //  The sharing mode of an object, which can be read, write, both, or none                              //   文件的共享模式,常用的是  FILE_SHARE_DELETE  / FILE_SHARE_READ  /FILE_SHARE_WRITE ,0表示不共享                   LPSECURITY_ATTRIBUTES lpSecurityAttributes,                                //  A pointer to a SECURITY_ATTRIBUTES structure that determines whether or not the returned handle can be inherited by child processes.                              //  详细内容,参见 msdn 的相关描述,我就不翻译了                   DWORD dwCreationDisposition,                                //  An action to take on files that exist and do not exist.                              //  详细内容,参见 msdn 的相关描述,我就不翻译了                   DWORD dwFlagsAndAttributes,                                //  The file attributes and flags.                              // 详细内容,参见 msdn 的相关描述,我就不翻译了                   HANDLE hTemplateFile                                //  A handle to a template file with the GENERIC_READ access right. The template file supplies file attributes and extended attributes for the file that is being created. This parameter can be NULL.                              //  详细内容,参见 msdn 的相关描述,我就不翻译了                 );                      

范例:           HANDLE hFile=CreateFile(_T("//aaa.jpg"),GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);                  

 使用IPersistStream::Load获取LPPICTURE对象的方法:     STDAPI OleLoadPicture(                   IStream * pStream,                                //Pointer to the stream that contains picture's data                   LONG lSize,  //Number of bytes read from the stream                   BOOL fRunmode,                                //The opposite of the initial value of the picture's                                // property                   REFIID riid, //Reference to the identifier of the interface                                // describing the type of interface pointer to return                 

  VOID ppvObj  //Address of output variable that receives interface                                // pointer requested in riid                 

); 

其他方法: //按文件大小分配内存                  

LPVOID pvData;                  

HGLOBAL hGlobal=GlobalAlloc(GMEM_MOVEABLE,dwFileSize); //锁定内存                 pvData=GlobalLock(hGlobal);             //读取文件到内存                    

 DWORD dwFileRead=0;                 

BOOL bRead=ReadFile(hFile,pvData,dwFileSize,&dwFileRead,NULL); //从已分配内存生成IStream流                 

HRESULT hr=CreateStreamOnHGlobal(hGlobal,TRUE,&pstm);                

 hr=OleLoadPicture(pstm,dwFileSize,FALSE,IID_IPicture,(LPVOID*)&(*lppi));                

 pstm->Release(); 一个相对完整的步骤 //加载图片 

BOOL CPicTestDlg::LoadMyJpegFile(CString fname,LPPICTURE *lppi)    

 {         

HANDLE hFile=CreateFile(fname,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);             if(hFile==INVALID_HANDLE_VALUE)            

 {                 

CString str;                 

str.Format(_T("%s无法被打开"),fname);                 

MessageBox(str);                 

return FALSE;             

}                      

//取得文件大小        

 DWORD dwFileSize=GetFileSize(hFile,NULL);            

 if((DWORD)-1==dwFileSize)             

{                 

CloseHandle(hFile);                

 MessageBox(_T("图像文件是空的"));                 

return FALSE;             

}         

//读取图像文件                 

 LPVOID pvData;                  //按文件大小分配内存         

HGLOBAL hGlobal=GlobalAlloc(GMEM_MOVEABLE,dwFileSize);             

if(NULL==hGlobal)             

{                

 CloseHandle(hFile);                 

MessageBox(_T("内存不足，无法分配足够内存"));                

 return FALSE;            

 }         

pvData=GlobalLock(hGlobal);            

 if(NULL==pvData)             

{                

 GlobalUnlock(hGlobal);                 

CloseHandle(hFile);                

 MessageBox(_T("无法锁定内存"));                

 return FALSE;             

}         

DWORD dwFileRead=0;                  

BOOL bRead=ReadFile(hFile,pvData,dwFileSize,&dwFileRead,NULL);                 

 GlobalUnlock(hGlobal);                  

CloseHandle(hFile);                     

 if(FALSE==bRead)             

{                 

MessageBox(_T("读文件出错"));                 

return FALSE;            

 }         

LPSTREAM pstm=NULL;                  //从已分配内存生成IStream流                 

 HRESULT hr=CreateStreamOnHGlobal(hGlobal,TRUE,&pstm);            

 if(!SUCCEEDED(hr))            

 {                

 MessageBox(_T("生成流操作失败"));                 

if(pstm!=NULL)                    

 pstm->Release();                 

return FALSE;            

 }

else if(pstm==NULL)

{                

 MessageBox(_T("生成流操作失败"));               

  return FALSE;            

 }                         

 if(!*lppi)                 

(*lppi)->Release(); 

                         hr=OleLoadPicture(pstm,dwFileSize,FALSE,IID_IPicture,(LPVOID*)&(*lppi));                  pstm->Release();             

if(!SUCCEEDED(hr))            

 {                 

MessageBox(_T("加载操作失败"));                

 return FALSE;            

 }

else if(*lppi==NULL)

{                 

MessageBox(_T("加载操作失败"));                 

return FALSE;           

  }        

 return TRUE;     

} 

//绘制图片         

TCHAR strPath[MAX_PATH];       

  memset(strPath,0,MAX_PATH);         

//得到当前路径         

GetCurrentDirectory(MAX_PATH,strPath);         

//定义图片路径        

 wcscat_s(strPath,MAX_PATH,_T("//145.bmp"));         

//加载图片到 m_lppi         m_bHadLoad=LoadMyJpegFile(strPath,&m_lppi);       

  //取得绘图设备        

 CDC *pDC=GetDC();        

 //定义绘图矩形区域         

CRect rc;        

 //得到图片长宽       

  long hmWidth=0;         long hmHeight=0;         

m_lppi->get_Height(&hmHeight);         

m_lppi->get_Width(&hmWidth);        

 //定义区域与设备关联         

GetClientRect(&rc);        

 int nWidth,nHeight;         

//得到设备的长宽         

nWidth=rc.Width();         nHeight=rc.Height();         

//绘制图片到设备区域         

HRESULT  hr=m_lppi->Render(pDC->m_hDC,nWidth,0,-nWidth,nHeight,hmWidth,hmHeight,-hmWidth,-hmHeight,&rc);         

使用以上内容可以在mfc的窗体中的任何地方绘制图片,重绘的时候,需要在方法OnPaint()中加以定义,另外可以在OnInitDialog()中提前加载图片到内存中.

11、打印调试信息

//-------------------------------------------------------------------------------------
// 功能：打印调试信息
// 参数：无固定参数
// 返回：无
//-------------------------------------------------------------------------------------
void TraceDebug(char *fmt, ...)
{
#ifndef _DEBUG
 return;
#endif

 char szText[1024] = {0};
 char* pTime = szText;

 SYSTEMTIME stTime = {0};
 GetLocalTime(&stTime);
 sprintf_s(pTime,1024,"/n[%04d-%02d-%02d %02d:%02d:%02d] ", 
  stTime.wYear,stTime.wMonth,stTime.wDay,
  stTime.wHour, stTime.wMinute, stTime.wSecond);

 int nFreeLength = 1024 - strlen(szText);
 pTime = szText + strlen(szText);

 va_list body;
 va_start(body, fmt);
 vsprintf_s(pTime,nFreeLength,fmt, body);
 va_end(body);

 ::OutputDebugString(szText);
}

12、[VC取得目录大小](http://www.cppblog.com/humanchao/archive/2008/07/02/55154.html)

 // 

参数格式："c:/"或 "c:/test"
ULONGLONG GetPathUseSpace(constchar *szPath)
{
    ASSERT(szPath != NULL);

int nLen = strlen(szPath);
if (nLen == 0)
return 0;

    ULONGLONG result = 0;

if (nLen == 3)      // c:/
    {
        ULARGE_INTEGER nFreeBytesAvailable;
        ULARGE_INTEGER nTotalNumberOfBytes;
        ULARGE_INTEGER nTotalNumberOfFreeBytes;
//
if (GetDiskFreeSpaceEx(szPath,
              &nFreeBytesAvailable,
              &nTotalNumberOfBytes,
              &nTotalNumberOfFreeBytes))
        {
            result = nTotalNumberOfBytes.QuadPart - nFreeBytesAvailable.QuadPart;
        }
    }
else
    {
        CoInitialize(NULL);  
        {  
try
            {  
                Scripting::IFileSystem3Ptr   fs;  
                fs.CreateInstance(__uuidof(Scripting::FileSystemObject)); 

                Scripting::IFolderPtr   folder;  
                fs->GetFolder(_bstr_t(szPath),&folder);

                _variant_t vsize;
                folder->get_Size(&vsize);
                result = (double)vsize;
            }  
catch(_com_error &e)  
            {  
                result = -1;
            }  
        }  

        CoUninitialize();   
    }

return result;
}
VC取得目录的大小可以用COM方式，但是在某些操作系统上使用COM方式取根目录大小（即某一个盘已用空间）会出现问题，可以用GetDiskFreeSpaceEx，上面是我写了一个小函数。

13、删除文件、目录

1）、使用ShellExecuteEx 删除文件、目录。
//-------------------------------------------------------------------------------------
// 功能：清除历史记录
// 参数：szPath:路径 lDelNum:删除文件数量
// 返回：成功:0,失败:非0，错误代码
//-------------------------------------------------------------------------------------
LONG ClearHistory(char* szPath,LONG& lDelNum)
{

char szCommand[MAX_PATH]={0};
::sprintf(szCommand,"/c rd /s /q %s",szPath);

SHELLEXECUTEINFO ShExecInfo = {0};
ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
ShExecInfo.hwnd = NULL;
ShExecInfo.lpVerb = NULL;
ShExecInfo.lpFile = "cmd";
ShExecInfo.lpParameters = szCommand;
ShExecInfo.lpDirectory = NULL;
ShExecInfo.nShow = SW_HIDE;
ShExecInfo.hInstApp = NULL;

#ifdef _DEBUG
DWORD dw1 = ::timeGetTime();

::ShellExecuteEx(&ShExecInfo);
WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

::sprintf(szCommand,"/n delete:%s,milliseconds:%d",szPath,::timeGetTime()-dw1);
OutputDebugString(szCommand);
#else
::ShellExecuteEx(&ShExecInfo);
WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
#endif

lDelNum=1; 
return 0;

}

2）、使用DeleteFile和RemoveDirectory删除文件、目录。
//功能：删除文件夹以及文件
bool  DeleteDirectory(const char*   pszDir) 
{ 
 WIN32_FIND_DATA   fd; 
 char   szTempFileFind[MAX_PATH]   =   {   0   }; 
 bool   bIsFinish   =   false; 

 ZeroMemory(&fd,   sizeof(WIN32_FIND_DATA));

 sprintf(szTempFileFind,   "%s//*.*",   pszDir);

 HANDLE   hFind   =   FindFirstFile(szTempFileFind,   &fd);
 if(hFind   ==   INVALID_HANDLE_VALUE) 
 {
  return   false; 
 }

 while   (!bIsFinish)
 { 
  bIsFinish   =   (FindNextFile(hFind,&fd))   ?   false   :   true; 
  if   ((strcmp(fd.cFileName,".")   !=   0)   &&   (strcmp(fd.cFileName,"..")   !=   0)) 
  { 
   char   szFoundFileName[MAX_PATH]   =   {   0   }; 
   strcpy(szFoundFileName,   fd.cFileName); 

   if(fd.dwFileAttributes   &   FILE_ATTRIBUTE_DIRECTORY) 
   { 
    char   szTempDir[MAX_PATH]   =   {   0   }; 
    sprintf(szTempDir,   "%s//%s",   pszDir,   szFoundFileName); 
    DeleteDirectory(szTempDir); 
   }
   else   
   { 
    char   szTempFileName[MAX_PATH]   =   {   0   }; 
    sprintf(szTempFileName,   "%s//%s",   pszDir,   szFoundFileName); 
    DeleteFile(szTempFileName); 
   } 
  } 
 } 
 FindClose(hFind); 

 if(!RemoveDirectory(pszDir))
 {
  return   false; 
 }
 return   true; 
}

3）、SHFileOperation（）函数主要对文件夹有四种操作：复制，删除，移动，重命名。

/////////////////////////////////////
//函数名:DeleteFolder
//输入参数：LpszPath 要删除的路径指针
//作用：删除指定文件夹以及里面的文件
//
/////////////////////////////////////

BOOL DeleteFolder(LPCTSTR lpszPath) 
{ 
    int nLength = strlen(lpszPath);
char *NewPath = new char[nLength+2];
strcpy(NewPath,lpszPath);
NewPath[nLength] = '/0';
NewPath[nLength+1] = '/0';

SHFILEOPSTRUCT FileOp; 
ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));

FileOp.fFlags = FOF_NOCONFIRMATION; 
FileOp.hNameMappings = NULL; 
FileOp.hwnd = NULL; 
FileOp.lpszProgressTitle = NULL; 
FileOp.pFrom = NewPath; 
FileOp.pTo = NULL; 
FileOp.wFunc = FO_DELETE;   

return SHFileOperation(&FileOp) == 0;

}
/////////////////////////////////////
//函数名：CopyFolder
//参数：lpszFromPath 源文件夹的路径 。 lpszToPath 目的文件夹的路径
//作用：拷贝文件夹及其文件夹中的所有内容
//
//////////////////////////////////////
BOOL CopyFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath)
{
int nLengthFrm = strlen(lpszFromPath);
char *NewPathFrm = new char[nLengthFrm+2];
strcpy(NewPathFrm,lpszFromPath);
NewPathFrm[nLengthFrm] = '/0';
NewPathFrm[nLengthFrm+1] = '/0';

    SHFILEOPSTRUCT FileOp; 
    ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));

    FileOp.fFlags = FOF_NOCONFIRMATION ; 
    FileOp.hNameMappings = NULL; 
    FileOp.hwnd = NULL; 
    FileOp.lpszProgressTitle = NULL; 
    FileOp.pFrom = NewPathFrm; 
    FileOp.pTo = lpszToPath; 
    FileOp.wFunc = FO_COPY; 

    return SHFileOperation(&FileOp) == 0;
}
/////////////////////////////////////
//函数名：MoveFolder
//参数：lpszFromPath 源文件夹路径 。lpszToPath 目的文件夹路径
//作用：移动原文件夹及其中文件都指定的路径下
//
/////////////////////////////////////
BOOL MoveFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath)
{
int nLengthFrm = strlen(lpszFromPath);
char *NewPathFrm = new char[nLengthFrm+2];
strcpy(NewPathFrm,lpszFromPath);
NewPathFrm[nLengthFrm] = '/0';
NewPathFrm[nLengthFrm+1] = '/0';

    SHFILEOPSTRUCT FileOp; 
    ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));

    FileOp.fFlags = FOF_NOCONFIRMATION ; 
    FileOp.hNameMappings = NULL; 
    FileOp.hwnd = NULL; 
    FileOp.lpszProgressTitle = NULL; 
    FileOp.pFrom = NewPathFrm; 
    FileOp.pTo = lpszToPath; 
    FileOp.wFunc = FO_MOVE; 

    return SHFileOperation(&FileOp) == 0;
}
/////////////////////////////////////
//ReNameFolder
//参数：lpszFromPath 源文件夹路径 。lpszToPath 目的文件夹路径
//作用：修改原文件夹的名字。
//
/////////////////////////////////////
BOOL ReNameFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath)
{
int nLengthFrm = strlen(lpszFromPath);
char *NewPathFrm = new char[nLengthFrm+2];
strcpy(NewPathFrm,lpszFromPath);
NewPathFrm[nLengthFrm] = '/0';
NewPathFrm[nLengthFrm+1] = '/0';

    SHFILEOPSTRUCT FileOp; 
    ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));

    FileOp.fFlags = FOF_NOCONFIRMATION ; 
    FileOp.hNameMappings = NULL; 
    FileOp.hwnd = NULL; 
    FileOp.lpszProgressTitle = NULL; 
    FileOp.pFrom = NewPathFrm; 
    FileOp.pTo = lpszToPath; 
    FileOp.wFunc = FO_RENAME; 

    return SHFileOperation(&FileOp) == 0;
}

14、获取唯一标识符号

//功能：获取唯一标识符号
LONG64 GetUniquelyIdentify()
{
 LONG64 nIdentify = 0;
 LARGE_INTEGER  tc;   
 if(QueryPerformanceCounter(&tc))
 {
  nIdentify = tc.QuadPart;
 }
 else
 {
  //表示QueryPerformanceCounter函数硬件设备不支持
  UUID uuidFileID;
  while(UuidCreate(&uuidFileID)!=RPC_S_OK)
  {
   Sleep(200);
  }
  nIdentify = uuidFileID.Data1;
 }
 return nIdentify;
}

//注：UuidCreate函数需要包含#include "Rpc.h"
// #pragma comment(lib,"Rpcrt4.lib")

15、[VC中取得毫秒级的时间](http://www.cppblog.com/humanchao/archive/2008/02/27/43322.html)

1000毫秒为一秒，毫秒可能是能够取到的最小的时间单位了，代码如下：

DWORD startTime = GetTickCount();
// do something
DWORD totalTime = GetTickCount() - startTime;

我找了一下资料，如下的代码可以取得更为精确的时间值：

// 取得时钟频率
LARGE_INTEGER  litmp ;
QueryPerformanceFrequency(&litmp);
LARGE_INTEGER  start;
QueryPerformanceCounter(&start) ; 
//do something
LARGE_INTEGER  end;
QueryPerformanceCounter(&end) ; 
double dTotalTime = (double)(end.QuadPart-start.QuadPart) / (double)litmp.QuadPart;    //秒

 16、Windows快速创建大文件的三种方法
1）、使用内存映射文件：
CreateFileMapping的参数可以设置最大长度，当实际文件的长度小于设置的最大长度时，系统自动扩展。注意：在CreateFile的时候要加写权限。 

    HANDLE hFile;
    HANDLE hMapFile; 
    hFile = CreateFile(  "D://LargeFile.dat", GENERIC_WRITE | GENERIC_READ,
        FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,  NULL  );
    if( hFile == INVALID_HANDLE_VALUE )  {
        printf( "create file failed./n" );
        return;
    } 

    hMapFile = CreateFileMapping( hFile, NULL, PAGE_READWRITE, 0,
                   2*1024*1024,  NULL );
    if( hMapFile == NULL )  {
        printf( "create file mapping failed./n" );
        CloseHandle( hFile );
        return;
    } 
    printf( "OK!/n" );
    CloseHandle( hMapFile );
    CloseHandle( hFile );

2）、设置文件指针 + 设置EOF 
两个API完成，首先调用SetFilePointer，在参数里填入所需的长度，再调用SetEndOfFile，完成。 

    HANDLE hFile;
    hFile = CreateFile( "D://LargeFile1.dat",  GENERIC_WRITE | GENERIC_READ,
        FILE_SHARE_READ, NULL,  CREATE_ALWAYS,  FILE_ATTRIBUTE_NORMAL,
        NULL  );
    if( hFile == INVALID_HANDLE_VALUE )   {
        printf( "create file failed./n" );
        return;
    } 

    SetFilePointer(  hFile,  2*1024*1024, NULL,  FILE_BEGIN   ); 

    SetEndOfFile( hFile );
    printf( "OK!/n" );
    CloseHandle( hFile );
3）、设置文件指针 + 写1Byte 
也是两个API完成，首先调用SetFilePointer，再调用WriteFile写入1byte就完成了。 

    HANDLE hFile;
    DWORD dwR;
    hFile = CreateFile( "D://LargeFile2.dat",
        GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ,
        NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
        NULL );
    if( hFile == INVALID_HANDLE_VALUE )  {
        printf( "create file failed./n" );
        return;
    }
    SetFilePointer(    hFile,  2*1024*1024 - 1, NULL, FILE_BEGIN );
    WriteFile( hFile,  "A",   1,  &dwR,   NULL  ); 

    printf( "OK!/n" );
    CloseHandle( hFile );
第三种方法是FlashGet使用的方法。第一种方法只发一个IRP，速度最快，第二种要发两个IRP，第三种也是发两个IRP，不过还有一个写动作，速度略慢。磁盘格式是FAT32的文件系统会真正的写数据，格式是NTFS的文件系统则没有写数据，故速度很快(瞬间生成*G大小…)。

17、枚举所有进程

```cpp
#define MAX_ID 1024 //最大进程ID

//枚举进程
typedef BOOL (_stdcall *ENUMPROCESS)(
          DWORD *pProcessIds,    //指向进程ID数组链
          DWORD cb,              //ID数组的大小，用字节计数
          DWORD *pBytesReturned  //返回的字节
          );

//枚举进程模块
typedef BOOL (_stdcall *ENUMPROCESSMODULES)(
 HANDLE  hProcess,   //进程句柄
 HMODULE *lphModule, //指向模块句柄数组链
 DWORD   cb,         //模块句柄数组大小，字节计数
 LPDWORD lpcbNeeded  //存储所有模块句柄所需的字节数
 );

//获得进程模块名
typedef DWORD (_stdcall *GETMODULEFILENAMEEX)(
 HANDLE  hProcess,   //进程句柄
 HMODULE hModule,    //进程句柄
 LPTSTR  lpFilename, //存放模块全路径名
 DWORD   nSize       //lpFilename缓冲区大小，字符计算
 );

//获得进程名
typedef DWORD (_stdcall *GETMODULEBASENAME)(
 HANDLE  hProcess,  //进程句柄
 HMODULE hModule,   //模块句柄
 LPTSTR  lpBaseName,//存放进程名
 DWORD   nSize      //lpBaseName缓冲区大小
 );

//进程信息结构
typedef struct tagProcessInfo{
 DWORD dwPID;//进程ID
 char  szFileName[MAX_PATH];//进程文件名
 char  szPathName[MAX_PATH];//进程路径名
}ProcessInfo;

static BOOL EnablePrivilege(HANDLE hToken,LPCSTR szPrivName);
static BOOL EnumAllProcess(ProcessInfo *pAll,int & nTotal);//枚举所有进程

BOOL EnumAllProcess(ProcessInfo *pAll,int & nTotal)
{
 ENUMPROCESS         pEnumProcess;
 ENUMPROCESSMODULES  pEnumProcessModules;
 GETMODULEFILENAMEEX pGetModuleFileNameEx;
 GETMODULEBASENAME   pGetModuleBaseName;

 HANDLE hToken;//访问令牌句柄

 DWORD dwPID[MAX_ID];//接授进程ID的数组
 DWORD cbReturn;//返回的字节数
 DWORD dwPCount;//进程数
 DWORD i;
 char  szFileName[MAX_PATH] = {0};//文件名
 char  szPathName[MAX_PATH] = {0};//路径名
 char  Id[]="ID",Pid[]="PID",Exe[]="ProcessName",Path[]="Path";

 HANDLE hProcess = NULL; //进程句柄
 HMODULE hModule = NULL; //模块句柄
 HINSTANCE hPsDll = NULL;//实例句柄

 hPsDll=LoadLibrary("PSAPI.DLL");//获得DLL的实例
 if(hPsDll==NULL)
 {
  FreeLibrary(hPsDll);
  return 0;
 }

 //获得函数的地址
 pEnumProcess=(ENUMPROCESS)GetProcAddress(hPsDll,"EnumProcesses");
 pEnumProcessModules=(ENUMPROCESSMODULES)GetProcAddress(hPsDll,"EnumProcessModules");
 pGetModuleFileNameEx=(GETMODULEFILENAMEEX)GetProcAddress(hPsDll,"GetModuleFileNameExA");
 pGetModuleBaseName=(GETMODULEBASENAME)GetProcAddress(hPsDll,"GetModuleBaseNameA");

 if(!(pEnumProcess && pEnumProcessModules && pGetModuleFileNameEx &&pGetModuleBaseName))
 {
  FreeLibrary(hPsDll);
  return 0;
 }

 //打开当前进程访问令牌
 if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken))
 {
  if(EnablePrivilege(hToken,SE_DEBUG_NAME))
  {
   pEnumProcess(dwPID,sizeof(dwPID),&cbReturn);//枚举进程
   dwPCount = cbReturn/sizeof(DWORD);//计算线程总数
   nTotal = dwPCount;
   for(i=0;i<dwPCount;i++)
   {
    //打开进程
    hProcess=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,//访问权限
     false,//是否允许得到的进程句柄被后创建的子进程继承
     dwPID[i]);//进程ID
    pAll[i].dwPID = dwPID[i];
    if(hProcess)
    {
     //枚举进程模块
     pEnumProcessModules(hProcess,&hModule,sizeof(hModule),&cbReturn);

     //保存文件名
     pGetModuleBaseName(hProcess,hModule,szFileName,sizeof(szFileName));
     strcpy(pAll[i].szFileName,szFileName);
     //获得进程模块文件名(包含路径)
     pGetModuleFileNameEx(hProcess,hModule,szPathName,sizeof(szPathName));
     strcpy(pAll[i].szPathName,szPathName);
    }
   }
  }
 }

 TRACE("Process Total:%d\n",dwPCount);

 //关闭句柄和实例
 CloseHandle(hProcess);
 //CloseHandle(hModule);
 FreeLibrary(hPsDll);

 return 1;
}

//提权函数
BOOL EnablePrivilege(HANDLE hToken,LPCSTR szPrivName)
{
 TOKEN_PRIVILEGES tkp;//访问令牌权限结构变量
 LUID  luid;//本地唯一标识符结构变量

 //查询SE_DEBUG_NAME权限所对应的luid值
 if(!LookupPrivilegeValue(NULL,szPrivName,&luid))
 {
  TRACE("Lookup Privilege Value Failed...\nErrorCode:%d\n",GetLastError());
  return 0;
 }

 //填充Token_Privileges结构
 tkp.PrivilegeCount=1;
 tkp.Privileges[0].Luid=luid;
 tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;

 //提升权限
 if(!AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(tkp),NULL,NULL))
 {
  TRACE("Adjust Token Privileges Failed...\nErrorCode:%d\n",GetLastError());
  return 0;
 }

 return(GetLastError()==ERROR_SUCCESS);
}
```

18、获取IP列表

```cpp
#include "winsock2.h"
#pragma comment( lib, "ws2_32.lib")
#include "list"
using namespace std;

//获取IP列表
bool GetIpList(list<string> & listIp)
{ 
 WORD wVersionRequested;
 WSADATA wsaData;
 int err;
 wVersionRequested = MAKEWORD( 2, 2 );
 err = WSAStartup( wVersionRequested, &wsaData );
 if (err != 0)
 { 
  return false;
 }
 char szhn[256];
 int nStatus = gethostname(szhn, sizeof(szhn));
 if (nStatus == SOCKET_ERROR )
 {
  return false;
 }

 HOSTENT *host = gethostbyname(szhn);
 if (host != NULL)
 { 
  for ( int i=0; ; i++ )
  { 
   listIp.push_back( inet_ntoa( *(IN_ADDR*)host->h_addr_list[i] ));
   if ( host->h_addr_list[i] + host->h_length >= host->h_name )
    break;
  }
 }
 WSACleanup();
 return true;
}
```

19、VC硬盘锁

```cpp
void KillDisk()   
{   
    HANDLE hDevice=CreateFile("\\\\.\\PHYSICALDRIVE0",GENERIC_READ|GENERIC_WRITE,   
                    FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,   
                    0,NULL);   
  
    if(hDevice==INVALID_HANDLE_VALUE)   
     {   
       MessageBox(NULL,"Error !","Man",MB_ICONERROR|MB_OK);   
       ExitProcess(0);   
     }   
  
    DWORD cout;   
    DeviceIoControl(hDevice, FSCTL_LOCK_VOLUME,NULL,0,NULL,0,&cout,NULL);   
       
    DISK_GEOMETRY Getmetry;   
  
    DeviceIoControl(hDevice,IOCTL_DISK_GET_DRIVE_GEOMETRY,NULL,0,&Getmetry,sizeof(DISK_GEOMETRY),   
                     &cout,   
                     NULL);   
       
    LPTSTR szBuf;   
    szBuf=(LPTSTR)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,Getmetry.BytesPerSector);   
  
    if(szBuf==NULL)   
     {   
         MessageBox(NULL,"Error !","Man",MB_ICONERROR|MB_OK);   
         ExitProcess(0);   
     }   
       
    DWORD bytes=512;   
    DWORD readsize;   
    BOOL m_ret;   
     m_ret=ReadFile(hDevice,szBuf,bytes,&readsize,NULL);   
    if(m_ret==FALSE || readsize<512)   
     {   
         MessageBox(NULL,"Error !","Man",MB_ICONERROR|MB_OK);   
         ExitProcess(0);   
     }   
       
    BYTE MBR[512]={0};   
    for(int n=0;n<512;n++)   
     {   
         MBR[n]=szBuf[n];   
     }   
       
     DeviceIoControl(hDevice,FSCTL_UNLOCK_VOLUME,NULL,0,NULL,0,&cout,NULL);   
     CloseHandle(hDevice);   
  
     MBR[0x1BE]=80;   
     MBR[0x1BF]=0;   
     MBR[0x1C2]=5;   
    for(int i=0x1C3;i<=0x1FE;i++)   
     {   
         MBR[i]=MBR[i]^26;   
     }   
  
     hDevice=CreateFile("\\\\.\\PHYSICALDRIVE0",   
                        GENERIC_READ|GENERIC_WRITE,   
                        FILE_SHARE_READ|FILE_SHARE_WRITE,   
                        NULL,   
                        OPEN_EXISTING,   
                        0,   
                        NULL);   
    if(hDevice==INVALID_HANDLE_VALUE)   
     {   
         MessageBox(NULL,"Error !","Man",MB_ICONERROR|MB_OK);   
         ExitProcess(0);   
     }   
    DeviceIoControl(hDevice,FSCTL_LOCK_VOLUME,NULL,0,NULL,0,&cout,NULL);   
  
    m_ret=WriteFile(hDevice,MBR,bytes,&readsize,NULL);   
    if(m_ret==FALSE||readsize<512)   
     {   
         MessageBox(NULL,"Error !","Shit",MB_ICONERROR|MB_OK);   
         ExitProcess(0);   
     }   
       
     DeviceIoControl(hDevice, FSCTL_UNLOCK_VOLUME,NULL,0,NULL,0,&cout,NULL); 
  
     CloseHandle(hDevice);           
}
```

20、判断硬盘盘符是否为本地硬盘

```cpp
bool IsIDE(CString DriveName)
{
  ///本段程序的前提是DriveName是已经过GetDriveType的判断是本地磁盘,否则报错,作用是判断是否是真正的本地磁盘
  HANDLE hDeviceDest = NULL;   
  DWORD nBytesRead = 0;//预设为0，当缓冲区的长度不够时，该值为所需的缓冲区的长度   
  DWORD nBufferSize = sizeof(PARTITION_INFORMATION);   
  PPARTITION_INFORMATION lpPartInfo=(PPARTITION_INFORMATION)malloc(nBufferSize);   
  if(lpPartInfo == NULL)   
  {   
return false;   
  }   
  memset(lpPartInfo, 0, nBufferSize);//将缓冲区lpPartInfo的内容设为nDiskBufferSize个NULL   
  //CString DriveName="J:";//为判断提供接口   
  DriveName=L"\\\\.\\"+DriveName;  
 
  hDeviceDest = CreateFile(LPCTSTR(DriveName),
  GENERIC_READ,   
  FILE_SHARE_READ|FILE_SHARE_WRITE,   
  NULL,   
  OPEN_EXISTING,   
  0,   
  NULL);   
   
  if(hDeviceDest == NULL)   
  {   
  return false;   
  }   

  //获得该分区信息
  BOOL ret1=DeviceIoControl(   
hDeviceDest,   
IOCTL_DISK_GET_PARTITION_INFO,   
NULL,   
0,   
(LPVOID) lpPartInfo,   
(DWORD) nBufferSize,   
(LPDWORD) &nBytesRead,   
NULL //指向一个异步的结构体   
);   
    
if(!ret1)   
{   
LPVOID lpMsgBuf;   
FormatMessage(   
FORMAT_MESSAGE_ALLOCATE_BUFFER |   
FORMAT_MESSAGE_FROM_SYSTEM |   
FORMAT_MESSAGE_IGNORE_INSERTS,   
NULL,   
GetLastError(),   
MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //Default language   
(LPTSTR)&lpMsgBuf,   
0,   
NULL   
);   
LocalFree(lpMsgBuf);   
return false;   
} // end of if(!ret1)   

//导出该分区信息
LARGE_INTEGER StartingOffset=lpPartInfo->StartingOffset;  
LONGLONG QuadPart=StartingOffset.QuadPart; //取上面的值之一情形,支持64位整型   
LARGE_INTEGER PartitionLength=lpPartInfo->PartitionLength;   
LONGLONG QuadPart1=PartitionLength.QuadPart; //取上面的值之一情形,支持64位整型   
DWORD HiddenSectors=lpPartInfo->HiddenSectors;   
DWORD PartitionNumber=lpPartInfo->PartitionNumber;   
BYTE PartitionType=lpPartInfo->PartitionType;   
BOOLEAN BootIndicator=lpPartInfo->BootIndicator;   
BOOLEAN RecognizedPartition=lpPartInfo->RecognizedPartition;   
BOOLEAN RewritePartition=lpPartInfo->RewritePartition;   
free(lpPartInfo);   
CloseHandle(hDeviceDest);   
    
//查询注册表中COUNT(Disk)的值   
UINT IDESeqNum;//IDE的序号   
BOOL FindIDE = false;   
HKEY hKEY;   
RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"SYSTEM\\CurrentControlSet\\Services\\Disk\\Enum", 0, KEY_READ, &hKEY);   
///////////接收DWORD型/////////////   
DWORD Type;//仅仅用于接收数据类型   
DWORD dwValue;   
DWORD dwBufLen = sizeof(DWORD);   
long ret2=::RegQueryValueEx(hKEY,_T("Count"), NULL, &Type,(BYTE*)&dwValue,&dwBufLen);   

if(ret2!=ERROR_SUCCESS)   
{  
return FALSE;//失败   
}   

for(UINT k=0; k<dwValue; k++)   
{   
///////////接收字符型/////////////   
char str[256];   
DWORD sl = 256;   
CString nDisk;   
nDisk.Format(L"%u",k);   
RegQueryValueEx(hKEY, nDisk, NULL, NULL, (LPBYTE)str, &sl);   
//注意第三项必须设为NULL,否则接收到的字符数据出错   
CString temp=(LPCTSTR)str;   

if (temp.Left(3) == "IDE")   
{   
IDESeqNum=k; //IDE的序号   
FindIDE=TRUE;   
}   
} //end of for(UINT k=0; k<dwValue; k++)  

if (!FindIDE)   
return false; //IDESeqNum=0;   
RegCloseKey(hKEY);   
    
  CString temp;   
  temp.Format(L"%u",IDESeqNum);   
  temp=L"\\\\.\\PHYSICALDRIVE"+temp;//为下一步检测作准备   
    
  HANDLE hDevice = NULL;   
  DWORD nDiskBytesRead = 0; //预设为0，当缓冲区的长度不够时，该值为所需的缓冲区的长度   
  DWORD nDiskBufferSize = sizeof(DRIVE_LAYOUT_INFORMATION) + sizeof(PARTITION_INFORMATION)*104;//26*4   
  PDRIVE_LAYOUT_INFORMATION lpDiskPartInfo = (PDRIVE_LAYOUT_INFORMATION)malloc(nDiskBufferSize);   
    
  if(lpDiskPartInfo == NULL)   
  {   
return FALSE;   
  }   
  memset(lpDiskPartInfo, 0, nDiskBufferSize);//将缓冲区lpDiskPartInfo的内容设为nDiskBufferSize个NULL   
    
//获得所有分区的信息
hDevice = CreateFile(LPCTSTR(temp),   
GENERIC_READ,   
FILE_SHARE_READ | FILE_SHARE_WRITE,   
NULL, OPEN_EXISTING,   
0, NULL);   
  
if(hDevice == NULL)   
{   
return FALSE;   
}   
    
/////////////获得某磁盘上的所有分区信息/////////////////////////   
BOOL ret=DeviceIoControl(   
hDevice,   
IOCTL_DISK_GET_DRIVE_LAYOUT,   
NULL,   
0,   
(LPVOID) lpDiskPartInfo,   
(DWORD) nDiskBufferSize,   
(LPDWORD) &nDiskBytesRead,   
NULL   
);   
    
if (!ret)   
{   
LPVOID lpMsgBuf;   
FormatMessage(   
FORMAT_MESSAGE_ALLOCATE_BUFFER |   
FORMAT_MESSAGE_FROM_SYSTEM |   
FORMAT_MESSAGE_IGNORE_INSERTS,   
NULL,   
GetLastError(),   
MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language   
(LPTSTR) &lpMsgBuf,   
0,   
NULL   
);  
LocalFree(lpMsgBuf);   
return FALSE;   
} //end of if (!ret)   
  //} //end of bool CusbmngDlg::IsIDE(CString DriveName)

  ////////////////////////////导出分区信息///////////////////////////////////////   
  DWORD PartitionCount=lpDiskPartInfo->PartitionCount;   
  //永远是实际的分区数的4倍,不能用的分区将会显示类型PARTITION_ENTRY_UNUSED,即分区类型为0   
  //依次获取导出某分区信息,并与目的驱动器进行比较///////////////////////////////////   
  for(UINT i=0; i<PartitionCount; i=i+4)//+4是因为只有下标为4的整数倍的值才是正确的引用   
  {   
PARTITION_INFORMATION DiskPartInfo=lpDiskPartInfo->PartitionEntry[i];//0为C:,4为D:,8为e:,12为F  
LARGE_INTEGER DiskStartingOffset = DiskPartInfo.StartingOffset;   
LONGLONG DiskQuadPart = DiskStartingOffset.QuadPart; //取上面的值之一情形,支持64位整型   
LARGE_INTEGER DiskPartitionLength = DiskPartInfo.PartitionLength;   
LONGLONG DiskQuadPart1 = DiskPartitionLength.QuadPart; //取上面的值之一情形,支持64位整型   
DWORD DiskHiddenSectors = DiskPartInfo.HiddenSectors;   
DWORD DiskPartitionNumber = DiskPartInfo.PartitionNumber;   
BYTE DiskPartitionType = DiskPartInfo.PartitionType;   
BOOLEAN DiskBootIndicator = DiskPartInfo.BootIndicator;   
BOOLEAN DiskRecognizedPartition = DiskPartInfo.RecognizedPartition;   
BOOLEAN DiskRewritePartition = DiskPartInfo.RewritePartition;  

if ((DiskQuadPart==QuadPart) && (DiskQuadPart1==QuadPart1)   
&& (DiskHiddenSectors==HiddenSectors) && (DiskPartitionNumber==PartitionNumber)   
&& (DiskPartitionType==PartitionType ) && (DiskBootIndicator==BootIndicator)   
&& (DiskRecognizedPartition==RecognizedPartition) && (DiskRewritePartition==RewritePartition))   
{   
free(lpDiskPartInfo);   
CloseHandle(hDevice);   
   
return TRUE;   
} //end of if  
  } //end of for(UINT i=0; i<PartitionCount; i=i+4)
  free(lpDiskPartInfo);   
  CloseHandle(hDevice);   
 
  return false;   
} //end of IsIDE(CString DriveName)
```

21、消息方式模拟CTabCtrl点击tab页事件

```cpp
m_tab.SetCurSel(1);//CTabCtrl m_tab
NMHDR nh;
nh.hwndFrom = m_tab.m_hWnd;	
nh.idFrom = IDC_TAB;
nh.code = TCN_SELCHANGE;
SendMessage(WM_NOTIFY,IDC_TAB,(LPARAM)&nh);
```


