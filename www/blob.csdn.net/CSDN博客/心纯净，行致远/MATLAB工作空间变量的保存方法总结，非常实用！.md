# MATLAB工作空间变量的保存方法总结，非常实用！ - 心纯净，行致远 - CSDN博客





2018年09月18日 19:18:26[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：3621








对于工作空间中变量的保存和载入可以使用save和load命令，详细的使用方法通过help指令获取(help save，help load)。两条指令最常用的情况为：

1、%** 保存整个工作空间至指定的mat文件**FILE_PATH_NAME，如E:\workspace.mat

% 若已经存在同名的mat文件，该文件会被覆盖重写而不是追加

save FILE_PATH_NAME

例如：
`save C:\Users\QinJX\Desktop\华为杯\2018赛题\wo.mat`


2、% **保存指定的变量至指定的mat文件**FILE_PATH_NAME

% 若已经存在同名的mat文件，该文件会被覆盖重写而不是追加

save FILE_PATH_NAME variable

例如：
`save C:\Users\QinJX\Desktop\华为杯\2018赛题\wo.mat c`
%保存c这个变量



3、% **保存指定的若干变量至指定的mat文件**FILE_PATH_NAME

% 若已经存在同名的mat文件，该文件会被覆盖重写而不是追加

save FILE_PATH_NAME variable1 variable2 variable3 ...



4、% **载入mat文件FILE_PATH_NAME中的所有变量至当前工作空间**

% 若当前工作空间中有同名变量，则此同名变量会被载入变量覆盖

load FILE_PATH_NAME



5、%** 载入mat文件FILE_PATH_NAME中的指定变量至当前工作空间**

% 若当前工作空间中有同名变量，则此同名变量会被载入变量覆盖

load FILE_PATH_NAME variable



6、%** 载入mat文件FILE_PATH_NAME中的若干指定变量至当前工作空间**

% 若当前工作空间中有同名变量，则此同名变量会被载入变量覆盖

load FILE_PATH_NAME variable1 variable2 variable3 ...

例如：load C:\Users\QinJX\Desktop\华为杯\2018赛题\2018年中国研究生数学建模竞赛E题\批量存储工作区变量\data.mat c b

%加载c,b这两个变量

7、**Matlab如何save workspace中除了某个变量的所有变量**

假设不想保存的这个变量名字叫strName,

存储当前工作空间其他变量到data.mat中

代码：
`save data.mat -regexp '[^strName]'`




