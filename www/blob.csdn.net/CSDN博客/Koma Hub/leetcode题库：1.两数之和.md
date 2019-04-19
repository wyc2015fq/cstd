# leetcode题库：1.两数之和 - Koma Hub - CSDN博客
2018年04月13日 22:56:43[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：2196
                给定一个整数数组和一个目标值，找出数组中和为目标值的两个数。
你可以假设每个输入只对应一种答案，且同样的元素不能被重复利用。
示例:
```
给定 nums = [2, 7, 11, 15], target = 9
因为 nums[0] + nums[1] = 2 + 7 = 9
所以返回 [0, 1]
```
我的答案：
```cpp
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target) {
    int i,j,sum=-1,io,jo,flag=0;
    for(i=0;i<numsSize;i++)
    {
        for(j=0;j<numsSize;j++){
            if(i!=j){
                sum = nums[i]+nums[j];
                if(sum == target){
                    io = i;
                    jo = j;
                    flag = 1;
                    break;
                }
            }
        }
        if(flag)break;
    }
    int *out = malloc(sizeof(int)*2);
    out[0] = io<jo?io:jo;
    out[1] = io>jo?io:jo;
    return out;
}
```
代码执行结果：
我的输入
[2,7,11,15]
9
我的答案
[0,1]
预期答案
[0,1]
