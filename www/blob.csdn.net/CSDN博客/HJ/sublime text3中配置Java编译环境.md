# sublime text3中配置Java编译环境 - HJ - CSDN博客
2017年01月25日 15:25:07[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：2317
方法一. 
打开Sublime Text 3-Preference- Browse Packages，双击User文件夹，新建JavaC.sublime-build文件，并写入下面的代码：
```
{  
    "cmd": ["javac","-encoding","UTF-8","-d",".","$file"],  
    "file_regex": "^(...*?):([0-9]*):?([0-9]*)",  
    "selector": "source.java",  
    "encoding":"GBK",  
    "variants":  
        [  
            {  
                "name": "Run",  
                "shell": true,  
                "cmd" :  ["start","cmd","/c", "java ${file_base_name} &echo. & pause"],  
                "working_dir": "${file_path}",  
                "encoding":"GBK"  
            }  
        ]  
    }  
   // /c是执行完命令后关闭cmd窗口,  
   // /k是执行完命令后不关闭cmd窗口。  
   // echo. 相当于输入一个回车  
   // pause命令使cmd窗口按任意键后才关闭
```
ctrl+b编译，Ctrl+shift+b运行
方法二 
1.安装jdk并配置环境变量 
2.JDK的bin目录下，新建runJava.bat文件，并写入下面的代码：
```
@echo off  
    cd %~dp1  
    echo Compiling %~nx1......  
    if exist %~n1.class (  
    del %~n1.class  
    )  
    javac -encoding UTF-8 %~nx1  
    if exist %~n1.class (  
    echo ------Output------  
    java %~n1  
    )
```
3.打开SublimeText3-Preference- Browse Packages，双击User文件夹，新建JavaC.sublime-build文件，并写入下面的代码：
```
{  
    "shell_cmd": "runJava.bat \"$file\"",  
    "file_regex": "^(...*?):([0-9]*):?([0-9]*)",  
    "selector": "source.java",  
    "encoding": "GBK"  
    }
```
方法二的缺点是无法在控制台输入数据。
