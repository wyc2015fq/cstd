# C语言程序结构「菜鸟入门篇」 - weixin_33985507的博客 - CSDN博客
2018年01月10日 13:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
![9850446-1759442cbd754650](https://upload-images.jianshu.io/upload_images/9850446-1759442cbd754650)
C语言程序结构
C Hello World 实例
C 程序主要包括以下部分：
预处理器指令
函数
变量
语句 & 表达式
注释
让我们看一段简单的代码，可以输出单词 "Hello World"：
实例
#includeintmain(){/* 我的第一个 C 程序 */printf("Hello, World! "); return0;}
接下来我们讲解一下上面这段程序：
程序的第一行 #include  是预处理器指令，告诉 C 编译器在实际编译之前要包含 stdio.h 文件。
下一行 int main() 是主函数，程序从这里开始执行。
下一行 /*...*/ 将会被编译器忽略，这里放置程序的注释内容。它们被称为程序的注释。
下一行 printf(...) 是 C 中另一个可用的函数，会在屏幕上显示消息 "Hello, World!"。
下一行return 0;终止 main() 函数，并返回值 0。
编译 & 执行 C 程序
![9850446-cf0d3246f0f11fec](https://upload-images.jianshu.io/upload_images/9850446-cf0d3246f0f11fec)
接下来让我们看看如何把源代码保存在一个文件中，以及如何编译并运行它。下面是简单的步骤：
打开一个文本编辑器，添加上述代码。
保存文件为 hello.c。
打开命令提示符，进入到保存文件所在的目录。
键入 gcc hello.c，输入回车，编译代码。
如果代码中没有错误，命令提示符会跳到下一行，并生成 a.out 可执行文件。
现在，键入 a.out 来执行程序。
您可以看到屏幕上显示 "Hello World"。
$ gcc hello.c$ ./a.outHello, World!】
请确保您的路径中已包含 gcc 编译器，并确保在包含源文件 hello.c 的目录中运行它。
![9850446-c9eefd39abfeb18a](https://upload-images.jianshu.io/upload_images/9850446-c9eefd39abfeb18a)
![9850446-be4b146a0539001e.png](https://upload-images.jianshu.io/upload_images/9850446-be4b146a0539001e.png)
![9850446-3769bc236eac7464.png](https://upload-images.jianshu.io/upload_images/9850446-3769bc236eac7464.png)
![9850446-4ddc235d1ac8466e.jpg](https://upload-images.jianshu.io/upload_images/9850446-4ddc235d1ac8466e.jpg)
以上知识点都了解清楚了吗？现如今是学好C语言走遍天下都不怕！如有疑问可点击右上角关注与小编一同探讨学习C语言，小编会继续发布C语言的干货。欢迎关注！
