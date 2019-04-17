# CMake 手册详解（二十） - DoubleLi - 博客园







# [SirDigit](http://www.cnblogs.com/coderfenghc/)






## [CMake 手册详解（二十）](http://www.cnblogs.com/coderfenghc/archive/2012/09/03/2668008.html)



`**CMD#51: list **`列表操作命令。
  　　list(LENGTH <list> <output variable>)
  　　list(GET <list> <element index> [<element index> ...] <output variable>)
  　　list(APPEND <list> <element> [<element> ...])
  　　list(FIND <list> <value> <output variable>)
  　　list(INSERT <list> <element_index> <element> [<element> ...])
  　　list(REMOVE_ITEM <list> <value> [<value> ...])
  　　list(REMOVE_AT <list> <index> [<index> ...])
  　　list(REMOVE_DUPLICATES <list>)
  　　list(REVERSE <list>)
  　　list(SORT <list>)
　　使用LENGTH选项时，该命令会返回给定list的长度。

　　使用GET选项时，该命令返回list中所有被index索引的元素构成的list。

　　使用APPEND选项时，该命令将会在该list之后追加若干元素。

　　使用FIND选项时，该命令将返回list中指定的元素的索引；若果未找到，返回-1。

　　使用INSERT选项时，该命令将在list中指定的位置插入若干元素。

　　使用REMOVE_AT和REMOVE_ITEM选项将会从list中删除一些元素。它们之间的区别是：REMOVE_ITEM删除的是指定的项，而REMOVE_AT删除的是在指定索引处的项。

　　使用REMOVE_DUPLICATES选项时，该命令将删除list中的重复项。

　　使用REVERSE选项时，该命令将把list的内容就地前后倒换。

　　使用SORT选项时，该命令将按字母序对list总的内容就地排序。

　　注意：在CMake中，一个list是一个由封号;分割的一组字符串。使用set命令可以创建一个list。例如，set(var a b c d e)命令将会创建一个list：a;b;c;d;e；而set(var "a b c d e")命令创建的只是一个字符串，或者说是只有一个项的list。

　　当使用指定索引的命令格式时，如果<element index>是大于等于0的数，<element index>是从list第一个项开始的序号，list的第一项的索引是0。如果<element index>小于等于-1，这个索引是从结尾开始的逆向索引，其中-1表示的是list的最后一项。当使用负数索引时，注意它们不是从0开始！-0与0等价，它指向list的第一个成员。

**`CMD#52：load_cache`** 从另一个工程的CMake cache中加载值。
  　　load_cache(pathToCacheFile READ_WITH_PREFIX
             prefix entry1...)
　　该命令读取指定的cache文件，并将以请求的前缀为其前缀的那些cache文件中的entry(ies)保存到变量中。这个格式仅仅读取值，但是不在本地工程的cache中创建entry(ies)。
  　　load_cache(pathToCacheFile [EXCLUDE entry1...]
             [INCLUDE_INTERNALS entry1...])
　　从另一个cache文件中加载值并像内部entry(ies)那样，将它们存储到本地工程的cache中。这条命令对于一个依赖于另一个不同构建树上的另一个工程的工程比较有用。EXCLUDE选项给出了那些需要排除在外的entry(ies)的一个list。INCLUDE_INTERNALS选项给出了需要包含的entry(ies)的内部entry(ies)的一个list。通常情况下，不需要引入内部entry(ies)。强烈不推荐使用该命令的这种格式，但是它可以被用来维持向后兼容性。

**`CMD#53：load_command`** 将一条命令加载到一个运行中的CMake。
  　　load_command(COMMAND_NAME <loc1> [loc2 ...])
　　该命令将在给定的路径下查找名字为cmCOMMAND_NAME的一个库。如果找到了，它将会以模块的方式被加载，然后该命令将会被添加到可用的CMake命令集中。通常，TRY_COMPILE选项被用在这个命令之前来编译这个模块。如果该命令被成功加载，一个名为CMAKE_LOADED_COMMAND_<COMMAND_NAME>的变量将会被设置为这个加载模块的完整路径。否则，这个变量就不会被设置。

**`CMD#54：macro`**  为后续以命令方式调用而开始记录一组宏。
  　　macro(<name> [arg1 [arg2 [arg3 ...]]])
    　　COMMAND1(ARGS ...)
    　　COMMAND2(ARGS ...)
    　　...
  　　endmacro(<name>)
　　定义一个名为<name>的宏，它以arg1 arg2 arg3 (...)为参数。在macro命令之后，在与之配对的endmacro命令之前出现的命令，只有在宏被调用的时候才会被调用。当被调用的时候，这些被记录的命令首先以传进来的实参替换掉形参(如${arg1})，然后像正常的命令那样执行。除了形参之外，你还可以引用变量${ARGC}，它表示传递到宏里的参数的数量；${ARG0}, ${ARG1}, ${ARG2} ...等等则是传进来的实参值。这些变量使得创建带可选参数的宏变得很便捷。此外，变量${ARGV}保留了所有传递到宏里的所有参数组成的一个list，变量${ARGN}保留了在最后一个形参之后的参数组成的一个list。注意：传递到宏内部的参数和值，比如ARGN不是CMake通常意义下的变量；它们只是字符串替换，这一点非常像C预处理器对C语言宏的处理过程。如果你想要用真正的CMake变量，你应该查看一下function命令的说明。

　　关于在macro内部的策略的行为，参见cmake_policy()命令的相关文档。

**`CMD#55：mark_as_advanced`**  将CMake 的缓存变量标记为高级。
  　　mark_as_advanced([CLEAR|FORCE] VAR VAR2 VAR...)
　　将缓存的变量标记为高级变量。其中，高级变量指的是那些在cmake GUI中，只有当“显示高级选项”被打开时才会被显示的变量。如果CLEAR是第一个选项，参数中的高级变量将变回非高级变量。如果FORCE是第一个选项，参数中的变量会被提升为高级变量。如果两者都未出现，新的变量会被标记为高级变量；如果这个变量已经是高级/非高级状态的话，它将会维持原状。

　　该命令在脚本中无效。

**`CMD#56：math`** 数学表达式。
  　　math(EXPR <output variable> <math expression>)
　　EXPR计算数学表达式然后通过output变量返回计算结果。数学表达式的一个例子是"5*(10+13)"。该命令支持的运算符包括：+ - * / % ^ ~ << >> ；它们的含义与C语言中的完全一致。

**`CMD#57：message`** 为用户显示一条消息。
  　　message([STATUS|WARNING|AUTHOR_WARNING|FATAL_ERROR|SEND_ERROR]
          "message to display" ...)
　　可以用下述可选的关键字指定消息的类型：
　　  　　(无)         　 = 重要消息；
  　　　　STATUS         = 非重要消息；
  　　　　WARNING        = CMake 警告, 会继续执行；
  　　　　AUTHOR_WARNING = CMake 警告 (dev), 会继续执行；
  　　　　SEND_ERROR     = CMake 错误, 继续执行，但是会跳过生成的步骤；
  　　　　FATAL_ERROR    = CMake 错误, 终止所有处理过程；
　　CMake的命令行工具会在stdout上显示STATUS消息，在stderr上显示其他所有消息。CMake的GUI会在它的log区域显示所有消息。交互式的对话框（ccmake和CMakeSetup）将会在状态行上一次显示一条STATUS消息，而其他格式的消息会出现在交互式的弹出式对话框中。

　　CMake警告和错误消息的文本显示使用的是一种简单的标记语言。文本没有缩进，超过长度的行会回卷，段落之间以新行做为分隔符。



from:http://www.cnblogs.com/coderfenghc/archive/2012/09/03/2668008.html













