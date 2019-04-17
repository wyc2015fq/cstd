# POCO库中文编程参考指南（2）基本数据类型（Poco/Types.h） - DoubleLi - 博客园






# POCO库中文编程参考指南（2）基本数据类型
- 作者：柳大·Poechant
- 博客：[Blog.CSDN.net/Poechant](http://blog.csdn.net/Poechant)
- 邮箱：zhongchao.ustc#gmail.com (# -> @)
- 日期：April 14th, 2012

基本类型在`Poco/Types.h`头文件中。以下以 GNU C/C++ 和 CLang 为例。不过要强调的是，POCO 有极好的跨平台性，你能想到的平台，几乎都可以运行 POCO。

## 1 有符号整型

```
typedef signed char            Int8;
typedef signed short           Int16;
typedef signed int             Int32;
```

## 2 无符号整型

```
typedef unsigned char          UInt8;
typedef unsigned short         UInt16;
typedef unsigned int           UInt32;
```

## 3 Pointer or Handler

```
typedef signed long            IntPtr;
typedef unsigned long          UIntPtr;
```

## 4 64 位整型

考虑了 32 位机和 64 位机的情况

```
#if defined(__LP64__)
    #define POCO_PTR_IS_64_BIT 1
    #define POCO_LONG_IS_64_BIT 1
    typedef signed long        Int64;
    typedef unsigned long      UInt64;
#else
    typedef signed long long   Int64;
    typedef unsigned long long UInt64;
#endif
#define POCO_HAVE_INT64 1
```

-

from：[Blog.CSDN.net/Poechant](http://blog.csdn.net/Poechant)









