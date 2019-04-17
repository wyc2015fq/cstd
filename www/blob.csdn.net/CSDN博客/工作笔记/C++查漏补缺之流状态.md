# C++查漏补缺之流状态 - 工作笔记 - CSDN博客





2015年03月28日 20:49:14[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5560








常用的流状态：

showpos       :在正数（包括0）之前显示+号        cout<<showpos<<12; 输出：+12

showbase    ：十六进制整数前加0X，八进制整数前加0   cout<<hex<<18<<" "<showbase<<18;输出:12 0X12

uppercase    :十六进制格式字母用大写字母表示（默认为小写字母）cout<<hex<<255<<" "<uppercase<<255;输出:ff FF

showpoint    :浮点输出即使小数点后都为0也加小数点 cout<<123.0<<" "<<showpoint<<123.0;输出:123 123.000

boolalpha     :逻辑值1和0用true和false表示  cout<<(2>3)<<" "<<boolalpha<<(2>3);输出：0 false

left                  :左对齐（填充字符填在右边）

right               :右对齐（填充字符填在左边）

dec                :十进制显示整数

hex                :十六进制显示整数

oct                 :八进制显示整数

fixed              :定点数格式输出  

scientific      :科学记数法格式输出 cout<<scientific<<123456.78;输出：1.234568e+05;





有参数的三个常用的流状态：

width(int) //设置显示宽度

fill(char)  //设置填充字符

precision(int)  //设置有效位数（普通显示方式）或精度（定点或科学记数法方式）



例：

cout.width(5);

cout.fill(‘S’);

cout<<23<<23;  输出：SSS2323





与<<连用的设置方式(使用时要包括头文件iomanip)：

setw(int)

setfill(char)

setprecision(int)



例：

cout<<setw(6)<<setfill(‘S’)<<27<<endl; 输出：SSSS27

﻿

﻿﻿



