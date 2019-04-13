
# tcl-tk参考——控制结构catch - Augusdi的专栏 - CSDN博客


2015年10月10日 13:10:16[Augusdi](https://me.csdn.net/Augusdi)阅读数：4063


﻿﻿
### 名称
catch - 执行脚本并捕获错误
### 语法
**catch***script*?*resultVarName*? ?*optionsVarName*?

### 描述
**catch**命令用来防止出现错误而导致脚本执行终止，**catch**命令可以调用Tcl解释器去执行脚本，并且能够正常返回。
如果脚本产生一个错误，**catch**将返回一个非0的整数，如果没有捕获到错误就返回**0**或**TCL_OK**，Tcl还定义了四种异常代码：**1**（**TCL_ERROR**）、**2**（**TCL_RETURN**）、**3**（**TCL_BREAK**）和**4**（**TCL_CONTINUE**）。当执行脚本产生错误时就返回**TCL_ERROR**，其它的异常由**return**、**break**和**continue**命令产生。
如果给出了*resultVarName*变元，当返回**1**时，存储在*resultVarName*中的为错误信息，如果返回**0**，存储在*resultVarName*中的为脚本运行结果。
如果给出了*optionsVarName*变元，变量里面包含有**-code**和**-level**两个条目，如果返回代码不是**TCL_RETURN**时，**-level**为**0**，**-code**为返回的异常代码，当返回代码为**TCL_RETURN**时，**-level**和**-code**为其它的值，详细解释见**return**命令。
当返回**TCL_ERROR**时，三个额外的条目将会添加到*optionsVarName*中：**-errorinfo**、**-errorcode**和**-errorline**，**-errorinfo**条目是产生错误的信息，**-errorcode**条目是关于错误的一些额外信息，存储为列表方式，**-errorline**指出了错误发生的位置。**-errorinfo**和**-errorcode**条目都是最近发生的错误并且可以使用**::errorInfo**和**::errorCode**。
### 示例
**catch**命令可以在**if**命令中使用。
if { [**catch**{open $someFile w} fid] } {puts stderr "Could not open $someFile for writing/n$fid"exit 1}在**return**命令中有更多的关于**catch**的示例。

