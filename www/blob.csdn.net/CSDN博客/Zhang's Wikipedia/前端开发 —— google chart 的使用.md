
# 前端开发 —— google chart 的使用 - Zhang's Wikipedia - CSDN博客


2018年05月06日 10:49:48[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：2967



## 1. 引入所需的 js 库
在<head></head>中
<scriptsrc="https://ajax.googleapis.com/ajax/libs/jquery/1.11.2/jquery.min.js"></script><scripttype="text/javascript"src="https://www.google.com/jsapi?autoload={
  'modules':[{
    'name':'visualization',
    'version':'1',
    'packages':['corechart']
  }]
}"></script>
## 2. 简单绘制 line
```python
<
```
```python
html
```
```python
>
```
```python
<
```
```python
body
```
```python
>
```
```python
<
```
```python
div
```
```python
id
```
```python
=
```
```python
"chart"
```
```python
style
```
```python
=
```
```python
"width: 800px; height: 600px"
```
```python
>
```
```python
</
```
```python
div
```
```python
>
```
```python
<
```
```python
script
```
```python
type
```
```python
=
```
```python
"text/javascript"
```
```python
>
```
```python
....
```
```python
</
```
```python
script
```
```python
>
```
```python
</
```
```python
body
```
```python
>
```
```python
</
```
```python
html
```
```python
>
```
作用在某 div 元素上：
varchart =newgoogle.visualization.LineChart($('chart')[0]);
表格选项：
varoptions={
    title:'Temperature',
    curveType:'function',
    animation: {duration:1000,
        easing:'in'},
    legend: {
        position:'bottom'}
}
存储二维数组的 DataTable
vardata= google.visualization.arrayToDataTable([
    ['Time', 'Temperature'],
    [newDate().toLocaleTimeString(), 0]]);
绘制：
chart.draw(data, options)

