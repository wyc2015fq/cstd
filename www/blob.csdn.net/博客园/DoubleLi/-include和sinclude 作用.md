# -include和sinclude 作用 - DoubleLi - 博客园







## -include和sinclude





如果有文件没有找到的话，make会生成一条警告信息，但不会马上出现致命错误。它

会继续载入其它的文件，一旦完成makefile的读取，make会再重试这些没有找到，或

是不能读取的文件，如果还是不行，make才会出现一条致命信息。如果你想让make不

理那些无法读取的文件，而继续执行，你可以在include前加一个减号“-”。



如：-include <filename>



其表示，无论include过程中出现什么错误，都不要报错继续执行。和其它版本make兼

容的相关命令是sinclude，其作用和这一个是一样的。










