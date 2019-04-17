# Lintcode——100. 删除排序数组中的重复数字 - westbrook1998的博客 - CSDN博客





2018年03月30日 15:32:00[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：55








> 
给定一个排序数组，在原数组中删除重复出现的数字，使得每个元素只出现一次，并且返回新的数组的长度。 
**不要使用额外的数组空间，必须在原地没有额外空间的条件下完成。**

  样例 

  给出数组A =[1,1,2]，你的函数应该返回长度2，此时A=[1,2]。
代码：

```
public class Solution {
    /*
     * @param nums: An ineger array
     * @return: An integer
     */
    public int removeDuplicates(int[] nums) {
        // write your code here
        int len=nums.length;
        //处理空数组
        if(len==0){
            return 0;
        }
        int count=1;
        int i=1;
        //当指针位置小于数组长度（可变）时循环进行
        while(i<len){
            //如果该位置数字与前一个数相同
            if(nums[i]==nums[i-1]){
                //通过数组的复制将这个数字覆盖，原数组多出最后一个数
                System.arraycopy(nums,i+1,nums,i,len-i-1);
                //将数组长度减一，即将最后一个数组排除
                len--;
                //指针无需后移，因为相当于后面的数字前移
            }
            //如果该位置数组与前一个数不同
            else{
                //则新数组长度加1，初始值为1，因为非空数组至少一个不同的数
                count++;
                //指针后移
                i++;
            }
        }
        //返回新数组长度
        return count;
    }
}
```





