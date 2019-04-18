# [C++ 11] Range-based For - Pangda NoSilly - CSDN博客

2017年05月24日 19:43:14[hepangda](https://me.csdn.net/hepangda)阅读数：181标签：[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C++](https://blog.csdn.net/hepangda/article/category/6946147)



## Range-based For 的一般形式

想要遍历C++的各个容器类型以及原生数组，除了使用下标以及迭代器访问之外，还可以使用C++11的一种新的for循环形式，Range-based For。其一般的形式：

```
for (declaration : expression) {
    statement;
}
```

以下是一个使用的例子：

```cpp
#include<iostream>

int main() {
    int a[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    for (auto it : a) {
        std::cout << it << ' ';
    }

    return 0;
}
```

打开编译器的”std=c++11”选项，可以得到输出
`1 2 3 4 5 6 7 8 9 10`
## 有关 Range-based For

### 输出顺序

在标准STL容器中，使用Range-based For得到的输出与使用其iterator得到的输出一致。虽然string并不是狭义上的STL容器，然而string在使用Range-based For时也是以iterator的顺序为顺序。 

比如以下这个例子：

```cpp
#include<iostream>
#include<string>
#include<vector>

int main() {
    std::string str("range-based for");
    std::vector<int> vec { 1, 3, 5, 7, 9, 2};

    std::cout << "对string使用iterator: ";
    for (auto it = str.begin(); it != str.end(); it++) {
        std::cout << *it;
    }
    std::cout << "\n对string使用Range-based for: " ;
    for (auto it : str) {
        std::cout << it;
    }
    std::cout << "\n对vector使用iterator: ";
    for (auto it = vec.begin(); it != vec.end(); it++) {
        std::cout << *it << ' ';
    }
    std::cout << "\n对vector使用Range-based for: ";
    for (auto it : vec) {
        std::cout << it << ' ';
    }
    std::cout << std::endl;

    return 0;
}
```

编译运行，得到输出结果：

```
对string使用iterator: range-based for
对string使用Range-based for: range-based for
对vector使用iterator: 1 3 5 7 9 2
对vector使用Range-based for: 1 3 5 7 9 2
```

### 使用引用来修改对象

需要注意的是，在默认的设计中，我们得到的是值的一个拷贝，直接修改是不影响原值的，如果我们想要依次修改对象，我们可以使用以下的形式：

```
for (auto &it : container) {
    statement;
}
```

比如，下面的程序将一个小写字符串转换为大写字符串：

```cpp
#include<iostream>
#include<string>
#include<cctype>
int main() {
    std::string str("rangebasedfor");

    for (auto &it : str) {
        it = toupper(it);
    }
    std::cout << str << std::endl;

    return 0;
}
```

运行结果：
`RANGEBASEDFOR`
### 容器的元素数量不应发生变化

在Range-based For循环中，不应当发生在容器中增加或者减少元素的情况，若发生了这种情况，可能并不会得到你所期望得到的结果，比如，对于以下例子：

```cpp
#include<iostream>
#include<vector>

int main() {
    std::vector<int> vec { 1, 3 };

    for (auto it : vec) {
        std::cout << it << ' ';
        vec.push_back(2);
    }

    return 0;
}
```

也许用户本期望得到的结果是：
`1 3 2 `
然而实际上，执行该程序，得到的输出为：
`1 0 `

