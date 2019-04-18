# Shell中的条件判断语句if~then~fi - bigfacesafdasgfewgf - CSDN博客





2015年05月18日 08:59:51[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：21322标签：[shell																[条件判断语句																[if																[elif																[case](https://so.csdn.net/so/search/s.do?q=case&t=blog)
个人分类：[Shell](https://blog.csdn.net/puqutogether/article/category/3236387)








   Shell中的条件判断语句是前面一篇“Shell中的条件测试语句”的升级篇，也就是说，前面的测试语句是为了现在的判断语句if~then~fi语句服务的。

    我们还是按照注意点和代码实现的方式铺开：



**    1）基本的if-then-fi语句可以用来判断基本的单层的分支结构，其形式如下：**

![](https://img-blog.csdn.net/20150518091015104?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

其中if后面的测试语句一般都使用[]命令来做。如下面的例子：



```
<span style="font-size:14px;">#-----------------------------/chapter4/ex4-18.sh------------------
#! /bin/sh
#使用条件测试判断/bin/bash是否是一个常规文件
if [ -f /bin/bash ]; then 
	echo "/bin/bash is a file"
fi
</span>
```

**    2）if-then-else-fi语句可以处理两层的分支判断语句。如下：**


![](https://img-blog.csdn.net/20150518091447827?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




```
<span style="font-size:14px;">#-----------------------------/chapter4/ex4-22.sh------------------
#! /bin/sh

#输出提示信息
echo "Please enter a number:"
#从键盘读取用户输入的数字
read num
#如果用户输入的数字大于10
if [ "$num" -gt 10 ]; then
		#输出大于10的提示信息
 	echo "The number is greater than 10."
#否则
else
	#输出小于或者等于10的提示信息
		echo "The number is equal to or less than 10."
fi
</span>
```





**    3）if-then-elif-then-elif-then-...-else-fi。这种语句可以实现多重判断，注意最后一定要以一个else结尾。如下：**

![](https://img-blog.csdn.net/20150518091544467?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




```
<span style="font-size:14px;">#-----------------------------/chapter4/ex4-24.sh------------------
#! /bin/sh

echo "Please enter a score:"

read score

if [ -z "$score" ]; then
   echo "You enter nothing.Please enter a score:"
   read score
else
   if [ "$score" -lt 0 -o "$score" -gt 100 ]; then
      echo "The score should be between 0 and 100.Please enter again:"
      read score
   else
      #如果成绩大于90
      if [ "$score" -ge 90 ]; then
         echo "The grade is A."
      #如果成绩大于80且小于90
      elif [ "$score" -ge 80 ]; then
         echo "The grade is B."
      #如果成绩大于70且小于80
      elif [ "$score" -ge 70 ]; then
         echo "The grade is C."
      #如果成绩大于60且小于70
      elif [ "$score" -ge 60 ]; then
         echo "The grade is D."
      #如果成绩小于60
      else
         echo "The grade is E."
      fi
   fi
fi
</span>
```

**    4）要退出程序的时候，可以使用exit status语句。退出的状态status为0的时候，意味着脚本成功运行结束；非0表示程序执行过程出现某些错误，不同的错误对应着不同的退出状态。尽管用户可以自定义程序中的退出状态status，但是通常情况下每个status都有特定的含义，因此在自定义返回status的时候，一定要避免造成歧义。例子如下：**




```
<span style="font-size:14px;">01   #-----------------------------/chapter4/ex4-26.sh------------------
02   #! /bin/sh
03   
04   #如果文件已经存在，则直接退出，文件名时输入的第一个参数
05   if [ -e "$1" ]
06   then
07      echo "file $1 exists."
08      exit 1
09   #如果文件不存在，则创建文件，使用touch来创建文件，也可以使用重定向来创建文件echo "Hello~" > ./test.log  即在当前目录下新建一个test.log文件
10   else
11      touch "$1"
12      echo "file $1 has been created."
13      exit 0
14   fi
</span>
```

**    5）case-esac语句实现多重条件判断。如下：**


![](https://img-blog.csdn.net/20150518093438905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**    注意：每一个变量内容的程序段最后都需要两个分号 (;;) 来代表该程序段落的结束；每个变量情况最后都要有)结尾；其余情况用*)来表示；最后要用esac来结束，即case反过来。**



```
<span style="font-size:14px;">#-----------------------------/chapter4/ex4-27.sh------------------
#! /bin/sh

#输出提示信息
echo "Hit a key,then hit return."
#读取用户按下的键
read keypress
#case语句开始
case "$keypress" in
   #小写字母
   [[:lower:]])
      echo "Lowercase letter.";;
   #大写字母
   [[:upper:]])
      echo "Uppercase letter.";;
   #单个数字
   [0-9])
      echo "Digit.";;
   #其他字符
   *)
      echo "other letter.";;
esac
</span>
```





    参考：

    《鸟哥的Linux私房菜》

    《Shell从入门到精通》](https://so.csdn.net/so/search/s.do?q=elif&t=blog)](https://so.csdn.net/so/search/s.do?q=if&t=blog)](https://so.csdn.net/so/search/s.do?q=条件判断语句&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)




