# 什么是元编程(meta-promgramming)？ - Machine Learning with Peppa - CSDN博客





2018年05月24日 12:30:52[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：2018







**可以扩展程序自身，这样的能力，为元编程。**


比如Ruby，使用元编程可以扩展出领域专用语言DSL, 比如RSpec、puppet、chef等。相对于新创造的领域专用语言DSL来说， Ruby语言就成为其的元语言，所以叫做元编程 。





「我的电话是 +86 123 4567 8910」
  ——这是一条数据；
「+86 123 4567 8910 有十三个数字和一个字符，前两位是国家代码，后面是一个移动电话号码」   —— 这是关于前面那条数据的数据。
那么照猫画虎，怎样才算 meta-programming 呢？泛泛来说，只要是与编程相关的编程就算是 meta-programming 了——比如，若编程甲可以输出 A - Z，那么写程序甲算「编程」；而程序乙可以**生成**程序甲（也许还会连带着运行它输出 A - Z），那么编写程序乙的活动，就可以算作 meta-programming，「元编程」。注意，程序甲和程序乙并不一定是同一种语言：


```
('A'..'Z').each do |char|
    system("python -c 'print \"#{char}\"'")
end
```


如此说来，inline SQL 甚至动态生成 HTML 也是元编程了？抠定义的话是这样吧。元编程是用代码在编译期或运行期生成/改变代码。除非程序的运行期的输入数据会被直接或间接转化成代码，否则元编程不会给程序带来新的逻辑（信息论原理），此时它本质上是一种（让程序员自我感觉良好的）手工压缩源代码技术。
不过 metaprogramming 更狭义的意思应该是指「编写能改变语言语法特性或者运行时特性的程序」。换言之，一种语言本来做不到的事情，通过你编程来修改它，使得它可以做到了，这就是元编程。本版同文提及 method_missing，那么 [Wat —    Destroy All Software   Talks](https://link.zhihu.com/?target=https%3A//www.destroyallsoftware.com/talks/wat) 之中给出了运行时元编程的经典范例：


```
>> ruby has no bare words
NameError: undefined local variable or method `words' for main:Object
        from (irb) 1
>> def method_missing(*args); args.join(" "); end
=> nil
>> ruby has bare words
=> "ruby has bare words"
>> bare words can even have bangs!
=> "bare words can even have bangs!"
```

C、C++、Python、JavaScript…… 多数流行的语言或多或少都有元编程能力；Lisp 诸方言更是以元编程为基本。而 Ruby 更是因为元编程易用又强大，被许多人拿来写 DSL，因为元编程可以捏出「本不存在的语法特性」来让书写 DSL 变得简单。





