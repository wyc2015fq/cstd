# error: expected unqualified-id before ‘dynamic_cast’ - Likes的博客 - CSDN博客
2018年12月20日 11:58:57[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：160
错误代码：
```cpp
Deride * q= std::dynamic_cast<Deride*>(p);
```
其实很简单，但没有想到又会卡半天，dynamic_cast并不是std库中的部件，而是C++关键字，所以应该去掉std::
```cpp
Deride * q= dynamic_cast<Deride*>(p);
```
 需要提醒一下的是：
用于动态转换shared_ptr的[std::dynamic_pointer_cast](https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast)是std中的部件。其实也是显然的，因为shared_ptr是std中的部件
