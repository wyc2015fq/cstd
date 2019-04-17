# matlab 获取目录下所有文件信息 - 家家的专栏 - CSDN博客





2011年08月18日 17:12:55[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3063








 matlab code 示例：

filepath = ‘*****’;

如：filepath = 'Images/；   //文件的相对路径

f=dir(filepath)

f = 

593x1 struct array with fields:

    name

    date

    bytes

    isdir

    datenum



