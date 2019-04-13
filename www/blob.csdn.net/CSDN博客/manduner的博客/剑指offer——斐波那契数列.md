
# 剑指offer——斐波那契数列 - manduner的博客 - CSDN博客


2019年02月25日 22:13:49[Manduner_TJU](https://me.csdn.net/manduner)阅读数：21标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[剑指offer																](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)



# 1，问题描述
大家都知道斐波那契数列，现在要求输入一个整数n，请你输出斐波那契数列的第n项（从0开始，第0项为0）。n<=39
# 2，解题思路
（1）首先得知道什么是[斐波那契数列](https://baike.baidu.com/item/%E6%96%90%E6%B3%A2%E9%82%A3%E5%A5%91%E6%95%B0)，通常情况下斐波那契数列的第一项和第二项是1，从第三项开始，每一项的值等于前两项之和，即：1,1,2,3,5......
（2）需要注意的是，该题目限定从第0项开始，第0项的值为0，那么数列应该为：0,1,1,2,3,5......
（3）解决方法很简单，就是利用递归的思想
# 3，源码
```python
public class Solution {
    public int Fibonacci(int n) {
        if(n==0){
            return 0;
        }else if(n==1){
            return 1;
        }else{
            return Fibonacci(n-1) + Fibonacci(n-2);
        }
    }
}
```
**运行结果：**
![](https://img-blog.csdnimg.cn/20190225221311481.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy,size_16,color_FFFFFF,t_70)


