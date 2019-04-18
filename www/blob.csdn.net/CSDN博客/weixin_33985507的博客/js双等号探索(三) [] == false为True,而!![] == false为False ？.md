# js双等号探索(三): [] == false为True,而!![] == false为False ？ - weixin_33985507的博客 - CSDN博客
2018年09月28日 14:49:13[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
```java
[] == false; //为True
!![] == false; //为False
复制代码
```
## 一、`[] == false`为True
### 第一步 转成`[] == 0`
根据 **MDN Web 文档-比较操作符**:[developer.mozilla.org/zh-CN/docs/…](https://link.juejin.im?target=https%3A%2F%2Fdeveloper.mozilla.org%2Fzh-CN%2Fdocs%2FWeb%2FJavaScript%2FReference%2FOperators%2FComparison_Operators)
> 
如果其中一个操作数为布尔类型，那么布尔操作数如果为true，那么会转换为1，如果为false，会转换为整数0，即0。
所以`![]`为**false**为`0`.
```java
[] == 0
复制代码
```
### 第二步 转成`"" == 0`
根据 **MDN Web 文档-比较操作符**:[developer.mozilla.org/zh-CN/docs/…](https://link.juejin.im?target=https%3A%2F%2Fdeveloper.mozilla.org%2Fzh-CN%2Fdocs%2FWeb%2FJavaScript%2FReference%2FOperators%2FComparison_Operators)
> 
如果一个对象与数字或字符串相比较，JavaScript会尝试返回对象的默认值。操作符会尝试通过方法valueOf和toString将对象转换为其原始值（一个字符串或数字类型的值）。
所以`[].valueOf().toString()`为`""`
```java
"" == 0
复制代码
```
### 第三步 转成`0 == 0`
根据 **MDN Web 文档-比较操作符**:[developer.mozilla.org/zh-CN/docs/…](https://link.juejin.im?target=https%3A%2F%2Fdeveloper.mozilla.org%2Fzh-CN%2Fdocs%2FWeb%2FJavaScript%2FReference%2FOperators%2FComparison_Operators)
> 
当比较数字和字符串时，字符串会转换成数字值。 JavaScript 尝试将数字字面量转换为数字类型的值。
`Number("")`为`0`。
```java
0 == 0
复制代码
```
最后`0==0`位`True`,所以`[] == false`为`Ture`
## 二、`!![] == false`为False
### 第一步 先运行`!![]`
根据 **MDN Web 文档-运算符优先级**:[developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Operators/Operator_Precedence](https://link.juejin.im?target=https%3A%2F%2Fdeveloper.mozilla.org%2Fzh-CN%2Fdocs%2FWeb%2FJavaScript%2FReference%2FOperators%2FOperator_Precedence)
`!`的优先级为**16** ,`==`的优先级为**10**，`!`的优先级更高，所以先运行`!![]`
```java
!![]
复制代码
```
### 第二步 先运行`!false`
根据 **《Javascript高级程序设计》**这本书第44页中`逻辑非`的说明：
> 
如果操作数是一个对象，返回false
所以运行`!false`，为`True`
```java
!false //为True
复制代码
```
最后`true == false`为`False`,所以`!![] == false`为`False`
则`[] == false`为`True`而`!![] == false`为`False`。
#### 其他链接
> - [raoenhui.github.io/js/2018/09/…](https://link.juejin.im?target=https%3A%2F%2Fraoenhui.github.io%2Fjs%2F2018%2F09%2F22%2Fcompare1%2F)
- [raoenhui.github.io/js/2018/09/…](https://link.juejin.im?target=https%3A%2F%2Fraoenhui.github.io%2Fjs%2F2018%2F09%2F23%2Fcompare2%2F)
- [raoenhui.github.io/js/2018/09/…](https://link.juejin.im?target=https%3A%2F%2Fraoenhui.github.io%2Fjs%2F2018%2F09%2F28%2Fcompare3%2F)
Happy coding ..
