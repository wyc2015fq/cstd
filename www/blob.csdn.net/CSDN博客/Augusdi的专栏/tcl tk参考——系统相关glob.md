
# tcl-tk参考——系统相关glob - Augusdi的专栏 - CSDN博客


2015年12月15日 17:47:25[Augusdi](https://me.csdn.net/Augusdi)阅读数：2003个人分类：[Tcl/Tk																](https://blog.csdn.net/Augusdi/article/category/2574303)



### 名称
glob - 返回模式匹配的文件名
### 语法
**glob**?*switches*?*pattern*?*pattern ...*?

### 描述
这个命令返回一个匹配*pattern*变元的文件列表，返回的列表并不排序，如果需要排序就需要调用**lsort**命令。
如果初始变元使用**-**开头，就作为标志出现，以下所述是目前支持的标志位：
**-directory***directory*在给定的文件夹中寻找匹配的文件，允许在文件夹中寻找包含通配风格字符的文件，这个标志位不能和**-path**一起使用**-join**剩下的pattern变元被认为是一个路径名的各个部分，将会被组合起来，类似于**file join**命令的方式**-nocomplain**允许返回一个空列表而不是错误，如果没有这个标志位当寻找不到与模式匹配的文件时就返回一个错误**-path***pathPrefix*寻找以*pathPrefix*开头、其它部分匹配给定模式的文件，这种模式允许在pathPrefix中出现glob敏感的匹配字符，不能够和**-directory**一起使用。例如，寻找所有包含相同**$path**根名字但是不同扩展名的文件，需要使用命令**glob -path [file rootname $path] .***，这个命令中的**$path**允许包含**glob**敏感的匹配字符**-tails**只返回文件的文件名而不是包含整个路径的文件名，需要和**-directory**或**-path**一起使用。**glob -tails -directory $dir ***这个命令和**set pwd [pwd]; cd $dir ; glob *; cd $pwd**这一系列命令的执行结果是相同的，对于**-path**，返回的文件名是包含整个文件名的，所以**glob -tails -path [file rootname ~/foo] .***将返回形如**foo.aux foo.bib foo.tex**的文件而不是形如**.aux .bib .tex**的文件。**-types***typeList*只有列出匹配*typeList*的文件或文件夹，*typeList*有两种形式。
第一种形式就像Unix搜索命名中的**-type**可选项，**b**块文件，**c**字符文件，**d**文件夹，**f**普通文件，**l**软链接，**p**命名管道，**s**套接字，可以有多个**-type**选项一起使用。**glob**命令返回至少匹配一种类型的所有的文件。注意，如果指定了**-type l**或者链接的目标文件匹配都会返回软链接文件，所以在指定**-types d**时一个文件夹的软链接也会返回。第二种形式指定的类型必须是所有的类型都匹配，**r**、**w**、**x**作为文件存取属性，**readonly**、**hidden**可以是特殊的存取属性。在Macintosh上，MacOS类型和创建者也是支持的。像{macintosh type XXXX}或{macintosh creator XXXX} 将匹配文件类型和创建者。
这两种形式可以混合使用，所以**-type {d f r w}**将寻找有可读和可写权限的普通文件和文件夹。下面两个命令是等价的：
**glob -type d *glob */**第一种方式不会返回"/"并且是和操作系统无关的。**--**标志位结束符号。在它之后的变元都将被当作是*pattern**pattern*变元可以包含任何以下特殊字符：
**?**匹配任何一个字符*****匹配0个或多个任何字符**[***chars***]**匹配*chars*中的任何一个字符，如果*chars*包含一个a-b的形式，那么是a-b当中的任何一个字符（包括a和b）**/***x*匹配字符x**{***a***,***b***,***...*}匹配a、b……中的任意一个字符串在Unix的csh中，以.开头或者包含/的文件名必须正确匹配或者在{}中，除非**-type hidden**指定了（因为以.开头的文件为隐藏文件）。在其它操作系统中，以.开头的文件没有什么特殊，但是文件名为.或者..时必须明确匹配。所有的/字符必须明确匹配。
如果在*pattern*中第一个字符为~，就引用主目录，如果~后面为/那么就会使用HOME环境变量。
**glob**命令与csh不同，第一，它不会排列返回的结果；第二，**glob**只返回实际存在的文件名。
### 示例
搜索当前目录下的所有的Tcl文件：
**glob***.tcl在用户主目录中搜索所有的Tcl文件：
**glob**-directory ~ *.tcl搜索当前目录下的所有的子目录：
**glob**-type d *搜索包含字符串"a"，"b"和"cde"的文件：
**glob**-type f *{a,b,cde}*

