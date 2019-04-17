# [LeetCode] Subdomain Visit Count 子域名访问量统计 - Grandyang - 博客园







# [[LeetCode] Subdomain Visit Count 子域名访问量统计](https://www.cnblogs.com/grandyang/p/9486061.html)








A website domain like "discuss.leetcode.com" consists of various subdomains. At the top level, we have "com", at the next level, we have "leetcode.com", and at the lowest level, "discuss.leetcode.com". When we visit a domain like "discuss.leetcode.com", we will also visit the parent domains "leetcode.com" and "com" implicitly.

Now, call a "count-paired domain" to be a count (representing the number of visits this domain received), followed by a space, followed by the address. An example of a count-paired domain might be "9001 discuss.leetcode.com".

We are given a list `cpdomains` of count-paired domains. We would like a list of count-paired domains, (in the same format as the input, and in any order), that explicitly counts the number of visits to each subdomain.
Example 1:
Input: 
["9001 discuss.leetcode.com"]
Output: 
["9001 discuss.leetcode.com", "9001 leetcode.com", "9001 com"]
Explanation: 
We only have one website domain: "discuss.leetcode.com". As discussed above, the subdomain "leetcode.com" and "com" will also be visited. So they will all be visited 9001 times.

Example 2:
Input: 
["900 google.mail.com", "50 yahoo.com", "1 intel.mail.com", "5 wiki.org"]
Output: 
["901 mail.com","50 yahoo.com","900 google.mail.com","5 wiki.org","5 org","1 intel.mail.com","951 com"]
Explanation: 
We will visit "google.mail.com" 900 times, "yahoo.com" 50 times, "intel.mail.com" once and "wiki.org" 5 times. For the subdomains, we will visit "mail.com" 900 + 1 = 901 times, "com" 900 + 50 + 1 = 951 times, and "org" 5 times.


Notes:
- The length of `cpdomains` will not exceed `100`. 
- The length of each domain name will not exceed `100`.
- Each address will have either 1 or 2 "." characters.
- The input count in any count-paired domain will not exceed `10000`.
- The answer output can be returned in any order.




这道题让我们统计子域名的访问量，所谓的子域名，就是一个完整的域名以点断开的，每个断开的地方到末尾之间的子字符串就是一个子域名，现在给了我们很多完整域名的访问量，让我们统计所有子域名的访问量，题目中给的例子很好的说明了问题。那么这种统计字符串出现个数的问题，我们应该不难想到需要用一个HashMap来建立字符串和其出现次数的映射。那么接下来要做的就是将每一个全域名提取出来，然后拆分成子域名。提取全域名操作不难，因为给的格式都是一样的，前面是数字，中间一个空格，后面是全域名。我们只需要找到空格的位置，前面的部分转为整型数cnt，后面的就是全域名了。取出全域名之后就要进行拆分成子域名了，我们可以进行遍历，每当找到小数点的位置时，将后面的子字符串的映射值增加cnt，以此类推直到拆完所有的子域名。注意之前的全域名的映射值别忘了也要加上cnt，最后的最后我们只要将HashMap中的映射对组成题目中要求返回的格式即可，参见代码如下：



解法一：

```
class Solution {
public:
    vector<string> subdomainVisits(vector<string>& cpdomains) {
        vector<string> res;
        unordered_map<string, int> subdomainCnt;
        for (string cpdomain : cpdomains) {
            int spaceIdx = cpdomain.find(" ");
            int cnt = stoi(cpdomain.substr(0, spaceIdx));
            string rem = cpdomain.substr(spaceIdx + 1);
            for (int i = 0; i < rem.size(); ++i) {
                if (rem[i] == '.') {
                    subdomainCnt[rem.substr(i + 1)] += cnt;
                }
            }
            subdomainCnt[rem] += cnt;
        }
        for (auto a : subdomainCnt) {
            res.push_back(to_string(a.second) + " " + a.first);
        }
        return res;
    }
};
```



下面这种解法和上面的基本相同，唯一改变的地方就是拆分子域名的时候，没用使用遍历的for循环，而是继续使用了find函数来查找下一个小数点的位置，参见代码如下：



解法二：

```
class Solution {
public:
    vector<string> subdomainVisits(vector<string>& cpdomains) {
        vector<string> res;
        unordered_map<string, int> subdomainCnt;
        for (string cpdomain : cpdomains) {
            int spaceIdx = cpdomain.find(" ");
            int cnt = stoi(cpdomain.substr(0, spaceIdx));
            while (spaceIdx != string::npos) {
                subdomainCnt[cpdomain.substr(spaceIdx + 1)] += cnt;
                spaceIdx = cpdomain.find('.', spaceIdx + 1);
            }
        }
        for (auto a : subdomainCnt) {
            res.push_back(to_string(a.second) + " " + a.first);
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/subdomain-visit-count/solution/](https://leetcode.com/problems/subdomain-visit-count/solution/)

[https://leetcode.com/problems/subdomain-visit-count/discuss/157942/C++-concise-solution](https://leetcode.com/problems/subdomain-visit-count/discuss/157942/C++-concise-solution)

[https://leetcode.com/problems/subdomain-visit-count/discuss/121738/C++JavaPython-Easy-Understood-Solution](https://leetcode.com/problems/subdomain-visit-count/discuss/121738/C++JavaPython-Easy-Understood-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












