# Leetcode 870. 优势洗牌 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 870. 优势洗牌

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-93/problems/advantage-shuffle/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-93/)







- 用户通过次数49
- 用户尝试次数92
- 通过次数49
- 提交次数192
- 题目难度Medium



给定两个大小相等的数组 `A` 和 `B`，A 相对于 B 的*优势*可以用满足 `A[i] > B[i]` 的索引 `i` 的数目来描述。

返回 `A` 的任意排列，使其相对于 `B` 的优势最大化。



示例 1：
输入：A = [2,7,11,15], B = [1,10,4,11]
输出：[2,11,7,15]

示例 2：
输入：A = [12,24,8,32], B = [13,25,32,11]
输出：[24,32,8,12]



提示：
- `1 <= A.length = B.length <= 10000`
- `0 <= A[i] <= 10^9`
- `0 <= B[i] <= 10^9`

```
class Solution {
public:
    vector<int> advantageCount(vector<int>& A, vector<int>& B) {
        vector<pair<int,int>> mp;
        vector<int> ans(A.size());
        
        for(int i=0;i < B.size();i++){
            mp.push_back(make_pair(B[i],i)); // make_pair 类似map(key->value)
        }
        
        sort(A.begin(),A.end());
        sort(mp.begin(),mp.end()); // 按照key的大小排序
        
        for(int i=0;i < B.size();i++){
            vector<int>::iterator it = upper_bound(A.begin(),A.end(),mp[i].first); //在A中找到B[i]上界,返回的是A的指针，所以用vector迭代器
            if(it != A.end()){  // 找到了
                ans[mp[i].second] = *it;
                *it = -1;        //遍历过的A中数组都置为-1
            }
            else{             //超出A上界后，只要没有访问过的A[i]就随便插入到ans中
                int pos = 0;
                for(pos=0;pos<A.size();pos++){if(A[pos] != -1)break;}
                ans[mp[i].second] = A[pos];
                A[pos] = -1;
            }
        }
        return ans;      
    }
};
```

_思路很好发现，但代码能力有限，看别人代码实现了
 vector<int>::iterator it = upper_bound(A.begin(),A.end(),mp[i].first); 重点

lower_bound( begin,end,num)：从数组的begin位置到end-1位置二分查找第一个大于或等于num的数字，找到返回该数字的地址，不存在则返回end。通过返回的地址减去起始地址begin,得到找到数字在数组中的下标。














