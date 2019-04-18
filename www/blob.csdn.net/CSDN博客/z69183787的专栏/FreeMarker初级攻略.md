# FreeMarker初级攻略 - z69183787的专栏 - CSDN博客
2015年07月31日 10:12:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：695
1. 备忘单
字符串："Foo" 或者 'Foo' 或者 "It's \"quoted\"" 或者 r"C:\raw\string"
 数字：123.45 <#list[BR] animals as[BR] being[BR] >[BR] ${being.name} for ${being.price} Euros[BR] </#list >
 布尔值：true, false
 序列：["foo", "bar", 123.45], 1..100
 哈希表：{"name":"green mouse", "price":150}
 检索变量
 顶层变量：user
 从哈希表中检索数据：user.name, user[“name”]
 从序列中检索：products[5]
 特殊变量：.main
 字符串操作
 插值（或连接）："Hello ${user}!"（或"Free" + "Marker"）
 获取一个字符：name[0]
 序列操作
 连接：users + ["guest"]
 序列切分：products[10..19] 或 products[5..]
 哈希表操作
 连接：passwords + {"joe":"secret42"}
 算数运算: (x * 1.5 + 10) / 2 - y % 100
 比较运算：x == y, x != y, x < y, x > y, x >= y, x <= y, x < y, 等等
 逻辑操作：!registered && (firstVisit || fromEurope)
 内建函数：name?upper_case
 方法调用：repeat("What", 3)
 处理不存在的值
 默认值：name!"unknown" 或者(user.name)!"unknown" 或者name! 或者 (user.name)!
 检测不存在的值：name?? 或者(user.name)??
2.字符串操作
插值（或连接）
${...}   ${"${user}${user}${user}${user}"}
${user[0]}  等同 user?string[0]
3.序列操作
<#list ["Joe", "Fred"] + ["Julia", "Kate"] as user>
- ${user}
</#list>
序列切分
[firstindex..lastindex]    。如果seq存储序列"a", "b", "c", "d", "e", "f"，那么表达式seq[1..4]将会是含有"b", "c", "d", "e"的序列  ，从FreeMarker 2.3.3版本以后lastindex才能省略。
4.哈希表操作
<#assign ages = {"Joe":23, "Fred":25} + {"Joe":30, "Julia":18}>
- Joe is ${ages.Joe}
- Fred is ${ages.Fred}
- Julia is ${ages.Julia}
打印
- Joe is 30
- Fred is 25
- Julia is 18
5.算数运算
算数运算包含基本的四则运算和求模运算，运算符有：
 加法：+
<#assign ages = {"Joe":23, "Fred":25} + {"Joe":30, "Julia":18}>
- Joe is ${ages.Joe}
- Fred is ${ages.Fred}
- Julia is ${ages.Julia}
- Joe is 30
- Fred is 25
- Julia is 18
 减法：-
 乘法：*
 除法：/
 求模（求余）：%
${100 – x*x}
${x/2}
${12%10}
+号，它可用来连接字符串的  ${3 + "5"} --》 35    FreeMarker不会自动将字符串转换为数字，反之会自动进行
${(x/2)?int}   计算结果的整数
比较运算
<#if user = "Big Joe">
It is Big Joe
</#if>
<#if user != "Big Joe">
It is not Big Joe
</#if>
<#if (x > y)>   等同  <#if x > y>
可以使用lt代替<，lte代替<=，gt代替>，gte代替>=
常用的逻辑操作符：
 逻辑或：||
 逻辑与：&&
 逻辑非：!
6.内建函数
字符串使用的内建函数：
 html: 字符串中所有的特殊HTML字符都需要用实体引用来代替（比如<代替<）。
 cap_first:字符串的第一个字母变为大写形式
 lower_case:字符串的小写形式
 upper_case:字符串的大写形式
 trim:去掉字符串首尾的空格
序列使用的内建函数：
 size：序列中元素的个数
数字使用的内建函数：
 int:数字的整数部分（比如-1.9?int就是-1）
7.方法调用
${repeat("What", 3)}   打印WhatWhatWhat
8.处理不存在的值     FreeMarker 2.3.7版本以后（用来代替内建函数default，exists和if_exists）。
这会出现undefinded
9.默认值
unsafe_expr!default_expr或unsafe_expr!或(unsafe_expr)!default_expr或(unsafe_expr)!
这个操作符允许你为可能不存在的变量指定一个默认值或空值空序列空哈希。
建议使用(unsafe_expr)!default_expr或(unsafe_expr)!  不管多少层里面多少操作，出错就变默认值
允许其中表达式的任意部分可以未定义
如 (something.product.color)!"red"     something为null也默认为red
10.检测不存在的值
unsafe_expr??或(unsafe_expr)??
11.操作符优先级规则 同java javascript
12.插值仅仅可以在两种位置使用：
文本区（如<h1>Hello ${name}!</h1>）和字符串表达式（如<#include "/footer/${company}.html">）中
如果插值在文本区（也就是说，不再字符串表达式中），如果escapse指令起作用了，即将被插入的字符串会被自动转义。如果你要生成HTML，那么强烈建议你利用它来阻止跨站脚本攻击和非格式良好的HTML页面。
<#escape x as x?html>
...
<p>Title: ${book.title}</p>
<p>Description: <#noescape>${book.description}</#noescape></p>
<h2>Comments:</h2>
<#list comments as comment>
<div class="comment">
${comment}
</div>
</#list>
...
</#escape>
美式数字格式  ${product.id?c}   给机器看的
日期/时间插入指南
date_format，time_format和datetime_format的setting   
布尔值插入指南
${married?string("yes", "no")}     后面的会出错${a == 2}
精确的转换规则
对于有兴趣研究的人，表达式的值转换为字符（仍有变量不存在的可能）串精确的规则就是下面这些，以这个顺序进行：
1. 如果这个值是数字，那么它会按照指定的number_format设置规则来转换为字符串。所以这些转换通常是对用户进行的，而不是对计算机。
2. 如果这个值是日期，时间或时间日期类型的一种，那么它们会按照指定的time_format，date_format和datetime_format设置规则来转换为字符串，这要看日期信息中是只包含时间，日期还是全包括了。如果它不能被探测出来是哪种日期类型（日期或时间或日期时间）时，就会发生错误了。
3. 如果值本来就是字符串类型的，不需要转换。
4. 如果FreeMarker引擎在传统兼容模式下：
1. 如果值是布尔类型，那么就转换成”true”，false值将会转换为空串。
2. 如果表达式未被定义（null或者变量未定义），那么就转换为空串。
3. 否则就会发生错误中止模板执行。
5. 否则就会发生错误中止模板执行。
序列的索引
<#if (secondGoodsCategoryT3??)&&(secondGoodsCategoryT3?size>0) >
        <#list secondGoodsCategoryT3 as second3>  
        <#if  second3_index == 0>
        <a href="${basepath}/${second3.htmlpath}}" target="_blank">${second3.name}</a>      
       </#if> 
        <#if second3_has_next>
        | 
        </#if>                                     
        </#list>
        </#if> 
