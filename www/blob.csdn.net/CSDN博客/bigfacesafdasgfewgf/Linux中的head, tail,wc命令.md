# Linux中的head, tail,wc命令 - bigfacesafdasgfewgf - CSDN博客





2015年05月07日 22:36:14[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：2805











    Linux系统中读取文件还有几个命令：head, tail, wc。我们解释一下这几个命令。




# 1. head：显示前面几行

head file：显示1-10行（默认为10行）

head -n 20 file：显示1-20行

head -n -20 file：显示1-120行，-20表示不显示后20行





# 2. tail：显示后面几行

tail与head类似，只说明“+”的用法：

tail -n +100 file：显示10-140行，+100表示不显示前99行




如果要显示10~20行：head -n 20 | tail -n 10    这个是管道命令







# 3. wc：查询文件中有多少字，多少行，多少字符

例如：cat /etc/man.config | wc




-l: 列出多少行

-w：列出多少字

-m：列出多少字符












