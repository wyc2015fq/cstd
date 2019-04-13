
# c++与Tcl通过管道通信，并传递参数,获取测试仪实时测试进度 - Augusdi的专栏 - CSDN博客


2015年08月22日 20:43:48[Augusdi](https://me.csdn.net/Augusdi)阅读数：1866


﻿﻿
目前，我在做测试自动化的项目，要用到smartbits,bps,testcenter和mu测试仪。通过调用测试仪提供的TCL API操作测试仪，从而实现测试用例的具体流程。我们用测试仪测试TOE时，会发现测试仪上会显示测试进度（1%，2%...）。以Bps为例，当执行到set objectName [$testObjectName run
-progress "bps::textprogress stdout"]命令时，就会在屏幕上同步输出当前的测试进度。为了与其他模块方便通信，我利用c++调用TCL脚本来实现用例。为了将tcl输出的测试进度获取并传递给其他模块，我利用windows API CreateProcess()，在c++与TCL脚本之间开辟了一条管道。下面是我的.tcl源码和c++源码，希望对从事这方面开发的同仁有益。
【.tcl】
set dir [file dirname [info script]]
lappend auto_path $dir
wm withdraw .
proc testProc {ip uname pwd} {
puts "loading bps package..."
package require bps
puts "bps package loading complete."
set var [bps::connect $ip $uname $pwd]
set test1 [$var createTest]
set comName [$test1 createComponent bitblaster bb41 1 2]
puts "test running.."
set fid [open "testProgress.txt" w+]
if [catch {open "testProgress.txt" w+} $fid] {
puts stderr "cann't open "testProgress.txt" w+: $fid"
} else {
set reObj [$test1 run -progress "bps::textprogress stdout"]
puts $fid $reObj
close $fid
}
puts "quit"
}
\#------------------------------------------------------------------
\# Analyze cmd options
\#set proc_list [ info commands ]
\#for { set i 0 } { $i < $::argc } { incr i } {
\#    if { -1 != [ lsearch -exact $proc_list [ lindex $::argv $i ] ] } {
\#        puts "Try to execute proc '[ lindex $::argv $i ]' ......"
\#       eval [ lindex $::argv $i ]
\#   }
\#}
\#-----------------------------------------------------------------
\# Analyze cmd options
set proc_list [ info commands ]
for { set i 0 } { $i < $::argc } { incr i } {
set proc_name [ lindex $::argv $i ]
if { [ expr [ regexp -- {^-} $proc_name ] && \
-1 != [ lsearch -exact $proc_list [ string range $proc_name 1 end ] ] ] } {
set param_list ""
for { set j [ expr $i+1 ] } { $j < $::argc } { incr j } {
set param_name [ lindex $::argv $j ]
if { ![ regexp -- {^-} $param_name ] } {
lappend param_list $param_name
} else {
set i [ expr $j - 1 ]
break
}
}
puts "Try to execute proc '[ string range $proc_name 1 end ]' with params '$param_list' ......"
set eval_code [ list [ string range $proc_name 1 end ] ]
foreach param $param_list { lappend eval_code $param }
eval $eval_code
}
}
【注】：如果testProc不需要参数，则可以将两条“————”之间的代码解注释，将“----”下面的代码注释掉即可。
【c++】
\#include "stdafx.h"
\#include <Windows.h>
\#include <string>
\#include <process.h>
\#include <iostream>
\#include <stdio.h>
\#include "tcl.h"
\#include "tclDecls.h"
\#pragma comment(lib,"tcl85.lib")
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{

SECURITY_ATTRIBUTES sa;
HANDLE hRead,hWrite;
sa.nLength = sizeof(SECURITY_ATTRIBUTES);
sa.lpSecurityDescriptor = NULL;
sa.bInheritHandle = TRUE;
if (!CreatePipe(&hRead,&hWrite,&sa,0)) {
cout<<"Error On CreatePipe()"<<endl;
return 0;
}
STARTUPINFO si;
PROCESS_INFORMATION pi;
si.cb = sizeof(STARTUPINFO);
GetStartupInfo(&si);
si.hStdError = hWrite;
si.hStdOutput = hWrite;
si.wShowWindow = SW_HIDE;
si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
string str1 = "D:\\Tcl\\bin\\wish85.exe test.tcl";
string str2 = "-";
string str3 = "test";
string str4 = "10.10.0.1";
string str6 = " ";
string str7 = "mobei";
string str8 = "mobei";
string str = str1+str6+str2+str3+str6+str4+str6 + str7 + str6 + str8;
char* strC = const_cast<char*>(str.c_str());
if (!CreateProcess(NULL,strC, NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
{
cout<<"Error on CreateProcess()"<<endl;
return 0;
}
CloseHandle(hWrite);
char buffer[4096] = {0};
DWORD bytesRead;
while (true)
{
if (ReadFile(hRead,buffer,4095,&bytesRead,NULL) == NULL)
{
cout<<"test"<<endl;
break;
}
else
{
string bufStr;
bufStr = buffer;

if (bufStr.find("quit") != -1)
{
break;
}
else
{
sendInfo(bufStr);//将测试进度发送给其他模块
cout<<"content of buffer is: "<<buffer<<endl;
}
}
}
cout<<"hello world"<<endl;

return 0;
}
由于ReadFile(）是个阻塞函数，所以在tcl脚本里面输出一个字符，当在bufStr中找到这个字符时就退出循环，这里我输出的是quit，作为退出循环标志。测试的结果，我分别保存在了两个.txt文件中，可以通过分析文件获取，下面是用c++读取.txt文件的代码。
【readTxt】
// readTxt.cpp : Defines the entry point for the console application.
//
\#include "stdafx.h"
\#include "iostream"
\#include <fstream >
\#include <string>
using namespace std;
string readTxt(string fileName);
int main(int argc, char* argv[])
{
string result = readTxt("passorfailed.txt");
cout<<"result is: "<<result<<endl;
if (result == "passed")
{
cout<<"test ok"<<endl;
}
else
cout<<"test failed"<<endl;
return 0;
}
string readTxt(string fileName)
{
ifstream infile;
char* fileNamePtr = const_cast<char*>(fileName.c_str());
infile.open (fileNamePtr);
if(!infile)
cout<<"error"<<endl;   //判断文件是被成功打开
string num;             //定义文件中的数据类型
char p;                 //定义一个字符读取文件中的空格和/或回车符
int M=1;               //这里首先定义为1是因为最后一行的结束符是EOF,所以用'\n'来判断行的话，                   //最后一行会漏悼。所以先把它定义为1，也就相当于加上了最后一行！
int N=0;
while(!infile.eof())   //每次读入之前都要判断是否到了文件末
{
do{
infile.get (p);
if(p=='\n')
{
M++;         //统计行数;
N=0;        //一个新行开始时，列数置为0
cout<<endl; //一行之后输出一个换行符
}
}while(isspace((int)p) && !infile.eof ());//结束条件是读入的是空格或已到达文件末
N++;                 //统计列数
infile.putback (p); // 如果前面读入的不是空格或回车符，则要把刚才读入的字符返回到文件流中！
infile>>num;        //   读入一个数
cout<<num<<" ";    //   输出刚才读入的数据
}
infile.close (); //     关闭文件
return num;
}
如果从事过这方面开发的同仁有更好的解决方案，欢迎不吝赐教。

