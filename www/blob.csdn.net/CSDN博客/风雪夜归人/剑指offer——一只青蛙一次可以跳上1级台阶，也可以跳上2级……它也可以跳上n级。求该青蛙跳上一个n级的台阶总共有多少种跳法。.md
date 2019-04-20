# 剑指offer——一只青蛙一次可以跳上1级台阶，也可以跳上2级……它也可以跳上n级。求该青蛙跳上一个n级的台阶总共有多少种跳法。 - 风雪夜归人 - CSDN博客
2018年04月26日 20:56:29[cdjccio](https://me.csdn.net/qq_34624951)阅读数：1489
Q：一只青蛙一次可以跳上1级台阶，也可以跳上2级。求该青蛙跳上一个n级的台阶总共有多少种跳法。
A：该问题实质是斐波那契数列求和，递推公式为 f(n)=f(n-1)+f(n-2);
```cpp
class Solution {
public:
    int jumpFloor(int number) {
        if(number <= 0)
        {
            return 0;
        }
        else if(number == 1 || number == 2)
        {
            return number;
        }
        else
        {
          //int re = 0;                                           
            return jumpFloor(number - 1) + jumpFloor(number - 2); 
        }
       
    }
};
```
Q：一只青蛙一次可以跳上1级台阶，也可以跳上2级……它也可以跳上n级。求该青蛙跳上一个n级的台阶总共有多少种跳法。
A：f(n) = f(n-1)+f(n-2)+...+f(1)
      f(n-1) = f(n-2)+ f(n-3)...+f(1)
两式相减，得到f(n) = 2*f(n-1)
```cpp
class Solution {
public:
    int jumpFloorII(int number) {
        if(number <= 0)
        {
            return 0;
        }
        else if(number == 1 || number == 2)
        {
            return number;
        }
        else
        {
            return 2 * jumpFloorII(number - 1);
        }
    }
};
```
