# 返回数组中任意某个重复的数字--时间复杂度O(n)空间复杂度O(1) - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年03月12日 19:54:21[QuJack](https://me.csdn.net/A784586)阅读数：536








题目：在一个长度为n的数组里的所有数字都在0到n-1的范围内。 数组中某些数字是重复的，但不知道有几个数字是重复的。也不知道每个数字重复几次。请找出数组中任意一个重复的数字。 例如，如果输入长度为7的数组{2,3,1,0,2,5,3}，那么对应的输出是重复的数字2或者3。

思路分析：

思路1：将数组排序，然后遍历数组每次判断当前元素和下一个元素如果相等则返回。时间复杂度排序O（nlogn）.

思路2：用hash表，比如用数组，初始化一个大小为N的数组为0，遍历寻找的数组，找到元素值为下标的位置置1.找到第二个同样元素时候，发现是1，则返回该元素。比如：遍历numbers[3]===5,
  判断辅助数组a[5]是否为0，如果为0，说明是第一次找到5，置1.后面再找到numbers[8]==5,此时准备将辅助数组a[5]置1，发现已经是1，则找到重复元素。时间复杂度O（n）,空间复杂度O(n).【数组可以模拟简单的hash表，标记法】

思路3：时间复杂度O（n）,空间复杂度O（1）。由于数组的值都在0----n-1之间，如果排序后，应该有些位置有相同元素或者有一个元素出现在两个位置，扫描遍历数组，当numbers[i]!=i,
 说明该元素应该出现在下标为numbers[i]的位置，当numbers[i]!=numbers[numbers[i]],交换，当numbers[i]
  ==numbers[numbers[i]]则找到两个位置有同样的数组，返回。




**java实现代码如下：**





```java
public class Solution {
    // Parameters:
    //    numbers:     an array of integers
    //    length:      the length of array numbers
    //    duplication: (Output) the duplicated number in the array number,length of duplication array is 1,so using duplication[0] = ? in implementation;
    //                  Here duplication like pointor in C/C++, duplication[0] equal *duplication in C/C++
    //    这里要特别注意~返回任意重复的一个，赋值duplication[0]
    // Return value:       true if the input is valid, and there are some duplications in the array number
    //                     otherwise false
    public boolean duplicate(int numbers[],int length,int [] duplication) {
        //异常处理
    	if(numbers==null || length<0)
            return false;
        for(int number:numbers)
            if(number<0 || number>length-1)  return false;
        
       for(int i=0;i<length;i++){
        while(numbers[i]!=i){
        	if(numbers[i]==numbers[numbers[i]]){
           		 duplication[0]=numbers[i];
          		  return true;
        		}
        //交换
        int temp=numbers[numbers[i]];
        numbers[numbers[i]]=numbers[i];
        numbers[i]=temp;
        } 
    }
        
        return false;
               
    }
}
```





