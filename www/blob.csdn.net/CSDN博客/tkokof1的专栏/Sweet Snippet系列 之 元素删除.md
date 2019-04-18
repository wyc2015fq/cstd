# Sweet Snippet系列 之 元素删除 - tkokof1的专栏 - CSDN博客

2014年04月08日 17:32:24[tkokof1](https://me.csdn.net/tkokof1)阅读数：991


1. 引子:

  平时代码总会遇到一些关于集合的操作，例如添加，排序等等，都可算作稀松平常，但是集合涉及的删除操作却一直有个大坑，我自己便跳进去过好几回，在此简单一记，以自警警人～

  2. 问题:

  删除整数容器中的偶数

  问题很简单，似乎一个简单的for循环就搞定了，就像这样：


```cpp
#include <vector>
#include <iostream>

using namespace std;

int main() {
    vector<int> vec = { 1, 2, 2, 3 };

　　auto isEven = [](int value) -> bool { 
　　    return (value & 1) == 0; 
　　};
　　
    auto output = [&vec]() {
        for (auto v : vec) {
            cout << v << " ";
        }
        cout << "\n";
    };

    for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
        if (isEven(*iter)) {
            vec.erase(iter);
        }
    }

    output();

    return 0;
}
```

代码虽易，可惜明眼人一看就知道问题所在了：erase操作会影响iterator的正常迭代！所以上面看似简洁的代码并不能产生正确的结果（就上面集合来说，我们会遗漏处理第三个元素，即2），我们必须采用另外的方法：

一种就是使用buffer（稍有些冗余）：

```cpp
// omit codes ...

    {
        vector<int> buffer;
        for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
            if (!isEven(*iter)) {
                buffer.push_back(*iter);
            }
        }
        vec.swap(buffer);
    }

    // omit codes ...
```

近来还看到一种使用goto的tricky 方案（当然，个人不是很喜欢）：

```cpp
// omit codes ...

 restart:
    for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
        if (isEven(*iter)) {
            vec.erase(iter);
            goto restart;
        }
    }
   
    // omit codes ...
```

总的来说，我最认可的还是下面这种写法（平心而论，确实不及for形式简洁）：

```cpp
// omit codes ...
 
  auto iter = vec.begin();
    while (iter != vec.end()) {
        if (isEven(*iter)) {
            iter = vec.erase(iter);
        }
        else {
            ++iter;
        }
    }  
    
   // omit codes ...
```

不知还有什么另外的方法，有兴趣的朋友可以告知一下 ：）

OK，That’s all ~

