# 栈的使用_leetcode刷题总结 - zhuzhuxia183的博客 - CSDN博客





2019年01月16日 19:35:33[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：60
个人分类：[数据结构和算法																[栈](https://blog.csdn.net/zhuzhuxia183/article/category/8617505)](https://blog.csdn.net/zhuzhuxia183/article/category/8608092)

所属专栏：[数据结构与算法(leetcode)](https://blog.csdn.net/column/details/33915.html)








## 那些可以用栈可以轻松AC的题目：
- 
1、有效的括号  [https://leetcode-cn.com/problems/valid-parentheses/](https://leetcode-cn.com/problems/valid-parentheses/)

给定一个只包括 ‘(’，’)’，’{’，’}’，’[’，’]’ 的字符串，判断字符串是否有效。

有效字符串需满足：

左括号必须用相同类型的右括号闭合。

左括号必须以正确的顺序闭合。

注意空字符串可被认为是有效字符串。
示例 1:

输入: “()”

输出: true

示例 2:
输入: “()[]{}”

输出: true

示例 3:
输入: “(]”

输出: false

示例 4:
输入: “([)]”

输出: false

示例 5:
输入: “{[]}”

输出: true

这道题直接用Stack来做，

1、如果碰到 }、[、(中的一个时，判断栈是不是空，或者不为空pop出来的字符不为对应的 }、]、)时，return false;否则继续

2、如果碰到 }、]、)，直接入栈

3、最后判断栈是不是空的，如果是则全部匹配完全
这里通过使用HashMap的映射关系来记录这些对应的符号

```
public boolean isValid(String s) {   
        HashMap<Character,Character> map = new HashMap<>();
        map.put(')','(');
        map.put(']','[');
        map.put('}','{');
        Stack<Character> stack = new Stack<>();
        for (int i = 0; i < s.length(); i++) {
            if (map.containsKey(s.charAt(i))){
                if(stack.isEmpty()||stack.pop()!=map.get(s.charAt(i))) return false;
            }else {
                stack.push(s.charAt(i));
            }
        }
        return stack.isEmpty();
    }
```
- 
2、最长有效括号  [https://leetcode-cn.com/problems/longest-valid-parentheses/](https://leetcode-cn.com/problems/longest-valid-parentheses/)

给定一个只包含 ‘(’ 和 ‘)’ 的字符串，找出最长的包含有效括号的子串的长度。

示例 1:

输入: “(()”

输出: 2

解释: 最长有效括号子串为 “()”

示例 2:
输入: “)()())”

输出: 4

解释: 最长有效括号子串为 “()()”
这道题，是针对’(’ 和 ‘)’ 的一个计算最长有效长度，这里我们还是通过使用栈来进行记录：

```
public int longestValidParentheses(String s) {
    
        Stack<Integer> stack = new Stack<>();
        HashMap<Character,Integer> map = new HashMap<>();
        int res = 0;
        for (int i = 0; i < s.length(); i++) {
            if (s.charAt(i)==')'&& !stack.isEmpty() && s.charAt(stack.peek())=='('){
                stack.pop();
                if (stack.isEmpty()){
                    res = Math.max(res,i+1);
                }else {
                    res = Math.max(res,i-stack.peek());
                }
            }else {
                stack.push(i);
            }
        }
        return res;
    }
```

这里的计算的话，总有一种滑动窗口的感觉，就是通过Stack保存的是下标。如果找到了可以对应的，就不断的进行更新max_length
- 
3、接雨水 [https://leetcode-cn.com/problems/trapping-rain-water/](https://leetcode-cn.com/problems/trapping-rain-water/)

给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。![在这里插入图片描述](https://img-blog.csdnimg.cn/20190116183914366.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXpodXhpYTE4Mw==,size_16,color_FFFFFF,t_70)

上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的高度图，在这种情况下，可以接 6 个单位的雨水（蓝色部分表示雨水）。 感谢 Marcos 贡献此图。
示例:

输入: [0,1,0,2,1,0,1,3,2,1,2,1]

输出: 6

我们思考的时候，可以想像如果要装雨水，那么应该是先下降再上升，所以我们首先用一个单调递减的栈来记录，然后遇到非递减的数值，就开始判断是不是可以有装雨水。代码：

```
public int trap(int[] height) {
        int area = 0;
        Stack<int[]> stack = new Stack<>();
        int lenth = height.length;
        for (int i = 0; i < lenth; i++) {
            if (stack.isEmpty()||stack.peek()[0]>height[i]){
                int[] item = {height[i],i};
                stack.push(item);
            }else{
                while (!stack.isEmpty()&&stack.peek()[0]<=height[i]){
                    int[] temp = stack.pop();
                    if (!stack.isEmpty()){
                        area+= (i-stack.peek()[1]-1) * (Math.min(stack.peek()[0],height[i])-temp[0]);
                    }
                }
                int[] item = {height[i],i};
                stack.push(item);
            }
        }
        return area;
    }
```
- 
4、柱状图中最大的矩形  [https://leetcode-cn.com/problems/largest-rectangle-in-histogram/submissions/](https://leetcode-cn.com/problems/largest-rectangle-in-histogram/submissions/)

给定 n 个非负整数，用来表示柱状图中各个柱子的高度。每个柱子彼此相邻，且宽度为 1

求在该柱状图中，能够勾勒出来的矩形的最大面积
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190116192522977.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXpodXhpYTE4Mw==,size_16,color_FFFFFF,t_70)

以上是柱状图的示例，其中每个柱子的宽度为 1，给定的高度为 [2,1,5,6,2,3]。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190116192751363.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXpodXhpYTE4Mw==,size_16,color_FFFFFF,t_70)

图中阴影部分为所能勾勒出的最大矩形面积，其面积为 10 个单位。

解答这种题目的时候，要得到答案其实并不是没有办法，但是写程序是一个分析逻辑的过程，对于这道题，如果使用递增栈的话，如果递增的话，说明最大矩形面积肯定还在不断的增加过程中，如果遇到递减的栈，我们就开始计算到递减之前的peek高度对应最大矩形面试，就不断的pop()，然后不断的判断是不是，最后我们需要在遍历最后一个即n-1之后，然后剩下栈中的内容也能够全部遍历一遍，所以我们是需要多便利一个n，但是制定第n为0，所以通过这样的方式可以把所有的可能性都求一遍。

```
public int largestRectangleArea(int[] heights) {
        int res = 0;
        Stack<Integer> stack = new Stack<>();
        int n = heights.length;
        int i=0;
        while (i<=n){
            int height = (i==n? 0:heights[i]);
            if (stack.isEmpty()||heights[stack.peek()]<height){
                stack.push(i);
                i++;
            }else {
                int cur = stack.pop();
                int cur_height = heights[cur];
                res = Math.max(res,cur_height * (stack.isEmpty()?i:(i-stack.peek()-1)));
            }
        }
        return res;
    }
```


















