# CMake命令之list - DoubleLi - 博客园






用途：提供一些列表操作

list(LENGTH <list><output variable>)
  list(GET <list> <elementindex> [<element index> ...]
       <output variable>)
  list(APPEND <list><element> [<element> ...])
  list(FIND <list> <value><output variable>)
  list(INSERT <list><element_index> <element> [<element> ...])
  list(REMOVE_ITEM <list> <value>[<value> ...])
  list(REMOVE_AT <list><index> [<index> ...])
  list(REMOVE_DUPLICATES <list>)
  list(REVERSE <list>)
  list(SORT <list>)

LENGTH返回列表的长度

GET返回列表中指定下标的元素

APPEND添加新元素到列表中

INSERT 将新元素插入到列表中指定的位置

REMOVE_ITEM从列表中删除某个元素

REMOVE_AT从列表中删除指定下标的元素

REMOVE_DUPLICATES从列表中删除重复的元素

REVERSE 将列表的内容实地反转，改变的是列表本身，而不是其副本

SORT 将列表按字母顺序实地排序，改变的是列表本身，而不是其副本

列表的子命令APPEND, INSERT, REMOVE_AT, REMOVE_ITEM,REMOVE_DUPLICATES, REVERSE以及SORT在当前的CMake变量域创建一些新值。与SET命令类似，即使列表本身是在父域中定义的，LIST命令也只会在当前域创建新的变量值，为了将这些操作的结果向上传递，需要通过SET PARENT_SCOPE， SET CACHE INTERNAL或其他值域扩展的方法。

注意：cmake中的列表是以分号隔开的一组字符串。可以使用set命令创建一个列表。例如：set(var a b c d e)创建了一个这样的列表：a;b;c;d;e。 set(var “a b c d e”)创建了一个字符串或只有一个元素的列表。

当指定索引值时，<element index>为大于或等于0的值。它从列表的开始处索引，0代表列表的第一个元素。如果<element index>为小于或等于-1的值，它从列表的结尾处索引，-1代表列表的最后一个元素。









