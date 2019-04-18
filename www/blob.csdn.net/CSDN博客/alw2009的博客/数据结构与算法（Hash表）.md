# 数据结构与算法（Hash表） - alw2009的博客 - CSDN博客





2019年01月05日 23:29:08[traveler_zero](https://me.csdn.net/alw2009)阅读数：12








下面的内容有自己的理解， 未必正确， 欢迎探讨。

一、Hash是什么?

Hash是一个集合到另一集合的映射， 比如集合U = {'Alice', 'Bob', 'Carol', 'Dave'}存在一个函数h(x)使得：

h('Alice') = 0

h('Bob') = 1

h('Carol') = 2

h('Dave') = 3

令M={1,2,3,4}, 我们就可以说h(x)是U到M的哈希函数， 0， 1， 2， 3叫做Hash槽或者Hash桶

二、Hash表的原理

下面用一个例子说明， Hash表的工作原理。

假设我有好友U = {'Alice', 'Bob', 'Carol', 'Dave'}, 我现在想用一个数据结构保存他们的电话号码， 结合前面的Hash， 我们可以设计一个高效的数据结构， 让增删改查的时间复杂度都是O(1). C++代码实现如下：

```cpp
class PhoneNumberHashTable {
public:
    void insert(std::string name, std::string phoneNumber) {
        hashTable[hash(name)] = new std::string{phoneNumber};
    }

    void remove(std::string name) {
        delete hashTable[hash(name)];
    }

    void update(std::string name, std::string phoneNumber) {
        *hashTable[hash(name)] = phoneNumber;
    }

    std::string get_phone_number(std::string name) {
        return *hashTable[hash(name)];
    }

private:
    static const int HASH_SIZE = 26;
    std::string* hashTable[HASH_SIZE] = {};

    int hash(std::string key) {
        return key[0] - 'A';
    }
};
```

当然， 段示例代码有很多问题， 这里只是用来揭示hash表的原理。我们继续看一下代码， 如果我又有了一个新朋友Baby, 添加Baby号码会发生什么事情呢？ 对了， Bob的号码被冲掉了。 原因是Bob和Baby都被映射到同一个槽了， 这个现象叫做Hash碰撞。如何解决hash碰撞的问题呢？ Hash表允许不同的元素被映射到相同的Hash槽， 映射到相同Hash槽的这些元素可以用一个链表来存储。可以证明这时Hash表的平均时间复杂度仍然为O(1). 下面给了一个改进版本的C++实现：

```cpp
struct NamePhone {
    std::string name;
    std::string phone;
};

class PhoneNumberHashTable {
public:
    void insert(std::string name, std::string phoneNumber) {
        // 获取哈希槽的list
        auto& list = hashTable[hash(name)];

        // 如果list已经包含了这个名字， 只做更新， 否则插入
        auto it = std::find_if(std::begin(list), std::end(list),
                            [name](const NamePhone& np) {
                                return np.name == name;});
        if (it != std::end(list)) {
            it->phone = phoneNumber;
        }
        else {
            list.push_back({name, phoneNumber});
        }
    }

    void remove(std::string name) {
        // 获取哈希槽的list
        auto& list = hashTable[hash(name)];
        list.remove_if([name](const NamePhone& np) {return np.name == name;});
    }

    std::string find(std::string name) {
        auto& list = hashTable[hash(name)];
        auto it = std::find_if(std::begin(list), std::end(list),
                               [name](const NamePhone& np){return np.name == name;});
        return it->phone;
    }

private:
    static const int HASH_SIZE = 26;
    std::list<NamePhone> hashTable[HASH_SIZE] = {};

    int hash(std::string key) { return std::toupper(key[0]) - 'A'; }
};
```

可以看到改进之后的版本还需要存储key。



三、常见的Hash函数实现

现在已经知道， Hash表的核心就是Hash函数。可以尝试总结一下什么是好的Hash函数：

1. 碰撞越少越好。 如果存在一个函数能将集合U映射到集合S， 没有碰撞。我们就说这个hash函数是完美hash。

2. 在有碰撞的情况下， 碰撞越均匀越好。

3. hash函数的计算越快越好。

实现1：

```
U = {0, . . . , p − 1} p是一个质数
S = {0, . . . , s − 1} s ≤ p
ha: U → S | ha(x) = (a(x+b) mod p) mod s, 1 ≤ a ≤ p − 1
```

实现2：
`(A[x1,x2..xk] + b) mod p) mode s`
这种方式用于字符串的hash，下面用代码演示一下：

```
class StringHash {
private:
    static constexpr int MAXP = 46337;
    std::list<int> al;
    int b;
    int size_;

public:
    StringHash(int size)
        :b(rand()%MAXP), size_(size){
    }

    int hash(std::string s) {
        int sum = b;
        auto it = std::begin(al);
        for(auto c : s) {
            if (it == std::end(al)) {
                it = al.insert(it, rand() % MAXP);
            }
            sum += *it * c % MAXP;
            ++it;
        }
        return sum % size_;
    }
};
```

总结： 假设有n个元素， 哈希表的大小是s， 支持的操作是find, insert, delete, 那么用上面的方法，每个操作的时间复杂度是O(1 + n/s), 空间复杂度是O(n + s).





