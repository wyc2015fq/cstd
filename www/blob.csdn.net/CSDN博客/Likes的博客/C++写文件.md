# C++写文件 - Likes的博客 - CSDN博客
2018年10月18日 14:57:49[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：23
### 1、头文件
```cpp
#include<fstream>
```
### 2、fstream对象
```cpp
fstream out("./output.out");//没有文件返回out==0，不会创建文件
out<<"hello world"<<endl;
```
    要想fstream对象在没有文件时自动创建对象：
```cpp
fstream out("output.out",ofstream::out);
//或者
fstream out("output.out",ofstream::app);
```
### 3、ofstream对象
```cpp
ofstream out(“./output.out”);//没有文件创建文件
```
