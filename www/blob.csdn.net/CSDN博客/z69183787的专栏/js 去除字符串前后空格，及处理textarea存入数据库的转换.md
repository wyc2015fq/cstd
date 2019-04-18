# js 去除字符串前后空格，及处理textarea存入数据库的转换 - z69183787的专栏 - CSDN博客
2013年01月10日 09:49:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3568

当通过<textarea>插数据进数据的库，取出来后都变成一行变成，用这个<pre>标签能原样输入插入时的格式。
当时要对<pre>加一些CSS样式才行啦。
以下为引用的内容：
```
<style>
			pre {
				white-space: pre-wrap;       /* css-3 */
				white-space: -moz-pre-wrap;  /* Mozilla, since 1999 */
				white-space: -pre-wrap;      /* Opera 4-6 */
				white-space: -o-pre-wrap;    /* Opera 7 */
				word-wrap: break-word;       /* Internet Explorer 5.5+ */
				word-break:break-all;
				overflow:hidden;
			}
		</style>
```
该CSS方案可让pre标签内的文本自动换行，我在我有的浏览器上都测试了一下，全部支持，IE6，IE7, IE8, Firefox, Opera, Safari和Chrome。
仅当你把窗口缩小到小于20个字符的宽度的时候，才会超出边界
去除首尾空格
```java
a.replace(/(^\s*)|(\s*$)/g,'')
```
textarea处理：
页面提交时：
```java
var getValue=$("#dangersState").val();
```
var endValue=(getValue.replace(/<(.+?)>/gi,"<$1>")).replace(/\n/gi,"<br>");
从数据库获取并显示在textarea时：
```java
String.prototype.replaceAll = function(s1,s2) { 
		    return this.replace(new RegExp(s1,"gm"),s2); 
		}
```
```java
$("#dangersState").val($("#dangersState").val().replaceAll("<br>","\n"));
```
以文本显示
```java
$("#dangersState").html($("#dangersState").text());
```
```java
<td id="dangersState" colspan="3"></td>
```
