
# 401. Binary Watch - OraYang的博客 - CSDN博客

2017年08月10日 09:23:30[OraYang](https://me.csdn.net/u010665216)阅读数：101标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
A binary watch has 4 LEDs on the top which represent thehours(0-11), and the 6 LEDs on the bottom represent theminutes(0-59).
Each LED represents a zero or one, with the least significant bit on the right.
![](https://img-blog.csdn.net/20170810092429566?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







For example, the above binary watch reads "3:25".
Given a non-negative integernwhich represents the number of LEDs that are currently on, return all possible times the watch could represent.
Example:
Input: n = 1
Return: ["1:00", "2:00", "4:00", "8:00", "0:01", "0:02", "0:04", "0:08", "0:16", "0:32"]
Note:
The order of output does not matter.
The hour must not contain a leading zero, for example "01:00" is not valid, it should be "1:00".
The minute must be consist of two digits and may contain a leading zero, for example "10:2" is not valid, it should be "10:02".
思路：本题乍一看，感觉需要用回溯法，感觉代码量有点多，思考了一会没有思路，然后看官方讨论，发现一个java解决方案，代码如下：

```python
public List<String> readBinaryWatch(int num) {
    List<String> times = new ArrayList<>();
    for (int h=0; h<12; h++)
        for (int m=0; m<60; m++)
            if (Integer.bitCount(h * 64 + m) == num)
                times.add(String.format("%d:%02d", h, m));
    return times;        
}
```
代码解释：该方法使用了Integer的bitCount方法，感觉思路真的神奇，遇到问题我惯性思维是将num拆分组合成hour:min形成，来判断是否满足要求，但是人家这个方法是倒过来的，遍历一切组合的可能，来判断位数是否满足num，真的佩服，于是用C++来改写，自己实现了bitCount函数：
```python
class Solution {
public:
    vector<string> readBinaryWatch(int num) {
        vector<string> res;
        if(num<0)
            return res;
        for(int h =0;h<12;h++)
            for(int min = 0;min<60;min++)
            {
                if(bitcount(h)+bitcount(min)==num)
                    if(min>9)
                       res.push_back(to_string(h) + ":" +to_string(min));
                     else 
                        res.push_back(to_string(h) + ":0" + to_string(min));
            }
        return res;
    }
private:
    int bitcount(int x)  
    {  
    int b;  
   
    for (b = 0; x != 0; x >>= 1)  
        if (x & 01)  
            b++;  
    return b;  
    }  
};
```


