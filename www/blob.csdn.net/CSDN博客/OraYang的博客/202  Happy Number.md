
# 202. Happy Number - OraYang的博客 - CSDN博客

2017年07月30日 14:52:05[OraYang](https://me.csdn.net/u010665216)阅读数：246标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Write an algorithm to determine if a number is "happy".
A happy number is a number defined by the following process: Starting with any positive integer, replace the number by the sum of the squares of its digits, and repeat the process until the number equals 1 (where it will stay), or it loops endlessly in a cycle
 which does not include 1. Those numbers for which this process ends in 1 are happy numbers.
Example:19 is a happy number
12+ 92= 82
82+ 22= 68
62+ 82= 100
12+ 02+ 02= 1
思路：本题有两种思路：第一种寻找特殊值，发现无论怎样的数字，要想最后结果为1，除了10的倍数外，其余数字必须求和为70才能保证为欢乐树，否则false;第二种思路就是找个数字计算，找规律，发现经历一定不走计算后，和要么为1要么就经历死循环。
代码（特殊值）：

```python
class Solution {
public:
    bool isHappy(int n) {
        vector<int> a = ret(n);
        int sum = res(a);
        cout<<sum<<endl;
        if(sum==1)
            return true;
        else if(sum%10==0&&ret(sum).size()==2)
        {
            if(sum==10||sum==70)
                return true;
            else
                return false;
        }
        else
            return isHappy(sum);      
    }
    vector<int> ret(int n){
        vector<int> num;
        while(n)
        {
            num.push_back(n%10);
            n = n/10;
        }
        return num;
    }
    int res(vector<int> num){
        int sum = 0;
        for(int i = 0;i<num.size();i++)
            sum+=num[i]*num[i];       
        return sum;
    }
    
};
```
代码（找规律）：
```python
class Solution {
public:
int digitSquareSum(int n) {
    int sum = 0, tmp;
    while (n) {
        tmp = n % 10;
        sum += tmp * tmp;
        n /= 10;
    }
    return sum;
}
bool isHappy(int n) {
    int slow, fast;
    slow = fast = n;
    do {
        slow = digitSquareSum(slow);
        fast = digitSquareSum(fast);
        fast = digitSquareSum(fast);
    } while(slow != fast);
    if (slow == 1) return 1;
    else return 0;
}
};
```



