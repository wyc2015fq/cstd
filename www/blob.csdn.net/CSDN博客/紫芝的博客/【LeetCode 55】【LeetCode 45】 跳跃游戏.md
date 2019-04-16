# 【LeetCode 55】【LeetCode 45】 跳跃游戏 - 紫芝的博客 - CSDN博客





2018年11月10日 23:09:10[紫芝](https://me.csdn.net/qq_40507857)阅读数：217








### 55. 跳跃游戏



给定一个非负整数数组，你最初位于数组的第一个位置。

数组中的每个元素代表你在该位置可以跳跃的最大长度。

判断你是否能够到达最后一个位置。

**示例 1:**
**输入:** [2,3,1,1,4]
**输出:** true
**解释:** 从位置 0 到 1 跳 1 步, 然后跳 3 步到达最后一个位置。

**示例 2:**
**输入:** [3,2,1,0,4]
**输出:** false
**解释:** 无论怎样，你总会到达索引为 3 的位置。但该位置的最大跳跃长度是 0 ， 所以你永远不可能到达最后一个位置。
解法1：

贪心，遍历所有的元素，维护的指标为当前能够达到的最大值，该最大值由上一次跳跃的最大值和这次跳跃的距离取较大值
- 一定要遍历所有的元素，而不是跳跃
- maxreach的更新方法：max（上一次跳跃的最大值，本次跳跃的最大距离）

```cpp
#include<bits/stdc++.h>
using namespace std;
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int N=nums.size();
        int maxreach=0;//注意是下标值而不是元素值
        for(int i=0;i<N;i++){
            //maxreach停止了，i仍然在增加
            if(i>maxreach)
                return false;
            maxreach=max(maxreach,i+nums[i]);//更新
            if(maxreach>=N-1)
                return true;
        }
    }//bool
};
int main(){
    int n;
    cin>>n;
    vector<int>a;
    for(int i=0;i<n;i++)
    {
        int x;
        cin>>x;
        a.push_back(x);
    }
    Solution work;
    if(work.canJump(a))
        cout<<"true"<<endl;
    else
        cout<<"false"<<endl;
}
/*
2 3 1 1 4
3 2 1 0 4
3 0 8 2 0 0 7
*/
```

解法2

由于数组元素大于等于0，所以当且仅当数组中有0，才可能过不去；其他情况一定可以跳到终点。

因此找到数组中的0元素，对于每一个0元素，查看其是否可以过去；

重点：不能过去的充要条件是：0元素之前所有元素的值不大于该元素与0元素的距离

```cpp
#include<bits/stdc++.h>
using namespace std;
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int N=nums.size();
        if(N<=1)    return true;
        if(nums[0]==0)  return false;//点数N>1,不可能跳到终点
        for(int i=1;i!=N-1;i++){//遍历:第2个点到倒数第2个点
            if(nums[i]==0){
                vector<int>subnums(nums.begin(),nums.begin()+i);
                if(!jumpZERO(subnums))
                    return false;
            }
        }
        return true;
    }//bool
private:
    bool jumpZERO(vector<int>& subnums){
        vector<int>::iterator it;
        for(it=subnums.begin();it!=subnums.end();it++){
            //当前点可以跳到末尾0，就可以通过
            if(*it>(subnums.end()-it))
                return true;
        }
        return false;
    }
};
int main(){
    int n;
    cin>>n;
    vector<int>a;
    for(int i=0;i<n;i++)
    {
        int x;
        cin>>x;
        a.push_back(x);
    }
    Solution work;
    if(work.canJump(a))
        cout<<"true"<<endl;
    else
        cout<<"false"<<endl;
}
/*
2 3 1 1 4
3 2 1 0 4
3 0 8 2 0 0 7
*/
```

### 45. 跳跃游戏 II



给定一个非负整数数组，你最初位于数组的第一个位置。

数组中的每个元素代表你在该位置可以跳跃的最大长度。

你的目标是使用最少的跳跃次数到达数组的最后一个位置。

**示例:**
**输入:** [2,3,1,1,4]
**输出:** 2
**解释:** 跳到最后一个位置的最小跳跃数是 2。从下标为 0 跳到下标为 1 的位置，跳 1步，然后跳 3 步到达数组的最后一个位置。
这道题的要求是给定一整数数组，数组元素表示每次可以跳跃的最大距离。然后初始位置在数组的第一个元素，目的是以最少的步数到达最后元素。

这道题是Jump Game II，是其Jump Game的扩展。

在Jump Game中，采用贪心的思路，采用reach变量维护能到达最远处，即为全局最优解。当遍历到i的时候，局部最优解为A[i]+i，因此，此时的全局最优解即为reach和A[i]+i的最大值：reach = max(reach, A[i] + i)。

而这道题，需要求的是最少的步数。因此需要添加step变量记录最少步数。至于什么时候step需要加1？答案是当前的i超过了前一步的最远位置。所以引入last变量记录上一步能到达的最远位置。reach、step、last的初始值均为0。当遍历到i的时候，如果i超过了last（即上一步能到达的最远位置），说明步数需要加1（即step++），此时仍需要更新last为当前最远位置reach。全程只需遍历1次数组，而且空间复杂度为常量。

时间复杂度：O(n)

空间复杂度：O(1)

```cpp
class Solution {
public:
    int jump(vector<int>& nums) {
        int n=nums.size();
        int reach=0;
        int step=0;
        int last=0;//上一步能达到的最远位置
        for(int i=0;i<=reach&&i<n;i++){
            if(i>last){
                ++step;
                last=reach;
            }
            reach=max(reach,nums[i]+i);
        }
        return reach>=n-1?step:0;
    }
};
```





