# matlab 读写txt文件 - 家家的专栏 - CSDN博客





2014年07月01日 15:44:06[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：744








                
fidout = fopen('listout.txt');  %打开读取文件夹

fidin   = fopen('listin.txt','w');  %打开写入文件夹

while ~feof(fid)

        tline=fgetl(fidout);     %读取

        fprintf(fidin, '%s \n',tline);    %写入        

end

fclose(fidout);

fclose(fidin);



