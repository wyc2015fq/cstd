# VC++ (VS2008)里面char和LPTSTR的转换问题 - 江南烟雨 - CSDN博客
2011年07月21日 09:28:24[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：5416
在VS里面写如下代码，实现两个数的相加并且显示结果：
```cpp
int num1,num2,num3;
    char ch1[10],ch2[10],ch3[10];
    GetDlgItem(IDC_EDIT1)->GetWindowText(ch1,10);
    GetDlgItem(IDC_EDIT2)->GetWindowText(ch2,10);
    num1=atoi(ch1);
    num2=atoi(ch2);
    num3=num1+num2;
    itoa(num3,ch3,10);
    GetDlgItem(IDC_EDIT3)->SetWindowText(ch3);
```
编译运行出现错误：
```cpp
1>.\TestDlg.cpp(42) : error C2664: “int CWnd::GetWindowTextW(LPTSTR,int) const”: 不能将参数 1 从“char [10]”转换为“LPTSTR”
1>        与指向的类型无关；转换要求 reinterpret_cast、C 样式转换或函数样式转换
1>.\TestDlg.cpp(43) : error C2664: “int CWnd::GetWindowTextW(LPTSTR,int) const”: 不能将参数 1 从“char [10]”转换为“LPTSTR”
1>        与指向的类型无关；转换要求 reinterpret_cast、C 样式转换或函数样式转换
1>.\TestDlg.cpp(53) : error C2664: “CWnd::SetWindowTextW”: 不能将参数 1 从“char [10]”转换为“LPCTSTR”
1>        与指向的类型无关；转换要求 reinterpret_cast、C 样式转换或函数样式转换
```
原因是字符集的问题。VS2008和VC6.0还是有些不一样的。
参考资料：[http://topic.csdn.net/u/20090506/17/d7e4b312-ba8a-4611-b94b-59c5c7a96aea.html](http://topic.csdn.net/u/20090506/17/d7e4b312-ba8a-4611-b94b-59c5c7a96aea.html)
解决方案：
char 改成TCHAR
atoi 改成 _ttoi
itoa 改成 _itot
```cpp
TCHAR ch1[10],ch2[10],ch3[10];
	GetDlgItem(IDC_EDIT1)->GetWindowText((ch1),10);
	GetDlgItem(IDC_EDIT2)->GetWindowText((ch2),10);
	num1=_ttoi(ch1);
	num2=_ttoi(ch2);
	num3=num1+num2;
	_itot(num3,ch3,10);
	GetDlgItem(IDC_EDIT3)->SetWindowText(ch3);
```
