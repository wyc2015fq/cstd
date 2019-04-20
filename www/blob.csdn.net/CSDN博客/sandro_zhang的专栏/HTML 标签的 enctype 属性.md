# HTML  标签的 enctype 属性 - sandro_zhang的专栏 - CSDN博客
2012年05月22日 09:41:45[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：442
## 定义和用法
enctype 属性规定在发送到服务器之前应该如何对表单数据进行编码。
默认地，表单数据会编码为 "application/x-www-form-urlencoded"。就是说，在发送到服务器之前，所有字符都会进行编码（空格转换为 "+" 加号，特殊符号转换为 ASCII HEX 值）。
## 实例
在下面的例子中，表单数据会在未编码的情况下进行发送：
<form action="form_action.asp" `enctype="text/plain"`>
  <p>First name: <input type="text" name="fname" /></p>
  <p>Last name: <input type="text" name="lname" /></p>
  <input type="submit" value="Submit" />
</form>
[TIY](http://www.w3school.com.cn/tiy/t.asp?f=html_form_enctype)
## 语法
<form enctype="*value*">
### 属性值
|值|描述|
|----|----|
|application/x-www-form-urlencoded|在发送前编码所有字符（默认）|
|multipart/form-data|不对字符编码。在使用包含文件上传控件的表单时，必须使用该值。|
|text/plain|空格转换为 "+" 加号，但不对特殊字符编码。|
原文：[http://www.w3school.com.cn/tags/att_form_enctype.asp](http://www.w3school.com.cn/tags/att_form_enctype.asp)
