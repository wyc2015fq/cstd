# es5 温故而知新 创建私有成员、私有变量、特权变量的方法 - weixin_33985507的博客 - CSDN博客
2018年10月27日 17:19:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
其实js是不支持私有变量的。哪怕到es6的class语法。虽然有许多变相的方式。但非常冗余而不推崇。
这里介绍的实际上也不是class语法，而是普通的函数，并且利用IIFE（闭包）的方式来实现私有。
这种方式也被称为“**模块模式”**
varperson=(function(){varage=25return{name:'Lee',getAge:function(){returnage},setAge:function(){age++}}}());console.log(person.name)// Leeconsole.log(person.getAge())// 25person.age=100// hack try...console.log(person.getAge())// 25
构造函数的私有变量
functionPerson(name){this.name=namevarage=18this.getAge=function(){returnage}this.setAge=function(){age++}}varperson=newPerson('Lee')console.log(person.name)// Leeconsole.log(person.getAge())// 18person.age=100// hack try...Person.age=100// hack try...console.log(person.getAge())// 18
