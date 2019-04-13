
# 剑指offer——变态跳台阶 - manduner的博客 - CSDN博客


2019年03月08日 21:25:54[Manduner_TJU](https://me.csdn.net/manduner)阅读数：30


# 1，问题描述
一只青蛙一次可以跳上1级台阶，也可以跳上2级……它也可以跳上n级。求该青蛙跳上一个n级的台阶总共有多少种跳法。
# 2，解题思路
解题思路同上一篇博客[跳台阶](https://blog.csdn.net/manduner/article/details/88358425)
# 3，源码
```python
public class Solution {
    public int JumpFloorII(int target) {
        if(target==0){
            return 0;
        }else if(target==1){
            return 1;
        }else if(target==2){
            return 2;
        }else{
            int sum=1;//一下子跳n阶台阶
            for(int i=target-1;i>=1;i--){
                sum += JumpFloorII(i);
            }
            return sum;
        }
    }
}
```


