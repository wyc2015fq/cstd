# Leetcode刷题修炼手册 - Machine Learning with Peppa - CSDN博客





2018年01月25日 13:53:13[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：8872
所属专栏：[C语言入门](https://blog.csdn.net/column/details/18792.html)











#### “跟着我左手右手一个二叉树，堆栈链表反转换不同风格”

对于各位读研或者找工作的同学来说，Leetcode可能是无法绕过去的一个坎，很多公司的技术类岗位会从题库中抽题。此外，刷leetcode对于提高我们对数据结构与算法的应用和理解能力也有很大帮助，相信不少题友也是痛并快乐着。

同样将开始leetcode征程的我，特意整理了一套Leetcode刷题修炼手册，对于还没有开始或者正处于迷茫的同学可以起到一些帮助。里面有四种不同的刷题策略，欢迎自行对号入座。




### 一、刷题选择

盲目刷题不可取，因此，刷题要一定要搞清楚刷题的目的和原因。其实无外乎4种：

- 如果想提升自己的思维能力，可以按照AC率由低到高二分查找匹配自己当前水平难度的题目，然后适当挑战高难度题（二分时间复杂度是O(logn)，至少比从易到难的O(n)节省时间）
- 如果想巩固某一专题，那自然应该按照tag来刷题，但是因为所用的方法在求解前已知，不太利于思维能力的提升
- 如果什么都不懂，那么建议随机刷题，一来可以涨见识，二来进步空间比较大
- 如果想提高AC率或者增加自信，那么建议刷水题


人与人之间还是有天赋差别的，但区别在于经验可以慢慢积累、再有个建议，题目如果太难超过当前自己能力的话，尝试一定时间后还是老老实实看题解吧






### 二、刷题方法





##### 方法一：顺序法

建议未刷过题的新人按着顺序（AC）来。前 150 题覆盖了很多经典题目和知识点，指针法类如『3 sum』系列，动规类如『regex matching』，搜索类题目如『Sodoku Solver』。

基本熟悉知识点（图、树、堆、栈、链表、哈希表、记忆搜索、动态规划、指针法、并查集等）后，可以一类类标签强攻。Leetcode 右侧的标签系统虽然未必 100% 完整，但是大致分类做得还不错。

面试前的一个月可以只做『Hard』标签的题目，因为一般两遍之后对于大部分『Medium』难度以下的题目都是肌肉记忆了。多练习『Hard』类题目可以让自己的思路更开阔，因为很多题目使用的奇淫巧技让人惊讶，比如 Leetcode 精心设计连续题号的『84. Largest Rectangle in Histogram』、『85. Maximal Rectangle』。








##### 方法二：标签法




按照网站给大家排列的不同tags，起到模块化的复习和学习作用。举个例子：比如复习链表的内容，就选Linked List这部分的23个题目。刷完之后可以再总结一下常用的方法和数据结构与构造方式。请不要为了刷题而刷题，一定是为了弥补一部分的知识去做。
![](https://img-blog.csdn.net/20180125132449973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



##### 方法三：随机法

随心所欲的选择难度与刷题顺序，哪个顺眼做哪个。本方法只适合业余编程，不从事本行业的同学以及大神级人物








##### 方法四：必杀法


leetcode是有公司题库的，一句话：面哪家，刷哪家
![](https://img-blog.csdn.net/20180125134906755?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





### 三、刷题攻略



TIP 1：


众所周知，上古时期号称刷完leetcode所有题的大神，只不过是因为当时只有150道。而截至2018年1月，leetcode已经有700道题，且部分难度不小。要全部刷完，除了浪费你自己的时间，似乎找不到别的目的。

因此，对于大多数人来说，没有时间也没有必要把所有题目都做一遍（时间充裕可以随意）。可以考虑序号为前250位的题目，因为那些全是经典与必考题。

TIP 2：

善用收藏夹，要养成『一道题第二次练习尚不能解就加入收藏夹』的习惯，且需要定期清空收藏夹：每道题不需提示下通过两次后才能移出收藏夹。

TIP 3：


可以按照下文的面试出题频率顺序来做，从频率最高的一批开始。 而且请尽量不使用IDE，直接在平台上写代码。 


面试前可以购买会员，按照公司的标签来练习，也可以结合白板练习。面试前如果时间紧迫，那么练习的优先级分别是：即将面试公司的题目、收藏夹里的旧题目、剩余的新题。

冲刺阶段的练习请尽量不要打开题型标签，给自己思考的空间。如果真的刷了三遍以上还没法达到理想目标，那么一定是学习方法出了问题，请多总结。

TIP 4：

写好代码先不要提交，人工检查一下代码，比如分号是否都有写，return有没少等。 人工检查完后使用“Custom Testcase”功能自定义测试用例，注意检查边界，然后“Run
 Code”，这步可以发现蛮多问题的。  等RunCode通过后，再去提交。






————


最后附上[Lnho](http://blog.csdn.net/lnho2015/article/details/50962989)大神曾经整理过的刷题顺序，虽然部分已经调整，但总体来说题目还是绝对经典的： 


出现频度为5： 

1. [Leet Code OJ 1. Two Sum [Difficulty: Easy]](http://blog.csdn.net/lnho2015/article/details/50889558)

2. [Leet Code OJ 8. String to Integer (atoi) [Difficulty: Easy]](http://blog.csdn.net/lnho2015/article/details/50953068)

3. [Leet Code OJ 15. 3Sum [Difficulty: Medium]](http://blog.csdn.net/lnho2015/article/details/51314133)

4. [Leet Code OJ 20. Valid Parentheses [Difficulty: Easy]](http://blog.csdn.net/Lnho2015/article/details/50970920)

5. [Leet Code OJ 21. Merge Two Sorted Lists [Difficulty: Easy]](http://blog.csdn.net/lnho2015/article/details/50801842)

6. [Leet Code OJ 28. Implement strStr() [Difficulty: Easy]](http://blog.csdn.net/lnho2015/article/details/51275683)

7. Leet Code OJ 56. Merge Intervals [Difficulty: Hard] 

8. Leet Code OJ 57. Insert Interval [Difficulty: Hard] 

9. Leet Code OJ 65. Valid Number [Difficulty: Hard] 

10. [Leet Code OJ 70. Climbing Stairs [Difficulty: Easy]](http://blog.csdn.net/lnho2015/article/details/50756205)

11. Leet Code OJ 73. Set Matrix Zeroes [Difficulty: Medium] 

12. [Leet Code OJ 88. Merge Sorted Array [Difficulty: Easy]](http://blog.csdn.net/lnho2015/article/details/50912902)

13. Leet Code OJ 98. Validate Binary Search Tree [Difficulty: Medium] 

14. [Leet Code OJ 125. Valid Palindrome [Difficulty: Easy]](http://blog.csdn.net/Lnho2015/article/details/50978799)

15. Leet Code OJ 127. Word Ladder [Difficulty: Medium]


出现频度为4： 

1. [Leet Code OJ 2. Add Two Numbers [Difficulty: Medium]](http://blog.csdn.net/lnho2015/article/details/50962615)

2. Leet Code OJ 12. Integer to Roman 

3. Leet Code OJ 13. Roman to Integer 

4. Leet Code OJ 22. Generate Parentheses 

5. Leet Code OJ 23. Merge k Sorted Lists 

6. Leet Code OJ 24. Swap Nodes in Pairs 

7. [Leet Code OJ 27. Remove Element [Difficulty: Easy]](http://blog.csdn.net/lnho2015/article/details/50802024)

8. Leet Code OJ 46. Permutations 

9. Leet Code OJ 49. Anagrams 

10. Leet Code OJ 67. Add Binary 

11. Leet Code OJ 69. Sqrt(x) 

12. Leet Code OJ 77. Combinations 

13. Leet Code OJ 78. Subsets 

14. Leet Code OJ 79. Word Search 

15. [Leet Code OJ 91. Decode Ways [Difficulty: Medium]](http://blog.csdn.net/Lnho2015/article/details/51752243)

16. [Leet Code OJ 102. Binary Tree Level Order Traversal [Difficulty: Easy]](http://blog.csdn.net/lnho2015/article/details/50826565)

17. Leet Code OJ 129. Sum Root to Leaf Numbers 

18. Leet Code OJ 131. Palindrome Partitioning



