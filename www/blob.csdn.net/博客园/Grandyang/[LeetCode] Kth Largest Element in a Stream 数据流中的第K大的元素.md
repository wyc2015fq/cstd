# [LeetCode] Kth Largest Element in a Stream 数据流中的第K大的元素 - Grandyang - 博客园







# [[LeetCode] Kth Largest Element in a Stream 数据流中的第K大的元素](https://www.cnblogs.com/grandyang/p/9941357.html)







Design a class to find the kth largest element in a stream. Note that it is the kth largest element in the sorted order, not the kth distinct element.

Your `KthLargest` class will have a constructor which accepts an integer `k` and an integer array `nums`, which contains initial elements from the stream. For each call to the method `KthLargest.add`, return the element representing the kth largest element in the stream.

Example:
int k = 3;
int[] arr = [4,5,8,2];
KthLargest kthLargest = new KthLargest(3, arr);
kthLargest.add(3);   // returns 4
kthLargest.add(5);   // returns 5
kthLargest.add(10);  // returns 5
kthLargest.add(9);   // returns 8
kthLargest.add(4);   // returns 8

Note: 
You may assume that `nums`' length ≥ `k-1` and `k` ≥ 1.



这道题让我们在数据流中求第K大的元素，跟之前那道[Kth Largest Element in an Array](http://www.cnblogs.com/grandyang/p/4539757.html)很类似，但不同的是，那道题的数组是确定的，不会再增加元素，这样确定第K大的数字就比较简单。而这道题的数组是不断在变大的，所以每次第K大的数字都在不停的变化。那么我们其实只关心前K大个数字就可以了，所以我们可以使用一个最小堆来保存前K个数字，当再加入新数字后，最小堆会自动排序，然后把排序后的最小的那个数字去除，则堆中还是K个数字，返回的时候只需返回堆顶元素即可，参见代码如下：



解法一：

```
class KthLargest {
public:
    KthLargest(int k, vector<int> nums) {
        for (int num : nums) {
            q.push(num);
            if (q.size() > k) q.pop();
        }
        K = k;
    }
    
    int add(int val) {
        q.push(val);
        if (q.size() > K) q.pop();
        return q.top();
    }

private:
    priority_queue<int, vector<int>, greater<int>> q;
    int K;
};
```



我们也可以使用multiset来做，利用其可重复，且自动排序的功能，这样也可以达到最小堆的效果，参见代码如下：



解法二：

```
class KthLargest {
public:
    KthLargest(int k, vector<int> nums) {
        for (int num : nums) {
            st.insert(num);
            if (st.size() > k) st.erase(st.begin());
        }
        K = k;
    }
    
    int add(int val) {
        st.insert(val);
        if (st.size() > K) st.erase(st.begin());
        return *st.begin();
    }

private:
    multiset<int> st;
    int K;
};
```



类似题目：

[Kth Largest Element in an Array](http://www.cnblogs.com/grandyang/p/4539757.html)



参考资料：

[https://leetcode.com/problems/kth-largest-element-in-a-stream](https://leetcode.com/problems/kth-largest-element-in-a-stream)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












