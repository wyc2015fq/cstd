
# HTLM表单的概述 - 李昆鹏的博客 - CSDN博客


2018年05月08日 16:11:28[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：99


------------------------------------------HTLM表单的概述--------------------------------------------
**表单标签**：form
属性：
Action：表单提交的目的地，默认是当前页
Html，jsp，servlet，do，action
Method：提交方式，post，get**推荐使用post**
子标签：
Input
属性：type表单元素的类型  默认的是文本框类型
**文本框**：
Type=”text”表示一个文本框
Name=”usemame”将来用服务器取值使用
Id=”usemame”将来用于js获取值使用
Size=”40”用于控制文本框的长度
Value=”请在这里输入用户名” 是文本框的默认值，一般用于提示
readonly="readonly"是标记属性，告诉文本框我是只读的。本身没有值，
但是为了符合新的规范，属性必须有值，所以，
我们就把它的值和属性设置为一样的。可以通过表单提交获得到值
disabled=”disabled”把当前文本框变成灰色，不能修改只能看，不能通过表单来获得到值的
Ø**隐藏域：**type=”hidden”, 要设置name和value
Ø**密码框：**
type="password"表示这是一个密码框
name="password"将来用于服务器取值使用
id="password"将来用于js获取值使用
size="40"用于控制密码框的长度
value="admin"默认密码
readonly="readonly"是标记属性
Ø**单选框：**
type="radio"表示这是一个单选框
name="sex"将来用于服务器取值使用
注意：如果多个单选按钮时互斥关系，那么，就必须把这多个按钮看做一个按钮组。
一个按钮组的名字必须是一致的。
id="man"将来用于js获取值使用
value="男" 将来服务器真正获取到的值
checked="checked"是标记属性
Ø**复选框：**
type="checkbox"表示这是一个复选框
name="likes"将来用于服务器取值使用
注意：这里的name如果不一样，那么，有多少个复选框，将来服务器就要获取多少次值。
然后判断值是否是null,不是null才把值给存储起来。
如果这里的name值一样，在服务器只需要获取一次即可，这次获取的返回值是一个
字符串数组，把所有选中的都直接包含了。
StringgetValue(String name) 根据名字获取单个值
String[]getValues(String name) 根据名字获取数组
id="sleep"将来用于js获取值使用
value="睡觉" 将来服务器真正获取到的值
checked="checked"是标记属性
Ø**文件框：**
type="file"表示这是一个文件框
name="file"将来用于服务器取值使用
id="file"将来用于js获取值使用
Ø**按钮：**
type="button"表示这是一个按钮
type="submit"表示这是一个提交按钮，内置了js事件
type="reset"表示这是一个重置按钮，内置了js事件

注意：
下拉框和文本域不是input的type属性能够决定的。
Ø**select 标签**
name="province"将来用于服务器取值使用
id="province"将来用于js获取值使用
|--option标签
value="湖北省" 服务器实际获取的值
selected="selected"默认选中项
multiple：把select标签展开，可以多选
size="6"：设置select展开的行数
Ø**textarea 标签**
cols="40" 列数
rows="10" 行数
name="myself" 将来用于服务器取值使用
id="myself" 将来用于js获取值使用
不具有value的属性，值写在元素中间
**代码示例：**
<html>
<head>
<title>www.likunpeng.com</title>
</head>
<body>
<h1>HTML表单</h1>
<form action="/xxx/xxx.jsp" method="post">
<input type="hidden" name="useID" value="1">
用户：<input size="50" id="usename" type="text"name="usename"
value="请输入用户名" readonly="readonly"disabled="disabled"><br/>
<br/>密码：<input id="password" type="password"  size="50"  readonly="readonly"valu="123"><br/>
<br/>性别：<input id="man" value="1"checked="checked" type="radio" name="gender">男&nbsp;&nbsp;
<input id="woman"value="2" type="radio" name="gender">女<br/>
<br/>爱好: <input type="checkbox" checked value="1"name="favor">篮球&nbsp;&nbsp;
<input type="checkbox"value="2" name="favor">足球&nbsp;&nbsp;
<input type="checkbox"value="3" name="favor">乒乓球<br/>
<br/>头像: <input id="pic" type="file"name="pic"><br/>
<br/>工资: <select id="sal" name="sal" >
<optionvalue="">--请选择--</option>
<optionvalue="6000">6000</option>
<optionvalue="8000">8000</option>
<optionvalue="10000" selected="selected">10000</option>
</select><br/>
<br/>岗位: <select id="job" size="3"name="job" multiple>
<option value="1">中级工程师</option>
<option value="1">测试工程师</option>
<option value="1">构架师</option>
</select><br/>
<br/>个人介绍: <textarea name="introduce" disabledrows="10" cols="40">请输入...
</textarea><br/><br/>
<input name="submit" type="submit" value="提交">
<input name="button" type="button" value="按钮提交">
<input name="reset" type="reset" value="重置">
</form>
</body>
</html>
-------------------------------------------------------------
![](https://img-blog.csdn.net/20180508161109880?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




