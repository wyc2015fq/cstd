# Leetcode 970. 强整数 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 970. 强整数

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-118/problems/powerful-integers/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-118/)







- 用户通过次数223
- 用户尝试次数258
- 通过次数231
- 提交次数801
- 题目难度Easy



给定两个正整数 `x` 和 `y`，如果某一整数等于 `x^i + y^j`，其中整数 `i >= 0` 且 `j >= 0`，那么我们认为该整数是一个*强整数*。

返回值小于或等于 `bound` 的所有*强整数*组成的列表。

你可以按任何顺序返回答案。在你的回答中，每个值最多出现一次。



示例 1：
输入：x = 2, y = 3, bound = 10
输出：[2,3,4,5,7,9,10]
解释： 
2 = 2^0 + 3^0
3 = 2^1 + 3^0
4 = 2^0 + 3^1
5 = 2^1 + 3^1
7 = 2^2 + 3^1
9 = 2^3 + 3^0
10 = 2^0 + 3^2

示例 2：
输入：x = 3, y = 5, bound = 15
输出：[2,4,6,8,10,14]



提示：
- `1 <= x <= 100`
- `1 <= y <= 100`
- `0 <= bound <= 10^6`

```
class Solution {
public:
    vector<int> powerfulIntegers(int x, int y, int bound) {
        set<int> jihe;
        vector<int> ans;
        if(x == 1&&y == 1){
            if(bound < 2) return ans;
            else{
                ans.push_back(2);
                return ans;
            }
        }
        if(x == 1){
            for(int j=0;1+pow(y,j)<=bound;j++){
                jihe.insert(1+pow(y,j));
            }
        }
        else{
            for(int i=0;pow(x,i) <= bound;i++){
                if(y == 1){if(pow(x,i)+1<=bound)jihe.insert(pow(x,i)+1);}
                else{
                    for(int j=0;pow(x,i)+pow(y,j)<=bound;j++){
                        jihe.insert(pow(x,i)+pow(y,j));
                    }
                }
            }
        }
        vector<int> res;
        set<int>::iterator iter;
        for(iter = jihe.begin();iter != jihe.end();iter++){
            res.push_back(*iter);
        }
        return res;
    }
};
```

————主要是第一个数是1比较烦

我真的蠢，看别人的解法，只要加点限制就好了

i = bound if x == 1 else x j = bound if y == 1 else y

```
class Solution {
public:
    vector<int> powerfulIntegers(int x, int y, int bound) {
        set<int> ans;
        vector<int> res;
        for(int i = 1; i<=bound; i = i*x){
            for(int j= 1; j<=bound;j = j*y){
                if(i+j <=bound){
                    ans.insert(i+j);
                }
                if(y==1){
                    break;
                }
            }
            if( x == 1){
                break;
            }
        }
        
        for(auto tmp:ans){
            res.push_back(tmp);
        }
        return res;
    }
};
```

——看看别人的，我特么写的是坨屎 啊














