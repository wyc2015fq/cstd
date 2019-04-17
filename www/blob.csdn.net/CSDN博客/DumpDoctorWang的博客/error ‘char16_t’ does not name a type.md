# error: ‘char16_t’ does not name a type - DumpDoctorWang的博客 - CSDN博客





2019年03月13日 20:18:12[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：54








## 一、问题描述

最近在编译一个依赖于ICU库的项目时，出现了错误：

```bash
/usr/include/unicode/umachine.h:347:13: error: ‘char16_t’ does not name a type
     typedef char16_t UChar;
```

## 二、解决办法

原因是编译器默认的C/C++标准是C98/C++98，那时候还没有char16_t这种类型，故出现了编译错误。因此需要给编译器指定C++的版本，把标准设置到C11/C++11，这样编译就可以编译过了。

若使用cmake，在CMakeLists.txt的project()之前加入这一行：

```bash
set(CMAKE_C_STANDARD  11)
set(CMAKE_CXX_STANDARD 11)
```

对应的编译器选项为`-std=c++11`。



