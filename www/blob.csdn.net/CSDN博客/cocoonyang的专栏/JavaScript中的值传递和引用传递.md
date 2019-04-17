# JavaScript中的值传递和引用传递 - cocoonyang的专栏 - CSDN博客





2013年11月09日 19:35:32[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：1035








翻译自 http://snook.ca/archives/javascript/javascript_pass  

（水平有限，如有错误，责任在我）




In JavaScript, we have functions and we have arguments that we pass into those functions. But how JavaScript handles what you’re passing in is not always clear. When you start getting into object-oriented development, you may find yourself perplexed over
 why you have access to values sometimes but not other times.

在JavaScript中，我们会遇到函数，也会向这些函数传递一些参数。但你不一定很清楚JavaScript是如何处理你传递进去的那些参数。当你开始用面向对象方法开发时，这些时而可用，时而不可用的参数会让你晕头转向。





When passing in a primitive type variable like a string or a number, the value is passed in by value. This means that any changes to that variable while in the function are completely separate from anything that happens outside the function. Let’s take a
 look at the following example:

如果参数是基本数据类型，如字符串或数字，是通过值传递方式传递参数。也就是说在函数体内部对这些参数所做的改动不会影响到函数体以外的环境。下面就看一个例子：



```java
function myfunction(x)
{
      // x 等于 4
      x = 5;
      // x 现在等于 5
}

var x = 4;
alert(x); // x 等于 4
myfunction(x); 
alert(x); // x 还是等于 4
```


Passing in an object, however, passes it in by reference. In this case, any property of that object is accessible within the function. Let’s take a look at another example:



然而，如果进来的参数是一个对象，那么它是通过引用的方式传递的。这种情况下，在函数体内可操作这个对象的所有属性。我们来看另外一个例子：



```java
function myobject()
{
	this.value = 5;
}
var o = new myobject();
alert(o.value); // o.value = 5
function objectchanger(fnc)
{
	fnc.value = 6;
}
objectchanger(o);
alert(o.value); // o.value is now equal to 6
```


So, what happens when you pass in a method of an object? Most would expect (or at least I did) that it would be passed by reference allowing the method to access other parts of the object it is apart of. Unfortunately, that’s not the case. Check out this example:



如果我们传递一个对象的方法，会有什么结果呢？多数人（至少作者本人）会以为会传递进来一个引用，我们可以通过这个引用操作其所属对象的其它方法和属性。不幸的是，事情不是我们想象的那样。请看这个例子：



```java
function myobject()
{
	this.value = 5;
}
myobject.prototype.add = function()
{
	this.value++;
}
var o = new myobject();
alert(o.value); // o.value = 5
o.add();
alert(o.value); // o.value = 6
function objectchanger(fnc)
{
	fnc(); // 运行传递进来的方法
}
objectchanger(o.add);
alert(o.value); // 仍然是 6
```


The problem here is the use of the ‘this’ keyword. It’s a handy short-hand for referring to the current object context. When passing a function as a parameter, though, the context is lost. More accurately, this now refers to the context of the object making
 the call instead of the object’s function we just passed in. For standalone functions, this would be the window object and for functions called from an event, this would be the event object.



问题的在于使用了关键字‘this’。它指向当前对象所处环境。当一个方法被当作参数传递时，它的环境会丢失。确切的说，‘this’指向的不是我们传递的对象方法的环境，而是调用这个方法的对象的环境。在独立函数中，‘this’指向窗口对象；在事件中，‘this’指向事件对象。




Solving the problem  解决方案



There are two possible ways to get around this. 有两种解决方案


Option 1: When you know the method  方案1： 当你了解这个方法时


If you know the method of the object that will be called then it’s fairly easy. Just pass in the object instead of the function and call that instead. Using the objectchanger from the last example you’d get the following:


如果你清楚你要用某个对象的某个方法，直接传递这个对象而不是传递那个方法，然后通过这个对象调用方法。我们可以用这种对象替代的方式重写上一个objectchanger 例子：



```java
function objectchanger(obj)
{
	obj.add(); // runs the method of the object being passed in
}
objectchanger(o);
alert(o.value); // 现在是 7
```


Option 2: When you don’t know the method  方案1： 当你不了解这个方法时


If you don’t know the method of the object being passed in then you need to pass both the method and the object as parameters and use the call method. call is part of the JavaScript specification and allows a function to run in the context of another object.
 As a result, the this keyword will reference the right object: the object we passed in.

如果你不清楚你要用某个对象的某个方法，那不仅需要传递方法，而且也要把这个方法所属的那个对象一起传递进来，然后通过这个对象用call方法调用传递进来的方法。call是JavaScript 的特性，它可让一个方法在其它对象的环境中运行。最终，关键字this会指向正确的对象，也就是我们传递进来那个对象。

Here’s our objectchanger function one more time:

这里是我们 objectchanger 方法的另一个版本：



```java
function objectchanger(fnc, obj)
{
	fnc.call(obj); // runs the method of the object being passed in
}
objectchanger(o.add, o);
alert(o.value); // 现在是 7
```




























