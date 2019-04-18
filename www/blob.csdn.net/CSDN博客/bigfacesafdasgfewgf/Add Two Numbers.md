# Add Two Numbers - bigfacesafdasgfewgf - CSDN博客





2015年01月13日 15:22:59[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：353标签：[leetcode																[list																[链表																[进位																[指针](https://so.csdn.net/so/search/s.do?q=指针&t=blog)
个人分类：[LeetCode](https://blog.csdn.net/puqutogether/article/category/2832605)





**Add Two Numbers**




You are given two linked lists representing two non-negative numbers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.

Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8

需要考虑的情况比较多，考虑全了就好~




代码如下：（代码有待改进）



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
    int getLength(ListNode* head)  //get the length of list
    {
        if(head==NULL)
        {
            return 0; 
        }
        int length=0; 
        ListNode* p=head; 
        while(p)
        {
            length++; 
            p=p->next; 
        }
        return length; 
    }
    
    ListNode* fun(ListNode *l1, ListNode *l2) 
    {
        ListNode *p=l1;
        ListNode *q=l2;
        ListNode *r=p;
        bool flag=false;
        while(q)
        {
            r=p;
            if(flag)
            {
                p->val+=q->val+1;
                flag=false;
            }
            else
                p->val+=q->val;
            if(p->val>9)
            {
                flag=true;
                p->val=p->val%10;
            }
            p=p->next;
            q=q->next;
        }
        while(flag)
        {
            if(!p)
            {
                ListNode *newnode=new ListNode(1);
                r->next=newnode;
                flag=false;
            }
            else
            {
                p->val+=1;
                flag=false;
                if(p->val>9)
                {
                    flag=true;
                    p->val=p->val%10;
                    r=p;
                    p=p->next;
                }
            }
        }
        return l1;
    }
    
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        
        int len1=getLength(l1); 
        int len2=getLength(l2); 
        
        if(len1==0)
        {
            return l2; 
        }
        else if(len2==0)
        {
            return l1; 
        }
        else if(len1<len2)
        {
            return fun(l2, l1); 
        }
        else 
        {
            return fun(l1, l2); 
        }
        
    }
};
```](https://so.csdn.net/so/search/s.do?q=进位&t=blog)](https://so.csdn.net/so/search/s.do?q=链表&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)




