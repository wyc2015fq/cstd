# 字符串处理 - ANSI - Unicode - UTF8 转换 - sandro_zhang的专栏 - CSDN博客
2012年04月26日 15:03:52[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：4660标签：[null																[input																[file																[语言																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[vc++](https://blog.csdn.net/sandro_zhang/article/category/884062)
最近发现一个比较全的字符串编码转换的方法，记录如下：
```cpp
2008-11-4: 使用MultiByteToWideChar和WideCharToMultiByte写的4个ANSI <-> Unicode <-> UTF-8 相互转换的函数。
2008-11-5: 使用C语言标准库mbstowcs和wcstombs写的w2m和m2w两个函数，分别对应ANSI <-> Unicode 相互转换。
2008-11-8: 包装一下 ANSI <-> UTF-8 。
#include <stdio.h>
#include <windows.h>
#include <locale.h>
#define BUFF_SIZE 1024
wchar_t * ANSIToUnicode( const char* str )
{
     int textlen ;
     wchar_t * result;
     textlen = MultiByteToWideChar( CP_ACP, 0, str,-1, NULL,0 ); 
     result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t)); 
     memset(result,0,(textlen+1)*sizeof(wchar_t)); 
     MultiByteToWideChar(CP_ACP, 0,str,-1,(LPWSTR)result,textlen ); 
     return result; 
}
char * UnicodeToANSI( const wchar_t* str )
{
     char* result;
     int textlen;
     textlen = WideCharToMultiByte( CP_ACP, 0, str, -1, NULL, 0, NULL, NULL );
     result =(char *)malloc((textlen+1)*sizeof(char));
     memset( result, 0, sizeof(char) * ( textlen + 1 ) );
     WideCharToMultiByte( CP_ACP, 0, str, -1, result, textlen, NULL, NULL );
     return result;
}
wchar_t * UTF8ToUnicode( const char* str )
{
     int textlen ;
     wchar_t * result;
     textlen = MultiByteToWideChar( CP_UTF8, 0, str,-1, NULL,0 ); 
     result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t)); 
     memset(result,0,(textlen+1)*sizeof(wchar_t)); 
     MultiByteToWideChar(CP_UTF8, 0,str,-1,(LPWSTR)result,textlen ); 
     return result; 
}
char * UnicodeToUTF8( const wchar_t* str )
{
     char* result;
     int textlen;
     textlen = WideCharToMultiByte( CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL );
     result =(char *)malloc((textlen+1)*sizeof(char));
     memset(result, 0, sizeof(char) * ( textlen + 1 ) );
     WideCharToMultiByte( CP_UTF8, 0, str, -1, result, textlen, NULL, NULL );
     return result;
}
/*宽字符转换为多字符Unicode - ANSI*/
char* w2m(const wchar_t* wcs)
{
      int len;
      char* buf;
      len =wcstombs(NULL,wcs,0);
      if (len == 0)
          return NULL;
      buf = (char *)malloc(sizeof(char)*(len+1));
      memset(buf, 0, sizeof(char) *(len+1));
      len =wcstombs(buf,wcs,len+1);
      return buf;
}
/*多字符转换为宽字符ANSI - Unicode*/
wchar_t* m2w(const char* mbs)
{
      int len;
      wchar_t* buf;
      len =mbstowcs(NULL,mbs,0);
      if (len == 0)
          return NULL;
      buf = (wchar_t *)malloc(sizeof(wchar_t)*(len+1));
      memset(buf, 0, sizeof(wchar_t) *(len+1));
      len =mbstowcs(buf,mbs,len+1);
      return buf;
}
char* ANSIToUTF8(const char* str)
{
     return UnicodeToUTF8(ANSIToUnicode(str));
}
char* UTF8ToANSI(const char* str)
{
     return UnicodeToANSI(UTF8ToUnicode(str));
}
int main()
{
     /*使用wcstombs和mbstowcs之前必须调用setlocale，以便决定内码*/
     setlocale(LC_ALL,".936");
     /*假定有一个Unicode(UTF-16LE)编码的文件，将其打开，重新编码为ANSI，写入aa.txt中，再继续编码回Unicode，写入aw.txt中*/
     /*如果不存在a.txt文件，则程序出错，没有做错误处理*/
     char* filename = "a.txt";
     char* filenamea = "aa.txt";
     char* filenamew = "aw.txt";
     FILE*     input=fopen( filename, "rb"); 
     FILE*     inputa=fopen( filenamea, "wb"); 
     FILE*     inputw=fopen( filenamew, "wb"); 
     wchar_t * buf ;
     /*BOE设置，UTF-16LE的BOE为FEFF，如果不先将其读取出来，wcstombs会调用失败*/
     fgetwc(input);
     fputwc(0xFEFF,inputw);
     /*开始读取文件*/
     while(!feof(input))
     {
        buf = (wchar_t *)malloc(sizeof(wchar_t)*BUFF_SIZE)         ;
        memset(buf,    0, sizeof(wchar_t) * BUFF_SIZE );
        fgetws(buf,    BUFF_SIZE,    input);
        fputs(w2m(buf),    inputa);
        fputws(m2w(w2m(buf)),    inputw);
     }
     /*后续处理*/
     fclose(input);
     fclose(inputa);
     fclose(inputw);
     free(buf);
     return 0;
}
```
感谢：[http://hi.baidu.com/end2012/blog/item/9ff286df5d57bc53cdbf1ab9.html](http://hi.baidu.com/end2012/blog/item/9ff286df5d57bc53cdbf1ab9.html)
