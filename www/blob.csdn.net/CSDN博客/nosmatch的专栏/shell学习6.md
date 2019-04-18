# shell学习6 - nosmatch的专栏 - CSDN博客
2011年08月26日 08:49:28[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：485标签：[shell																[脚本																[signal																[file																[测试																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[shell](https://blog.csdn.net/HDUTigerkin/article/category/874908)
把这几天的笔记记录如下：
1.赋值的一些办法
- variable1=`<file1`      #  将"file1"的内容放到"variable1"中.  
- variable2=`cat file2`   #  将"file2"的内容放到"variable2"中. 
- ############################################################ 
- var=`echo "one two three"` # 在这个方法中set --$var 后，有one=$1 
- set -- $var                # two=$2,three=$3，其中$1,$2,$3是位置参数 
2.重定向和文件描述符
- : > filename #将会把文件"filename"变为一个空文件(就是size为0），如果  
-              #文件不存在，就创建一个长度为0的文件（同touch）。 
- 
- N<>filename  #为了读写"filename", 把文件"filename"打开, 并且将文件描 
-              #描述符N分配给它。如果filename不存在，就创建。如果N没有 
-              #指定，那默认就是fd 0，stdin 。 
- 
-                  示例 
- echo 1234567890 > File    # 写字符串到"File".  
- exec 3<> File             # 打开"File"并且将fd 3分配给它.  
- read -n 4 <&3             # 只读取4个字符.  
- echo -n . >&3             # 写一个小数点.  
- exec 3>&-                 # 关闭fd 3. 
- cat File                  # ==> 1234.67890 
-          关闭文件描述符 
- n<&-        # 关闭输入文件描述符n 
- 0<&-, <&-   # 关闭stdin 
- n>&-        # 关闭输出文件描述符n 
- 1>&-, >&-   # 关闭stdout 
- 
-          ####比较#### 
- ls -yz >> command.log 2>&1 # 将错误选项"yz"的结果放到 
-                            # 文件command.log中，因为 stderr                                  
-                            # 被重定向到这个文件中，所有的错误消息  
-                            # 也就被指向那里了（进入command.log）。  
- 
- ls -yz 2>&1 >> command.log # 输出一个错误消息, 但是并不写到文件中 
- 
- exec <filename # 将stdin重定向到文件中 
- 
- 
- 
再给出一个脚本：
- /bin/bash 
- # 使用'exec'重定向stdin. 
- 
- 
- exec 6<&0          # 将文件描述符#6与stdin链接起来. 
-                    # 保存stdin. 
- 
- exec <data-file   # stdin被文件"data-file"所代替. 
- 
- read a1            # 读取文件"data-file"的第一行. 
- read a2            # 读取文件"data-file"的第二行. 
- 
- echo 
- echo "Following lines read from file." 
- echo "-------------------------------" 
- echo $a1 
- echo $a2 
- 
- echo; echo; echo 
- 
- exec 0<&6 6<&- 
- #  现在将stdin从fd #6中恢复, 因为刚才我们把stdin重定向到#6了, 
- #+ 然后关闭fd #6 ( 6<&- ), 好让这个描述符继续被其他进程所使用. 
- # 
- # <&6 6<&-    这么做也可以. 
- 
- echo -n "Enter data  " 
- read b1  # 现在"read"已经恢复正常了, 就是能够正常的从stdin中读取. 
- echo "Input read from stdin." 
- echo "----------------------" 
- echo "b1 = $b1" 
- 
- echo 
- exit 0 
- [max@localhost xx]$ clear 
- [max@localhost xx]$ cat 2 
- /bin/bash 
- # 使用'exec'重定向stdin. 
- 
- 
- exec 6<&0          # 将文件描述符#6与stdin链接起来. 
-                    # 保存stdin. 
- 
- exec <data-file   # stdin被文件"data-file"所代替. 
- 
- read a1            # 读取文件"data-file"的第一行. 
- read a2            # 读取文件"data-file"的第二行. 
- 
- echo 
- echo "Following lines read from file." 
- echo "-------------------------------" 
- echo $a1 
- echo $a2 
- 
- echo; echo; echo 
- 
- exec 0<&6 6<&- 
- #  现在将stdin从fd #6中恢复, 因为刚才我们把stdin重定向到#6了, 
- #+ 然后关闭fd #6 ( 6<&- ), 好让这个描述符继续被其他进程所使用. 
- # 
- # <&6 6<&-    这么做也可以. 
- 
- echo -n "Enter data  " 
- read b1  # 现在"read"已经恢复正常了, 就是能够正常的从stdin中读取. 
- echo "Input read from stdin." 
- echo "----------------------" 
- echo "b1 = $b1" 
- 
- echo 
- exit 0 
- 
- 
3. shell中函数返回值最大的正整数是255.‘
4.调试shell的方法有下面这些
a): echo语句可以放在脚本中存在疑问的位置上, 来观察变量的值, 也可以了解脚本后续的动作;
b): 使用过滤器tee来检查临界点上的进程或数据流;
c)：设置选项-n -v -x
      sh -n scriptname不会运行脚本, 只会检查脚本的语法错误. 这等价于把set -n或set -o noexec插入脚本中。注意, 某些类型的语法错误不会被这种方式检查出来。
      sh -v scriptname将会在运行脚本之前, 打印出每一个命令. 这等价于把set -v或set -o verbose插入到脚本中。
      选项-n和-v可以同时使用. sh -nv scriptname将会给出详细的语法检查。
      sh -x scriptname会打印出每个命令执行的结果, 但只使用缩写形式。 这等价于在脚本中插入set -x或set -o xtrace。
      把set -u或set -o nounset插入到脚本中, 并运行它, 就会在每个试图使用未声明变量的地方给出一个unbound variable错误信息. 
d):使用"assert"(断言)函数在脚本的临界点上测试变量或条件;
e):使用变量$LINENO和内建命令caller;
f):用trap捕获exit等信号，捕获exit在某些情况下很有用, 比如说强制"打印"变量值。trap命令必须放在脚本中第一个命令的位置上。
trap '' SIGNAL (两个引号之间为空)在剩余的脚本中禁用了SIGNAL信号的动作。trap SIGNAL则会恢复处理SIGNAL的动作。当你想保护脚本的临界部分不受意外的中断骚扰, 那么上面讲的这种办法就非常有用了。
如下:
- trap '' 2  # 信号2就是Control-C, 现在被禁用了.  
- command 
- command 
- command 
- trap 2     # 重新恢复Control-C 
- 
用trap捕获exit信号，来触发操作的脚本示例：
- #!/bin/bash 
- 
- trap 'echo Variable Listing -----a = $a b = $b' EXIT 
- 
- echo "This prints before the \"trap\" --" 
- echo "Even though the script sees the \"trap\" first." 
- echo 
- 
- a=39
- 
- b=36
- 
- exit 0 
输出一下结果：
- This prints before the "trap" -- 
- Even though the script sees the "trap" first. 
- 
- Variable Listing -----a = 39b = 36
5。shell选项，shell有很多选项，如下图所示：
![](http://img1.51cto.com/attachment/201103/195911762.jpg)
     好了，这是这几天的笔记、、、、、
