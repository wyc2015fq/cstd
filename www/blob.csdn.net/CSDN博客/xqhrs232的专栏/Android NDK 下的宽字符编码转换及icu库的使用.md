# Android NDK 下的宽字符编码转换及icu库的使用 - xqhrs232的专栏 - CSDN博客
2011年06月23日 13:36:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2379
原文地址::[http://blog.csdn.net/liujian885/archive/2011/06/10/6536897.aspx](http://blog.csdn.net/liujian885/archive/2011/06/10/6536897.aspx)
CSDN相关网贴
1。跨平台的文字编码转换方法--ICU 
[http://blog.csdn.net/serverxp/archive/2010/04/28/5538017.aspx](http://blog.csdn.net/serverxp/archive/2010/04/28/5538017.aspx)
原贴[http://topic.csdn.net/u/20101022/16/1b2e0cec-b9d2-42ea-8d9c-4f1bb8320a54.html?r=70149216](http://topic.csdn.net/u/20101022/16/1b2e0cec-b9d2-42ea-8d9c-4f1bb8320a54.html?r=70149216) ，看过并动手实现，记录下来以备再用。
    如果是在java层，有String类可以很好的转换各种编码，在ndk下面就没有现成的公开的工具，不过可以用icu4c。
    ICU4C 是IBM的国际化开发组件ICU的C语言实现版本。在android系统里也有实现。ndk里面并没有公开可用的api，需要自己加载动态库来调用转换函数。
    android下icu库路径为"/system/lib/libicuuc.so",主要用到的转换函数为ucnv_convert_?_?。这里的问号是根据版本的不同函数名也不一样。在2.2的模拟器中的libicuuc.so中此函数名为ucnv_convert_4_2，在2.1模拟器中为ucnv_convert_3_8，貌似要根据版本不同来分开对待，还没发现可以统一的办法。
    函数原型：
    void ucnv_convert(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*)；
    用法：
    view plaincopy to clipboardprint?
//声明函数指针   
void (*ucnv_convert)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*)=0;  
//加载动态库   
void* pDL = dlopen("/system/lib/libicuuc.so", RTLD_LAZY);   
//这里以android2.2为例，函数名就是ucnv_convert_4_2   
ucnv_convert = (void (*)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*))dlsym(pDL, "ucnv_convert_4_2");  
//加载成功就可以用了   
if(ucnv_convert){   
    char* cbuf = "...";   
    char buffer[100];   
    int errcode = 0;   
    //utf8是目标编码，ucs2是原字符编码   
     //buffer是存放转换出来的字符的缓冲，给了100字节   
     //cbuf是要转换的字符串指针   
     //errcode是错误编码，具体可网上搜索   
    ucnv_convert("utf8","ucs2", buffer, 100, cbuf, strlen(cbuf),&errcode);   
}  
//声明函数指针
void (*ucnv_convert)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*)=0;
//加载动态库
void* pDL = dlopen("/system/lib/libicuuc.so", RTLD_LAZY);
//这里以android2.2为例，函数名就是ucnv_convert_4_2
ucnv_convert = (void (*)(const char *, const char *, char * , int32_t , const char *, int32_t,int32_t*))dlsym(pDL, "ucnv_convert_4_2");
//加载成功就可以用了
if(ucnv_convert){
    char* cbuf = "...";
    char buffer[100];
    int errcode = 0;
    //utf8是目标编码，ucs2是原字符编码
     //buffer是存放转换出来的字符的缓冲，给了100字节
     //cbuf是要转换的字符串指针
     //errcode是错误编码，具体可网上搜索
    ucnv_convert("utf8","ucs2", buffer, 100, cbuf, strlen(cbuf),&errcode);
} 
    转换成功后的字符串就放在buffer里面，如果出错了就会在errcode里面放错误代码。
    如标题所示，ndk下还有一个宽字符，也就是wchar_t的问题，跟其他平台移植也是个麻烦的事。
    linux下wchar_t默认是4个字节，而windows下（包括CE,MOBILE）和symbian下都是2个字节。解决的办法是在android.mk文件中，找到LOCAL_CFLAGS 为其加上编译开关 -fshort-wchar（如果没有此项就手动写上），如 LOCAL_CFLAGS :=  -fshort-wchar  。这样强制编译器用2个字节处理wchar_t，不过编译时会有warning，可以不管。
    这样虽然编译器处理成2个字节，但是预编译的库libc等依然是4个字节，会导致wcslen等函数无法使用（其实ndk下wcslen本来就是废的），解决的办法可以重新编译libc，不过最简单的还是自己实现wcslen就行了。
    下面的代码是copy网上的，具体哪里的忘了，可以把wchar_t转换成char字符串，这样就可以用icu库随意转换了。
    view plaincopy to clipboardprint?
//取得wchar_t字符串长度   
int wlen(const wchar_t* strString){   
    int i=0;   
    while (1) {   
        if (strString[i] == 0) {   
            break;   
        }else{   
            i++;   
        }   
    }   
    return i;   
}   
char *W2C(const wchar_t *pw , char *pc)   
{   
      *pc++ = *pw >> 8 ;   
      *pc = *pw ;   
      return 0 ;   
}   
//转换字符串   
char *wstr2cstr(const wchar_t *pwstr, char *pcstr, size_t len) {   
    char *ptemp = pcstr;   
    if (pwstr != NULL && pcstr != NULL) {   
        size_t wstr_len = wlen(pwstr);   
        len = (len > wstr_len) ? wstr_len : len;   
        while (len-- > 0) {   
            W2C(pwstr, pcstr);   
            pwstr++;   
            pcstr += 2;    
        }   
        *pcstr = '/0';   
        return ptemp;   
    }   
    return 0;   
}  
//取得wchar_t字符串长度
int wlen(const wchar_t* strString){
 int i=0;
 while (1) {
  if (strString[i] == 0) {
   break;
  }else{
   i++;
  }
 }
 return i;
}
char *W2C(const wchar_t *pw , char *pc)
{
      *pc++ = *pw >> 8 ;
      *pc = *pw ;
      return 0 ;
}
//转换字符串
char *wstr2cstr(const wchar_t *pwstr, char *pcstr, size_t len) {
 char *ptemp = pcstr;
 if (pwstr != NULL && pcstr != NULL) {
  size_t wstr_len = wlen(pwstr);
  len = (len > wstr_len) ? wstr_len : len;
  while (len-- > 0) {
   W2C(pwstr, pcstr);
   pwstr++;
   pcstr += 2; 
  }
  *pcstr = '/0';
  return ptemp;
 }
 return 0;
} 
    使用wstr2cstr就可以转换出来。这里还有个字节序的问题，在W2C函数里面，一个wchar_t转到char究竟是低位在前还是高位在前恐怕还是要看转换前后的编码具体对待。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/liujian885/archive/2011/06/10/6536897.aspx](http://blog.csdn.net/liujian885/archive/2011/06/10/6536897.aspx)
