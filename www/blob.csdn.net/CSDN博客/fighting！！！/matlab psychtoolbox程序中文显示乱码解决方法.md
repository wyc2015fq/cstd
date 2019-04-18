# matlab psychtoolbox程序中文显示乱码解决方法 - fighting！！！ - CSDN博客
2018年04月17日 21:07:07[dujiahei](https://me.csdn.net/dujiahei)阅读数：367

在Open window后，设置支持中文的字体，例如宋体（Simsun）Screen('TextFont', wPtr, 'Simsun'); 给汉字加double。在这些会出现乱码的地方，在汉字的外面加一个double，例如'编程'就变成double('编程')。
