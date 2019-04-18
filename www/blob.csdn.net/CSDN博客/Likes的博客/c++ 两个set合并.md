# c++ 两个set合并 - Likes的博客 - CSDN博客
2018年10月10日 10:50:09[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：470
C++17提供set和unordered_set的merge，但是C++11并不提供，但依然有简便的方法实现两个set的合并（求并集），如下：
```cpp
set<int> a,b;
//合并到a
a.insert(b.begin(),b.end());
```
