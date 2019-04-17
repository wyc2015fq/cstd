# Lintcode181——将整数A转换为B - westbrook1998的博客 - CSDN博客





2018年02月19日 19:21:46[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：136








## 题目：

> 
如果要将整数A转换为B，需要改变多少个bit位？  

样例 

  如把31转换为14，需要改变2个bit位。 

  (31)10=(11111)2 

  (14)10=(01110)2
## 题解：

### 解法一：

思路分析：先将两个数异或得到的二进制数中，1代表需要改变的位，所以题目就变成求一个数的二进制有多少个1  

```
public class Solution {
    /*
     * @param a: An integer
     * @param b: An integer
     * @return: An integer
     */
    public int bitSwapRequired(int a, int b) {
        // write your code here
        int t=a^b;
        int count=0;
        int flag=1;
        //flag从0000....0001 循环向左移位
        //t和flag与运算相当于判断t对应位是不是1
        while(flag!=0){
            if((t&flag)!=0){    //要加括号     
                count++;
            }
            flag<<=1;
        }
        return count;
    }
}
```

### 解法二：

思路分析：计算二进制含1位数的不同思路，因为一个数与运算本身减1的数，会将该整数最右边的1变为0，所以重复操作循环到0，即得到1的位数  

```
public class Solution {
    /*
     * @param a: An integer
     * @param b: An integer
     * @return: An integer
     */
    public int bitSwapRequired(int a, int b) {
        // write your code here
        int t=a^b;
        int count=0;
        while(t!=0){
            t&=(t-1);
            count++;
        }
        return count;
    }
}
```





