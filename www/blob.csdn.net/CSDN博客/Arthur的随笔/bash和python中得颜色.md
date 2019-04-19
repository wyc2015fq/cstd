# bash和python中得颜色 - Arthur的随笔 - CSDN博客
2011年11月15日 21:08:15[largetalk](https://me.csdn.net/largetalk)阅读数：2373标签：[bash																[python																[shell																[工作																[终端](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=bash&t=blog)
个人分类：[linux																[python](https://blog.csdn.net/largetalk/article/category/715661)](https://blog.csdn.net/largetalk/article/category/727912)
在bash中想使输出得字符串有颜色，或者高亮某些字符，这就需要在输出得字符中插入一些转义字符，如
`echo -e "\e[1;31mabc\e[0m"`
就会高亮输出abc
bash中\e[x;ym是设置某些属性， \e[0m关闭所有属性，\e也可以用\033来代替
上面例子中x是1表示高亮，x是0表示不高亮， y是31表示前景红色，41表示背景红, 其实，十位3和4表示前后背景，个位0-7表示黑、红、绿、橙、蓝、品红、青、白色。
so，要输出前景红背景绿得字符就是
`echo -e "\033[42m\e[1;31mabc\e[0m"`
其他还有一些转义字符如下：
\33[0m   关闭所有属性
\33[1m   设置高亮度
\33[4m   下划线
\33[5m   闪烁
\33[7m   反显
\33[8m   消隐
\33[30m -- \33[37m   设置前景色
\33[40m -- \33[47m   设置背景色
\33[nA   光标上移n行
\33[nB   光标下移n行
\33[nC   光标右移n行
\33[nD   光标左移n行
\33[y;xH设置光标位置
\33[2J   清屏
\33[K    清除从光标到行尾的内容
\33[s    保存光标位置
\33[u    恢复光标位置
\33[?25l   隐藏光标
\33[?25h   显示光标
所有的转移字符表:
|\a|ASCII 响铃字符（也可以键入 \007）|
|----|----|
|\d|"Wed Sep 06" 格式的日期|
|\e|ASCII 转义字符（也可以键入 \033）|
|\h|主机名的第一部分（如 "mybox"）|
|\H|主机的全称（如 "mybox.mydomain.com"）|
|\j|在此 shell 中通过按 ^Z 挂起的进程数|
|\l|此 shell 的终端设备名（如 "ttyp4"）|
|\n|换行符|
|\r|回车符|
|\s|shell 的名称（如 "bash"）|
|\t|24 小时制时间（如 "23:01:01"）|
|\T|12 小时制时间（如 "11:01:01"）|
|\@|带有 am/pm 的 12 小时制时间|
|\u|用户名|
|\v|bash 的版本（如 2.04）|
|\V|Bash 版本（包括补丁级别） ?/td>|
|\w|当前工作目录（如 "/home/drobbins"）|
|\W|当前工作目录的“基名 (basename)”（如 "drobbins"）|
|\!|当前命令在历史缓冲区中的位置|
|\#|命令编号（只要您键入内容，它就会在每次提示时累加）|
|\$|如果您不是超级用户 (root)，则插入一个 "$"；如果您是超级用户，则显示一个 "#"|
|\xxx|插入一个用三位数 xxx（用零代替未使用的数字，如 "\007"）表示的 ASCII 字符|
|\\|反斜杠|
|\[|这个序列应该出现在不移动光标的字符序列（如颜色转义序列）之前。它使 bash 能够正确计算自动换行。|
|\]|这个序列应该出现在非打印字符序列之后。|
比如我在bashrc里设置PS1='\[\033[01;31m\]\u@\h\[\033[01;34m\]: \w \$\[\033[00m\] '， 则我bash得prompt就是彩色的了。
有时候我们在写程序得时候想把输出得字符也能带有颜色，比如程序产生得log，error得用红色标记使得更醒目等待，就需要在输出得字符窜里也加上写转义字符。
python中写法是
```python
print '\x1b[0;33madaf\x1b[0m'
```
除了把\e换成\x1b其他都一样， \x1b转化成10进制是27， ESC的ascii码, 换成八进制就是\033，哈哈，原来还是一样得东西。
            
