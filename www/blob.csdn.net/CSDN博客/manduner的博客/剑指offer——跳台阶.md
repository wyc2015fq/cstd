
# 剑指offer——跳台阶 - manduner的博客 - CSDN博客


2019年03月08日 21:20:07[Manduner_TJU](https://me.csdn.net/manduner)阅读数：15标签：[剑指offer																](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)



# 1，问题描述
一只青蛙一次可以跳上1级台阶，也可以跳上2级。求该青蛙跳上一个n级的台阶总共有多少种跳法（先后次序不同算不同的结果）。
# 2，解题思路
（1）先假设有n阶台阶的时候有f(n)种跳法
（2）当n = 1时只有一种跳法，即，f(1) = 1；
当n = 2时有两种跳法，即，f(2) = 2;
当n = 3时，一共有多少种跳法，**只和第一次跳的选择有关**，当第一次跳1级台阶时，台阶剩余2阶，那么剩余的2阶就有f(2)种跳法；当第一次跳2级台阶时，台阶剩余1阶，那么剩余的1阶就有f(1)种跳法。所以总共有f(3) = f(2) + f(1) 种跳法。依次类推，f(n) = f(n-1) + f(n-2)。图形实例如下（**其实，整个算法可以用一句话概括：确定了第一次跳几阶台阶，剩下的台阶跳法和之前的是一样的**）：
![](https://img-blog.csdnimg.cn/20190308211309140.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy,size_16,color_FFFFFF,t_70)
# 3，源码
```python
public class Solution {
    public int JumpFloor(int target) {
        if(target==0){
            return 0;
        }else if(target==1){
            return 1;
        }else if(target==2){
            return 2;
        }else{
            return JumpFloor(target-2) + JumpFloor(target-1);
        }
    }
}
```


