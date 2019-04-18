# 使用CSS border-collapse样式 合并表格边框 - z69183787的专栏 - CSDN博客
2012年10月23日 08:58:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4261
|separate|默认值。边框会被分开。不会忽略 border-spacing 和 empty-cells 属性。|
|----|----|
|collapse|如果可能，边框会合并为一个单一的边框。会忽略 border-spacing 和 empty-cells 属性。|
|inherit|规定应该从父元素继承 border-collapse 属性的值。|
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
<style type="text/css">
table
  {
  border-collapse:collapse;
  }
table, td, th
  {
  border:1px solid black;
  }
</style>
</head>
<body>
<table>
<tr>
<th>Firstname</th>
<th>Lastname</th>
</tr>
<tr>
<td>Bill</td>
<td>Gates</td>
</tr>
<tr>
<td>Steven</td>
<td>Jobs</td>
</tr>
</table>
<p><b>注释：</b>如果没有规定 !DOCTYPE，border-collapse 属性可能会引起意想不到的错误。</p>
</body>
</html>
```
![](https://img-my.csdn.net/uploads/201210/22/1350914346_1624.jpg)
