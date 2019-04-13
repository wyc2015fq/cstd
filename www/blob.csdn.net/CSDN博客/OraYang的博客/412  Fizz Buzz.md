
# 412. Fizz Buzz - OraYang的博客 - CSDN博客

2017年08月11日 09:34:26[OraYang](https://me.csdn.net/u010665216)阅读数：179所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Write a program that outputs the string representation of numbers from 1 ton.
But for multiples of three it should output “Fizz” instead of the number and for the multiples of five output “Buzz”. For numbers which are multiples of both three and five output “FizzBuzz”.
Example:
n = 15,
Return:
[
    "1",
    "2",
    "Fizz",
    "4",
    "Buzz",
    "Fizz",
    "7",
    "8",
    "Fizz",
    "Buzz",
    "11",
    "Fizz",
    "13",
    "14",
    "FizzBuzz"
]思路：本题题目说得很清楚，3的倍数为"Fizz",5的倍数"Buzz",15的倍数"FizzBuzz"，但是要注意数组下表从0计数
代码：

```python
class Solution {
public:
    vector<string> fizzBuzz(int n) {
        vector<string> s;
        for(int i =0;i<n;i++)
        {
            if(!((i+1)%3)&&!((i+1)%5))
            {
                s.push_back("FizzBuzz");
            }
            else if(!((i+1)%3))
                s.push_back("Fizz");
            else if(!((i+1)%5))
                s.push_back("Buzz");
            else
                s.push_back(to_string(i+1));
        }
        return s;
        
    }
};
```


