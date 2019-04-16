# 如何编写合理的Javascript代码并牢记javascript的地位 - ggabcda的博客 - CSDN博客





2017年09月07日 09:37:53[ggabcda](https://me.csdn.net/ggabcda)阅读数：117








        作为前端程序员，一定要认清javascript的地位，不要被它乱七八糟的特点所迷惑。JavaScript主要是用来操控和重新调整DOM，通过修改DOM结构，从而达到修改页面效果的目的。

      要用这个中心思想指导以后的所有js学习，包括它的框架。

      所谓合理就是合乎道理。那么合理的javascript代码通常遵循哪些原则？笔者认为至少要遵循以下几点： 

## 有意义的命名 

无论选择驼峰命名法还是匈牙利命名法，见名知意是最重要也最基本的一条法则。同时也要避免会产生歧义的命名。

```
例如 变量“当前时间”

var d;不好的命名    

 var currentTime 好的命名
```





## 每一个函数只干一件事

        每个函数只干一件事就是避免把所有业务逻辑都放在同一个函数中完成，这样的好处是代码更为简洁，可读性提高，易于维护及复用。

## 合理的传参



```
举个简单的例子：

好的传参

funcation  queryStudent(Json student){

      ……………….

}

不好的传参

funcation  queryStudent(id,name,…){

      ……………….

}
```



## 合理的DOM

```
合理的

var  phaseObject =  

{

northLeftPhase = document.getElementById('northLeftPhase')

 }

phaseObject.northLeftPhase.src='http://phase.com/n/left.png';

不合理的

document.getElementByID('northLeftPhase').src = 'http://phase.com/n/left.png';
```



## 合理的注释

```
语言精练，使用规范的注释模式，防止多意义，常更新。

原则，并不是所有的变量函数都需要注释，一般注释哪些比较生涩的算法。

精练的注释

//Selector the Elements

 var  findElements = window. findElements = function( exp ){ ....................... }

不精练的注释

//Selector the Elements by GetElementById & getElementsByTagN ....

Var findElements = window. findElements = function( exp ){ ....................... }

注释 ！= 字幕

好的

// find [ID = A]

 var oA = document.getElementById('A');

不好的

/*Found through the document.getElementByID*/

 var oA = document.getElementById('A');
```



## 合理的排版

### 缩进排版(Indentation)

4个空格常被作为缩进排版的一个单位。缩进的确切解释并未详细指定(空格 vs. 制表符)。一个制表符等于n个空格(视具体的编辑器而定，Eclipse默认一个制表符为4个字符)。

### 行长度(Line Length)

尽量避免一行的长度超过80个字符，因为很多终端和工具不能很好处理之。

注意：鉴于Eclipse开发工具工作区的左侧和右侧都被非代码编辑器所占据，因此建议每行的代码长度不要超过70个字符。

###  换行(Wrapping Lines)

 当一个表达式无法容纳在一行内时，可以依据如下一般规则断开之：
- -  在一个逗号后面断开； 
-  在一个操作符前面断开； 
-  宁可选择较高级别(higher-level)的断开，而非较低级别(lower-level)的断开； 
-  新的一行应该与上一行同一级别表达式的开头处对齐。 


## 模块化编程
- 便于管理； 
- 提高开发效率； 
- 减少错改代码的次数； 
- 易于维护； 
- 提高代码可读性； 
- 易于构建大型项目。 



