# ubuntu中在终端下如何查找具有某一特定字符串的文件 - 三少GG - CSDN博客
2012年11月23日 16:26:16[三少GG](https://me.csdn.net/scut1135)阅读数：5323
                
我只知道用find -name 文件名查找文件，但是现在想根据文件中的内容来查找，怎么做呢？
比如如何找到**具有"main"这个字符串的文件**的位置？
grep main *.*
继续追问：**如果要找的文件在当前文件夹的子文件夹下，并且有多个子文件夹，该怎么找呢？**
**补充回答：**
**grep -r main *.c**
如果不分大小写查找，加上-i
grep -r -i main ./
////////////////////
http://technet.microsoft.com/zh-cn/library/cc755993%28WS.10%29.aspx
f**ind命令常用来查找特定目录下，所有相关文件**，可是也经常要排除特定内容的文件/目录，比如不想看svn/cvs的系统文件目录，如何做到呢，明显-name不能解决问题，但是可以用find 的 -not参数，比如查找不包含.svn的所有文件：
find . -not -name '.svn' -name '*' -print  
or
find . -! -name '.svn' -name '*' -print  
再者find经常结合xargs处理复杂的功能:
find ttt -type f -name "*" | xargs grep gis
查找当前目录下的ttt文件夹中，包含字符gis的所有文件
///////////////////////////
## Find
在一个文件或多个文件中搜索指定的文本字符串。搜索指定的文件后，**find** 将显示包含指定字符串的所有行。
#### 语法
**find** [**/v**] [**/c**] [**/n**] [**/i**]
**"***String***" **[[*Drive***:**][*Path*]*FileName*[...]]
#### 参数
- **/v **
显示不包含指定 *String* 的所有行。- **/c **
计数包含指定 *String* 的行并显示总行数。- **/n **
将文件行号置于每行开头。- **/i **
指定搜索不区分大小写。- **"***String***"**
必需参数。指定要搜索的字符组。必须用引号将 *String* 引起来（即 **"***String***"**）。- **[*Drive***:**][*Path*] *FileName***
指定要在其中搜索指定字符串的文件的位置和名称。- **/?**
在命令提示符下显示帮助。
#### 注释
- 指定字符串 
如果不使用 **/i**，**find** 将精确搜索指定的 *String*。例如，**find** 命令认为字符“a”和“A”是不同的。但是，如果使用
**/i**，**find** 不区分大小写，并将“a”和“A”视为同一个字符。 
如果要搜索的字符串包含引号，必须将包含在字符串中的引号再用引号引起来（即**"***StringContaining***""***QuotationMarks***"**）。
- 将 **find** 用作筛选器 
如果省略文件名，**find** 将用作筛选器，从标准输入源（通常是键盘、管道或重定向的文件）得到输入，然后显示包含 *String* 的所有行。
- 命令语法次序 
可以按任何顺序键入 **find** 命令的参数和命令行选项。
- 使用通配符 
在 **find** 命令中指定的文件名或扩展名中不能使用通配符（* 和 ?）。要搜索使用通配符指定的一组文件中的字符串，可以在 **for** 命令中使用 **find** 命令。 
- 将 **/v** 或 **/n** 与 **/c** 一起使用
如果在相同的命令行中使用 **/c** 和 **/v**，**find** 将显示不包含指定字符串的行数。如果在相同的命令行中指定
**/c** 和 **/n**，**find** 将忽略 **/n**。
- 针对回车使用 **findfind** 命令不能识别回车。当使用 **find** 在包含回车的文件中搜索文本时，必须将搜索字符串限制在可以在两个回车之间找到的文本，也就是不能被回车中断的字符串。例如，“tax”与“file”之间出现回车时，**find** 不会报告找到“tax file”字符串。
#### 示例
要显示 Pencil.ad 中所有包含字符串“Pencil Sharpener”的行，请键入： 
**find "Pencil Sharpener" pencil.ad **
要查找将文字包含在引号之内的字符串，必须首先将整个字符串用引号引起来。其次，每个包含在字符串内的引号必须使用双引号。要在 Report.doc 中查找 "The scientists labeled their paper "for discussion only."It is not a final report."，请键入：
**find "The scientists labeled their paper ""for discussion only.""It is not a final report." report.doc**
如果要搜索一组文件，可以组合使用 **find** 命令和 **for** 命令。要在当前目录中搜索包含 .bat 扩展名以及包含“PROMPT”字符串的文件，请键入：
**for %f in (*.bat) do find "PROMPT" %f**
要搜索硬盘以便在驱动器 C 上查找和显示包含字符串“CPU”的文件名，请使用管线 (|) 将 **dir** 命令的结果重新定向到如下所示的
**find** 中： 
**dir c:\ /s /b | find "CPU" **
因为 **find** 搜索区分大小写并且 **dir** 产生的输出是大写的，所以必须用大写字母键入字符串“CPU”或者将
**/i** 命令行选项与 **find **一起使用。 

