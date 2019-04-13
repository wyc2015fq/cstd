
# 1.19 JQuery2：节点插入与节点选取 - 尹成的技术博客 - CSDN博客

2018年11月11日 17:11:27[尹成](https://me.csdn.net/yincheng01)阅读数：87



# 节点选取
jQuery.parent(expr)
找父亲节点，可以传入expr进行过滤，比如$("span").parent()或者$("span").parent(".class")
jQuery.parents(expr),类似于jQuery.parents(expr),但是是查找所有祖先元素，不限于父元素
jQuery.children(expr).返回所有子节点，这个方法只会返回直接的孩子节点，不会返回所有的子孙节点
jQuery.contents(),返回下面的所有内容，包括节点和文本。这个方法和children()的区别就在于，包括空白文本，也会被作为一个jQuery对象返回，children()则只会返回节点
jQuery.prev()，返回上一个兄弟节点，不是所有的兄弟节点
jQuery.prevAll()，返回所有之前的兄弟节点
jQuery.next(),返回下一个兄弟节点，不是所有的兄弟节点
jQuery.nextAll()，返回所有之后的兄弟节点
jQuery.siblings(),返回兄弟姐妹节点，不分前后
jQuery.find(expr),跟jQuery.filter(expr)完全不一样。jQuery.filter()是从初始的jQuery对象集合中筛选出一部分，而jQuery.find()的返回结果，不会有初始集合中的内容，比如$("p"),find("span"),是从 p元素开始找,等同于$("p span")
# 节点插入
### 添加节点的jquery方法：
```python
append()、prepend()、appendTo() 、prependTo()
```
**插入节点的jquery方法：**
```python
after() 、before() 、insertBefore() 、insertAfter()
```
举个例子来说明以上几种方法的用法：
```python
html例子:
<nav>
    <ul>
        <li>序列号1</li>
        <li>序列号2</li>
        <li>序列号3</li>
    </ul>
</nav>
```
### 添加节点
1、append()方法：
```python
script:
var $div = '<div>append添加的节点</div>';
$('nav').append($div); //将新创建的div节点插入到nav容器的内容底部
html：
<nav>
    <ul>
        <li>序列号1</li>
        <li>序列号2</li>
        <li>序列号3</li>
    </ul>
    <div>append添加的节点</div>
</nav>
```
2、prepend()方法：
```python
script:
var $div = '<div>append添加的节点</div>';
$('nav').append($div); //将新创建的div节点插入到nav容器的内容顶部
html：
<nav>
    <div>prepend添加的节点</div>
    <ul>
        <li>序列号1</li>
        <li>序列号2</li>
        <li>序列号3</li>
    </ul>
</nav>
```
3、appendTo()方法：
```python
script:
var $div = '<div>append添加的节点</div>';
var nav = $('nav');
//$div.appendTo($nav);  //这样写是错误的
$('<div>append添加的节点</div>').appendTo($nav);//真理在这里。。  将新创建的节点添加到nav的内容后面
html：
<nav>
    <ul>
        <li>序列号1</li>
        <li>序列号2</li>
        <li>序列号3</li>
    </ul>
    <div>appendTo添加的节点</div>
</nav>
```
4、prependTo()方法：
```python
script:
var $div = '<div>append添加的节点</div>';
var nav = $('nav');
//$div.prependTo($nav);  //和前面同理，这样写是错误的 
$('<div>append添加的节点</div>').appendTo($nav);//真理在这里。。  将新创建的节点添加到nav的内容的前面
html：
<nav>
    <div>prependTo添加的节点</div>
    <ul>
        <li>序列号1</li>
        <li>序列号2</li>
        <li>序列号3</li>
    </ul>
</nav>
```
### 插入节点
5、after()方法：
```python
script:
var $l_1 = $('ul>li:nth-of-type(1));
var $l_2 = $('ul>li:nth-of-type(2)); 
var nav = $('nav');
$l_2.after($l_1);  //这里可以这么理解，第2个li后面跟着第1个li。
html：
<nav>
    <ul>
        <li>序列号2</li>
        <li>序列号1</li>
        <li>序列号3</li>
    </ul>
</nav>
```
6、before()方法：
```python
script:
var $l_1 = $('ul>li:nth-of-type(1));
var $l_2 = $('ul>li:nth-of-type(2)); 
var nav = $('nav');
$l_1.before($l_2);  //这里可以这么理解，第1个li前面是第2个li。
html：
<nav>
    <ul>
        <li>序列号2</li>
        <li>序列号1</li>
        <li>序列号3</li>
    </ul>
</nav>
```
7、insertBefore()方法：
```python
script:
var $l_1 = $('ul>li:nth-of-type(1));
var $l_2 = $('ul>li:nth-of-type(2)); 
var nav = $('nav');
$l_2.insertBefore($l_1);  //将第2个li插入到第1个li前面
html：
<nav>
    <ul>
        <li>序列号2</li>
        <li>序列号1</li>
        <li>序列号3</li>
    </ul>
</nav>
```
8、insertAfter()方法：
```python
script:
var $l_1 = $('ul>li:nth-of-type(1));
var $l_2 = $('ul>li:nth-of-type(2)); 
var nav = $('nav');
$l_1.insertAfter($l_2);  //将第1个li插入到第2个li后面
html：
<nav>
    <ul>
        <li>序列号2</li>
        <li>序列号1</li>
        <li>序列号3</li>
    </ul>
</nav>
```
容易出现问题的地方在 appendTo()和preappendTo()。

学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/337650?tuin=63946d38)
扫码获取海量视频及源码   QQ群：721929980
![](https://img-blog.csdnimg.cn/20181108132958856.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)






