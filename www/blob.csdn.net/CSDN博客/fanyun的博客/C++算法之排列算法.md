# C++算法之排列算法 - fanyun的博客 - CSDN博客
2018年06月23日 22:24:20[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：133

排列算法公式：
![](https://img-blog.csdn.net/20180623224407139?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
排列用符号A(n,m)表示，m≦n。
计算公式是：A(n,m)＝n(n-1)(n-2)……(n-m+1)＝n!/(n-m)!
此外规定0!=1，n!表示n(n-1)(n-2)…1
例如：6!=6x5x4x3x2x1=720，4!=4x3x2x1=24。
STL提供了用来计算下一个排列关系的算法，分别是next_permucation以及求上一个排列的prev_permucation
下面给出我自己的求全排列的搜索实现，代码如下：
```cpp
#include <cstdlib>
#include <vector>
#include <iostream>
using namespace std;
class Solution {
public:
    vector< vector<int> > result;
    vector<vector<int>> permute(vector<int>& nums) {
        vector<int> tmp;
        if(nums.size() <= 1)
        {
            result.push_back(nums);
            return result;
        }
        dfs(0, nums);
        return result;
    }
    void dfs(int index, vector<int> nums)
    {
        if(index == nums.size())
        {
            cout << "---" <<endl;
            vector<int> tmp;
            for(int i = 0; i < nums.size(); ++i)
            {
                tmp.push_back(nums[i]);
            }
            result.push_back(tmp);
            return;
        }
        // 将nums[index] 与后面的每个数交换，然后递归遍历交换之后的序列
        for(int j = index; j < nums.size(); ++j)
        {
            cout << "***" <<endl;
            swap(nums[j], nums[index]);
            cout << index + 1 << endl;
            dfs(index+1, nums);  
            //swap(nums[j], nums[index]);
        }
    }
    void swap(int &a, int &b)
    {
        int tmp = a;
        a = b;
        b = tmp;
    }
};
int main(int args, char* argv[])
{
    int i, j;
    int numbers[] = {1,2,3,4};
    vector<int> nums(numbers, numbers + sizeof(numbers) / sizeof(numbers[0]));
    Solution s;
    vector< vector<int> > result = s.permute(nums);
    cout << result.size() << endl;
    for (i = 0; i<result.size(); i++)
    {
        for (j = 0; j<result[i].size(); j++)
        {
            cout << result[i][j] << "  ";
        }
        cout << endl;
    }
    system("pause");
    return 0;
}
```
