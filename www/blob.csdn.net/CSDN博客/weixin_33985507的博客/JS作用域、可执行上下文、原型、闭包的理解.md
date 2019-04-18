# JS作用域、可执行上下文、原型、闭包的理解 - weixin_33985507的博客 - CSDN博客
2017年03月14日 15:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
```
1、区分 函数调用栈 和 作用域链 
2、JS代码执行分：
        a) 编译阶段：语法、词法解析等，确定作用域（非作用域链）
        b) 执行阶段：生成可执行上下文
3、函数执行即激活就生成可执行上下文，可执行上下文的生成又分为2个阶段：
       a) 创建阶段：
            i. 生成VO变量对象
                1. arguments: […]
                2. funcName:函数引用地址（变量提升）
                3. varName: undefined（变量提升）
                4. …
            ii. 生成作用域链[[scope]]：[ VO1， VO2，…..，VO _golbal ]作用域链是一系列VO对象
            iii. 确定this指向
        b) 执行阶段：具体的代码执行（此时，VO变成AO，里面的变量也会被赋值，undefined的会被赋值）
4、浏览器不关闭，全局上下文就一直存在
```
参考：
[http://www.jianshu.com/p/181da2b57eb2](https://www.jianshu.com/p/181da2b57eb2)
[http://www.jianshu.com/p/76ed896bbf91](https://www.jianshu.com/p/76ed896bbf91)
##### 作用域链和原型链的区别：
[https://www.zhihu.com/question/38963988](https://link.jianshu.com?t=https://www.zhihu.com/question/38963988)
- 作用域链（scope chain）是用来 **解析标识符** （identifier resolution）的，它是词法作用域（lexical scope）的一种实现方式。
- 原型链（prototype chain）是用来 **查找对象属性** （property in an object）的，它是原型继承（prototype-based inheritance）的一种实现方式。
##### 原型的理解：
所有对象都有原型对象。
> 
js中每个对象都包含一个指向其原型对象的指针。在构造函数中就是prototype属性，可以直接访问；普通对象中就是 _proto_ 内部属性，不可直接访问。
js中的对象就是一个指向其prototype的指针和一个属性列表。
类似于其他语言中的静态变量和方法，js中为了维持一份公有的属性和方法，避免每个实例对象都生成一份，将其注册到对应的原型对象prototype上即可。
新建一个构造函数，会有一个prototype属性指向其原型对象，原型对象中又有一个constructor属性指向构造函数，生成的实例对象中又有一个内部属性*proto*指向原型对象。
hasOwnProperty检测一个属性是否存在于实例属性中。
##### 闭包的理解：
闭包的作用：创建块级作用域，一般常见于创建类的时候。
函数中创建另一个函数，就形成了一个闭包。
下面这篇文章，可以知道为什么外部函数执行完毕后，内部函数还是可以访问外部函数的变量：因为内部函数在创建作用域链的时候，其作用域链中还存在着对外部函数变量对象的引用。js才有引用计数的方式进行内存回收，所以外部函数执行完毕，仅仅是其上下文执行环境销毁了，其变量对象还存在，所以内部函数可以访问到外部函数的变量。
参考：[http://www.jianshu.com/p/76950dd12f78](https://www.jianshu.com/p/76950dd12f78)
```
function outer(){
    //......
    return function(){
        //......
   };
}
var inner = outer();
inner();
```
