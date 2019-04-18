# linux下eclipse cdt主函数main参数值传递设置 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年01月22日 10:58:58[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2790









ubuntu下eclipse cdt 编写C/C++代码时，如果main函数的argv[]字符数组需要传值（命令行可以直接在程序后带参数值），需要通过如下设置：


    —右键工程，选择Run as->Run Configurations...，


    —选择"arguments"标签页进入配置参数及其值，如添加：argv[1]=xx,argv[2]=xx；



