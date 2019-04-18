# 关于jquery appendTo方法 是移动对象还是复制对象 - z69183787的专栏 - CSDN博客
2014年03月11日 07:14:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3649
## 结论：appendto 是将 匹配的元素 移动到 目标元素
在 jquery 官方网站中 发现：
```
`$( "h2" ).appendTo( $( ".container" ) );`
```
If an element selected this way is inserted into a single location elsewhere in the DOM, it will be moved into the target (not cloned) and a new set consisting of the inserted element is returned:
```
`<div class="container">`
`  <div class="inner">Hello</div>`
`  <div class="inner">Goodbye</div>`
`  <h2>Greetings</h2>`
`</div>`
```
If there is more than one target element, however, cloned copies of the inserted element will be created for each target after the first, and that new set (the original element plus clones) is returned.
**按照我的理解，如果target目标元素只有1个，那么是将需要移动的元素移动至target目标元素，移除这些需要移动的元素的原始位置；**
**如果是多个target目标元素，那么是将需要移动的元素的copy移动至target目标元素，而不移除这些需要移动的元素的原有位置。**
**但尝试后发现：无论是多个还是单个 都是 移动。。不是复制。**
```java
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>appendTo demo</title>
  <style>
  .foo {
    background: yellow;
  }
  </style>
  <script src="//code.jquery.com/jquery-1.10.2.js"></script>
</head>
<body>
 
<span>I have nothing more to say1... </span>
 <div class="foo">FOO! </div>
 
<script>
alert(($( "span" ).appendTo( ".foo" )).length);//alert  1
//$( "span" ).appendTo( ".foo" );
</script>
 
</body>
</html>
```
```java
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>appendTo demo</title>
  <style>
  .foo {
    background: yellow;
  }
  </style>
  <script src="//code.jquery.com/jquery-1.10.2.js"></script>
</head>
<body>
 <span>I have nothing more to say1... </span>
<span>I have nothing more to say1... </span>
 <div class="foo">FOO! </div>
  <div class="foo">FOO! </div>
 
<script>
alert(($( "span" ).appendTo( ".foo" )).length); //alert 4 
//$( "span" ).appendTo( ".foo" );
</script>
 
</body>
</html>
```
