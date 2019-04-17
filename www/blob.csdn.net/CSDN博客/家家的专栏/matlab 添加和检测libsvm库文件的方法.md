# matlab 添加和检测libsvm库文件的方法 - 家家的专栏 - CSDN博客





2011年01月24日 09:31:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1147








1、添加路径：      在command windows中，

 输入：addpath D:/libsvm-mat-2.89-3    

 然后：savepath



2、 为了检验libsvm和matlab之间的接口是否已经配置完成，可以在matlab下执行以下命令： 

load heart_scale.mat 

model = svmtrain(heart_scale_label, heart_scale_inst, '-c 1 -g 2'); 

如果运行正常并生成了model这个结构体（其中保存了所有的支持向量及其系数），那么说明libsvm和matlab之间的接口已经完全配置成功。



参考网址：[http://qoitaaxb.spaces.live.com/Blog/cns!5A83D8EB40AAD7B6!173.entry](http://qoitaaxb.spaces.live.com/Blog/cns!5A83D8EB40AAD7B6!173.entry)

libsvm库文件下载：[http://www.csie.ntu.edu.tw/~cjlin/libsvm/](http://www.csie.ntu.edu.tw/~cjlin/libsvm/)

matlab  svmtrain函数：[http://www.ymlib.net/article/sort010/info-1465.html](http://www.ymlib.net/article/sort010/info-1465.html)



