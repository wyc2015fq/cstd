# [u]intN_t - uint8_t, uint16_t, uint32_t, uint64_t - Koma Hub - CSDN博客
2018年06月14日 09:48:22[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：53
个人分类：[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)
## `uint8_t, uint16_t, uint32_t, uint64_t`
在c/c++中，很多以`_t`结尾的数据类型，如`uint8_t`,`size_t`等等，乍一看什么鬼，实际上`_t`的意思就是`typedef`的后缀缩写。具体如下：
## [](https://github.com/Rtoax/Knowledge/blob/master/cpp/%5Bu%5DintN_t.md#c99%E6%A0%87%E5%87%86%E4%B8%ADinttypesh%E7%9A%84%E5%86%85%E5%AE%B9)C99标准中`inttypes.h`的内容
/*   inttypes.h: C99标准中inttypes.h的内容   Contributors:     Createdby Marek Michalkiewicz <marekm@linux.org.pl>   THISSOFTWARE IS NOT COPYRIGHTED   Thissource code is offered for use in the public domain.  You may   use,modify or distribute it freely.   Thiscode is distributed in the hope that it will be useful, but   WITHOUTANY WARRANTY.  ALLWARRANTIES, EXPRESS OR IMPLIED ARE HEREBY   DISCLAIMED.  This includes but is not limited towarranties of   MERCHANTABILITYor FITNESS FOR A PARTICULAR PURPOSE.*/
#ifndef __INTTYPES_H_
#define__INTTYPES_H_/* Use [u]intN_t if you need exactly N bits.  XXX- doesn't handle the -mint8 option.  */typedefsignedcharint8_t;
typedefunsignedcharuint8_t;
typedefintint16_t;
typedefunsignedintuint16_t;
 
typedeflongint32_t;
typedefunsignedlonguint32_t;
typedeflonglongint64_t;
typedefunsignedlonglonguint64_t;
typedefint16_tintptr_t;
typedefuint16_tuintptr_t;
 
#endif
## [](https://github.com/Rtoax/Knowledge/blob/master/cpp/%5Bu%5DintN_t.md#c99%E6%A0%87%E5%87%86%E5%8F%A6%E4%B8%80%E4%B8%AA%E6%96%87%E4%BB%B6usrincludestdinth%E5%86%85%E5%A6%82%E4%B8%8B)C99标准另一个文件`/usr/include/stdint.h`内如下：
/*在C99标准中定义了这些数据类型，  具体定义在：/usr/include/stdint.h  参考：https://blog.csdn.net/Mary19920410/article/details/71518130?locationNum=4&fps=1*/
#ifndef __int8_t_defined    
#define__int8_t_definedtypedefsignedcharint8_t;     
typedefshortintint16_t;    
typedefintint32_t;    
#   if __WORDSIZE == 64    
typedeflongintint64_t;    
#   else    
__extension__    
typedeflonglongintint64_t;    
#   endif    
#endiftypedefunsignedcharuint8_t;    
typedefunsignedshortintuint16_t;    
#ifndef __uint32_t_defined    
typedefunsignedintuint32_t;    
# define__uint32_t_defined    
#endif    
#if __WORDSIZE == 64    
typedefunsignedlongintuint64_t;    
#else    
__extension__    
typedefunsignedlonglongintuint64_t;    
#endif
