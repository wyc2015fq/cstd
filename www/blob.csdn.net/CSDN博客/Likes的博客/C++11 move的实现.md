# C++11 move的实现 - Likes的博客 - CSDN博客
2018年10月29日 15:46:36[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：107
```cpp
template <class T>
typename remove_reference<T>::type&& move(T&& t) //通过trait技法推断出返回值。参数类型是T&&万能引用，所以move不仅可以把左值转成右值，也可以把右值转成右值
{
    using RRefType = typename remove_reference<T>::type&&;//取别名RRefType 
    return static_cast<RRefType>(t);//由此可见直接可以通过静态强转成右值
}
```
1、通过trait技法推断出返回值。
2、参数类型是T&&万能引用，所以move不仅可以把左值转成右值，也可以把右值转成右值。
3、通过using取别名RRefType
4、左值转成右值直接通过static_cast实现，并无神秘之处。
