# Leetcode全链表问题 - 無名黑洞 - CSDN博客
2014年02月05日 10:50:27[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：1760
**目录**
**1、编号4 Add Two Numbers (双指针)2、编号19 Remove Nth Node From End Of List (双指针)3、编号22 Merge K Sorted Lists (排序)4、编号23 Swap Nodes in Pairs (双指针)5、编号24 Reverse Nodes in k-Group (双指针、反转)6、编号58 Rotate List (双指针)7、编号64 Merge Two Sorted Lists (排序)8、编号84 Remove Duplicates from Sorted List9、编号85 Remove Duplicates from Sorted List II10、编号88 Partition List11、编号93 Reverse Linked List II (反转)12、编号139 Copy List with Random Pointer (哈希)13、编号142 Linked List Cycle (双指针)14、编号143 Linked List Cycle II (双指针)15、编号144 Reorder List (双指针、反转)16、编号148 Insertion Sort List (排序)17、编号149 Sort List (排序)**
链表的题，想法不难，但实现起来很容易错。需要熟练掌握链表元素的交换、合并和反转。如果没有特殊说明，各题使用的链表数据结构如下：
```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
```
**1、编号4 Add Two Numbers**
You are given two linked lists representing two non-negative numbers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.
Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
建立一个新链表，然后依次读入l1和l2的数据。注意两个情况：一是当两个元素的和大于10时要进位；还有当l1或l2到达末尾时的处理。这种用两个指针同时往后走的方法叫做双指针法。绝大部分链表问题都可以使用双指针法来求解，了解这一点是有必要的。
```cpp
class Solution {
public:
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        if(!l1 && !l2) return NULL;
        ListNode *resultIter = NULL, *resultIterLast = NULL, *resultHead = NULL;
        
        bool carry = false;
        while(true){
            resultIter = new ListNode( Add(l1?l1->val:0, l2?l2->val:0, carry) );
            if(resultIterLast) resultIterLast->next = resultIter;
            
            if(resultHead == NULL) resultHead = resultIter;
            if(l1) l1 = l1->next;            
            if(l2) l2 = l2->next;
            
            if(!l1 && !l2) break;
            resultIterLast = resultIter;
            resultIter = resultIter->next;
        }
        
        if(carry) {
            ListNode *last = new ListNode(1);
            resultIter->next = last;
        }
        return resultHead;
    }
    
    int Add(int x, int y, bool &carry){
        int r = x + y;
        if(carry) r++;
        if(r >= 10){
            r -= 10;
            carry = true;
        }else carry = false;
        return r;
    }
};
```
**2、编号19 Remove Nth Node From End Of List**
Given a linked list, remove the nth node from the end of list and return its head.
For example, Given linked list: 1->2->3->4->5, and n = 2.
After removing the second node from the end, the linked list becomes 1->2->3->5.
Note: Given n will always be valid. Try to do this in one pass. 
要求只能循环一次。如果只用一个指针是无法找到要删除的点。所以使用双指针法。第一个指针先走n步，然后两个指针同时走，直到第一个指针到达最后一个元素，第二个指针指着的就是要删除元素的前一个元素。
```cpp
class Solution {
public:
    ListNode *removeNthFromEnd(ListNode *head, int n) {
        if(!head) return NULL;
        ListNode* iter = head;
        ListNode* deleteIter = head;
        
        /*Loop 1: create the n gap between two pointers*/
        for(int i = 0; i < n; i++) iter = iter->next;
        if(!iter) return head->next;/*Special case: Return if the element to be removed is head*/
        /*Loop 2: keep going until the end*/
        while(iter->next != NULL){
            iter = iter->next;
            deleteIter = deleteIter->next;
        }
        
        ListNode* tmpNode = deleteIter->next;
        deleteIter->next = tmpNode->next;
        delete tmpNode;
        
        return head;
        
    }
};
```
**3、编号22 Merge K Sorted Lists**
Merge k sorted linked lists and return it as one sorted list. Analyze and describe its complexity.
把所有数据放到一个数组里面排序，然后再根据结果生成新的链表。
```cpp
class Solution {
public:
    ListNode *mergeKLists(vector<ListNode *> &lists) {
        vector<int> totalLists;
        
        for(int i = 0; i < lists.size(); i++)
            while(lists[i] != NULL){
                totalLists.push_back(lists[i]->val);
                lists[i] = lists[i]->next;
            }
        
        sort(totalLists.begin(), totalLists.end());/*NLogN*/
        
        ListNode *result = NULL;
        ListNode *resultIter;
        for(int i = 0; i < totalLists.size(); i++){
            if(result == NULL){
                result = new ListNode(totalLists[i]);
                resultIter = result;
                continue;
            }
            resultIter->next = new ListNode(totalLists[i]);
            resultIter = resultIter->next;
        }
        
        return result;
    }
};
```
**4、编号23 Swap Nodes in Pairs**
Given a linked list, swap every two adjacent nodes and return its head. 
For example, Given 1->2->3->4, you should return the list as 2->1->4->3. 
Your algorithm should use only constant space. You may not modify the values in the list, only nodes itself can be changed
题目要求不能改变链表的值，所以只能通过调整指针来达到目的。
```cpp
class Solution {
public:
    ListNode *swapPairs(ListNode *head) {
        if(head == NULL || head->next == NULL) return head;
        
        ListNode *iter1 = head->next;
        ListNode *iter2 = head;
        ListNode *iter2Parent = NULL;
        
        while(iter1 != NULL){
            iter2->next = iter1->next;
            iter1->next = iter2;
            ListNode *tmp = iter1;
            iter1 = iter2;
            iter2 = tmp;
            
            if(iter2Parent) iter2Parent->next = iter2; else head = iter2;
            
            iter1 = iter1->next;
            iter2 = iter2->next;
            if(iter2Parent) iter2Parent = iter2Parent->next; else iter2Parent = head;
            
            if(iter1) iter1 = iter1->next; else break;
            iter2 = iter2->next;
            iter2Parent = iter2Parent->next;
        }
        
        return head;
    }
};
```
**5、编号24 Reverse Nodes in k-Group**
Given a linked list, reverse the nodes of a linked list k at a time and return its modified list.
If the number of nodes is not a multiple of k then left-out nodes in the end should remain as it is.
You may not alter the values in the nodes, only nodes itself may be changed.
Only constant memory is allowed.
For example,
Given this linked list: 1->2->3->4->5
For k = 2, you should return: 2->1->4->3->5
For k = 3, you should return: 3->2->1->4->5
反转链表的基础题。思路很清楚，但是在不用额外空间和不改动结点值这两个条件下，很容易写错。建议把Reverse()这个函数背下来。
```cpp
class Solution {
public:
    ListNode *reverseKGroup(ListNode *head, int k) {
        if(head == NULL || head->next == NULL || k == 1) return head;
        
        ListNode *dummyHead = new ListNode(0);
        dummyHead->next = head;
        
        ListNode *iter1 = head;
        ListNode *iter0 = dummyHead;
        int count = 0;
        
        while(iter1 != NULL){
            count++;
            if(count == k){
                iter0 = Reverse(iter0, iter1->next);
                iter1 = iter0->next;
                count = 0;
            }
            else iter1 = iter1->next;
        }
        
        return dummyHead->next;
    }
    
    /*Since only constant memory is allowed, time cost is O(k) */
    ListNode* Reverse(ListNode *begin, ListNode *end){
        ListNode* pre = begin->next;
        ListNode* current = pre->next;
        
        while(current != end){
            pre->next = current->next;
            current->next = begin->next;
            begin->next = current;
            current = pre->next;
        }
        return pre;
    }
};
```
**6、编号58 Rotate List**
Given a list, rotate the list to the right by k places, where k is non-negative.
For example:
Given 1->2->3->4->5->NULL and k = 2,
return 4->5->1->2->3->NULL.
双指针，没什么难点。
```cpp
class Solution {
public:
    ListNode *rotateRight(ListNode *head, int k) {
        if(head == NULL || head->next == NULL) return head;
        
        ListNode *Tracker = head;
        ListNode *Looper = head;
        
        int size = 0;
        while(Looper != NULL){
            Looper = Looper->next;
            size++;
        }
        k = k % size;
        Looper = head;
        
        /*Core Loop*/
        int count = 0;
        while(Looper->next != NULL){
            if(count >= k) Tracker = Tracker->next;
            count++;
            Looper = Looper->next;
        }
        
        /*Reconstruction*/
        Looper->next = head;
        head = Tracker->next;
        Tracker->next = NULL;
        
        return head;
    }
};
```
**7、编号64 Merge Two Sorted Lists**
Merge two sorted linked lists and return it as a new list. The new list should be made by splicing together the nodes of the first two lists.
跟编号22差不多的题，做法也一样。要是不想用辅助空间的话，双指针也可以(也更快)。
```cpp
class Solution {
public:
    ListNode *mergeTwoLists(ListNode *l1, ListNode *l2) {
        if(l1 == NULL) return l2;
        if(l2 == NULL) return l1;
        
        vector<int> stdList;
        
        while(l1 != NULL){
            stdList.push_back(l1->val);
            l1 = l1->next;
        }
        
        while(l2 != NULL){
            stdList.push_back(l2->val);
            l2 = l2->next;
        }
        sort(stdList.begin(), stdList.end());/*NlogN*/
        
        ListNode *result = new ListNode(stdList[0]);
        ListNode *resultIter = result;
        
        for(int i = 1; i < stdList.size(); i++){
            ListNode *tmp = new ListNode(stdList[i]);
            resultIter->next = tmp;
            resultIter = resultIter->next;
        }
        
        return result;
    }
};
```
**8、编号84 Remove Duplicates from Sorted List**
Given a sorted linked list, delete all duplicates such that each element appear only once.
For example,
Given 1->1->2, return 1->2.
Given 1->1->2->3->3, return 1->2->3.
因为输入已经排好序了，所以简单。
```cpp
class Solution {
public:
    ListNode *deleteDuplicates(ListNode *head) {
        ListNode *iter = head; 
        
        while(iter != NULL && iter->next != NULL){
            if(iter->next->val == iter->val)
                iter->next = iter->next->next;
            else iter = iter->next;
        }
        
        return head;
    }
};
```
**9、编号85 Remove Duplicates from Sorted List II**
Given a sorted linked list, delete all nodes that have duplicate numbers, leaving only distinct numbers from the original list.
For example,
Given 1->2->3->3->4->4->5, return 1->2->5.
Given 1->1->1->2->3, return 2->3.
上一题的加强版，要求删掉所有duplicates。因为这种情况下，头结点有可能被删除，所以用了dummy结点。
```cpp
class Solution {
public:
    ListNode *deleteDuplicates(ListNode *head) {
        ListNode *iter = head; 
        
        ListNode *dumbHead = new ListNode(0);
        dumbHead->next = head;
        ListNode *dumbIter = dumbHead;/*!!!!!!!!*/
        
        bool removeSelf = false;;
        
        while(iter != NULL && iter->next != NULL){ //!!!!!!! iter!=NULL!!!!!!!
            if(iter->next->val == iter->val){
                iter->next = iter->next->next;
                removeSelf = true;
            }else {
                if(removeSelf){
                    dumbIter->next = iter->next;
                    removeSelf = false;
                }else dumbIter = dumbIter->next;
                iter = iter->next; /*!!!!Make sure dumbIter always point to iter!!!!*/
            }
        }
        
        if(removeSelf)  dumbIter->next = iter->next;
        
        return dumbHead->next;    
    }
};
```
**10、编号88 Partition List**
Given a linked list and a value x, partition it such that all nodes less than x come before nodes greater than or equal to x.
You should preserve the original relative order of the nodes in each of the two partitions.
For example,
Given 1->4->3->2->5->2 and x = 3,
return 1->2->2->4->3->5.
有点麻烦的问题。。。
```cpp
class Solution {
public:
    ListNode *partition(ListNode *head, int x) {
        ListNode* p = new ListNode(x-1);  
        p->next = head;  
        head = p;  
        ListNode* pre;  
        while(p!=NULL && p->val < x) {  
            pre = p;  
            p = p->next;  
        }  
        if(p!=NULL) {      
            ListNode* cur = pre;  
            while(p!=NULL) {  
                if(p->val <x) {  
                    ListNode* temp = cur->next;  
                    pre->next = p->next;  
                    cur->next = p;  
                    cur = p;  
                    p->next = temp;  
                    p = pre;  
                }  
                pre=p;  
                p= p->next;        
            }  
      }  
      ListNode* temp = head;  
      head = head->next;  
      delete temp;  
      return head;      
    }
};
```
**11、编号93 Reverse Linked List II**
Reverse a linked list from position m to n. Do it in-place and in one-pass.
For example:
Given 1->2->3->4->5->NULL, m = 2 and n = 4,
return 1->4->3->2->5->NULL.
Note:
Given m, n satisfy the following condition:
1 ≤ m ≤ n ≤ length of list.
只要用跟编号24一模一样的Reverse()函数就可以了。当然这题要用递归做也可以。
```cpp
class Solution {
public:
    ListNode *reverseBetween(ListNode *head, int m, int n) {
        if(head == NULL) return head;
        
        ListNode *dummyHead = new ListNode(0);
        dummyHead->next = head;
        
        ListNode *iter = head;
        ListNode *pm_prev = dummyHead;
        ListNode *pn = head;
        
        int count = 1;
        while(iter->next != NULL){
            if(count < m) pm_prev = pm_prev->next;
            if(count < n) pn = pn->next;
            iter = iter->next;
            ++count;
        }
        
        Reverse(pm_prev, pn->next);
        return dummyHead->next;
    }
    
    ListNode* Reverse(ListNode *begin, ListNode *end){
        ListNode* pre = begin->next;
        ListNode* current = pre->next;
        
        while(current != end){
            pre->next = current->next;
            current->next = begin->next;
            begin->next = current;
            current = pre->next;
        }
        return pre;
    }
    
};
```
**12、编号139 Copy List with Random Pointer**
A linked list is given such that each node contains an additional random pointer which could point to any node in the list or null.
Return a deep copy of the list
这个实际上是图的拷贝，是很重要的一类题。因为每个结点可以随机指向其他结点，想一次拷贝完全不太可能。这里要用哈西表的数据结构（unordered_map）来记录随机指针指向哪个元素。从这开始会遇到很多哈西表的问题。
```cpp
/**
 * Definition for singly-linked list with a random pointer.
 * struct RandomListNode {
 *     int label;
 *     RandomListNode *next, *random;
 *     RandomListNode(int x) : label(x), next(NULL), random(NULL) {}
 * };
 */
class Solution {
public:
    RandomListNode *copyRandomList(RandomListNode *head) {
        if(head==NULL) return NULL;
        
        RandomListNode *iter = head;
        RandomListNode *iter_random;
        RandomListNode *iter_next;
        
        RandomListNode *newHead = new RandomListNode(head->label);
        
        unordered_map<RandomListNode*, RandomListNode*> hashmap; //<oldNode, newNode>
        hashmap[iter] = newHead;
        
        while(iter != NULL) { /*1 pass build*/
            /*Build random*/
            iter_random = iter->random;
            if(iter_random != NULL) {
                if(hashmap.count(iter_random) == 0) {
                    RandomListNode *tmp = new RandomListNode(iter_random->label);
                    hashmap[iter_random] = tmp;
                    hashmap[iter]->random = tmp;
                }
                else hashmap[iter]->random = hashmap[iter_random];
            }
            
            /*Build next*/
            iter_next = iter->next;
            if(iter_next!=NULL) {
                if(hashmap.count(iter_next) == 0) {
                    RandomListNode *tmp = new RandomListNode(iter_next->label);
                    hashmap[iter_next] = tmp;
                    hashmap[iter]->next = tmp;
                }
                else hashmap[iter]->next = hashmap[iter_next];
            }
            
            iter = iter->next;
        }
        return newHead;
    }
};
```
**13、编号142 Linked List Cycle**
Given a linked list, determine if it has a cycle in it.
Follow up:
Can you solve it without using extra space?
一块一慢双指针解决。面试常见题了。
```cpp
class Solution {
public:
    bool hasCycle(ListNode *head) {
        if(head == NULL) return false;
        
        ListNode *iter1 = head->next;
        ListNode *iter2 = head;
        
        bool iter2Go = false;
        
        while(iter1 != NULL && iter1->next != NULL){
            if(iter1 == iter2) return true;
            iter1 = iter1->next;
            if(iter2Go)  iter2=iter2->next;
            iter2Go = !iter2Go;
        }
        
        return false;
    }
};
```
**14、编号143 Linked List Cycle II**
Given a linked list, return the node where the cycle begins. If there is no cycle, return null.
Follow up:
Can you solve it without using extra space?
上题的升级版。只要搞清楚规律就好办了。方法是当两个指针重合后，把其中一个指针回归第一个结点，然后两个同时走，当再次相遇时就使圆环的开始点。[图片说明点这里。]()
```cpp
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        if(head == NULL) return NULL;
        
        ListNode *iter1 = head;
        ListNode *iter2 = head;
        
        while(true){
            if(!iter1 || !iter2) return NULL;
            iter1 = iter1->next;
            if(iter1 != NULL) iter1 = iter1->next;  /*increase iter1 twice each time*/
            else return NULL;
            
            iter2 = iter2->next;
            
            /*Compare*/
            if(iter1 == iter2) {
                iter2 = head;
                while(iter1 != iter2){
                    iter1 = iter1->next;
                    iter2 = iter2->next;
                }    
                return iter1;
            }
        }
        
        
    }
};
```
**15、编号144 Reorder List**
Given a singly linked list L: L0→L1→…→Ln-1→Ln,
reorder it to: L0→Ln→L1→Ln-1→L2→Ln-2→…
You must do this in-place without altering the nodes' values.
For example,
Given {1,2,3,4}, reorder it to {1,4,2,3}.
分为四步。第一步找到链表长度。第二步把链表从中间分为两个子链表。第三步反转第二个子链表(使用跟编号24，编号93一模一样的Reverse函数)。最后把第二个子链表Merge到第一个子链表里面。
```cpp
class Solution {
public:
    void reorderList(ListNode *head) {
        if (head == NULL)  return;
        
        //Step1: Get the List len.
        ListNode *tmp = head;
        int len = 0;
        while (tmp != NULL) {
            tmp = tmp->next;
            ++len;
        }
        int num_insertion = (len-1)/2;
        
        //Step2: Seperate and find the start of second half.
        tmp = head;
        int count = 0;
        while (count < len-1-num_insertion) {
            tmp = tmp->next;
            ++count;
        }
        ListNode *second_half = tmp->next;
        tmp->next= NULL;
        
        //Step3: Reverse the second half of the List.
        if (second_half != NULL && second_half->next != NULL) {
            ListNode *dummyHead = new ListNode(0);
            dummyHead->next = second_half;
            Reverse(dummyHead, NULL); 
            second_half = dummyHead->next;
        }
        
        //Step4 Insert second half to first half.
        ListNode *first_half = head;
        while (second_half != NULL) {
            ListNode *temp1 = second_half->next;
            second_half->next = first_half->next;
            first_half->next = second_half;
            second_half = temp1;
            first_half = first_half->next->next;
        }
    }
    
    ListNode* Reverse(ListNode *begin, ListNode *end){
        ListNode* pre = begin->next;
        ListNode* current = pre->next;
        
        while(current != end){
            pre->next = current->next;
            current->next = begin->next;
            begin->next = current;
            current = pre->next;
        }
        return pre;
    }
};
```
**16、编号148 Insertion Sort List**
Sort a linked list using insertion sort.
二重循环，没什么好说的。
```cpp
class Solution {
public:
    /*O^2*/
    ListNode *insertionSortList(ListNode *head) {
        if(head == NULL) return head;
	ListNode* iter = head->next;
	ListNode* tmp = NULL;
	while(iter != NULL){
		tmp = head;
		//use iter->val as reference, find the one that bigger than iter->val
		while(tmp->val < iter->val && tmp != iter)	tmp = tmp->next; 
		//find the tmp->val before iter and bigger then iter->val
		if(tmp != iter){
			while(tmp != iter){
				Swap(tmp->val, iter->val);
				tmp = tmp->next;
			}
		}	
		iter = iter->next;
	}
	return head;
    }
    
    void Swap(int &x, int &y){
        int tmp = x;
        x = y;
        y = tmp;
    }
};
```
**16、编号149 Sort List**
Sort a linked list in O(n log n) time using constant space complexity.
归并排序的时间复杂度是O(n log n)。
```cpp
class Solution {
public:
    ListNode *sortList(ListNode *head) {
        if(!head)  return NULL;  
        
        ListNode *iter1 = head; //for the first half
        ListNode *iter2 = head; // for the second half
        
        //compute the mid index
        int length = 0;  
        while(iter1)  {  
            length++;  
            iter1 = iter1->next;  
        }  
        if(length == 1)   return head;  
        int mid = length/2;    //计算结点个数，将链表分开。 
        
        //将链表分开
        ListNode *tmpNode = NULL;  
        for(int i = 0; i < mid && iter2 != NULL; i++)  {  
            if(i == mid - 1) tmpNode = iter2;  
            iter2 = iter2->next;  
        }  
        if(tmpNode != NULL) tmpNode->next=NULL;  
    
        //分别对两段排序  
        iter1 = head;  //retrive the head
        iter1 = sortList(iter1);
        iter2 = sortList(iter2);  
    
        ListNode *newHead = NULL;
        ListNode *newIter = NULL; 
        
        while(iter1 && iter2) {  //合并
            if(iter1->val <= iter2->val)  {  
                if(!newHead)  newIter = newHead = iter1;             
                else{  
                    newIter->next = iter1;  
                    newIter = iter1;  
                }  
                iter1 = iter1->next;  
            }else{  
                if(!newHead) newHead = newIter = iter2;             
                else{  
                    newIter->next = iter2;  
                    newIter = iter2;  
                }  
                iter2 = iter2->next;  
            }     
        }  
        if(iter1) newIter->next = iter1;  
        if(iter2) newIter->next = iter2;  
        return newHead;  
    }
};
```
**参考文献：**
http://oj.leetcode.com/problems/
