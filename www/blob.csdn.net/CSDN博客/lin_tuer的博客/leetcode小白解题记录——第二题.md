
# leetcode小白解题记录——第二题 - lin_tuer的博客 - CSDN博客


2016年08月16日 16:31:42[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：1382


### 2. Add Two Numbers
You are given two linked lists representing two non-negative numbers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.
Eg:
Input:(2 -> 4 -> 3) + (5 -> 6 -> 4)
Output:7 -> 0 -> 8
所需基础知识：
## 1.链表
>与数组的区别：数组式计算机根据事先定义好的数组类型与长度自动为其分配一连续的存储单元，相同数组的位置和距离都是固定的，也就是说，任何一个数组元素的地址都可一个简单的公式计算出来，因此这种结构可以有效的对数组元素进行随机访问。但若对数组元素进行插入和删除操作，则会引起大量数据的移动，从而使简单的数据处理变得非常复杂，低效。为了能有效地解决这些问题，一种称为“链表”的数据结构得到了广泛应用。
>链表是一种动态数据结构，他的特点是用一组任意的存储单元（可以是连续的，也可以是不连续的）存放数据元素。
链表中每一个元素成为“结点”，每一个结点都是由数据域和指针域组成的，每个结点中的指针域指向下一个结点。Head是“头指针”，表示链表的开始，用来指向第一个结点，而最后一个指针的指针域为NULL(空地址)，表示链表的结束。
## 2.int 与 long int（这个好像关系不大，作个人知识储备用）
>64位系统：long是8字节64位，int是4字节32位。
# 解题代码：
```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        
        if(l1 == NULL) return l2;
        if(l2 == NULL) return l1;
        
        int flag = 0;     //进位标志
        ListNode* tail = new ListNode(0); 
        ListNode* ptr = tail; //?????

        while(l1 != NULL || l2 != NULL){
            int val1 = 0;
            if(l1 != NULL){
                val1 = l1->val;
                l1 = l1->next;
            }

            int val2 = 0;
            if(l2 != NULL){
                val2 = l2->val;
                l2 = l2->next;
            }

            int tmp = val1 + val2 + flag;
            ptr->next = new ListNode(tmp % 10);
            flag = tmp / 10;
            ptr = ptr->next;
        }

        if(flag == 1){
            ptr->next = new ListNode(1);
        }
        return tail->next; //??????????????/
    }
};
```


