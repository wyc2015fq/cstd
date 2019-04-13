
# 1.22 JQuery5：绑定自定义事件 - 尹成的技术博客 - CSDN博客

2018年11月14日 12:10:46[尹成](https://me.csdn.net/yincheng01)阅读数：62个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)



# JQuery 自定义事件
jQuery的事件自定义事件还是通过on绑定的，然后再通过trigger来触发这个事件
### 1. 绑定事件
```python
//给element绑定hello事件
element.bind("hello",function(){
    alert("hello world!");
});
```
### 2. 执行事件
```python
//触发hello事件
element.trigger("hello");
```
# trigger的几种常见用法
### 1.常用模拟
在jQuery中，可以使用trigger()方法完成模拟操作。例如可以使用下面的代码来触发id为btn按钮的click事件。
```python
$("#btn").trigger("click");
```
也可以直接用简化写法click()，来达到同样的效果（对已存在的系统事件调用）：
```python
$("#btn").click();
```
### 2.触发自定义事件
trigger()方法不仅能触发浏览器支持的具有相同名称的事件，也可以触发自定义名称的事件。
例如为元素绑定一个“myClick”的事件，jQuery代码如下：
```python
$("#btn").bind("myClick", function () {
    $("#test").append("<p>我的自定义事件。</p>")；
})；
```
想要触发这个事件，可以使用下面的代码来实现：
### 3.传递数据
trigger(tpye[,datea])方法有两个参数，第一个参数是要触发的事件类型，第二个单数是要传递给事件处理函数的附加数据，以数组形式传递。通常可以通过传递一个参数给回调函数来区别这次事件是代码触发的还是用户触发的。
下面的是一个传递数据的例子:
```python
$("#btn").bind("myClick", function (event, message1, message2) { //获取数据
    $("#test").append("p" + message1 + message2 + "</p>");
});
$("#btn").trigger("myClick",["我的自定义","事件"]); //传递两个数据
$(“#btn”).trigger(“myClick”,["我的自定义","事件"]); //传递两个数据
```
### 4.执行默认操作
triger()方法触发事件后，会执行浏览器默认操作。例如：
```python
$("input").trigger("focus");
```
以上代码不仅会触发为input元素绑定的focus事件，也会使input元素本身得到焦点（浏览器默认操作）。
如果只想触发绑定的focus事件，而不想执行浏览器默认操作，可以使用jQuery中另一个类似的方法-triggerHandler()方法。（没有试验过，知道有这么一回事）
```python
$("input").triggerHandler("focus");
```
该方法会触发input元素上绑定的特定事件，同时取消浏览器对此事件的默认操作，即文本框指触发绑定的focus事件，不会得到焦点。
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/337650?tuin=63946d38)
扫码获取海量视频及源码   QQ群：721929980
![](https://img-blog.csdnimg.cn/20181108132958856.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)


