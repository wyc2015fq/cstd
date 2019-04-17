# 如何评估一个ECG分析算法或设备 - 工作笔记 - CSDN博客





2012年01月17日 13:47:50[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5663标签：[算法																[database																[each																[数据库](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
个人分类：[信号处理](https://blog.csdn.net/App_12062011/article/category/1063270)





详细的评估ECG分析算法或设备的内容。参见：[http://www.physionet.org/physiotools/wag/eval.htm](http://www.physionet.org/physiotools/wag/.htm)

要点：

 1、 WFDB中包含了评估有关的程序；

 2、 EC38和EC57要求采用以下的数据库进行评估： AHA DB: The American Heart Association Database for Evaluation of Ventricular Arrhythmia Detectors (80 records, 35 minutes each) MIT DB: The Massachusetts Institute of Technology-Beth
 Israel Hospital Arrhythmia Database (48 records, 30 minutes each) ESC DB: The European Society of Cardiology ST-T Database (90 records, two hours each) NST DB: The Noise Stress Test Database (12 records, 30 minutes each) CU DB: The Creighton University Sustained
 Ventricular Arrhythmia Database (35 records, 8 minutes each)

 3、 AHA数据库中数据的格式和其它库中数据的格式不同，要用WFDB的命令a2m和ad2m来进行转换。

 4、 关于评估的细节，参见： http://www.physionet.org/physiotools/wag/evnode10.htm](https://so.csdn.net/so/search/s.do?q=each&t=blog)](https://so.csdn.net/so/search/s.do?q=database&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)




