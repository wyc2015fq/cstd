# JavaScript之将JS代码放在什么位置最合适 - weixin_33985507的博客 - CSDN博客
2016年07月18日 14:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
1.放到<head></head>标签里面
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title></title>
    <script type="text/javascript">
       JavaScript  goes here
    </script>
</head>
<body>
</body>
</html>
```
2.将JavaScript代码存为一个扩展名为.js的独立文件。然后在<head></head>标签里面放一个<script type="text/javascript"></script>并把他的src属性指向该文件的物理地址；
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title></title>
    <script src="js/Script.js" type="text/javascript"></script>
</head>
<body>
</body>
</html>
```
3.最好的做法是:因为网页是以文档流的形式加载文件,所以为了更快的加载页面,通常是将JS外部连接代码放置到</body>标签之前
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title></title>
</head>
<body>
    <script src="js/Script.js" type="text/javascript"></script>
</body>
</html>
```
