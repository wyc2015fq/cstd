
# 剑指offer——栈的压入、弹出序列（判断栈的出入合法性） - manduner的博客 - CSDN博客


2019年03月28日 15:30:29[Manduner_TJU](https://me.csdn.net/manduner)阅读数：11标签：[剑指offer																](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)



# 1，问题描述
输入两个整数序列，第一个序列表示栈的压入顺序，请判断第二个序列是否可能为该栈的弹出顺序。假设压入栈的所有数字均不相等。例如序列1,2,3,4,5是某栈的压入顺序，序列4,5,3,2,1是该压栈序列对应的一个弹出序列，但4,3,5,1,2就不可能是该压栈序列的弹出序列。（注意：这两个序列的长度是相等的）
# 2，解题思路
首先构建一个辅助栈，然后遍历入栈序列，遍历操作包含：（1）先将元素压入栈中，（2）紧接着做循环判断该辅助栈是否为空，若不为空且栈顶元素等于出栈序列的第j（j从0开始）个元素，则辅助栈执行弹出操作，同时出栈序列下标j向后移一位（j++），循环直至辅助栈为空或者栈顶元素不等于出栈序列的第j个元素。遍历完入栈序列再判断辅助栈是否为空，若为空说明出栈序列合法，若不为空说明出栈序列不合法。
# 3，源码
```python
import java.util.Stack;
public class Solution {
    public boolean IsPopOrder(int [] pushA,int [] popA) {
        //程序的严谨性：先做临界值检查
        if(pushA.length==0||popA.length==0) return false;
        Stack<Integer> st = new Stack<Integer>();
        int j=0;
        for(int i=0;i<pushA.length;i++) {
            st.push(pushA[i]);
            while(!st.empty()&&st.peek()==popA[j]) {
                st.pop();
                j++;
            }
        }
        return st.empty();
    }
}
```


