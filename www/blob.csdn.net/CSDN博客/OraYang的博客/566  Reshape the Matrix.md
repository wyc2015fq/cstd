
# 566. Reshape the Matrix - OraYang的博客 - CSDN博客

2017年08月27日 11:20:58[OraYang](https://me.csdn.net/u010665216)阅读数：391所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
In MATLAB, there is a very useful function called 'reshape', which can reshape a matrix into a new one with different size but keep its original data.
You're given a matrix represented by a two-dimensional array, and twopositiveintegersrandcrepresenting therownumber
 andcolumnnumber of the wanted reshaped matrix, respectively.
The reshaped matrix need to be filled with all the elements of the original matrix in the samerow-traversingorder as they were.
If the 'reshape' operation with given parameters is possible and legal, output the new reshaped matrix; Otherwise, output the original matrix.
Example 1:
Input:nums = 
[[1,2],
 [3,4]]
r = 1, c = 4Output:[[1,2,3,4]]Explanation:
Therow-traversingof nums is [1,2,3,4]. The new reshaped matrix is a 1 * 4 matrix, fill it row by row by using the previous list.
Example 2:
Input:nums = 
[[1,2],
 [3,4]]
r = 2, c = 4Output:[[1,2],
 [3,4]]Explanation:
There is no way to reshape a 2 * 2 matrix to a 2 * 4 matrix. So output the original matrix.
Note:
The height and width of the given matrix is in range [1, 100].
The given r and c are all positive.
思路：本题，分为以下几个步骤：
step1:判断reshape是都可行，不可行返回原数组，可行转step2
step2:建立一个vector一维数组存储nums值
step3:维护一个迭代器指针，根据要求构建数组
代码：

```python
class Solution {
public:
    vector<vector<int>> matrixReshape(vector<vector<int>>& nums, int r, int c) {
        int n_r = nums.size(),n_c = nums[0].size();
        vector<vector<int>> res;
        vector<int> temp,store;
        cout<<n_c<<endl;
        if(r*c==n_r*n_c)
        {
            for(int i = 0;i<n_r;i++)
            {
                for(int j=0;j<n_c;j++)
                {
                    store.push_back(nums[i][j]);
                }
            }
            vector<int>::iterator itr = store.begin();
            for(int z = 0;z<r;z++)
            {
                temp.clear();
                for(int y=0;y<c;y++)
                {
                    temp.push_back(*itr);
                    itr++;
                }
                res.push_back(temp);
            }
        }
        else
            return nums;
        return res;
    }
};
```


