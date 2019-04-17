# Lintcode——766. 闰年 - westbrook1998的博客 - CSDN博客





2018年04月04日 22:32:25[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：76标签：[Lintcode																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=Lintcode&t=blog)
个人分类：[Lintcode](https://blog.csdn.net/westbrook1998/article/category/7459920)








> 
闰年是包含额外一天的日历年. 如果年份可以被 4 整除且不能被 100 整除 或者 可以被 400 整除, 那么这一年为闰年. –wikipedia

代码：


```java
public class Solution {
    /**
     * @param n: a number represent year
     * @return: whether year n is a leap year.
     */
    public boolean isLeapYear(int n) {
        // write your code here
        if(n%4==0 && n%100!=0 || n%400==0){
            return true;
        }
        return false;
    }
}
```




