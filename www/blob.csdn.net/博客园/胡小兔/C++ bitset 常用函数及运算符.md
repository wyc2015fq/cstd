# C++ bitset 常用函数及运算符 - 胡小兔 - 博客园







# [C++ bitset 常用函数及运算符](https://www.cnblogs.com/RabbitHu/p/bitset.html)





# C++ bitset——高端压位卡常题必备STL

以下内容翻译自[cplusplus.com](http://www.cplusplus.com/)，极大地锻炼了我的英语能力。

bitset存储二进制数位。

bitset就像一个bool类型的数组一样，但是有空间优化——bitset中的一个元素一般只占1 bit，相当于一个char元素所占空间的八分之一。

bitset中的每个元素都能单独被访问，例如对于一个叫做foo的bitset，表达式foo[3]访问了它的第4个元素，就像数组一样。

bitset有一个特性：整数类型和布尔数组都能转化成bitset。

bitset的大小在编译时就需要确定。如果你想要不确定长度的bitset，请使用（奇葩的）vector<bool>。

## 定义一个bitset

```
// constructing bitsets
#include <iostream>       // std::cout
#include <string>         // std::string
#include <bitset>         // std::bitset

int main ()
{
  std::bitset<16> foo;
  std::bitset<16> bar (0xfa2);
  std::bitset<16> baz (std::string("0101111001"));

  std::cout << "foo: " << foo << '\n';
  std::cout << "bar: " << bar << '\n';
  std::cout << "baz: " << baz << '\n';

  return 0;
}
```

输出结果：

```
foo: 0000000000000000
bar: 0000111110100010
baz: 0000000101111001
```

## bitset的运算

bitset的运算就像一个普通的整数一样，可以进行与(&)、或(|)、异或(^)、左移(<<)、右移(>>)等操作。

```
// bitset operators
#include <iostream>       // std::cout
#include <string>         // std::string
#include <bitset>         // std::bitset

int main ()
{
  std::bitset<4> foo (std::string("1001"));
  std::bitset<4> bar (std::string("0011"));

  std::cout << (foo^=bar) << '\n';       // 1010 (XOR,assign)
  std::cout << (foo&=bar) << '\n';       // 0010 (AND,assign)
  std::cout << (foo|=bar) << '\n';       // 0011 (OR,assign)

  std::cout << (foo<<=2) << '\n';        // 1100 (SHL,assign)
  std::cout << (foo>>=1) << '\n';        // 0110 (SHR,assign)

  std::cout << (~bar) << '\n';           // 1100 (NOT)
  std::cout << (bar<<1) << '\n';         // 0110 (SHL)
  std::cout << (bar>>1) << '\n';         // 0001 (SHR)

  std::cout << (foo==bar) << '\n';       // false (0110==0011)
  std::cout << (foo!=bar) << '\n';       // true  (0110!=0011)

  std::cout << (foo&bar) << '\n';        // 0010
  std::cout << (foo|bar) << '\n';        // 0111
  std::cout << (foo^bar) << '\n';        // 0101

  return 0;
}
```

上面代码的输出结果见注释。（注意，这段代码涉及赋值操作）

## bitset的相关函数

对于一个叫做foo的bitset：
`foo.size()` 返回大小（位数）
`foo.count()` 返回1的个数
`foo.any()` 返回是否有1
`foo.none()` 返回是否没有1
`foo.set()` 全都变成1
`foo.set(p)` 将第p + 1位变成1
`foo.set(p, x)` 将第p + 1位变成x
`foo.reset()` 全都变成0
`foo.reset(p)` 将第p + 1位变成0
`foo.flip()` 全都取反
`foo.flip(p)` 将第p + 1位取反
`foo.to_ulong()` 返回它转换为unsigned long的结果，如果超出范围则报错
`foo.to_ullong()` 返回它转换为unsigned long long的结果，如果超出范围则报错
`foo.to_string()` 返回它转换为string的结果












