# 【剑指offer】题8：旋转数组的最小值 - xiaxzhou的博客 - CSDN博客





2017年06月15日 23:04:09[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：121








考虑到：

3 4 5 1 2

1 0 1 1 1

1 2 3 4 5

1

1 1 1 1

null

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <stack>

using namespace std;
#define debug_

int bin_find(vector<int>& vec, int left, int right)
{
        while (vec[left] == vec[right] && (left + 1 < right))
    {
        left += 1;
    }
    if (left +1 == right)
        return min(vec[right],vec[left]);

    if (vec[left] < vec[right])
        return vec[left];

    int mid = left + ((right - left) >> 1);

    if ( vec[mid] < vec[left] )
    {
        return bin_find(vec,left, mid);
    }
    else
    {
        return bin_find(vec, mid, right);
    }
}

int minNumberInRotateArray(vector<int> rotateArray)
{
    if (rotateArray.empty())
        throw new std::exception("error parameter");

    int left(0),right(rotateArray.size()-1);

    if (right == left)
        return rotateArray[left];

    return bin_find(rotateArray, left, right);
}

int main()
{
    vector<int> vec;
    //vector<int> vec{1,2,3};

    cout<<minNumberInRotateArray(vec);

    return 0;
}
```



