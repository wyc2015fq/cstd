# leetcode-215. Kth Largest Element in an Array 数组中的第K个最大元素 - 别说话写代码的博客 - CSDN博客





2019年02月20日 21:42:30[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：21标签：[Kth Largest Element in an Arra																[leetcode																[数组中的第K个最大元素](https://so.csdn.net/so/search/s.do?q=数组中的第K个最大元素&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|Find the **k**th largest element in an unsorted array. Note that it is the kth largest element in the sorted order, not the kth distinct element.**Example 1:****Input:**`[3,2,1,5,6,4] `and k = 2**Output:** 5**Example 2:****Input:**`[3,2,3,1,2,4,5,5,6] `and k = 4**Output:** 4**Note: **			You may assume k is always valid, 1 ≤ k ≤ array's length.|在未排序的数组中找到第 **k** 个最大的元素。请注意，你需要找的是数组排序后的第 k 个最大的元素，而不是第 k 个不同的元素。**示例 1:****输入:**`[3,2,1,5,6,4] 和` k = 2**输出:** 5**示例 2:****输入:**`[3,2,3,1,2,4,5,5,6] 和` k = 4**输出:** 4**说明:**你可以假设 k 总是有效的，且 1 ≤ k ≤ 数组的长度。|
|----|----|

思路：

**第一种**：排序，快排，冒泡， 选择，插入，归并等都可以。其实可以sort一把梭.

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        sort(nums.begin(),nums.end(),std::greater<int>());
        return nums[k-1];
    }
};
```

**第二种**：用快排的思路找到前k大，因为 快排选择一个枢轴能保证 左边都比数轴大，右边都比 枢轴小。这样只要枢轴元素是第k 个就可以，**时间复杂 度O(n)**

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int left = 0, right = nums.size() - 1;
        while (true) {
            int pos = partition(nums, left, right);
            if (pos == k - 1) return nums[pos];
            else if (pos > k - 1) right = pos - 1;
            else left = pos + 1;
        }
    }
    int partition(vector<int>& nums, int left, int right) {
        int pivot = nums[left], l = left + 1, r = right;
        while (l <= r) {
            if (nums[l] < pivot && nums[r] > pivot) {
                swap(nums[l++], nums[r--]);
            }
            if (nums[l] >= pivot) ++l;
            if (nums[r] <= pivot) --r;
        }
        swap(nums[left], nums[r]);
        return r;
    }
};
```

**第三种**：我们使用一个最小堆来保存k个 最大值，如果当前堆个数小于k, 如果某就直接加进去， 如果堆已经有k个元素，接下来的元素如果比最小堆堆顶的值大，那就将其换下就可以了。这种思路**适合海量数据，同时不会产生数据交换**。 时间复杂度为O(nlogk)。参见剑指offer

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        priority_queue<int,vector<int>,greater<int> > p; //建一个大小为k的最小堆
        for(int i=0;i<nums.size();++i)
        {
            if(p.size()<k) p.push(nums[i]);
            else
            {
                if(p.top()<nums[i])   //当前元素大于最小堆堆顶就刷新
                {
                    p.pop();
                    p.push(nums[i]);
                }
            }
        }
        return p.top();   //堆顶元素就是第k个需要的元素
    }
};
```

**第四种**：直接放进优先队列（也就是堆）。简单粗暴一把梭

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        priority_queue<int> p(nums.begin(),nums.end());
        for(int i=0;i<k-1;++i)
            p.pop();
        return p.top();
    }
};
```](https://so.csdn.net/so/search/s.do?q=Kth Largest Element in an Arra&t=blog)




