# PowerShell中的输出文件重定向命令Out-File - 小灰笔记 - CSDN博客





2017年04月09日 23:33:41[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：4568








       输出文件重定向其实有个简洁的命令>，我经常使用这个，主要还是因为在不同的操作系统命令中这个符号都能够奏效。

       为了能够了解PowerShell的风格，还是有必要熟悉一下PowerShell的命令。Cmdlet命令Out-File可以实现输出文件的重定向。这样可以在很多时候方便地通过文件记录命令执行的输出文本提示符号。

使用示范：

PSE:\WorkSpace\01_编程语言\04_PowerShell> treeE:\WorkSpace /f | out-file re_01.txt

       执行成功的时候，在当前目录中生成了一个名为re_01.txt的文件。如果在文件上加上路径信息，写的是文件的完整路径信息，那么在指定的位置会生成相应的输出文件。文件中的部分信息如下：

文件夹 PATH 列表

卷序列号为 2013-5089

E:\WORKSPACE

│  code_count.py

│  

├─01_编程语言

│  ├─01_C语言

│  │ │  new.py

│  │ │  

│  │ ├─01_算法

│  │ │  └─01_排序

│  │ │          a.exe

│  │ │          quick_sort.c

│  │ │          

│  │ ├─02_main_return_value

│  │ │      a.exe

│  │ │      test.c

│  │ │      

│  │ ├─03_declare

│  │ │      a.exe

│  │ │      demo.c

│  │ │      exp_31.c

│  │ │      exp_32.c

│  │ │      exp_33.c

       命令的使用并没有字符上大小写的要求，示范中使用了全部小写的形式。而帮助文档中提到这个命令的时候，两个单词的首字母通常都是大写的。

       换一种方式，使用大于号定向符做测试如下：

PS E:\WorkSpace\01_编程语言\04_PowerShell> tree E:\WorkSpace /f > re_02.txt

       程序依然没有执行上的信息提示，在当前目录下又多了一个名为re_02.txt的文件。其中的部分内容如下：

文件夹 PATH 列表

卷序列号为 2013-5089

E:\WORKSPACE

│  code_count.py

│  

├─01_编程语言

│  ├─01_C语言

│  │ │  new.py

│  │ │  

│  │ ├─01_算法

│  │ │  └─01_排序

│  │ │          a.exe

│  │ │          quick_sort.c

│  │ │          

│  │ ├─02_main_return_value

│  │ │      a.exe

│  │ │      test.c

│  │ │      

│  │ ├─03_declare

│  │ │      a.exe

│  │ │      demo.c

│  │ │      exp_31.c

│  │ │      exp_32.c

│  │ │      exp_33.c

│  │ │      exp_34.c

       现在手头没有比较工具或者bash中的diff命令，否则似乎可以验证一下两个文件是否一致。不过，通过简单的文本信息对比，两者应该是没有什么太大的差异。

       看一下Cmdlet命令out-file的帮助信息：

PSE:\WorkSpace\01_编程语言\04_PowerShell> help out-file



名称

    Out-File



语法

    Out-File [-FilePath] <string>[[-Encoding] {unknown | string | unicode | bigendianunicode | utf8 | utf7 |utf32 | a

    scii | default | oem}]  [<CommonParameters>]



    Out-File [[-Encoding] {unknown | string |unicode | bigendianunicode | utf8 | utf7 | utf32 | ascii | default | oem}

    ] [<CommonParameters>]





别名

    无





备注

    Get-Help 在此计算机上找不到该cmdlet 的帮助文件。它仅显示部分帮助。

        -- 若要下载并安装包含此cmdlet 的模块的帮助文件，请使用 Update-Help。

        -- 若要联机查看此cmdlet 的帮助主题，请键入: "Get-Help Out-File -Online"或

           转到 [http://go.microsoft.com/fwlink/?LinkID=113363](http://go.microsoft.com/fwlink/?LinkID=113363)。

       系统内置的信息中也没有太多可以借鉴的信息，除了这个简单的语法规则提示。真正深入使用的时候，还是跌借助于网络信息咯。



