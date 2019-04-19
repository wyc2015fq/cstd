# 剑指offer——连续子数组的最大和 - 刘炫320的博客 - CSDN博客
2017年07月22日 21:49:59[刘炫320](https://me.csdn.net/qq_35082030)阅读数：591标签：[剑指offer-java实现																[连续子数组最大和																[动态规划																[局部最优](https://so.csdn.net/so/search/s.do?q=局部最优&t=blog)](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=连续子数组最大和&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer-java实现&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/qq_35082030/article/category/6807625)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 题目描述
> 
HZ偶尔会拿些专业问题来忽悠那些非计算机专业的同学。今天测试组开完会后,他又发话了:在古老的一维模式识别中,常常需要计算连续子向量的最大和,当向量全为正数的时候,问题很好解决。但是,如果向量中包含负数,是否应该包含某个负数,并期望旁边的正数会弥补它呢？例如:{6,-3,-2,7,-15,1,2,2},连续子向量的最大和为8(从第0个开始,到第3个为止)。你会不会被他忽悠住？(子向量的长度至少是1)
# 2. 解决方法
这里的题目和全为正数的题目有点类似，下面看一级一级做法吧。
## 2.1 level1
如果什么算法都没有考虑，直接做的话，当然是通过一个例子来看了。 
首先我们把一个数组分为这么几个部分：
第一部分是从开头到第一个大于0的，之前肯定是都要舍弃的，
第二部分是从第一个大于0的数开始非负的连续子串，
第三部分是第二个负子串，
第四部分则是剩下部分，这部分我们使用递归来计算。
这样我们的思路就明确了，如果全为负数，则返回最大的那个负数。
如果第二部分与第三部分之和小于0，如果大于0，则返回第四部分，否则的话返回0。
如果三部分与第四部分之和是大于0的，就把这部分加上，否则只返回第二个。
```cpp
public static int FindGreatestSumOfSubArray(int[] array) {
        if(array==null||array.length==0){
            return 0;
        }
        //寻找第一个大于0的数
        int start=0;
        int max=Integer.MIN_VALUE;
        for(;start<array.length;start++){
            if(array[start]>0){
                break;
            }
            else{
                if(max<array[start]){
                    max=array[start];
                }
            }
        }
        if(start==array.length){
            return max;
        }
        //统计第一个大于0的子串
        int sum=0;
        for(;start<array.length;start++){
            if(array[start]>0){
                sum+=array[start];
            }
            else{
                break;
            }
        }
        int fu=0;
        //统计接下来的负子串
        for(;start<array.length;start++){
            if(array[start]<0){
                fu+=array[start];
            }
            else{
                break;
            }
        }
        int[] nextarray =new int[array.length-start];
        System.arraycopy(array, start, nextarray, 0, array.length-start);
        int next=FindGreatestSumOfSubArray(nextarray);
        //如果负数大于前面之和，如果后面大于0，就返回，否则返回0；
        if(-fu>sum){
            if(next>0)
                return next;
            else
                return 0;
        }
        //如果后面的数目是大于0的，就加上，否则不加。
        if(next>-fu){
            return sum+next+fu;
        }
        else{
            return sum;
        }
    }
```
## 2.2 level2
而正常的解法应当如下：遍历，遇到负和抛弃之前的结果，重新积累，期间保留最大值.这就有点局部模型的想法了。
```cpp
int FindGreatestSumOfSubArray(int[] array) {
        if(array==null || array.length==0)
            return 0;
        int cSum = 0;
        int result = array[0]; // result存储最大和，不能初始为0，存在负数
        for(int i = 0;i<array.length;++i)
        {
            if(cSum < 0) // 当前和<0，抛弃不要
                cSum = array[i];
            else
                cSum += array[i];
            if(cSum > result) // 存储最大结果
                result = cSum;
        }
        return result;
    }
```
## 2.3 level3
如果使用动态规划的话，就容易的多了。通过观察可以看到这样一个公式：
```php
dp[i] = max{dp[i-1]+array[i],array[i]}
```
而这里就是要找到dp中的最大值，所以只需要如下代码：
```cpp
public int FindGreatestSumOfSubArray(int[] array) {
            if(array==null || array.length==0) return 0;
            int mx = 0x80000000;
            for(int i = 0, s = 0; i < array.length; ++i){
                //动态规划的部分
                s =s + array[i]>array[i]?s + array[i]:array[i];
                //保留最大值
                mx = mx>s?mx:s;
            }
            return mx;
        }
```
# 3. 小结
通过上题我们可以发现，越是看透问题的本质，越容易解答。这个题目在动态规划里算是初级的。由此可想而知，一定要发现局部模型，也就是local model，这样才能够简化计算。
