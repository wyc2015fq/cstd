
# 167. Two Sum II - Input array is sorted - OraYang的博客 - CSDN博客

2017年07月26日 10:15:35[OraYang](https://me.csdn.net/u010665216)阅读数：186标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an array of integers that is alreadysorted in ascending order, find two numbers such that they add up to a specific target number.
The function twoSum should return indices of the two numbers such that they add up to the target, where index1 must be less than index2. Please note that your returned answers (both index1 and index2)
 are not zero-based.
You may assume that each input would haveexactlyone solution and you may not use thesameelement twice.
Input:numbers={2, 7, 11, 15}, target=9
Output:index1=1, index2=2
思路：本题其实很简单，利用两个for循环即可，还有一种O(n)的思路，即利用两个指针从首尾遍历，求和sum,sum==target,返回对应index，sum>target，尾指针左移，sum<target,首指针右移。
代码1（两个for循环）：

```python
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int index_t;
        vector<int> res(0);
        for(int i=0;i<numbers.size()-1;i++)
        {
            if(numbers[numbers.size()-1]<=target)
            {
                index_t = numbers.size()-1;
                break;
            }
            if(numbers[i]<=target&&target<=numbers[i+1])
            {
                index_t = i+1;
                break;
            }
        }
        cout<<index_t<<endl;
        for(int j = index_t;j>0;j--)
        {
            for(int z =0;z<j;z++)
            {
                if((numbers[z]+numbers[j])==target)
                {
                    res.push_back(z+1);
                    res.push_back(j+1);
                    return res;
                }
                    
            }
            
        }
        return res;
    }
};
```
代码2（两个指针）：
```python
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int l = 0, r = numbers.size() - 1;
        while (l < r) {
            int sum = numbers[l] + numbers[r];
            if (sum == target) return {l + 1, r + 1};
            else if (sum < target) ++l;
            else --r;
        }
        return {};
    }
};
```




