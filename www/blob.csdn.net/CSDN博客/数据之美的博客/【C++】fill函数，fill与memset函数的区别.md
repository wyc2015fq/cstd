# 【C++】fill函数，fill与memset函数的区别 - 数据之美的博客 - CSDN博客
2017年08月27日 14:23:37[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：185
【C++】fill函数，fill与memset函数的区别
- 
memset函数
- 按照字节填充某字符
- 在头文件`<cstring>`里面
- 
fill函数
- 按照单元赋值，将一个区间的元素都赋同一个值
- 在头文件`<algorithm>`里面
- 
因为memset函数按照字节填充，所以一般memset只能用来填充char型数组，（因为只有char型占一个字节）如果填充int型数组，除了0和-1，其他的不能。因为只有00000000 = 0，-1同理，如果我们把每一位都填充“1”，会导致变成填充入“11111111”
- 
而fill函数可以赋值任何，而且使用方法特别简便：
- fill(arr, arr + n, 要填入的内容);
- 例如：
```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
    int arr[10];
    fill(arr, arr + 10, 2);
    return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- vector也可以：
```cpp
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;
int main(){
    vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    fill(v.begin(), v.end(), -1);
    return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 而memset的使用方法是：
```cpp
#include <iostream>
#include <cstring>
using namespace std;
int main(){
    int a[20];
    memset(a, 0, sizeof a);
    return 0;
}
```
