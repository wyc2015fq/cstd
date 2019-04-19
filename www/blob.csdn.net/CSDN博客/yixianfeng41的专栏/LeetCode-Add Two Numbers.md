# LeetCode-Add Two Numbers - yixianfeng41的专栏 - CSDN博客
2015年12月06日 13:19:41[第2梦](https://me.csdn.net/yixianfeng41)阅读数：723
个人分类：[数据结构](https://blog.csdn.net/yixianfeng41/article/category/5829745)
所属专栏：[数据结构与算法](https://blog.csdn.net/column/details/14515.html)
## 题目：
给出两个数字，他们是反向保存在链表中的，计算两个数的和，也反向保存在链表中返回。
## 方法：
我开始想到的就是先将链表中的数字还原，然后相加，再反向保存到链表中，很是麻烦。这就是常规思路，总想着数字都是从右到左相加有进位的向左边进位，其实可以摆脱常规，数字从左向右相加也可以的，有进位就向右进位。
## 代码：
```cpp
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) 
{
    int sum=0;
    ListNode *headNode,*rearNode;
    rearNode=headNode=NULL;
    while(l1||l2||sum) //判断sum是最右边还要进位，也就是比如都是三位数相加，变成了四位
    {
        if(l1)
        {
          sum+=l1->val;
          l1=l1->next;
        }
        if(l2)
        {
            sum+=l2->val;
            l2=l2->next; 
        }
       if(!rearNode)
	    rearNode=headNode=new ListNode(sum%10);
       else
       {
	     rearNode->next=new ListNode(sum%10);
	     rearNode=rearNode->next; 
       }
       sum/=10; //是否有进位
    }
    return headNode;
}
```
