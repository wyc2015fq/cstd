# Matlab二进制文件的读写 - Koma Hub - CSDN博客
2018年01月19日 16:08:06[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：579
```python
%二进制文件的读入：
function sig_data=read_file(shot,nx,nz)
fp=fopen(shot,'rb');
sig_data=zeros(nx,nz);
for i=1:nx
        row_array=fread(fp,nz,'float32');
        sig_data(i,:)=row_array;
end
fclose(fp);
%output or show the 2D matrix 二进制文件的输出：
function    datoutput(shot,matrix);
fp=fopen(shot,'wb');
fwrite(fp,matrix,'float32');
fclose(fp);
```
