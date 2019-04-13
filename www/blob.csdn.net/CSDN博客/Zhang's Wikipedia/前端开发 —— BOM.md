
# 前端开发 —— BOM - Zhang's Wikipedia - CSDN博客


2018年05月06日 17:49:46[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：195



## 0. BOM vs. DOM
BOM：Browser Object Model，浏览器对象模型，是在 web 中使用 JavaScript 的绝对核心；
BOM：提供了独立于内容而与浏览器窗口进行交互的对象。
BOM 提供了很多对象（window、location、navigator），用于访问浏览器的功能，这些功能与任何网页内容无关；
DOM：Document Object Model，DOM是针对XML的基于树的API。描述了处理网页内容的方法和接口，是HTML和XML的API，DOM把整个页面规划成由节点层级构成的文档。DOM本身是与语言无关的API，它并不与Java，JavaScript或其他语言绑定。
1. window 对象
全局作用域：
window 对象同时扮演着 ECMAScript 中 Global 对象的作用，因此所有在全局作用域中声明的变量、函数都会变成 window 对象的属性和方法，
varage =26functionshowAge(){alert(this.age)
}// 访问全局作用域里的变量alert(window.age)//showAge()// 访问全局作用域中的函数window.showAge()

