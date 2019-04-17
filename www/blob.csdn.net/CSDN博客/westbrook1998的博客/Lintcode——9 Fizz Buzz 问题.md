# Lintcode——9. Fizz Buzz 问题 - westbrook1998的博客 - CSDN博客





2018年04月04日 22:29:57[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：46标签：[Lintcode																[算法																[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[Lintcode](https://blog.csdn.net/westbrook1998/article/category/7459920)





> 
给你一个整数n. 从 1 到 n 按照下面的规则打印每个数： 

  如果这个数被3整除，打印fizz. 

  如果这个数被5整除，打印buzz. 

  如果这个数能同时被3和5整除，打印fizz buzz.
代码：

```php
public class Solution {
    /*
     * @param n: An integer
     * @return: A list of strings.
     */
    public List<String> fizzBuzz(int n) {
        // write your code here
        List<String> list=new ArrayList<>(n);
        for(int i=1;i<=n;i++){
            if(i%3==0 && i%5==0){
                list.add("fizz buzz");
            }
            else if(i%3==0){
                list.add("fizz");
            }
            else if(i%5==0){
                list.add("buzz");
            }
            else{
                list.add(String.valueOf(i));
            }
        }
        return list;
    }
}
```](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=Lintcode&t=blog)




