# [LeetCode] Range Module 范围模块 - Grandyang - 博客园







# [[LeetCode] Range Module 范围模块](https://www.cnblogs.com/grandyang/p/8586531.html)







A Range Module is a module that tracks ranges of numbers. Your task is to design and implement the following interfaces in an efficient manner.


- `addRange(int left, int right)` Adds the half-open interval `[left, right)`, tracking every real number in that interval. Adding an interval that partially overlaps with currently tracked numbers should add any numbers in the interval `[left, right)` that are not already tracked.




- `queryRange(int left, int right)` Returns true if and only if every real number in the interval `[left, right)` is currently being tracked.




- `removeRange(int left, int right)` Stops tracking every real number currently being tracked in the interval `[left, right)`.



Example 1:
addRange(10, 20): null
removeRange(14, 16): null
queryRange(10, 14): true (Every number in [10, 14) is being tracked)
queryRange(13, 15): false (Numbers like 14, 14.03, 14.17 in [13, 15) are not being tracked)
queryRange(16, 17): true (The number 16 in [16, 17) is still being tracked, despite the remove operation)



Note:
- A half open interval `[left, right)` denotes all real numbers `left <= x < right`.
- `0 < left < right < 10^9` in all calls to `addRange, queryRange, removeRange`.
- The total number of calls to `addRange` in a single test case is at most `1000`.
- The total number of calls to `queryRange` in a single test case is at most `5000`.
- The total number of calls to `removeRange` in a single test case is at most `1000`.



这道题让我们实现一个RangeModule的类，里面有三个功能函数，分别好似插入范围，查找范围，删除范围，题目中的例子给的也很明确，基本不会引起什么歧义。其实不管范围也好，区间也好，都是一回事，跟之前的区间的题目[Insert Interval](http://www.cnblogs.com/grandyang/p/4367569.html)和[Merge Intervals](http://www.cnblogs.com/grandyang/p/4370601.html)没有什么不同。这里的插入范围函数的实现方法跟之前那道[Insert Interval](http://www.cnblogs.com/grandyang/p/4367569.html)的解法一样，直接抄过来就好。然后对于查找范围函数，由于题目中说只要有数字未被包括，就返回false。那么我们反过来想，只有当某个范围完全覆盖了这个要查找的范围才会返回true，所以我们可以遍历所有的范围，然后看是否有一个范围完全覆盖了要查找的范围，有的话返回true，循环结束后返回false。最后来看删除范围函数，其实现方法跟插入范围函数很类似，但又有少许不同。首先我们还是新建一个数组res存结果，然后遍历已有的范围，如果当前范围的结束位置小于等于要删除的范围的起始位置，由于题目中的范围定义是左开右闭，那么说明没有重叠，加入结果res，并且用变量cur自增1来记录当前位置。如果当前范围的起始位置大于等于要删除的范围的结束位置，说明咩有重叠，加入结果res。否则就是有重叠的情况，这里跟插入范围有所不同的是，插入范围只需要加入一个范围，而删除范围操作有可能将原来的大范围break成为两个小的范围，所以我们用一个临时数组t来存break掉后的小范围。如果当前范围的起始位置小于要删除的范围的起始位置left，说明此时一定有一段范围留下来了，即从当前范围的起始位置到要删除的范围的起始位置left，将这段范围加入临时数组t，同理，如果当前范围的结束位置大于要删除的范围的结束位置right，将这段范围加入临时数组t。最后将数组t加入结果res中的cur位置即可，参见代码如下：



解法一：

```
class RangeModule {
public:
    RangeModule() {}
    
    void addRange(int left, int right) {
        vector<pair<int, int>> res;
        int n = v.size(), cur = 0;
        for (int i = 0; i < n; ++i) {
            if (v[i].second < left) {
                res.push_back(v[i]);
                ++cur;
            } else if (v[i].first > right) {
                res.push_back(v[i]);
            } else {
                left = min(left, v[i].first);
                right = max(right, v[i].second);
            }
        }
        res.insert(res.begin() + cur, {left, right});
        v = res;
    }
    
    bool queryRange(int left, int right) {
        for (auto a : v) {
            if (a.first <= left && a.second >= right) return true;
        }
        return false;
    }
    
    void removeRange(int left, int right) {
        vector<pair<int, int>> res, t;
        int n = v.size(), cur = 0;
        for (int i = 0; i < n; ++i) {
            if (v[i].second <= left) {
                res.push_back(v[i]);
                ++cur;
            } else if (v[i].first >= right) {
                res.push_back(v[i]);
            } else {
                if (v[i].first < left) {
                    t.push_back({v[i].first, left});
                }
                if (v[i].second > right) {
                    t.push_back({right, v[i].second});
                }
            }
        }
        res.insert(res.begin() + cur, t.begin(), t.end());
        v = res;
    }

private:
    vector<pair<int, int>> v;
};
```



下面来看一种优化了时间复杂度的解法，我们使用TreeMap来建立范围的起始位置和结束位置之间的映射，利用了TreeMap的自动排序功能，其会根据起始位置从小到大进行排序。既然是有序的，我们就可以利用二分法来快速进行查找了。

在加入范围函数中，我们首先用upper_bound函数来查找第一个大于left的位置，标记为l，再用upper_bound函数来查找第一个大于right的位置，标记为r。我们其实是想找第一个不大于left和right的位置的，由于C++没有floorKey这个函数，所以我们只能用upper_bound找大于left和right的位置，然后再往前移一个。如果l不是TreeMap中的第一个位置，且前面一个范围的结束位置小于left，说明和前一个范围没有交集，那么还是回到当前这个范围吧。如果此时l和r指向同一个位置，说明当前要加入的范围没有跟其他任何一个范围有交集，所以我们直接返回即可，不需要其他任何操作。否则的话我们将left和l指向范围的起始位置中的较小值赋给i，将right和r指向的前一个位置的结束位置的较大值赋给j，然后将l和r之间的范围都删除掉（注意这里r自增了1，是因为之前先自减了1），然后将i和j返回即可。返回后我们建立起这个映射即可。

在查找范围函数中，我们先用upper_bound找出第一个大于left位置的范围it，然后看如果it不是第一个范围，并且如果其前面的一个范围的结束位置大于等于right，说明已经完全包括这个要查找的范围，因为前一个范围的起始位置小于left，且结束位置大于等于right，直接返回true。

在删除范围函数中，查找重叠范围的方法跟加入范围函数中的操作一样，所以抽出来放到了find函数中，由于删除的范围有可能完全覆盖了原有范围，也有可能只是部分覆盖，将一个大的范围拆成了一个或者两个范围。所以我们判断，如果left大于覆盖范围的起始位置，那么将这段建立映射，同理，如果覆盖范围的结束位置大于right，同样建立这段的映射，参见代码如下：



解法二：

```
class RangeModule {
public:
    RangeModule() {}
    
    void addRange(int left, int right) {
        auto x = find(left, right);
        m[x.first] = x.second;
    }
    
    bool queryRange(int left, int right) {
        auto it = m.upper_bound(left);
        return it != m.begin() && (--it)->second >= right;
    }
    
    void removeRange(int left, int right) {
        auto x = find(left, right);
        if (left > x.first) m[x.first] = left;
        if (x.second > right) m[right] = x.second;
    }

private:
    map<int, int> m;
    
    pair<int, int> find(int left, int right) {
        auto l = m.upper_bound(left), r = m.upper_bound(right);
        if (l != m.begin() && (--l)->second < left) ++l;
        if (l == r) return {left, right};
        int i = min(left, l->first), j = max(right, (--r)->second);
        m.erase(l, ++r);
        return {i, j};
    }
};
```



类似题目：

[Data Stream as Disjoint Intervals](http://www.cnblogs.com/grandyang/p/5548284.html)

[Insert Interval](http://www.cnblogs.com/grandyang/p/4367569.html)

[Merge Intervals](http://www.cnblogs.com/grandyang/p/4370601.html)



参考资料：

[https://leetcode.com/problems/range-module/discuss/108914/c++-code](https://leetcode.com/problems/range-module/discuss/108914/c++-code)

[https://leetcode.com/problems/range-module/discuss/108912/C++-vector-O(n)-and-map-O(logn)-compare-two-solutions](https://leetcode.com/problems/range-module/discuss/108912/C++-vector-O(n)-and-map-O(logn)-compare-two-solutions)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












