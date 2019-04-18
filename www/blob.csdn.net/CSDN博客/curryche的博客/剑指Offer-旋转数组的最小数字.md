# 剑指Offer - 旋转数组的最小数字 - curryche的博客 - CSDN博客





2018年08月01日 21:11:58[curryche](https://me.csdn.net/whwan11)阅读数：32标签：[剑指offer																[数组](https://so.csdn.net/so/search/s.do?q=数组&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

旋转数组的最小数字 

时间限制：3秒 空间限制：32768K 热度指数：358245 

本题知识点： 查找
题目描述 

把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。 输入一个非减排序的数组的一个旋转，输出旋转数组的最小元素。 例如数组{3,4,5,1,2}为{1,2,3,4,5}的一个旋转，该数组的最小值为1。 NOTE：给出的所有元素都大于0，若数组大小为0，请返回0。

## 解法

代码

```cpp
class Solution {
public:
    int minNumberInRotateArray(vector<int> rotateArray) 
{
    if (rotateArray.empty())
        return 0;

    if (rotateArray.size() == 1)
        return rotateArray[0];

    int low, high, middle;
    low = 0;
    high = rotateArray.size() - 1;


    while ((low+1)<high)
    {
        middle = (low+high) / 2;
        if (rotateArray[low] == rotateArray[middle] && rotateArray[middle] == rotateArray[high])
        {
            //当三者相等时只能顺序查找
            int min_num = rotateArray[low];
            for (int i = low + 1; i < high; i++)
            {
                if (rotateArray[i] < min_num)
                    min_num = rotateArray[i];
            }
            return min_num;
        }
        else if (rotateArray[low] <= rotateArray[middle] && rotateArray[middle] >= rotateArray[high])
            low = middle;
        else
            high = middle;
    }
    return rotateArray[high];
}
};
```





