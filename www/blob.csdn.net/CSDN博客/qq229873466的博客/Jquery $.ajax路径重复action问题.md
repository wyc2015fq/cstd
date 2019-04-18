# Jquery $.ajax路径重复action问题 - qq229873466的博客 - CSDN博客

2019年03月21日 20:39:04[qq229873466](https://me.csdn.net/qq229873466)阅读数：9


```
// demo1
$.ajax({
    url : "controller/action";
});

// demo2
$.ajax({
    url : "/controller/action";
});
```

当访问xxx/controller/action时，demo1实际提交url为xxx/controller/controller/action，而demo2提交url为xxx/controller/action；

当访问xxx/controller时，demo1、demo2都提交url为xxx/controller/action。

有知道原因的朋友麻烦告诉我为什么？

