# ECG的滤波和检波资源汇总 - 工作笔记 - CSDN博客





2012年01月12日 11:06:06[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7213








**ECG的滤波和检波资源汇总**

**![](http://hi.csdn.net/attachment/201102/16/0_1297822793v6Z5.gif)**

     ECG的滤波和检波，前人已经做了不少的工作，这里汇总一下相关的资源：

1.首先要推荐的是matlab的官网，发现mathworks公司在组织matlab的开源程序交流上做了很好的工作，见 

[http://www.mathworks.com/matlabcentral/fileexchange/11755-peak-finding-and-measurement-version-2](http://www.mathworks.com/matlabcentral/fileexchange/11755-peak-finding-and-measurement-version-2)

2.还有一个网站提供了C语言的程序   [http://www.codeproject.com/KB/cpp/ecg_dsp.aspx](http://www.codeproject.com/KB/cpp/ecg_dsp.aspx)

        这两个资源都比较详细的叙述了有关算法和原理

3.还有这个网站利用matlab生产了模拟的心电图

[http://webscripts.softpedia.com/script/Scientific-Engineering-Ruby/Controls-and-Systems-Modeling/ECG-simulation-using-MATLAB-32785.html](http://webscripts.softpedia.com/script/Scientific-Engineering-Ruby/Controls-and-Systems-Modeling/ECG-simulation-using-MATLAB-32785.html)

     我认为模拟的心电图也是很重要的研究工具，是研究的起点。因为图形是构造模拟出来的，那就能人为加入干扰和噪音，如果自己清楚原来的图像是什么，就能对滤波和检波的效果做简明的检验。

4.该网站有针对该课题的互动与答疑

http://thedailyreviewer.com/compsys/view/filtering-noisy-data-to-find-peaks-109283555

5.最重要的当然是MIT-BIH的数据库，这是最根本的检验标准，也是做进一步分析处理的基础。做好滤波和检波，是心电图智能诊断的基础，但是要继续做智能诊断的工作，离不开详尽专业的心电图数据库。

The MIT-BIH Noise Stress Test Database   [http://physionet.mit.edu/physiobank/database/nstdb/](http://physionet.mit.edu/physiobank/database/nstdb/)



