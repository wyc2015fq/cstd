# Js作用域 - LC900730的博客 - CSDN博客
2017年09月11日 17:42:28[lc900730](https://me.csdn.net/LC900730)阅读数：133标签：[javascript](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[js](https://blog.csdn.net/LC900730/article/category/7133535)
### 作用域Scope
作用域决定了变量的可见范围和生命周期。 
JS作用域不是花括号包围的块级作用域。
## 函数作用域
作用域是通过函数来定义的，在一个函数中定义的变量只对这个函数内部可见，我们称为函数作用域。 
在函数中引用一个变量时，JS会先搜索当前函数作用域，或者称为‘局部作用域’。如果没有搜索到上层作用域，则一直到全局作用域。
```java
var scope='global';
var f=function(){
    console.log(scope);
    var scope='f';
}
f();    //undefined
```
为什么输出undefined？ 
安装块级作用域搜索顺序，在console.log函数访问scope变量时，Js会先搜索函数f作用域，在f作用域中搜索到scope变量，所以上层作用域中定义的scope被屏蔽了，但是执行console.log语句时，scope还没有被定义或初始化，所以得到的就是undefined。
## 函数作用域的嵌套
函数作用域的嵌套关系是定义时候决定的，而不是调用时候决定的。Js的作用域是静态作用域，又叫词法作用域，这个因为作用域的嵌套关系可以在语法分析时确定，而不必等到运行时候在确定。
```java
var scope='top';
var f1=function(){
    console.log(scope);
};
f1();
var f2=function(){
    var scope='f2';
    f1();
};
f2(); //输出top
```
在f2调用的f1在查找scope定义的时候，找到的是父级作用域中定义的scope变量，而不是f2中定义的scope变量。这说明了作用域中的嵌套关系不是在调用的时候确定的，而是在定义时确定的。
## 闭包
closure是函数式编程中的概念。闭包的严格定义是“由函数及其封闭的自由变量组成的集合体。” 
通常上来讲，JS中的每一个函数都是一个闭包，但是通常意义上嵌套的函数更能够体现出闭包的特性。
```
var generateClosure=function(){
    var count=0;
    var get=function(){
        count++;
        return count;
    };
    return get;
};
var counter=generateClosure();
console.log(counter()); //输出1
console.log(counter()); //输出2
console.log(counter()); //输出3
```
genetareClosure()函数中有一个局部变量count，初始值是0；还有一个get函数，get将其父作用域，也就是genetareClosure()函数的count变量+1，并返回count值。 
generateClosure()的返回值是get函数。
闭包：闭包不但包含被返回的函数，还包括了这个函数的定义环境。当函数generateClosure()的内部函数get被一个外部变量的counter引用时候，counter和generateClosure()的局部变量就是一个闭包。
理解：
```
var generateClosure=function(){
    var count=0;
    var get=function(){
        count++;
        return count;
    };
    return get;
};
var counter1=generateClosure();
var counter2=generateClosure();
console.log(counter1()); //输出1
console.log(counter2()); //输出1
console.log(counter1()); //输出2
console.log(counter1()); //输出3
console.log(counter()); //输出2
```
counter1和counter2分别调用generateClosure()函数，生成了两个闭包的实例，它们内部引用的count变量分别属于不同的运行环境。
