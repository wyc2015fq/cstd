# 网站开发之HTML基础表格Table和表单Form（三） - 杨秀璋的专栏 - CSDN博客





2016年10月03日 01:00:55[Eastmount](https://me.csdn.net/Eastmount)阅读数：8017
所属专栏：[HTML网站前端设计](https://blog.csdn.net/column/details/13444.html)









前面两篇文章主要介绍了HTML的基础知识，详见： 
[网站开发之HTML入门知识及常用标记符 （一）](http://blog.csdn.net/eastmount/article/details/51971638)
[网站开发之HTML基础知识及超链接（二）](http://blog.csdn.net/eastmount/article/details/52717292)
这篇文章主要介绍HTML两个比较重要的基础知识：
- 表格Table及简单的网页布局
- 表单Form及Input控件
- 简单介绍HTML5新增Input控件

参考：W3School内容、北理学习网站时的课件及自己上课的内容。

## **一. 表格Table及简单的网页布局**

### **简单例子**

table标签定义 HTML 表格。简单的 HTML 表格由 table 元素以及一个或多个 tr、th 或 td 元素组成。
- 表格标记符table
- 表格标题 caption
- 表格行 tr （table row）
- 表格数据 td （table data）
- 表格表头 th （table heading）

举个简单例子：

```
<html>
<head>
      <title>HTML 表格</title>
</head>
<body>
     <table border=“1”>
         <tr>  
            <th>雇员</th>  
            <th>税前工资</th>  
            <th>税后工资</th>   
         </tr>  
         <tr>
            <td>阿星</td>
            <td>6000</td>
            <td>5600</td>
        </tr>
        <tr>
            <td>三毛</td>
            <td colspan=”2”>2500</td>
        </tr>
     </table>
</body>
</html>
```

输出如下图所示，通过th设置表头（通常第一行），td设置单元格。

![](https://img-blog.csdn.net/20161002225110056)

### **表格的构造**
- 在 TH 或 TD 标记符中使用 rowspan 属性进行行合并
- 在 TH 或 TD 标记符中使用  colspan 属性进行列合并

例如：rowspan=2表示合并两行，colspan=3表示合并三列。

```
<HTML>
<HEAD><TITLE>合并单元格示例</TITLE></HEAD>
<BODY>
<TABLE border=1 >
<CAPTION><H2>学生情况表</H2></CAPTION>
<TR>                             <!-第一行-->
  <TH rowspan = 2>学号</TH>
  <TH colspan = 3>个人信息</TH>
  <TH colspan = 2>入学信息</TH>
<TR>                             <!-第二行-->
  <TH>姓名<TH>性别<TH>年龄<TH>班级<TH>入学年月
<TR>                             <!-第三行-->
  <TD>007<TD>东方不败<TD>不详<TD>19<TD>888888<TD>2001年9月
<TR>                             <!-第四行-->
  <TD>008<TD>任我行<TD>男<TD>20<TD>888888<TD>2001年9月
</TABLE>
</BODY>
</HTML>
```

输出如下图所示，改图设置标题挺难的，用到的时候可以参考：

![这里写图片描述](https://img-blog.csdn.net/20161002230729163)

### **表格对齐**
- 表格的页面对齐
- 表格内容的对齐：水平对齐(align)、垂直对齐(valign)

### **边框与分隔线**
- Frame属性
- Rules属性
- Border属性

这是一个非常重要的例子，尤其是绘制三线表格的时候，其中：border=”4”表示边框宽度为 4 像素，align=”center”表示内容居中显示，frame=”hsides”规定边框那个部分可见， rules=”rows”规定内侧边框的哪个部分是可见的。 

详见：[http://www.w3school.com.cn/tags/tag_table.asp](http://www.w3school.com.cn/tags/tag_table.asp)

```
<HTML>
<HEAD><TITLE>表格的边框和分隔线示例</TITLE></HEAD>
<BODY>
    <TABLE border="4" frame="hsides" rules="rows" align="center"> 
    <!-边框宽度为 4 像素，仅显示上下边框和横向分隔线-->
    <CAPTION><H3>我的日程表<H3></CAPTION>
    <TR>
        <TH>星期一<TH>星期二<TH>星期三<TH>星期四<TH>星期五
    <TR>
        <TD> MUD<TD>NBA2001<TD>联众军棋<TD>联众军棋<TD>笑傲江湖
    <TR>
        <TD>DiabloII<TD> DiabloII<TD>联众军棋<TD>升级<TD>MUD
    <TR>
        <TD>MUD<TD>睡觉<TD>学习<TD>打篮球<TD>打篮球
    </TABLE>
</BODY>
</HTML>
```

输出如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20161002231724450)
### **控制单元格空白**
- Cellspacing属性：属性规定单元格之间的空间。
- Cellpadding属性：属性规定单元边沿与其内容之间的空白。

例子如下所示，建议HTML代码中< /TR>和< /TD>都补齐。

```
<html>
<head>
      <title>HTML 表格</title>
</head>
<body>
     <TABLE border cellpadding="10">
         <TR>
             <TD>大话西游<TD>大内密探008<TD>少林足球
         <TR>
             <TD>鹿鼎记<TD>喜剧之王<TD>九品芝麻官
         <TR>
             <TD>逃学威龙<TD>食神<TD>百变金刚
     </TABLE>
</body>
</html>
```

输出如下图所示，设置单元格与空白内容之间距离10。

![这里写图片描述](https://img-blog.csdn.net/20161002232541562)

### **Table布局**

通常通过DIV或TABLE来对页面进行布局，如母校北理网站。

![这里写图片描述](https://img-blog.csdn.net/20161002232825497)

比如前面课程介绍的DIV布局，设置align=”center”内容居中，比如李白的自画像+静夜思，但是现在想布局成左边字画像，右边静夜思，就可以通过table来实现。 

这段代码相当于：

```
<table width="935"  border="0" cellpadding="0" cellspacing="0">
  <tr>
    <td height="100" colspan="2" bgcolor="#33CCCC">&nbsp;</td>
  </tr>
  <tr>
    <td width="750" height="400" bgcolor="A41C8A">&nbsp;</td>
    <td width="185" height="400" bgcolor="#999999">&nbsp;</td>
  </tr>
  <tr>
    <td height="100" colspan="2" bgcolor="#33CCCC">&nbsp;</td>
  </tr>
</table>
```

输出如下所示，第一行为网站的标题图片（如贵州财经大学LOGO图），同时合并单元格两列；第二部分左边李白的图片，右边为李白的诗；第三行为版权所有等信息。
|| |
|----|----|
|||
|| |




### **表格背景的设置**

它们的基本用法和body一样，这里不再详细介绍。
- Bgcolor设置背景色
- Background设置背景图像

复杂的网页布局可能需要借助嵌套表格。但是，嵌套表格一般不推荐使用，因为它会导致页面的混乱，尤其会导致可用性问题。

只使用最基本的表格标记符（table, tr, td），保持简单和轻量级（尽量不使用嵌套表格）。 

后面会介绍使用样式表进行格式设置。保持布局表格简单明了的要点是只使用表格建立基本的框架，而用样式表进行所有的格式设置。

## **二. 表单Form及Input控件**

### **什么是表单**

表单是用于实现网页浏览者与服务器之间信息交互的一种页面元素，它由表单控件和一般内容组成。 例如：注册表单、登录表单。

![这里写图片描述](https://img-blog.csdn.net/20161002234622319)

### **表单包含属性**

FORM包含所有表单内容
- Action 属性 
- 服务器端脚本程序（ASP、JSP、PHP、Perl等）
- mailto:mailbox@mailserver.com

- Method属性（get、post）
- Enctype属性

表单控件的类型
- 文本框
- 复选框
- 单选框
- 按钮
- 选项菜单
- ……




### **表单控件**

这些控件通常都是input，设置type属性，可以得到不同的控件。同时所有表单需要填写到< form>和< /from>空格之间。 

 详见：[http://www.w3school.com.cn/tags/tag_input.asp](http://www.w3school.com.cn/tags/tag_input.asp)
- 
文本：单行文本框，可见。
`<input type="text" size="">`
- 
密码：口令框，不可见。
`<input type="password" size="">`
- 
单选框： 性别，男、女。
`<input type="radio" name="sex" checked>`- 注意：单选框 name 属性相同者为一组！例如name=”sex”，男女才能实现单选，否则两个都能选中。

- 
复选框：兴趣，吃饭、睡觉、打豆豆。checked表示选中当前值。
`<input type="checkbox" checked>`
- 
选项菜单：下拉菜单，出生年月日。
- SELECT标记符：Size属性、Multiple属性
- OPTION标记符

- 设置表单控件的标签

```
<LABEL for="controlID">标签文本< /LABEL>
<INPUT id="controlID" >
```
- 提交按钮：建议使用submit。

```
<input type="submit" value="点我试试" />
<input type="image" />
```
- 重置按钮：通常填写表单，如果填写错误，点击该按钮，清空所有内容。
`<input type="reset" value="清空内容" >`- 自定义按钮
` <input type="button">`
例子，详见这个代码，这是我上课讲述的内容。

```
<html>
<head>
        <title>HTML FORM</title>
</head>
<body >
    <div align="center">
    <h1>注册界面</h1>
    <hr width="80%" size="3"  />
        <form action="3.html" method="get">

        用户名: <input type="text" /> 
        <br /><br />
        密码: <input type="password" /> 
        <br /><br />
        性别: 男 <input type="radio" name="sex"/> 女 <input type="radio" name="sex"/> 
        <br /><br />
        兴趣： 吃饭 <input type="checkbox"> 
               睡觉 <input type="checkbox">
               打豆豆 <input type="checkbox">
        <br /><br />
        出生日期：
                  <select>
                      <option>70后</option>
                      <option>80后</option>
                      <option>90后</option>
                      <option>00后</option>
                  </select>
                  年份
                 <select>
                  <option>一月</option>
                  <option>二月</option>
                  <option>三月</option>
                  <option>四月</option>
                  </select>
                  月份
       <br /><br />
       <input type="submit" value="你点一下试试" /> 
       <input type="reset" value="有本事你就删除" />
       </form>
     </div>
</body>
</html>
```

输出如下图所示：

![这里写图片描述](https://img-blog.csdn.net/20161003000853003)

需要注意两个地方：
- input控件必须在form表单控件中。
- form控件action属性设置提交的页面，如”success.html”成功登陆界面或主页，通常和数据库后台一起执行。
- form控件中method属性包括get和post两种提交方法，其中getc提交参数可见，通常通过&连接，一个参数接着一个；post更加安全，参数不可见。

## **三. 简单介绍HTML5新增Input控件**

今天刚看到一张图片，介绍2016年最热门语言，其中JavaScript排名第一，感谢大神stormzhangV的分享。 

参考：[2016 年最受欢迎的编程语言是什么？](http://blog.csdn.net/googdev/article/details/52575079)

详见图片：过去一年在 GitHub 上最流行的开源项目

![这里写图片描述](https://img-blog.csdn.net/20161003001458229)

详见图片：最受欢迎的编程语言

![这里写图片描述](https://img-blog.csdn.net/20161003001643466)

> 
这里主要看到一篇很不错的文章，介绍HTML5的新增INPUT控件，因为最近推荐我前端的学生都在看这篇文章，这里就进行简单的补充，转过来给大家学习。 

  —— 原文： [[ HTML5 新的 Input 类型 - 神舟龙 · 博客园 ]](http://www.cnblogs.com/shenzhoulong/archive/2011/02/13/1952919.html)


HTML5 拥有多个新的表单输入类型。这些新特性提供了更好的输入控制和验证。

介绍这些新的输入类型：
- email
- url
- number
- range
- Date pickers (date, month, week, time, datetime, datetime-local)
- search
- color
- telephone
- color

**EMAIL**
`<input type="email" name="email" />`
在提交表单时会自动验证email的格式,opera浏览器中必须有name属性，否则不起作用。 

效果：

![这里写图片描述](https://img-blog.csdn.net/20161003003541102)

**URL**
`<input type="url"  />`
在提交表单时会自动验证url的格式。

**NUMBER**
`<input type="number" max="9" min="0" step="2"  />`
可以限制输入的数字，step为上一个数字与下一个数字的间隔。 

效果：  
![这里写图片描述](https://img-blog.csdn.net/20161003003800981)
**RANGE**
`<input type="range"  min="1" max="10"  />`
滑动条，能过选择性的对限制范围内的数值进行设置。 

效果： 
![这里写图片描述](https://img-blog.csdn.net/20161003003837935)
**Date Pickers（数据检出器）**

HTML5 拥有多个可供选取日期和时间的新输入类型：
- date - 选取日、月、年
- month - 选取月、年
- week - 选取周和年
- time - 选取时间（小时和分钟）
- datetime - 选取时间、日、月、年（UTC 时间）
- datetime-local - 选取时间、日、月、年（本地时间）

This is how Opera renders 
`<input type="date" />`
If you need a time to go with that date, Opera also supports 
`<input type="datetime" />`
If you only need a month + year (perhaps a credit card expiration date), Opera can render a 
`<input type="month" />`
Less common, but also available, is the ability to pick a specific week of a year with 
`<input type="week" />`
Last but not least, you can pick a time with 
`<input type="time"/>`
地址：[http://diveintohtml5.org/forms.html](http://diveintohtml5.org/forms.html)

以上都是从关于时间的都是网上找到，自己做的是这样的

![这里写图片描述](https://img-blog.csdn.net/20161003004120533)

其中图片中显示的UTC为世界统一时间 

**SEARCH**

```
<input id="search" type="url" list="searchlist" required />
<datalist id="searchlist">
                <option value="http://www.google.com" label="Google" />
                <option value="http://www.yahoo.com" label="Yahoo" />
                <option value="http://www.bing.com" label="Bing" />
</datalist>
```

这里的datalist相当于下拉列表，可以进行选择 

效果： 
![这里写图片描述](https://img-blog.csdn.net/20161003004215644)
**TELEPHONE**
`<input type="telephone" />`
可输入一个电话号码 

效果： 
![这里写图片描述](https://img-blog.csdn.net/20161003004249488)
**COLOR**
`<input type="color"/>`
可以获取颜色 

效果： 
![这里写图片描述](https://img-blog.csdn.net/20161003004318332)
以上个人图片效果显示均来自opera浏览器 ，面图片时各主流浏览器对input的支持情况：

![这里写图片描述](https://img-blog.csdn.net/20161003004358973)

input标签表单类型的显示情况：

![这里写图片描述](https://img-blog.csdn.net/20161003004424520)

再次推荐大家去原文学习”神舟龙”的文章。 
[http://www.cnblogs.com/shenzhoulong/archive/2011/02/13/1952919.html](http://www.cnblogs.com/shenzhoulong/archive/2011/02/13/1952919.html)




## **四. 总结**

同时HTML5的效果确实比较好，所以在课堂中，我是这样推荐学生学习的。
- 学习HTML基础知识；
- 重点研究CSS布局及美化网站；
- 学习HTML5+CSS知识；
- 学习Javascript、JS，包括框架Bootstrap等。

下面是某个大神在群里简单推荐前端入门学习的相关知识：
- HTML、js、css必备。对于大部分前端开发者，js是重中之重。（最近重点是es6）;
- 如果涉及服务端开发的话，nodejs相关的东东（js是基础）;
- 重要的周边知识：计算机网络（http协议，tcp/ip）、服务端知识（权限/文件系统/安全策略/进程管理…）、数据库、操作系统等；
- 各种框架、库：jQuery、react、angular、vue、RN、weex…（整天更新换代）。

总之，希望文章对你有所帮助，尤其是刚入门学习HTML基础知识的大一或大二学生，同时也包括我的学生，希望你们好好学习这些基础知识，这篇文章是我在国庆节期间，非常用心的在办公室完成的，结合以前自己学习的内容、教材及自己上课的知识完成，从下午一直写到了晚上1点，还是挺辛苦的。

虽然后面没有你们的课程了，是另外一个老师上，但是有时间我会接着补充的，尤其是CSS和JavaScript重点知识，也会补充博客。 

（By：Eastmount 2016-10-03 半夜1点 [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）











