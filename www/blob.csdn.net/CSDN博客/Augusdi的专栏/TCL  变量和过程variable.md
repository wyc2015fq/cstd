
# TCL: 变量和过程variable - Augusdi的专栏 - CSDN博客


2015年08月26日 18:47:58[Augusdi](https://me.csdn.net/Augusdi)阅读数：3414


﻿﻿
### 名称
variable - 创建和初始化一个名字空间变量
### 语法
**variable**?*name value...*?*name*?*value*?
### 描述
这个命令一般在名字空间中创建一个或多个变量，每个变量*name*使用*value*初始化，最后一个变量的*value*是可选的。
如果变量*name*不存在，就创建一个，在这种情况下，如果指定了*value*，就把它的值赋值给创建的变量，如果不指定*value*，新的变量*name*就没有初始化。如果变量已经存在，就赋值*value*，没有指定*value*则保持原来的值。一般来说*name*不做任何限定（不包含名字空间限定），并且在当前名字空间中创建，如果*name*包含有名字空间限定，变量就创建在指定的名字空间中，如果变量没有定义，对于**namespace which**命令来说就是可见的，但是对**info exists**命令来说是不可见的。
如果**variable**命令在一个tcl过程中执行，就创建一个局部变量，这个局部变量链接一个相关的名字空间变量（因此这些变量由**info vars**列出），这样variable命令类似**global**命令，虽然**global**命令只是链接一个在全局范围内的变量，如果给出了*values*，就用来修改相关名字空间变量，如果一个名字空间变量不存在，就创建一个并且初始化。
*name*变元不能引用一个数组的元素，但是*name*可以引用整个数组并且初始化的*value*必须符合数组的要求，声明过变量后，数组中的元素可以使用**set**和**array**命令来配置。
### 示例
在名字空间中创建一个变量：namespace eval foo {**variable**bar 12345  }在名字空间中创建一个数组：
namespace eval someNS {**variable**someAry      array set someAry {          someName  someValue          otherName otherValue      }  }为一个过程存取名字空间中的变量：
namespace eval foo {      proc spong {} {          \# Variable in this namespace**variable**bar          puts "bar is $bar"            \# Variable in another namespace**variable**::someNS::someAry          parray someAry      }  }

