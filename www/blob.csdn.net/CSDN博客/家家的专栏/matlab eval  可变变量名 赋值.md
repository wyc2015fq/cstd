# matlab eval    可变变量名  赋值 - 家家的专栏 - CSDN博客





2014年02月22日 19:04:24[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2198








filename=['./data/ALLfea_',num2str((index50-1)*50+1),'_',num2str(index50*50),'.mat'];




eval(['save -v7.3 ' filename  ' Fea_D Labels';]);
            


