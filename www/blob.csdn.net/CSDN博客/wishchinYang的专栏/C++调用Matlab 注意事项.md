# C++调用Matlab 注意事项 - wishchinYang的专栏 - CSDN博客
2014年07月11日 11:14:33[wishchin](https://me.csdn.net/wishchin)阅读数：1403

前言：脑残的使用了C++调用Matlab，没想到Matlab的使用者的智商还真TMD不一般，
            竟然有这样的 plot(x_Abnorm_index,Vec2(Abnorm_index),'sb','linewidth',1.5);  hold on; 语句。
            这'sb' 的配色方案还真有趣，为什么没有“fk'”的配色方案？？？？
注意事项：
1.  编写的函数里面不要添加：
clear allclc
等语句，否则会造成   Reference to a cleared variable f    
之类的错误！
原因：把运行代码直接改换成一个函数，忘记了清楚这些无聊代码。
2. Error - RtlWerpReportException failed with status code :-1073741823. Will tr
在Qt中调用Matlab函数，复制数据出数组时要注意数组形式，对于：
mwArrayNormal;
此句为Matlab语句........................
double*Ptrue=newdouble[20];Normal.GetData(Ptrue,20);//此句导致程序崩溃！Normal.GetData(Ptrue,20);导出为一维数组，如何导出二维数组？
3.不要把调用相同函数生成的Matlab头文件和函数调用写在一个包里
这样会造成函数使用出现问题，造成程序崩溃
一定要把每个函数只在一个类里面调用。
4.关于WmArray的输出格式：
这是最坑的地方，输出格式必须是行向量，而不能是列向量
输出前必须转化：Vec2 = Vec2';
否则又：Error - RtlWerpReportException failed with status code :-1073741823. Will tr
5.最要命的是指针一定要初始化！！！！！！
