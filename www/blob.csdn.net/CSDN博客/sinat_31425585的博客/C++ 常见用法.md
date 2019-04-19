# C++ 常见用法 - sinat_31425585的博客 - CSDN博客
2019年01月08日 07:42:19[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：63
1、std::sort
说实话，这个是真的好用，需要包含头文件：#include <algorithm>
```cpp
std::sort(A.begin(), A.end(), [](struct b1, struct b2) {return b1.m_ < b2.m_; });
```
其中，A为struct的vector数组，m_为struct的一个属性，这个就是按照结构体的m_属性降序排序；
2、重载操作符=
参考资料：
[https://blog.csdn.net/cook2eat/article/details/52420200](https://blog.csdn.net/cook2eat/article/details/52420200)
3、map用法
使用迭代器，find
```cpp
auto itc = objects.find(name.at(i));
if(itc == objects.end()) {
    std::cout << "error key..." << std::endl;
    return -1;
}
auto obj = itc->second;
```
4、extern
在头文件中声明全局变量 ：
```cpp
extern int x；
```
在cpp文件中进行定义：
```cpp
int x = 1；
```
在其他位置包含当前头文件，并进行宏定义：
```cpp
#define X x
```
5、从string路径中解析出文件名
```cpp
auto pos_start = img_file_curr.find_last_of('/');
auto pos_end = img_file_curr.find_last_of('.');
std::string file_name = img_file_curr.substr(pos_start + 1, pos_end - 1 - pos_start);
```
6、从char*文件名中解析出字符串
```cpp
char* ptr = nullptr;
char c_tmp = '_';
ptr = strrchr(objects.at(j).name, c_tmp);
std::cout << "the string is: " << ptr << std::endl;
```
~~~未完待续~~~
