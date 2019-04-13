
# [LeetCode] Add Digits - 数字各个位数求和 - 杨秀璋的专栏 - CSDN博客

2015年09月08日 18:06:18[Eastmount](https://me.csdn.net/Eastmount)阅读数：2318


**题目概述：**
Given a non-negative integer num, repeatedly add all its digits until the result has only one digit.
For example:
Given num = 38, the process is like: 3 + 8 = 11, 1 + 1 = 2. Since 2 has only one digit, return it.
Follow up:
Could you do it without any loop/recursion in O(1) runtime?
**题目解析：**
主要考察整数各个位数求和，当且仅当和小于10时输出。可能会走入整数转换为字符串的误区，直接使用取余(%)个位数相加即可，需要注意：
1.非负整数判断
2.输入0时输出0，输入10时输出1
3.至于O(1)时间复杂度考察数学公式：1 + (num-1) % 9
4.整数转换字符串可以类似思想：循环个位数转换reslut=num%10，num=num/10
**我的代码：**

```python
int addDigits(int num) {
    int result;
    //负数跳出
    if(num<=0) return 0;
    if(num<10) return num;
    result=num;
    while(result>=10)
    {
        num=result;
        result=0;
        while(num>=10) {       //计算数字和
            result+=num%10;    //个位
            num=num/10;
        }
        result+=num;
    }
    return result;
}
```
**推荐代码：**
```python
public class Solution {  
    public int addDigits(int num) {  
        return 1 + (num-1) % 9;  
    }  
}
```
**题目笔记：**
Roman to Integer 罗马数字转换考察switch和数字各个位数转换
```python
int getNumToInt(char ch) {
    //此题需要注意switch写法和罗马数字计算
    switch(ch) {
        case 'I': return 1;   
        case 'V': return 5;  
        case 'X': return 10;  
        case 'L': return 50;  
        case 'C': return 100;  
        case 'D': return 500;  
        case 'M': return 1000;  
        default: return 0; 
    }
    return 0;
}
int romanToInt(char* s) {
    /*
     1~9: {"I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"}
     10~90: {"X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"}
     100~900: {"C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"}
     1000~3000: {"M", "MM", "MMM"}
     如果当前处理的字符对应的值和上一个字符一样，那么临时变量加上这个字符。比如III = 3
     如果当前比前一个大，说明这一段的值应该是当前这个值减去前面记录下的临时变量中的值。比如IIV = 5 – 2
     如果当前比前一个小，那么就可以先将临时变量的值加到结果中，然后开始下一段记录。比如VI = 5 + 1
     DCXXI=500+100+10+10+1=621
    */
    int length;
    int i,j;
    int result;          //结果
    int last;            //前一个值
    int curr;            //当前值
    int num;             //临时变量
    length  = strlen(s);
    last = getNumToInt(s[0]);
    num = last;
    for(i=1; i<length; i++) {
        curr = getNumToInt(s[i]);
        if(curr == last) { 
            num += curr;
        }
        else if(curr < last) {
            result += num;
            num = curr;
        }
        else if(curr > last) {
            num = curr - num;
        }
        last = curr;        
    }
    result += num;
    return result;
}
```
PS:我代码的缺点包括：我喜欢过于复杂的注释，而直观的代码好于注释；代码中的临时变量转换过多，需要更精简；字符串数组操作习惯计算长度后s[i]，而不是使用*s。
(By:Eastmount 2015-9-8 下午6点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))

