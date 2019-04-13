
# 119. Pascal's Triangle II - OraYang的博客 - CSDN博客

2017年07月24日 10:06:40[OraYang](https://me.csdn.net/u010665216)阅读数：128所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an indexk, return thekthrow of the
 Pascal's triangle.
For example, givenk= 3,
Return`[1,3,3,1]`.
Note:
Could you optimize your algorithm to use onlyO(k) extra space?
思路：
本题由[上一篇博客](http://blog.csdn.net/u010665216/article/details/75969671)衍生而来，方法同上一篇博客，只是不需要额外数组来存储所有vector数组
代码：

```python
class Solution {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> res(0);
        if(rowIndex<0)
            return res;
        res.push_back(1);
        for(int i =1; i<=rowIndex;i++)
        {
            vector<int> temp;
            temp.push_back(1);
            for(int j=0;j<res.size()-1;j++)
            {
                temp.push_back(res[j]+res[j+1]);
            }
            temp.push_back(1);
            res = temp;
        }
        return res;
    }
};
```




