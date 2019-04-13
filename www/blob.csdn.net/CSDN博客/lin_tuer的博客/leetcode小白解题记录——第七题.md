
# leetcode小白解题记录——第七题 - lin_tuer的博客 - CSDN博客


2016年09月14日 20:35:33[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：1240


### 7. Reverse Integer
QuestionEditorial Solution
[My Submissions](https://leetcode.com/problems/reverse-integer/submissions/)

Total Accepted:165524
Total Submissions:696092
Difficulty:Easy

Reverse digits of an integer.
Example1:x = 123, return 321
Example2:x = -123, return -321
[click to show spoilers.](https://leetcode.com/problems/reverse-integer/#)
这个问题也比较直观，关键是一个考虑运算的时候有可能出现溢出问题，对于溢出问题，有如下的解决方案：
1.

```cpp
class Solution {
public:
    int reverse(int x) {

        double s=0;//一定要设置成double,否则 下面while循环里 s = s * 10 + x % 10; s每次都乘以10，可能会导致整数溢出
        int flag=1;
        if(x==0) return 0;
        if(x<0) 
        {
            flag=-1;
            x=x*(-1);
        }
        while(x>0){
            s = s * 10 + x % 10;
            x=x/10;
        }
        s=s*flag;
        return (s > INT_MAX || s < INT_MIN? 0 : s);
    }
};
```
2. 在做乘10运算的时候先判断一下
public class Solution {
public int reverse(int x) {
if (x == 0) return 0;
int res = 0;
int sign = 1;
if (x < 0) {
sign = -1;
x = -1 * x;
}
while (x != 0) {
if (res > (Integer.MAX_VALUE - x % 10) / 10) {
return 0;
}
res = res * 10 + x % 10;
x = x / 10;
}
return res * sign;
}
}




