# Variable Mangling in Bash with String Operators - nosmatch的专栏 - CSDN博客
2011年08月25日 20:53:36[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：486标签：[bash																[string																[null																[command																[filenames																[shell](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=filenames&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=bash&t=blog)
### Abstract
Have you ever wanted to change the names of many files at once? How about using a default value for a variable if it has no value? These and many other options are available to you through string operators in bash and other Bourne shell derived shells.
String operators allow you to manipulate the contents of a variable without having to write your own shell functions to do so. They are provided through 'curly brace' syntax. Any variable can be displayed like this
`${foo}` without changing its meaning. This functionality is often used to protect a variable name from surrounding characters.
``
     bash-2.02$ export foo=foo
     bash-2.02$ echo ${foo}bar # foo exists so this works
     foobar
     bash-2.02$ echo $foobar # foobar doesn't exist, so this fails
     bash-2.02$ 
By the end of this article, you'll be able to use it for a whole lot more.
There are three kinds of variable substitution: 
- Pattern Matching, 
- Substitution, 
- Command Substitution. 
I'll talk about the first two and leave command substitution for another article.
### Pattern Matching
There are two kinds of pattern matching available, matching from the left and matching from the right. The operators, with their functions and an expample, are shown in the following table:
|Operator|Function|Example|
|----|----|----|
|`${foo#t*is}`|deletes the shortest possible match from the left|```export $foo="this is a test"echo ${foo#t*is}is a test```|
|`${foo##t*is}`|deletes the longest possible match from the left|```export $foo="this is a test"echo ${foo#t*is}a test```|
|`${foo%t*st}`|deletes the shortest possible match from the right|```export $foo="this is a test" echo ${foo%t*st}this is a```|
|`${foo%%t*st}`|deletes the longest possible match from the right|```export $foo="this is a test"echo ${foo#t*is}```|
> 
**Note:***While the `#` and `%` identifiers may not seem obvious, they have a convenient mnemonic. The`#` key is on the left side of the `$` key and operates from the left. The`%` key is on the right of the `$` key and operated from the right.*
These operators can be used to do a variety of things. For example, the following script will change the extension of all '.html' files to '.htm'.
> ``#!/bin/bash
# quickly convert html filenames for use on a dossy system
# only handles file extensions, not file names
for i in *.html; do 
   if [ -f ${i%l} ]; then
       echo ${i%l} already exists
   else
       mv $i ${i%l}
   fi
done
### Substitution
Another kind of variable mangling you might want to employ is substitution. There are four substitution operators in Bash. They are shown in the following table:
|Operator|Function|Example|
|----|----|----|
|`${foo:-bar}`|If $foo exists and is not null, return $foo. If it doesn't exist, or is null, return bar.|```export foo=""echo ${foo:-one}oneecho $foo```|
|`${foo:=bar}`|If $foo exists and is not null, return $foo. If it doesn't exist, or is null, set $foo to bar and return bar|```export foo=""echo ${foo:=one}oneecho $fooone```|
|`${foo:+bar}`|If $foo exists and is not null, return bar. If it doesn't exist, or is null, return a null.|```export foo="this is a test" echo ${foo:+bar}bar```|
|`${foo:?"error message"}`|If $foo exists and isn't null, return it's value. If it doesn't exist, or is null, print the error message. If no error message is given, it prints`parameter null or not set`.**Note:***In a non-interactive shell, this will abort the current script. In an interactive shell, this will just print the error message.*|```export foo="one"for i in foo bar baz; doeval echo \${$foo:?}onebash: bar: parameter null or not setbash: baz: parameter null or not set```|
> **Note:***The `:` in the above operators can be omitted. Doing so changes the behavior of the operator to only test for existence of the variable. This will cause the creation of a variable in the case of*`${foo=bar}`
These operators can be used in a variety of ways. A good example would be to give a default value to a variable normally read from the command line arguments when no arguments are given. This is shown in the following script.
> ``#!/bin/bash
export INFILE=${1-"infile"}
export OUTFILE=${2-"outfile"}
cat $INFILE > $OUTFILE
Hopefully this gives you something to think about and to play with until the next article. If you're interested in more hints about bash (or other stuff I've written about), please take a look at
[my home page](http://tbr.nailed.org/). If you've got questions or comments, please
[drop me a line](mailto:pate@tbr.nailed.org). 
注：好文章，就是忙+懒，就不去翻译了......
