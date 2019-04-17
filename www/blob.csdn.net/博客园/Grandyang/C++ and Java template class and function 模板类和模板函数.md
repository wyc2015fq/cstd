# C++ and Java template class and function 模板类和模板函数 - Grandyang - 博客园







# [C++ and Java template class and function 模板类和模板函数](https://www.cnblogs.com/grandyang/p/4812922.html)







在C++和Java的泛式编程中，模板template的使用是必不可少的，但是Java中没有template关键字，所以两者的写法还是有些许区别的，请参见如下代码：



Java的模板

```
// Java
public class Cell<K, V> {
    private K _key;
    private V _value;
    public Cell(K k, V v) {
        _key = k;
        _value = v;
    }
    public boolean equivalent(Cell<K, V> c) {
        return equivalent(c.getKey());
    }
    public boolean equivalent(K k) {
        return key.equals(k);
    }
    public K getKey() { return _key; }
    public V getValue() { return _value; }
}
```



C++的模板

```
// C++
template<typename K, typename V>
class Cell{
public:
    Cell(K k, V v): _key(k), _value(v) {}
    bool equivalent(Cell *c) {
        return equivalent(c->getKey());
    }
    bool equivalent(K k) {
        return _key.equals(k);
    }
    K getKey() { return _key; }
    V getValue() { return _value; }

private:
    K _key;
    V _value;
};
```



由上面的实例我们可以看出C++和Java在使用模板template写法上的不同，对于同一个类的两种语言的实现的对比，我们也能观察到一些C++和Java的其他不同之处：

1. Java对于每个变量或函数分别定义public还是private，而C++是分区的

2. Java里不存在指针的概念，和C++的一大重点就是指针














