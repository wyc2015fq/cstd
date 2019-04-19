# leetcode(15):jump game - 刘炫320的博客 - CSDN博客
2018年04月10日 22:19:48[刘炫320](https://me.csdn.net/qq_35082030)阅读数：189标签：[跳跃游戏																[jumpgame																[动态规划																[全局最优](https://so.csdn.net/so/search/s.do?q=全局最优&t=blog)](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=jumpgame&t=blog)](https://so.csdn.net/so/search/s.do?q=跳跃游戏&t=blog)
个人分类：[leetcode](https://blog.csdn.net/qq_35082030/article/category/7480326)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)[算法思想培养指南](https://blog.csdn.net/column/details/36524.html)
这次我们讲解跳跃游戏的两道题，这两道题在leetcode上分别是中等和困难的，但是其实并不比有些简单题难。
# 1.55. Jump Game
## 1.1问题描述
> 
Given an array of non-negative integers, you are initially positioned at the first index of the array. 
  给定一个非负整数数组，您最初位于数组的第一个索引处。 
  Each element in the array represents your maximum jump length at that position. 
  数组中的每个元素表示您在该位置的最大跳跃长度。 
  Determine if you are able to reach the last index. 
  确定是否能够到达最后一个索引。 
  For example: 
  A = [2,3,1,1,4], return true. 
  A = [3,2,1,0,4], return false.
## 1.2思路
这道题是中等题目，其思路在于，我们想看看能走的最远的路径有没有到达终点。这就像人一样，我们每走一步都要往前看一下，主要看两点，第一，我们能不能到达这个点，第二，这个点能到达的最远的地方是哪里，然后比较全局最远的地方，这样我们就可以进行一个迭代的过程。
## 1.3代码
```java
public boolean canJump(int[] nums) {
        //当前能跑的最远的位置
        int cur=0;
        for(int i=0;i<nums.length;i++){
            //如果i位置能够到达
            if(i<=cur){
                //i点可以到达的最远的距离
                int tempnext=i+nums[i];
                //把当前能到达最远的位置进行更新
                cur=tempnext>cur?tempnext:cur;
            }
            //否则则不能到达i节点，更不可能到达最后节点了。
            else{
                return false;
            }
        }
        //最后位置都可以到达
        return true;
    }
```
# 2.45. Jump Game II
## 2.1问题描述
> 
Given an array of non-negative integers, you are initially positioned at the first index of the array. 
  给定一个非负整数数组，你最初位于数组的首位。 
  Each element in the array represents your maximum jump length at that position. 
  数组中的每个元素表示你在该位置的最大跳跃长度。 
  Your goal is to reach the last index in the minimum number of jumps. 
  你的目标是用最小跳跃次数到达最后一个索引。 
  For example: 
  Given array A = [2,3,1,1,4] 
  The minimum number of jumps to reach the last index is 2. (Jump 1 step from index 0 to 1, then 3 steps to the last index.) 
  Note: 
  You can assume that you can always reach the last index.
## 2.2思路
这道题和刚才那道题思路一样，但是这道题主要要记录步数，要注意的一点就是最少步数肯定是每次都走最远。这样就能找到突破口了。
## 2.3代码
```java
public int jump(int[] nums) {
        int jumps = 0;
        int bound = 0;
        int nextBound = 0;
        int l = nums.length - 1;
        for(int i = 0; i < l; i++) {
            int val = i + nums[i];
            //如果当前最远步骤没有当前步骤能走最远的
            if(nextBound < val) {
                nextBound = val;
            }
            //如果到达最远步骤
            if(i == bound) {
                jumps++;
                bound = nextBound;
            }
        }
        return jumps;
    }
```
# 3.小结
我们这次讲解了跳跃游戏的2个题目，它是被划为动态规划的题目，但是它的动态规划比较简单，只需要记住一个局部最优和全局最优即可，而且没有累积的步骤，因此相对简单一些。
