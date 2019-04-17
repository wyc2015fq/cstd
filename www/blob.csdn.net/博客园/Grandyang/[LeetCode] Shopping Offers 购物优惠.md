# [LeetCode] Shopping Offers 购物优惠 - Grandyang - 博客园







# [[LeetCode] Shopping Offers 购物优惠](https://www.cnblogs.com/grandyang/p/7261663.html)







In LeetCode Store, there are some kinds of items to sell. Each item has a price.

However, there are some special offers, and a special offer consists of one or more different kinds of items with a sale price.

You are given the each item's price, a set of special offers, and the number we need to buy for each item. The job is to output the lowest price you have to pay for exactly certain items as given, where you could make optimal use of the special offers.

Each special offer is represented in the form of an array, the last number represents the price you need to pay for this special offer, other numbers represents how many specific items you could get if you buy this offer.

You could use any of special offers as many times as you want.

Example 1:
Input: [2,5], [[3,0,5],[1,2,10]], [3,2]
Output: 14
Explanation: 
There are two kinds of items, A and B. Their prices are $2 and $5 respectively. 
In special offer 1, you can pay $5 for 3A and 0B
In special offer 2, you can pay $10 for 1A and 2B. 
You need to buy 3A and 2B, so you may pay $10 for 1A and 2B (special offer #2), and $4 for 2A.



Example 2:
Input: [2,3,4], [[1,1,0,4],[2,2,1,9]], [1,2,1]
Output: 11
Explanation: 
The price of A is $2, and $3 for B, $4 for C. 
You may pay $4 for 1A and 1B, and $9 for 2A ,2B and 1C. 
You need to buy 1A ,2B and 1C, so you may pay $4 for 1A and 1B (special offer #1), and $3 for 1B, $4 for 1C. 
You cannot add more items, though only $9 for 2A ,2B and 1C.



Note:
- There are at most 6 kinds of items, 100 special offers.
- For each item, you need to buy at most 6 of them.
- You are not allowed to buy more items than you want, even if that would lower the overall price.



这道题说有一些商品，各自有不同的价格，然后给我们了一些优惠券，可以在优惠的价格买各种商品若干个，要求我们每个商品要买特定的个数，问我们使用优惠券能少花多少钱，注意优惠券可以重复使用，而且商品不能多买。那么我们可以先求出不使用任何商品需要花的钱数作为结果res的初始值，然后我们遍历每一个coupon，定义一个变量isValid表示当前coupon可以使用，然后遍历每一个商品，如果某个商品需要的个数小于coupon中提供的个数，说明当前coupon不可用，isValid标记为false。如果遍历完了发现isValid还为true的话，表明该coupon可用，我们可以更新结果res，对剩余的needs调用递归并且加上使用该coupon需要付的钱数。最后别忘了恢复needs的状态，参见代码如下：



解法一：

```
class Solution {
public:
    int shoppingOffers(vector<int>& price, vector<vector<int>>& special, vector<int>& needs) {
        int res = 0, n = price.size();
        for (int i = 0; i < n; ++i) {
            res += price[i] * needs[i];
        }
        for (auto offer : special) {
            bool isValid = true;
            for (int j = 0; j < n; ++j) {
                if (needs[j] - offer[j] < 0) isValid = false;
                needs[j] -= offer[j];
            }
            if (isValid) {
                res = min(res, shoppingOffers(price, special, needs) + offer.back());
            }
            for (int j = 0; j < n; ++j) {
                needs[j] += offer[j];
            }
        }
        return res;
    }
};
```



下面这种解法也是递归的写法，总的来说思路跟上面没有啥差别，应该不难理解，参见代码如下：



解法二：

```
class Solution {
public:
    int shoppingOffers(vector<int>& price, vector<vector<int>>& special, vector<int>& needs) {
        int res = inner_product(price.begin(), price.end(), needs.begin(), 0);
        for (auto offer : special) {
            vector<int> r = helper(offer, needs);
            if (r.empty()) continue;
            res = min(res, shoppingOffers(price, special, r) + offer.back());
        }
        return res;
    }
    vector<int> helper(vector<int>& offer, vector<int>& needs) {
        vector<int> r(needs.size(), 0);
        for (int i = 0; i < needs.size(); ++i) {
            if (offer[i] > needs[i]) return {};
            r[i] = needs[i] - offer[i];
        }
        return r;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/95201/c-solution](https://discuss.leetcode.com/topic/95201/c-solution)

[https://discuss.leetcode.com/topic/95200/simple-java-recursive-solution](https://discuss.leetcode.com/topic/95200/simple-java-recursive-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












