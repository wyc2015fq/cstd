# LeetCode 287 Find the Duplicate Number - Likes的博客 - CSDN博客
2019年02月23日 11:30:33[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：28
个人分类：[算法](https://blog.csdn.net/songchuwang1868/article/category/7908138)
这道题目要求在含有n+1个数字的数组中找出唯一一个出现多次的数字，而这些数字的范围是1-n，要求不改变数组本身以及常数空间。
首先我想到的办法是用Bitmap，即一个整型的数字X来表示某个数字是否出现过，如果出现过，那么就在X中对应的位置1。但是这个前提即使n不能太大了，否则整型的数字存储不下。反正这道题用了long long也不行。
这道题目看了别人的解答，可以说非常巧妙了。因为数组的范围是1-n，那么我们可以把数字之间的转移关系变成一个有向图。比如[2,1,2]，那么2->2, 1->1，2->2。
这个例子太过简单，我们来举一个复杂一点的例子。此时数组为[3,1,3,4,2]。
|index|0|1|2|3|4|
|----|----|----|----|----|----|
|value|3|1|3|4|2|
index和value都代表唯一的LinkNode Id，或者直接理解成LinkNode 所在的内存地址。arr[0]=3，的含义就是是Node0的next节点为Node3，arr[1]=1的含义就是Node1的next节点为Node1......根据一个数组我们可以找到一个链表与之一一对应，这是本题最为核心的部分。剩下的工作其实就是在链表中找环。
首先第一个数字3指向下标3的数字4，
数字4指向下标4的数字2
数字2指向下标2的数字3
数字1指向下标1的数字1
这样变成一个有向图以后就是
![](https://img2018.cnblogs.com/blog/773873/201810/773873-20181006152534349-210305451.png)
从这里看出来3，4，2形成了一个环，而环的入口点是3，这个问题就转换成了找图入口点的问题，可以把图看成链表，找链表入口点的问题就成了[leetcode 142](https://leetcode.com/problems/linked-list-cycle-ii/) 了。利用快慢指针即可。
当然这里还有一个问题，看上图中，1是单独出来的，那么当我们初始化的时候，如果刚好碰到像1这种单独成环的怎么办？其实我们从数组第1个点进去即可，即下标为0的数字。因为题目说了数组中数字的范围是1-n的，所以0点处的值是不可能单独成环的，放心的从第一个数字开始找环即可。
|12345678910111213141516171819|`class``Solution {``public``:``    ``int``findDuplicate(vector<``int``>& nums) {``        ``int``n = (``int``)nums.size();``        ``// if(n == 1) return``        ``int``slow = nums[0];``        ``int``fast = nums[nums[0]];``        ``while``(slow != fast){``            ``slow = nums[slow];``            ``fast = nums[nums[fast]];``        ``}``        ``fast = 0;``        ``while``(slow != fast){``            ``slow = nums[slow];``            ``fast = nums[fast];``        ``}``        ``return``slow;``    ``}``};`|
