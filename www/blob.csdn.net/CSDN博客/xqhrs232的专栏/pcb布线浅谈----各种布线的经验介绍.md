# pcb布线浅谈----各种布线的经验介绍 - xqhrs232的专栏 - CSDN博客
2016年04月12日 23:10:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：604
原文地址::[http://wenku.baidu.com/link?url=kieC_1-w6On3HaNjuUHycrcbgnmQszCE_JuijSxr_etaEhVMG-_qQ_Jsi5-9FGJ5PE3wKejWbs5sgoaCrj0_hsP9EXL1aTLjRu5udgUM19m](http://wenku.baidu.com/link?url=kieC_1-w6On3HaNjuUHycrcbgnmQszCE_JuijSxr_etaEhVMG-_qQ_Jsi5-9FGJ5PE3wKejWbs5sgoaCrj0_hsP9EXL1aTLjRu5udgUM19m)
相关文章
1、**PCB布线及电源线粗细的讲究----[http://bbs.21ic.com/icview-147415-1-1.html](http://bbs.21ic.com/icview-147415-1-1.html)**
2、
# PCB板电流和布线宽度的关系----[http://wenku.baidu.com/link?url=TMfeoENqBloutNjWHKlYeoegl25aYkve4bSHrJTHGPaloGEhXneFHQ2pSn2Y3PXlq-DDKZk_j2zlaVd2DrIMj2lW7hVVt8n5V2so6Fc07s3](http://wenku.baidu.com/link?url=TMfeoENqBloutNjWHKlYeoegl25aYkve4bSHrJTHGPaloGEhXneFHQ2pSn2Y3PXlq-DDKZk_j2zlaVd2DrIMj2lW7hVVt8n5V2so6Fc07s3)
3、[PCB板绘制之我见，已经画了很长时间了](http://blog.csdn.net/memoryjs/article/details/7062917)----[http://blog.csdn.net/memoryjs/article/details/7062917](http://blog.csdn.net/memoryjs/article/details/7062917)
4、[自己总结的电子元器件的作用---光耦，电感，磁珠等等](http://blog.csdn.net/memoryjs/article/details/7055983)----[http://blog.csdn.net/memoryjs/article/details/7055983](http://blog.csdn.net/memoryjs/article/details/7055983)
5、pcb布线经验精华----[http://wenku.baidu.com/link?url=VmEpB9pPI6mYgIaJSp_EpV80JrfXmO_WUBZJyTwMnM-yq1oQfJWi1leUwLgrfIWHaIhJa8JO1F958tEjwqiu-KAcbbdbeTPiMEBnobSiLwC](http://wenku.baidu.com/link?url=VmEpB9pPI6mYgIaJSp_EpV80JrfXmO_WUBZJyTwMnM-yq1oQfJWi1leUwLgrfIWHaIhJa8JO1F958tEjwqiu-KAcbbdbeTPiMEBnobSiLwC)

//=================================================================================================================================
备注::
1>在线径粗细发生变化的时候，会发生一些反射的现象，所以最好的设计就是能过保证线径的粗细不要发生显著的变化，通常线的粗细变化发生在拐角处，直角是最差的，45度角好一些，圆角是最好的。
2>PCB板铜箔载流量【转】
2008-10-24 10:58、
铜箔厚度→       70um         50um         35um
铜箔宽度↓
2.50mm         6.00A         5.10A         4.50A
2.00mm         5.10A         4.30A         4.00A
1.50mm         4.20A         3.50A         3.20A
1.20mm         3.60A         3.00A         2.70A
1.00mm         3.20A         2.60A         2.30A
0.80mm         2.80A         2.40A         2.00A
0.60mm         2.30A         1.90A         1.60A
0.50mm         2.00A         1.70A         1.35A
0.40mm         1.70A         1.35A         1.10A
0.30mm         1.30A         1.10A         0.80A
0.20mm         0.90A         0.70A         0.55A
0.15mm         0.70A         0.50A         0.20A 
