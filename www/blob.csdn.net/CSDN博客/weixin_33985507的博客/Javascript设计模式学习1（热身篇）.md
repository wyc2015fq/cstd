# Javascript设计模式学习1（热身篇） - weixin_33985507的博客 - CSDN博客
2019年01月04日 05:30:43[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
#### 从创建一个函数开始
函数的创建有很多形式，通常我们会采用以下做法
```java
function getName(){
    //get name here
}
function getAge(){
 //get age here
}
var getName = function(){
    //get name here
}
var getAge= function(){
 //get age here
}
复制代码
```
但是这种做法有个弊端，这类函数因为都是全局变量，所以在团队合作，或者项目函数很多时就会出现函数命名重复的问题，那就糟糕了，会出现函数被覆盖，导致不必要的工作量。所以可以采用下面的方式（用对象来收编方法，变量）
```java
var GetUserInfo= {
    getName:function(){
        //get name here
    },
    getAge :function(){
        //get age here
    }
}
//或者像下面这样
var GetUserInfo= function(){};
GetUserInfo.getName = function(){
 //get name here
},
GetUserInfo.getAge = function(){
 //get age here
 }
复制代码
```
但是上面方法，还有个问题，就是这个对象不能复制，比如我只想使用里面的getUserInfo.getAge()这个方法，但是没有办法复制一个这样的对象来给自己使用，除非代码全部拷贝一遍，这无疑徒增了代码量，所以我们可以采用return来返回一个新的函数对象，如下
```java
var GetUserInfo= function(){
 return {
     getName :function(){
         //get name here
     },
     getAge : function(){
         //get age here
     }
 }
}
复制代码
```
这样我们就可以安心复制getUserInfo其中的某个方法来为自己所用了,使用方法如下
```java
var getUserInfo_myself = GetUserInfo()
getUserInfo_myself.getName()
复制代码
```
上面的方法虽然可以完美达到我们的要求，但是编程最重要的是什么？当然时面向对象！我们来回顾一下面向对象的三个基本特征：封装、继承、多态。
封装：类其实就是保存了一个函数的变量，这个函数有自己的属性和方法。将属性和方法组成一个类的过程就是封装。
所以要实现封装，我们就得用JS实现一个类
```java
//ES5
var GetUserInfo= function(){
    this.getName = function(){
         //get name here
     },
     this.getAge = function(){
         //get age here
     },
}
//this的指向简单理解就是函数被调用时的执行环境，也可以说时谁调用该函数，this就指向谁
//这里this就指向getUserInfo_myself
复制代码
```
上面的代码（构造函数）就实现了一个类（函数，有自己的属性，有自己的方法）
调用时，我们就不能var getUserInfo_myself = GetUserInfo()这样来用了。要像下面这样
```java
var getUserInfo_myself = new GetUserInfo()
getUserInfo_myself()
复制代码
```
以为这样就结束？这还不算是最好的方案，因为通过new来创建新对象的时候，新创建的对象都会复制一次this的属性，用多了就会造成很多消耗，我们可以避免吗？可以！通过原型的方式来实现，看下面
```java
//ES5方法一
var GetUserInfo=function(){};
GetUserInfo.prototype.getName={
    //get name here
}
GetUserInfo.prototype.getAge={
  //get age here
}
//ES5方法二
var GetUserInfo=function(){};
GetUserInfo.prototype={
    getName(){
     //get name here
    }
    getAge(){
     //get agehere
     }
}
//用ES6的语法
class GetUserInfo{
 constructor(){}
 getName(){
 //get name here
 }
 getAge(){
 //get age here
 }
}
复制代码
```
使用方法如下
```java
var getUserInfo_myself = new GetUserInfo()
// class方法必须使用new，否则会报错
getUserInfo_myself.getName()
getUserInfo_myself.getAge()
复制代码
```
之前的例子中我们想要执行getName和getAge方法都要将对象getUserInfo_myself写两次，那我们就可以用链式调用来优化一下，那我们应该怎么做？看下面
```java
//创建对象的方式实现
var GetUserInfo= {
 getName: function(){
 console.log('getName')
 console.log(this)
 return this
 },
 getAge: function(){
 console.log('getAge')
 console.log(this)
 return this
 }
}
GetUserInfo.getName().getAge()
//这里的this指向GetUserInfo，所以GetUserInfo.getName().getAge()其实就是这样执行的
//1. GetUserInfo.getName() 返回this,this指向GetUserInfo，GetUserInfo有两个方法getName()和getAge()
//2. this.getAge() 也就是 GetUserInfo.getAge()
//原型的方式实现
var GetUserInfo = function(){}
GetUserInfo.prototype={
    getName(){
         //get name here
         return this
     }
     getAge(){
         //get agehere
         return this
     }
}
复制代码
```
