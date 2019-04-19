# JS重名解决方案 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月29日 15:35:25[左直拳](https://me.csdn.net/leftfist)阅读数：4325
一个页面如果引用多个JS，或者像ASP.NET MVC，一个视图包含多个子视图，每个子视图有自己的JS，那么变量、函数的重名冲突机会将会大增。
如何解决？
这里有一个方案：
1、用类来封装子页的JS代码。
```java
<script type="text/javascript">
var globalName = "左直拳";
var A = function(){
    var name = "leftfist";
    function hi(){//私有函数，外部无法访问
        alert(name + " : Hi!");//私有变量，内部可以直接访问
    }
    this.hiGlobal = function(){
        alert(globalName + " : Hello!");//全局变量，内部可以直接访问
    }
    this.sayHello = function(){//public函数，外部可访问
        hi();//私有函数，内部可以直接访问
        this.hiGlobal();//公有函数，内部访问也要加上 this.
    }
};
var fa = new A();
</script>
```
`<input type="button" id="btnHi" value="sayHi" onclick="fa.sayHello();" />`
这种模式中，凡需要被外部访问，如HTML控件访问的，就定义成公有函数，其他的就是类内部的私有函数，冲突的机会将大大降低。
2、还有另外一种方式，就是将变量、函数封装到 JSON 对象里
```java
var G = {
    callback : function(){alert("callback!");}
    ,f1 : function(){
        G.callback();
    }
}
G.f1();
```
但是这种方式的话，里面的元素全部都是公有的，外部全都可以访问，固然解决了重名问题，但谈不上有所封装。这种方式，感觉就好像实现了接口的类一样。
并且里面的方法互相访问，同样要加上命名空间，麻烦了点。
