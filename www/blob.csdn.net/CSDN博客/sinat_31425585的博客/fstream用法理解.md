# fstream用法理解 - sinat_31425585的博客 - CSDN博客
2018年03月11日 10:37:13[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：85
    fstream为常用到的文件流写入操作，和cin、cout用法类似，不同之处在于：cout是将要输出的内容打印到CMD中，而ofstream是将要输出的内容打印到文件中。cin与ifstream的关系与之类似。
**   1、文件流操作对文件进行读写操作**
    添加头文件 -> 打开文件 -> 进行文件流操作 -> 关闭文件流
```cpp
#include <fstream>  // 包含文件
ofstream out("angles.txt", ios::out|ios::trunc); // 打开文件
out << M[j] << endl;  // 打印数据到文件中
out.close();         // 关闭文件流
```
ifstream的使用方法与之类似。
 其中，文件模式有：
   in：以读的方式打开，只对应于ifstream或fstream
  out：以写的形式打开，只对应于ofstream或fstream
  app：每次写操作均定位到文件末尾
  ate：打开文件后立即定位到文件末尾
  trunc：截断文件
  binary：二进制方式进行IO操作
  默认情况下，以out模式打开文件也会截断，为了保留以out模式打开的文件内容，需要指定app模式。
**2、文件读操作**
  1) 采用文件流操作：
```cpp
#include <fstream>  // 包含文件
ifstream in("angles.txt", ios::in|ios::trunc); // 打开文件
in >> M[j] >> endl;  // 打印数据到文件中
in.close();         // 关闭文件流
```
 2)  getline:读取一行
```cpp
in.getline(temp, 24, 0);  //
```
**3、read和write方式：**读写变量、数组和矩阵
```cpp
in.read((char*)&a, sizeof(int));
out.write((char*)&a, sizeof(int));
```
4、例子
按行读取一个文件：
```cpp
#include <fstream>
#include <iostream>
#include <string>
int main(int argc, char* argv[]) {
    std::string file_name = "name.txt";
    std::ifstream in(file_name.c_str(), std::ios::in);
    if(! in.is_open()) {
        std::cout << "open the file: " <<  file_name << " failed." << std::endl;
        return -1;
    }
    char name[128];
    while(!in.eof()) {
        in.getline(name, 64);
        std::cout << "the current name is: " << name << std::endl;
    }
    return 0;
}
```
参考资料：
 C++ Primer 中文版（第五版）
