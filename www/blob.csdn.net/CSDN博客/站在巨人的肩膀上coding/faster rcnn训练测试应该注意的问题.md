# faster  rcnn训练测试应该注意的问题 - 站在巨人的肩膀上coding - CSDN博客





2018年01月29日 15:09:00[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：240








训练自己的数据集的时候，注意：

1.修改train.ptototxt

//TODO具体修改内容不表


2.修改test.prototxt

//TODO具体修改内容不表


3.修改代码中pascal_voc中的class

//具体修改内容根据自己的分类类别来更改


4.修改demo-my.py中的class，class顺序与pascal_voc.py中的类别标签的顺序要一致

//TODO注意类别顺序要一致，换成自己的数据测试




下面是最终要的地方！！！画重点！！！


5.测试注意事项

![](https://img-blog.csdn.net/20180129171156861?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTE9WRTEwNTUyNTk0MTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


demo.py实际调用的是faster_rcnn_test.pt来测试的网络，所以这个地方也要相应的修改为对应的类别！！！




