
# 有意思的BAT CMD if while循环 整型自增 - 嵌入式Linux - CSDN博客

2019年03月06日 17:23:58[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：41



## 引言
我们开发过程中经常遇到写bat进行测试的，需要对这方面有些基础，我给出个示例，也方便自己以后工作做参考。
`@echo off
title qifa
:loop
	rem setlocal enabledelayedexpansion
	set /a a+=1	
	echo loop %a%
	@choice /t 1 /d y /n >nul
	rem 判断等于1，启动ping
	if %a% == 1 (
		echo start ping bat success!
		rem start ping.bat
		start adb shell ping -c 35 192.168.10.238 >> ping.txt
	)
	if %a% == 60 (
		rem 杀死ping进程
		echo Kill ping cmd!
		TASKKILL /F /IM adb.exe
	)
	if %a% == 120 (
		rem 重新初始化变量
		echo reset
		set /a a=0
	)
	
goto loop
pause`

