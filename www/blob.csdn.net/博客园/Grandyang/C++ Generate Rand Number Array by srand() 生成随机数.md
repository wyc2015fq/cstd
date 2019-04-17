# C++ Generate Rand Number Array by "srand()" 生成随机数 - Grandyang - 博客园







# [C++ Generate Rand Number Array by "srand()" 生成随机数](https://www.cnblogs.com/grandyang/p/4893992.html)







在C++中，我们有时想生成一个由随机数组成的数组，而且随机数的范围也可由我们来设定。那么我们就要用到srand()函数配合rand()来使用，参见如下代码：



```
#include <vector>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>  

using namespace std;

int main() {

    srand(time(NULL));
    vector<int> nums(10);
    
    for (int i = 0; i < 10; ++i) {
        nums[i] = 1 + (rand() % 100);
        cout << nums[i] << " ";
    }
    cout << endl;
    
    return 0;
}
```



上面代码中srand(time(NULL));是用来生成一个随机数种子，而rand() % 100限定了随机数范围是[0, 99]，我们在前面加1，范围就变成了[1, 100]。

我们再来看看Java中如何生成随机数数组，一两行就搞定：



```
int size = 100;
int[] list = AssortedMethods.randomArray(size, -100, 100);
```



简单的令人发指啊，怒转Java的节奏。。。












