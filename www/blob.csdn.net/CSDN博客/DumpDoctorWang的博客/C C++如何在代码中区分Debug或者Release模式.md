# C\C++如何在代码中区分Debug或者Release模式 - DumpDoctorWang的博客 - CSDN博客





2018年07月03日 18:44:33[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：514








在Release模式下会自动定义一个宏NDEBUG，Debug模式下没有，因此可以在在代码中判断宏NDEBUG是否存在来判断是Release模式还是Debug模式。示例代码如下

```cpp
#ifndef NDEBUG
    std::cout << "Hello, World! Debug Mode" << std::endl;
#else
    std::cout << "Hello, World! Release Mode" << std::endl;
#endif
```

因此，有时需要在程序中判断是不是Release模式，就可以通过上述代码来判断。




