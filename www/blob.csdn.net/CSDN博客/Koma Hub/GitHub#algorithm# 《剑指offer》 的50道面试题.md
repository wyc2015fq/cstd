# GitHub#algorithm#:《剑指offer》 的50道面试题 - Koma Hub - CSDN博客
2018年06月14日 14:28:33[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：364
- Copyright(C) nonstriater:[https://github.com/nonstriater/Learn-Algorithms](https://github.com/nonstriater/Learn-Algorithms)
《剑指offer》 这本书给出了50到面试题，涉及到字符串处理，堆栈，链表，二叉树等问题的处理。
- 代码鲁榜性：边界条件，特殊输入，异常处理：空指针
- 分析方法：画图，举例，分解
- 查找和排序是常考：重点掌握二分查找，快速排序，归并排序
- 本书完整源代码在：
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E8%B5%8B%E5%80%BC%E8%BF%90%E7%AE%97%E7%AC%A6%E5%87%BD%E6%95%B0)赋值运算符函数
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E5%AE%9E%E7%8E%B0singleton%E6%A8%A1%E5%BC%8F)实现Singleton模式
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%BA%8C%E7%BB%B4%E6%95%B0%E7%BB%84%E4%B8%AD%E7%9A%84%E6%9F%A5%E6%89%BE)二维数组中的查找
二维数组中每一行从左到右递增，每一列从上到下递增，判断数组中是否包含该整数。
boolfind(int *matrix,int rows,int columns,int numbers)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%9B%BF%E6%8D%A2%E7%A9%BA%E6%A0%BC)替换空格
如把字符串中的每个空格替换成`%20`
`二遍扫描`
voidreplace_blank(char *str);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%BB%8E%E5%B0%BE%E5%88%B0%E5%A4%B4%E6%89%93%E5%8D%B0%E9%93%BE%E8%A1%A8)从尾到头打印链表
`栈`
voidprint_reversing(LinkList *head)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E9%87%8D%E5%BB%BA%E4%BA%8C%E5%8F%89%E6%A0%91)重建二叉树
输入某二叉树的前序遍历和中序遍历的结果，重建该二叉树
BinaryTree *construct(int *preorder,int inroder,int length);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E7%94%A8%E4%B8%A4%E4%B8%AA%E6%A0%88%E5%AE%9E%E7%8E%B0%E9%98%9F%E5%88%97)用两个栈实现队列
队列就是在尾部插入节点，头部删除节点。
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%97%8B%E8%BD%AC%E6%95%B0%E7%BB%84%E7%9A%84%E6%9C%80%E5%B0%8F%E6%95%B0%E5%AD%97)旋转数组的最小数字
旋转数组是指把一个数组最开始的若干个元素搬到数组的末尾。输入一个递增排序的数组的旋转，比如{3，4，5，1，2}是{1，2，3，4，5}的一个旋转。求该数组的最小值。
intmin(int *num, int length)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E8%8F%B2%E6%B3%A2%E9%82%A3%E5%88%87%E6%95%B0%E5%88%97)菲波那切数列
longlongfabonacci(unsigned n)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%BA%8C%E8%BF%9B%E5%88%B6%E4%B8%AD1%E7%9A%84%E4%B8%AA%E6%95%B0)二进制中1的个数
输入一个整数，输出该数二进制中1出现的次数。比如9的二进制 10001，输出是2
`n=n&n-1`
intone_appear_count(int n)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%95%B0%E5%80%BC%E7%9A%84%E6%95%B4%E6%95%B0%E6%AC%A1%E6%96%B9)数值的整数次方
要求不得使用库函数。这里注意考虑指数是0和负数的情况
doublepower(double base,int exponent)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%89%93%E5%8D%B01%E5%88%B0%E6%9C%80%E5%A4%A7%E7%9A%84n%E4%BD%8D%E6%95%B0)打印1到最大的n位数
比如n=3，就打印1到999
voidprint_to_max_with_length(int n)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E5%9C%A8o1%E6%97%B6%E9%97%B4%E5%88%A0%E9%99%A4%E9%93%BE%E8%A1%A8%E8%8A%82%E7%82%B9)在O(1)时间删除链表节点
已经有一个头节点指针，还有一个指向改删除节点的指针
`用下一个节点的内容覆盖当前删除节点的内容，删除下一个节点`
voiddeleteNode(LinkList *head,LinkList *targetToDelete);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E8%B0%83%E6%95%B4%E6%95%B0%E7%BB%84%E9%A1%BA%E5%BA%8F%E4%BD%BF%E5%A5%87%E6%95%B0%E4%BD%8D%E4%BA%8E%E5%81%B6%E6%95%B0%E5%89%8D%E9%9D%A2)调整数组顺序使奇数位于偶数前面
调整后，所有奇数在前半部分，偶数在后半部分
`两边向中间扫描`
voidreorder(int *data,int length)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E8%BE%93%E5%87%BA%E9%93%BE%E8%A1%A8%E4%B8%AD%E5%80%92%E6%95%B0%E7%AC%ACk%E4%B8%AA%E8%8A%82%E7%82%B9)输出链表中倒数第K个节点
`使用两个指针，一个先走k-1步`
voidprint_lastK(LinkList *head);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E5%8F%8D%E8%BD%AC%E9%93%BE%E8%A1%A8)反转链表
`三个指针`
voidreverse(LinkList *head);
反转二叉树呢？
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E5%90%88%E5%B9%B62%E4%B8%AA%E6%8E%92%E5%BA%8F%E7%9A%84%E9%93%BE%E8%A1%A8)合并2个排序的链表
要求合并以后链表任然排序
`递归`
LinkList *merge(LinkList *one,LinkList *two);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%A0%91%E7%9A%84%E5%AD%90%E7%BB%93%E6%9E%84)树的子结构
考察二叉树的基本操作。输入2课二叉树A和B，判断B是不是A的子结构。
structBinaryTreeNode{
    int m_value;
    BinaryTreeNode *m_pleft;
    BinaryTreeNode *m_pRight;
}
```
8                   
       / \              10
      /   \             / \                        
     6     10   子结构 11  9   
    / \       / \            
   5   7  9  11
```
boolsubTree(BinaryTreeNode *root1,BinaryTreeNode *root2);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%BA%8C%E5%8F%89%E6%A0%91%E7%BF%BB%E8%BD%AC)二叉树翻转
88
       / \                   / \
      /   \                 /   \                           
     610     翻转后   106
    / \    / \            / \   / \
   5791111975
`交换每个节点的左右子树`
voidreverse(BinaryTreeNode *root);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%BB%8E%E5%A4%96%E5%90%91%E9%87%8C%E9%A1%BA%E6%97%B6%E9%92%88%E6%89%93%E5%8D%B0%E7%9F%A9%E9%98%B5)从外向里顺时针打印矩阵
voidprint_matrix_clockwise(int *matrix,int cols,int rows);
延伸：按大小顺序打印矩阵
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E5%AE%9E%E7%8E%B0%E4%B8%80%E4%B8%AA%E8%83%BD%E6%89%BE%E5%88%B0%E6%A0%88%E7%9A%84%E6%9C%80%E5%B0%8F%E5%85%83%E7%B4%A0%E7%9A%84%E5%87%BD%E6%95%B0)实现一个能找到栈的最小元素的函数
`最小元素用辅助栈保存`
intmin(Stack *stack)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%A0%88%E7%9A%84%E5%8E%8B%E5%85%A5%E5%BC%B9%E5%87%BA%E5%BA%8F%E5%88%97)栈的压入，弹出序列
输入2个整数序列，第一个序列表示栈的压入顺序，请判断第二个序列是否为该栈的弹出顺序。比如：
1，2，3，4，5是压栈序列，4，5，3，2，1是弹栈序列，但是4，3，5，1，2就不是弹栈序列
boolis_pop_order(int *push,int *pop,int length)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%BB%8E%E4%B8%8A%E5%BE%80%E4%B8%8B%E6%89%93%E5%8D%B0%E4%BA%8C%E5%8F%89%E6%A0%91)从上往下打印二叉树
`辅助队列`
voidprint_binary_level(BinaryTreeNode *root)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%BA%8C%E5%8F%89%E6%90%9C%E7%B4%A2%E6%A0%91%E7%9A%84%E5%90%8E%E7%BB%AD%E9%81%8D%E5%8E%86%E5%BA%8F%E5%88%97)二叉搜索树的后续遍历序列
输入一个整数数组，判断该数组是不是某二叉查找树的后续遍历序列的结果。比如【 5，7，6，9，11，10，8】 是下面二叉查找树的后续遍历结果：
```
8               
       / \                   
      /   \                                            
     6     10     
    / \    / \         
   5   7  9  11
```
`寻找规律`
boolis_post_order(BST *root,int *data, int length);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%BA%8C%E5%8F%89%E6%A0%91%E4%B8%AD%E5%92%8C%E4%B8%BA%E6%9F%90%E4%B8%80%E5%80%BC%E7%9A%84%E8%B7%AF%E5%BE%84)二叉树中和为某一值的路径
```
10               
       / \                   
      /   \                                            
     5     12     
    / \               
   5   7
```
和为22的路径有2条：10--5--7， 10--12
`递归，栈`
voidprint_path(BinaryTree *root,int n)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E5%A4%8D%E6%9D%82%E9%93%BE%E8%A1%A8%E7%9A%84%E5%A4%8D%E5%88%B6)复杂链表的复制
在复杂链表结构中，每个节点都有一个指向下一个节点的m_pNext;还有一个指向任意节点的m_pSibling
typedefstructLinkListNode{
    int m_value;
    LinkListNode *m_pNext;
    LinkListNode *m_pSlbling;
}LinkList;
LinkList * copy(LinkList *head);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%BA%8C%E5%8F%89%E6%90%9C%E7%B4%A2%E6%A0%91%E4%B8%8E%E5%8F%8C%E5%90%91%E9%93%BE%E8%A1%A8)二叉搜索树与双向链表
将二叉搜索树转换成一个排序的双向链表，只调整树中节点的指针指向
`递归``分解问题`
BST *transform(BST *root);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E5%AD%97%E7%AC%A6%E4%B8%B2%E7%9A%84%E6%8E%92%E5%88%97)字符串的排列
输入一个字符串，打印该字符串中字符的所有排列
`递归，分解`
voidprint_full_permutation(char *string)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%95%B0%E7%BB%84%E4%B8%AD%E5%87%BA%E7%8E%B0%E6%AC%A1%E6%95%B0%E8%B6%85%E8%BF%87%E4%B8%80%E5%8D%8A%E7%9A%84%E6%95%B0%E5%AD%97)数组中出现次数超过一半的数字
`遍历数组，下一个数字和之前保存的数字一样就+1，否则就-1`
intfind_more_than_half_num(int *nums ,int length)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#n%E4%B8%AA%E6%95%B4%E6%95%B0%E4%B8%AD%E6%9C%80%E5%B0%8F%E7%9A%84k%E4%B8%AA%E6%95%B0)n个整数中最小的K个数
`快速排序``最大堆`
voidfind_least_k(int *data,int n,int *ouput,int k)
最大的K个数呢？
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E8%BF%9E%E7%BB%AD%E5%AD%90%E6%95%B0%E7%BB%84%E7%9A%84%E6%9C%80%E5%A4%A7%E5%92%8C)连续子数组的最大和
输入一个整数数组，有正有负，求所有子数组的和的最大值
`分析规律``动态规划`
intmax_of_subarray(int *data,int length)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%BB%8E1%E5%88%B0n%E6%95%B4%E6%95%B0%E4%B8%AD1%E5%87%BA%E7%8E%B0%E7%9A%84%E6%AC%A1%E6%95%B0)从1到n整数中，1出现的次数
比如 12，从1到12这些整数中，包含1的数字有 1， 10，11，12 ，1出现了5次
intone_appear_count(int n)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%8A%8A%E6%95%B0%E7%BB%84%E6%8E%92%E6%88%90%E6%9C%80%E5%B0%8F%E7%9A%84%E6%95%B0)把数组排成最小的数
输入一个正整数数组,把所有数字拼起来排出一个最小数
intminSort(int *nums, int length);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%B8%91%E6%95%B0)丑数
> 
只包含因子 2，3，5的数叫做丑数
求按从小到大的顺序，第1500个丑数
intugly(int n)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E7%AC%AC%E4%B8%80%E4%B8%AA%E5%8F%AA%E5%87%BA%E7%8E%B0%E4%B8%80%E6%AC%A1%E7%9A%84%E5%AD%97%E7%AC%A6)第一个只出现一次的字符
在字符串中查找第一个只出现一次的字符
`哈希表：值为出现的次数``二次扫描`
charfind_appear_once_char(char *string)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%95%B0%E7%BB%84%E4%B8%AD%E7%9A%84%E9%80%86%E5%BA%8F%E5%AF%B9)数组中的逆序对
> 
数组中的两个数字如果前面一个数字大于后面的数字，这两个数字组成一个逆序对。如：[7，5，6，4] 的逆序对：(7,5)(7,6)(7,4)(5,4)(6,4)
输入一个数组，求出这个数组逆序对总数。
`归并排序 O(nlogn),空间O(n)`
intreversePairs(int *data,int length)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%B8%A4%E4%B8%AA%E9%93%BE%E8%A1%A8%E7%9A%84%E7%AC%AC%E4%B8%80%E4%B8%AA%E5%85%AC%E5%85%B1%E8%8A%82%E7%82%B9)两个链表的第一个公共节点
`长的链表先走k步`
LinkListNode *common_node(LinkList *head1,LinkList head2);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%95%B0%E5%AD%97%E5%9C%A8%E6%8E%92%E5%BA%8F%E6%95%B0%E7%BB%84%E4%B8%AD%E5%87%BA%E7%8E%B0%E7%9A%84%E6%AC%A1%E6%95%B0)数字在排序数组中出现的次数
比如 {1，2，3，3，3，3，4，5}， 数字 3出现了4次
`使用二分查找找第一个3，和最后一个3出现的位置`
intappear_count(int *nums,int length,int n);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%BA%8C%E5%8F%89%E6%A0%91%E7%9A%84%E6%B7%B1%E5%BA%A6)二叉树的深度
`递归`
inttree_depth(BTree *root);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%95%B0%E7%BB%84%E4%B8%AD%E5%8F%AA%E5%87%BA%E7%8E%B0%E4%B8%80%E6%AC%A1%E7%9A%84%E6%95%B0%E5%AD%97)数组中只出现一次的数字
数组中除了2个数字之外，其他的数组都出现了2次，找出这两个数
`异或``二进制`
> 
如果是只有1个数字只出现一次，我们可以通过对数组依次做异或运算。
如果我们能把原数组分成2个子数组，每个子数组都包含一个只出现一次的数字，问题就能解决了。我们把数组中的所有数字依次做异或操作，如果有2个数字不一样，结果肯定不是0，且异或结果数字的二进制表示中至少有一位是1（不然结果不就是0了）
- 在结果数字二进制表示中找到第一个为1的位的位置，标记n
- 以二进制表示中第n位是不是1为标准，把原数组分成2个子数组
voidfind_two_numbers_appear_once(int *data,int length,int *ouput)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E5%92%8C%E4%B8%BAs%E7%9A%84%E4%B8%A4%E4%B8%AA%E6%95%B0%E5%AD%97-vs--%E5%92%8C%E4%B8%BAs%E7%9A%84%E8%BF%9E%E7%BB%AD%E6%AD%A3%E6%95%B0%E5%BA%8F%E5%88%97)和为s的两个数字 VS 和为s的连续正数序列
有一个递增排序数组，和一个数字s，找出数组中的2个数，使得和等于s。输出任意一对即可
`两边向中间扫描`
voidprint_two_numbers(int *data,int length,int sum)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E5%8F%8D%E8%BD%AC%E5%8D%95%E8%AF%8D%E9%A1%BA%E5%BA%8F-vs-%E5%B7%A6%E6%97%8B%E8%BD%AC%E5%AD%97%E7%AC%A6%E4%B8%B2)反转单词顺序 VS 左旋转字符串
a. 翻转句子中单词的顺序，但单词内字符不变。如 『I am a student』 -> 『student. a am I』
`先以单词为单位翻转,整个句子再次翻转`
char *reverse_by_word(char *string)
b. 左旋转字符串是把字符串其那面的若干位转义到字符串的尾部。比如"abcedfsz"和数字2，结果是"cedfszab"
char *left_rotate_string(char *s,int n)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#n%E4%B8%AA%E8%89%B2%E5%AD%90%E7%9A%84%E7%82%B9%E6%95%B0)n个色子的点数
把n个色子丢地上，朝上一面的点数之和为s。输入n，打印可能的值出现的概率
voidprint_sum_probability(int n)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%89%91%E5%85%8B%E7%89%8C%E4%B8%AD%E7%9A%84%E9%A1%BA%E5%AD%90)扑克牌中的顺子
从扑克牌从随机抽5张牌，判断是不是顺子。A是1，J~~K是11~~13，大小王可以看出任意数字。
boolis_straight(int *data,int length)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E5%9C%86%E5%9C%88%E4%B8%AD%E6%9C%80%E5%90%8E%E5%89%A9%E4%B8%8B%E7%9A%84%E6%95%B0%E5%AD%97%E7%BA%A6%E7%91%9F%E5%A4%AB%E9%97%AE%E9%A2%98)圆圈中最后剩下的数字(约瑟夫问题)
0,1,...,n-1 这n个数字排成一个圆圈，从数字0开始从这个圆圈里面删除第m个数字，求出这个圆圈里最后剩下的数字。
intlast_remaining(unsignedint n,unsignedint m)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%B1%82-12n)求 1+2+...+n
要求不用乘除法，for/while/if/else/switch等关键字及条件判断语句
longlongsum(unsignedint n);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%B8%8D%E7%94%A8%E5%8A%A0%E5%87%8F%E4%B9%98%E9%99%A4%E5%81%9A%E5%8A%A0%E6%B3%95)不用加减乘除做加法
求2个整数之和
`位运算`
intsum(int,int)
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E4%B8%8D%E8%83%BD%E8%A2%AB%E7%BB%A7%E6%89%BF%E7%9A%84%E7%B1%BB)不能被继承的类
```
```
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%8A%8A%E5%AD%97%E7%AC%A6%E4%B8%B2%E8%BD%AC%E6%8D%A2%E6%88%90%E6%95%B4%E6%95%B0)把字符串转换成整数
比如 "12343567754" -> 12343567754
`NULL,空串,正负号,溢出`
intstrToInt(char str);
### [](https://github.com/Rtoax/Knowledge/blob/master/Algorithm/%3C%E5%89%91%E6%8C%87offer%3E-50%E9%81%93%E7%AE%97%E6%B3%95%E9%A2%98.md#%E6%A0%91%E4%B8%AD2%E4%B8%AA%E7%BB%93%E7%82%B9%E7%9A%84%E6%9C%80%E4%BD%8E%E5%85%AC%E5%85%B1%E7%A5%96%E5%85%88)树中2个结点的最低公共祖先
如果这个树是二叉排序树
如果不是二叉排序树，但是有父节点指针
如果不是二叉树，也没有父节点指针
