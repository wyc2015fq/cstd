
# GetGlyphOutline函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 11:09:54[seven-soft](https://me.csdn.net/softn)阅读数：286


中西文化的差异，导致在电子信息里处理也大不相同，在英文里只需要26个字母就可以显示所有文章了，而在中文里需要最基本的字符就有2000多个。对于一些在嵌入式软件里要显示的字符，那么就得手动去构造所有图形，这是一个比较大的工作量，如果让每个厂家都去完成这个任务，显然是不可能的。面对着大量嵌入式用户的需求，那么就需要解决中文字模的图形问题。毕竟大家经常使用Windows，最先想到的，肯定是怎么样把里面的字符提取图形出来，生成自己需要的几个字库。下面就来介绍怎么样用函数GetGlyphOutline获取显示字符的图形数据。
函数GetGlyphOutline声明如下：
WINGDIAPI DWORD WINAPI GetGlyphOutlineA(    __in HDC hdc,
__in UINT uChar,
__in UINT fuFormat,
__out LPGLYPHMETRICS lpgm,
__in DWORD cjBuffer,
__out_bcount_opt(cjBuffer) LPVOID pvBuffer,
__in CONST MAT2 *lpmat2
);
WINGDIAPI DWORD WINAPI GetGlyphOutlineW(    __in HDC hdc,
__in UINT uChar,
__in UINT fuFormat,
__out LPGLYPHMETRICS lpgm,
__in DWORD cjBuffer,
__out_bcount_opt(cjBuffer) LPVOID pvBuffer,
__in CONST MAT2 *lpmat2
);
\#ifdef UNICODE
\#define GetGlyphOutline GetGlyphOutlineW
\#else
\#define GetGlyphOutline GetGlyphOutlineA
\#endif // !UNICODE
**hdc**是设备句柄。
**uChar**是需要获取图形数据的字符。
**fuFormat**是获取数据的格式。
**lpgm**是获取字符的相关信息。
**cjBuffer**是保存字符数据的缓冲区大小。
**pvBuffer**是保存字符数据的缓冲区。
**lpmat2**是3*3的变换矩阵。
调用函数的例子如下：
\#001 //浮点数据转换为固定浮点数。
\#002  FIXED FixedFromDouble(double d)
\#003  {
\#004         long l;
\#005          l = (long) (d * 65536L);
\#006         return *(FIXED *)&l;
\#007  }
\#008
\#009  //设置字体图形变换矩阵。
\#010  void SetMat(LPMAT2 lpMat)
\#011  {
\#012         lpMat->eM11 = FixedFromDouble(2);
\#013         lpMat->eM12 = FixedFromDouble(0);
\#014         lpMat->eM21 = FixedFromDouble(0);
\#015         lpMat->eM22 = FixedFromDouble(2);
\#016  }
\#017
\#018  //
\#019  //获取字模信息。
\#020  //
\#021  void TestFontGlyph(void)
\#022  {
\#023         //创建字体。
\#024         HFONT hFont = GetFont();
\#025
\#026         //设置字体到当前设备。
\#027         HDC hDC = ::GetDC(m_hWnd);
\#028         HFONT hOldFont = (HFONT)SelectObject(hDC,hFont);
\#029
\#030         //设置字体图形变换矩阵
\#031         MAT2 mat2;
\#032         SetMat(&mat2);
\#033
\#034
\#035         GLYPHMETRICS gm;
\#036
\#037         //设置要显示的字符。
\#038         TCHAR chText = L'蔡';
\#039
\#040         //获取这个字符图形需要的字节的大小。
**\#041        DWORD dwNeedSize = GetGlyphOutline(hDC,chText,GGO_BITMAP,&gm,0,NULL,&mat2);**
\#042         if (dwNeedSize > 0 && dwNeedSize < 0xFFFF)
\#043         {
\#044               //按需要分配内存。
\#045               LPBYTE lpBuf = (LPBYTE)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwNeedSize);
\#046               if (lpBuf)
\#047               {
\#048                    //获取字符图形的数据到缓冲区。
**\#049               GetGlyphOutline(hDC,chText,GGO_BITMAP,&gm,dwNeedSize,lpBuf,&mat2);**
\#050
\#051                    //计算图形每行占用的字节数。
\#052                    int nByteCount = ((gm.gmBlackBoxX +31) >> 5) << 2;
\#053
\#054                    //显示每行图形的数据。
\#055                    for (int i = 0; i < gm.gmBlackBoxY; i++)
\#056                    {
\#057                          //
\#058                          for (int j = 0; j < nByteCount; j++)
\#059                          {
\#060
\#061                               BYTE btCode = lpBuf[i* nByteCount + j];
\#062
\#063                               //按字节输出每点的数据。
\#064                               for (int k = 0; k < 8; k++)
\#065                               {
\#066
\#067                                     if (btCode & (0x80>>k))
\#068                                     {
\#069
\#070                                           OutputDebugString(_T("1"));
\#071                                     }
\#072                                     else
\#073                                     {
\#074                                           OutputDebugString(_T("0"));
\#075                                     }
\#076
\#077                               }
\#078
\#079                          }
\#080
\#081                          //
\#082                          OutputDebugString(_T("/r/n"));
\#083                    }
\#084
\#085                    //
\#086                    HeapFree(GetProcessHeap(),0,lpBuf);
\#087               }
\#088         }
\#089
\#090         //
\#091         SelectObject(hDC,hOldFont);
\#092         DeleteObject(hFont);
\#093
\#094         //
\#095         ReleaseDC(m_hWnd,hDC);
\#096  }
\#097
输出的结果如下：
00000000000000010000000000000000
00000000110000011000000000000000
00000000100000011000000000000000
00000000100000011000011000000000
11111111111111111111111100000000
01000000100000011000000000000000
00000000100000011000000000000000
00000100100000011000000000000000
00000110100000010000000000000000
00000100000000000000000000000000
00001100000001000000100000000000
00001111111101111111110000000000
00001000001111000000110000000000
00011000001000100001100000000000
00010100001000100001000000000000
00010010011000100011000000000000
00100011010000010010000000000000
00110010110000011010000000000000
01011000110000001100000000000000
10001000100000001100000000000000
00001001100000010110000000000000
00001011011111111011000000000000
00000010000000000001110000000000
00000110000000000000111100000000
00001100000000000110011100000000
00011111111111111111001000000000
00110000000010000000000000000000
01000000000010000000000000000000
00000001000010000000000000000000
00000011100010001100000000000000
00000011000010000010000000000000
00000110000010000011100000000000
00001100000010000001100000000000
00011000100010000000110000000000
00110000011110000000110000000000
01000000001110000000010000000000
00000000000100000000000000000000


