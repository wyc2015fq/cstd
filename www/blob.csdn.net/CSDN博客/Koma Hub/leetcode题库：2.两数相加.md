# leetcode题库：2.两数相加 - Koma Hub - CSDN博客
2018年04月14日 11:06:17[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：519
给定两个非空链表来表示两个非负整数。位数按照逆序方式存储，它们的每个节点只存储单个数字。将两数相加返回一个新的链表。
你可以假设除了数字 0 之外，这两个数字都不会以零开头。
示例：
```
输入：(2 -> 4 -> 3) + (5 -> 6 -> 4)
输出：7 -> 0 -> 8
原因：342 + 465 = 807
```
我的答案：
```cpp
/**
 * ADD two linklist val to INT
 */
int ADD(struct ListNode* l1, struct ListNode* l2){
    float v1 = 0, v2 = 0;
    int c1 = 0, c2 = 0;
    
    do{
        v1 = v1 + l1->val * pow(10,c1);
        l1 = l1->next;
        c1 ++;
    }while(l1 != NULL);
    do{
        v2 = v2 + l2->val * pow(10,c2);
        l2 = l2->next;
        c2 ++;
    }while(l2 != NULL);
    return (int)(v1) + (int)(v2);
}
/**
 * Int back sort
 */
void backInt(int *in){
    int i;
    int v = *in;
    *in = 0;
    while(v>0){
        *in = (*in)*10 +v%10;
        v = v/10;
    };
}
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) 
{
    int v3 = ADD(l1,l2);
    backInt(&v3);   
    struct ListNode *p, *node;
    p = (struct ListNode*)malloc(sizeof(struct ListNode));
    p->val = v3%10;
    v3 /= 10;
    
    while(v3>0){
        node = (struct ListNode*)malloc(sizeof(struct ListNode));
        node->val = v3%10;
        v3 /= 10;
        node->next = p;
        p = node;
        node = NULL;
        
    }
    
    return p;
}
```
结果：
```
代码执行结果：
我的输入
[2,1,3]
[5,1,1]
我的答案
[7,2,4]
预期答案
[7,2,4]
```
