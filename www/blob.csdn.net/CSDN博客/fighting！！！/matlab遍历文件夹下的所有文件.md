# matlab遍历文件夹下的所有文件 - fighting！！！ - CSDN博客
2018年04月09日 19:04:54[dujiahei](https://me.csdn.net/dujiahei)阅读数：1452

源地址：https://blog.csdn.net/hqh45/article/details/46004987
输入为目录，输出为cell数组。使用第i个文件时，文件名为files{i}。
[html][view plain](https://blog.csdn.net/hqh45/article/details/46004987#)[copy](https://blog.csdn.net/hqh45/article/details/46004987#)
- function [ files ] = scanDir( root_dir )  
- 
- files={};  
- if root_dir(end)~='/'  
- root_dir=[root_dir,'/'];  
- end  
- fileList=dir(root_dir);  %扩展名  
- n=length(fileList);  
- cntpic=0;  
- for i=1:n  
-     if strcmp(fileList(i).name,'.')==1||strcmp(fileList(i).name,'..')==1  
-         continue;  
-     else  
-         fileList(i).name  
-         if ~fileList(i).isdir  
- 
- full_name=[root_dir,fileList(i).name];  
- 
- %              [pathstr,name,ext,versn]=fileparts(full_name);  
- %              if strcmp(ext,'.jpg')  
- cntpic=cntpic+1;  
-                  files(cntpic)={full_name};  
- %              end  
-         else  
- files=[files,scanDir([root_dir,fileList(i).name])];  
-         end  
-     end  
- end  
- 
- 
- 
- end  

