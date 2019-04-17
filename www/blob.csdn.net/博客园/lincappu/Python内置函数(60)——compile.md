# Python内置函数(60)——compile - lincappu - 博客园







# [Python内置函数(60)——compile](https://www.cnblogs.com/lincappu/p/8145331.html)





**英文文档：**

`compile`(*source*, *filename*, *mode*, *flags=0*, *dont_inherit=False*, *optimize=-1*)
- 
- Compile the *source* into a code or AST object. Code objects can be executed by [`exec()`](mk:@MSITStore:D:\Python\Python35-32\Doc\Python352.chm::/library/functions.html#exec) or [`eval()`](mk:@MSITStore:D:\Python\Python35-32\Doc\Python352.chm::/library/functions.html#eval). *source* can either be a normal string, a byte string, or an AST object. Refer to the `ast` module documentation for information on how to work with AST objects.
- 
- The *filename* argument should give the file from which the code was read; pass some recognizable value if it wasn’t read from a file (`'<string>'` is commonly used).
- 
- The *mode* argument specifies what kind of code must be compiled; it can be `'exec'` if *source* consists of a sequence of statements, `'eval'` if it consists of a single expression, or `'single'` if it consists of a single interactive statement (in the latter case, expression statements that evaluate to something other than `None` will be printed).
- 
- The optional arguments *flags* and *dont_inherit* control which future statements (see [**PEP 236**](https://www.python.org/dev/peps/pep-0236)) affect the compilation of *source*. If neither is present (or both are zero) the code is compiled with those future statements that are in effect in the code that is calling [`compile()`](mk:@MSITStore:D:\Python\Python35-32\Doc\Python352.chm::/library/functions.html#compile). If the *flags* argument is given and *dont_inherit* is not (or is zero) then the future statements specified by the *flags* argument are used in addition to those that would be used anyway. If *dont_inherit* is a non-zero integer then the *flags* argument is it – the future statements in effect around the call to compile are ignored.
- 
- Future statements are specified by bits which can be bitwise ORed together to specify multiple statements. The bitfield required to specify a given feature can be found as the `compiler_flag` attribute on the `_Feature` instance in the `__future__` module.
- 
- The argument *optimize* specifies the optimization level of the compiler; the default value of `-1` selects the optimization level of the interpreter as given by `-O` options. Explicit levels are `0` (no optimization; `__debug__` is true), `1` (asserts are removed, `__debug__` is false) or `2` (docstrings are removed too).
- 
- This function raises `SyntaxError` if the compiled source is invalid, and `ValueError` if the source contains null bytes.
- 
- If you want to parse Python code into its AST representation, see `ast.parse()`.
- 
- Note: When compiling a string with multi-line code in `'single'` or `'eval'` mode, input must be terminated by at least one newline character. This is to facilitate detection of incomplete and complete statements in the `code` module.
- 
- Changed in version 3.2: Allowed use of Windows and Mac newlines. Also input in `'exec'` mode does not have to end in a newline anymore. Added the *optimize* parameter.
- Changed in version 3.5: Previously, `TypeError` was raised when null bytes were encountered in *source*.
- 
- 
- 
- 
- 
- **　　将字符串编码成代码或者AST对象，使之能通过 exec 语句来执行或者 eval 来取值**
- **说明：**
- 
- 
- 　　1. 将source编译为代码或者AST对象。代码对象能够通过exec语句来执行或者eval()进行求值。
- 　　2. 参数source：字符串或者AST（Abstract Syntax Trees）对象。即需要动态执行的代码段。
- 　　3. 参数 filename：代码文件名称，如果不是从文件读取代码则传递一些可辨认的值。当传入了source参数时，filename参数传入空字符即可。
- 　　4. 参数mode：指定编译代码的种类，可以指定为 ‘exec’,’eval’,’single’。当source中包含流程语句时，mode应指定为‘exec’；当source中只包含一个简单的求值表达式，mode应指定为‘eval’；当source中包含了交互式命令语句，mode应指定为'single'。

```
>>> #流程语句使用exec
>>> code1 = 'for i in range(0,10): print (i)'
>>> compile1 = compile(code1,'','exec')
>>> exec (compile1)
0
1
2
3
4
5
6
7
8
9


>>> #简单求值表达式用eval
>>> code2 = '1 + 2 + 3 + 4'
>>> compile2 = compile(code2,'','eval')
>>> eval(compile2)
10


>>> #交互语句用single
>>> code3 = 'name = input("please input your name:")'
>>> compile3 = compile(code3,'','single')
>>> name #执行前name变量不存在
Traceback (most recent call last):
  File "<pyshell#29>", line 1, in <module>
    name
NameError: name 'name' is not defined
>>> exec(compile3) #执行时显示交互命令，提示输入
please input your name:'pythoner'
>>> name #执行后name变量有值
"'pythoner'"
```












