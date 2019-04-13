
# tcl-tk实例详解——glob使用例解 - Augusdi的专栏 - CSDN博客


2015年12月15日 17:41:56[Augusdi](https://me.csdn.net/Augusdi)阅读数：1898


glob命令
这里以实例的形式解释一下glob命令的用法，很多时候纯粹的语法讲解根本讲不清楚，往往没有一个例子清晰，一下就glob命令进行一些分析，环境为Tclsh85，操作系统为windows XP。在windows下面文件夹使用/隔开，为了讲述方便统一使用/来描述。如果需要了解glob命令的使用说明，请参考[文章](http://www.it-crazy.com/)：[http://blog.csdn.net/dulixin/archive/2008/04/17/2300326.aspx](http://blog.csdn.net/dulixin/archive/2008/04/17/2300326.aspx)
最简单的方式，cd到目标目录下，使用
% glob *
显示目标文件加里所有的文件。使用
% glob *.tcl
显示目标文件夹里所有的.tcl文件。如果搜索不到文件就会返回一个错误，可以使用-nocomplain标志位来使没有文件时返回空，直接使用
% glob test
no files matched glob pattern "test"
报回一个错误，而使用
% glob -nocomplain test
就返回空。
如果目前不在目标文件夹，而知道目标文件夹的绝对路径，可以使用
% glob -directory d:/tcl/ *
显示目标文件夹d:/tcl/里面的文件信息。文件信息的显示方式是显示文件的完整的路径名。比如d:/tcl/lib，如果只想显示lib文件名，可以使用
% glob -directory d:/tcl/ -tails *
-tails表示只显示文件名，可以与-directory和-path一起使用。
如果知道文件的前半部分，需要找后半部分
% glob -path d:/tcl/li *
返回d:/tcl/lib d:/tcl/licenses d:/tcl/license-at8.5-thread.terms等三个文件，如果需要只返回文件名，使用
% glob -path d:/tcl/li -tails *
只返回文件名lib licenses license-at8.5-thread.terms。
可以限定返回的文件类型，使用-type来限定，比如只返回文件夹文件
% glob -directory d:/tcl/ -type d *
只返回文件夹文件，使用-type f只返回普通文件等。

