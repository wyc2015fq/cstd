# 剑指offer--求出数组中最大元素出现的次数 - 风雪夜归人 - CSDN博客
2018年09月05日 10:27:32[cdjccio](https://me.csdn.net/qq_34624951)阅读数：48
个人分类：[剑指offer](https://blog.csdn.net/qq_34624951/article/category/7623846)
```cpp
#include <iostream>
using namespace std;
// 思想：用空间换取时间
// 读取数组中的元素，将其出现的次数保存在新的数组中
void findMaxnum(int data[], int len)
{
    int sp[1000] = {0};
    int Max = 0;
    // 遍历数组元素，将元素出现的次数保存在新的数组中
    for(int i = 0; i<len; i++)
    {
        int index = data[i] - 1;
        sp[index] ++;
    }
    // 找出新的数组中元素出现次数的最大值
    for(int i = 0; i<1000; i++)
    {
        if(Max < sp[i])
        {
            Max = sp[i];
        }
    }
    // 根据元素出现的最大次数，在新的数组中找出对应的元素
    for(int i = 0; i<1000; i++)
    {
        if(sp[i] == Max)
            cout << i+1 << " => " << Max<< endl;
    }
}
int main()
{
    int data[] = {1,3,4,6,3,6,2,6,24,6,24,1,1,9,9,9,99,9,9,9,9,9,9,9};
    findMaxnum(data, sizeof(data)/sizeof(data[0]));
    return 0;
}
```
