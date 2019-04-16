# LoadRunner12_脚本中运行JavaScrip - ggabcda的博客 - CSDN博客





2017年08月17日 20:13:25[ggabcda](https://me.csdn.net/ggabcda)阅读数：243








版权声明：本文为博主原创文章，未经博主允许不得转载。

**【系统及软件配置】**

　　LR版本：12.53

　　JDK版本：1.8

　　函数：web_js_run，该函数仅在LR12版本提供支持，LR11不支持JavaScript的运行。

　　LR配置：开启了允许JavaScript运行：Replay→RunTime Settings→Preferences→Javascript→勾选“Enabe running JavaScript code”

**【测试1】-----执行JavaScript自带对象。**
|web_js_run(        "Code=alert('Hello world');",        LAST);|
|----|

　　备注： 该函数"Code="的等号前后不能有空格，否则该函数会报错：argument (number 1) is unrecognized or misplaced [MsgId: MERR-26396]

　　运行结果：

![](http://images2017.cnblogs.com/blog/717400/201708/717400-20170816230930115-1896677541.png)

**【测试2】-----执行JavaScript自带对象的方法，并获取返回值。**
|web_js_run(        "Code=new Date().getTime();",        "ResultParam=timestamp",        LAST); lr_output_message(lr_eval_string("{timestamp}"));|
|----|

　　运行结果：

![](http://images2017.cnblogs.com/blog/717400/201708/717400-20170816230945084-235359007.png)

**【测试3】-----执行外部js文件中的无参函数，并获取返回值。**
|web_js_run(        "Code=getString()",        "ResultParam=string1",        SOURCES,        "File=000.js",         ENDITEM,        LAST); lr_output_message(lr_eval_string("{string1}"));|000.js内容如下：function getString(){    var date = "This is LoadRunner12";     return date;}|
|----|----|

备注：①File参数中的js文件放在工作目录下即可使用相对路径；②计算机-组织-文件夹和搜索选项-取消“隐藏已知文件的扩展名”，否则可能会出现找不到文件的错误，测试使用的文件属性如下；

![](http://images2017.cnblogs.com/blog/717400/201708/717400-20170816231019850-264776338.png)

　　运行结果：

![](http://images2017.cnblogs.com/blog/717400/201708/717400-20170816231033131-2112454674.png)

**【测试4】执行外部js文件中的有参函数且参数固定，并获取返回值。**
|web_js_run(        "Code=getSum(1,2)",        "ResultParam=sum",        SOURCES,        "File=000.js",           ENDITEM,        LAST); lr_output_message(lr_eval_string("{sum}"));|000.js文件的内容如下：function getSum(arg1,arg2){    var date = arg1*1 + arg2*1;       return date;}//若直接使用arg1+arg2，则Javscript会执行字符串的相加；|
|----|----|

　　运行结果：

![](http://images2017.cnblogs.com/blog/717400/201708/717400-20170816231058600-281405927.png)

**【测试5】执行外部js文件中的有参函数且参数需要动态关联，并获取返回值。**
|web_js_run(        "Code=getSum(LR.getParam('Param'),LR.getParam('Param'))",        "ResultParam=sum2",        SOURCES,        "File=000.js",           ENDITEM,        LAST); lr_output_message(lr_eval_string("{sum2}"));|
|----|

备注：①使用LR.getParam()函数，作用是将LoadRunner的参数值提取出来，用为web_js_run函数运行时Code中的函数参数，传递给本地js文件，类似于LoadRunner的lr_eval_string()；

　　运行结果：

![](http://images2017.cnblogs.com/blog/717400/201708/717400-20170816231119896-906692769.png)

　　参数格式尝试及结果：
|"Code=getSum({Param},{Param})",|Error from JS Engine: ":0:SyntaxError: invalid object initializer" [MsgId: MERR-26000]|
|----|----|
|"Code=getSum(\"{Param}\",\"{Param}\")",|Action.c(35): web_js_run started [MsgId: MMSG-26355]Action.c(35): web_js_run was successful [MsgId: MMSG-26392]Action.c(42): NaN|

**【测试6】执行多个js函数，且函数的参数需要动态关联，并需要获取返回值。**
|web_js_run(        "Code="         "let rsaKey = new RSAKey();"         "rsaKey.setPublic(b64tohex(LR.getParam('exponent')),b64tohex(LR.getParam('modulus')));"         "hex2b64(rsaKey.encrypt(LR.getParam('password')));", //        "ResultParam=NewPassword",        SOURCES,        "File=sec.min.js",           ENDITEM,        LAST); lr_output_message(lr_eval_string("{NewPassword}"));|
|----|

备注：Code参数中连续执行了3条Javascript语句；

　　目前上面的语句还留有问题，虽脚本未报错，但NewPassword值为空；

**【测试7】函数一次定义多次使用**
|web_js_run(        "Code=myFunc(1, 2);",        "ResultParam=sum4",         SOURCES,         "Code=var myFunc=function(x,y) { return x*1+y*1; }",          ENDITEM,         LAST); lr_output_message(lr_eval_string("{sum4}")); web_js_run("Code=myFunc(3, 4);", "ResultParam=sum5", LAST); lr_output_message(lr_eval_string("{sum5}"));|
|----|

运行结果：

![](http://images2017.cnblogs.com/blog/717400/201708/717400-20170816231257568-373377253.png)

　　在这个例子中，myFunc()一次定义，多次使用，那么我们就会问该函数的生命周期是什么呢?多次迭代会生成多个该函数吗？如果是，那么重复定义造成的资源浪费怎么处理呢？其实：LoadRunner在设计时已经考虑了这些，为此有一个函数web_js_reset，如下。

**【扩展：web_js_reset】**

　　作用：①清理web_js_run执行后，生成的“变量、函数”等内容，释放其所在内存，否则之前定义过的函数和保存的变量会一直存在，会消耗本地资源的内存；

　　备注：如果在“Replay→RunTime Settings→Browser→Browser Emulation”中勾选了“simulate a new user on each iteration”，在开始新一次的迭代时，LoadRunner会自动执行web_js_reset。



