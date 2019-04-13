
# 380. Insert Delete GetRandom O(1) - OraYang的博客 - CSDN博客

2017年12月20日 14:16:08[OraYang](https://me.csdn.net/u010665216)阅读数：325所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 380. Insert Delete GetRandom O(1)
标签（空格分隔）： leetcode array medium
---题目

---Design a data structure that supports all following operations in

---average

---O(1)

---time.


---insert(val)

---: Inserts an item val to the set if not already present.

---remove(val)

---: Removes an item val from the set if present.

---getRandom

---: Returns a random element from current set of elements. Each element must have the

---same probability

---of being returned.


---Example:

---// Init an empty set.
RandomizedSet randomSet = new RandomizedSet();
// Inserts 1 to the set. Returns true as 1 was inserted successfully.
randomSet.insert(1);
// Returns false as 2 does not exist in the set.
randomSet.remove(2);
// Inserts 2 to the set, returns true. Set now contains [1,2].
randomSet.insert(2);
// getRandom should return either 1 or 2 randomly.
randomSet.getRandom();
// Removes 1 from the set, returns true. Set now contains [2].
randomSet.remove(1);
// 2 was already in the set, so return false.
randomSet.insert(2);
// Since 2 is the only number in the set, getRandom always return 2.
randomSet.getRandom();


---思路

---这类问题一开始会想到用set来解决，但是在返回随机数值时，由于set不支持[]操作符，所以没法达到O(1)时间。于是把set改成unordered_map,后来同样在做返回随机数值时遇到问题，因为随机返回数组下标，map没法直接输出。因此额外维护一个vector数组用来存储数值下标。但是vector数组在进行删除操作时需要注意删除的方法：

---首先根据map找到要删除元素val的下表index,然后在vector将nums[index]与末尾数组互换，最后使用pop方法删除，这样能保证O(1)的时间。

---代码

---class

---RandomizedSet

---{

---public:

---/** Initialize your data structure here. */

---RandomizedSet() {
    }

---/** Inserts a value to the set. Returns true if the set did not already contain the specified element. */

---bool insert(int

---val

---) {

---if

---(m.find(

---val

---) != m.end())

---return

---false

---;
        nums.emplace_back(

---val

---);
        m[

---val

---] = nums.size() -

---1

---;

---return

---true

---;
    }

---/** Removes a value from the set. Returns true if the set contained the specified element. */

---bool remove(int

---val

---) {

---if

---(m.find(

---val

---) == m.end())

---return

---false

---;
        int last = nums.back();
        m[last] = m[

---val

---];
        nums[m[

---val

---]] = last;
        nums.pop_back();
        m.erase(

---val

---);

---return

---true

---;
    }

---/** Get a random element from the set. */

---int getRandom() {

---return

---nums[rand() % nums.size()];
    }

---private

---:
    vector<int> nums;
    unordered_map<int, int> m;
};


