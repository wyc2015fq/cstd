
# 学习tcl的几个好网络连接 - Augusdi的专栏 - CSDN博客


2015年08月22日 20:46:49[Augusdi](https://me.csdn.net/Augusdi)阅读数：1561


﻿﻿
1.[http://archive.eso.org/skycat/docs/tclutil/tclutil.44.html](http://archive.eso.org/skycat/docs/tclutil/tclutil.44.html)
2.[http://vzzllblog.bokee.com/4711864.html](http://vzzllblog.bokee.com/4711864.html)
3.[http://tmml.sourceforge.net/doc/tcl/Tcl_Main.html](http://tmml.sourceforge.net/doc/tcl/Tcl_Main.html)
4.[http://www.tcl.tk/man/tcl8.4/TclLib/Tcl_Main.htm](http://www.tcl.tk/man/tcl8.4/TclLib/Tcl_Main.htm)
5.[http://www.fundza.com/tcl/script_shell/arguments.html](http://www.fundza.com/tcl/script_shell/arguments.html)
---------------------------------------------------------------------------
|Tcl
|Command Line Arguments
|[return to main index](http://www.fundza.com/index.html)
|

|Introduction
|Although Tcl scripts/procedures can be controlled via graphical user interfaces (GUI's) that have been implemented with the Tk toolkit, it is often easier to use traditional command line techniques. Another tutorial,|[using the command line](http://www.fundza.com/tcl/script_shell/command_line.html)|, gave a simple example of invoking a Tcl script from the command line.
|This tutorial demonstrates how, at the time a script is invoked, data can be passed to the script from the command line.
|arguments
|Items of data passed to a script from the command line are known as|arguments|. For example, take the simple script presented in|[using the command line](http://www.fundza.com/tcl/script_shell/command_line.html)|tutorial.
|puts [expr 3 + 2]
|Lets assume the script has been saved as|add.tcl|and that the present working directory of the shell window in which we are working matches the directory in which the script has been saved. We know the script can be run using the following command,
|tclsh add.tcl
|Of course the output is always the same because the values being added are hard-coded into the script. The script would be more useful if we could pass values to the script from the command line.
|tclsh add.tcl|23 15
|The method by which numbers can be passed into, and used by a script, is as follows.
|

|argc argv argv0
|All Tcl scripts have access to three predefined variables.
|$argc|- number items of arguments passed to a script.
|$argv|- list of the arguments.
|$argv0|- name of the script.
|To use the arguments, the script could be re-written as follows.
|if { $argc != 2 } {
|puts "The add.tcl script requires two numbers to be inputed."
|puts "For example, tclsh add.tcl 2 5".
|puts "Please try again."
|} else {
|puts [expr [lindex $argv 0] + [lindex $argv 1]]
|}
|The|lindex|command returns the first and second items from the list of arguments entered at the command line. Items in a list are counted from zero.
|


