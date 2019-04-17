# matlab 读写txt文本文件的方法 - 家家的专栏 - CSDN博客





2011年02月20日 16:48:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1397








**读取txt文件：**

matlab示例语句：

[filename, date, timestamp, direction, dayornight, weather, startframe, numframes, class] = textread('info.txt', ...

'%s %s %s %s %s %s %s %s %s', -1);



text文本文件中的数据格式如下：

cctv052x2004080516x01638 20040805 16.01638 south day overcast 2 53 medium 

cctv052x2004080516x01639 20040805 16.01639 south day overcast 2 53 medium 

cctv052x2004080516x01640 20040805 16.01640 south day overcast 2 48 light 



因无法复制，先将参考网址记录如下：[http://www.360doc.com/content/10/0710/16/2122544_38081828.shtml](http://www.360doc.com/content/10/0710/16/2122544_38081828.shtml)







**写入txt文件：**

%新建txt文件

fid=fopen('flower17testlabel.txt','a+');


fprintf(fid, ‘explain how to write some in txt file. \r\n');


fclose(fid);



