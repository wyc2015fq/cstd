# Nebula3学习笔记(4): 工具库 - 逍遥剑客 - CSDN博客
2008年07月04日 14:48:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2924
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
Nebula3工具库, 包含一些工具类, 容器类, 还有一个强大的String类.
下面分别来看一下有哪些东东:
|Array< TYPE >|动态数组, 类似std::vector, 自带了排序方法和二分查找|
|----|----|
|Atom< TYPE >|对于持续存在对象的共享引用. 简单得来说, 就是一个生命周期很长的对象的智能指针, Atom<String>是最常用的, 作为常量字符串的封装.|
|Blob|大块内存空间的封装, 可以比较, 复制, 计算Hash值|
|CmdLineArgs|通用的命令行参数解析器, 格式: cmd arg0[=]value0 arg1[=]value1 arg2[=]value2|
|Crc|计算一段内存的CRC值|
|Dictionary< KEYTYPE, VALUETYPE >|词典类, 用于存储映射. 类似于std::map. 取元素的时间复杂度为O(log n). 内部是一个排序的Array实现的. 注意它只是在需要排序时才排, 所以加入元素很快, 而第一次的搜索会慢一些.|
|FixedArray< TYPE >|定长数组, 一维|
|FixedTable< TYPE >|表格, 定长二维数组|
|FourCC|四字符编码, 相当于一个uint, 可以做为ID, 具有可读性. 前面的工厂方法就用到了. (第一次见单引号里写多个字符@_@, 如uint = ‘ABCD’;)|
|Guid|全局统一标识符(GUID), 每台机器在不同时间生成的都不一样, 可以说是唯一性的.|
|HashTable< KEYTYPE, VALUETYPE >|跟Dictionary很像, 不过内部是用哈希表实现的, 搜索时间更快(O(1)), 内存占用要大一些. 相当于stdext::hash_map做KEY的类必需实现这个方法: IndexT HashCode() const|
|KeyValuePair< KEYTYPE, VALUETYPE >|相当于std::pair|
|List< TYPE >|双向链表, 相当于std::list|
|Proxy< TYPE >|相当于带引用计数的智能指针, 普通类也可以用它进行包装, 而不用继承Core::RefCounted|
|Queue< TYPE >|队列, 相当于std::queue|
|SimpleTree< VALUETYPE >|简单的树型结构, 结构存储在Array中|
|Stack< TYPE >|堆栈, 相当于std::stack|
|String|字符串类, 相当于std::string, 但是功能强大得多. 提供了与其它Nebula数据类型的转换方法, 还有文件名操作函数.|
|Variant|通用数据类型, 相当于COM中的VARIANT|
关于各个类的详细用法,可以参考testfoundation_win32工程.
