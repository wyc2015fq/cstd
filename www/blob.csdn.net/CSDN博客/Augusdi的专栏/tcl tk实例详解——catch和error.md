
# tcl-tk实例详解——catch和error - Augusdi的专栏 - CSDN博客


2015年10月10日 12:42:27[Augusdi](https://me.csdn.net/Augusdi)阅读数：2823


﻿﻿
(转载自[http://blog.csdn.net/dulixin](http://blog.csdn.net/dulixin))
这两个命令可以成对出现，catch命令捕获一个错误，error命令产生一个错误。
catch命令参考：[http://blog.csdn.net/dulixin/archive/2008/03/24/2214636.aspx](http://blog.csdn.net/dulixin/archive/2008/03/24/2214636.aspx)
error命令参考：[http://blog.csdn.net/dulixin/archive/2008/03/26/2221254.aspx](http://blog.csdn.net/dulixin/archive/2008/03/26/2221254.aspx)
catch命令可以用来捕获一个错误，一个没有捕获的错误将终止脚本的执行。error会产生一个错误，error命令产生的错误如果没有被catch捕获将会导致脚本的终止。
catch命令格式：**catch**script ?resultVarName? ?optionsVarName?
error命令格式：**error**message ?info? ?code?
下面举一个常用的例子来解释error和catch是怎么一起使用的，对其可选项中的各种用途不加详细解释。
% proc myError {} {
error "This is a error"
}
% catch myError errorValue
1
% puts $errorValue
This is a error


