# matlab 两个txt文件的合并。。 - 家家的专栏 - CSDN博客





2015年03月06日 10:59:32[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3594








                
fidin1=fopen('r.txt','r');

fidin2=fopen('2.txt','r');

fidout=fopen('r_1.txt','w');

nline=0;

while ~feof(fidin2) % ?????????    

    tline1=fgetl(fidin1); % ?????

    tline2=fgetl(fidin2); % ?????  

     nline=nline+1   

    fprintf(fidout, [tline1,' ',tline2,'\n']);

end

fclose(fidin1);

fclose(fidin2);

fclose(fidout);
            


