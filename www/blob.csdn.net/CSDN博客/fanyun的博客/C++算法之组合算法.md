# C++算法之组合算法 - fanyun的博客 - CSDN博客
2018年06月23日 22:36:41[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：995
什么是组合？
    在组合中取出元素的顺序则不在考虑之中。从![n](http://upload.wikimedia.org/math/7/b/8/7b8b965ad4bca0e41ab51de7b31363a1.png)个元素中取出![k](http://upload.wikimedia.org/math/8/c/e/8ce4b16b22b58894aa86c421e8759df3.png)个元素，这![k](http://upload.wikimedia.org/math/8/c/e/8ce4b16b22b58894aa86c421e8759df3.png)个元素可能出现的组合数的总数量为：
![](https://img-blog.csdn.net/20140309174127093)
以1234中选2个数为例：
1,2、 1,3、 1,4、 2,3、 2,4、3,4
```cpp
#include "stdafx.h"  
#include <iostream>  
#include <vector>  
#include <algorithm>  
  
using namespace std;  
  
  
void printEachResult(int t[], int index, int total){  
  
    for(int i = 0; i < total; ++i){  
        if((index>>i)&1 == 1){  
            cout << t[i] << " ";  
        }  
    }  
    cout << endl;  
}  
  
void combination(int t[],int count){  
    for(int i = 0; i < (1<<count); ++i){  
        printEachResult(t, i, count);  
    }  
}  
  
  
int _tmain(int argc, _TCHAR* argv[])  
{  
    const int N = 3;   
    int t[N] = {1, 2, 3};  
    combination(t,N);  
    system("pause");  
    return 0;  
}
```
