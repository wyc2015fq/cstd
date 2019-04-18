# 【Linux入门学习之】数据流重定向 - 江南烟雨 - CSDN博客
2011年12月22日 15:32:49[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：2027
Linux中数据流重定向的意思是将某个命令执行后应该要出现在屏幕上的数据传输到其他地方，例如文件或设备之类。
## 一、stadard output与standard error output
首先介绍一下命令执行过程的数据传输情况：
![](http://hi.csdn.net/attachment/201112/22/0_1324539275bs17.gif)
图中的standard output和standard error output分别代表“标准输出”和“标准错误输出”。这两个命令默认都是输出到屏幕中来。
标准输出：命令执行所回传的正确的信息。
标准错误输出：命令执行失败后，所回传的错误信息。
我们通过数据流重定向功能可以将这两条信息分开，分别传送到其他的文件或设备中去。
传送使用的特殊字符是：
标准输入(stdin)：代码为0，使用<或<<；
标准输出(stdout)：代码为1，使用>或>>；
标准错误输出(stderr)：代码为2，使用2>或>>。
范例：
命令ll /：屏幕会显示文件名信息。
而ll / > /tmp/fileinfo：屏幕无任何信息。
ll /tmp/fileinfo：可以发现有一个新文件被创建（文件不存在时），里面的内容就是原本应该显示在屏幕上的信息。
要注意的是，如果我再次使用重定向功能>:ll /home > /tmp/fileinfo，将信息重定向到文件/tmp/info中，则原有的信息会被覆盖(文件已存在时)。如果要将数据叠加而不是覆盖，那使用符号>>就可以了。
stdout代码为1，当我们仅用>和>>默认就是1>,1>>。而当我们要重定向standard error output数据时，用2>和2>>即可。其意义和stdout一样。
下面再来看一个范例：
*find /home –name .bashrc> list*
可以发现list里面存储了正确的信息，但是错误的信息还是显示在了屏幕上面。那么如果要将正确信息和错误信息分别存入不同的文件呢，应该怎么办？
*find /home –name.bsdhrc > list_right 2> list_error*
这样就可以了。
## **二、/dev/null垃圾黑洞设备与特殊写法**
此功能适用于我们知道有错误信息要发生，我们不想将其存储或者显示出来而将其忽略。
范例：
*find /home –name.bashrc 2> /dev/null*
这样屏幕上就只有正确的数据，错误信息被忽略了。
如果我们想将正确数据和错误数据写入同一个文件呢？这个时候就要适用特殊写法了。
范例：将命令的数据全部写入list文件中。
*find /home –name.bashrc >list 2> list   //错误*
*find /home –name.bashrc >list 2> &1   //正确：推荐*
*find /home –name.bashrc &>list       //正确*
第一行错误的原因是：两条数据同时写入一个文件，又没有使用特殊写法，此时两条数据会交叉写入该文件内，造成次序错乱。最终产生的文件里面数据排列会很混乱。
## **三、standard input:<和<<**
可以将原本由键盘输入的数据由文件替代。
范例：
*cat > catfile< ~/.bashrc*
可以发现catfile里面的内容就是~/.bashrc里面的内容。
而<<代表的是结束输入的意思。
范例：我要用cat直接将输入的信息输出到catfile中，而且由键盘输入eof结束。可以这样做：
*cat > catfile<< “eof”*
*> This is atest*
*> OK,I willstop.*
*> eof*
这样就不必使用Ctrl+d来结束。
