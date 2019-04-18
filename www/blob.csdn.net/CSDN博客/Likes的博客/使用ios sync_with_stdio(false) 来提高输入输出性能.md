# 使用ios::sync_with_stdio(false) 来提高输入输出性能 - Likes的博客 - CSDN博客
2019年03月05日 09:16:13[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：34
cin和cout用起来很方便，但是性能不及scanf和printf，我们可以使用
```cpp
std::ios::sync_with_stdio(false);
```
来提升cin和cout的性能与sanf和printf相当
