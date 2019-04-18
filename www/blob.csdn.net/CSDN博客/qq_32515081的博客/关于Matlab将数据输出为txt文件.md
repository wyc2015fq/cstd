# 关于Matlab将数据输出为txt文件 - qq_32515081的博客 - CSDN博客
2018年03月16日 19:52:17[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：15741
fid1=fopen('hello.txt','w');                                                         %需要改文件名称的地方
count=fprintf(fid1,' %f \n',data);                                                         %data：需要导出的数据名称
