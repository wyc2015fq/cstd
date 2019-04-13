
# uplevel 和 upvar - Augusdi的专栏 - CSDN博客


2015年05月08日 23:30:05[Augusdi](https://me.csdn.net/Augusdi)阅读数：2248


在 Tcl 中,Tcl 支持使用命名空间 (namespace) 的概念, namespace 的概念是不同指令与参数的集合, 你可以在不同的  namespace 中使用名字相同的程序或是变量而不至于造成混淆. 每次 Tcl 的直译器(Interpreter) 碰到一个新的程序 (procdure)  或是新的命名空间宣告 (Ex: "namespace eval" , "namesapce inscope"), Interpreter 就会把一个新的  "call frame" 塞进解译器自己的堆栈(call stack) (你可以参考 Tcl 的原始码, 找到相关的Tcl_PushCallFrame,  Tcl_PopCallFrame, TclGetFrame. Source code is the best document)
我们不要挖的太深, 先看看表面的东西. 用实例来说明会容易消化点. :)
\#!/usr/bin/tclseti0procloop{}{globaliputs"loop 的 level: [info level]";if{$i<256}{incri;loop;}}puts"预设的 global namesapce level: [info level]"loop这支脚本中我们使用 [ info level ] 取得目前的 level 层次. Tcl 中的 global 命名空间是所有的 Tcl 程序中预设的.  因此他是的层次是 "0". 每次loop 这支脚本被呼叫的时候, 他的层次就会被加一, 这完全是因为要作命名空间区隔的关系.  这个机制在你使用一个命名空间的时候也会产生作用.
\#!/usr/bin/tclnamespaceevalfoo{namespaceexport barprocbar{}{puts[infolevel]}}foo::bar那么这样的机制跟 uplevel 有甚么关系呢? uplevel 是用来在不同的 "stack frame" 中执行脚本用的. (我们 knowledge  based 中的 tcl 文件翻译的太怪了? "在一个不同的环境下执行某个script" :p )你可以使用 uplevel 在不同层次的命名空间动态切换.
\#!/usr/bin/tclnamespaceevalfoo{namespaceexport barprocbar{}{uplevel1{setname"Kid";}setname"blah...";}}setname"Rex Tsai"puts$namefoo::barputs$name执行后, 你可以发现 name 被改为 Kid.这是因为 uplevel 上到高一层的命名空间. 并修改了 name 这个变量. 请注意, 其中的 set  name "blah..." 跟uplevel 中的 name 是不同两个世界的唷!
另外一个有趣的指令是 upvar,upvar 可以让你产生一个链接连到别的堆栈框架. 他是使用 pass by reference 的方式连结,  因此如果你直接更动了变量,就会直接更动原本的变量. (请参考 Tcl 的原始码, MakeUpvar Tcl_UpVar Tcl_UpVar2 )
\#!/usr/bin/tclnamespaceevalfoo{namespaceexport barprocbar{}{upvar1name j\# 或是 upvar \#0 name jsetj"Kid"}}setname"Rex Tsai"puts$namefoo::barputs$nameupvar 与 uplevel 都必须指定你想使用的命名空间层级, 如果不指定, 他会使用上一层, 默认值是 1. 另外, 也可以使用抽象的层级号码,  他的方式是以 \#开头, 后面接一个层级数字. 假设我们有这样的一只脚本
\#!/usr/bin/tclprocfoobar{}{puts"foobar:\t[info level]";foo;};procfoo{}{puts"foo:\t[info level]";bar;};procbar{}{puts"bar:\t[info level]"};puts"global:\t[info level]"foobar其实他的层级号码是像这样的.
global: 0
foobar: 1
foo: 2
bar: 3因此预设的 global namesapce 的抽象层级号码是 0 , foobar 抽象层级号码是 1. 如果我们使用指定号码的方式存取其它堆栈框架,  这些数字应该为相对应的, 例如在 bar 中想存取 foobar层级, foobar 是 bar 的上面两层, 因此应该使用 uplevel 2 { body;  } 来撰写. 懂了吗?
说了那么多, 到底这个玩意能作些甚么, 这里举出一个范例. 在 Tcl 中, 并没有像 C 一样 static variables 的量类型.  这里是一个利用 uplevel 与upvar 实作 static variables 的 procedure. 看得懂, 你就出师了. :p
\#!/usr/bin/tclprocstatic{args}{setprocName[lindex[infolevel[expr[infolevel]-1]]0]foreachvarName$args{uplevel1"upvar \#0 staticvars($procName:$varName) $varName"}}procfoo{}{static nameif{![infoexist name]}{setname"Rex Tsai"}puts$name}foo
fooBy the way, 你还可以使用多重解译器来作隔离命名空间, 利用 safeTcl 模块使用多个编译器对象, 维持完全隔离的命名空间, 维护安全功能.  如果各位有兴趣, 这个东西我们可以再多做讨论.
[http://blog.chinaunix.net/uid-7262118-id-71422.html](http://blog.chinaunix.net/uid-7262118-id-71422.html)
﻿﻿

