# DOS命令中的mv命令——move - 小灰笔记 - CSDN博客





2017年04月03日 23:26:44[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：6698







一直以来还是挺依赖bash shell中的几个常用的命令的，mv就是其中之一。尤其是在桌面上下载了一大堆文件需要处理的时候，创建一个临时文件夹把文件转移到相应的文件夹是我常用的方法之一。不过，在现在的公司工作，在工具选择上有着很大的不自由。本来就是Windows系统在加上没有安装常用的bash shell扩展，更让这使用习惯受到了挺大的束缚。其实，Windows下也有可以免安装的携带绿色版shell，其中的命令基本上也够我使用。幽默的是，公司的电脑我们自己没有修改环境变量的权限，而多次申请权限感觉到不方便之后遇到这种类似的情况基本上也就选择向一个比较笨拙的方式妥协。
不过，这种环境下工作也很好。这让我觉得工作之余的时间分外宝贵，极大地增进了我在工作之余学习的动力。
前阵子有稍加学习DOS批处理或者PowerShell以弥补这项缺陷的想法，今天晚上整理电脑桌面的时候做了一下尝试。没有借助于互联网，仅仅是简单的尝试。
很久之前我就知道DOS批处理命令copy，也时长使用。相应的bash shell的类似命令自然是cp。突然间联想到，mv既然是move的缩写，Windows下提供的DOS命令会不会是move？简单试了一下果真是！

示例：把当前文件夹的所有pdf文件转移到my_book文件夹

目录树：

C:\Users\ThinkPad\Desktop\test>tree /f

文件夹 PATH 列表

卷序列号为 00000016 28BD:E720

C:.

│  新建文本文档 (2).txt

│  新建文本文档.txt

│  第10章.pdf

│  第11章.pdf

│  第12章.pdf

│  第13章.pdf

│  第1章.pdf

│  第2章.pdf

│  第3章.pdf

│  第4章.pdf

│  第5章.pdf

│  第6章.pdf

│  第7章.pdf

│  第8章.pdf

│  第9章.pdf

│

└─my_book
为了测试，特意加入了两个冗余文件。

命令执行：

C:\Users\ThinkPad\Desktop\test>move *.pdf ./my_book

C:\Users\ThinkPad\Desktop\test\第10章.pdf

C:\Users\ThinkPad\Desktop\test\第11章.pdf

C:\Users\ThinkPad\Desktop\test\第12章.pdf

C:\Users\ThinkPad\Desktop\test\第13章.pdf

C:\Users\ThinkPad\Desktop\test\第1章.pdf

C:\Users\ThinkPad\Desktop\test\第2章.pdf

C:\Users\ThinkPad\Desktop\test\第3章.pdf

C:\Users\ThinkPad\Desktop\test\第4章.pdf

C:\Users\ThinkPad\Desktop\test\第5章.pdf

C:\Users\ThinkPad\Desktop\test\第6章.pdf

C:\Users\ThinkPad\Desktop\test\第7章.pdf

C:\Users\ThinkPad\Desktop\test\第8章.pdf

C:\Users\ThinkPad\Desktop\test\第9章.pdf

移动了        13 个文件。
看着提示，似乎执行结果跟期待比较相符。再次看一下目录树：

C:\Users\ThinkPad\Desktop\test>tree /f

文件夹 PATH 列表

卷序列号为 0000002D 28BD:E720

C:.

│  新建文本文档 (2).txt

│  新建文本文档.txt

│

└─my_book

        第10章.pdf

        第11章.pdf

        第12章.pdf

        第13章.pdf

        第1章.pdf

        第2章.pdf

        第3章.pdf

        第4章.pdf

        第5章.pdf

        第6章.pdf

        第7章.pdf

        第8章.pdf

        第9章.pdf
从目录树可以看出，文件的转移成功，而冗余文件也成功避开了。
            


