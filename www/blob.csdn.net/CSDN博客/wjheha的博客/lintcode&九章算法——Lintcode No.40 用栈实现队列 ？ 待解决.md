# lintcode&九章算法——Lintcode No.40 用栈实现队列 ？ 待解决 - wjheha的博客 - CSDN博客
2017年12月28日 10:43:54[wjheha](https://me.csdn.net/wjheha)阅读数：71
原文：fourierhai 
链接：[http://mp.weixin.qq.com/s/ssAc1AodazaVtjRnFsTTgQ](http://mp.weixin.qq.com/s/ssAc1AodazaVtjRnFsTTgQ)
Lintcode No.40 用栈实现队列 
![这里写图片描述](https://img-blog.csdn.net/20171228104244925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
代码实现：
```
public class MyQueue{
     private Stack<Integer> stack1;
     private Stack<Integer> stack2;
     public MyQueue() {
           // do intialization if necessary
           stack1 = new Stack<Integer>();
           stack2 = new Stack<Integer>();
     }
     /*
      * @param element: An integer
      *
      * @return: nothing
      */
     public void push(int element) {
           // write your code here
           stack1.push(element);
     }
     /*
      * @return: An integer
      */
     public int pop() {
           // write your code here
           if (stack2.empty()) {
                while (!stack1.empty()) {
                     int s1 = stack1.pop();
                     stack2.push(s1);
                }
           }
           return stack2.pop();
     }
     /*
      * @return: An integer
      */
     public int top() {
           // write your code here
           if (stack2.empty()) {
                while (!stack1.empty()) {
                     int s1 = stack1.pop();
                     stack2.push(s1);
                }
           }
           return stack2.peek();
     }
}
```
AC时间：在2500ms左右。
