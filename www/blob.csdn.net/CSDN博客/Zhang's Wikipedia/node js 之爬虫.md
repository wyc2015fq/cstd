
# node.js 之爬虫 - Zhang's Wikipedia - CSDN博客


2018年03月20日 00:17:06[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：247



## 1. cheerio 与 request
request：模拟客户端行为，对页面进行请求
cheerio：对服务器端返回的页面进行解析；
```python
var
```
```python
cheerio =
```
```python
require
```
```python
(
```
```python
'cheerio'
```
```python
);
```
```python
var
```
```python
request =
```
```python
require
```
```python
(
```
```python
'request'
```
```python
);
```
```python
var
```
```python
startUrl =
```
```python
'http://www.baidu.com'
```
```python
request(startUrl,
```
```python
function
```
```python
(err, response)
```
```python
{
```
```python
if
```
```python
(err) {
        console.log(err);
    }
```
```python
var
```
```python
$ = cheerio.load(response.body);
```
```python
var
```
```python
title = $(
```
```python
'title'
```
```python
).text();
    console.log(title);
}
```
## 2. 认识 cheerio
获取 element 位置
通过 class 属性进行匹配：
var$=cheerio.load('<div class="container"></div>');$('.container')取其链接：<a class="downbtn" href="http://mov.bn.netease.com/mobilev/2013/1/F/G/S8KTEF7FG.mp4" id="M8KTEKR84" target="_blank"></a>
$('a.downbtn').attr('href')某一页面下的全部可链接：
varurl =var$ = cheerio.load(data);
    $("a.downbtn").each(function(i, e){console.log($(e).attr("href"));
    });
    console.log("done");
  }else{
      console.log("error");
  }

