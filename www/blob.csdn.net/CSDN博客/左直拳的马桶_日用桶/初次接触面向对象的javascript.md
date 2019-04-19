# 初次接触面向对象的javascript - 左直拳的马桶_日用桶 - CSDN博客
2013年01月23日 16:05:36[左直拳](https://me.csdn.net/leftfist)阅读数：1352
一、对象作用域
```java
function ClassA(){
    var v = "我是变量 v";
    this.v = "我是属性 v";
    var me = this;
    
    ClassA.prototype.F = function(){
        alert("我是可供外部访问的函数");
    }
    function _F(){
        alert("我只是一个内部函数，外部不能访问我。");
    }
    ClassA.prototype.F2 = function(){
        alert(v);       //OK
        alert(this.v);  //OK
        _F();           //OK
        F();            //错
        this.F();       //OK
    }    
    function _F2(){
        F();            //错
        this.F();       //错
        me.F();         //OK
    }
}
var objA = new ClassA();
alert(objA.v);          //显示“我是属性 v”
objA.F2();              
objA._F2();             //系统报错，提示缺少对象
```
二、改写
```java
function ClassA(){
    var v = "我是变量 v";
    this.v = "我是属性 v";
    
    if (typeof ClassA._initialized == "undefined"){
        ClassA.prototype.F = function(){
            alert("我是可供外部访问的函数");
        }
        ClassA._initialized = true;
    }
        
    function _F(){
        alert("我只是一个内部函数，外部不能访问我。");
    }    
}
var objA = new ClassA();
objA.v = "我是属性 v，现在被重新赋值鸟";
ClassA.prototype.F = function(){
            alert("我是可供外部访问的函数，现在被改写鸟");
        }
alert(objA.v);          //显示“我是属性 v，现在被重新赋值鸟”
objA.F();               //显示"我是可供外部访问的函数，现在被改写鸟"
```
```java
function ClassA(){
    this.v = "我是属性 v";
    if (typeof ClassA._initialized == "undefined"){
        ClassA.prototype.F = function(){
            alert("我是可供外部访问的静态函数 F");
        }
        ClassA._initialized = true;
    }
    
    this.F2 = function(){
        alert("我是可供外部访问的函数 F2");
    }
}
var objA1 = new ClassA();
var objA2 = new ClassA();
ClassA.prototype.F = function(){
            alert("我是可供外部访问的静态函数 F，被修改鸟。");
        }
objA1.F2 = function(){
        alert("我是可供外部访问的函数 F2，被 objA1 修改鸟");
    }
objA1.F();  //我是可供外部访问的静态函数 F，被修改鸟。
objA2.F();  //我是可供外部访问的静态函数 F，被修改鸟。
objA1.F2(); //我是可供外部访问的函数 F2，被 objA1 修改鸟
objA2.F2(); //我是可供外部访问的函数 F2
```
三、继承
```java
function ClassA(){
    this.vA = "我是 ClassA 的属性 vA";
    if (typeof ClassA._initialized == "undefined"){
        ClassA.prototype.F = function(){
            alert("我是 ClassA 可供外部访问的静态函数 F");
        }
        ClassA._initialized = true;
    }
    
    this.F2 = function(){
        alert("我是可供外部访问的函数 F2");
    }
}
function ClassB(){
    this.vB = "我是 ClassB 的属性 vB";
}
ClassB.prototype = new ClassA();
var objA = new ClassA();
var objB = new ClassB();
alert(objB.vA);     //我是 ClassA 的属性 vA
alert(objB.vB);     //我是 ClassB 的属性 vB
objB.F();           //我是 ClassA 可供外部访问的静态函数 F
objB.F2();          //我是可供外部访问的函数 F2
ClassB.prototype.F = function(){
            alert("我是 ClassA 可供外部访问的静态函数 F，被 ClassB 修改鸟。");
        }
objA.F();           //我是 ClassA 可供外部访问的静态函数 F
objB.F();           //我是 ClassA 可供外部访问的静态函数 F，被 ClassB 修改鸟。
```
参考文章：
http://www.w3school.com.cn/js/pro_js_object_defining.asp
