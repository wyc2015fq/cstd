# Lintcode——765. Valid Triangle - westbrook1998的博客 - CSDN博客





2018年04月04日 22:13:37[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：36标签：[Lintcode																[算法																[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[Lintcode](https://blog.csdn.net/westbrook1998/article/category/7459920)





> 
判断三角形


三角形两边和大于第三边，两边差小于第三边

代码：

```java
public class Solution {
    /**
     * @param a: a integer represent the length of one edge
     * @param b: a integer represent the length of one edge
     * @param c: a integer represent the length of one edge
     * @return: whether three edges can form a triangle
     */
    public boolean isValidTriangle(int a, int b, int c) {
        // write your code here
        if(a+b>c && a+c>b && b+c>a){
            return true;
        }
        return false;
    }
}
```](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=Lintcode&t=blog)




