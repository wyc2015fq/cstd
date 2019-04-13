
# adb 抓取日志到指定文件，文件命名为当前电脑时间 - 嵌入式Linux - CSDN博客

2018年10月12日 15:11:56[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：339



# 需求产生原因
我们保存日志的时候，需要把日志文件名命名为当前的系统时间，方便我们以后查看日志。
# 命令如下
`adb logcat > "%date:~0,4%-%date:~5,2%-%date:~8,2% %time:~0,2%-%time:~3,2%-%time:~6,2%.txt"``rem 根据当前日期获取，年月日串  
set yyyy=%date:~,4%  
set mm=%date:~5,2%  
set day=%date:~8,2%   
set "YYYYmmdd=%yyyy%%mm%%day%"  
rem 把年月日串中的空格替换为0  
set "YYYYmmdd=%YYYYmmdd: =%"  
echo "YYYYmmdd%YYYYmmdd%YYYYmmdd"  
rem 根据当前时间获取，时分秒串  
set hh=%time:~0,2%  
set mi=%time:~3,2%  
set ss=%time:~6,2%   
set "hhmiss=%hh%%mi%%ss%"  
set "hhmiss=%hhmiss: =%"  
echo "hhmiss%Time%hhmiss"  
echo %hhmiss%  
rem 把时间串中的:替换为0  
set "hhmiss=%hhmiss::=%"  
rem 把时间串中的空格替换为0  
set "hhmiss=%hhmiss: =%"  
rem 根据日期时间生成文件名称，中间以HH区分日期和时间部分  
set "filename=%YYYYmmdd%_%hhmiss%.log"  
echo %filename%  
md C:\Users\Administrator\Documents\adblogcat\%YYYYmmdd%\
adb logcat -v time > C:\Users\Administrator\Documents\adblogcat\%YYYYmmdd%\Android_logcat_%filename%
pause`
# 命令解析
输出为：2016-11-09 15-40-16.txt
说明：
在windows 的cmd命令行窗口执行date命令后这个环境变量的值为
图片描述
那么如下的各个操作的意义如下：
%date:~0,4% 表示从左向右指针向右偏0位，然后从指针偏移到的位置开始提取4位字符，结果是2016（年的值）
%date:~5,2% 表示指针从左向右偏移5位，然后从偏移处开始提取2位字符，结果是11（月的值）
%date:~8,2% 表示指针从左向右偏移8位，然后从偏移处开始提取2位字符，结果是09（日的值）
再举个例子：
%date:~5% 表示指针从左向右偏移5位，然后提取所有的值
%date:~-5% 表示指针反方向偏移，从最右端开始，偏移5位，然后从指针处提取左边的所有数值。
再来看时间time变量的值：
图片描述
那么如下的各个操作的意义如下：
%time:~0,2% 表示从左向右指针向右偏0位，然后从指针偏移到的位置开始提取2位字符，结果是小时字段数值
%time:~3,2% 表示指针从左向右偏移3位，然后从偏移处开始提取2位字符，结果是分钟字段数值
%time:~6,2% 表示指针从左向右偏移6位，然后从偏移处开始提取2位字符，结果是秒字段数值
# 鸣谢
觉得写的不错，看了文章的，请帮忙关注公众号-嵌入式Linux

