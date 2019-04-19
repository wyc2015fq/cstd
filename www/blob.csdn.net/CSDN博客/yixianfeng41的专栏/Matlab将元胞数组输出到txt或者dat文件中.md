# Matlab将元胞数组输出到txt或者dat文件中 - yixianfeng41的专栏 - CSDN博客
2016年09月20日 04:01:46[第2梦](https://me.csdn.net/yixianfeng41)阅读数：14759
### 原链接：http://cn.mathworks.com/help/matlab/import_export/write-to-delimited-data-files.html#br2ypq2-1
### 方法一：用fprint输出元胞数组
这个例子展示了怎么用fprint方法将元胞数组输出到TEXT 文件中。
创建一个元胞数组例子, 以便输出
C = {'Atkins',32,77.3,'M';'Cheng',30,99.8,'F';'Lam',31,80.2,'M'}
C =
  3×4 cell array
    'Atkins'    [32]    [77.3000]    'M'
    'Cheng'     [30]    [99.8000]    'F'
    'Lam'       [31]    [80.2000]    'M'
打开一个名字为celldata.dat的文件，将元宝数组内容输出到这个文件里.
fileID = fopen('celldata.dat','w');
``
fopen 返回一个文件标示fileid,通过这个标示，fprint将数组数据写入到文件中
构建字符串格式，'%s'表示字符串, '%d' 标示int数据, '%f'表示是浮点数据 ，每一个数据后换行('\n').
formatSpec = '%s %d %2.1f %s\n';
获得元胞数组的行和列值. 然后用fprint逐行将数据输出到文件中.
[nrows,ncols] = size(C);
for row = 1:nrows
    fprintf(fileID,formatSpec,C{row,:}); // 注意此处不用{}，而是（）时，得到的字符串酱油引号，需注意
end
``
输出完毕后，关闭文件流。
fclose(fileID);
查看文件内容
type celldata.dat
Atkins 32 77.3 M
Cheng 30 99.8 F
Lam 31 80.2 M

### 方法2：将元胞数组转换成Table
例子如下：
T = cell2table(C,'VariableNames',{'Name','Age','Result','Gender'});
将Table输出到TEXT中。
writetable(T,'tabledata.dat')
查看这个文件
type tabledata.dat
Name,Age,Result,Gender
Atkins,32,77.3,M
Cheng,30,99.8,F
Lam,31,80.2,M
