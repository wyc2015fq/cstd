# Js 中 正则表达式匹配 str  match 与 exec 的区别 - z69183787的专栏 - CSDN博客
2012年10月31日 14:57:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1527
个人分类：[Javascript-概述																[正则表达式](https://blog.csdn.net/z69183787/article/category/2175421)](https://blog.csdn.net/z69183787/article/category/2175033)
Javacript exec方法：
## 定义和用法
exec() 方法用于检索字符串中的正则表达式的匹配。
### 语法
RegExpObject.exec(string)|参数|描述|
|----|----|
|string|必需。要检索的字符串。|
### 返回值
返回一个数组，其中存放匹配的结果。如果未找到匹配，则返回值为 null。
### 说明
exec() 方法的功能非常强大，它是一个通用的方法，而且使用起来也比 test() 方法以及支持正则表达式的 String 对象的方法更为复杂。
如果 exec() 找到了匹配的文本，则返回一个结果数组。否则，返回 null。此数组的第 0 个元素是与正则表达式相匹配的文本，第 1 个元素是与 RegExpObject 的第 1 个子表达式相匹配的文本（如果有的话），第 2 个元素是与 RegExpObject 的第 2 个子表达式相匹配的文本（如果有的话），以此类推。除了数组元素和 length 属性之外，exec() 方法还返回两个属性。index
 属性声明的是匹配文本的第一个字符的位置。input 属性则存放的是被检索的字符串 string。我们可以看得出，在调用非全局的 RegExp 对象的 exec() 方法时，返回的数组与调用方法 String.match() 返回的数组是相同的。
但是，当 RegExpObject 是一个全局正则表达式时，exec() 的行为就稍微复杂一些。它会在 RegExpObject 的 lastIndex 属性指定的字符处开始检索字符串 string。当 exec() 找到了与表达式相匹配的文本时，在匹配后，它将把 RegExpObject 的 lastIndex 属性设置为匹配文本的最后一个字符的下一个位置。这就是说，您可以通过反复调用
 exec() 方法来遍历字符串中的所有匹配文本。当 exec() 再也找不到匹配的文本时，它将返回 null，并把 lastIndex 属性重置为 0。
## 提示和注释
**重要事项：**如果在一个字符串中完成了一次模式匹配之后要开始检索新的字符串，就必须手动地把 lastIndex 属性重置为 0。
**提示：**请注意，无论 RegExpObject 是否是全局模式，exec() 都会把完整的细节添加到它返回的数组中。这就是 exec() 与 String.match() 的不同之处，后者在全局模式下返回的信息要少得多。因此我们可以这么说，在循环中反复地调用
 exec() 方法是唯一一种获得全局模式的完整模式匹配信息的方法。
## 实例
在本例中，我们将全局检索字符串中的 W3School：
<script type="text/javascript">
var str = "Visit W3School"; 
var patt = new RegExp("W3School","g");
var result;
while ((`result = patt.exec(str)`) != null)  {
  document.write(result);
  document.write("<br />");
  document.write(`result.lastIndex`);
 }
</script>
输出：
W3School
14
Javascript match 方法：
## 定义和用法
match() 方法可在字符串内检索指定的值，或找到一个或多个正则表达式的匹配。
该方法类似 indexOf() 和 lastIndexOf()，但是它返回指定的值，而不是字符串的位置。
### 语法
stringObject.match(searchvalue)
stringObject.match(regexp)|参数|描述|
|----|----|
|searchvalue|必需。规定要检索的字符串值。|
|regexp|必需。规定要匹配的模式的 RegExp 对象。如果该参数不是 RegExp 对象，则需要首先把它传递给 RegExp 构造函数，将其转换为 RegExp 对象。|
### 返回值
存放匹配结果的数组。该数组的内容依赖于 regexp 是否具有全局标志 g。
### 说明
match() 方法将检索字符串 stringObject，以找到一个或多个与 regexp 匹配的文本。这个方法的行为在很大程度上有赖于 regexp 是否具有标志 g。
如果 regexp 没有标志 g，那么 match() 方法就只能在 stringObject 中执行一次匹配。如果没有找到任何匹配的文本， match() 将返回 null。否则，它将返回一个数组，其中存放了与它找到的匹配文本有关的信息。该数组的第 0 个元素存放的是匹配文本，而其余的元素存放的是与正则表达式的子表达式匹配的文本。除了这些常规的数组元素之外，返回的数组还含有两个对象属性。index
 属性声明的是匹配文本的起始字符在 stringObject 中的位置，input 属性声明的是对 stringObject 的引用。
如果 regexp 具有标志 g，则 match() 方法将执行全局检索，找到 stringObject 中的所有匹配子字符串。若没有找到任何匹配的子串，则返回 null。如果找到了一个或多个匹配子串，则返回一个数组。不过全局匹配返回的数组的内容与前者大不相同，它的数组元素中存放的是 stringObject 中所有的匹配子串，而且也没有 index 属性或 input 属性。
**注意：**在全局检索模式下，match() 即不提供与子表达式匹配的文本的信息，也不声明每个匹配子串的位置。如果您需要这些全局检索的信息，可以使用 RegExp.exec()。
## 实例
### 例子 1
在本例中，我们将在 "Hello world!" 中进行不同的检索：
<script type="text/javascript">
var str="Hello world!"
document.write(str.match("world") + "<br />")
document.write(str.match("World") + "<br />")
document.write(str.match("worlld") + "<br />")
document.write(str.match("world!"))
</script>
输出：
world
null
null
world!
### 例子 2
在本例中，我们将使用全局匹配的正则表达式来检索字符串中的所有数字：
<script type="text/javascript">
var str="1 plus 2 equal 3"
document.write(`str.match(/\d+/g)`)
</script>
输出：
1,2,3
本机测试：
```java
<script>
var str ='xxtest00test11test22test'
        regexp =new RegExp(/(test)(.{1})/g);
        while((result = regexp.exec(str)) !=null){
            document.write(result +'<br />');
            document.write(regexp.lastIndex+'<br />');
        }
    
            document.write(str.match(/(test)(.{1})/g));
</script>
```
result：
test0,test,0
7
test1,test,1
13
test2,test,2
19
test0,test1,test2
