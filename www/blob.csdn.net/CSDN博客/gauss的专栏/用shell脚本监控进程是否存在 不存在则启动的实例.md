# 用shell脚本监控进程是否存在 不存在则启动的实例 - gauss的专栏 - CSDN博客
2016年09月01日 19:44:45[gauss](https://me.csdn.net/mathlmx)阅读数：362
个人分类：[linux](https://blog.csdn.net/mathlmx/article/category/2303095)

用shell脚本监控进程是否存在 不存在则启动的实例，先上代码干货：
#!/bin/sh
ps -fe|grep processString |grep -v grep
if [ $? -ne 0 ]
then
echo "start process....."
else
echo "runing....."
fi
#####
processString 表示进程特征字符串，能够查询到唯一进程的特征字符串
0表示存在的
$? -ne 0 不存在，$? -eq 0 存在
-------------------------------------------------------------
定时执行：
crontab -e
0 04,12,21 * * * /bin/bash /home/mysh/monitorprocess.sh
每天晚上4点，中午12点,晚上21点检测
0 4,12 * * * 
分 时 日 月 周
* 04,12 * * * 这个是指4点和12点中每分钟执行一次
0 4,12 * * *  4点12点 第0分钟执行一次
详细内容请看本人百度经验： 用shell脚本监控进程是否存在 不存在则启动教程_百度经验
[http://jingyan.baidu.com/article/59a015e3a3a404f795886542.html](http://jingyan.baidu.com/article/59a015e3a3a404f795886542.html)
=======================================
扩展相关知识：
shell的if else 语法以及大于,小于等逻辑表达式：
if ....; then
　 ....
elif ....; then
　 ....
else
　 ....
fi
大多数情况下，可以使用测试命令来对条件进行测试。比如可以比较字符串、判断文件是否存在及是否可读等等… 　　通常用" [ ] "来表示条件测试。注意这里的空格很重要。要确保方括号的空格。
[ -f "somefile" ] ：判断是否是一个文件
[ -x "/bin/ls" ] ：判断/bin/ls是否存在并有可执行权限
[ -n "$var" ] ：判断$var变量是否有值
[ "$a" = "$b" ] ：判断$a和$b是否相等          -r file　　　　　用户可读为真
　　-w file　　　　　用户可写为真
　　-x file　　　　　用户可执行为真
　　-f file　　　　　文件为正规文件为真
　　-d file　　　　　文件为目录为真
　　-c file　　　　　文件为字符特殊文件为真
　　-b file　　　　　文件为块特殊文件为真
　　-s file　　　　　文件大小非0时为真
　　-t file　　　　　当文件描述符(默认为1)指定的设备为终端时为真 
######################################################### 含条件选择的shell脚本
    对于不含变量的任务简单shell脚本一般能胜任。但在执行一些决策任务时，就需要包含if/then的条件判断了。shell脚本编程支持此类运算，包括比较运算、判断文件是否存在等。基本的if条件命令选项有： - eq —比较两个参数是否相等（例如，if [ 2 –eq 5 ]）
-ne —比较两个参数是否不相等
-lt —参数1是否小于参数2
-le —参数1是否小于等于参数2
-gt —参数1是否大于参数2
-ge —参数1是否大于等于参数2
-f — 检查某文件是否存在（例如，if [ -f "filename" ]）
-d — 检查目录是否存在
几乎所有的判断都可以用这些比较运算符实现。脚本中常用-f命令选项在执行某一文件之前检查它是否存在。 ################################################################## 判断文件是否存在 #!/bin/sh
today=`date -d yesterday +%y%m%d`
file="apache_$today.tar.gz"
cd /home/chenshuo/shell
if [ -f "$file" ];then
echo "OK"
else
echo "error $file" >error.log
mail -s "fail backup from test" chenshuo@soufun.com <error.log
fi
=============================================
shell基本命令
（1）ps aux    显示系统全部进程，一行一个
（2）grep “abc” 从标准输入读取字符流，输出包含字符串“abc”的行
（3）grep -v "acb"   从标准输入读取字符流，输出不包含字符串“abc”的行
（4）wc -l        从标准输入读取字符流，输出行数
例如需要检测进程httpd是否存在，操作流程如下：
（1）读取系统所有进程
（2）判断包含指定进程名字的信息是否存在
通过管道连接，命令如下：
ps axu      |    grep "httpd"           |      grep -v "grep"    |      wc -l
所有进程-->获取包含“httpd”的行-->删除grep进程信息-->输出最后的行数
通过判断命令的执行结果是否为 0 ，可以知道进程是否存在。
脚本如下:
#!/bin/sh
while true;do
        count=`ps -ef|grep http|grep -v grep`
        if [ "$?" != "0" ];then
echo    ">>>>no httpd,run it"
service httpd start
else
echo ">>>>httpd is runing..."
fi
sleep 5
done
