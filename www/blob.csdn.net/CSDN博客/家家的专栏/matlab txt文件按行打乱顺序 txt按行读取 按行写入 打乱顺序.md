# matlab txt文件按行打乱顺序 txt按行读取 按行写入 打乱顺序 - 家家的专栏 - CSDN博客





2015年03月05日 16:57:30[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2266








                
clear all;

clc;



fidin=fopen('Cund1.txt');

fidout = fopen('Cnd1_rand.txt','w');

index =0;

while ~feof(fidin)                   %?????????                                      

   tline=fgetl(fidin);

   index =index+1

   str{index} = tline;

end



rand_index = randperm(index);



for i=1:index

    fprintf(fidout, '%s\n',str{rand_index(i)});   

    i

end



fopen(fidin);

fopen(fidout);
            


