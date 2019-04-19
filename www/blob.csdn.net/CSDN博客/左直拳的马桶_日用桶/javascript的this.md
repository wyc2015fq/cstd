# javascript的this - 左直拳的马桶_日用桶 - CSDN博客
2014年12月10日 19:45:14[左直拳](https://me.csdn.net/leftfist)阅读数：1546
个人分类：[node.js																[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)](https://blog.csdn.net/leftfist/article/category/2626561)
如果是面向对象语言，this在类里面，我们都很清楚，那是代表类实例自己。
而在javascript里面呢，this代表什么？
javascript中，this都存在于函数内部。因此，在javascript里面，有一个总的原则，这个this，指的是调用该函数的对象。
所以，this代表什么这个问题，就转化成究竟是谁在调用该函数的问题。所谓的跟运行时上下文有关。
阮一峰的文章里，列举了四种情况。可以总结为
1、普通调用，全局调用，就是没有指明是谁调用，那么默认为全局对象在调用
```java
function test(){
　　　　this.x = 1;
　　　　alert(this.x);
}
test(); // 1
```
这时的this，就是全局对象，在前端，应该是window。而在后端，未可知也。因为同样的代码（除了alert 改成 console.log），放在node.js运行和在浏览器运行，结果可能是不一样的。这点要特别注意。node.js，似乎并没有一个所谓的全局对象。
2、明确指定对象调用，包括apply和call指定的对象
那么this很明显，就是这个对象。
比如：
```java
function test(){
　　　　alert(this.x);
　　}
　　var o = {};
　　o.x = 1;
　　o.m = test;
　　o.m(); // 1
```
apply或call的时候，也许没有指明对象，那么默认就是全局对象。比如
```java
var x = 0;
　　function test(){
　　　　alert(this.x);
　　}
　　var o={};
　　o.x = 1;
　　o.m = test;
　　o.m.apply(); //0，默认全局对象
```
阮一峰在解说闭包一文中，在文章末尾出了一道思考题：
[http://www.ruanyifeng.com/blog/2009/08/learning_javascript_closures.html](http://www.ruanyifeng.com/blog/2009/08/learning_javascript_closures.html)
代码片段一
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/41849471#)[copy](http://blog.csdn.net/leftfist/article/details/41849471#)
- var name = "The Window";  
- var object = {  
- 　　　　name : "My Object",  
- 　　　　getNameFunc : function(){  
- returnfunction(){  
- returnthis.name;  
- 　　　　　　};  
- 　　　　}  
- 　　};  
- 　　alert(object.getNameFunc()());//运行结果：The Window；如果在node.js中运行，则显示undefine
代码片段二
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/41849471#)[copy](http://blog.csdn.net/leftfist/article/details/41849471#)
- var name = "The Window";  
- var object = {  
- 　　　　name : "My Object",  
- 　　　　getNameFunc : function(){  
- var that = this;  
- returnfunction(){  
- return that.name;  
- 　　　　　　};  
- 　　　　}  
- 　　};  
- 　　alert(object.getNameFunc()());//运行结果：My Object；node.js运行结果一样
两段代码差别细微，结果却大相径庭。这究竟是为什么？
阮一峰的那篇文章并没有给出答案。文后的评论，众说纷纭。有的说代码片段二之所以是"My Object"，关键在于那句 var that = this;这是在编译的时候就确定了，所以that只能是object。这种解释，让人惊叹。
其实并没有这么复杂。
既然this跟运行时调用对象有关，那么重点不在于它如何定义，而要看被调用之处：
alert(object.getNameFunc()());
可以看到，alert()里面其实是分为2个步骤：
1、获得函数 var fn = object.getNameFunc();，调用对象是object，this指向object
2、执行函数 fn();，调用对象是全局对象，这时this指向全局变量
具体分析：
代码片段一：
步骤1，获得的函数是：
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/41849471#)[copy](http://blog.csdn.net/leftfist/article/details/41849471#)
- var fn = function(){  
- returnthis.name;  
- };  
然后在步骤二， fn() 为全局对象所调用，层层传递之下，this最后指向全局对象，所以会显示“The Window"
代码片段二
步骤1，获得的函数是：
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/41849985#)[copy](http://blog.csdn.net/leftfist/article/details/41849985#)
- var fn = function(){  
- return that.name;  
- };  
如上面所分析，在步骤1的时候，this 指向 object；这时 var that = this; 因此 that 指向object。于是
尽管在步骤2，fn()也为全局对象所调用，但 显示的是that.name，即 “My Object”。在这里，that属于函数（object.getNameFunc()）内部变量。而闭包的作用之一，就是让外界可以访问函数内部变量，因此这里的这个例子，确实是涉及到了闭包。
同时通过这个例子，也让我们更加深刻地领会到：this指的是函数的调用对象。
参考文章;
[http://www.ruanyifeng.com/blog/2010/04/using_this_keyword_in_javascript.html](http://www.ruanyifeng.com/blog/2010/04/using_this_keyword_in_javascript.html)
