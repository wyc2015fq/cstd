# leetcode_457. Circular Array Loop - wjheha的博客 - CSDN博客
2018年01月10日 15:58:50[wjheha](https://me.csdn.net/wjheha)阅读数：358
You are given an array of positive and negative integers. If a number n at an index is positive, then move forward n steps. Conversely, if it’s negative (-n), move backward n steps. Assume the first element of the array is forward next to the last element, and the last element is backward next to the first element. Determine if there is a loop in this array. A loop starts and ends at a particular index with more than 1 element along the loop. The loop must be “forward” or “backward’.
Example 1: Given the array [2, -1, 1, 2, 2], there is a loop, from index 0 -> 2 -> 3 -> 0.
Example 2: Given the array [-1, 2], there is no loop.
Note: The given array is guaranteed to contain no element “0”.
Can you do it in O(n) time complexity and O(1) space complexity?
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/circular-array-loop/description/](https://leetcode.com/problems/circular-array-loop/description/)
***思路***： 
这道题，题意读不懂，不知道在说啥，后来仔细看看例题，才反应过来。题目意思：给你一个数组，比如·[2, -1, 1, 2, 2]，你先站在索引0处，因为索引0处的数字为2，所以你往前走两步，到达索引2处（注意：数组元素并不移动，只是你所站的位置在移动），该处数字为1，所以继续往前走一步，到达索引3处，此处数字为2，继续前进两步，超出数组范围，但是这里我们把数组看做是首位相连的，所以，超出了一步，到达索引0处，回到了起始的位置。环必须是单向的，这个意思是说：你在第一步是往前走的话，那么，你第一次回来的时候必然是从另外一个方向回来的。如下图：第二个才是满足题目要求的循环数组环。 
![这里写图片描述](https://img-blog.csdn.net/20180111164855242?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**代码思路**：
