
# Java assert在什么时候使用 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月31日 06:46:06[seven-soft](https://me.csdn.net/softn)阅读数：267


断言是一个包含布尔表达式的语句，在执行这个语句时假定该表达式为 true。如果表达式计算为 false，那么系统会报告一个 AssertionError。它用于调试目的：
assert(a > 0); // throws an AssertionError if a <= 0
断言可以有两种形式：assert Expression1 ;
assert Expression1 : Expression2 ;
Expression1 应该总是产生一个布尔值。Expression2 可以是得出一个值的任意表达式。这个值用于生成显示更多调试信息的 String 消息。
断言在默认情况下是禁用的。要在编译时启用断言，需要使用 source 1.4 标记：
javac -source 1.4 Test.java
要在运行时启用断言，可使用 -enableassertions 或者 -ea 标记。
要在运行时选择禁用断言，可使用 -da 或者 -disableassertions 标记。
要系统类中启用断言，可使用 -esa 或者 -dsa 标记。还可以在包的基础上启用或者禁用断言。
可以在预计正常情况下不会到达的任何位置上放置断言。断言可以用于验证传递给私有方法的参数。不过，断言不应该用于验证传递给公有方法的参数，因为不管是否启用了断言，公有方法都必须检查其参数。不过，既可以在公有方法中，也可以在非公有方法中利用断言测试后置条件。另外，断言不应该以任何方式改变程序的状态。

