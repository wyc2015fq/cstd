# [LeetCode] IP to CIDR 将IP地址转为CIDR无类别域间路由 - Grandyang - 博客园







# [[LeetCode] IP to CIDR 将IP地址转为CIDR无类别域间路由](https://www.cnblogs.com/grandyang/p/8440087.html)







Given a start IP address `ip` and a number of ips we need to cover `n`, return a representation of the range as a list (of smallest possible length) of CIDR blocks.

A CIDR block is a string consisting of an IP, followed by a slash, and then the prefix length. For example: "123.45.67.89/20". That prefix length "20" represents the number of common prefix bits in the specified range.

**Example 1:**

**Input:** ip = "255.0.0.7", n = 10
**Output:** ["255.0.0.7/32","255.0.0.8/29","255.0.0.16/32"]
**Explanation:**
The initial ip address, when converted to binary, looks like this (spaces added for clarity):
255.0.0.7 -> 11111111 00000000 00000000 00000111
The address "255.0.0.7/32" specifies all addresses with a common prefix of 32 bits to the given address,
ie. just this one address.

The address "255.0.0.8/29" specifies all addresses with a common prefix of 29 bits to the given address:
255.0.0.8 -> 11111111 00000000 00000000 00001000
Addresses with common prefix of 29 bits are:
11111111 00000000 00000000 00001000
11111111 00000000 00000000 00001001
11111111 00000000 00000000 00001010
11111111 00000000 00000000 00001011
11111111 00000000 00000000 00001100
11111111 00000000 00000000 00001101
11111111 00000000 00000000 00001110
11111111 00000000 00000000 00001111

The address "255.0.0.16/32" specifies all addresses with a common prefix of 32 bits to the given address,
ie. just 11111111 00000000 00000000 00010000.

In total, the answer specifies the range of 10 ips starting with the address 255.0.0.7 .

There were other representations, such as:
["255.0.0.7/32","255.0.0.8/30", "255.0.0.12/30", "255.0.0.16/32"],
but our answer was the shortest possible.

Also note that a representation beginning with say, "255.0.0.7/30" would be incorrect,
because it includes addresses like 255.0.0.4 = 11111111 00000000 00000000 00000100 
that are outside the specified range.

**Note:**

- `ip` will be a valid IPv4 address.
- Every implied address `ip + x` (for `x < n`) will be a valid IPv4 address.
- `n` will be an integer in the range `[1, 1000]`.



这道题博主刚开始做的时候，看了半天，读不懂题目的意思，结果一看是一道Easy的题，直接？？？尼克杨问号脸？？？后来通过研究论坛上大家的解法，才总算明白了这道题让我们做什么。此题给了我们一个用字符串表示的ip地址，还有一个整数n，让我们以给定的ip地址为起点，需要覆盖n个ip地址。而这n个ip地址的写法使用无类别域间路由CIDR块来写，所谓的CIDR块，是由一个正常的ip地址，加上斜杠数字，斜杠后面的数字表示这些ip地址具有相同的前缀的个数，比如"255.0.0.7/32"，如果有32个相同的前缀，说明只有唯一的一个ip地址，因为IPv4总共就只有32位。再比如"255.0.0.8/29"，表示有29个相同的前缀，那么最后3位可以自由发挥，2的3次方为8，所以就共有8个ip地址。同理，"255.0.0.16/32"只表示一个地址，那么这三个CIDR块总共覆盖了10个地址，就是我们要求的结果。

由于题目中要求尽可能少的使用CIDR块，那么在n确定的情况下，CIDR块能覆盖的越多越好。根据我们前面的分析，当CIDR块斜杠后面的数字越小，该块覆盖的ip地址越多。那么就是说相同前缀的个数越少越好，但是我们的ip地址又不能小于给定的ip地址，所以我们只能将0变为1，而不能将1变为0。所以我们的选择就是有将最低位1后面的0进行变换，比如"255.0.0.8"末尾有3个0，可以变换出8个不同的地址。那么我们只要找出末尾1的位置，就知道能覆盖多少个地址了。找末尾1有个trick，就是利用 x & -x 来快速找到，这个trick在之前做的题中也有应用。知道了最多能覆盖地址的数量，还要考虑到n的大小，不能超过n，因为题目只要求覆盖n个。确定了覆盖的个数，我们就可以进行生成CIDR块的操作了，之前我们为了求 x & -x，将ip地址转为了一个十进制的数，现在我们要把每一块拆分出来，直接按对应位数量进行右移并与上255即可，斜杠后的数字计算通过覆盖的个数进行log2运算，再被32减去即可，参见代码如下： 



```
class Solution {
public:
    vector<string> ipToCIDR(string ip, int n) {
        vector<string> res;
        long x = 0;
        istringstream is(ip);
        string t;
        while (getline(is, t, '.')) {
            x = x * 256 + stoi(t);
        }
        while (n > 0) {
            long step = x & -x;
            while (step > n) step /= 2;
            res.push_back(convert(x, step));
            x += step;
            n -= step;
        }
        return res;
    }
    string convert(long x, int step) {
        return to_string((x >> 24) & 255) + "." + to_string((x >> 16) & 255) + "." + to_string((x >> 8) & 255) + "." + to_string(x & 255) + "/" + to_string(32 - (int)log2(step));
    }
};
```



类似题目：

[Validate IP Address](http://www.cnblogs.com/grandyang/p/6185339.html)

[Restore IP Addresses](http://www.cnblogs.com/grandyang/p/4305572.html)



参考资料：

[https://discuss.leetcode.com/topic/115127/10-lines-java-solution](https://discuss.leetcode.com/topic/115127/10-lines-java-solution)

[https://discuss.leetcode.com/topic/114848/very-simple-java-solution-beat-100](https://discuss.leetcode.com/topic/114848/very-simple-java-solution-beat-100)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












