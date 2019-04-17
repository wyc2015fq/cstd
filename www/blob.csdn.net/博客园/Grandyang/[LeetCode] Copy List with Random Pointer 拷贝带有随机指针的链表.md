# [LeetCode] Copy List with Random Pointer 拷贝带有随机指针的链表 - Grandyang - 博客园







# [[LeetCode] Copy List with Random Pointer 拷贝带有随机指针的链表](https://www.cnblogs.com/grandyang/p/4261431.html)







A linked list is given such that each node contains an additional random pointer which could point to any node in the list or null.

Return a deep copy of the list.



这道链表的深度拷贝题的难点就在于如何处理随机指针的问题，由于每一个节点都有一个随机指针，这个指针可以为空，也可以指向链表的任意一个节点，如果我们在每生成一个新节点给其随机指针赋值时，都要去遍历原链表的话，OJ上肯定会超时，所以我们可以考虑用Hash map来缩短查找时间，第一遍遍历生成所有新节点时同时建立一个原节点和新节点的哈希表，第二遍给随机指针赋值时，查找时间是常数级。代码如下：



解法一：

```
class Solution {
public:
    RandomListNode *copyRandomList(RandomListNode *head) {
        if (!head) return NULL;
        RandomListNode *res = new RandomListNode(head->label);
        RandomListNode *node = res;
        RandomListNode *cur = head->next;
        map<RandomListNode*, RandomListNode*> m;
        m[head] = res;
        while (cur) {
            RandomListNode *tmp = new RandomListNode(cur->label);
            node->next = tmp;
            m[cur] = tmp;
            node = node->next;
            cur = cur->next;
        }
        node = res;
        cur = head;
        while (node) {
            node->random = m[cur->random];
            node = node->next;
            cur = cur->next;
        }
        return res;
    }
};
```



当然，如果使用哈希表占用额外的空间，如果这道题限制了空间的话，就要考虑别的方法。下面这个方法很巧妙，具体细节可参见神网友[水中的鱼的博客](http://fisherlei.blogspot.com/2013/11/leetcode-copy-list-with-random-pointer.html)，该方法可以分为以下三个步骤：

1. 在原链表的每个节点后面拷贝出一个新的节点

2. 依次给新的节点的随机指针赋值，而且这个赋值非常容易 cur->next->random = cur->random->next

3. 断开链表可得到深度拷贝后的新链表



解法二：

```
class Solution {
public:
    RandomListNode *copyRandomList(RandomListNode *head) {
        if (!head) return NULL;
        RandomListNode *cur = head;
        while (cur) {
            RandomListNode *node = new RandomListNode(cur->label);
            node->next = cur->next;
            cur->next = node;
            cur = node->next;
        }
        cur = head;
        while (cur) {
            if (cur->random) {
                cur->next->random = cur->random->next;
            }
            cur = cur->next->next;
        }
        cur = head;
        RandomListNode *res = head->next;
        while (cur) {
            RandomListNode *tmp = cur->next;
            cur->next = tmp->next;
            if(tmp->next) tmp->next = tmp->next->next;
            cur = cur->next;
        }
        return res;
    }
};
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












