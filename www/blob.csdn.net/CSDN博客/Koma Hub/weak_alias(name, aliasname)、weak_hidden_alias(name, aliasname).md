# weak_alias(name, aliasname)、weak_hidden_alias(name, aliasname) - Koma Hub - CSDN博客
2019年01月22日 21:25:41[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：29
个人分类：[Glibc](https://blog.csdn.net/Rong_Toa/article/category/8605571)
```cpp
/* Define ALIASNAME as a weak alias for NAME.
   If weak aliases are not available, this defines a strong alias.  */
# define weak_alias(name, aliasname) _weak_alias (name, aliasname)
# define _weak_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((weak, alias (#name)));
```
```cpp
/* Same as WEAK_ALIAS, but mark symbol as hidden.  */
# define weak_hidden_alias(name, aliasname) \
  _weak_hidden_alias (name, aliasname)
# define _weak_hidden_alias(name, aliasname) \
  extern __typeof (name) aliasname \
    __attribute__ ((weak, alias (#name), __visibility__ ("hidden")));
```
