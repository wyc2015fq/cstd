
# 1.7 Python源文件 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月30日 09:57:43[seven-soft](https://me.csdn.net/softn)阅读数：431


现在让我们回到编程。每当你学习一种新的编程语言时，有一个传统，你编写和运行的第一个程序是“Hello World”程序——当你运行它时，它所做的只是说“Hello World”。正如Simon Cozens(神奇的"Beginning Perl"的作者)所说，这是“向编程神祈求帮你更好学习语言的传统咒语。”
开始你选择的编辑器，输入以下程序并将其保存为“hello.py’。
如果你使用Komodo编辑器，点击File --- New --- New File,输入下行：print('Hello World')

在Komodo编辑器，选File --- Save保存文件。
你应将文件保存在哪里？你知道位置的任何文件夹。如果你不明白这是什么意思，创建一个新文件夹，并使用该位置保存和运行你所有的Python程序：C:\py 在Windows上
/tmp/py 在Linux上
/tmp/py 在Mac OS X上
使用'mkdir'命令在命令行创建一个文件夹，例如,“mkdir/tmp/py”。
注意：总要确保你给它的文件扩展名是.py，例如，“foo.py”。
在Komodo Edit，请单击“Tools”---“Run Command”，输入"python3 hello.py",单击“Run”,你应该看到打印输出的内容。
尽管最好的方式是在Komodo中输入它，但在命令行也可以：打开在安装章节中介绍的一个命令行。
Change directory（改变路径）到你保存文件的目录，例如cd /tmp/py
在命令行输入python3 hello.py命令运行程序。
输出如下所示：
$ python3 hello.py
Hello World
如果你得到了如上所示有输出，祝贺你!——你已经成功地运行了你的第一个Python程序。您已经成功地越过学习编程最难的部分－－开始你的第一个程序!
如果你得到了一个错误，请完全输入如上所示程序，再次运行这个程序。注意，Python是区分大小写的，即“print”并不等于“Print”——注意，前者是小写字母“p”和后者是大写字母“P”。同样，确保每一行的第一个字母之前没有空格或制表符——之后我们将[明白为什么这很重要](http://zhgdg.gitcafe.com/static/doc/byte_of_python.html#%E7%BC%A9%E8%BF%9B)。
## 它是如何工作的
Python程序是由语句组成，在我们的第一个程序中，我们只有一个语句，在这个语句中，我们调用“print”函数，它只是打印文本“Hello World”。我们在(稍后章节](\#函数)将详细学习函数——你现在应该理解的是,无论你在括号提供什么，都将打印到屏幕上。在本例中，我们提供了文本“Hello World”。
## 可执行的Python程序
这只适用于Linux和Unix用户，但Windows用户应该知道。
每次，你想要运行一个Python程序，我们必须显式地调用的“python3 foo.py”，但是我们为什么不能像运行我们电脑中的其它任何程序一样运行它呢？我们通过使用叫hashbang行代码可以实现。
添加下面的一行作为程序的第一行：\#!/usr/bin/env python3

这样，你的程序应该像现在这样：\#!/usr/bin/env python3
print('Hello World')

第二，我们必须给程序的可执行权限－－使用' chmod '命令，然后运行源程序。
这里使用的chmod命令是change the mode of the file （改变文件的模式），通过给系统All(所有)用户execute（可执行）权限。
$ chmod a+x hello.py
现在，我们可以直接运行我们的程序，因为我们的操作系统调用“/usr/bin/env”，它按次序找到我们的Python 3软件，然后知道如何运行我们的源文件：
$ ./hello.py
Hello World
我们使用“./”用来指示程序在当前目录中。
为了让事情变得更有趣，你可以将文件重命名为“hello”然后像“./hello”这样运行它，因为系统知道它必须使用源代码文件中第一行指定位置的解释器运行它，所以它仍将工作。
到目前为止,我们已经能够运行我们的程序,只要我们知道确切的路径。如果我们希望能够运行该程序从文件夹?您可以通过将程序存储在列在“路径”环境变量中的一个文件夹中来实现。
无论你何时运行任何程序，系统在列在“PATH”（路径）环境变量中的每个文件夹查找该程序，然后运行程序。我们通过简单地复制这个源文件到“PATH”（路径）列出的目录中，可以让这个程序到处可用。
$ echo $PATH
/usr/local/bin:/usr/bin:/bin:/usr/X11R6/bin:/home/swaroop/bin
$ cp hello.py /home/swaroop/bin/hello
$ hello
Hello World
使用'echo'（回响）命令和变量（PATH）名称加前缀'$'，指示shell我们需要“环境变量”的值，我们可以显示“PATH”（路径）变量的值。我们看到,“/home/swaroop/bin”是PATH变量中的一个目录，* swaroop *是在我的系统中我用的用户名，通常在你的系统中为你的用户名，会有一个类似的目录。
如果你想添加您所选择的目录到“PATH”变量中——这可以通过运行“export PATH=$PATH:/home/swaroop/mydir”,“:/home/swaroop/mydir”是我想添加到“PATH”变量中的目录。
如果你想写可以在任何时间、在任何地点运行的命令，这个方法非常有用。这就像创建你自己的在命令行使用的命令，就像“cd”或任何其他命令。

