# QTP测试结果中无法捕获截图（Screen Recorder） - weixin_33985507的博客 - CSDN博客
2011年06月18日 10:02:06[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
大家在日常使用QuickTest时，经常会遇到无法测试结果捕获截图（Screen Recorder），经常会遇到如下图所示的情况。
![](http://blog.51cto.com/attachment/201106/100030814.jpg)
遇到该情况如何解决呢，其实很简单，只需要修改一下Options即可。安装QuickTest后，默认如下图所示。在【Run】-【Screen Capture】下默认“Save movie to results”是未被选中的，正是由于未被选中的原因，QTP测试结果中才不能录制上截图。
![](http://blog.51cto.com/attachment/201106/100103528.jpg)
当将“Save movie to results”修改后，其下有多个选项，这里我设定为Always，即所有回放过程中的截图全部记录，然后再次回放脚本，可以看到测试结果中，记录了详细的步骤。
![](http://blog.51cto.com/attachment/201106/100146706.jpg)
