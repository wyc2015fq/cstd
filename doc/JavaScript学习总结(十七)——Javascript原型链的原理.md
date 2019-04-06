## [JavaScript学习总结(十七)——Javascript原型链的原理](https://www.cnblogs.com/xdp-gacl/p/3704722.html)

## 一、JavaScript原型链

　　ECMAScript中描述了原型链的概念，并将原型链作为实现继承的主要方法。其基本思想是利用原型让一个引用类型继承另一个引用类型的属性和方法。在JavaScript中，用 `__proto__` 属性来表示一个对象的原型链。当查找一个对象的属性时，JavaScript 会**向上**遍历原型链，直到找到给定名称的属性为止！

比如现在有如下的代码：

**扩展Object类，添加Clone和Extend方法**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 /*扩展Object类，添加Clone，JS实现克隆的方法*/
 2 Object.prototype.Clone = function(){
 3     var objClone;
 4     if (this.constructor == Object){
 5         objClone = new this.constructor(); 
 6     }else{
 7         objClone = new this.constructor(this.valueOf()); 
 8     }
 9     for(var key in this){
10         if ( objClone[key] != this[key] ){ 
11             if ( typeof(this[key]) == 'object' ){ 
12                 objClone[key] = this[key].Clone();
13             }else{
14                 objClone[key] = this[key];
15             }
16         }
17     }
18     objClone.toString = this.toString;
19     objClone.valueOf = this.valueOf;
20     return objClone; 
21 }
22 
23 /*扩展Object类，添加Extend方法来实现JS继承, 目标对象将拥有源对象的所有属性和方法*/
24 Object.prototype.Extend = function (objDestination, objSource) {
25     for (var key in objSource) {
26         if (objSource.hasOwnProperty(key) && objDestination[key] === undefined) {
27             objDestination[key] = objSource[key];
28         }
29     }
30     return objDestination;
31 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

**定义Person类**

```
1 /*定义一个Person类*/
2 function Person(_name,_age){
3     this.name = _name;
4     this.age = _age;
5 }
```

　　**在JavaScript中，Object类是所有类的父类，所以Person类从Object类继承，继承了Object类的所有public属性和public方法，包括Object类新添加的Clone和Extend方法**

**可以用如下的代码证明，Person类确实是继承了Object类**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```JavaScript
 1 document.write("<pre>");
 2 
 3 var p  = new Person("孤傲苍狼",24);//创建一个人，名字是孤傲苍狼
 4 var cloneP = p.Clone();//p调用在Object类中定义的Clone方法来克隆自己，如果能得到一个cloneP，那就证明了Person类确实是继承了Object类，所以就拥有了Clone
 5 document.writeln("p是使用Person类以构造函数的方式创建出来的对象，p.name = "+p.name+"，p.age = "+p.age);
 6 document.writeln("cloneP是p调用Clone方法克隆出来的对象，cloneP.name = "+cloneP.name+"，cloneP.age = "+cloneP.age);
 7 document.writeln("cloneP对象和p对象是两个相互独立的对象，这两个对象的内存地址肯定是不相等，p == cloneP的结果是："+(p == cloneP));
 8 cloneP.name="白虎神皇";//修改cloneP的名字
 9 document.writeln("cloneP的name被修改了，cloneP.name = "+cloneP.name);
10 document.writeln("cloneP的name修改了，但是不影响到p，p.name = "+p.name);
11 
12 document.write("</pre>");
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

运行结果：

　　![img](data:image/png;base64,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)

　　那么Person类通过神马方式来继承Object类的呢，是使用原型(prototye)的方式继承的：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```javascript
1  /*定义一个Person类*/
2  function Person(_name,_age){
3      this.name = _name;
4      this.age = _age;
5  }
6  Person.prototype = new Object();//让Person类继承Object类
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　由于JavaScript规定，任何类都继承自Object类，所以"**Person.prototype =** **new Object();**//让Person类继承Object类"即使我们不写，我**猜想**JavaScript引擎也会自动帮我们加上这句话，或者是使用"**Person.prototype =** **Object.prototype;**"这种方式，让Person类去继承Object类。"**Person.prototype =** **new Object();"，**其实这样就相当于Object对象是Person的一个原型，这样就相当于了把Object对象的属性和方法复制到了Person上了。

## 二、new运算符是如何工作的

　　我们先看看这样一段代码：

```
1 var p  = new Person("孤傲苍狼",24);//创建一个人，名字是孤傲苍狼
```

　　很简单的一段代码，我们来看看这个new究竟做了什么？我们可以把new的过程拆分成以下三步：

　　1.var p={}; 初始化一个对象p。

　　2. p.__proto__=Person.prototype;，将对象p的 `__proto__` 属性设置为 `Person.prototype`

　　3.Person.call(p,"孤傲苍狼",24);调用构造函数Person来初始化p。

关键在于第二步，我们来证明一下：

```
1 var p  = new Person("孤傲苍狼",24);//创建一个人，名字是孤傲苍狼
2 alert("p.__proto__ === Person.prototype的结果是："+(p.__proto__ === Person.prototype));
```

 在火狐下的运行结果是：

　　![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAUMAAACNCAIAAACixI8gAAAJJUlEQVR4nO3cfWtb1x3A8fsq8g5C3oTfgv/OGxBsc6wICbN/uoVtJLCQtdSJ3GRTCy1dxgJVMqUeM0oJtCOxrTihWwWlbhLbqWXZkvV4JT/D7R8HH07vg2JbV4r00/fDwTjylXTv0f3qXIlSywEwzJaXv//jH35rvevdANCteCxCycDQo2RAAkoGJKBkQAJKBiSgZEACSgYkoGRAAkoGJKBkQAJKBiSgZEACSgYkoGRAAkoGJKBkQAJKBiSgZEACSgYkoGRAAkoGJKBkQAJKBiSgZEACSgYkoGRAAkoGJKBkQAJKBiSgZEACSgYkoGRAAkoGJKBkQAJKxoBKJBKlUinon4pl/eLszWaz+XzedYsVIJvNmo/jqweH1SuU3CeJREKfH66zDV6lUimVSgX9U8lms2aNTkDJepug3x3PO4KSSCS6OIJ+G+6SE4nEsFShl5RSqWRZlnd5GXAnmeqwXo58Pm8ujKVSybW0qtnT+ZnvkprqsJ9r8rs9Gym5T8yLw1Qq5VpMBl+fS9bzo+bNXB5TqZRqWz2XObHvdk0e7pLV3p/5ujHo7vp2PZved0rXu6/vNqfiff82X/gz/Ml1pL4le+/lOnbz8fWj+R6pepCgfej1VJvbJBIJvQP5fF7dq8Op4poEV8mpVMrcWJWs7+Jdsc3H78WanEqlvJf6rikyZ1XtrdrM/N331T+zEErWe6Z2PZS7m7c7x+eo72b6NQ7aZkD4Xl1bxmW2Phbz2F0vvBJ0pJZlqTPM9wK+D1Ott9H1Oo6jOwzaAe8k+F5d62dXJTuOc9qz3/u5+mx8S3Z+OUVBL6L5u++rf2bhrMlB/zzz3c3bzdMiaLMO2wwI87Ocev1c56vOw7Xn5l2cjkfqOjO8Jfd6ql21ez/NenfAdxK8V9eO8Y6gSnbNp+9y2mFBVtTF0Vu3efur6zlA83ffkoNe/TMb1pLVq9v59Drtt0p9u7oOOi7fOdE3qlO2w5FavSn55FNtPlT2mC4hqGTvJPiW7Bx/cNVrsub9Ztv7mOoq/a3fUOTz+ZOn632KU5Uc7nfjIZSsj1yd085pvp71vbvjty55P3e5NgvaZkD4vrNYxsdF3zNepeu6PehIvSWbL0Qfptq15+oDs7lLvjvgnYSgkl236KXYfL/Q25i/W8eX6Irl+UJHL/iupdL3LDrh1bWrZP2JoMOBdyOckvWRqxtPW7Lr7o7fumTOr75RzUuHr2EGh2/J5mWh70ngOI6eH/PF9j3Sk5Tc06l2bZNKpVxF+e6AdxK8n5Ndu6d+6gcPurpWT2d+s63fINSf1JSau32SNTmoZPNxXLOqL9zU8wYdeDdCvrru891xcv2fatel7Ml3wHdN1ie9t6KTXF07Ad946bcb7wfXoKcbTL0t2XdqzPehXp9eb92B3j31oOlzyd5v3XnX7inW5FHRz6lW15Cup+O17qnh/m+8ACiUDEhAyYAElAxIQMmABJQMSEDJgASUDEhAyYAElAxIQMmABJQMSEDJgASUDEhAyYAElAxIQMmABJQMSEDJgASUDEhAyYAElAxIQMmABJQMSEDJgASUDEhAyYAElAxIQMmABJQMSEDJgASUDEhAyYAElAxIEI9FrA0AQy4ei1hNAEOOkgEJKBmQgJIBCSgZkICSAQkoGZCAkgEJKBmQgJIBCSgZkICSAQkoGZCAkjGgarXqVrFQLKxtrK/2Yay++r53Qz9LsbBWLLzZKhZqtWq400XJGFBbxUKpuPre7S/OX7xqjU3JGOfGr/z6+ufbpZ+qlXK400XJGFDFwpv3bn9xYfzai6IjaVwYv/a722nWZIyKYuHN+YtXv8rvxGMRYT/PX7xq23a400XJGFDFwpo1NpX9rp39bufo6Ojo6FDAyP6/HY9FrLGpVqsV7nRRMgZUsfDGGpv6z7eteCxycLAvYuzN/a89923bGptqt9vhThclY0Cpkmef2/9+Ye/t7QgYu7vt2Re2WpMpGaNClZx51ozHIjs7toDRbjczz5qZpSYlY4SoktML9fR8Y2enGe746KNbz58vdtjg8uXJzhucYbTbjfR8nTUZo0WVfO9JLR6LtNuNbsaXX/7LOra0tLC0tGAZlpYW2u3G2torq6O1tVdd7karVb/3pHbvSY2SMUJ0yf98Wu8yITUuX55Uxc7M3FS3LC0t6N/X1l6pDXzHzMzNsEpmTcZoUSXf/boSj0VarXqXY3X15cOHD1qtejR6KRq95Fpv1Z96PWy7dve/1bvfVCkZI0SX/PdvKt1XlEzeVNE+e7YQjV5aXX25uvoymbzZatUfPnygRuer6+5rVyWzJmO0qJI/fbwdj0Vsu9bNyOXmLcvK5eaTyelcbt67Jmcy9113WVn5MZmc7vJ5XaPZrH72uPLZ421KxgjRJX/6eLvLhKLRS6phXfLKyo+61Uzmvi45k7mfy833tGTWZIwWVfLHj0rxWMS2q12OZHI6l3uqfkajE35rctW2q9HoxMrKsm1XV1aWzQ2i0Ynu96HZrHzyqPzxozIlY4Tokj/5qhxiyZnMfZXlyspyMjlt29XjNbmayz21LCuXe2r+NcShSmZNxmhRJd+ZK8VjkWaz0uW4detDtbpOTk5kMulms/L69Q+3bn3YbFYymbS6xbKs169/sCwrk0nrv4Y4Go3tv86V7sxtUTJGiC75ztxWKCUvLj5pNiuTkxPqFtXq5OSEZVmLi0/0Bmb2pu7DViWzJmO0qJKTs5vxWKTRqAgY9fr2zOzmzOwmJWOEqJKnM8XpzEajsS1g1Ovl6UyRNRmjRZX8wYONeCxSr5cFjFpt64MHG+8/KFAyRogq+UZ6/UZ6vV4vCRi16uaN9DprMkaLKvl6ej0ei5TLhe1yYbu8MbyjXC5sba5dT69fT69TMkbI5sZP58avqFM/HotI+nlu/Mru7m6400XJGFDlreKv/vz5hfFr47//+jd/qooZF8avXfrL3b29vXCni5IxoKrVSr1WuvK3+8L+z/XR9/+xs9PY398Pd7ooGQPKtu1Wq7W7u7u/v3/Qe0e9pJ/l8PDw8PDw4OCA/0suAB+UDEhAyYAElAxIQMmABJQMSEDJgASUDEhAyYAElAxIQMmABJQMSEDJgASUDEhAyYAElAxIEI9FfgYYbxAVKf7raAAAAABJRU5ErkJggg==)

这段代码会返回true。说明我们步骤2的正确。

　　注意：__proto__这个属性只有在firefox或者chrome浏览器中才是公开允许访问的，因此，其他基于IE内核的浏览器是不会返回true的。

　　那么__proto__是什么？在这里简单地说下。每个对象都会在其内部初始化一个属性，就是  __proto__，当我们访问一个对象的属性时，如果这个对象内部不存在这个属性，那么他就会去__proto__里找这个属性，这个  __proto__又会有自己的__proto__，于是就这样一直找下去，也就是我们平时所说的原型链的概念。

　　按照标准，__proto__是不对外公开的，也就是说是个私有属性，在IE下是无法访问__proto__属性的，但是Firefox的引擎将他暴露了出来成为了一个公有的属性，我们可以对外访问和设置。

好，概念说清了，让我们看一下下面这些代码：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 <script type="text/javascript">
2         var Person = function () { };
3         Person.prototype.Say = function () {
4             alert("Person say");
5         }
6         var p = new Person();
7         p.Say();
8 </script>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　这段代码很简单，我们看下为什么p可以访问Person的Say。

　　首先

```
1 var p=new Person()；
```

　　可以得出

```
1 p.__proto__=Person.prototype
```

　　那么当我们调用p.Say()时，首先p中没有Say这个属性， 于是，他就需要到他的__proto__中去找，也就是Person.prototype，而我们在上面定义了 

```
1 Person.prototype.Say=function(){
2         alert("Person say");
3 };
```

　　 于是，就找到了这个方法。

　接下来，让我们看个更复杂的。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```javascript
 1 <script type="text/javascript">
 2         var Person = function () { };
 3         Person.prototype.Say = function () {
 4             alert("Person say");
 5         }
 6         Person.prototype.Salary = 50000;
 7         var Programmer = function () { };
 8         Programmer.prototype = new Person();//让程序员类从人这个类继承
 9         Programmer.prototype.WriteCode = function () {
10             alert("programmer writes code");
11         };
12         Programmer.prototype.Salary = 500;
13         var p = new Programmer();
14         p.Say();
15         p.WriteCode();
16         alert(p.Salary);
17 </script>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 　　我们来做这样的推导：

```
1 var p=new Programmer();
```

　　可以得出

```
1 p.__proto__=Programmer.prototype;
```

　　而在上面我们指定了

```
1 Programmer.prototype=new Person();
```

　　我们来这样拆分，

```
1 var p1=new Person();
2 Programmer.prototype=p1;
```

　　那么:

```
1 p1.__proto__=Person.prototype;
2 Programmer.prototype.__proto__=Person.prototype;
```

　　由根据上面得到

```
1 p.__proto__=Programmer.prototype
```

　　可以得到：

```
1 p.__proto__.__proto__=Person.prototype
```

　　好，算清楚了之后我们来看上面的结果,p.Say()。由于p没有Say这个属性，于是去  p.__proto__，也就是Programmer.prototype，也就是p1中去找，由于p1中也没有Say，那就去  p.__proto__.__proto__，也就是Person.prototype中去找，于是就找到了Say方法。这也就是原型链的实现原理。

　　以下代码展示了JS引擎如何查找属性：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 function getProperty(obj, prop) {
2     if (obj.hasOwnProperty(prop))
3         return obj[prop];
4     else if (obj.__proto__ !== null)
5         return getProperty(obj.__proto__, prop);//递归
6     else
7         return undefined;
8 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 　　范例：查找p对象的Say方法

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1  <script type="text/javascript">
 2     /*查找obj对象的prop属性*/
 3      function getProperty(obj, prop) {
 4         if (obj.hasOwnProperty(prop))
 5             return obj[prop];
 6         else if (obj.__proto__ !== null)
 7             return getProperty(obj.__proto__, prop);//递归
 8         else
 9             return undefined;
10     }
11 
12     var Person = function () { };//定义Person类
13     Person.prototype.Say = function () {
14         alert("Person say");
15     }
16     Person.prototype.Salary = 50000;
17 
18     var Programmer = function () { };//定义Programmer类
19     //Programmer.prototype = new Person();//让程序员类从人这个类继承，写法一
20     Programmer.prototype = Person.prototype;//让程序员类从人这个类继承，写法二
21     Programmer.prototype.WriteCode = function () {
22         alert("programmer writes code");
23     };
24     Programmer.prototype.Salary = 500;
25     var p = new Programmer();
26     var SayFn = getProperty(p,"Say");//查找p对象的Say方法
27     SayFn.call(p);//调用找到的Say方法
28 </script>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

在火狐下的运行结果：

　　![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAO8AAACSCAIAAAAirgrxAAAFGUlEQVR4nO3bz07bSADH8XmXvIYLtIeEa1Wplz6B+xKRCBdLQP760FUF6snn3EpV1KoupxYIW6AVla8r0d3VNtAeKIfsYdRZ1wHULoNCfv5+FEWTwWNb5itjIcWsNFutdjeO414vXlputru9OI7b3d7ScrPbi+M4brW7y81WHMdxHDdbnWarY8csZOFNW2hWV1cXFxdDYMotLi6aRqMxGAxGwJTb3t42YRhO+jQAP6gZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqgZOqj5B2EYmu8qlcqkTwe/hpqLXMRJkkRRNNmTwS+h5iJqnl7UXDT+pJEkiZ2xcdunkSiK3GA0GkVRVFjlZtyqSqVSq9WMMbVa7aKjj+/HPfzkDxSGoRtc58WYMlyOosLjcpZl7hJFUZRl2Wg0StPUGJMkidvM3cXtxmmaumTDMEzTdDQaGWPs4PKa8/vJsszNuFW1Ws2eBr+7AmouKtRsw3VsjmmaFh5C3GZ2gyiKXOvuicXt+ZKaC/sZ5e7WbpU9epIkbhtY1Fx0yb3ZGa85fwfNsixNU7fK3Zt/pubCfqIoGr83233yixtHzT/IPwo77rnZGGPjdh/dNpVKpXAHHX9utmM7f1HQhf0U/jK4X1aSJPnnHFjUPJX4Z8u5qHnKZFk2/tcDFjVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDBzVDhwnD8A9AggnD8BiQQM3QQc3QQc3QQc3QQc3QQc3QQc3QQc3QQc3QQc3QQc3QQc3QQc3QQc1T4MmL/fvNZ0G9L/O633z25MW+9wtFzTfd2sbebO9V9c1w/sOZzKv6Zjjbe7W2sef3WlHzTXdvZb36dvgwO530ifj0MDutvh3eW1n3u1tqvumCen/+/dmkz8K/+fdnQb3vd5/UfNPZmodyqLmMbM2fr4cx5n//9IqouYyCer928O0fH0zORTPjS7wcelzt4Bs1l46t+W9/jDGFQWHycr5Og5rL6PpqPnfm8l6pGVcS1PvVg9O/PDHG5McFvo7yM6oHp9RcOkG9X90//dMTF64du/f8x8v5OpPqPjWXT1Dv39n9mvlgc7QD9zHv3CVeDj3uzu5Xai6doN6/Pfjy0RNjzLnv+cFFk37dHnyh5tKxNR96YozJvxcUNhvfxtdpHB4eUnMZBfX+3ODkgyeuSzt27/nBRZN+zQ1OqLl0vNd87vv4oPBT76i5jIJ6f27n5MA3Y0zho53Jz5/7Pw1fJzC3Q83lE9T7s9vHe3Jmt4+puXRszb/LoeYyCur9ma3hrpyZrSE1l869lfWZ9OjB1qcdIQ+2Ps2kR3z3pHTWNvaC9stbr49ubX3Web0+Ctov+V5gGT1+/u7u0tOJf9Ha4+vu0tPHz995v1DUDB3UDB3UDB3UDB3UDB3UDB3UDB3UDB3UDB3UDB3UDB3UDB3UDB3UDB3UDB3UDB3UDB1mYWFhc3Nz0qcBXFWapubRo98ajUYITLlGo2Ha3V6nG9uXHXd7/43z851u3On2Ot/HLGThTVv4L+4gsnLP97rlAAAAAElFTkSuQmCC)

　　其实prototype只是一个假象，他在实现原型链中只是起到了一个辅助作用，换句话说，他只是在new的时候有着一定的价值，而原型链的本质，其实在于__proto__。



分类: [JavaScript](https://www.cnblogs.com/xdp-gacl/category/431982.html)