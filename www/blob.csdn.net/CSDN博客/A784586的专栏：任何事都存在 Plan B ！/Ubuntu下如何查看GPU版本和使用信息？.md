# Ubuntu下如何查看GPU版本和使用信息？ - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年12月01日 17:23:18[QuJack](https://me.csdn.net/A784586)阅读数：20892










**nvidia-smi是用来查看GPU版本信息，GPU使用信息查询：**

**nvidia-smi  **

**![](https://img-blog.csdn.net/20171201172520060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**

第一栏的Fan：N/A是风扇转速，从0到100%之间变动，这个速度是计算机期望的风扇转速，实际情况下如果风扇堵转，可能打不到显示的转速。有的设备不会返回转速，因为它不依赖风扇冷却而是通过其他外设保持低温（比如我们实验室的服务器是常年放在空调房间里的）。 


第二栏的Temp：是温度，单位摄氏度。 

第三栏的Perf：是性能状态，从P0到P12，P0表示最大性能，P12表示状态最小性能。 

第四栏下方的Pwr：是能耗，上方的Persistence-M：是持续模式的状态，持续模式虽然耗能大，但是在新的GPU应用启动时，花费的时间更少，这里显示的是off的状态。 

第五栏的Bus-Id是涉及GPU总线的东西，domain:bus:device.function 

第六栏的Disp.A是Display Active，表示GPU的显示是否初始化。 

第五第六栏下方的Memory Usage是显存使用率。 

第七栏是浮动的GPU利用率。 

第八栏上方是关于ECC的东西。 

第八栏下方Compute M是计算模式。 

下面一张表示每个进程占用的显存使用率。

显存占用和GPU占用是两个不一样的东西，显卡是由GPU和显存等组成的，显存和GPU的关系有点类似于内存和CPU的关系。我跑caffe代码的时候显存占得少，GPU占得多，师弟跑TensorFlow代码的时候，显存占得多，GPU占得少。



