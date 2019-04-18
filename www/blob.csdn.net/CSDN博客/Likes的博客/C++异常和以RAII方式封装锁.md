# C++异常和以RAII方式封装锁 - Likes的博客 - CSDN博客
2018年12月05日 11:17:29[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：32
在多线程编程中，我们应该使用“获得资源时即初始化”RAII（resource acquisition is initialization）。简单说来就是在构造函数中统一分配资源，在析构函数中统一回收资源。
```cpp
{
    Lock<vector<int>> lock(v);
    ...
}
```
这种方式有诸多好处，除了使代码更加简洁之外，也是异常安全的。
C++保证，**如果有异常被抛出，局部对象会被析构**，所以，即便在我们使用Lock对象的过程中有异常抛出，Lock仍然会释放它所拥有的互斥变量（因为它是局部变量）。而如果我们在代码中手工调用getMutex和releaseMutex，那么，在get和release之间发生了异常，我们永远无法释放互斥变量。
