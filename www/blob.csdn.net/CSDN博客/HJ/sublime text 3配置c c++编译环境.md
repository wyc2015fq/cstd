# sublime text 3配置c/c++编译环境 - HJ - CSDN博客
2017年01月25日 15:21:31[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：1506
个人分类：[环境配置和安装教程](https://blog.csdn.net/feizaoSYUACM/article/category/6695121)
sublime text 3提供了构建功能，它的构建系统（Build systems）可以运行一段外部命令，还可以捕获输出并显示。
要在sublime text 3中实现c或c++代码的编译和运行，在本质上说也是调用外部的命令，windows中也可以理解为执行一段cmd命令。
目前c/c++编译器最流行的就是gcc和g++，本文将先介绍gcc和g++的基本命令格式，然后介绍win7 64bit下 Sublime Text 3 build 3083版本中build配置文件的编写。
## 关于gcc和g++
安装编译器是后面所有工作的基础，如果没有编译器，后面的一切都无从谈起。在windows下使用gcc和g++，是通过安装MinGW实现的。 
安装MinGW
MinGW是Minimalist GNU on Windows的首字母缩写，安装后就可以使用很多的GNU工具。GNU（GNU’s Not Unix）是linux中的一个著名的项目，包含了gcc\g++\gdb等工具。也就是说，安装MinGw后，我们就可以使用gcc和g++命令了。
MinGW的官网是 **[http://www.mingw.org/](http://www.mingw.org/)** ，但是从官网安装很麻烦，在线安装经常龟速容易失败。
雅乐网推荐的方法是借助codeblocks，选择带有mingw的版本安装，安装后把mingw文件夹复制出来就可以了。
这里提供了解压版的MinGW，是使用 codeblocks-13.12mingw-setup 安装后复制出来的：
**[http://pan.baidu.com/s/1gd5YzVP](http://pan.baidu.com/s/1gd5YzVP)**
解压后，可以在 MinGW/bin 目录下找到我们需要的gcc.exe和g++.exe 。
我这里把MinGW文件夹放到c盘根目录
![这里写图片描述](http://7d9rd6.com1.z0.glb.clouddn.com/wp-content/uploads/2015/02/scrn20151003205109.png)
## 在cmd中使用gcc
假设我们有一个test.c文件在Z盘的work目录下。首先我们要在cmd中进入此目录。方法可以是在work目录空白处按住Shift点击鼠标右键，选择“在此处打开命令窗口”；也可以使用cd命令进入。 
![这里写图片描述](http://7d9rd6.com1.z0.glb.clouddn.com/wp-content/uploads/2015/02/scrn20151003210112.png)
gcc的一般格式是
```
gcc 源文件名 -o 可执行文件名
```
但是我们输入命令
```
gcc test.c -o test
```
执行后却提示
‘gcc’ 不是内部或外部命令，也不是可运行的程序或批处理文件。
这是因为命令执行时，会在当前目录下查找名为gcc的可执行文件，如果查不到就在系统环境变量path记录的路径里寻找gcc可执行文件。但是目前这两个地方都没有。我们的gcc文件所在的目录是c盘下的MinGW/bin。
这时可以使用绝对路径来调用gcc可执行文件
```
Z:\work>c:/MinGW/bin/gcc test.c -o test
Z:\work>test.exe
hello world
```
这样就成功编译生成了可执行文件test.exe，然后就可以在cmd里运行了。
![这里写图片描述](http://7d9rd6.com1.z0.glb.clouddn.com/wp-content/uploads/2015/02/scrn20151003210705.png)
## 配置环境变量
为了方便，一般我们会把gcc所在的路径加入系统的环境变量，这样就可以直接使用gcc命令而不用绝对路径。
右键计算机->属性->高级系统设置->环境变量 
![这里写图片描述](http://ww4.sinaimg.cn/mw690/6958b27dgw1eexlfpt744j20g40e1mz9.jpg)
在path的值中，可以发现有一些目录，他们之间用英文的分号分隔。我们双击path，把我们gcc的路径  C:\MinGW\bin  添加进去。   要注意前后的英文分号。 
![这里写图片描述](http://ww3.sinaimg.cn/mw690/6958b27dgw1eexlfqdb33j209x041mxb.jpg)
确定以后 就可以在任意目录下直接使用gcc命令了。可以在任意目录打开cmd窗口，输入gcc查看环境变量是否设置成功。如果仍然提示不是内部或外部命令，说明环境变量设置失败。
![这里写图片描述](http://ww2.sinaimg.cn/mw690/6958b27dgw1eexlfqwxk7j2094058glt.jpg)
**注意：在sublime text 3 build 3083中，环境变量的修改不会立即在sublime中生效，需要重启windows。**
## cmd编译运行c语言
总结一下流程：
首先我们要在cmd中进入.c文件所在的目录作为工作目录
然后执行gcc source.c -o dest来生成可执行文件
最后输入生成的可执行文件名来运行生成的程序。
建议大家加入-Wall选项，打开常用的警告。
下面是几种常用的命令：
编译c语言
```
gcc -Wall 源文件名 -o 可执行文件名
```
编译c++语言
```
g++ -Wall 源文件名 -o 可执行文件名
```
# Sublime Text 3默认c/c++编译系统的不足
## 编译系统
把g++加入环境变量后，sublime中默认的编译系统就可以正常使用了。
我们在Sublime Text 3中打开一个cpp文件，按Ctrl+B    
![这里写图片描述](http://7d9rd6.com1.z0.glb.clouddn.com/wp-content/uploads/2015/02/2015-10-03-21_22_45-Z__t.cpp-Sublime-Text-UNREGISTERED.png)
这是sublime自带的默认c++编译命令。第一个是编译，第二个是运行。这时候是可以正常使用的。（环境变量配置后需重启windows）
![](http://7d9rd6.com1.z0.glb.clouddn.com/wp-content/uploads/2015/02/scrn20151003212409.png)
Sublime Text 3 3080版本之后修改了编译系统，具体设置是
Ctrl+B 执行改格式上次的编译命令。如果第一次执行则提示选择执行哪个
Ctrl+Shift+B 选择执行哪个
## 不足之处：
- 
程序输出捕获到Sublime窗口中，这样导致不能运行时输入信息。执行含有scanf语句的代码会卡住。
- 
默认情况下c和c++没有进行区分，全部当做c++格式来处理了。
解决办法
第一个是设置在新的cmd窗口执行程序，这样就可以输入信息。
第二个是针对c语言单独写一个build配置文件。
## 默认的编译配置文件在哪
在Sublime的安装目录的Packages文件夹中，有个文件叫C++.sublime-package
![这里写图片描述](http://7d9rd6.com1.z0.glb.clouddn.com/wp-content/uploads/2015/02/scrn20151003213410.png)
这个实际上是zip的压缩包包含了c++的默认系统设置，修改后缀名为zip后解压，可以在里面找到C++ Single File.sublime-build文件，内容如下：
```
{
    "shell_cmd": "g++ \"${file}\" -o \"${file_path}/${file_base_name}\"",
    "file_regex": "^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)$",
    "working_dir": "${file_path}",
    "selector": "source.c, source.c++",
    "variants":
    [
        {
            "name": "Run",
            "shell_cmd": "g++ \"${file}\" -o \"${file_path}/${file_base_name}\" && \"${file_path}/${file_base_name}\""
        }
    ]
}
```
这是JSON格式的配置文件，可以看到 selector部分确实是c和c++都选择的。
我们只要略作修改，就可以实现我们的需要了。
但是这是系统的配置，并不建议修改。建议大家把用户配置放到用户文件夹下，来代替默认的编译配置。
# 新建编译系统
## c语言
选择tool –> Build System –> New Build System 
![这里写图片描述](http://7d9rd6.com1.z0.glb.clouddn.com/wp-content/uploads/2015/02/scrn20150221161646.png)
然后输入以下代码
```
{
    "working_dir": "$file_path",
    "cmd": "gcc -Wall \"$file_name\" -o \"$file_base_name\"",
    "file_regex": "^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)$",
    "selector": "source.c",
    "variants": 
    [
        {   
        "name": "Run",
            "shell_cmd": "gcc -Wall \"$file\" -o \"$file_base_name\" && start cmd /c \"\"${file_path}/${file_base_name}\" & pause\""
        }
    ]
}
```
按Ctrl+s保存，会自动打开user目录（Sublime Text 3\Packages\User），我们修改 文件名为 c.sublime-build，保存在此目录。
![这里写图片描述](http://7d9rd6.com1.z0.glb.clouddn.com/wp-content/uploads/2015/02/scrn20150221162528.png)
这时候，可以在Tools -> Build System下看到刚才新建的c了 
![这里写图片描述](http://7d9rd6.com1.z0.glb.clouddn.com/wp-content/uploads/2015/02/2015-07-20-21_29_56-.png)
选中后就可以使用了。
Build System中除了选择具体的编译系统，还可以选择第一个：Automatic 自动选择，会根据打开的文件后缀自动选择。由于默认情况下.c文件sublime识别为c++类型，所以使用自动选择的时候还需要修改一点：
先用sublime打开.c文件的时候 默认是c++格式 。（注：最新的3013版本已经默认是c格式，则不必修改） 
![这里写图片描述](http://7d9rd6.com1.z0.glb.clouddn.com/wp-content/uploads/2014/12/scrn20141219154819.png)
点击红色箭头处的c++  选择Open all with current extension as .. 然后选择C
![这里写图片描述](http://7d9rd6.com1.z0.glb.clouddn.com/wp-content/uploads/2014/12/scrn20141219154915.png)
这样以后打开.c文件就默认是c类型
这时候按Ctrl+Shift+B
![这里写图片描述](http://7d9rd6.com1.z0.glb.clouddn.com/wp-content/uploads/2015/02/2015-10-03-21_42_36-Z__c.c-Sublime-Text-UNREGISTERED.png)
第三个c就是对应执行配置文件中的第三行  gcc -Wall $file_name -o $file_base_name  作用是编译。
第四个c-Run对应后面的命令  gcc -Wall $file -o $file_base_name && start cmd /c \”${file_path}/${file_base_name} & pause\” ，作用是是在新的cmd窗口运行。这样就可以对scanf等函数进行输入了。
## c++
gcc虽然可以编译c++代码，但是不能进行c++的连接函数库操作。所以针对c++代码一般使用g++来编译。
方法和上面的c语言的配置一样，只要把配置文件中的gcc改为g++ ，source.c改为source.c++ ，保存文件名c.sublime-build改为c++.sublime-build就可以了。
这里增加了-std=c++11 选项，是按照C++11标准进行编译，不需要的话可以去掉，配置文件如下：
```
{
    "encoding": "utf-8",
    "working_dir": "$file_path",
    "shell_cmd": "g++ -Wall -std=c++11 \"$file_name\" -o \"$file_base_name\"",
    "file_regex": "^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)$",
    "selector": "source.c++",
    "variants": 
    [
        {   
        "name": "Run",
            "shell_cmd": "g++ -Wall -std=c++11 \"$file\" -o \"$file_base_name\" && start cmd /c \"\"${file_path}/${file_base_name}\" & pause\""
        }
    ]
}
```
这个配置文件编译的时候也会运行  g++ -Wall -std=c++0x $file_name -o $file_base_name && cmd /c ${file_path}/${file_base_name} ，如果只想编译，可以把&&后面去掉就可以了。
实际上，我们可以利用Varians ，来配置多个不同的编译命令。例如下面的配置文件有编译 ，捕获输出运行，cmd运行三种
![这里写图片描述](http://7d9rd6.com1.z0.glb.clouddn.com/wp-content/uploads/2015/02/2015-10-03-21_55_43-Z__t.cpp-Sublime-Text-UNREGISTERED.png)
```
{
    "encoding": "utf-8",
    "working_dir": "$file_path",
    "shell_cmd": "g++ -Wall -std=c++11 \"$file_name\" -o \"$file_base_name\"",
    "file_regex": "^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)$",
    "selector": "source.c++",
    "variants": 
    [
        {   
        "name": "Run in sublime",
            "shell_cmd": "g++ -Wall -std=c++11 \"$file_name\" -o \"$file_base_name\" && cmd /c \"${file_path}/${file_base_name}\""
        },
        {   
        "name": "CMD Run",
            "shell_cmd": "g++ -Wall -std=c++11 \"$file\" -o \"$file_base_name\" && start cmd /c \"\"${file_path}/${file_base_name}\" & pause\""
        }
    ]
}
```
