# 人脸识别评测的标准ROC曲线及其他标准 - wydbyxr的博客 - CSDN博客
2018年11月15日 09:53:22[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：212

ROC曲线
![这里写图片描述](https://img-blog.csdn.net/20170113134618971?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWFvMTE4MTQ5MTYzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170113134633659?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWFvMTE4MTQ5MTYzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170113134645841?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWFvMTE4MTQ5MTYzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170113134704060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWFvMTE4MTQ5MTYzMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
１、误识率(False Accept Rate, FAR)：这是将其他人误作指定人员的概率 
２、拒识率(False Reject Rate, FRR)：这是将指定人员误作其它人员的概率。
　　计算机在判别时采用的阈值不同，这两个指标也不同。一般情况下，误识率FAR 随阈值的减小（放宽条件）而增大，拒识率FRR 随阈值的减小而减小。因此，可以采用等错误率（Equal Error Rate, ERR）作为性能指标，即通过调节阈值，使这FAR和FRR两个指标相等时的FAR 或 FRR。
