# node.js中的DIRT程序 - LC900730的博客 - CSDN博客
2017年08月27日 20:17:07[lc900730](https://me.csdn.net/LC900730)阅读数：341
## DIRT
data-intensive real-time：数据密集型实时程序。 
package.json总是被放在程序的根目录下。package.json用于描述你的应用程序
### 创建模块
模块既可以是一个文件，也可以是包含一个或者多个文件的目录。如果模块是目录，Node通常会在这个目录下找一个index.js的文件作为模块的入口。
### 用module.exports微调模块的创建
尽管用函数和变量组装exports对象能满足大多数的模块创建需要，但是有时可能需要不同的模型创建该模块。 
如有时候只返回一个构造函数，而不是包含2个函数的对象，面向对象的实现看起来：
```
var Currency =require('./currency');
var CanadianDollar=0.91;
var currency=new Currency(canadianDollar);
consoloe.log(currency.canadianToUs(50));
```
Node不允许把任何其他对象、函数、变量给exports赋值。 
如：
```java
var Currency=function(canadianDollar){
        this.canadianDollar=canadianDollar;
    }
    Currency.prototype.roundTwoDecimals=function(amount){
    return Math.round(amount*100)/100;
}
    Currency.prototype.canadianToUS=function(canadian){
        return this.roundTwoDecimals(canadian*this.canadianDollar);
        ...
    }
    exports=Currency;
```
### 回调
回调是一个函数，它被当做参数传给异步函数，它描述了异步操作完成之后要做什么。
