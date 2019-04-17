# [CareerCup] 8.10 Implement a Hash Table 实现一个哈希表 - Grandyang - 博客园







# [[CareerCup] 8.10 Implement a Hash Table 实现一个哈希表](https://www.cnblogs.com/grandyang/p/4812018.html)







8.10 Design and implement a hash table which uses chaining (linked lists) to handle collisions.



这道题让我们实现一个简单的哈希表，我们采用了最简单的那种取余映射的方式来实现，我们使用Cell来保存一对对的key和value的映射关系，然后每一个格子都用一个list链表来保存所有的余数为该格子序号的Cell，我们设定格子总数为10，然后我们用泛式编程来适用于所有的参数类型，然后实现哈希表的基本存数和取数的功能。现在大多数的哈希表都是用二叉搜索树来实现的，但是用BST的话取数就是不是O(1)的时间复杂度了(如果我们以后很多的collision的话)，但是BST的好处就是省空间，不需要建立超大的数组来保存映射。



```
template<typename K, typename V>
class Cell{
public:
    Cell(K k, V v): _key(k), _value(v) {}
    bool equivalent(Cell *c) {
        return equivalent(c->getKey());
    }
    bool equivalent(K k) {
        return _key == k;
    }
    K getKey() { return _key; }
    V getValue() { return _value; }

private:
    K _key;
    V _value;
};

template<typename K, typename V>
class Hash {
public:
    Hash() {
        _items.resize(_MAX_SIZE);
    }
    int hashCodeOfKey(K key) {
        return sizeof(key).size() % _items.size();
    }
    void put(K key, V value) {
        int x = hashCodeOfKey(key);
        if (_items[x] == nullptr) {
            _items[x] = new list<Cell<K, V>*> ();
        }
        list<Cell<K, V>*> *collided = _items[x];
        for (auto a : *collided) {
            if (a->equivalent(key)) {
                collided->remove(a);
                break;
            }
        }
        Cell<K, V> *cell = new Cell<K, V>(key, value);
        collided->push_back(cell);
    }
    V get(K key) {
        V v;
        int x = hashCodeOfKey(key);
        if (_items[x] == nullptr) {
            return v;
        }
        list<Cell<K, V>*> *collided = _items[x];
        for (auto a : *collided) {
            if (a->equivalent(key)) {
                return a->getValue();
            }
        }
        return v;
    }

private:
    const int _MAX_SIZE = 10;
    vector<list<Cell<K, V>*>*> _items;
};
```














