
# tcl-tk参考——控制结构error - Augusdi的专栏 - CSDN博客


2015年10月10日 13:09:58[Augusdi](https://me.csdn.net/Augusdi)阅读数：1227


﻿﻿
### 名称
error - 产生一个错误
### 语法
**error***message*?*info*? ?*code*?

### 描述
返回一个**TCL_ERROR**代码并且终止脚本运行，*message*存储错误消息。
解释器的**-errorinfo**可选项用来记录当错误发生时堆栈状况（通俗点说就是发生错误的地方），当命令执行时，解释器会给**-errorinfo**添加信息。如果info变元存在，就用来初始化**-errorinfo**。换句话说，包含**error**命令的命令不会出现在堆栈记录上，它的信息将被*info*代替。这个用法在**catch**中非常有用，当捕获一个不能够传递的错误时，*info*可以用来返回一个堆栈跟踪信息来反馈错误发生点。
**catch {...} errMsg****set savedInfo $::errorInfo****...****error $errMsg $savedInfo**当在Tcl8.5或更新的版本中，下面的代码将被用于替代：
**catch {...} errMsg options****...****return -options $options $errMsg**如果*code*变元存在，它的值就存储在**-errorcode**中返回，**-errorcode**返回一个机器可读的错误描述，察看**return**帮助以获取更多的信息。
### 示例
如果基本数学操作失败就产生一个错误：
if {1+2 != 3} {**error**"something is very wrong with addition"}

