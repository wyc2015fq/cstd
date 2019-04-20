# FPGA之quartus入门第一课 - situheyu的博客 - CSDN博客
2017年10月20日 21:59:40[司徒合羽](https://me.csdn.net/situheyu)阅读数：259
作业一  4-16译码器
1.打开quartus,新建一个工程，定义实体，采用原理图输入法，采用两片74138芯片，连线 
![这里写图片描述](https://img-blog.csdn.net/20171020213234388?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2编译，综合。 
![这里写图片描述](https://img-blog.csdn.net/20171020213416001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3建立波形文件，仿真。 
file->new->vector waveform file 
processing->generate functional simulation netlist 
注，坊真前需添加输入输出管脚  
双击空白处->node finder 
仿真结果如下!
![这里写图片描述](https://img-blog.csdn.net/20171020214107213?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
作业二，M=12的计数器 
00000000~00001011置数法 
![这里写图片描述](https://img-blog.csdn.net/20171020215344232?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171020215405066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171020215104478?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
作业三，M=20的计数器 
00000000~00010011置数法 
![这里写图片描述](https://img-blog.csdn.net/20171020215235085?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171020215214420?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171020215248962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
quartus菜鸟一枚，望批评改正！
