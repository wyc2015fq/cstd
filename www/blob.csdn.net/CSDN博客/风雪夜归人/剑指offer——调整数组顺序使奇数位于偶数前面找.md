# 剑指offer——调整数组顺序使奇数位于偶数前面找 - 风雪夜归人 - CSDN博客
2018年05月02日 22:17:42[cdjccio](https://me.csdn.net/qq_34624951)阅读数：42
个人分类：[剑指offer](https://blog.csdn.net/qq_34624951/article/category/7623846)
Q：输入一个整数数组，实现一个函数来调整该数组中数字的顺序，使得所有的奇数位于数组的前半部分，所有的偶数位于位于数组的后半部分，并保证奇数和奇数，偶数和偶数之间的相对位置不变。
A：1、新建两个数组分别放原数组中的奇数和偶数。
      2、将奇数数组中的数复制给原数组
      3、将偶数数组中的数复制到原数组奇数的后边
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
class solution
{
public:
    void reOrderArray(vector<int > &Array) // 不考虑相对位置不变的case
    {
        int len = Array.size();
        int *p = &Array[1];
        int *q = &Array[len - 1];
        while(q > p)
        {
            if(*p % 2 == 0)
            {
                if(*q % 2 == 0)
                {
                    q --;
                }
                else
                {
                    int tmp = *p;
                    *p = *q;
                    *q = tmp;
                }
            }
            else
            {
                if(*q % 2 == 0)
                {
                    q --;
                }
                else
                {
                    p ++;
                }
            }
        }
    }
    void _reOrderArray(vector<int > &Array) // 考虑相对位置不变的case
    {
        vector<int > even, odd;
        int num_even = 0;
        int num_odd = 0;
        for(int i = 0; i<Array.size(); i++)
        {
            if(Array[i]%2 == 0)
            {
                even.push_back(Array[i]);
                num_even ++;
            }
            else
            {
                odd.push_back(Array[i]);
                num_odd ++;
            }
        }
        //cout << num_even << " " << num_odd << endl;
        for(int i = 0; i<num_odd; i++)
        {
            Array[i] = odd[i];
        }
        for(int i = 0; i<num_even; i++)
        {
            Array[num_odd ++] = even[i];
        }
    }
};
int main()
{
    int a[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
    int l = sizeof(a)/sizeof(int);
    vector<int > v(a, a + l);
    solution s;
    s._reOrderArray(v);
    for(int i = 0; i<v.size(); i++)
    {
        cout << v.at(i) << " ";
    }
    cout << endl;
    return 0;
}
```
