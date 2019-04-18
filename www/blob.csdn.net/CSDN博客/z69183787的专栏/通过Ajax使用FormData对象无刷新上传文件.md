# 通过Ajax使用FormData对象无刷新上传文件 - z69183787的专栏 - CSDN博客
2016年09月13日 15:35:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6344
写在前面：本文说的这个方案有浏览器兼容性问题；所有主流浏览器的较新版本已经支持这个对象了，比如Chrome 7+、Firefox 4+、IE 10+、Opera 12+、Safari 5+，对兼容性比较敏感的网站慎用。
在工作中遇到了一个问题：在一个页面中，有4块内容，每块内容都包含一个图片上传功能，希望可以实现一键把这四块内容都上传上去。
我没有用插件实现上传功能，就是用的input[type=file]，因此就遇到一个问题就是：
　　①传统的form表单会导致页面刷新，无法实现上述功能
　　②把表单serialize()序列化用Ajax的方式提交，也无法把上传文件的文件流进行序列化，也不行
我现有的知识就搞不定了，只能求助网上的大神了，百度了一下，大概看了两个方案：
　　①在js中创建一个新form表单，把页面中原form表单copy一份，然后再用js搞一个iframe，把form表单的target设置为iframe，这样提交后返回的内容就在iframe里，最后再把form表单、iframe移除
　　　　该方案我没有尝试，只能说个大概；参考地址：[http://www.oschina.net/code/snippet_569983_11316](http://www.oschina.net/code/snippet_569983_11316)
　　②就是本文下面要说的使用FormData对象实现
有其他思路方案的希望不吝赐教！ 
好，介绍完背景之后，开始介绍我们今天的主题：FormData对象。
有两种方式可以创建一个FormData对象：
　　①创建一个空的FormData对象，然后使用append()方法向该对象里添加字段
　　②使用HTML表单来初始化一个FormData对象
下面分别介绍一下：
　　第一种方式：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
var oMyForm = new FormData();
oMyForm.append("username", "Groucho");
oMyForm.append("accountnum", 123456); 
oMyForm.append("file", $('#file')[0].files[0]);
$.ajax({
    url: '/Manage/UploadImg',
    type: 'POST',
    cache: false,
    data: oMyForm,
    processData: false,
    contentType: false,
    async: false
}).done(function(res) {}).fail(function(res) {});
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　第二种方式：
```
<form id="uploadForm" enctype="multipart/form-data">
    <p>指定文件名： <input type="text" name="filename" value="" /></p>
    <p>上传文件： <input type="file" name="file" /></ p>
        <input type="button" value="上传" onclick="doUpload()" />
</form>
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
var formData = new FormData($('#uploadForm')[0]);
formData.append('num', '1');//可以在已有表单数据的基础上，继续添加新的键值对
$.ajax({
    url: '/upload',
    type: 'POST',
    cache: false,
    data: new FormData($('#uploadForm')[0]),
    processData: false,
    contentType: false
}).done(function(res) {}).fail(function(res) {});
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
注意：
- Ajax的processData设置为false。因为data值是FormData对象，不需要对数据做处理。
- 第二种方式中<form>标签加enctyp　　e="multipart/form-data"属性。
- cache设置为false，上传文件不需要缓存。
- contentType设置为false。因为是由<form>表单构造的FormData对象，且已经声明了属性enctype="mutipart/form-data"，所以这里设置为false。
前端搞定之后，剩下的就是后端处理了。ok，就到这里了。
`FormData`对象，是可以使用一系列的键值对来模拟一个完整的表单，然后使用`XMLHttpRequest`发送这个"表单"。
在 **Mozilla Developer** 网站 [使用FormData对象](https://developer.mozilla.org/zh-CN/docs/Web/Guide/Using_FormData_Objects) 有详尽的`FormData`对象使用说明。
但上传文件部分只有底层的`XMLHttpRequest`对象发送上传请求，那么怎么通过`jQuery`的`Ajax`上传呢？
本文将介绍通过`jQuery`使用`FormData`对象上传文件。
## 使用`<form>`表单初始化`FormData`对象方式上传文件
HTML代码
```xml
<form id="uploadForm" enctype="multipart/form-data">
    <input id="file" type="file" name="file"/>
    <button id="upload" type="button">upload</button>
</form>
```
javascript代码
```java
```java
$.ajax({
    url: '/upload',
    type: 'POST',
    cache: false,
    data: new FormData($('#uploadForm')[0]),
    processData: false,
    contentType: false
}).done(function(res) {
}).fail(function(res) {});
```
```
这里要注意几点：
- `processData`设置为`false`。因为`data`值是`FormData`对象，不需要对数据做处理。
- `<form>`标签添加`enctype="multipart/form-data"`属性。
- `cache`设置为`false`，上传文件不需要缓存。
- `contentType`设置为`false`。因为是由`<form>`表单构造的`FormData`对象，且已经声明了属性`enctype="multipart/form-data"`，所以这里设置为false。
上传后，服务器端代码需要使用从查询参数名为`file`获取文件输入流对象，因为`<input>`中声明的是`name="file"`。
如果不是用`<form>`表单构造`FormData`对象又该怎么做呢？
## 使用`FormData`对象添加字段方式上传文件
HTML代码
```xml
<div id="uploadForm">
    <input id="file" type="file"/>
    <button id="upload" type="button">upload</button>
</div>
```
这里没有`<form>`标签，也没有`enctype="multipart/form-data"`属性。
javascript代码
```java
```java
var formData = new FormData();
formData.append('file', $('#file')[0].files[0]);
$.ajax({
    url: '/upload',
    type: 'POST',
    cache: false,
    data: formData,
    processData: false,
    contentType: false
}).done(function(res) {
}).fail(function(res) {});
```
```
这里有几处不一样：
- `append()`的第二个参数应是文件对象，即`$('#file')[0].files[0]`。
- `contentType`也要设置为‘false’。
从代码`$('#file')[0].files[0]`中可以看到一个`<input type="file">`标签能够上传多个文件，
只需要在`<input type="file">`里添加`multiple`或`multiple="multiple"`属性。
## 服务器端读文件
从`Servlet 3.0` 开始，可以通过 `request.getPart()` 或 `request.getPars()` 两个接口获取上传的文件。
这里不多说，详细请参考官网教程 [Uploading Files with Java Servlet Technology](https://docs.oracle.com/javaee/7/tutorial/servlets011.htm#BABFGCHB) 以及示例 [The fileupload Example Application](https://docs.oracle.com/javaee/7/tutorial/servlets016.htm#BABDGFJJ)
### 参考
- [https://developer.mozilla.org/zh-CN/docs/Web/Guide/Using_FormData_Objects](https://developer.mozilla.org/zh-CN/docs/Web/Guide/Using_FormData_Objects)
- [http://stackoverflow.com/questions/10292382/html-5-formdata-and-java-servlets](http://stackoverflow.com/questions/10292382/html-5-formdata-and-java-servlets)
- [https://docs.oracle.com/javaee/7/tutorial/servlets011.htm#BABFGCHB](https://docs.oracle.com/javaee/7/tutorial/servlets011.htm#BABFGCHB)
- [https://docs.oracle.com/javaee/7/tutorial/servlets016.htm#BABDGFJJ](https://docs.oracle.com/javaee/7/tutorial/servlets016.htm#BABDGFJJ)
文／Agreal（简书作者）
原文链接：http://www.jianshu.com/p/46e6e03a0d53
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
