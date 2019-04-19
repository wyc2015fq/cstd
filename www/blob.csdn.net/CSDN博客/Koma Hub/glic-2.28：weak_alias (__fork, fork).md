# glic-2.28：weak_alias (__fork, fork) - Koma Hub - CSDN博客
2019年01月15日 22:39:21[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：21
个人分类：[Glibc](https://blog.csdn.net/Rong_Toa/article/category/8605571)
```cpp
weak_alias (__fork, fork)
/* Define ALIASNAME as a weak alias for NAME.
   If weak aliases are not available, this defines a strong alias.  */
# define weak_alias(name, aliasname) _weak_alias (name, aliasname)
# define _weak_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((weak, alias (#name)));
```
