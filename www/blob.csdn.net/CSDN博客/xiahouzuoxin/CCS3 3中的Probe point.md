# CCS3.3中的Probe point - xiahouzuoxin - CSDN博客





2013年11月10日 15:01:01[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2294







Probe point在CCS2.2中单独的菜单，在CCS3.3中和断点操作合并在一起了。以DSK6713\tutorial\dsk6713\sinewave为例，列举操作方法如下：

- 打开sine.c，找到dataIO()语句，F9设断点
- Debug菜单——>Breakpoints，打开断点管理窗口
- 选中dataIO()所在断点行，点右键菜单，选择Property window...，打开断点属性页
- 在Action列表框中，选择Read data from file，然后在下边的条目中选择文件名，如sine.dat
- Wrap Around项目打勾，显示为True，让文件读到结尾时反绕到文件开头
- Start Address填写文件数据填充到内存的位置，可以填写currentBuffer.input，也可以填写数组的地址0x8000....，都是一码事
- Length填写每次从文件中读出的数据长度，填写0x64，或者十进制数100
- 点击该属性页上端的Sumbit change按钮，保存设置
- 此时，会出现一个类似播放器的窗口，可以显示和控制从文件读取数据的进度。


- 至此，仅仅完成了数据输入工作。单步运行程序，可以看到输入缓冲区数据和输出缓冲区的变化，证实是从数据文件中读取的数据




