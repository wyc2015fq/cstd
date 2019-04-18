# 丑数----java实现 - weixin_33985507的博客 - CSDN博客
2018年01月18日 21:05:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
# 题目：丑数
- 我们把只包含因子2，3，5的数称为丑数（Ugly Number).
- 求按从小到大的顺序的第1500个丑数。
- 例如6，8都是丑数，但14不是，因为它含有因子7.习惯上我们把1当作第一个丑数
##### 方法一：逐个判断每个整数是不是丑数，直观但不够高效：
所谓一个数m是另一个数n的因子，是指n能被m整除，也就是说n%m==0.根据丑数的定义，丑数只能被2，3，5整除。也就是说如果一个数能被2整除，我们把它连续除以2；如果能被3整除，就连续除以3；如果能被5整除，就除以5.如果最后我们得到的是1，那么这个数就是丑数，否则不是。
```
public boolean isUgly(int number){  
        while(number % 2 == 0)  
            number/=2;  
        while(number % 3 == 0)  
            number /=3;  
        while(number % 5 == 0)  
            number /=5;  
        return (number ==1)? true:false;  
    }
```
接下来，只需要按照顺序判断每个整数是不是丑数即可。
```
public int getUglyNumber(int index){  
        if(index <= 0)  
            return 0;  
        int number = 0;  
        int uglyFound = 0;  
        while(uglyFound < index){  
            number++;  
            if(isUgly(number)){  
                ++uglyFound;  
            }  
        }  
        return number;  
    }
```
我们只需要在函数getUglyNumber 中传入参数1500，就能得到第1500个丑数。该算法非常直观，代码也非常简洁，但最大的问题是每个整数都需要计算。即使一个数字不是丑数，我们还是需要对它做求余和除法操作。因此该算法的时间效率不是很高，面试官也不会就此满足，还会提示我们有更高效的算法。
##### 方法二：创建数组保存已经找到的丑数，用空间换时间的解法：
前面的算法之所以效率低，很大程度上是因为不管一个数是不是丑数我们对它都要作计算。接下来我们试着找到一种只要计算丑数的方法，而不在非丑数的整数上花费时间。根据丑数的定义，丑数应该是另一个丑数乘以2，3，5的结果。因此我们可以创建一个数组，里面的数字是排序好的丑数，每一个丑数都是前面的丑数乘以2，3，5得到的。
这种思路的关键在于怎样确定数组里面的丑数是排序好的。假设数组中已经有若干个丑数排好后存放在数组中，并且把已有的最大的丑数记作M，我们接下来分析如何生成下一个丑数。该丑数肯定是前面某个丑数乘以2，3，5的结果。所以我们首先考虑把已有的每个丑数乘以2.在乘以2的时候，能得到若干个小于或等于M的结果。由于是按照顺序生成的，小于或者等于M肯定已经在数组中了，我们不需要再次考虑；还会得到若干个大于M的结果，但我们只需要第一个大于M的结果，因为我们希望丑数是指按从小到大的顺序生成的，其他更大的结果以后再说。我们把得到的第一个乘以2后大于M的结果即为M2.同样，我们把已有的每一个丑数乘以3，5，能得到第一个大于M的结果M3和M5.那么下一个丑数应该是M2,M3,M5。这3个数的最小者。
前面分析的时候，提到把已有的每个丑数分别都乘以2，3，5.事实上这不是必须的，因为已有的丑数都是按顺序存放在数组中的。对乘以2而言，肯定存在某一个丑数T2，排在它之前的每一个丑数乘以2得到的结果都会小于已有的最大丑数，在它之后的每一个丑数乘以2得到的结果都会太大。我们只需记下这个丑数的位置，同时每次生成新的丑数的时候，去更新这个T2.对乘以3和5而言，也存在这同样的T3和T5.
因此java代码实现如下：
```
public int getUglyNumber_Solution2(int index){
        if(index <=0)
            return 0;
        int[] uglyArray = new int[index];
        uglyArray[0] = 1;
        int multiply2 = 0;
        int multiply3 = 0;
        int multiply5 = 0;
        for(int i = 1;i<index;i++){
            int min = min(uglyArray[multiply2]*2,uglyArray[multiply3]*3,uglyArray[multiply5]*5);
            uglyArray[i] = min;
            //生成新的丑数后及时更新三个游标
            while(uglyArray[multiply2]*2 <= uglyArray[i])
                ++multiply2;
            while(uglyArray[multiply3]*3 <= uglyArray[i])
                ++multiply3;
            while(uglyArray[multiply5]*5 <= uglyArray[i])
                ++multiply5;
        }
        return uglyArray[index-1];
    }
    public int min(int number1,int number2,int number3){
        int min = (number1<number2)?number1:number2;
        return min <number3?min:number3;
    }
```
和第一种思路相比，第二种思路不需要在非丑数的整数上做任何计算，因此时间效率有明显上升。但也需要指出，第二种算法由于需要保存已经生成的丑数，因此需要一个数组，从而增加了空间消耗。如果是求第1500个丑数，将创建一个能容纳1500个丑数的数组，这个数组占内存6KB。而第一种思路没有这样的内存开销。总的来说，第二种思路相当于用较少的空间换取了时间效率上的提升。
