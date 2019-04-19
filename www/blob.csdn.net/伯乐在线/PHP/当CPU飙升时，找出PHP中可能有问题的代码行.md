# 当CPU飙升时，找出PHP中可能有问题的代码行 - 文章 - 伯乐在线
原文出处： [淘宝搜索技术博客 - 沧龙](http://www.searchtb.com/2014/04/%E5%BD%93cpu%E9%A3%99%E5%8D%87%E6%97%B6%EF%BC%8C%E6%89%BE%E5%87%BAphp%E4%B8%AD%E5%8F%AF%E8%83%BD%E6%9C%89%E9%97%AE%E9%A2%98%E7%9A%84%E4%BB%A3%E7%A0%81%E8%A1%8C.html)
当你发现一个平时占用cpu比较少的进程突然间占用cpu接近100%时，你如何找到导致cpu飙升的原因？我的思路是，首先找到进程正在执行的代码行，从而确定可能有问题的代码段。然后，再仔细分析有问题的代码段，从而找出原因。
如果你的程序使用的是c、c++编写，那么你可以很容易的找到正在执行的代码行。但是，程序是php编写的，如何找到可能有问题的代码行呢？这个问题就是本文要解决的问题。
**背景知识:**
> 
如果你对c语言不熟悉的话，可以略过，直接看 示例演示。
大家都知道php是一个解释性语言。用户编写的php代码会生成opcode，由解释器引擎去解释执行。在解释执行过程中，有一个全局变量包含了执行过程中用到的各种数据。它就是executor_globals。在源码的Zend/zend_globals.h 文件中可以找到他的类型定义。

PHP
```
struct _zend_executor_globals {
	zval **return_value_ptr_ptr;
	zval uninitialized_zval;
	zval *uninitialized_zval_ptr;
	zval error_zval;
	zval *error_zval_ptr;
	zend_ptr_stack arg_types_stack;
	/* symbol table cache */
	HashTable *symtable_cache[SYMTABLE_CACHE_SIZE];
	HashTable **symtable_cache_limit;
	HashTable **symtable_cache_ptr;
	zend_op **opline_ptr;
	HashTable *active_symbol_table;
	HashTable symbol_table;		/* main symbol table */
	HashTable included_files;	/* files already included */
	JMP_BUF *bailout;
	int error_reporting;
	int orig_error_reporting;
	int exit_status;
	zend_op_array *active_op_array;
	HashTable *function_table;	/* function symbol table */
	HashTable *class_table;		/* class table */
	HashTable *zend_constants;	/* constants table */
	zend_class_entry *scope;
	zend_class_entry *called_scope; /* Scope of the calling class */
	zval *This;
	long precision;
	int ticks_count;
	zend_bool in_execution;
	HashTable *in_autoload;
	zend_function *autoload_func;
	zend_bool full_tables_cleanup;
	/* for extended information support */
	zend_bool no_extensions;
#ifdef ZEND_WIN32
	zend_bool timed_out;
	OSVERSIONINFOEX windows_version_info;
#endif
	HashTable regular_list;
	HashTable persistent_list;
	zend_vm_stack argument_stack;
	int user_error_handler_error_reporting;
	zval *user_error_handler;
	zval *user_exception_handler;
	zend_stack user_error_handlers_error_reporting;
	zend_ptr_stack user_error_handlers;
	zend_ptr_stack user_exception_handlers;
	zend_error_handling_t  error_handling;
	zend_class_entry      *exception_class;
	/* timeout support */
	int timeout_seconds;
	int lambda_count;
	HashTable *ini_directives;
	HashTable *modified_ini_directives;
	zend_objects_store objects_store;
	zval *exception, *prev_exception;
	zend_op *opline_before_exception;
	zend_op exception_op[3];
	struct _zend_execute_data *current_execute_data;
	struct _zend_module_entry *current_module;
	zend_property_info std_property_info;
	zend_bool active;
	void *saved_fpu_cw;
	void *reserved[ZEND_MAX_RESERVED_RESOURCES];
};
```
这里我们只说两个对我们比较重要的变量，active_op_array 和 current_execute_data。
**active_op_array**变量中保存了引擎正在执行的op_array。在Zend/zend_compile.h中有关于op_array的数据类型的定义。

PHP
```
struct _zend_op_array {
	/* Common elements */
	zend_uchar type;
	char *function_name;
	zend_class_entry *scope;
	zend_uint fn_flags;
	union _zend_function *prototype;
	zend_uint num_args;
	zend_uint required_num_args;
	zend_arg_info *arg_info;
	zend_bool pass_rest_by_reference;
	unsigned char return_reference;
	/* END of common elements */
	zend_bool done_pass_two;
	zend_uint *refcount;
	zend_op *opcodes;
	zend_uint last, size;
	zend_compiled_variable *vars;
	int last_var, size_var;
	zend_uint T;
	zend_brk_cont_element *brk_cont_array;
	int last_brk_cont;
	int current_brk_cont;
	zend_try_catch_element *try_catch_array;
	int last_try_catch;
	/* static variables support */
	HashTable *static_variables;
	zend_op *start_op;
	int backpatch_count;
	zend_uint this_var;
	char *filename;
	zend_uint line_start;
	zend_uint line_end;
	char *doc_comment;
	zend_uint doc_comment_len;
	zend_uint early_binding; /* the linked list of delayed declarations */
	void *reserved[ZEND_MAX_RESERVED_RESOURCES];
};
```
看完定义，就不用我多说了把。定义中，filename和 function_name分别保存了正在执行的文件名和方法名。
**current_execute_data**保存了正在执行的op_array的execute_data。execute_data保存了每个op_array执行过程中的一些数据。其定义在，Zend/zend_compile.h：

PHP
```
struct _zend_execute_data {
    struct _zend_op *opline;
    zend_function_state function_state;
    zend_function *fbc; /* Function Being Called */
    zend_class_entry *called_scope;
    zend_op_array *op_array;
    zval *object;
    union _temp_variable *Ts;
    zval ***CVs;
    HashTable *symbol_table;
    struct _zend_execute_data *prev_execute_data;
    zval *old_error_reporting;
    zend_bool nested;
    zval **original_return_value;
    zend_class_entry *current_scope;
    zend_class_entry *current_called_scope;
    zval *current_this;
    zval *current_object;
    struct _zend_op *call_opline;
};
```
定义中的opline就是正在执行的opcode。opcode的结构定义如下：

PHP
```
struct _zend_op {
	opcode_handler_t handler;
	znode result;
	znode op1;
	znode op2;
	ulong extended_value;
	uint lineno;
	zend_uchar opcode;
};
```
其中lineno就是opcode所对应的行号。
**示例说明:**
看完上面的数据结构定义，你是否已经知道如何找php正在执行的文件名，方法名和行号呢?如果还有疑问的话，那就接着看下面的例子。创建一个文件test.php,代码如下：

PHP
```
<?php function test1(){
        while(true){
              sleep(1);
        }
}
test1();
?>
```
cli方式执行php脚本，加入执行的进程号为14973。我们使用gdb命令来调试进程。

PHP
```
$sudo gdb -p 14973
(gdb) print (char *)executor_globals.active_op_array->filename
$1 = 0x9853a34 "/home/xinhailong/test/php/test.php"
(gdb) print (char *)executor_globals.active_op_array->function_name
$2 = 0x9854db8 "test1"
(gdb) print executor_globals->current_execute_data->opline->lineno
$3 = 4
```
很显然，他正在执行第四行的sleep方法。
如果上面的方法你感觉麻烦，那你可以使用.gdbinit文件。这个文件在php源码的根目录下。使用方法如下：

PHP
```
$sudo gdb -p 14973
(gdb) source /home/xinhailong/.gdbinit
(gdb) zbacktrace
[0xa453f34] sleep(1) /home/xinhailong/test/php/test.php:4
[0xa453ed0] test1() /home/xinhailong/test/php/test.php:7
(gdb)
```
**题外话：**
​从php5.6开始，php中集成了一个phpdbg的工具。可以像gdb调试c语言程序一样，调试php程序。感兴趣的话，可以打开下面的连接看看。
[https://wiki.php.net/rfc/phpdbg](https://wiki.php.net/rfc/phpdbg)
[http://phpdbg.com/docs](http://phpdbg.com/docs)
