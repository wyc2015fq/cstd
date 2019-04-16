# Java学习笔记27：模板Velocity使用方法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年01月17日 14:07:26[initphp](https://me.csdn.net/initphp)阅读数：5302
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- Velocity是什么？


> 
Velocity是一个基于java的模板引擎（template engine），它允许任何人仅仅简单的使用模板语言（template language）来引用由java代码定义的对象。




- 如何配置？

> 
在servlet-context.xml中增加以下内容，如果有jsp的配置先注释掉：



```
<beans:bean id="velocityConfig"
<span style="white-space:pre">	</span>class="org.springframework.web.servlet.view.velocity.VelocityConfigurer">
	<beans:property name="resourceLoaderPath" value="/WEB-INF/views" />
	<beans:property name="configLocation" value="classpath:common/velocity.properties" />
</beans:bean>

<beans:bean id="velocityViewResolver"
	class="org.springframework.web.servlet.view.velocity.VelocityViewResolver">
	<beans:property name="suffix" value=".htm" />
</beans:bean>
```


在resources/common目录下创建velocity.properties：


```
#encoding
input.encoding  =UTF-8
output.encoding=UTF-8
contentType=text/html;charset=UTF-8

#autoreload when vm changed
file.resource.loader.cache=false
file.resource.loader.modificationCheckInterval  =1
velocimacro.library.autoreload=false
```


- 模板语法使用：
- 设置变量

```
#set($directoryRoot="www")
#set($templateName="index.vm")
#set($template="$directoryRoot/$templateName")
```


- 输出变量

```
#set($username="zhuli")
$username
${username} //输出变量，一般最好带上{}
```


- 变量赋值

```
#set($obj = "zhuli")
${obj}
$!{objd} //如果加上感叹号，变量不存在的情况，会隐藏不会显示，推荐使用
```


- 循环语句

```
#set( $array = ["one", "two", "three"] )
#foreach ($element in $array)
元素：$!{element}<br/>
#end
```


```
#foreach( $key in $allProducts.keySet() )
    <li>Key: $key -> Value: $allProducts.get($key)</li>
    #end

    #foreach( $customer in $customerList )
    <tr><td>$velocityCount</td><td>$customer.Name</td></tr>
    #end
```


- 条件语句


```
#if($!errorMessage)
	可能的原因是：xxxx
#end
```


- 注释


```
##单行注释

 #*
多行注释
  *#
```

- 关系运算符


```
#if ($foo && $bar)
   YES
#end
```


- 宏的用法


```
#macro (showtTest $errorMessage)
    #if ($errorMessage)
		<p style="color:#FF0000">$!errorMessage</p>
    #end
#end
#showtTest($errorMessage)
```


- 引入文件


```
#parse("header.vm") //只能引单个文件
#include("header.vm", "header.vm") //可以引入多个文件
```


- \转义符号

```
#set($email = "foo" )
\${email} //输出“${email}”
```





