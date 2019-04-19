# 自上而下，逐步揭开 PHP 解析大整数的面纱 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [hoohack](http://www.jobbole.com/members/hector) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
浮点数精度丢失是一个长久的问题，PHP中精度丢失并不是PHP的bug，是计算机表示范围导致的问题。精度，这个问题的原因看起来不太重要，虽然学这个对于实际上的业务开发也没什么用，不会让你的开发能力“duang”地一下上去几个level，但是了解了PHP对于大整数的处理，也是自己知识框架的一个小小积累。
## 遇到的问题
最近遇到一个PHP大整数的问题，问题代码是这样的


```
$shopId = 17978812896666957068;
var_dump($shopId);
```
上面的代码输出，会把$shopId转换成float类型，且使用了科学计数法来表示，输出如下：
> 
float(1.7978812896667E+19)
但在程序里需要的是完整的数字作为查找数据的参数，所以需要用的是完整的数字，当时以为只是因为数据被转换成科学计数法了，于是想到的解决方案是强制让它不使用科学计数法表示：


```
$shopId= number_format(17978812896666957068);
var_dump($shopId);
```
这时候奇怪的事情出现了，输出的是：
> 
17978812896666957824
当时没有仔细看，对比了前十位就没有继续往下看，所以认为问题解决了，等到真正根据ID去找数据的时候才发现数据查不出来，这时候才发现是数据转换错误了。
这里使用number_format失败的原因在后面会讲到，当时就想到将原来的数据转成字符串的，但是使用了以下方法仍然不行


```
$shopId= strval(17978812896666957068);
var_dump($shopId);
$shopId = 17978812896666957068 . '';
var_dump($shopId);
```
输出的结果都是
> 
float(1.7978812896667E+19)
最后只有下面这种方案是可行的：


```
$shopId = '17978812896666957068';
var_dump($shopId);
// 输出
// string(20) "17978812896666957068"
```
众所周知，PHP是一门解释型语言，所以当时就大胆地猜测PHP是在编译期间就将数字的字面量常量转换成float类型，并用科学计数法表示。但仅仅猜测不能满足自己的好奇心，想要看到真正实现代码才愿意相信。于是就逐步分析、探索，直到找到背后的实现。
刚开始根据这个问题直接上网搜“PHP大整数解析过程”，并没有搜到答案，因此只能自己去追查。一开始对PHP的执行过程不熟悉，出发点就只能是一步一步地调试，然后
示例代码：


```
// test.php
$var = 17978812896666957068;
var_dump($var);
```
## 追查过程
1、查看opcode 通过vld查看PHP执行代码的opcode，可以看到，赋值的是一个ASSIGN的opcode操作 ![php整数01](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%281%29.png)
接下来就想看看ASSIGN是在哪里执行的。
2、gdb调试 2-1、用list查看有什么地方可以进行断点 ![php整数02](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%282%29.png)
2-2、暂时没有头绪，在1186断点试试 ![php整数03](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%283%29.png)
结果程序走到sapi/cli/php_cli.c文件的1200行了，按n不断下一步执行，一直到这里就走到了程序输出结果了： ![php整数04](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%284%29.png)
2-4、于是可以猜测，ASSIGN操作是在do_cli函数里面进行的，因此对do_cli函数做断点：break do_cli。 输入n，不断回车，在sapi/cli/php_cli.c文件的993行之后就走到程序输出结果了： ![php整数05](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%285%29.png)
2-5、再对php_execute_script函数做断点：break php_execute_script 不断逐步执行，发现在main/main.c文件的2537行就走到程序输出结果了： ![php整数06](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%286%29.png)
2-6、继续断点的步骤：break zend_execute_scripts 重复之前的步骤，发现在zend/Zend.c文件的1476行走到了程序输出结果的步骤： ![php整数07](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%287%29.png)
看到这里的时候，第1475行里有一个op_array，就猜测会不会是在op_array的时候就已经有值了，于是开始打印op_array的值： ![php整数08](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%288%29.png)
打印之后并没有看到有用的信息，但是其实这里包含有很大的信息量，比如opcode的handler: **ZEND_ASSIGN_SPEC_CV_RETVAL_CV_CONST_RETVAL_UNUSED_HANDLER**，但是当时没注意到，因为就想着看看op_array是怎么被赋值的，相关步骤做了什么，忽略了这个重要的信息，之后还是会回到这个handler。
2-7、重新从2-5的断点开始，让程序逐步执行，看到op_array的赋值如下： ![php整数09](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%289%29.png)
将zend_compile_file函数运行的结果赋值给op_array了，于是break zend_compile_file，被告知zend_compile_file未定义，通过源码工具追踪到zend_compile_file指向的是compile_file，于是break zend_compile
发现是在Zend/zend_language_scanner.l 文件断点了，逐步执行，看到这行pass_two(op_array)，猜测可能会在这里就有值，所以打印看看： ![php整数10](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%2810%29.png)
结果发现还是跟之前的一样，但是此时看到有一个opcodes的值，打印看看 ![php整数11](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%2811%29.png)
![php整数12](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%2812%29.png)
看到opcode = 38，网上查到38代表赋值
![php整数13](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%2813%29.png)
2-8、于是可以知道，在这一步之前就已得到了ASSIGN的opcode，因此，不断的往前找，从op_array开始初始化时就开始，逐步打印op_array->opcodes的值，一直都是null： ![php整数14](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%2814%29.png)
直到执行了CG(zend_lineno) = last_lineno;才得到opcode = 38 的值： ![php整数15](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%2815%29.png)
因为这一句：CG(zend_lineno) = last_lineno;是一个宏，所以也没头绪，接近放弃状态。。。
于是先去了解opcode的数据结构，在[深入理解PHP内核](http://www.php-internals.com/book/)书里找到opcode处理函数查找这一章，给了我一些继续下去的思路。
引用里面的内容： 在PHP内部有一个函数用来快速的返回特定opcode对应的opcode处理函数指针：zend_vm_get_opcode_handler()函数： ![php整数16](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%2816%29.png)
知道其实opcode处理函数的命名是有以下规律的


```
ZEND_[opcode]_SPEC_(变量类型1)_(变量类型2)_HANDLER
```
根据之前调试打印出来的内容，在2-6的时候就看到了一个handler的值： ![php整数17](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%2817%29.png)
是 **ZEND_ASSIGN_SPEC_CV_CONST_RETVAL_UNUSED_HANDLER**，
找出函数的定义如下： ![php整数18](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%2818%29.png)
可以看到，opcode操作的时候，值是从EX_CONSTANT获取的，根据定义展开这个宏，那就是


```
opline->op2->execute_data->literals
```
这里可以得到两个信息： 1、参数的转换在opcode执行前就做好了 2、赋值过程取值时是在op2->execute_data->literals，如果猜想没错的话，op2->execute_data->literals此时保存的就是格式转换后的值，可以打印出来验证一下
打印结果如下： ![php整数19](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%2819%29.png)
猜想验证正确，但是没有看到真正做转换的地方，还是不死心，继续找PHP的Zend底层做编译的逻辑代码。
参考开源的[GitHub项目](https://github.com/pangudashu/php7-internal)，PHP编译阶段如下图： ![php整数20](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%2820%29.png)
猜测最有可能的是在zendparse、zend_compile_top_stmt这两个阶段完成转换，因为这个两个阶段做的事情就是将PHP代码转换成opcode数组。
上网搜索了PHP语法分析相关的文章，有一篇里面讲到了解析整数的过程，因此找到了PHP真正将大整数做转换的地方：


```
{LNUM} {
    char *end;
if (yyleng
```
可以看到，zend引擎在对PHP代码在对纯数字的表达式做词法分析的时候，先判断数字是否有可能会溢出，如果有可能溢出，先尝试将其用LONG类型保存，如果溢出，先用zend_strtod将其转换为double类型，然后用double类型的zval结构体保存之。
## number_format失败的原因
通过gdb调试，追查到number_format函数，在PHP底层最终会调用php_conv_fp函数对数字进行转换： ![php整数21](http://7u2eqw.com1.z0.glb.clouddn.com/PHP%E5%A4%A7%E6%95%B4%E6%95%B0%20%2821%29.png)
函数原型如下：


```
PHPAPI char * php_conv_fp(register char format, register double num, boolean_e add_dp, int precision, char dec_point, bool_int * is_negative, char *buf, size_t *len);
```
这里接收的参数num是一个double类型，因此，如果传入的是字符串类型数字的话，number_format函数也会将其转成double类型传入到php_conf_fp函数里。而这个double类型的num最终之所以输出为17978812896666957824，是因为进行科学计数法之后的精度丢失了，重新转成double时就恢复不了原来的值。在C语言下验证：


```
double local_dval = 1.7978812896666958E+19;
printf("%f\n", local_dval);
```
输出的结果就是
> 
17978812896666957824.000000
所以，这不是PHP的bug，它就是这样的。
## 此类问题解决方案
对于存储，超过PHP最大表示范围的纯整数，在MySQL中可以使用bigint/varchar保存，MySQL在查询出来的时候会将其使用string类型保存的。 对于赋值，在PHP里，如果遇到有大整数需要赋值的话，不要尝试用整型类型去赋值，比如，不要用以下这种：


```
$var = 17978812896666957068;
```
而用这种：


```
$var = '17978812896666957068';
```
而对于number_format，在64位操作系统下，它能解析的精度不会丢失的数，建议的最大值是这个：9007199254740991。参考鸟哥博客：[关于PHP浮点数你应该知道的](http://www.laruence.com/2011/12/19/2399.html)
## 总结
这个问题的原因看起来不太重要，虽然学这个对于实际上的业务开发也没什么用，不会让你的开发能力“duang”地一下上去几个level，但是了解了PHP对于大整数的处理，也是自己知识框架的一个小小积累，知道了为什么之后，在日常开发中就会多加注意，比如从存储以及使用赋值的角度。了解这个细节还是很有好处的。
回想整个解决问题的过程，个人感觉有点长，总共大约花了4个小时去定位这个问题。因为对PHP的内核只是一知半解，没有系统的把整个流程梳理下来，所以一开始也不知道从哪里开始下手，就开始根据自己的猜测来调试。现在回想起来，应该先学习PHP的编译、执行流程，然后再去猜测具体的步骤。
原创文章，文笔有限，才疏学浅，文中若有不正之处，万望告知。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/7aa70eaa311d5a5dec82ca0d49624741.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/1a4b335ec981d013f4002205a23fedd0.jpg)
