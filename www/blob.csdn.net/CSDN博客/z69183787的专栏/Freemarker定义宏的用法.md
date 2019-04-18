# Freemarker定义宏的用法 - z69183787的专栏 - CSDN博客
2015年07月17日 17:55:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：864
Freemarker宏 
一.什么是宏:macro 
引用
宏是在模板中使用macro指令定义 
基本用法 
宏是和某个变量关联的模板片断，以便在模板中通过用户定义指令使用该变量， 
Java代码  ![收藏代码](http://jbeduhai.iteye.com/images/icon_star.png)
- <#macro greet>     
-      <font size="+2">Hello Joe!</font>     
- </#macro>    
- <#macro greet>  
-       <font size="+2">Hello Joe!</font>  
- </#macro>  
调用宏时，与使用FreeMarker的其他指令类似，只是使用@替代FTL标记中的#。 
<@greet></@greet> <#--<@greet/>--> 
在macro指令中可以在宏变量之后定义参数，如： 
Java代码  ![收藏代码](http://jbeduhai.iteye.com/images/icon_star.png)
- <#macro greet person>     
-     <font size="+2">Hello ${person}!</font>     
- </#macro>    
- <#macro greet person>  
-      <font size="+2">Hello ${person}!</font>  
- </#macro>  
可以这样使用这个宏变量： 
<@greet person="Fred"/> 
但是下面的代码具有不同的意思： 
<@greet person=Fred/> 
这意味着将Fred变量的值传给person参数，该值不仅是字符串，还可以是其它类型，甚至是复杂的表达式。 
宏可以有多参数，下面是一个例子： 
Java代码  ![收藏代码](http://jbeduhai.iteye.com/images/icon_star.png)
- <#macro greet person color>     
-  <font size="+2" color="${color}">Hello ${person}!</font>     
- </#macro>    
-  <#macro greet person color>  
-     <font size="+2" color="${color}">Hello ${person}!</font>  
-   </#macro>  
可以这样使用该宏变量，其中参数的次序是无关的： 
引用
  <@greet person="Fred" color="black"/>
引用
    可以在定义参数时指定缺省值，否则，在调用宏的时候，必须对所有参数赋值： 
<#macro greet person color="black"> 
    <font size="+2" color="${color}">Hello ${person}!</font> 
  </#macro> 
注意：宏的参数是局部变量，只能在宏定义中有效。
嵌套内容 
FreeMarker的宏可以有嵌套内容，<#nested>指令会执行宏调用指令开始和结束标记之间的模板片断，举一个简单的例子： 
引用
  <#macro border> 
      <table border=4 cellspacing=0 cellpadding=4><tr><td> 
        <#nested> 
      </tr></td></table> 
  </#macro>
执行宏调用： 
引用
<@border>The bordered text</@border> 输出结果： 
<table border=4 cellspacing=0 cellpadding=4><tr><td> 
      The bordered text 
  </tr></td></table> 
<#nested>指令可以被多次调用，每次都会执行相同的内容。 
<#macro do_thrice> 
     <#nested> 
     <#nested> 
     <#nested> 
   </#macro> 
   <@do_thrice> 
     Anything. 
   </@do_thrice> 
FMPP 输出结果： 
Anything. 
   Anything. 
   Anything. 
引用
嵌套内容可以是有效的FTL，下面是一个有些复杂的例子，我们将上面三个宏组合起来： 
<@border> 
    <ul> 
    <@do_thrice> 
      <li><@greet person="Joe"/> 
    </@do_thrice> 
    </ul> 
  </@border> 
输出结果： 
<table border=4 cellspacing=0 cellpadding=4><tr><td> 
    <ul> 
      <li><font size="+2">Hello Joe!</font> 
      <li><font size="+2">Hello Joe!</font> 
      <li><font size="+2">Hello Joe!</font> 
    </ul> 
   </tr></td></table> 
  <table border=4 cellspacing=0 cellpadding=4><tr><td>
引用
宏定义中的局部变量对嵌套内容是不可见的，例如： 
<#macro repeat count> 
    <#local y = "test"> 
    <#list 1..count as x> 
      ${y} ${count}/${x}: <#nested> 
    </#list> 
  </#macro> 
  <@repeat count=3>${y?default("?")} ${x?default("?")} ${count?default("?")}</@repeat> 
输出结果： 
test 3/1: ? ? ? 
  test 3/2: ? ? ? 
  test 3/3: ? ? ?
引用
在宏定义中使用循环变量 
nestted指令也可以有循环变量（循环变量的含义见下节），调用宏的时候在宏指令的参数后面依次列出循环变量的名字，格式如下： 
<@ macro_name paramter list; loop variable list[,]> 
例如： 
<#macro repeat count> 
    <#list 1..count as x> 
      <#nested x, x/2, x==count> 
    </#list> 
  </#macro> 
  <@repeat count=4 ; c, halfc, last> 
    ${c}. ${halfc}<#if last> Last!</#if> 
  </@repeat> 
这里count是宏的参数，c, halfc,last则为循环变量，输出结果： 
1. 0.5 
  2. 1 
  3. 1.5 
  4. 2 Last!
引用
循环变量和宏标记指定的不同不会有问题，如果调用时少指定了循环变量，那么多余的值不可见。调用时多指定了循环变量，多余的循环变量不会被创建： 
<@repeat count=4 ; c, halfc, last> 
    ${c}. ${halfc}<#if last> Last!</#if> 
  </@repeat> 
  <@repeat count=4 ; c, halfc> 
    ${c}. ${halfc} 
  </@repeat> 
  <@repeat count=4> 
    Just repeat it... 
  </@repeat>
在模版中定义变量 
在模板中定义的变量有三种类型： 
引用
plain变量：可以在模板的任何地方访问，包括使用include指令插入的模板，使用assign指令创建和替换。 
局部变量：在宏定义体中有效，使用local指令创建和替换。 
循环变量：只能存在于指令的嵌套内容，由指令（如list）自动创建；宏的参数是局部变量，而不是循环变量 
局部变量隐藏（而不是覆盖）同名的plain变量；循环变量隐藏同名的局部变量和plain变量，下面是一个例子： 
<#assign x = "plain"> 
  ${x}  <#-- we see the plain var. here --> 
  <@test/> 
    6. ${x}  <#-- the value of plain var. was not changed --> 
  <#list ["loop"] as x> 
    7. ${x}  <#-- now the loop var. hides the plain var. --> 
    <#assign x = "plain2"> <#-- replace the plain var, hiding does not mater here --> 
    8. ${x}  <#-- it still hides the plain var. --> 
  </#list> 
   9. ${x}  <#-- the new value of plain var. --> 
  <#macro test> 
   2. ${x}  <#-- we still see the plain var. here --> 
  <#local x = "local"> 
   3. ${x}  <#-- now the local var. hides it --> 
   <#list ["loop"] as x> 
   4. ${x}  <#-- now the loop var. hides the local var. --> 
   </#list> 
   5. ${x}  <#-- now we see the local var. again --> 
  </#macro> 
输出结果： 
1. plain 
  2. plain 
  3. local 
  4. loop 
  5. local 
  6. plain 
  7. loop 
  8. loop 
  9. plain2 
内部循环变量隐藏同名的外部循环变量，如： 
<#list ["loop 1"] as x> 
    ${x} 
    <#list ["loop 2"] as x> 
      ${x} 
      <#list ["loop 3"] as x> 
        ${x} 
      </#list> 
      ${x} 
    </#list> 
    ${x} 
  </#list> 
输出结果： 
loop 1 
    loop 2 
      loop 3 
    loop 2 
  loop 1 
模板中的变量会隐藏（而不是覆盖）数据模型中同名变量，如果需要访问数据模型中的同名变量，使用特殊变量global，下面的例子假设数据模型中的user的值是Big Joe： 
<#assign user = "Joe Hider"> 
   ${user}          <#-- prints: Joe Hider --> 
   ${.globals.user} <#-- prints: Big Joe -->
引用
名字空间 
通常情况，只使用一个名字空间，称为主名字空间，但为了创建可重用的宏、变换器或其它变量的集合（通常称库），必须使用多名字空间，其目的是防止同名冲突 
创建库 
下面是一个创建库的例子（假设保存在lib/my_test.ftl中）： 
  <#macro copyright date> 
    <p>Copyright (C) ${date} Julia Smith. All rights reserved. 
    <br>Email: ${mail}</p> 
  </#macro>  
  <#assign mail = "jsmith@acme.com"> 
使用import指令导入库到模板中，Freemarker会为导入的库创建新的名字空间，并可以通过import指令中指定的散列变量访问库中的变量： 
  <#import "/lib/my_test.ftl" as my> 
  <#assign mail="fred@acme.com"> 
  <@my.copyright date="1999-2002"/> 
  ${my.mail} 
  ${mail} 
输出结果： 
<p>Copyright (C) 1999-2002 Julia Smith. All rights reserved. 
    <br>Email: jsmith@acme.com</p> 
  jsmith@acme.com 
  fred@acme.com 
可以看到例子中使用的两个同名变量并没有冲突，因为它们位于不同的名字空间。还可以使用assign指令在导入的名字空间中创建或替代变量，下面是一个例子： 
<#import "/lib/my_test.ftl" as my> 
  ${my.mail} 
  <#assign mail="jsmith@other.com" in my> 
  ${my.mail} 
输出结果： 
  jsmith@acme.com 
  jsmith@other.com 
数据模型中的变量任何地方都可见，也包括不同的名字空间，下面是修改的库： 
<#macro copyright date> 
    <p>Copyright (C) ${date} ${user}. All rights reserved.</p> 
  </#macro> 
  <#assign mail = "${user}@acme.com"> 
假设数据模型中的user变量的值是Fred，则下面的代码： 
<#import "/lib/my_test.ftl" as my> 
  <@my.copyright date="1999-2002"/> 
  ${my.mail} 
输出结果： 
<p>Copyright (C) 1999-2002 Fred. All rights reserved.</p> 
  Fred@acme.com
