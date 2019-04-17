# 第七课：js的DOM操作 - PeterBishop - CSDN博客





2018年12月09日 17:07:13[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：26
个人分类：[JavaScript](https://blog.csdn.net/qq_40061421/article/category/8510184)









  这节课我们来学js的DOM操作



  先来说下啥事DOM，就是Document Object Model，文档对象模型

  DOM操作就是用来操作html里的各个标签的，看演示


//这节课讲dom操作

//1 获取html的某个标签



//1.1 根据标签名

*/** * document.getElementsByTagName(tagName) * **@type **{Node} * **@Param **tagName **标签名 * **@return **所有名为tagName的标签的数组 */***var **body = document.getElementsByTagName("body")[0];



//1.2 根据id

*/** * document.getElementById(id) * **@Param **id **标签的id属性 * **@return **id为参数的标签，由于html里的标签的id是不允许重复的，所以可以获得唯一的标签对象 */*// var element = document.getElementById("");



//1.3 根据name获取标签

*/** * document.getElementsByName(name) * **@Param **name **标签的name属性 * **@return **name为参数的所有标签，由于html里的标签的name属性是可重复的，所以返回的是标签对象数组 */*// document.getElementsByName("")



//1.4 根据class样式获取标签



//2 创建标签

*/** * document.createElement(elementName) * **@Param **elementName **标签名 * **@return **创建标签名对应的标签对象并返回 */***var **aElement = document.createElement("a");



//3 操作标签



//3.1 给标签添加子标签

*/** * element.appendChild(childElement) * * element**是标签对象 * childElement是子标签 * appendChild()方法可以让childElement成为element的子标签且追加在最后面 */*body.appendChild(aElement);



//3.2 给标签删除子标签

*/** * element.removeChild(childElement) * * element**是标签对象 * childElement是子标签 * removeChild()方法可以移除element里子标签里的childElement标签 */*// body.removeChild(aElement);



//3.3 操作标签的属性



//3.3.1 操作标签的内容

//通过innerHTML属性设置标签的内容，此举相当于 <a>js教程</a>

aElement.innerHTML = "js教程";



//3.3.2 操作标签的属性



//3.3.2.1 设置标签的属性

*/** * * element.attribute = value * **相当于 <elementName attribute=value></elementName> * element.style.xxx = value * 相当于 <elementName style="xxx:value;"></elementName> * * element.className = value; * 相当于 <elementName class=value></elementName> */*aElement.id = "firsta";

aElement.style.color="red";

aElement.style.border = "1px bolder black";

// aElement.className = ""



//3.3.2.2 删除标签的属性

//element.removeAttribute(attributeName); 删除element标签的attributeName属性

// aElement.removeAttribute("")



//3.4 给标签设置事件

// element.eventName = function(){事件触发时要执行的代码}

aElement.onclick = **function **() {

    //alert(msg) 这个方法就是在浏览器弹出一个框然后显示msg的信息

    alert("你触发了点击事件");

};





//貌似就这些了




