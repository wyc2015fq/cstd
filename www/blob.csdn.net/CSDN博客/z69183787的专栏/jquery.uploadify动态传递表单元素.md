# jquery.uploadify动态传递表单元素 - z69183787的专栏 - CSDN博客
2013年11月17日 10:38:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4031
**3.2版本 写法为：**
**$('#<%=fileGroupName%>').uploadify('settings', 'formData', { 'attachMemo' : $('#attachMemo').val() } );**
**j**query.[uploadify](http://www.pooy.net/category/network-programming/uploadify)动态传递表单元素
在给网站开发的时候，[璞玉](http://www.pooy.net/)需要用到uploadify这个上传插件，在使用的时候，遇到一个问题就是通过前端的上传脚本，把一个动态的数据传递到上传后台处理页面做一个参数。
看了手册之后发现有一个接口，可以使用。那就是’scriptData’,（这个是在Uploadify3.0的版本下才有，3.0以上改为formData）.
在使用uploadify时，如果使用初始化参数的方式传递参数，会发现修改过的表单元素传不到后台。
```
```java
'scriptData'
```
```java
:
 {
```
```java
'ttype'
```
```java
:document.getElementById(
```
```java
'name'
```
```java
).value},
```
```
仔细分析了一下，这里传递的参数是表单初始化的时候值，所以一定是空的，或者是默认的。
解决方法是在提交表单时，加上这么一句代码：
```
```php
<a
 href=
```
```php
"javascript:$('#uploadify').uploadifySettings('scriptData',{'ttype':document.getElementById('name').value});
 jQuery('#uploadify').uploadifyUpload()"
```
```php
>开始上传</a>
```
```
注意书写格式，不然会出错的。
如果对[uploadify](http://www.pooy.net/tag/uploadify-2)参数不是很了解的话，可以参考之前写的那篇[《Uploadify与php使用详解](http://www.pooy.net/uploadify-php.html)》
 ，里面每个参数都有注解。希望对大家有帮助！有问题可以直接留言。
