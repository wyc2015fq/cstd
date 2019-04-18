# [C++] algorithm库内容学习（1） - Pangda NoSilly - CSDN博客

2018年03月06日 21:05:38[hepangda](https://me.csdn.net/hepangda)阅读数：102


# algorithm

## [C++ 11] std::all_of

**原型**:

```
template <class InputIterator, class UnaryPredicate>
  bool all_of (InputIterator first, InputIterator last, UnaryPredicate pred);
```

**功能**: 指定上下界内是否都符合某条件 
**用例**:

```cpp
#include<iostream>
#include<algorithm>
using namespace std;

int main(int argc, char *argv[])
{
    int numbers[] = { 1, 3, 5, 7, 9, 11 };

    if (all_of(begin(numbers), end(numbers), [](int i)
    {
        return i & 1;
    })) {
        cout << "所有的数都是奇数" << endl;
    } else {
        cout << "部分数字不是奇数" << endl;
    }

    return 0;
}
```

## [C++ 11] std::any_of

**原型**:

```
template <class InputIterator, class UnaryPredicate>
  bool any_of (InputIterator first, InputIterator last, UnaryPredicate pred);
```

**功能**: 测试指定上下界内是否有元素满足条件 
**用例**:

```cpp
#include<iostream>
#include<algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    int numbers[] = { 1, 3, 5, 7, 9, 11, 6 };

    if (any_of(begin(numbers), end(numbers), [](int i)
    {
        return i & 1;
    })) {
        cout << "数组中存在偶数" << endl;
    } else {
        cout << "数组中不存在偶数" << endl;
    }

    return 0;
}
```

## [C++ 11] std::none_of

**原型**:

```
template <class InputIterator, class UnaryPredicate>
  bool none_of (InputIterator first, InputIterator last, UnaryPredicate pred);
```

**功能**: 测试指定上下界内是否均不符合某一条件 
**用例**:

```cpp
#include<iostream>
#include<algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    int numbers[] = { 1, 3, 5, 7, 9, 11, 6 };

    if (none_of(begin(numbers), end(numbers), [](int i)
    {
        return i & 1;
    })) {
        cout << "数组中全是偶数" << endl;
    } else {
        cout << "数组中不全是偶数" << endl;
    }

    return 0;
}
```

## std::for_each

**原型**:

```
template <class InputIterator, class Function>
   Function for_each (InputIterator first, InputIterator last, Function fn);
```

**功能**: 以指定上下界的每一项为参数执行函数 
**用例**:

```cpp
#include<iostream>
#include<algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    int numbers[] = { 1, 3, 5, 7, 9, 11, 6 };

    for_each(begin(numbers), end(numbers), [](int &i)
    {
        cout << i << ' ';
    });
    return 0;
}
```

## std::find

**原型**:

```
template <class InputIterator, class T>
   InputIterator find (InputIterator first, InputIterator last, const T& val);
```

**功能**: 在指定上下界内寻找指定值，返回首个对应迭代器。若未找到则返回`last`。 
**用例**:

```cpp
#include<iostream>
#include<algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    int numbers[] = { 1, 3, 5, 7, 9, 11, 6 };
    auto result = find(begin(numbers), end(numbers), 5);

    if (result != end(numbers)) {
        cout << *result << endl;
    } else {
        cout << "未找到5" << endl;
    }
    return 0;
}
```

## std::find_if / [C++ 11] std::find_if_not

**原型**:

```
template <class InputIterator, class UnaryPredicate>
   InputIterator find_if (InputIterator first, InputIterator last, UnaryPredicate pred);

template <class InputIterator, class UnaryPredicate>
   InputIterator find_if_not (InputIterator first, InputIterator last, UnaryPredicate pred);
```

**功能**: 在指定上下界内寻找满足某一条件的值，返回对应迭代器。若未找到则返回`last`。 
**用例**:

```cpp
#include<iostream>
#include<algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    int numbers[] = { 1, 3, 5, 7, 8, 9, 11, 6 };
    auto r1 = find_if(begin(numbers), end(numbers), [](int i)
    {
        return !(i & 1);
    });

    auto r2 = find_if_not(begin(numbers), end(numbers), [](int i)
    {
        return i & 1;
    });

    if (r1 != end(numbers)) {
        cout << "找到的第一个偶数是: " << *r2 << endl;
    } else {
        cout << "未找到偶数" << endl;
    }
    return 0;
}
```

## std::search / std::find_end

**原型**:

```
template <class ForwardIterator1, class ForwardIterator2>
   ForwardIterator1 search (ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2);

template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
   ForwardIterator1 search (ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2,
                            BinaryPredicate pred);

template <class ForwardIterator1, class ForwardIterator2>
   ForwardIterator1 find_end (ForwardIterator1 first1, ForwardIterator1 last1,
                              ForwardIterator2 first2, ForwardIterator2 last2);

template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
   ForwardIterator1 find_end (ForwardIterator1 first1, ForwardIterator1 last1,
                              ForwardIterator2 first2, ForwardIterator2 last2,
                              BinaryPredicate pred);
```

**功能**: 在[first1, last1)区间内寻找第一个或最后一个与[first2, last2)匹配的子区间，若未找到则返回last1。 
**用例**:

```
#include<iostream>
#include<algorithm>

using namespace std;

bool equalsTo(int &a, int &b)
{
    return a == b;
}

int main(int argc, char *argv[])
{
    int seq[] = { 5, 6, 1, 2, 3, 1, 3, 2, 1, 2, 3, 5, 6, 4, 3, 2, 1, 2, 3 };
    int ar[] = { 1, 2, 3 };
    auto r1 = search(begin(seq), end(seq), begin(ar), end(ar));
    auto r2 = search(begin(seq), end(seq), begin(ar), end(ar), equalsTo);
    auto r3 = find_end(begin(seq), end(seq), begin(ar), end(ar));
    auto r4 = find_end(begin(seq), end(seq), begin(ar), end(ar), equalsTo);

    if (r1 != end(seq)) {
        cout << "序列第一次出现在 " << std::distance(begin(seq), r2) << " 号位置。" << endl;
    } else {
        cout << "未找到指定序列。" << endl;
    }

    if (r3 != end(seq)) {
        cout << "序列最后一次出现在 " << std::distance(begin(seq), r4) << " 号位置。" << endl;
    } else {
        cout << "未找到指定序列。" << endl;
    }
    return 0;
}
```

## std::search_n

**原型**:

```
template <class ForwardIterator, class Size, class T>
   ForwardIterator search_n (ForwardIterator first, ForwardIterator last,
                             Size count, const T& val);

template <class ForwardIterator, class Size, class T, class BinaryPredicate>
   ForwardIterator search_n ( ForwardIterator first, ForwardIterator last,
                              Size count, const T& val, BinaryPredicate pred );
```

**功能**: 在[first, last)区间内寻找第一个连续n个等于指定值（或符合某条件）的位置，若未找到返回`last`。 
**用例**:

```cpp
#include<iostream>
#include<algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    int seq[] = { 5, 6, 1, 1, 2, 3, 1, 1, 3, 2, 1, 1, 1, 2, 3, 5, 6, 1, 4, 3, 2, 1, 2, 3 };
    auto r1 = search_n(begin(seq), end(seq), 3, 1);
    auto r2 = search_n(begin(seq), end(seq), 3, 1, [] (int a, int b)
    {
        return a == b;
    });

    if (r1 != end(seq)) {
        cout << "连续的3个1第一次出现在 " << std::distance(begin(seq), r2) << " 号位置。" << endl;
    } else {
        cout << "序列中没有连续的3个1。" << endl;
    }

    return 0;
}
```

## std::find_first_of

**原型**:

```
template <class InputIterator, class ForwardIterator>
   InputIterator find_first_of (InputIterator first1, InputIterator last1,
                                   ForwardIterator first2, ForwardIterator last2);

template <class InputIterator, class ForwardIterator, class BinaryPredicate>
   InputIterator find_first_of (InputIterator first1, InputIterator last1,
                                   ForwardIterator first2, ForwardIterator last2,
                                   BinaryPredicate pred);
```

**功能**: 在[first1, last1)区间中寻找第一个在[first2, last2)出现（或符合某种条件）的值，返回它的迭代器。 
**用例**:

```cpp
#include<iostream>
#include<algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    int seq[] = { 5, 6, 1, 8, 2, 3 };
    int arr[] = { 3, 4, 2, 1 };
    auto r1 = find_first_of(begin(seq), end(seq), begin(arr), end(arr));
    auto r2 = find_first_of(begin(seq), end(seq), begin(arr), end(arr), [] (int a, int b)
    {
        return a + b > 10;
    });

    if (r1 != end(seq)) {
        cout << "第一个出现在arr中的seq值在 " << std::distance(begin(seq), r1) << " 号位置。" << endl;
    } else {
        cout << "序列中没有出现arr中的值。" << endl;
    }

    if (r2 != end(seq)) {
        cout << "第一个出现与arr中相加大于10的seq值在 " << std::distance(begin(seq), r2) << " 号位置。" << endl;
    } else {
        cout << "序列中没有出现与arr中相加大于10的值。" << endl;
    }
    return 0;
}
```

## std::adjacent_find

**原型**:

```
template <class ForwardIterator>
   ForwardIterator adjacent_find (ForwardIterator first, ForwardIterator last);

template <class ForwardIterator, class BinaryPredicate>
   ForwardIterator adjacent_find (ForwardIterator first, ForwardIterator last,
                                  BinaryPredicate pred);
```

**功能**:  在给定区间内寻找第一个连续两个元素相同（或符合某种条件）的元素，并返回它的迭代器。若未找到则返回`last`。 
**用例**:

```cpp
#include<iostream>
#include<algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    int seq[] = { 5, 1, 6, 1, 1, 8, 2, 3 };
    auto r1 = adjacent_find(begin(seq), end(seq));
    auto r2 = adjacent_find(begin(seq), end(seq), [] (int a, int b)
    {
        return a == b;
    });

    if (r1 != end(seq)) {
        cout << "第一个连续相同的值在 " << std::distance(begin(seq), r2) << " 号位置。" << endl;
    } else {
        cout << "序列中没有连续相同的值" << endl;
    }
    return 0;
}
```

