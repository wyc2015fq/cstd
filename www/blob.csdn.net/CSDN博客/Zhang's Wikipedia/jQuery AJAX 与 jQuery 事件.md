
# jQuery AJAX 与 jQuery 事件 - Zhang's Wikipedia - CSDN博客


2018年03月08日 23:30:54[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：210



> jQuery 本身即是为事件处理而特别设计的，jQuery 事件处理方法是 jQuery 中的核心函数。
$(function() { ... });是如下格式的缩写：
$(document).ready(function() {...});
## 0. $ 符号
根据类别，定位标签：
<button>click me</button><p>hide me</p><p>hide me 2</p>$('button').click(function(){
    $('p').hide();          // 作用在两个<p>/</p>上
});
根据 id 定位标签：
<h2 id='temp'></p>
$('\#temp').html(...);
## 1. demo：点击按钮，将段落折叠
```python
$(document).ready(
```
```python
function
```
```python
()
```
```python
{
```
```python
$(
```
```python
'button'
```
```python
).click(
```
```python
function
```
```python
()
```
```python
{
```
```python
$(
```
```python
'p'
```
```python
).hide();
    });
});
```
## 2. jQuery ajax - getJSON() 方法
使用 AJAX 请求来获得 JSON 数据，并输出结果：
$("button").click(function(){$.getJSON("demo_ajax_json.js",function(result){$.each(result,function(i, field){// 遍历数组$("div").append(field +" ");
    });
  });
});

