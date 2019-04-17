# [CareerCup] 10.3 Integer not Contain in the File 文件中不包含的数 - Grandyang - 博客园







# [[CareerCup] 10.3 Integer not Contain in the File 文件中不包含的数](https://www.cnblogs.com/grandyang/p/4855652.html)







10.3 Given an input file with four billion non-negative integers, provide an algorithm to generate an integer which is not contained in the file. Assume you have 1 GB of memory available for this task.
 FOLLOW UP
 What if you have only 10 MB of memory? Assume that all the values are distinct and we now have no more than one billion non-negative integers.



这道题给我们了一个很大很大的数据文件，里面都是有四十亿个非负整数，现在给了我们1GB的内存大小，让我们找到一个不包括在这个文件中的非负整数，我们需要用位向量Bit Vector来做，跟之前那道[5.8 Draw Horizonatal Line 画横线](http://www.cnblogs.com/grandyang/p/4752053.html)有些类似，题目中说数据文件总共有四十亿个数字，也就是232个，那么非负数就有231个，给了我们1GB内存，也就是八十亿位，我们可以把每个整数都映射到内存中的不同位，逻辑如下：

1. 建立一个四十亿位大小的位向量Bit Vector，位向量是一个使用整型数组来存储bool型（或其他数据类型）值的数组，每个整型或bool型占32位。

2. 初始化位向量为0.

3. 遍历所有数字，给每个数字对应的位设为1.

4. 遍历位向量，找到第一个为0的位，算出其对应的整数。

参见代码如下：



```
class Solution {
public:
    void findOpenNumber() {
        vector<unsigned char> v(INT_MAX / 8);
        ifstream in("input.txt");
        int d;
        if (in.is_open()) {
            while (in >> d) {
                v[d / 8] |= 1 << (d % 8);
            }
            in.close();
        } else cout << "Cannot open file!\n";
        for (int i = 0; i < v.size(); ++i) {
            for (int j = 0; j < 8; ++j) {
                if ((v[i] & (1 << j)) == 0) {
                    cout << i * 8 + j << endl;
                    return;
                }
            }
        }
    }
};
```



这道题有个Follow Up，是说只给了我们10MB的内存大小，问如何解题。那么既然内存有限，我们只能讲大数据拆分为许多小的块Block，比如说每个块大小为1000，所以块0为数字0到999，块1为数字1000到1999等等。这样我们只要先找到是哪个块少了数字，然后再在这个块中具体查找少了哪个数字。下面我们就要来看每个块大小设定为多少，给了我们10MB内存，也就是223个字节，由于一个整型占4个字节，所以最多能有221个元素，所以我们区间大小不能小于231/221=210个,又由于10MB内存，也就是223个字节，共有226位，所以每个块可表示的数字范围可以在210到226之间选，我们选取靠近中间的220作为数字范围，因为越靠近中间，任意时间就会有更少的内存被占用，代码如下：



```
class Solution {
public:
    void findOpenNumber() {
        int bitSize = 1048576; // 2^20 bits (2^17 bytes)
        int blockNum = 4096;
        vector<unsigned char> v(bitSize / 8);
        vector<int> b(blockNum);
        int starting = -1;
        ifstream in("input.txt");
        int d;
        if (in.is_open()) {
            while (in >> d) {
                ++b[d / (v.size() * 8)];
            }
            in.close();
        } else cout << "Cannot open file!\n";
        for (int i = 0; i < b.size(); ++i) {
            if (b[i] < v.size() * 8) {
                starting = i * v.size() * 8;
                break;
            }
        }
        ifstream in2("input.txt");
        if (in2.is_open()) {
            while (in2 >> d) {
                if (d >= starting && d < starting + v.size() * 8) {
                    v[(d - starting) / 8] |= 1 << ((d - starting) % 8);
                }
            }
            in2.close();
        }
        for (int i = 0; i < v.size(); ++i) {
            for (int j = 0; j < 8; ++j) {
                if ((v[i] & (1 << j)) == 0) {
                    cout << i * 8 + j + starting << endl;
                    return;
                }
            }
        }
    }
};
```














