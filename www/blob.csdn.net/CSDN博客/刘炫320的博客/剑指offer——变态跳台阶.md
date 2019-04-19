# 剑指offer——变态跳台阶 - 刘炫320的博客 - CSDN博客
2017年03月27日 19:18:21[刘炫320](https://me.csdn.net/qq_35082030)阅读数：449
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 问题描述
> 
一只青蛙一次可以跳上1级台阶，也可以跳上2级……它也可以跳上n级。求该青蛙跳上一个n级的台阶总共有多少种跳法。
# 2.求解方法
这道题目之所以称为是变态跳台阶，是因为它是跳台阶的一个变种。我们来回顾一下跳台阶的题目：
> 
一只青蛙一次可以跳上1级台阶，也可以跳上2级。求该青蛙跳上一个n级的台阶总共有多少种跳法。
可以看到他们俩之间的差距，一个是只需要回顾前2个状态，一个是需要回顾前n个状态。对于跳台阶的题目来说，和斐波那契数列题目没有什么本质区别，而这个变态跳台阶，只不过是让状态回顾变成了向前看n次。
如果你知道这个的话，它的通项公式你应该可以写出来： 
$f(n)=\displaystyle \sum^{n-1}_{i=1}f(n-i)+1$
如果有第0项的话，把第0项看做是1，那么这个式子会更加漂亮： 
$f(n)=\displaystyle \sum^{n}_{i=1}f(n-i)$
但其实，如果你是傻乎乎的把前几项写出来，你会发现更惊人的一个结论： 
第一项：1，第二项：2，第三项：4，第四项：8… 
没错，其实它的通项公式就是： 
$f(n)=2^{n-1}$
下面，我们来看看不同级别的写法吧。
## 2.1 Level1
最笨的方法 
使用我们最初找到的公式： 
$f(n)=\displaystyle \sum^{n-1}_{i=1}f(n-i)+1$
```java
public static int JumpFloorII(int target) {
            int sum=1;
            for(int i=1;i<target;i++){
                sum+=JumpFloorII(target-i);
            }
            return sum;
    }
```
## 2.2 Level2
就像我们之前一样，要减少递归次数和冗余计算，于是我们把递归转成迭代，并且保存下来中间过程的结果。
```
public static int JumpFloorII1(int target) {
        int[] sum=new int[target+1];
        for(int i=1;i<=target;i++){
            for(int j=1;j<i;j++){
                sum[i]+=sum[j];
            }
            sum[i]+=1;
        }
        return sum[target];
}
```
## 2.3 Level3
如果你嫌弃之前的那种麻烦，又发现了新的规律： 
$f(n)=2\times f(n-1)$
```cpp
public static int JumpFloorII(int target) {
        if(target>0){
            int[] array=new int[target+1];
            array[1]=1;
            for(int i=2;i<=target;i++){
                array[i]=2*array[i-1];
            }
            return array[target];
        }
        else{
            return 0;
        }
    }
```
## 2.4 Level4
如果你已经看透了本质，那么我们可以使用这个公式： 
$f(n)=2^{n-1}$
```
public static int JumpFloorII(int target) {
        return (int)Math.pow(2, target-1);
    }
```
## 2.5 Level5
当然如果你是一位极客，追求的是最完美的写法：
```
public static int JumpFloorII(int target) {
        return  1<<--target;
    }
```
# 3. 算法分析
## 3.1
就我们这一题目来讲，第一种做法是极不可取的，因为它是一个重复计算的过程，并且冗余性非常大，时间复杂度已经超出想象了，具体来说是$O(2^n)$级别了，这个时间复杂度是完全没有办法接受的。
第二种做法，我们减少了大量的冗余,使用的是常规的递归转迭代，并且保存中间结果，其时间复杂度减少到$O(n^2)$的级别。
第三种做法则是使用动态规划，看到了状态转移，那么就又进一步简化了计算，其时间复杂度变为了$O（n）$
第四种做法和第五种做法则是看到了事物的本质，达到了时间复杂度的极致，也就是$O（1）$。
## 3.2 思维层面
我们已经经历和好几个类似的题目了，他们主要是使用迭代和递归，其问题描述多为第n项与前若干项之间的关系。
第一层面是只看到了大关系（根据题目描述出公式），第二层面是看到了状态转移（求出递推公式），第三层面才是看到本质（直接求通项公式），这也是我们以后需要学习和思考的地方。
# 4. 小结
我们已经做过几个关于迭代和循环的题目了，他们的解题方法都近乎类似，只要稍加总结，在正常的应用中，就可以有所突破。
