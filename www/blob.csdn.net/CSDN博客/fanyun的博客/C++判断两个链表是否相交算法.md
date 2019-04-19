# C++判断两个链表是否相交算法 - fanyun的博客 - CSDN博客
2018年10月31日 22:13:26[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：40
1.判断一个链表是否相交，若相交，求交点（假设链表带环）
2.具体思路如下图所示
![](https://img-blog.csdnimg.cn/20181031214122549.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/2018103121414366.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20181031214208836.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==,size_16,color_FFFFFF,t_70)
既然已经分析清楚，那么代码就很好实现了.
```cpp
Node* IsHaveCrossNode(Node* head1,Node* head2)
    {
        assert(head1);
        assert(head2);
        Node* meetNode1 = IsCircle(head1);
        Node* meetNode2 = IsCircle(head2);
        //排除第1种情况
        if (meetNode1 == NULL || meetNode2 == NULL)
        {
            return NULL;
        }
        //说明两个链表都带环
        //根据相遇点排除第2种情况
        Node* cur1 = meetNode1->_next;
        while (cur1 != meetNode1 && cur1 != meetNode2)
        {
            cur1 = cur1->_next;
        }
        if (cur1 != meetNode2)
        {
            return NULL;
        }
        //区分第3和第4种情况（需要求出两个入口点）
        Node* entry1 = GetEntryNode(head1, meetNode1);
        Node* entry2 = GetEntryNode(head2, meetNode2);
        //第3种情况(转化为求两个链表的交点,链表不带环时)
        if (entry1 == entry2)
        {
            entry1->_next = NULL;
            entry2->_next = NULL;
            Node* meet = IsMeet(head1, head2);
            return meet;
        }
        //第4种情况（有两个入口点，返回一个即可）
        return entry1;
    }
内部调用的函数如下：
```
```cpp
//判断一个链表是否带环
    //思路：快慢指针,返回相遇点
    Node* IsCircle(Node* head)
    {
        if (head == NULL)
        {
            return NULL;
        }
        Node* pFast = head;
        Node* pSlow = head;
        //快指针一次走两步，慢指针一次走一步
        while (pFast && pFast->_next)
        {
            pFast = pFast->_next->_next;
            pSlow = pSlow->_next;
            if (pFast == pSlow)
            {
                break;
            }
        }
        if (pFast && pFast->_next)
        {
            return pFast;
        }
        return NULL;
    }
Node* GetEntryNode(Node* head, Node* meetNode)
    {
        assert(head && meetNode);
        Node* p1 = head;
        Node* p2 = meetNode;
        while (p1 != p2)
        {
            p1 = p1->_next;
            p2 = p2->_next;
        }
        return p1;
    }
//判断是否相交，判断尾节点
    //若相交，则返回交点
    Node* IsMeet(Node* head1,Node* head2)
    {
        assert(head1 && head2);
        Node* cur1 = head1;
        int count1 = 0;
        Node* cur2 = head2;
        int count2 = 0;
        while (cur1->_next)
        {
            ++count1;
            cur1 = cur1->_next;
        }
        while (cur2->_next)
        {
            count2++;
            cur2 = cur2->_next;
        }
        if (cur1 != cur2)
        {
            return NULL;//说明没有交点
        }
        int D_val = count2 - count1;
        if (D_val < 0)
        {
            D_val = -D_val;
        }
        //
        cur1 = head1;
        cur2 = head2;
        if (count1 < count2)
        {
            //让head2链表先走D_val长度
            while (cur2 && D_val--)
            {
                cur2 = cur2->_next;
            }
            while (cur2 && cur1 != cur2)
            {
                cur1 = cur1->_next;
                cur2 = cur2->_next;
            }
            if (cur1 == cur2)
            {
                return cur1;
            }
        }
        else
        {
            //让head1链表先走D_val长度
            while (cur1 && D_val--)
            {
                cur1 = cur1->_next;
            }
            while (cur1 && cur1 != cur2)
            {
                cur1 = cur1->_next;
                cur2 = cur2->_next;
            }
            if (cur1 == cur2)
            {
                return cur1;
            }
        }
        return NULL;
    }
```
