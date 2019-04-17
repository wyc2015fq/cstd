# Leetcode 1014. 在 D 天内送达包裹的能力 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 1014. 在 D 天内送达包裹的能力

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-128/problems/capacity-to-ship-packages-within-d-days/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-128/)







- 用户通过次数197
- 用户尝试次数272
- 通过次数203
- 提交次数538
- 题目难度Medium



传送带上的包裹必须在 D 天内从一个港口运送到另一个港口。

传送带上的第 `i` 个包裹的重量为 `weights[i]`。每一天，我们都会按给出重量的顺序往传送带上装载包裹。我们装载的重量不会超过船的最大运载重量。

返回能在 `D` 天内将传送带上的所有包裹送达的船的最低运载能力。



示例 1：
输入：weights = [1,2,3,4,5,6,7,8,9,10], D = 5
输出：15
解释：
船舶最低载重 15 就能够在 5 天内送达所有包裹，如下所示：
第 1 天：1, 2, 3, 4, 5
第 2 天：6, 7
第 3 天：8
第 4 天：9
第 5 天：10

请注意，货物必须按照给定的顺序装运，因此使用载重能力为 14 的船舶并将包装分成 (2, 3, 4, 5), (1, 6, 7), (8), (9), (10) 是不允许的。 

示例 2：
输入：weights = [3,2,2,4,1,4], D = 3
输出：6
解释：
船舶最低载重 6 就能够在 3 天内送达所有包裹，如下所示：
第 1 天：3, 2
第 2 天：2, 4
第 3 天：1, 4

示例 3：
输入：weights = [1,2,3,1,1], D = 4
输出：3
解释：
第 1 天：1
第 2 天：2
第 3 天：3
第 4 天：1, 1



提示：
- `1 <= D <= weights.length <= 50000`
- `1 <= weights[i] <= 500`

```
class Solution {
public:
    int shipWithinDays(vector<int>& weights, int D) {
        int r = 0;
        for(auto wei:weights) r+=wei;
        int l=1;
        while(l<r){
            int m = (l+r)/2;
            // cout << "l:" << l << " m:" << m << " r:" << r << endl;
            if(panduan(weights,m,D)) r = m;
            else l = m+1;
            cout << endl;
        }
        return l;
    }
    
    bool panduan(vector<int> weights,int count,int D){
        int pos = 0;
        for(int i=0;i < D;i++){
            int sum = 0;
            while(pos < weights.size()&&(sum += weights[pos]) <= count){
                pos++;
            }
            // cout << "pos:" << pos << " ";
            if(pos == weights.size()) return true;
        }
        return false;
    }
    
};
```

_一定要记住，提交的时候把各种输出去掉，不然会超时。。。














