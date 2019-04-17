# C++ 头文件中的输出 - Alex_McAvoy的博客 - CSDN博客





2018年02月07日 14:50:46[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：255








## 【使用控制符符控制】



 1.使用方法：cout<<控制符;（例：cout<<setw(3);）

 2.常见控制符及作用


|控制符|作用|
|----|----|
|dec、oct、hex|设置整数基数，分别代表十、八、十六|
|setbase(n)|设置整数基数，n只可为8，10，16|
|setfill(c)|设置填充字符，c为字符常量或字符变量|
|setprecision(n)|设置精度，十进制小数输出时，代表有效数字，以fixed、scientific输出时，代表小数位数|
|setw(n)|设置字段宽度|
|setiosflags(ios::fixed)|设置以浮点数固定的小数位数显示|
|setiosflags(ios::scientific)|设置以浮点数指数形式显示(科学计数法)|
|setiosflags(ios::uppercase)|大写表示，以科学计数法或以十六进制输出时，将e、x大写输出|
|setiosflags(ios::showpos)|输出正数时，给出“+”|
|setiosflags(ios::right)|右对齐输出|
|setiosflags(ios::left)|左对齐输出|
|setiosflags(ios::shipws)|忽略前导空格|
|resetiosflags()|终止已设置的输出格式状态，括号内指定内容|



## 【使用流对象控制】





 1.使用方法：cout.流成员函数（例：cout.precision(3);）

 2.常见流成员函数与其对应的控制符


|流成员函数|与之作用相同的控制符|
|----|----|
|precision(n)|setprecision(n)|
|width(n)|setw(n)|
|fill(c)|setfill(c)|
|setf()|setiosflags()|
|ubsetf()|resetiosflags()|



## 【设置格式状态的格式标志】



 1.经典使用：cout<<setiosflags(ios::fixed)<<setprecision(3);//设置保留三位小数输出



 2.常见格式标志及作用


|格式标志|作用|
|----|----|
|ios::dec、ios::oct、ios::hex|设置整数基数，分别代表十、八、十六|
|ios::showbase|强制输出整数基数(八进制为0，十六进制为X)|
|ios::showpoint|强制输出浮点数的“.”与尾数的“0”|
|ios::uppercase|输出格式为科学计数法或十六进制时，将e、x以大写表示|
|ios::showpos|输出正数时，给出“+”|
|ios::scientific|设置浮点数输出时以科学计数法形式显示|
|ios::fixed|设置浮点数以固定小数位数显示|
|ios::left|左对齐输出|
|ios::right|右对齐输出|
|ios::internal|符号位左对齐，数位右对齐输出|
|ios::unitbuf|每次输出后，刷新所有流|
|ios::stdio|每次输出后，清除stdout，stderr|





