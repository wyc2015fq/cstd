# keil MDK中如何生成*.bin格式的文件 - xqhrs232的专栏 - CSDN博客
2012年12月15日 23:22:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：760
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://blog.csdn.net/zhzht19861011/article/details/5927924](http://blog.csdn.net/zhzht19861011/article/details/5927924)
 在Realview MDK的集成开发环境中，默认情况下可以生成*.axf格式的调试文件和*.hex格式的可执行文件。虽然这两个格式的文件非常有利于ULINK2仿真器的下载和调试，但是ADS的用户更习惯于使用*.bin格式的文件，甚至有些嵌入式软件开发者已经拥有了*.bin格式文件的调试或烧写工具。为了充分地利用现有的工具，同时发挥Realview MDK集成开发环境的优势，将*.axf格式文件或*.hex格式文件转换成*.bin格式的文件是十分自然的想法。本文将详细的探讨这种转换方法。
      在Realview MDK中，生成*.bin格式文件的工具是ARM公司的RVCT开发套件中的fromelf.exe。默认安装下，它位于C/：Keil/ARM/BIN40文件夹下。
      下面将介绍它在Keil MDK中的使用方法：
      1. 新建一个工程，例如test.uvproj；
      2. 打开Options for Target ‘target 1’对话框，选择User标签页；
      3. 构选Run User Programs After Build/Rebuild框中的Run #1多选框，在后边的文本框中输入C:/Keil/ARM/BIN31/fromelf.exe --bin -o ./bin/test.bin ./test.axf命令行；步骤2、3可见下图所示。
![](http://hi.csdn.net/attachment/201010/8/0_1286532557dGnF.gif)
      4. 重新编译文件，在./bin/文件夹下生成了test.bin文件。
经过上述4步的操作以后，将得到我们希望的test.bin格式的文件。
ps：步骤3中的"C:/Keil/ARM/BIN31/fromelf.exe ”也可以通过点击下图红色标注出进入keil安装目录下选择。
![](http://hi.csdn.net/attachment/201010/8/0_1286533031Rn3n.gif)
        以上是很久前写的,对于如何使用fromelf工具,通过上面的示例,想必都能很轻松的生成bin文件,今天补写一下fromelf工具的基本命令:
        --bin:输出二进制文件
        --i32:Intel 32位Hex
        --m32：Motorola 32位Hex
         --output <file>:file为输出文件名
         -o<file>:这个是armcc编译器命令,也可用于这里,指定输出文件的名字
其它命令可以在Dos中运行fromelf.exe工具,会列出详细命令和用法.
