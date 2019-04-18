# jquery源码阅读知识储备(3)exec() 方法和match方法 - z69183787的专栏 - CSDN博客
2013年10月24日 15:44:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1003
 jquery中的源码：
**[javascript]**[view plain](http://blog.csdn.net/hackerhope/article/details/6174153#)[copy](http://blog.csdn.net/hackerhope/article/details/6174153#)
- quickExpr = /^(?:[^<]*(<[/w/W]+>)[^>]*$|#([/w/-]+)$)/,  
- ...  
- ...  
- if ( typeof selector === "string" ) {  
- // Are we dealing with HTML string or an ID?
-             match = quickExpr.exec( selector );//得到一个完全匹配和子匹配的数组。
- 
- ....  
- 
- .....}  
在后续代码中大量判断match，match[0],match[1]的值，而进行后续的操作。
1.match
match方法属于String正则表达方法. 
语法: str.match(regexp)
str:要进行匹配的字符串. regexp:一个正则表达式(或者由RegExp()构造成的正则表达式)
match的用法主要区分就是,正则表达式是否有全局标示g.
1)如果有g全局标志,那么返回的数组保存的是,所有匹配的内容.
2)如果没有g全局标志,那么返回的数组arr.arr[0]保存的是完整的匹配.arr[1]保存的是第一个括号里捕获的字串,依此类推arr[n]保存的是第n个括号捕获的内容. 
**[javascript]**[view plain](http://blog.csdn.net/hackerhope/article/details/6174153#)[copy](http://blog.csdn.net/hackerhope/article/details/6174153#)
- <mce:script type="text/javascript"><!--  
- str = "abbb34eftab0modabbbbb6";  
- var reg = /a(b)+(/d+)/i;// then test: var reg = /a(b)+(/d+)/ig
- var arr = str.match(reg);  
- for(var i = 0; i < arr.length; i++){  
-     document.write(arr[i] + "---->"); //根据返回结果可知如果有g全局,那么返回所有匹配[abbb34,ab0,abbbbb6]
- //如果没有g全局,那么返回数组arr[0]为匹配的完整字串后面是括号里的捕获[abbb3,b,3];
- }  
- // --></mce:script>
2.exec() 方法用于检索字符串中的正则表达式的匹配。
**exec与match的关联就是exec(g有没有都无影响)就等价于不含有g全局标志的match.即返回数组arr[0]为匹配的完整串.其余的为括号里捕获的字符串.**
**语法**
RegExpObject.exec(string)
|**参数**|**描述**|
|----|----|
|string|必需。要检索的字符串。|
**返回值**
返回一个数组，其中存放匹配的结果。如果未找到匹配，则返回值为 null。
**说明**
exec() 方法的功能非常强大，它是一个通用的方法，而且使用起来也比 test() 方法以及支持正则表达式的 String 对象的方法更为复杂。
如果 exec() 找到了匹配的文本，则返回一个结果数组。否则，返回 null。此数组的第 0 个元素是与正则表达式相匹配的文本，第1 个元素是与 RegExpObject 的第 1 个子表达式相匹配的文本（如果有的话），第 2 个元素是与 RegExpObject 的第 2 个子表达式相匹配的文本（如果有的话），以此类推。除了数组元素和 length 属性之外，exec() 方法还返回两个属性。index 属性声明的是匹配文本的第一个字符的位置。input 属性则存放的是被检索的字符串 string。我们可以看得出，在调用非全局的 RegExp对象的 exec() 方法时，返回的数组与调用方法 String.match() 返回的数组是相同的。
但是，当 RegExpObject 是一个全局正则表达式时，exec() 的行为就稍微复杂一些。它会在 RegExpObject 的 lastIndex 属性指定的字符处开始检索字符串 string。当 exec() 找到了与表达式相匹配的文本时，在匹配后，它将把 RegExpObject 的lastIndex 属性设置为匹配文本的最后一个字符的下一个位置。这就是说，您可以通过反复调用 exec() 方法来遍历字符串中的所有匹配文本。当 exec() 再也找不到匹配的文本时，它将返回 null，并把 lastIndex 属性重置为 0。
**提示和注释**
**重要事项：**如果在一个字符串中完成了一次模式匹配之后要开始检索新的字符串，就必须手动地把 lastIndex 属性重置为 0。
**提示：**请注意，无论 RegExpObject 是否是全局模式，exec() 都会把完整的细节添加到它返回的数组中。这就是 exec() 与String.match() 的不同之处，后者在全局模式下返回的信息要少得多。因此我们可以这么说，在循环中反复地调用 exec() 方法是唯一一种获得全局模式的完整模式匹配信息的方法。
--------------------------------------------------------------
