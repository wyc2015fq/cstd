# [CareerCup] 10.4 Find All Duplicates Elements 寻找所有的重复项 - Grandyang - 博客园







# [[CareerCup] 10.4 Find All Duplicates Elements 寻找所有的重复项](https://www.cnblogs.com/grandyang/p/4860445.html)







10.4 You have an array with all the numbers from 1 to N, where N is at most 32,000. The array may have duplicate entries and you do not know what N is. With only 4 kilobytes of memory available, how would you print all duplicate elements in the array?



这道题给了我们很多在区间[1, 32000]中的数，让我们只用4KB的内存大小来找出所有的重复项。跟之前那道题很类似[10.3 Integer not Contain in the File 文件中不包含的数](http://www.cnblogs.com/grandyang/p/4855652.html)，还是需要用位向量Bit Vector来解，4KB内存共有215个位Bit，大于32000个数，所以我们可以用每个bit来表示一个数，我们需要一个位向量类BitSet，来建立和数字之间的映射，有点像哈希表的功能，是一个整型数组，由于int型最大可以表示232，所以一个位置就可以映射232个数字，参见代码如下：



```
class BitSet {
public:
    vector<int> _bitset;
    BitSet(int size) {
        _bitset.resize((size >> 5) + 1);
    }
    bool get(int pos) {
        int wordNum = (pos >> 5);
        int bitNum = (pos % 32);
        return (_bitset[wordNum] & (1 << bitNum)) != 0;
    }
    void set(int pos) {
        int wordNum = (pos >> 5);
        int bitNum = (pos % 32);
        _bitset[wordNum] |= 1 << bitNum;
    }
};

class Solution {
public:
    void checkDuplicates(vector<int> array) {
        BitSet *bs = new BitSet(32000);
        for (int i = 0; i < array.size(); ++i) {
            int num = array[i];
            int num0 = num - 1;
            if (bs->get(num0)) {
                cout << num << endl;
            } else {
                bs->set(num0);
            }
        }
    }
};
```














