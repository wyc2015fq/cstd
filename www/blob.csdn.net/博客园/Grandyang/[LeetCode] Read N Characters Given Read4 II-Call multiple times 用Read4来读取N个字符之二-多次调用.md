# [LeetCode] Read N Characters Given Read4 II - Call multiple times 用Read4来读取N个字符之二 - 多次调用 - Grandyang - 博客园







# [[LeetCode] Read N Characters Given Read4 II - Call multiple times 用Read4来读取N个字符之二 - 多次调用](https://www.cnblogs.com/grandyang/p/5181672.html)







The API: int read4(char *buf) reads 4 characters at a time from a file.

The return value is the actual number of characters read. For example, it returns 3 if there is only 3 characters left in the file.

By using the read4 API, implement the function int read(char *buf, int n) that reads n characters from the file.

Note:
The read function may be called multiple times. 



这道题是之前那道[Read N Characters Given Read4](http://www.cnblogs.com/grandyang/p/5174322.html)的拓展，那道题说read函数只能调用一次，而这道题说read函数可以调用多次，那么难度就增加了，为了更简单直观的说明问题，我们举个简单的例子吧，比如：

buf = "ab", [read(1),read(2)]，返回 ["a","b"]

那么第一次调用read(1)后，从buf中读出一个字符，那么就是第一个字符a，然后又调用了一个read(2)，想取出两个字符，但是buf中只剩一个b了，所以就把取出的结果就是b。再来看一个例子：

buf = "a", [read(0),read(1),read(2)]，返回 ["","a",""]

第一次调用read(0)，不取任何字符，返回空，第二次调用read(1)，取一个字符，buf中只有一个字符，取出为a，然后再调用read(2)，想取出两个字符，但是buf中没有字符了，所以取出为空。


但是这道题我不太懂的地方是明明函数返回的是int类型啊，为啥OJ的output都是vector<char>类的，然后我就在网上找了下面两种能通过OJ的解法，大概看了看，也是看的个一知半解，貌似是用两个变量readPos和writePos来记录读取和写的位置，i从0到n开始循环，如果此时读和写的位置相同，那么我们调用read4函数，将结果赋给writePos，把readPos置零，如果writePos为零的话，说明buf中没有东西了，返回当前的坐标i。然后我们用内置的buff变量的readPos位置覆盖输入字符串buf的i位置，如果完成遍历，返回n，参见代码如下：



解法一：

```
class Solution {
public:
    int read(char *buf, int n) {
        for (int i = 0; i < n; ++i) {
            if (readPos == writePos) {
                writePos = read4(buff);
                readPos = 0;
                if (writePos == 0) return i;
            }
            buf[i] = buff[readPos++];
        }
        return n;
    }
private:
    int readPos = 0, writePos = 0;
    char buff[4];
};
```



下面这种方法和上面的方法基本相同，稍稍改变了些解法，使得看起来更加简洁一些：



解法二：

```
class Solution {
public:
    int read(char *buf, int n) {
        int i = 0;
        while (i < n && (readPos < writePos || (readPos = 0) < (writePos = read4(buff))))
            buf[i++] = buff[readPos++];
        return i;
    }
    char buff[4];
    int readPos = 0, writePos = 0;
};
```



类似题目：

[Read N Characters Given Read4](http://www.cnblogs.com/grandyang/p/5174322.html)



参考资料：

[https://leetcode.com/discuss/21219/a-simple-java-code](https://leetcode.com/discuss/21219/a-simple-java-code)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












