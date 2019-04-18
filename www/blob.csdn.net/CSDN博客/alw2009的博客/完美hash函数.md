# 完美hash函数 - alw2009的博客 - CSDN博客





2019年01月08日 23:51:25[traveler_zero](https://me.csdn.net/alw2009)阅读数：72








之前的学习了Hash表的基础知识（[数据结构与算法（Hash表）](https://mp.csdn.net/postedit/85645182)）， 知道了Hash表的核心就是Hash函数，今天来学习完美hash函数。

一、什么是完美hash函数？

如果存在函数h(x)将集合U映射到集合S并且没有碰撞， 我们就可以说h(x)是集合U到集合S的完美hash函数。

二、如何实现一个完美hash函数

实现的思路不复杂， 但是有一个前提：在构建hash表之前， 我们要先获得所有可能的key。然用之前的方法（好像有些地方叫除留余数法）进行hash分组， 然后用同样的方法将落在不同槽的数据再做一次映射， 确保这次映射不再有碰撞。

![](https://img-blog.csdnimg.cn/20190108222002654.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)

 三、c++代码实例实现完美hash函数

```cpp
class Hash {
public:
    Hash(std::list<int> keys)
        :size_(keys.size())
        ,region_sizes(keys.size())
        ,offset_table(keys.size())
        ,a2_array(keys.size()){

        // 一级hash
        int sum = 10 * size_;
        int max_sum = 5 * size_;
        while (sum > max_sum) { // 获得分布相对均匀的hash
          a = rnd.rand();
          sum = 0;

          for (auto& v : region_sizes) {
              v = 0;
          }

          for (auto key : keys) {
              int h = key * a % MAXP % size_;
              ++region_sizes[h];
              sum += region_sizes[h] * region_sizes[h];
          }
        }

        // 二级hash
        sum *= 2;
        hash_table.resize(sum, MAXP);

        for (auto& v : region_sizes) {
            v *= 2;
        }
        // 计数个区间起始位置
        offset_table[0] = 0;
        for (int i = 1; i < size_; ++i) {
            offset_table[i] = offset_table[i-1] + region_sizes[i-1];
        }
        // 生成二级hash映射系数
        for (auto& v : a2_array) {
            v = rnd.rand();
        }

        // 检查是否有碰撞， 如果有碰撞，重新生成映射系数，直到没有碰撞为止
        bool isCollision = true;
        while (isCollision) {
            isCollision = false;

            for (auto key : keys) {
                int h1 = a * key % MAXP % size_;
                int h2 = a2_array[h1] * key % MAXP % region_sizes[h1] + offset_table[h1];

                if (hash_table[h2] == MAXP
                    || hash_table[h2] == key) {
                    hash_table[h2] = key;
                }
                else {
                    isCollision = true;
                    a2_array[h1] = 0; // 系数需要重新生成
                }
            }

            if (isCollision) {
                for (int i = 0; i < size_; ++i) {
                    if (a2_array[i] == 0) {
                        a2_array[i] = rnd.rand();

                        // 清除对应区的hash表
                        for (int j = offset_table[i]; j < offset_table[i] + region_sizes[i]; ++j) {
                            hash_table[j] = MAXP;
                        }
                    }
                }

            }
        }
    }

    int hash(int key) {
        int h1 = a * key % MAXP % size_;
        return a2_array[h1] * key % MAXP % region_sizes[h1] + offset_table[h1];
    }

private:
    int a; // 原始映射系数
    int size_; // 所以key的数量
    std::vector<int> region_sizes; // 一级hash计数及保存最终hash表的各区间长度
    std::vector<int> offset_table; // 各区间的起始位置
    std::vector<int> a2_array; // 各区间的映射系数
    std::vector<int> hash_table; // 最终hash表
    HashRand rnd; // 用于生成在区间[1,46337)的随机数
};
```

这个实现的内存有些浪费， 有待改进。



