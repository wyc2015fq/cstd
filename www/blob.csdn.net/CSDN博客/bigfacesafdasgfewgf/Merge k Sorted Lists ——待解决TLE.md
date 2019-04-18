# Merge k Sorted Lists ——待解决TLE - bigfacesafdasgfewgf - CSDN博客





2015年05月09日 11:16:31[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：363标签：[LeetCode																[链表																[递归																[vector																[merge](https://so.csdn.net/so/search/s.do?q=merge&t=blog)
个人分类：[LeetCode](https://blog.csdn.net/puqutogether/article/category/2832605)








    【题目】Merge k sorted linked lists and return it as one sorted list. Analyze and describe its complexity.




    【分析】




    思路基于两个有序链表合并。只不过每次都需要遍历选出K个链表中最小的那个数。




    【代码】

    下面的代码始终LTE，原因是一个很长的case超时，改了好久也没有改对，希望后面可以纠正！



```cpp
class Solution {
public:
    ListNode *mergeKLists(vector<ListNode *> &lists)
    {
        int n=lists.size();
        if(n==0)return NULL;      
        ListNode *head=new ListNode(INT_MIN);
        ListNode *ret=head;
        int min; 
        bool finish;
        while(1)
       {
          finish=true;
          min=INT_MAX;
          int i;int mini;
          for(i=0;i<lists.size();i++)
          {
             if(lists[i]!=NULL)
             {
                if(lists[i]->val<min)
                {
                    min=lists[i]->val;
                    mini=i;
                    finish=false;
                }
             }           
          }
          if(finish)break;   
          head->next=lists[mini];
          head=head->next;
          lists[mini]=lists[mini]->next;     
       }
       return ret->next;
    }
};
```](https://so.csdn.net/so/search/s.do?q=vector&t=blog)](https://so.csdn.net/so/search/s.do?q=递归&t=blog)](https://so.csdn.net/so/search/s.do?q=链表&t=blog)](https://so.csdn.net/so/search/s.do?q=LeetCode&t=blog)




