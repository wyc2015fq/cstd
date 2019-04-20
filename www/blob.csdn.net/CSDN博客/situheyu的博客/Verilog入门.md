# Verilog入门 - situheyu的博客 - CSDN博客
2017年10月23日 20:59:00[司徒合羽](https://me.csdn.net/situheyu)阅读数：194
![这里写图片描述](https://img-blog.csdn.net/20171023205250106?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第一题 
4-16译码器 
新建一个Verilog文本文件，把相关文件输入进去，编译综合。 
![这里写图片描述](https://img-blog.csdn.net/20171023204436713?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
建立一个波形文件，给输入信号激励，然后进行仿真，得到结果波形图。
![这里写图片描述](https://img-blog.csdn.net/20171023204745273?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第二题 M=12的计数器 
方法如上。 
![这里写图片描述](https://img-blog.csdn.net/20171029165247568?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
得到仿真图如下。 
![这里写图片描述](https://img-blog.csdn.net/20171029165313388?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第三题 
M=20的计数器 
![这里写图片描述](https://img-blog.csdn.net/20171029165435398?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171029165453758?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
设计一个计数器，从0开始计数一直到9，再从9到0, 
0,1,2,3,4,5，…….9,8,7,6,…….0,1,2…… 
verilog代码如下 
![这里写图片描述](https://img-blog.csdn.net/20171029165519538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171029165543332?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171029165554279?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171029165614364?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
