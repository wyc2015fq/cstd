# Groovy闭包深入学习 - z69183787的专栏 - CSDN博客
2015年01月05日 19:10:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：611
个人分类：[Groovy-概述](https://blog.csdn.net/z69183787/article/category/2816515)
**闭包**
**1. 定义和执行闭包**
Java代码  ![收藏代码](http://lggege.iteye.com/images/icon_star.png)
- def one = { num1, num2 ->  
-     println "param is: $num1 & $num2"
- }  
- one(2, 3)       // 简便写法。输出 param is: 2 & 3
- one 2, 3// 省略()的等效写法
- 
- one.call(2, 3) // 使用call方法
- one.call 2, 3// 省略()等效写法
**注意：**
- a) 闭包自身的定义写法。在参数与具体代码执行端间的分隔符是->，老版本的是|
- b) 使用call方法，或简便写法。
- c) 由于groovy可省略()，而引发的众多等效写法。
- d) 单参数，可省略书写参数，在闭包内使用it变量引用参数。
- 
**2. 闭包作为参数返回**
Java代码  ![收藏代码](http://lggege.iteye.com/images/icon_star.png)
- def makeClosure(name) {  
- return {  
-         println "Hello ${name}"
-     }  
- }  
- 
- println makeClosure(‘World’) // 请问输出结果? 
**3. 闭包作为参数传递**
Java代码  ![收藏代码](http://lggege.iteye.com/images/icon_star.png)
- def run(closure) {  
-     closure.call()  
- }  
- 
- one = { println 'Hello, World!' }  
- 
- run(one)  
**4. 闭包使用外部变量**
Java代码  ![收藏代码](http://lggege.iteye.com/images/icon_star.png)
- class OneClosure {  
-     def static execute(closure) {  
-         def word = 'Cool'// !!! 注意不使用def的输出结果，理解方式使用引用
-         closure('Grails')  
-     }  
- 
- publicstaticvoid main(args) {  
-         def word = 'Hello'
- 
-         def two = {param -> println "${word} ${param}"}  
-         two('Groovy') // 输出 Hello Groovy
- 
-         word = 'Wow'
-         two('Java') // 输出 Wow Java
- 
-         OneClosure.execute(two) // 输出 Wow Grails，而不是Cool Grails
-     }  
- }  
注意： 
- a) 闭包可使用（引用）闭包外部定义的变量
- b) 变量的定义必须在闭包的上面，否则有groovy.lang.MissingPropertyException异常。
- c) 注意在代码标记出，如果不使用def的输出差异。具体解释可使用引用来理解。在一个闭包被定义后，使用的是闭包定义所在的外部对象，对于使用的外部对象的引用一直都不会改变（无论是否被作为参数传递）。
- 
**5. 使用闭包实现单方法接口**
Java代码  ![收藏代码](http://lggege.iteye.com/images/icon_star.png)
- interface Test {  
-     def one()  
- }  
- 
- def test = {println 'one'} as Test  
- 
- test.one()  
- a) 使用关键字as
**6. 使用闭包实现多方法接口**
Java代码  ![收藏代码](http://lggege.iteye.com/images/icon_star.png)
- interface Test {  
-     def one()  
-     def two()  
- }  
- 
- def test = [  
-     one: {println 'one'},  
-     two: {println 'two'}  
-     ] as Test  
- 
- test.one()  
- test.two()  
- a) 使用关键字as
- b) 使用Map, key为接口名,value为闭包
