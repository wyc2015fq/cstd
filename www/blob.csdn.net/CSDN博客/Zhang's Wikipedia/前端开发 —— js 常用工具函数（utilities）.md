
# 前端开发 —— js 常用工具函数（utilities） - Zhang's Wikipedia - CSDN博客


2018年05月06日 11:03:36[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：568



## 1. 时间
```python
function
```
```python
getCurTime
```
```python
()
```
```python
{
```
```python
var
```
```python
date =
```
```python
new
```
```python
Date
```
```python
();
```
```python
return
```
```python
date.toLocaleTimeString();
}
```
date.toLocaleTimeString()：上午还是下午，几点几分；
date.toLocaleDateString()：日期，月和日；
## 2. 随机
区间：
functionrandInt(low, high){returnMath.floor(Math.random()*(high-low) + low)
}
## 3. setInterval 与 setTimeout
每隔 1 秒执行一次：
setInterval (function(){console.log('hello world')
},1000)
setInterval (function(){console.log(Math.floor(Math.random()))
},1000)如何关闭定时周期执行呢，需要事先保存 id 识别码：
varid = setInterval (function(){varnum =Math.floor(Math.random() *1024)
    console.log(num)
},3000);
....
clearInterval(id)
setTimeout() ：定时运行函数，只会在指定时间到时运行一次；


