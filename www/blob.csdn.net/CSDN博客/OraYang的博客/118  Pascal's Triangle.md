
# 118. Pascal's Triangle - OraYang的博客 - CSDN博客

2017年07月24日 09:51:53[OraYang](https://me.csdn.net/u010665216)阅读数：119所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
GivennumRows, generate the firstnumRowsof Pascal's triangle.
For example, givennumRows= 5,
Return
[
     [1],
    [1,1],
   [1,2,1],
  [1,3,3,1],
 [1,4,6,4,1]
]思路：Pascal三角形，每一行除首尾为1外其余均为上一样相邻两数的和
代码：

```python
class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> res(0);
        vector<int> ress;
        if(numRows==0)
            return res;
        ress.push_back(1);
        res.push_back(ress);
        for(int i=2;i<=numRows;i++)
        {
            vector<int> temp;
            temp.push_back(1);
            for(int j = 0;j<ress.size()-1;j++)
            {
                temp.push_back(ress[j]+ress[j+1]);
            }
            temp.push_back(1);
            res.push_back(temp);
            ress = temp;
            
        }
        return res;
    }
};
```


