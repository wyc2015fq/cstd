
# 605. Can Place Flowers - OraYang的博客 - CSDN博客

2017年08月29日 10:10:16[OraYang](https://me.csdn.net/u010665216)阅读数：223所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Suppose you have a long flowerbed in which some of the plots are planted and some are not. However, flowers cannot be planted in adjacent plots - they would compete for water and both would die.
Given a flowerbed (represented as an array containing 0 and 1, where 0 means empty and 1 means not empty), and a numbern, return ifnnew flowers can be planted in it without violating
 the no-adjacent-flowers rule.
Example 1:
Input:flowerbed = [1,0,0,0,1], n = 1Output:True
Example 2:
Input:flowerbed = [1,0,0,0,1], n = 2Output:False
Note:
The input array won't violate no-adjacent-flowers rule.
The input array size is in the range of [1, 20000].
nis a non-negative integer which won't exceed the input array size.
思路：本题，只要就算可以插花的个数，每连续三个0可以插一朵花，但是要注意首尾为0的特殊情况
代码：

```python
class Solution {
public:
    bool canPlaceFlowers(vector<int>& flowerbed, int n) {
        int i = 0;
        while(i<flowerbed.size())
        {
            if(flowerbed[i]==0&&(i==0||flowerbed[i-1]==0)&&(i==flowerbed.size()-1||flowerbed[i+1]==0))
            {
                n--;
                flowerbed[i] = 1;
            }
            i++;
        }
    return n<1?true:false;
    }
};
```


