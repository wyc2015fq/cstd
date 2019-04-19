# Linux下如何指定调用约定(calling convention) - xqhrs232的专栏 - CSDN博客
2017年04月24日 15:26:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：762
原文地址::[http://www.linuxidc.com/Linux/2011-04/35032.htm](http://www.linuxidc.com/Linux/2011-04/35032.htm)
相关文章
1、Linux下如何指定调用约定(calling convention)----[http://blog.sina.com.cn/s/blog_81fcea160101mpoo.html](http://blog.sina.com.cn/s/blog_81fcea160101mpoo.html)
2、[__stdcall和__cdecl的区别](http://www.cnblogs.com/zhangchaoyang/articles/2725189.html)----[http://www.cnblogs.com/zhangchaoyang/articles/2725189.html](http://www.cnblogs.com/zhangchaoyang/articles/2725189.html)

Windows下的调用约定可以是stdcall/cdecl/fastcall，这些标识加在函数名前面，如：
int __stdcall funca()
但在Linux下，如按照上面写法后，编译程序将导致编译错误，Linux下正确的语法如下：
int __attribute__((__stdcall__)) funca()
int __attribute__((__cdecl__)) funca()
Linux下如果函数不指定调用约定，默认的情况应该是__attribute__((__cdecl__))
int __attribute__((__cdecl__)) myfunc(int i, int j, int k)
{
    return i+j+k;
}
 080483d0 <myfunc>:
 80483d0:    55                       push   %ebp
 80483d1:    89 e5                    mov    %esp,%ebp
 80483d3:    8b 45 0c                 mov    0xc(%ebp),%eax
 80483d6:    03 45 08                 add    0x8(%ebp),%eax
 80483d9:    03 45 10                 add    0x10(%ebp),%eax
 80483dc:    5d                       pop    %ebp
 80483dd:    c3                       ret    
 80483de:    66 90                    xchg   %ax,%ax
int __attribute__((__stdcall__)) myfunc(int i, int j, int k)
{
    return i+j+k;
}
 080483d0 <myfunc>:
 80483d0:    55                       push   %ebp
 80483d1:    89 e5                    mov    %esp,%ebp
 80483d3:    8b 45 0c                 mov    0xc(%ebp),%eax
 80483d6:    03 45 08                 add    0x8(%ebp),%eax
 80483d9:    03 45 10                 add    0x10(%ebp),%eax
 80483dc:    5d                       pop    %ebp
 80483dd:    c2 0c 00                 ret    $0xc
