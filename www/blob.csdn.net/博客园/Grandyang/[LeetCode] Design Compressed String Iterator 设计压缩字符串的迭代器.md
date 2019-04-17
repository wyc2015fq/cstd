# [LeetCode] Design Compressed String Iterator 设计压缩字符串的迭代器 - Grandyang - 博客园







# [[LeetCode] Design Compressed String Iterator 设计压缩字符串的迭代器](https://www.cnblogs.com/grandyang/p/7026999.html)







Design and implement a data structure for a compressed string iterator. It should support the following operations: `next` and `hasNext`.

The given compressed string will be in the form of each letter followed by a positive integer representing the number of this letter existing in the original uncompressed string.

`next()` - if the original string still has uncompressed characters, return the next letter; Otherwise return a white space.
`hasNext()` - Judge whether there is any letter needs to be uncompressed.

Note:
Please remember to RESET your class variables declared in StringIterator, as static/class variables are persisted across multiple test cases. Please see [here](https://leetcode.com/faq/#different-output) for more details.

Example:
StringIterator iterator = new StringIterator("L1e2t1C1o1d1e1");

iterator.next(); // return 'L'
iterator.next(); // return 'e'
iterator.next(); // return 'e'
iterator.next(); // return 't'
iterator.next(); // return 'C'
iterator.next(); // return 'o'
iterator.next(); // return 'd'
iterator.hasNext(); // return true
iterator.next(); // return 'e'
iterator.hasNext(); // return false
iterator.next(); // return ' '


这道题给了我们一个压缩字符串，就是每个字符后面跟上其出现的次数，这里就算只出现一次，后面还是要加上1，那么其实如果当字符串很好有连续字符的时候，压缩字符串反而要比原字符串长。不过这题的重点不在于压缩字符串本身，而是让我们设计一个压缩字符串的迭代器，那么实际上是要我们根据压缩字符串来输出原字符串中的所有字符。那么我们关键就是要取出每个字符和其出现的次数，每当调用一次next，次数减1，如果减到0了，我们就要取出下一个字符和其出现的次数。我们要用个私有变量s来保存原字符串，然后用个变量i来记录当前遍历到的位置，变量c为当前处理的字符，变量cnt为字符c的当前次数。变量i的初始化为0，指向第一个字符，我们在hasNext()函数中，现将s[i]存入c，然后i自增1，然后我们用while循环取出所有的数字，存入cnt中。在next()函数中，如果hasNext()返回true，那么cnt就自减1，返回c；如果hasNext()返回false，那么字节返回空字符。在hasNext()函数中首先判断cnt的值，如果大于0，直接返回true，参见代码如下：



解法一：

```
class StringIterator {
public:
    StringIterator(string compressedString) {
        s = compressedString;
        n = s.size();
        i = 0;
        cnt = 0;
        c = ' ';
    }
    
    char next() {
        if (hasNext()) {
            --cnt;
            return c;
        }
        return ' ';
    }
    
    bool hasNext() {
        if (cnt > 0) return true;
        if (i >= n) return false;
        c = s[i++];
        while (i < n && s[i] >= '0' && s[i] <= '9') {
            cnt = cnt * 10 + s[i++] - '0';
        }
        return true;
    }

private:
    string s;
    int n, i, cnt;
    char c;
};
```



我们可以用C++中的字符流类来处理字符串，写法非常的简洁，可以少定义一些变量，在hasNext()函数中，如果cnt为0了，那么我们用字符流类直接读出下一个字符和次数，然后看是否能读出大于0的次数来返回真假值，参见代码如下：



解法二：

```
class StringIterator {
public:
    StringIterator(string compressedString) {
        is = istringstream(compressedString);
        cnt = 0;
        c = ' ';
    }
    
    char next() {
        if (hasNext()) {
            --cnt;
            return c;
        }
        return ' ';
    }
    
    bool hasNext() {
        if (cnt == 0) {
            is >> c >> cnt;
        }
        return cnt > 0;
    }

private:
    istringstream is;
    int cnt;
    char c;
};
```



下面这种解法还是用字符流类，和上面方法不同的地方是，在构建函数中完成了所有字符和次数的拆分，然后字符和其次数组成一个pair，加入一个队列queue中，这样我们每次处理的时候就直接去queue中取值就行了，这样hasNext()函数就变的非常简洁，只需要判断队列queue是否为空即可，参见代码如下：



解法三：

```
class StringIterator {
public:
    StringIterator(string compressedString) {
        istringstream is(compressedString);
        int cnt = 0;
        char c = ' ';
        while (is >> c >> cnt) {
            q.push({c, cnt});
        }
    }
    
    char next() {
        if (hasNext()) {
            auto &t = q.front();
            if (--t.second == 0) q.pop();
            return t.first;
        }
        return ' ';
    }
    
    bool hasNext() {
        return !q.empty();
    }

private:
    queue<pair<char, int>> q;
};
```



参考资料：

[https://discuss.leetcode.com/topic/92098/java-concise-single-queue-solution](https://discuss.leetcode.com/topic/92098/java-concise-single-queue-solution)

[https://discuss.leetcode.com/topic/92159/short-solution-of-c-using-stringstream-python-using-re](https://discuss.leetcode.com/topic/92159/short-solution-of-c-using-stringstream-python-using-re)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












