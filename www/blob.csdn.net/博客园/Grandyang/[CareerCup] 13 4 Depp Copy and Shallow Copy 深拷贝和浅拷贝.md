# [CareerCup] 13.4 Depp Copy and Shallow Copy 深拷贝和浅拷贝 - Grandyang - 博客园







# [[CareerCup] 13.4 Depp Copy and Shallow Copy 深拷贝和浅拷贝](https://www.cnblogs.com/grandyang/p/4924758.html)







13.4 What is the difference between deep copy and shallow copy? Explain how you would use each.



这道题问深拷贝和浅拷贝的区别。浅拷贝复制对象中所有的成员值到另一个对象中，而深拷贝不仅复制这些，还复制所有的指针对象，参见下面代码：

```
struct Test {
    char *ptr;
};

void shallow_copy(Test &src, Test &dest) {
    dest.ptr = src.ptr;
}

void deep_copy(Test &src, Test &dest) {
    dest.ptr = (char*)malloc(strlen(src.ptr) + 1);
    strcpy(dest.ptr, src.ptr);
}
```



浅拷贝有时会引起许多的运行错误，尤其是在创建和删除对象的时候。在使用浅拷贝的时候务必要小心，必须要完全明白程序在做什么。通常我们在传递复杂结构的信息时不需要赋值实际数据时在使用浅拷贝，必须对析构浅拷贝的对象格外注意。

实际应用中，浅拷贝并不经常使用。深拷贝应该是大多是情况下使用的，尤其是当要拷贝的结构体不大时。












