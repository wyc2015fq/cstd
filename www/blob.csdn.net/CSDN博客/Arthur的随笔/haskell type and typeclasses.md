# haskell type and typeclasses - Arthur的随笔 - CSDN博客
2011年06月26日 00:40:00[largetalk](https://me.csdn.net/largetalk)阅读数：828标签：[haskell																[numbers																[behavior																[integer																[interface																[class](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=integer&t=blog)](https://so.csdn.net/so/search/s.do?q=behavior&t=blog)](https://so.csdn.net/so/search/s.do?q=numbers&t=blog)](https://so.csdn.net/so/search/s.do?q=haskell&t=blog)
个人分类：[haskell](https://blog.csdn.net/largetalk/article/category/837038)
use :t to show expression's type
common types: 
  Int, bounded, in 32-bit system, between -2147483648 and 2147483648
  Integer, like Int, not bounded
  Float
  Double
  Bool
  Char
typeclasses: typeclass is a sort of interface hant defines some behavior, if a type is a part of a typeclass, that means that it support and implements the behavior the typeclass describes. so we can think the typeclass is kind of as java interface
common typeclasses:
Eq, if there's an Eq class constraint for a type variable in a function, it uses == or /= somewhere inside its definition. why said it uses but not have capability? because in haskell, no variable, no object, so 只有这个函数使用了== or /=，才会说这个函数类型有Eq这个class constraint
Ord: ordering, > < >=, <=
Show: presented as string
Read: opposite typeclass of Show
Enum: members are sequentially ordered types
Bounded: members have an upper and lower bound
Num: is a numeric typeclass
Integral: is also a numeric typeclass. Num includes all numbers including real numbers and integral numbers, Integral includes only integral(whole) numbers.
Floating: includes only floating poing number
