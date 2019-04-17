# matlab 字符串提取数字 - 家家的专栏 - CSDN博客





2011年08月18日 17:25:51[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：6929








                
 示例：

str='ajsk123sjkl'

A=isstrprop(str,'digit');  

%A =

%     0     0     0     0     1     1     1     0     0     0     0


B=str(A);

%B =123  字符

C=str2num(B)  

%C=123  数字



