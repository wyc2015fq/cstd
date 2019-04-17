# matlab 写txt文件 - 家家的专栏 - CSDN博客





2014年04月10日 09:23:52[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4763











clear all;

clc;

scenes = dir(fullfile('D:/Resized'));


fid1=['name','.txt'];   %创建新的txt文件

c=fopen(fid1,'wt');   %打开txt文件

for i=3:size(scenes,1)

    str=['D:/Resized/' scenes(i,1).name];

    fprintf(c,'%s\n',str);        %按行写入txt文件，%s为数据形式，str为写入数据。。。

end

fclose(c);    %关闭txt文件
            


