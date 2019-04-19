# easyui datagrid 表格组件列属性formatter和styler使用方法 - BlueSky - CSDN博客
2016年01月15日 19:45:58[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：3570
#### 明确单元格DOM结构
要想弄清楚formatter和styler属性是怎么工作的，首先要弄清楚datagrid组件内容单元格的DOM接口，注意，这里指的是内容单元格，不包括标题单元格，标题单元格的结构有所区别。我们所有内容单元格的默认DOM结构如下：
```
`<``td``field="code">`
`    ``<``div`
```
style="text-align:left"
 class="datagrid-cell datagrid-cell-c1-code">
```
`         ``文字`
`    ``</``div``>`
`</``td``>`
```
很简单，其实只是两层结构，td标签的field属性便是字段的编码，而单元格内容统一用一个div包裹起来，在我举的这个例子中，div标签有个 text-align样式，这个样式其实是由列属性align决定的，同时div标签的class属性值并不是一个定值，需要注意一下。
#### 理解formatter是什么
顾名思义，formatter是格式化的意思，也就是以何种形式呈现的意思，对于一个纯文本，我可以将它呈现为checkbox，也可以呈现为 input输入框，甚至下拉框等等，或者是在文本外层包裹更多的DOM(当然包裹这样做并没有多大意义)，这就是formatter的真正意义。
定义示例:
```
```java
formatter:
```
```java
function
```
```java
(value,
 row, index){
```
```java
```
```java
if
```
```java
(value
 ==
```
```java
"007"
```
```java
)
 {
```
```java
```
```java
return
```
```java
'<font
 color="red">'
```
```java
+
 value +
```
```java
'</font>'
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
else
```
```java
{
```
```java
```
```java
return
```
```java
value;
```
```java
```
```java
}
```
```java
}
```
```
使用formatter需要注意以下几点：
- 无论formatter出何种形式，格式化出的DOM一定都是被包含在默认的div标签内
- 在写formatter函数时要保证有值返回，否则单元格没有内容可展示，所以if的时候别忘了else
- formatter函数不会作用在列属性checkbox为true的单元格上，checkbox列是组件预留的。
#### 理解styler是什么
顾名思义，styler是样式的意思，听起来跟formatter容易混淆，其实它只是利用jQuery的css函数修改默认td标签的样式，所以styler属于低能儿，只能定义单元格的背景色等，而且往往被默认的div标签样式覆盖。
定义示例：
```
```java
styler:
```
```java
function
```
```java
(value,
 row, index){
```
```java
```
```java
if
```
```java
(value
 ==
```
```java
"007"
```
```java
)
 {
```
```java
```
```java
return
```
```java
'background-color:blue;'
```
```java
;
```
```java
```
```java
}
```
```java
}
```
```java
<br><br>
```
```
```
```java
<br><br>
```
```
