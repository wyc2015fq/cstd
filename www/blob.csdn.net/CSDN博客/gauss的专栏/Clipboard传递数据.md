# Clipboard传递数据 - gauss的专栏 - CSDN博客
2011年12月27日 23:09:42[gauss](https://me.csdn.net/mathlmx)阅读数：333
在window下面不同的进程之间可以利用剪切板传递文本，图片等数据.
API讲解
打开剪切板
BOOL OpenClipboard(      
    HWND *hWndNewOwner*
);- *hWndNewOwner是剪切板关联的窗口，一般设为空就可以了。*
关闭剪切板
- BOOL CloseClipboard(  VOID );
清空剪切板内容
BOOL EmptyClipboard(      
    VOID 
);
从剪切板获取数据
HANDLE GetClipboardData(      
    UINT *uFormat*
);
设置数据给剪切板
HANDLE SetClipboardData(      
    UINT *uFormat*,
    HANDLE *hMem*
);*uFormat*
数据格式类型一般有下列类型
/*
 * Predefined Clipboard Formats
 */
#define CF_TEXT             1
#define CF_BITMAP           2
#define CF_METAFILEPICT     3
#define CF_SYLK             4
#define CF_DIF              5
#define CF_TIFF             6
#define CF_OEMTEXT          7
#define CF_DIB              8
#define CF_PALETTE          9
#define CF_PENDATA          10
#define CF_RIFF             11
#define CF_WAVE             12
#define CF_UNICODETEXT      13
#define CF_ENHMETAFILE      14
#if(WINVER >= 0x0400)
#define CF_HDROP            15
#define CF_LOCALE           16
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0500)
#define CF_DIBV5            17
#endif /* WINVER >= 0x0500 */
#define CF_OWNERDISPLAY     0x0080
#define CF_DSPTEXT          0x0081
#define CF_DSPBITMAP        0x0082
#define CF_DSPMETAFILEPICT  0x0083
#define CF_DSPENHMETAFILE   0x008E
/*
 * "Private" formats don't get GlobalFree()'d
 */
#define CF_PRIVATEFIRST     0x0200
#define CF_PRIVATELAST      0x02FF
/*
 * "GDIOBJ" formats do get DeleteObject()'d
 */
#define CF_GDIOBJFIRST      0x0300
#define CF_GDIOBJLAST       0x03FF
1:拷贝文本数据到剪切板
void copyToClipboard(const char* text) const
{
    if (strlen(text)==0)
        return;
    if (!OpenClipboard(NULL) || text == 0)
        return;
    EmptyClipboard();
    HGLOBAL clipbuffer;
    char * buffer;
    clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(text)+1);
    buffer = (char*)GlobalLock(clipbuffer);
    strcpy_s(buffer, text);
    GlobalUnlock(clipbuffer);
    SetClipboardData(CF_TEXT, clipbuffer);
    CloseClipboard();
}
这里只是简单举例文本数据，其它类型产不多一样
2：从剪切板读取数据
const char* getTextFromClipboard() 
{
    if (!OpenClipboard(NULL))
        return 0;
    char * buffer = 0;
    HANDLE hData = GetClipboardData( CF_TEXT );
    buffer = (char*)GlobalLock( hData );
    GlobalUnlock( hData );
    CloseClipboard();
    return buffer;
}
