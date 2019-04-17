# Leetcode 89 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





回溯写到自闭；不想就删了；

```
class Solution {
public:
    vector<int> grayCode(int n) {
        vector<vector<int>> res;
        vector<int> add(n,0);
        DFS(res,add,n,0);
        vector<int> realres;
        for(int i=0;i < res.size();i++){
            realres.push_back(func(res[i]));
        }
        return realres;
    }

    int func(vector<int> nums){
        int res = 0;
        int x = 1;
        for(int i=nums.size()-1;i >= 0;i--){
            if(nums[i] == 1){
                res += x;
            }
            x = x*2;
        }
        return res;
    }

    int trans(int a){
        if(a == 0) return 1;
        else return 0;
    }



    void DFS(vector<vector<int>>& res,vector<int> add,int n,int pos){ 
        if(add.size() == pos){
            res.push_back(add);
        }
        else{
            DFS(res,add,n,pos+1); //其实就是递归的顺序不对！改不来
            add[pos] = trans(add[pos]);
            DFS(res,add,n,pos+1);
        }
    }
};
```

 正解：（数学方法）

```
// Binary to grey code
class Solution {
public:
    vector<int> grayCode(int n) {
        vector<int> res;
        for (int i = 0; i < pow(2,n); ++i) {
            res.push_back((i >> 1) ^ i);
        }
        return res;
    }
};
```













