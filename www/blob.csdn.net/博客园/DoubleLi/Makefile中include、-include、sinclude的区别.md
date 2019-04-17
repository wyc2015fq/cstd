# Makefile中include、-include、sinclude的区别 - DoubleLi - 博客园






如果指示符“include”指定的文件不是以斜线开始（绝对路径，如/usr/src/Makefile...），而且当前目录下也不存在此文件；make将根据文件名试图在以下几个目录下查找：首先，查找使用命令行选项“-I”或者“--include-dir”指定的目录，如果找到指定的文件，则使用这个文件；否则继续依此搜索以下几个目录（如果其存在）：“/usr/gnu/include”、“/usr/local/include”和“/usr/include”。

当在这些目录下都没有找到“include”指定的文件时，make将会提示一个包含文件未找到的告警提示，但是不会立刻退出。而是继续处理Makefile的后续内容。当完成读取整个Makefile后，make将试图使用规则来创建通过指示符“include”指定的但未找到的文件，当不能创建它时（没有创建这个文件的规则），make将提示致命错误并退出。会输出类似如下错误提示：

**Makefile:**错误的行数：未找到文件名：提示信息（**No such file or directory**）

**Make**： ***** No rule to make target ‘’. Stop**

通常我们在Makefile中可使用“-include”来代替“include”，来忽略由于包含文件不存在或者无法创建时的错误提示（“-”的意思是告诉make，忽略此操作的错误。make继续执行）。像下边那样：

**-include FILENAMES...**

使用这种方式时，当所要包含的文件不存在时不会有错误提示、make也不会退出；除此之外，和第一种方式效果相同。以下是这两种方式的比较：

使用“**include FILENAMES...**”，make程序处理时，如果“**FILENAMES**”列表中的任何一个文件不能正常读取而且不存在一个创建此文件的规则时make程序将会提示错误并退出。

使用“**-include FILENAMES...**”的情况是，当所包含的文件不存在或者不存在一个规则去创建它，make程序会继续执行，只有真正由于不能正确完成终极目标的重建时（某些必需的目标无法在当前已读取的makefile文件内容中找到正确的重建规则），才会提示致命错误并退出。

为了和其它的make程序进行兼容。也可以使用“sinclude”来代替“-include”（GNU所支持的方式）。









