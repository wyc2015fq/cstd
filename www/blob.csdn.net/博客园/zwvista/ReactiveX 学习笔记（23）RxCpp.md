# ReactiveX 学习笔记（23）RxCpp - zwvista - 博客园

## [ReactiveX 学习笔记（23）RxCpp](https://www.cnblogs.com/zwvista/p/9589298.html)

### RxCpp

RxCpp 是 ReactiveX 的 C++ 语言实现。

下载 RxCpp
`$ git clone --recursive https://github.com/ReactiveX/RxCpp.git`
之后，再将 RxCpp/Rx/v2/src 加入 include 文件夹中。

```
#include "rxcpp/rx.hpp"
namespace Rx {
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::operators;
    using namespace rxcpp::util;
}
using namespace Rx;
```

### Creating

```
auto ints = observable<>::create<int>(
    [](subscriber<int> s){
        s.on_next(1);
        s.on_next(2);
        s.on_completed();
    });
ints.subscribe(
    [](int v){printf("OnNext: %d\n", v);},
    [](){printf("OnCompleted\n");});
/*
OnNext: 1
OnNext: 2
OnCompleted
*/
```

### Converting

```
std::array< int, 3 > a={{1, 2, 3}};
auto values1 = observable<>::iterate(a);
values1.subscribe(
    [](int v){printf("OnNext: %d\n", v);},
    [](){printf("OnCompleted\n");});
/*
OnNext: 1
OnNext: 2
OnNext: 3
OnCompleted
*/
```

### Combining

```
auto values = observable<>::range(1); // infinite (until overflow) stream of integers
auto s1 = values.
take(3).
map([](int prime) { return std::make_tuple("1:", prime);});
auto s2 = values.
take(3).
map([](int prime) { return std::make_tuple("2:", prime);});
s1.
concat(s2).
subscribe(apply_to(
    [](const char* s, int p) {
        printf("%s %d\n", s, p);
    }));
/*
1: 1
1: 2
1: 3
2: 1
2: 2
2: 3
*/
```

```
auto values = observable<>::range(1); // infinite (until overflow) stream of integers
auto s1 = values.
map([](int prime) { return std::make_tuple("1:", prime);});
auto s2 = values.
map([](int prime) { return std::make_tuple("2:", prime);});
s1.
merge(s2).
take(6).
as_blocking().
subscribe(apply_to(
    [](const char* s, int p) {
        printf("%s %d\n", s, p);
    }));
/*
1: 1
2: 1
1: 2
2: 2
1: 3
2: 3
*/
```


