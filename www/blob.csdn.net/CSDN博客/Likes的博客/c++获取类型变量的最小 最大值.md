# c++获取类型变量的最小/最大值 - Likes的博客 - CSDN博客
2018年10月26日 11:00:46[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：398
```cpp
#include <limits>
int min = std::numeric_limits<int>::min();//模板推断，若是char直接改为char即可
int max = std::numeric_limits<int>::max();
```
