# [LeetCode] Split Linked List in Parts 拆分链表成部分 - Grandyang - 博客园







# [[LeetCode] Split Linked List in Parts 拆分链表成部分](https://www.cnblogs.com/grandyang/p/7878548.html)







Given a (singly) linked list with head node `root`, write a function to split the linked list into `k` consecutive linked list "parts".

The length of each part should be as equal as possible: no two parts should have a size differing by more than 1. This may lead to some parts being null.

The parts should be in order of occurrence in the input list, and parts occurring earlier should always have a size greater than or equal parts occurring later.

Return a List of ListNode's representing the linked list parts that are formed.

Examples 1->2->3->4, k = 5 // 5 equal parts [ [1], [2], [3], [4], null ]

Example 1:
Input: 
root = [1, 2, 3], k = 5
Output: [[1],[2],[3],[],[]]
Explanation:
The input and each element of the output are ListNodes, not arrays.
For example, the input root has root.val = 1, root.next.val = 2, \root.next.next.val = 3, and root.next.next.next = null.
The first element output[0] has output[0].val = 1, output[0].next = null.
The last element output[4] is null, but it's string representation as a ListNode is [].



Example 2:
Input: 
root = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10], k = 3
Output: [[1, 2, 3, 4], [5, 6, 7], [8, 9, 10]]
Explanation:
The input has been split into consecutive parts with size difference at most 1, and earlier parts are a larger size than the later parts.



Note:
- The length of `root` will be in the range `[0, 1000]`.
- Each value of a node in the input will be an integer in the range `[0, 999]`.
- `k` will be an integer in the range `[1, 50]`.



这道题给我们一个链表和一个正数k，让我们分割链表成k部分，尽可能的平均分割，如果结点不够了，就用空结点，比如例子1中的。如果无法平均分，那么多余的结点就按顺序放在子链表中，如例子2中所示。我们要知道每个部分结点的个数，才能将整个链表断开成子链表，所以我们首先要统计链表中结点的总个数，然后除以k，得到的商avg就是能分成的部分个数，余数ext就是包含有多余的结点的子链表的个数。我们开始for循环，循环的结束条件是i小于k且root存在，要生成k个子链表，在循环中，先把头结点加入结果res中对应的位置，然后就要遍历该子链表的结点个数了，首先每个子链表都一定包含有avg个结点，这是之前除法得到的商，然后还要有没有多余结点，如果i小于ext，就说明当前子链表还得有一个多余结点，然后我们将指针向后移动一个，注意我们这里的j是从1开始，我们希望移动到子链表的最后一个结点上，而不是移动到下一个子链表的首结点，因为我们要断开链表。我们新建一个临时结点t指向下一个结点，也就是下一个子链表的首结点，然后将链表断开，再将root指向临时结点t，这样就完成了断开链表的操作，参见代码如下：



```
class Solution {
public:
    vector<ListNode*> splitListToParts(ListNode* root, int k) {
        vector<ListNode*> res(k);
        int len = 0;
        for (ListNode *t = root; t; t = t->next) ++len;
        int avg = len / k, ext = len % k;
        for (int i = 0; i < k && root; ++i) {
            res[i] = root;
            for (int j = 1; j < avg + (i < ext); ++j) {
                root = root->next;
            }
            ListNode *t = root->next;
            root->next = NULL;
            root = t;
        }
        return res;
    }
};
```



下面的内容和本题无关，是博主对于这个感恩节刷屏的大事，国内某恶魔色幼儿园的侵害幼童事件的看法。博主经常听人说，贫穷限制了我们的想象，意思是你永远无法了解富人生活的奢华程度。同样的道理，博主要说，善良限制了我们的想象，你永远无法想象这个世界恶魔的邪恶程度。鲁迅先生说，我向来不惮以最坏的恶意揣测中国人，实际应该改成，我向来不惮以最坏的恶意揣测人类。人性中的恶就像潘多拉的盒子，在没有限制和约束的时候就会被打开，飞出去祸害众生。幼童是人的一生中可以说性格形成的重要时期，也是最脆弱最容易受到伤害的时期，多少变态的罪犯都是有过幼年时期被侵害的经历，形成了畸形的人格，幼年时期被侵害就像被在心中种下了恶，恶之花结出了恶果从而再去侵害其他无辜的人，形成了恶性循环，贻害无穷。难以想象，是什么样扭曲的心灵可以对那么天真无邪的儿童下得去毒手。毕竟这些受侵害的孩子还有父母，侵害还是可能被发现，博主不敢想象孤儿院或者聋哑学校的那些可怜的孩子遭受了侵害，谁会为他们伸张正义。韩国拍出了《熔炉》，《素媛》这些伟大的电影去直面人性中的恶，其中《熔炉》更是推动了韩国对于幼童侵害方面的立法的完善。欣慰的是，国内近期上映的《嘉年华》也是一部反映社会问题的电影，希望能有多一些这类型的电影出现，直到成为中国《熔炉》，改变完善国内相关保障儿童安全的法案。另一方面，家长们也需要加强对于幼童这方面的教育，这一点美国做的能好一些，他们不会让陌生人轻易碰触，估计家长的功课做的比较足。这边的学校也会让做在线的儿童保护的trainning，说明还是相当重视的。国内由于种种原因，对于性教育这方面总感觉能拖就拖，或者干脆自己去领悟，记得之前某出版社曾推出过一本幼童性教育的书，后来因为家长们的强烈反对没有使用，博主也看了，当时偏向于保守的博主也觉得的这书有些“少儿不宜”，但是现在看来，与其被坏人侵害，还不如早些普及这些知识，及早预防，毕竟你觉得孩子小，不应该接触这些东西，但是坏人不会觉得你孩子小而不施加侵害。希望善恶到头终有报，希望世上能少一些罪恶，多一些美好。



类似题目：

[Rotate List](http://www.cnblogs.com/grandyang/p/4355505.html)

[Odd Even Linked List](http://www.cnblogs.com/grandyang/p/5138936.html)



参考资料：

[https://discuss.leetcode.com/topic/110475/c-solution-o-1-space-9ms](https://discuss.leetcode.com/topic/110475/c-solution-o-1-space-9ms)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












