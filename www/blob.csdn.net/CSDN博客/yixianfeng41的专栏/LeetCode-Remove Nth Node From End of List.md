# LeetCode-Remove Nth Node From End of List - yixianfeng41的专栏 - CSDN博客
2015年12月07日 21:43:34[第2梦](https://me.csdn.net/yixianfeng41)阅读数：794
个人分类：[数据结构](https://blog.csdn.net/yixianfeng41/article/category/5829745)
所属专栏：[数据结构与算法](https://blog.csdn.net/column/details/14515.html)
## 方法一：
先计算出链表总的节点数，再找要删除的节点
```cpp
ListNode* removeNthFromEnd(ListNode* head, int n) 
{
      if(head==NULL)
          return NULL;
      int count=0;
      ListNode *tempNode1=head,*tempNode2=head;
      while(tempNode1)
      {
          tempNode1=tempNode1->next;
          count++;
      }
      if(n==count)
      {
	  return head->next;
      }
      int step=count-n-1;
      for(;step>0;step--)
      {
          tempNode2=tempNode2->next;
      }
      tempNode2->next=tempNode2->next->next;
      return head;
}
```
## 方法二：
两个头结点，一个先走n步，然后再一起走，就找到要删除的节点了。
```cpp
ListNode* removeNthFromEnd(ListNode* head, int n) 
{
        ListNode *tempNode1=head,*tempNode2=head;
	if(!tempNode1->next&&n==1) return NULL;
	int k=1;
	while(k++<=n) //先走n步
	{
		if(tempNode1->next) //注意头结点的处理
			return head->next;
		tempNode1=tempNode1->next;
	}
	while(tempNode1->next)
	{		
		   tempNode1=tempNode1->next;
		   tempNode2=tempNode2->next;
	}
	tempNode2->next=tempNode2->next->next;
	return head;
}
```
## 方法三：
跟方法二思想一样，亮点在删除节点处，即二级指针的运用。
```cpp
ListNode* removeNthFromEnd(ListNode* head, int n)
{
        ListNode** t1 = &head, *t2 = head;
        for(int i = 1; i < n; ++i)
        {
            t2 = t2->next;
        }
        while(t2->next != NULL)
        {
            t1 = &((*t1)->next); //往下走
            t2 = t2->next;
        }
        *t1 = (*t1)->next; //删除节点
        return head;
}
```
