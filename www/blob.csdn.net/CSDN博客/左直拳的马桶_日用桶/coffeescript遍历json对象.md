# coffeescript遍历json对象 - 左直拳的马桶_日用桶 - CSDN博客
2014年11月20日 18:23:13[左直拳](https://me.csdn.net/leftfist)阅读数：3931
直接给代码：
```python
headers = a:"this is a"
  ,b:"this is b"
  ,c:"this is c"
exheaders = e : "this is e",c:"this is c"
headers[key] = value for key,value of exheaders
alert "key:#{key},value:#{value}" for key,value of headers
for i in headers
  headers[i] = exheaders[i]
```
这个例子中，有两个JSON对象：headers，exheaders。遍历的方法为：
for key,value of ...
以上代码编译成javascript为：
```java
var exheaders, headers, key, value;
headers = {
  a: "this is a",
  b: "this is b",
  c: "this is c"
};
exheaders = {
  e: "this is e",
  c: "this is c"
};
for (key in exheaders) {
  value = exheaders[key];
  headers[key] = value;
}
for (key in headers) {
  value = headers[key];
  alert("key:" + key + ",value:" + value);
}
```
从中也可以看到javascript遍历json的方法。使用for(var i = 0; i < json对象.length;i++)的方法是行不通的，因为json对象没有length的属性
所以，coffeescript下，遍历json对象的方法不能写成：
for i in headers
  headers[i] = exheaders[i]
它会编译成：
```java
for (_i = 0, _len = headers.length; _i < _len; _i++) {
  i = headers[_i];
  headers[i] = exheaders[i];
}
```

